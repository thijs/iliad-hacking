/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <gtk/gtk.h>
#include <libermanifest/ermanifest.h>
#include <liberregxml/erregapi.h>

#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "erMSDisk.h"
#include "contentListerLog.h"

// Global variables
static expMemType_e g_export_memory_type = expMain;
static expMemType_e g_memory_type_used   = expUndefined;

static int erMSDiskDisconnectFlash(void);
static int erMSDiskReconnectFlash(void);
static gboolean is_device_mounted(const char* dev);

static const char* dev[expUndefined] = 
{
    "/dev/tffsa6",
    "/dev/mmc1",
    "/dev/sda1",
    "/dev/hda1"
};

expMemType_e get_exported_memory_type(void)
{
    return g_export_memory_type;
}

expMemType_e get_memory_type_used(void)
{
    return g_memory_type_used;
}

expMemType_e read_exported_memory_type(void)
{
    regExportMemType_t* theExportMemType = NULL;
    expMemType_e        theExpMemType    = expMain;

    // Get settings from registry
    theExportMemType = erRegGetExportMemType();
    if (theExportMemType)
    {
        // Remember settings
        if (strcmp(theExportMemType->location, MAIN_CATEGORY) == 0)
        {
            theExpMemType =  expMain; 
        }
        else if (strcmp(theExportMemType->location, SD_CATEGORY) == 0)
        {
            theExpMemType = expMMC;
        }
        else if (strcmp(theExportMemType->location, USB_CATEGORY) == 0)
        {
            theExpMemType = expUSB;
        }
        else if (strcmp(theExportMemType->location, CF_CATEGORY) == 0)
        {
            theExpMemType =  expCF;
        }
        else
        {
            CL_ERRORPRINTF("Invalid exporting memory type: %s", theExportMemType->location);
        }

        erRegFreeExportMemType(theExportMemType);
    }
    else
    {
        CL_ERRORPRINTF("erRegGetExportMemType returns NULL");
    }
    
    CL_LOGPRINTF("leave, return [%d]", theExpMemType);
    return theExpMemType; 
}

void erMSDiskInit()
{
    g_export_memory_type = read_exported_memory_type();
}

// Return TRUE if dev is mounted, otherwise FALSE is returned
static gboolean is_device_mounted(const char* dev)
{
    FILE* fp = fopen(PROCFS_MOUNT_FILE, "r");
    if (NULL == fp)
    {
        printf("Can NOT open %s\n", PROCFS_MOUNT_FILE);
        return -1;
    }

    char buf[128];
    int len = strlen(dev);
    gboolean bFound = FALSE;
    while (!feof(fp))
    {
        fgets(buf, 128, fp);
        if (strncmp(buf, dev, len) == 0)
        {
            // Mount point found
            bFound = TRUE;
            break;
        }
    }

    fclose(fp);
    return bFound;
}

void erStartMSDiskApp()
{
    char cmd[CMD_LEN];
    expMemType_e memory_type_new = expMain;  // memory to be exported, default is internal memory

    CL_WARNPRINTF("entry: g_export_memory_type [%d] g_memory_type_used [%d]", g_export_memory_type, g_memory_type_used);

    // Use export type only when mounted
    if ( is_device_mounted(dev[g_export_memory_type]) )
    {
        memory_type_new = g_export_memory_type;
    }

    if (g_memory_type_used == memory_type_new)
    {
        ;  // msdisk application already running on required memory type: do nothing
    }
    else
    {
        // Stop msdisk application if running
        if (g_memory_type_used != expUndefined)
        {
            erStopMSDiskApp();
        }

        // Start msdisk application
        g_memory_type_used = memory_type_new;
        snprintf(cmd, sizeof(cmd), "sync ; msdisk %s &", dev[g_memory_type_used]);
        CL_WARNPRINTF("-- starting msdisk [%s]", cmd);
        system(cmd);
    }
}

void erStopMSDiskApp()
{
    // TODO: The better solution is using fork/exec and then sending SIGTERM signal
    CL_WARNPRINTF("-- stopping msdisk");
    system("killall msdisk; sync; sync;");

    g_memory_type_used = expUndefined;
}

