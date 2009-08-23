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

 /**
 * \file systemcalls.c
 * \brief control - application interface with miscelaneous shell scripts and programs
 * 
 * <File description>
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/statfs.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <time.h>

#include "logging.h"
#include "systemcalls.h"


static char*    szDownloadHistoryDir = NULL;
static pid_t    g_forkexec_child_pid = 0;
static gboolean g_fileCopy_abort = FALSE;


static gboolean isSymLink(const char *szPath);


void setDownloadHistory(char *dir)
{
    szDownloadHistoryDir = strdup(dir);
}


// check whether the specified path is a symbolic link
static gboolean isSymLink(const char *szPath)
{
    struct stat statBuf;

    if (   lstat(szPath, &statBuf) == 0
        && S_ISLNK(statBuf.st_mode)    )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void scFlushFileBuffers()
{
    sync();
    sync();
    sync();
}

// get current battery charge
int get_battery_charge(void)
{
    int pwr_fd;
    unsigned int battery_current;
    short current;

    int charge = 0;  // return value

    unsigned int charge_status = 0;

    pwr_fd = open(POWERMGMTIFACE, O_RDWR);
    if (pwr_fd < 0)
    {
        perror("Error opening battery device");
    }
    else
    {
        if (ioctl(pwr_fd, BATTERY_IOCTL_READ_CHARGE, &charge_status) == -1)  // percentage 0 ... 100
        {
            perror("ioctl read battery_charge failed");
        }
        else if (ioctl(pwr_fd, BATTERY_IOCTL_READ_CURRENT, &battery_current) == -1) // current in milliamps
        {
            perror("ioctl read battery_current failed");
        }
        else
        {
            current = battery_current & 0xFFFF;
            if (current > 0)
            {
                charge = 100;
            }
            else
            {
                charge = charge_status;
            }
        }

        close(pwr_fd);
    }
    
    DL_LOGPRINTF("return [%d]", charge);
    return charge;
}

int recursive_dircopy(const char * source, const char * dest, GSourceFunc progress_callback)
{
    int             ret;
    int             argc;
    char*           argv[MAX_ARGS + 1];
    int             rc;
    char*           cp;
    guint           timeout = 0;
    
    if (progress_callback)
    {
        // Show some indication of being busy
        timeout = g_timeout_add(PROGRESS_CALLBACK_TIMEOUT, progress_callback, NULL);
    }
    
    // command = cp -r <source-directory> <target-directory>
    argc = 0;
    argv[argc++] = "cp";
    argv[argc++] = "-r";
    //
    cp = alloca(strlen(source) + 1);
    strcpy(cp, source);
    argv[argc++] = cp;
    //
    cp = alloca(strlen(dest) + 1);
    strcpy(cp, dest);
    argv[argc++] = cp;
    //
    argv[argc] = NULL;
    g_assert(argc < sizeof(argv)/sizeof(argv[0]));
    rc = fork_exec(argc, argv);
    if (rc == 0)
    {
        sync();
        ret = 0;  // success
    }
    else
    {
        DL_ERRORPRINTF( "Cannot copy [%s] to [%s] - error [%d] [%s]",
                         source, dest, rc, strerror(rc) );
        ret = rc;

        // clean up incomplete destination
        delDir(dest);
    }
    
    if (progress_callback)
    {
        // stop showing busy
        g_source_remove(timeout);
    }
    
    return ret;
}
    
pid_t get_forkexec_child_pid()
{
    return g_forkexec_child_pid;
}

// Execute specified command with arguments in child process
int fork_exec(const int argc, char *const argv[])
{
    int rc = -1;
    int pid;
    int status;

    g_assert(argv[argc] == NULL);
    
    // spawn child process
    switch (pid = fork())
    {
        case 0:
            // child process: execute command
            rc = execvp(argv[0], argv);
            DL_ERRORPRINTF("execvp [%s] returns [%d] errno [%d] - %s", argv[0], rc, errno, strerror(errno));
            exit(1);

        case -1:
            // error: fork failed
            DL_ERRORPRINTF("fork returns [%d] errno [%d] - %s", pid, errno, strerror(errno));
            g_assert_not_reached();
            break;

        default:
            // parent process: wait for child and return its exit value
            g_forkexec_child_pid = pid;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                rc = WEXITSTATUS(status);
            }
            g_forkexec_child_pid = 0;
    }

    return rc;
}

int fileExists(const char* filename)
{
    struct stat buf;

    if (filename == NULL)
    {
        return 0;
    }

    return (stat(filename, &buf) == 0);
}

char *dirCopy(const char* dirPath, const char* destinationDir, GSourceFunc progress_callback)
{
    int i, ret;
    char *cp, *extension;
    char *dirName;
    char *buf;
    int bufsize;

    //
    // Check if the resulting dirName already exists
    //
    
    // Layout of dirPath:
    // [PATH NAME]/[FILENAME].[EXT]
    //   dirName--^          ^
    //       extension--------

    // First: extract the filename from the dirPath
    cp = strrchr(dirPath, '/');
    if (cp)
    {
        cp++;
    }
    else
    {
        cp = (char*)dirPath;
    }
    dirName = strdup(cp);

    // Create destination filename
    bufsize = strlen(dirName) + strlen(destinationDir) + 5;
    buf = malloc(sizeof(char) * bufsize);
    snprintf(buf, bufsize, "%s/%s", destinationDir, dirName);

    // Extract extension, modify dirName to strip extension
    cp = strrchr(dirName, '.');
    if (cp)
    {
        *cp = '\0';
        extension = cp + 1;
    }
    else
    {
        extension = NULL;
    }
    
    // Add a sequence number, in case of duplicate filenames
    // TODO: MvdW: Do we need to change the title in the manifest to reflect the sequence number?
    for (i = 1 ; fileExists(buf) && i <= 99 ; i++)
    {
        if (extension)
        {
            
            snprintf(buf, bufsize, "%s/%s_%02d.%s", destinationDir, dirName, i, extension);
        }
        else
        {
            snprintf(buf, bufsize, "%s/%s_%02d", destinationDir, dirName, i);
        }
    }
    if (--i)
    {
        DL_WARNPRINTF("Still need to rewrite the manifest title/description for this entry [%s] [%d]", buf, i);
    }
    
    DL_LOGPRINTF("Copy '%s' to '%s'", dirPath, buf);
    if ((ret = recursive_dircopy(dirPath, buf, progress_callback)) != 0)
    {
        DL_ERRORPRINTF("recursive_dircopy returned %d (%s)", ret, strerror(ret));
        goto DIRCOPY_ERROR;
    }
    
    free(dirName);
    return buf;
    
    // Clean up on error
    DIRCOPY_ERROR:
        perror("");
        free(dirName);
        free(buf);
        return NULL;
}

// Copy 'filePath' to 'destinationDir'
// Make sure destinationDir/fileName does not exist
// otherwise try adding a sequence number to fileName
// Return the full path of the copied file
char *fileCopy(const char* filePath, const char* destinationDir, GSourceFunc progress_callback)
{
    int i;
    char *cp, *extension;
    char *fileName;
    char *destinationFile;
    int bufsize, bytes_read;
    FILE *fp_from, *fp_to = NULL;
    char fbuf[1024];
    guint timeout = 0;

    g_fileCopy_abort = FALSE;

    if (progress_callback)
    {
        // Show some indication of being busy
        timeout = g_timeout_add(PROGRESS_CALLBACK_TIMEOUT, progress_callback, NULL);
    }

    //
    // Check if the resulting filename already exists
    //
    
    // Layout of filePath:
    // [PATH NAME]/[FILENAME].[EXT]
    //   fileName--^          ^
    //       extension--------

    // First: extract the filename from the filePath
    cp = strrchr(filePath, '/');
    if (cp)
    {
        cp++;
    }
    else
    {
        cp = (char*)filePath;
    }
    fileName = strdup(cp);

    // Create destination filename
    bufsize = strlen(fileName) + strlen(destinationDir) + 5;
    destinationFile = malloc(sizeof(char) * bufsize);
    snprintf(destinationFile, bufsize, "%s/%s", destinationDir, fileName);
    
    // Extract extension, modify fileName to strip extension
    cp = strrchr(fileName, '.');
    if (cp)
    {
        *cp = '\0';
        extension = cp + 1;
    }
    else
    {
        extension = NULL;
    }
    
    // Add a sequence number, in case of duplicate filenames
    for (i = 1 ; fileExists(destinationFile) && i <= 99 ; i++)
    {
        if (extension)
        {
            
            snprintf(destinationFile, bufsize, "%s/%s_%02d.%s", destinationDir, fileName, i, extension);
        }
        else
        {
            snprintf(destinationFile, bufsize, "%s/%s_%02d", destinationDir, fileName, i);
        }
    }
    
    DL_LOGPRINTF("Copy '%s' to '%s'", filePath, destinationFile);
    
    fp_from = fopen(filePath, "rb");
    if (!fp_from)
    {
        DL_ERRORPRINTF("Error opening file %s:", filePath);
        goto FILECOPY_ERROR;
    }
    
    fp_to = fopen(destinationFile, "wb+");
    if (!fp_to)
    {
        DL_ERRORPRINTF("Error opening new file %s:", destinationFile);
        goto FILECOPY_ERROR;
    }
    
    // copy source file to target file 1024 bytes at a time 
    while ((bytes_read = fread(fbuf, 1, sizeof(fbuf), fp_from))) 
    {
        if (fwrite(fbuf, 1, bytes_read, fp_to) == EOF  ||  g_fileCopy_abort) 
        {  
            DL_ERRORPRINTF("Error writing to target file %s:", destinationFile);
            goto FILECOPY_ERROR;
        }
    }
    
    if (!feof(fp_from))
    {
        DL_ERRORPRINTF("Error reading from source file %s:", filePath);
        goto FILECOPY_ERROR;
    }
    
    fclose(fp_from);
    fclose(fp_to);

    if (progress_callback)
    {
        // stop showing busy
        g_source_remove(timeout);
    }
    
    free(fileName);
    return destinationFile;

    // Clean up on error
FILECOPY_ERROR:
    perror("");
    fclose(fp_from);
    fclose(fp_to);
    if (progress_callback) g_source_remove(timeout);
    free(fileName);
    unlink(destinationFile);
    free(destinationFile);
    return NULL;
}

void fileCopy_abort()
{
    g_fileCopy_abort = TRUE;
}

int delDir(const char* sDir)
{
    char* cp;
    char* argv[10];
    int   argc;
    
    // Do nothing if called with empty string or a string of 1 char
    // This kind of makes sure no stupid person calls this with "/"
    if ((sDir == NULL) || (strlen(sDir) <= 1))
    {
        return -1;
    }
   
    argc = 0;
    argv[argc++] = "rm";
    argv[argc++] = "-rf";
    //
    cp = alloca( strlen(sDir) + 1 );
    g_assert(cp != NULL);
    strcpy(cp, sDir);
    argv[argc++] = cp;
    //
    argv[argc] = NULL;
    g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
    return fork_exec(argc, argv);
}

// add item to download history
void createDownloadHistoryEntry(const char *filePath)
{
    g_assert(filePath != NULL);
    DL_LOGPRINTF("Entry: filename [%s]", filePath);

    int      sequence;
    int      err;
    int      n;
    gboolean done;
    char     szLinkName[PATH_MAX]   = "";
    char     szLinkTarget[PATH_MAX] = "";

    DIR           *dirp;
    struct dirent *direntp;

    // remove existing link(s) to the target, if any
    if ((dirp = opendir(szDownloadHistoryDir)) == NULL)
    {
        DL_ERRORPRINTF("Could not open directory [%s] error [%s].", szDownloadHistoryDir, strerror(errno));
    }
    else
    {
        while ((direntp = readdir(dirp)) != NULL)
        {
            // directory entry read
            snprintf(szLinkName, sizeof(szLinkName), "%s/%s", szDownloadHistoryDir, direntp->d_name);
            if ( isSymLink(szLinkName) )
            {
                // directory entry is a symlink
                n = readlink(szLinkName, szLinkTarget, sizeof(szLinkTarget) - 1);
                if (n > 0  &&  strncmp(szLinkTarget, filePath, n) == 0)
                {
                    // symlink points to our target: delete symlink
                    szLinkTarget[n] = '\0';
                    DL_LOGPRINTF("Remove symlink [%s] -> [%s]", szLinkName, szLinkTarget);
                    unlink(szLinkName);
                }
            }
        }
        closedir(dirp);
    }
    
    // create a new symlink to the target
    done = FALSE;
    for ( sequence = 0 ; !done  &&  sequence <= 99 ; sequence++ )
    {
        // determine name of symlink
        time_t     t_time  = time(NULL);
        struct tm* tm_time = localtime( &t_time );
        snprintf( szLinkName,
                  MAX_PATH,
                  "%s/Link_%04d-%02d-%02dT%02d:%02d:%02d:%02d",
                   szDownloadHistoryDir,
                           tm_time->tm_year + 1900,
                                tm_time->tm_mon + 1,
                                     tm_time->tm_mday,
                                          tm_time->tm_hour,
                                               tm_time->tm_min,
                                                    tm_time->tm_sec,
                                                         sequence   );

        // create symbolic link, replace existing link when present
        if (symlink(filePath, szLinkName) == 0)
        {
            DL_LOGPRINTF("Created symlink [%s] -> [%s]", szLinkName, filePath);
            done = TRUE;
        }
        else
        {
            err = errno;
            DL_ERRORPRINTF("Error [%s] on create symlink [%s] -> [%s]", strerror(err), szLinkName, filePath);
            if (err == EEXIST)
            {
                DL_ERRORPRINTF("[%s] already exists", szLinkName);
            }
        }
    }
}


//get last char of directory name, if it's "/" ,return the location, 
//else return -1;
int getEndSepratorLocation(const char* sDir)
{
    if( NULL==sDir) return -1;
    
    int iLastPos=strlen(sDir)-1;
    if( '/' == sDir[iLastPos] )
    {
        return iLastPos;
    }
    return -1;
}

// mkdir hierarchical: creates specified directory and all parent directories
const int mkdirhier(const char *path, mode_t mode)
{
	char *ptr;
	char *dir;
	char old_char;
	struct stat s;
	int rv = 0;

	if (*path == '\0')
	{
		return 1;
	}

	dir = strdup(path);
	if (dir == NULL)
	{
		return 0;
	}

	ptr = dir;
	while (*ptr == '/')
	{
		ptr++;
	}
	while (*ptr != '\0')
	{
		// ptr = next '/' or end-of-string
		ptr = strchr(ptr, '/');
		if (ptr == NULL)
		{
			ptr = dir + strlen(dir);
		}

		// truncate dir and check whether it exists
		old_char = *ptr;
		*ptr = '\0';
		if ((stat(dir, &s) != 0))
		{
			if (mkdir(dir, mode) != 0)
			{
				// cannot make dir: fail
				goto END_MKDIRHIER;
			}
		}
		else if ( !S_ISDIR(s.st_mode)  &&  !S_ISLNK(s.st_mode))
		{
			// parent not directory or link: fail
			goto END_MKDIRHIER;
		}

		// restore dir and advance
		*ptr = old_char;
		if (*ptr)
		{
			ptr++;
		}
	}
	rv = 1;

END_MKDIRHIER:
	free(dir);
	return rv;
}

/* convert string to lower case */
char * strtolower(register char *s)
{
    char *ret = s;
    for ( ; *s; s++) if (isupper(*s)) *s = tolower(*s);
    return ret;
}
