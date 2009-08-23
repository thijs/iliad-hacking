/*
 * This file is part of pcshareMgr.
 *
 * pcshareMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pcshareMgr is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <mntent.h>
#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>

#include <config.h>
#include <gtk/gtk.h>
#include <liberdm/display.h>

#include <liberregxml/erregapi.h>

#include "control.h"
#include "logging.h"
#include "displayUpdate.h"
#include "systemcalls.h"
#include "view.h"
#include "sharethread.h"
#include "languages.h"
#include "main.h"
#include "settings.h"

#define OUTBOX_NAME            "outbox"
#define INBOX_NAME             "inbox"
#define INBOX_NAME_TEMP        "inbox.tmp"
#define SHARE_MOUNT_TYPE       "cifs"

#define ILIAD_DOCUMENTS        "documents"
#define ILIAD_NEWSPAPERS       "newspapers"
#define ILIAD_BOOKS            "books"
#define ILIAD_NOTES            "notes"

#define PC_STORAGE_INTERNAL    "Internal"
#define PC_STORAGE_CF          "CF"
#define PC_STORAGE_USB         "USB"
#define PC_STORAGE_MMC         "MMC"
#define ILIAD_STORAGE_INTERNAL "/mnt/free"
#define ILIAD_STORAGE_CF       "/media/cf"
#define ILIAD_STORAGE_USB      "/mnt/usb"
#define ILIAD_STORAGE_MMC      "/media/card"

#define MANIFEST_FILENAME      "manifest.xml"

static shareMgr_t *theStaticShareMgr = NULL;
static char szShareRootDir[MAX_PATH];
static char szShareOutbox[MAX_PATH];
static char szShareInbox[MAX_PATH];
static char szShareInboxTmp[MAX_PATH];

static gboolean g_error_confirmed = FALSE;
static int g_notRemovedFiles = 0;
static int g_filesDownloaded = 0;
static int g_filesUploaded = 0;
static int g_transferError = 0;
static pcShareCfg_t* thePCShareSettings = NULL;

static void report_error(const gchar* msg);
static void DisplayMessage(shareMgr_t * theShareMgr, gchar * szMessageProblem, gchar * szMessageSolution);
static int shareDoUpload(void);
static int shareDoDownload(void);
static int recursiveFilesDownload(const char* sPath, const char* sResultDir);
static gboolean isPartitionMounted(const char *partition, const char *type);
void shareThreadErrorConfirmed(void);
static gboolean g_progress_callback(gpointer data);

#define ARRAY_LEN(x) (sizeof(x) / sizeof ((x) [0]))
static const char * outbox_locations[] = 
{
    "/mnt/free/outbox",
    "/mnt/usb/outbox",
    "/mnt/cf/outbox",
    "/mnt/card/outbox",
};

static volatile GThread* g_share_thread = NULL;
static volatile gboolean g_share_abort  = FALSE;

static gpointer shareThread(gpointer arg)
{
    shareMgr_t *theShareMgr = (shareMgr_t *) arg;
    regContentCategory_t *regCc = NULL;
    int rc;
    char msg[512];

    theStaticShareMgr = theShareMgr;

    DL_LOGPRINTF("enter");
    DisplayMessage(theShareMgr, _("Connecting to PC share..."), "");
    
    regCc = erRegGetContentCategory(DOWNLOAD_HISTORY);
    if (regCc == NULL)
    {
        DL_ERRORPRINTF("Could not access info for download history location");
        // Display a sensible message
        report_error( _("Cannot find download history location") );
        theShareMgr->returnVal = DOWNLOAD_HISTORY_LOCATION_ERROR;
        goto EXIT_THREAD;
    }

    if (regCc->location)
    {
        DL_LOGPRINTF("Download History Location: %s", regCc->location);
        setDownloadHistory(regCc->location);
    }
    else
    {
        DL_ERRORPRINTF("Failed to retrieve download history location");
        report_error( _("Cannot find download history location") );
        theShareMgr->returnVal = DOWNLOAD_HISTORY_LOCATION_ERROR;
        goto EXIT_THREAD;
    }

    // The location of the CIFS mount
    sprintf(szShareRootDir, SHARE_MOUNT_POINT);

    if (isPartitionMounted(szShareRootDir, SHARE_MOUNT_TYPE) == FALSE)
    {
        DL_ERRORPRINTF("Cannot find the shared folder on the PC");
        report_error( _("Cannot find the shared folder on the PC") );
        theShareMgr->returnVal = SHARE_NOT_MOUNTED;
        goto EXIT_THREAD;
    }

    thePCShareSettings = get_pc_share_settings();

    DL_LOGPRINTF("thePCShareSettings->overwrite = %d", thePCShareSettings->overwrite);
    
    // Outbox / Inbox locations on the server
    sprintf(szShareOutbox, "%s/%s", szShareRootDir, OUTBOX_NAME);
    sprintf(szShareInbox, "%s/%s", szShareRootDir, INBOX_NAME);
    sprintf(szShareInboxTmp, "%s/%s", szShareRootDir, INBOX_NAME_TEMP);

    // Do upload of all files in outboxes
    rc = shareDoUpload();
    DL_MSGPRINTF("Upload finished, uploaded %d items", rc);
    
    scFlushFileBuffers();

    // Do recursive download of all files in PC outbox to iLiad
    shareDoDownload();
    DL_MSGPRINTF("Download finished, downloaded %d items", g_filesDownloaded);
    
    scFlushFileBuffers();
    
    if (g_share_abort)
    {
        // If aborted, immediately exit
        theShareMgr->returnVal = SHARE_THREAD_ABORTED;
        goto EXIT_THREAD;
    }

    if (g_filesDownloaded || g_filesUploaded)
    {
        DisplayMessage(theShareMgr, _("--- Transfer Complete ---"), "");
    }
    else
    {
        DisplayMessage(theShareMgr, _("--- Nothing to Transfer ---"), "");
    }
    
    if (g_filesDownloaded)
    {
        main_report_new_content();
    }

    if (g_transferError != 0)
    {
        snprintf(msg, sizeof(msg),
                _("There were errors during the transfer of %d items. "
                  "Please make sure the destination location "
                  "on your iLiad has enough free space "
                  "and the PC share has both read and write permissions."),
                  g_transferError);
        report_error(msg);
    }
    else
    {
        sleep(5);
    }
    
  EXIT_THREAD:
    // Tell GTK thread we are done ...
    g_share_thread = NULL;
    gdk_threads_enter();
    {
        if (gtk_main_level() == 0)
        {
            DL_WARNPRINTF("    -- oops, gtk_main has quit!");
        }
        else
        {
            g_signal_emit_by_name(theShareMgr->topLevelWindow, "destroy");
        }
    }
    gdk_threads_leave();
    // ... and quit
    DL_LOGPRINTF("leave");
    return NULL;
}

gboolean shareThread_start(shareMgr_t* share_parms)
{
    GThread* thread = NULL;
    GError*  error = NULL;

    DL_LOGPRINTF("entry");
    if (g_share_thread)
    {
        DL_ERRORPRINTF("share thread already running");
        return FALSE;
    }

    // create the share thread
    g_share_abort = FALSE;
    thread = g_thread_create(shareThread, share_parms, FALSE, &error);
    if (error != NULL)
    {
        DL_ERRORPRINTF("Failed to create share thread - %s [%d]", error->message, error->code);
        g_free(error);
        error = NULL;
    }
    else
    {
        g_share_thread = thread;
    }

    return TRUE;
}

gboolean shareThread_stop()
{
    DL_LOGPRINTF("entry");

    if (g_share_thread)
    {
        // stop the share thread
        g_share_abort = TRUE;

        // stop any process spawned by shareThread
        pid_t pid = get_forkexec_child_pid();
        if (pid > 0)
        {
            kill(pid, SIGTERM);
        }

        // stop fileCopy that might be running on shareThread
        fileCopy_abort();

        return TRUE;
    }
    else
    {
        DL_ERRORPRINTF("share thread not running");
        return FALSE;
    }
}

gboolean shareThread_wait(int timeout_sec)
{
    int max_ticks = 2 * timeout_sec;

    DL_LOGPRINTF("entry");

    if (g_share_thread)
    {
        // sorry don't like busy-wait, but g_thread_join has no timeout option
        while (g_share_thread  &&  --max_ticks >= 0)
        {
            g_usleep(500*1000L);
        }
        return TRUE;
    }
    else
    {
        DL_ERRORPRINTF("share thread not running");
        return FALSE;
    }
}

// Make sure the partition is mounted, optionally check for type of mount
static gboolean isPartitionMounted(const char *partition, const char *type)
{
    gboolean found = FALSE;
    FILE *fp = setmntent("/proc/mounts", "r");
    struct mntent* mt = NULL;
    while ((mt = getmntent(fp)))
    {
//        DL_LOGPRINTF("Partition: %s: %s %s", mt->mnt_fsname, mt->mnt_dir, mt->mnt_type);
        if (strcasecmp(mt->mnt_dir, partition) == 0)
        {
            DL_LOGPRINTF("Found mounted partition: %s [%s]", mt->mnt_fsname, mt->mnt_type);
            if ( (type == NULL) || (strcasecmp(mt->mnt_type, type) == 0) )
            {
                found = TRUE;
            }
            else
            {
                found = FALSE;
            }
        }
    }
    endmntent(fp);

    return found;
}

static int shareDoUpload(void)
{
    int i, ret;
    DIR *dir;
    struct dirent *entry;
    struct stat d;
    char *filename;
    int copied = 0;
    char msg[512];
    char szDestination[MAX_PATH];
    char szTempDestination[MAX_PATH];
    
    // Check abort status, exit if aborted
    if (g_share_abort)
    {
        return 0;
    }

    filename = (char *) malloc(sizeof(char) * MAX_PATH);
    
    // Check if Inbox exists, otherwise create it
    if (!fileExists(szShareInbox))
    {
        DL_LOGPRINTF("Create PC Inbox");
        if (mkdir(szShareInbox, 0755) != 0)
        {
            DL_ERRORPRINTF("Unable to create PC inbox: [%d] %s", errno, strerror(errno));
            snprintf(msg, sizeof(msg), _("Unable to upload files to PC: cannot create inbox folder.\nFile upload has been cancelled."));
            report_error(msg);
            // Stop uploading files
            return 0;
        }
    }

    delDir(szShareInboxTmp);
    DL_LOGPRINTF("Create PC Inbox Temp");
    if (mkdir(szShareInboxTmp, 0755) != 0)
    {
        DL_ERRORPRINTF("Unable to create PC inbox tmp: [%d] %s", errno, strerror(errno));
        snprintf(msg, sizeof(msg), _("Unable to upload files to PC: cannot create temporary inbox folder.\nFile upload has been cancelled."));
        report_error(msg);
        // Stop uploading files
        return 0;
    }

    // Traverse all outboxes
    for(i = 0; i < ARRAY_LEN(outbox_locations) && !g_share_abort; i++)
    {
        // Check if the outbox exists
        if ((dir = opendir(outbox_locations[i])) != NULL)
        {
            // Yes: this is a dir, process it
            DL_LOGPRINTF("Processing Outbox: %s", outbox_locations[i]);

            // Read all files in dir
            while ((entry = readdir(dir)) != NULL  &&  !g_share_abort)
            {
                // Ignore '.' and '..'
                if (!((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)))
                {
                    filename[0] = '\0';
                    strncat(filename, outbox_locations[i], MAX_PATH);
                    strncat(filename, "/", MAX_PATH);
                    strncat(filename, entry->d_name, MAX_PATH);

                    // Check if this entry is a dir or a normal file
                    if ((stat(filename, &d) == 0) && (S_ISDIR(d.st_mode) || S_ISREG(d.st_mode)))
                    {
                        // Copy the file to the Inbox of the PC
                        // TODO: Take title from manifest.xml 
                        DL_LOGPRINTF("Copy command: cp -r %s %s", filename, szShareInboxTmp);
                        DisplayMessage(theStaticShareMgr, _("Uploading to PC:"), entry->d_name);

                        // Put ShareInbox plus name into a variable, so we can check if the destination path
                        // exists. We need to call recursive_dircopy with the full destination path, because
                        // it also deletes the destination path on failure
                        // We create both the temporary destination and the ultimate destination path here, to
                        // make sure both do not already exist.
                        snprintf(szTempDestination, sizeof(szTempDestination), "%s/%s", szShareInboxTmp, entry->d_name);
                        snprintf(szDestination, sizeof(szDestination), "%s/%s", szShareInbox, entry->d_name);

                        DL_ERRORPRINTF("szTempDestination = %s", szTempDestination);
                        if (fileExists(szTempDestination))
                        {
                            // It might not be a dir, but delDir uses 'rm -rf' so it should be fine for a file as well
                            DL_LOGPRINTF("Remove %s because it exists", szTempDestination);
                            delDir(szTempDestination);
                        }
                        
                        DL_ERRORPRINTF("szDestination = %s", szDestination);
                        if (fileExists(szDestination))
                        {
                            // It might not be a dir, but delDir uses 'rm -rf' so it should be fine for a file as well
                            DL_LOGPRINTF("Remove %s because it exists", szDestination);
                            delDir(szDestination);
                        }

                        DL_ERRORPRINTF("recursive_dircopy(%s, %s)", filename, szTempDestination);
                        ret = recursive_dircopy(filename, szTempDestination, g_progress_callback);
                        if (ret != 0)
                        {
                            if (copied == 0)
                            {
                                // First upload failed, probably all others as well.
                                DL_ERRORPRINTF("Error while uploading %s to PC: %s (%d) [STOPPING UPLOAD]", entry->d_name, strerror(ret), ret);
                                if ( !g_share_abort )
                                {
                                    snprintf( msg, sizeof(msg),
                                              _("Error while uploading %s to PC. File upload has been cancelled.\nError: %s (%d)"),
                                              entry->d_name, strerror(ret), ret );
                                    report_error(msg);
                                }
                                return 0;  // Abort upload
                            }
                            else
                            {
                                DL_ERRORPRINTF("Error while uploading %s to PC: %s (%d)", entry->d_name, strerror(ret), ret);
                                g_transferError++;
                            }
                        }
                        else
                        {
                            // Transfer is OK. We need to move the file from szShareInboxTmp to szShareInbox
                            DL_LOGPRINTF("Moving %s to %s...", szTempDestination, szDestination);
                            if (rename(szTempDestination, szDestination) != 0)
                            {
                                // Rename failed. Delete file from szShareInboxTmp, don't delete it on the iLiad
                                DL_ERRORPRINTF("Error: unable to move file to %s, deleting %s", szDestination, szTempDestination);
                                delDir(szTempDestination);
                                g_transferError++;
                            }
                            else
                            {
                                // Delete the uploaded file from the iLiad Outbox
                                if (delDir(filename) == -1)
                                {
                                    g_notRemovedFiles++;
                                    DL_ERRORPRINTF("Unable to delete file '%s' on iLiad: %s", filename, strerror(errno));
                                }
                                copied++;
                            }
                        }
                    }
                    else
                    {
                        // Strange file type, skip it
                        DL_WARNPRINTF("Skipping %s of type %d", filename, d.st_mode);
                    }
                }
            }
        }
        else
        {
            DL_LOGPRINTF("No outbox found at %s", outbox_locations[i]);
        }
    }

    g_filesUploaded = copied;

    // Clear and delete szShareInboxTmp
    delDir(szShareInboxTmp);
    
    free(filename);

    return copied;
}

static int shareDoDownload(void)
{
    DIR *dir;
    struct dirent *entry;
    char szComputerLocation[MAX_PATH];
    char *sziLiadLocation = NULL;

    // Check abort status, exit if aborted
    if (g_share_abort)
    {
        return 0;
    }

    g_filesDownloaded = 0;
    
    // Open PC outbox
    if ((dir = opendir(szShareOutbox)) != NULL)
    {
        // Read all files in dir
        while ((entry = readdir(dir)) != NULL  &&  !g_share_abort)
        {
            if (strcasecmp(entry->d_name, PC_STORAGE_INTERNAL) == 0)
            {
                sziLiadLocation = strdup(ILIAD_STORAGE_INTERNAL);
            }
            else if (strcasecmp(entry->d_name, PC_STORAGE_CF) == 0)
            {
                sziLiadLocation = strdup(ILIAD_STORAGE_CF);
            }
            else if (strcasecmp(entry->d_name, PC_STORAGE_USB) == 0)
            {
                sziLiadLocation = strdup(ILIAD_STORAGE_USB);
            }
            else if (strcasecmp(entry->d_name, PC_STORAGE_MMC) == 0)
            {
                sziLiadLocation = strdup(ILIAD_STORAGE_MMC);
            }
            else
            {
                // Ignore
                sziLiadLocation = NULL;
            }
            
            if ( sziLiadLocation )
            {
                // First; we need to see if sziLiadLocation is a mounted filesystem
                // otherwise we skip it
                if (isPartitionMounted(sziLiadLocation, NULL))
                {
                    // Recursively copy all manifest directories inside sziLiadLocation
                    // All copied manifest dirs need an entry in the download history
                    snprintf(szComputerLocation, MAX_PATH, "%s/%s", szShareOutbox, entry->d_name);
                    recursiveFilesDownload(szComputerLocation, sziLiadLocation);
                }
                else
                {
                    DL_WARNPRINTF("Skipping [%s] because it is not mounted", sziLiadLocation);
                }
            }
        }
    }
    else
    {
        DL_WARNPRINTF("No PC outbox found, nothing to download. Creating the structure on the PC.");
        // Create the structure on the PC
        if (mkdir(szShareOutbox, 0755) == 0)
        {
            // Make all appropriate dirs
            snprintf(szComputerLocation, MAX_PATH, "%s/%s",    szShareOutbox, PC_STORAGE_INTERNAL);
            mkdir(szComputerLocation, 0755);
            snprintf(szComputerLocation, MAX_PATH, "%s/%s/%s", szShareOutbox, PC_STORAGE_INTERNAL, ILIAD_DOCUMENTS);
            mkdir(szComputerLocation, 0755);
            snprintf(szComputerLocation, MAX_PATH, "%s/%s/%s", szShareOutbox, PC_STORAGE_INTERNAL, ILIAD_NEWSPAPERS);
            mkdir(szComputerLocation, 0755);
            snprintf(szComputerLocation, MAX_PATH, "%s/%s/%s", szShareOutbox, PC_STORAGE_INTERNAL, ILIAD_BOOKS);
            mkdir(szComputerLocation, 0755);
            snprintf(szComputerLocation, MAX_PATH, "%s/%s/%s", szShareOutbox, PC_STORAGE_INTERNAL, ILIAD_NOTES);
            mkdir(szComputerLocation, 0755);
            snprintf(szComputerLocation, MAX_PATH, "%s/%s",    szShareOutbox, PC_STORAGE_MMC);
            mkdir(szComputerLocation, 0755);
            snprintf(szComputerLocation, MAX_PATH, "%s/%s",    szShareOutbox, PC_STORAGE_CF);
            mkdir(szComputerLocation, 0755);
            snprintf(szComputerLocation, MAX_PATH, "%s/%s",    szShareOutbox, PC_STORAGE_USB);
            mkdir(szComputerLocation, 0755);
        }
    }
    
    if (sziLiadLocation != NULL)
    {
        free(sziLiadLocation);
    }
}

static int recursiveFilesDownload(const char* sPath, const char* sResultDir)
{
    DL_LOGPRINTF("entry: path [%s] resultDir [%s]", sPath, sResultDir);

    DIR* dirp = opendir(sPath);
    if (dirp == NULL)
    {
        DL_ERRORPRINTF("Cannot open dir [%s] - %s", sPath, strerror(errno));
        return -1;  // error
    }

    char*           cp;
    struct dirent*  direntp;
    struct stat     statBuf;
    char            buf[MAX_FILENAME_SIZE];
    char            resolvedPath[PATH_MAX];
    char            fileName[MAX_FILENAME_SIZE];
    char            fname[MAX_FILENAME_SIZE];
    char            newResultDir[PATH_MAX];
    char*           newFile;


    // TODO: we need to make sure recursion ends. The realpath() call seems to ensure we cannot
    // get into an endless loop, but we might want to limit the amount of subdirs to something
    // more workable than the maximum directory depth of a FAT32 filesystem.

    while ( (direntp = readdir(dirp)) != NULL  &&  !g_share_abort)
    {
        // skip hidden entries and current/parent directory
        // do copy files starting with a '_'
        if ( direntp->d_name[0] == '.' )
        {
            continue;
        }

        // convert symlink to the file or directory behind it
        snprintf(buf, sizeof(buf), "%s/%s", sPath, direntp->d_name);
        cp = realpath(buf, resolvedPath);
        if (cp == NULL)
        {
            DL_ERRORPRINTF("Cannot resolve path [%s] - %s", buf, strerror(errno));
            continue;  // while
        }
        DL_LOGPRINTF("resolved path [%s]", resolvedPath);

        // set a pointer to the filename
        cp = strrchr(resolvedPath, '/');
        g_assert(cp != NULL);
        snprintf(fileName, sizeof(fileName), "%s", cp + 1);
        DL_LOGPRINTF("filename [%s]", fileName);

        // determine what kind of directory entry this really is
        if (stat(resolvedPath, &statBuf) == 0)
        {
            // Handle file that is not in manifest dir
            if ( S_ISREG(statBuf.st_mode) )
            {
                // Copy the file, if destination exists, rewrite filename
                DisplayMessage(theStaticShareMgr, _("Downloading from PC:"), fileName);

                if (thePCShareSettings->overwrite)
                {
                    // Make sure file with same name does not exist in the result dir
                    char buf[MAX_PATH];
                    snprintf( buf, sizeof(buf), "%s/%s", sResultDir, fileName);
                    delDir(buf);
                }

                if ((newFile = fileCopy(resolvedPath, sResultDir, g_progress_callback)) != NULL)
                {
                    createDownloadHistoryEntry(newFile);
                    free(newFile);
                    DL_LOGPRINTF("unlink('%s');", resolvedPath);
                    if (unlink(resolvedPath) == -1)
                    {
                        g_notRemovedFiles++;
                        DL_ERRORPRINTF("Unable to delete file '%s' on PC: %s", resolvedPath, strerror(errno));
                    }
                    g_filesDownloaded++;
                }
                else
                {
                    DL_ERRORPRINTF("Unable to copy [%s]", fileName);
                    g_transferError++;
                }
            }
            else if ( S_ISDIR(statBuf.st_mode) )
            {
                // TODO: parse manifest file to see if it is really a container or just a manifest dir
                // Check whether directory holds a manifest file
                snprintf(buf, sizeof(buf), "%s/%s", resolvedPath, MANIFEST_FILENAME);
                if ( fileExists(buf) )
                {
                    // We have a manifest file, recursively copy this dir
                    DL_LOGPRINTF("Found a manifest file, copy container: [cp -r '%s' %s]", resolvedPath, sResultDir);
                    DisplayMessage(theStaticShareMgr, _("Downloading from PC:"), fileName);

                    if (thePCShareSettings->overwrite)
                    {
                        // Make sure file with same name does not exist in the result dir
                        char buf[MAX_PATH];
                        snprintf( buf, sizeof(buf), "%s/%s", sResultDir, fileName);
                        delDir(buf);
                    }
 
                    if ((newFile = dirCopy(resolvedPath, sResultDir, g_progress_callback)))
                    {
                        createDownloadHistoryEntry(newFile);
                        free(newFile);
                        DL_LOGPRINTF("delDir('%s');", resolvedPath);
                        if (delDir(resolvedPath) == -1)
                        {
                            g_notRemovedFiles++;
                            DL_ERRORPRINTF("Unable to delete file '%s' on PC: %s", resolvedPath, strerror(errno));
                        }
                        g_filesDownloaded++;
                    }
                    else
                    {
                        DL_ERRORPRINTF("Unable to copy manifest dir %s to %s", resolvedPath, sResultDir);
                        g_transferError++;
                    }
                }
                else
                {
                    // A plain dir:
                    // - Check if the strtolower() version exists, if so use that one
                    // - Otherwise use the original name (with upper and lowercase)
                    // - Create this folder in sResultDir (only if not exists)
                    // - Recursively call myself to copy its contents
                    cp = strrchr(buf, '/');
                    g_assert(cp != NULL);
                    *cp = '\0';
                    strcpy(fname, fileName);
                    strtolower(fname);
                    snprintf(newResultDir, sizeof(newResultDir), "%s/%s", sResultDir, fname);
                    if (!fileExists(newResultDir))
                    {
                        // Make the dir as requested, not all in lowercase
                        snprintf(newResultDir, sizeof(newResultDir), "%s/%s", sResultDir, fileName);
                        DL_LOGPRINTF("mkdir(%s)", newResultDir);
                        mkdir(newResultDir, 0755);
                    }
                    else
                    {
                        // Use the existing dir, all in lowercase
                        DL_LOGPRINTF("Dir %s exists, using that one", newResultDir);
                    }
                    recursiveFilesDownload(buf, newResultDir);
                }
            }
        }
    }
    closedir(dirp);

    return 0;  // ok
}

static void report_error(const gchar* msg)
{
    assert(g_error_confirmed == FALSE);

    if ( !main_get_background() )
    {
        // trigger error-confirmation and show error message
        gdk_threads_enter();
        view_callback_on_error_confirmed(shareThreadErrorConfirmed);
        viewShowError(msg);
        gdk_threads_leave();

        // wait for confirmation,
        // polling wait allowed because shareThread has nothing else to do now
        while (g_error_confirmed == FALSE)
        {
            g_usleep(500*1000L);
        }
        g_error_confirmed = FALSE;
        
        // disable callback, hide error message
        gdk_threads_enter();
        view_callback_on_error_confirmed(NULL);
        viewHideError();
        gdk_threads_leave();
    }

    DL_ERRORPRINTF("%s", msg);
}

void shareThreadErrorConfirmed(void)
{
    g_error_confirmed = TRUE;
}

void DisplayMessage(shareMgr_t * theShareMgr, gchar * szMessageProblem, gchar * szMessageSolution)
{
    gchar   szMessage[VIEW_MAX_INF0_STRING_SIZE];

    g_snprintf(szMessage, VIEW_MAX_INF0_STRING_SIZE, " %s %s", szMessageProblem, szMessageSolution);
    
    if (!main_get_background())
    {
        gdk_threads_enter();
        viewShowInfo(szMessage);
        viewHideProgress();
        gdk_threads_leave();
    }
    else
    {
        DL_MSGPRINTF("%s", szMessage);
    }
}

static gboolean g_progress_callback(gpointer data)
{
    if (!main_get_background())
    {
        gdk_threads_enter();
        viewShowProgress();
        gdk_threads_leave();
        
        return TRUE;
    }
    else
    {
        // No need for progress indication when in background
        return FALSE;
    }
}