int erMSDiskSetConnected(int connected)
{
    CL_WARNPRINTF("connected = %d", connected);

    if (connected == 1)
    {
        listerShowUSBScreen();
        setListerState(STATE_USB_CONNECTED);

#ifdef ENABLE_CONFIG_EXPORT_IMPORT
        ctrl_export_config();
#endif

        // For USB stick, if we umount USB memory before exporting it, it fails.
        // So just handle USB memory as an exeception, still don't know why.
        if (g_export_memory_type != expUSB)
        {
            erMSDiskDisconnectFlash();
        }

        return 0;
    }
    else if (connected == 0)
    {
        // Again, it can't work for USB stick...
        if (g_export_memory_type != expUSB)
        {
            erMSDiskReconnectFlash();
        }

        listerHideUSBScreen();
        setListerState(STATE_NORMAL);

#ifdef ENABLE_CONFIG_EXPORT_IMPORT
        ctrl_handle_import_config();
#endif

        mdsRecreateArchives(st_StorageTypeMain);
        mdsRecreateArchives(st_StorageTypeCF);
        mdsRecreateArchives(st_StorageTypeSD);
        mdsRecreateArchives(st_StorageTypeUSB);
        
        return 0;
    }
    else
    {
        return -1;
    }
}

static int erMSDiskDisconnectFlash()
{
    // stop background connect, if any
    ctrl_disconnect_wait();

    // unmount filesystem(s) exported over USB
    system("sync; sync; sync");

    char cmd[CMD_LEN];
    snprintf(cmd, CMD_LEN, "umount %s", dev[g_memory_type_used]);
    
    int ret = system(cmd);
    if (ret == 0)
    {
        return 0;
    }
    else
    {
        CL_ERRORPRINTF("Mount process returned error %d", ret);
        return -1;
    }
}

static int erMSDiskReconnectFlash()
{
    int rc;
    int ret = -1;  // return code: 0 = ok
    char cmd[CMD_LEN];
    
    // Determine mount command,
    // note that erStopMSDiskApp() sets g_memory_type_used to "not used"
    snprintf(cmd, CMD_LEN, "mount %s", dev[g_memory_type_used]);

    // stop msdisk application, to free any locks on g_memory_type_used device
    erStopMSDiskApp();

    // mount the filesystem so iLiad can access it
    CL_WARNPRINTF("remounting memory, command [%s]", cmd);
    rc = system(cmd);
    if (rc == 0)
    {
        rc = 0;
    }
    else
    {
        CL_ERRORPRINTF("Mount process returned error %d", rc);
    }

    // start msdisk application again
    erStartMSDiskApp();

    return ret;
}

void erMSDisk_storage_changed(st_ContentType_e storageType, gboolean storagePresent)
{
    CL_LOGPRINTF("storageType = %d, storagePresent = %d", storageType, storagePresent);

    // Convert st_ContentType_e to expMemType_e
    expMemType_e storage_type = expUndefined;
    switch (storageType)
    {
        case st_StorageTypeCF:
            storage_type = expCF;
            break;
        case st_StorageTypeSD:
            storage_type = expMMC;
            break;
        case st_StorageTypeUSB:
            storage_type = expUSB;
            break;
        default:
            break;
    }

    if (getListerState() == STATE_USB_CONNECTED)
    {
        // Connected to PC
        if (storagePresent == FALSE)
        {
            // Card plug out event
            if (storage_type == g_memory_type_used)
            {
                // The memory type we are using is plugged out
                CL_WARNPRINTF("WARNING! Card is plugged out when connected to PC.");

                // Stop msdisk application
                erStopMSDiskApp();
                listerHideUSBScreen();
                setListerState(STATE_NORMAL);
            }
            else
            {
                // Another type of memory is plugged out, don't care
            }
        }
    }
    else if (storage_type == g_export_memory_type)
    {   
        erStartMSDiskApp();
    }
}

void erMSDisk_handle_reread_settings()
{
    // Re-read export settings from registry
    g_export_memory_type = read_exported_memory_type();
}

