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

/**
 * \file toolbar.c
 * \brief contentLister - toolbar interaction with contentLister application
 *
 * <File description>
 *
 */
#include <glib.h>
#include <gtk/gtk.h>

#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "displayUpdate.h"
#include "toolbar.h"


static erClientChannel_t toolbarChannel;

static gboolean g_trashcan_selected    = FALSE;
static gboolean g_locked_selected      = FALSE;
static gboolean g_tagging_selected     = FALSE;
static gboolean g_search_selected      = FALSE;
static gboolean g_sort_selected        = FALSE;
static gboolean g_share_selected       = FALSE;

static gint     g_lock_screen_state = iconState_normal;
static gint     g_connect_state     = iconState_grey;


static void     toolbar_clear_internal(void);



void toolbar_init()
{
    int returnValue;

    CL_TBPRINTF("entry");
    
    // create/init the channel to communicate with the toolbar service
    returnValue = erIpcStartClient(ER_TOOLBAR_CHANNEL, &toolbarChannel);

    if (returnValue != 0)
    {
        CL_WARNPRINTF("erIpcStartClient returned %d", returnValue);
        return;
    }

    toolbar_restore();
}

void toolbar_restore()
{
    CL_TBPRINTF("entry");

    // freeze toolbar layout
    tbDisableUpdate(toolbarChannel, ER_CONTENT_LISTER_UA_ID);
    
    // clear toolbar, set system icons
    toolbar_clear_internal();
    
    // set application-specific icons
    //
    //     disable search icon
    tbAppendPlatformIcon(  toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_search, ccClToolbarIcon);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_search, iconState_grey );
    g_search_selected = FALSE;
     //     disable sort icon
    tbAppendPlatformIcon(  toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_sort, ccClToolbarIcon);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_sort, iconState_grey );
    g_sort_selected = FALSE;  

    //     disable share icon
    tbAppendPlatformIcon(  toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_share, ccClToolbarIcon);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_share, iconState_grey );

    // redraw toolbar now
    tbEnableUpdate(toolbarChannel, ER_CONTENT_LISTER_UA_ID);
}

static void toolbar_clear_internal()
{
    CL_TBPRINTF("entry");

    // tell toolbar which iconset to display
    tbSelectIconSet(toolbarChannel, ER_CONTENT_LISTER_UA_ID);

    // clear the toolbar
    tbClearIconSet(toolbarChannel, ER_CONTENT_LISTER_UA_ID);

    // set system icons
    //
    //     disable trashcan icon
    tbAppendPlatformIcon(  toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_trashcan, ccClToolbarIcon);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_trashcan, iconState_grey );
    g_trashcan_selected = FALSE;
    //
    //     restore connect icon
    tbAppendPlatformIcon(  toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_connect, ccClToolbarIcon);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_connect, g_connect_state);
    //
    //    restore screen lock (PIN code) icon
    tbAppendPlatformIcon(  toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_lock_screen, ccClToolbarIcon);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_lock_screen, g_lock_screen_state);
    //
    //     enable/disable locked icon
    tbAppendPlatformIcon(  toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_locked, ccClToolbarIcon);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_locked, g_locked_selected ? iconState_selected : iconState_normal);
    //
    //     disable keyboard icon    
    tbAppendPlatformIcon(  toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_keyboard, ccClToolbarIcon);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_keyboard, iconState_grey );
    //
    //     disable tagging icon
    tbAppendPlatformIcon(  toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_tagging, ccClToolbarIcon);
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID_tagging, iconState_grey );
    g_tagging_selected = FALSE;
}

void toolbar_clear()
{
    CL_TBPRINTF("entry");

    // freeze toolbar layout
    tbDisableUpdate(toolbarChannel, ER_CONTENT_LISTER_UA_ID);
    
    // clear the toolbar
    toolbar_clear_internal();

    // redraw toolbar now
    tbEnableUpdate(toolbarChannel, ER_CONTENT_LISTER_UA_ID);
}


// freeze toolbar layout
void toolbar_disableUpdate()
{
    CL_TBPRINTF("entry");

    tbDisableUpdate(toolbarChannel, ER_CONTENT_LISTER_UA_ID);
}

// redraw toolbar now
void toolbar_enableUpdate()
{
    CL_TBPRINTF("entry");

    tbEnableUpdate(toolbarChannel, ER_CONTENT_LISTER_UA_ID);
}

void toolbar_setIconState(int iconID, int iconState)
{
    CL_TBPRINTF("entry: iconID [%d] iconState [%d]", iconID, iconState);
    
    tbSetStatePlatformIcon(toolbarChannel, ER_CONTENT_LISTER_UA_ID, iconID, iconState);
    switch (iconID)
    {
        case iconID_trashcan:
            g_trashcan_selected = (iconState == iconState_selected);
            break;
        case iconID_connect:
            g_connect_state = iconState;
            break;
        case iconID_lock_screen:
            g_lock_screen_state = iconState;
            break;
        case iconID_locked:
            g_locked_selected = (iconState == iconState_selected);
            break;
        case iconID_tagging:
            g_tagging_selected = (iconState == iconState_selected);     
            break;
        case iconID_search:
            g_search_selected = (iconState == iconState_selected);
            break;            
        case iconID_sort:
            g_sort_selected = (iconState == iconState_selected);
            break;            
        case iconID_share:
            g_share_selected = (iconState == iconState_selected);
            break;            
        default:
            /* ignore */ ;
    }
}

void toolbar_synchronise(void)
{
    CL_TBPRINTF("entry");

    tbSynchronise(toolbarChannel, ER_CONTENT_LISTER_UA_ID, ccClToolbarSynchronised);
}

gboolean toolbar_is_trashcan_selected()
{
    CL_TBPRINTF("return [%d]", g_trashcan_selected);
    return g_trashcan_selected;
}

gboolean toolbar_is_locked_selected()
{
    CL_TBPRINTF("return [%d]", g_locked_selected);
    return g_locked_selected;
}
gboolean toolbar_is_tagging_selected()
{
    CL_TBPRINTF("return [%d]", g_tagging_selected);
    return g_tagging_selected;
}

gboolean toolbar_is_search_selected()
{
    CL_TBPRINTF("return [%d]", g_search_selected);
    return g_search_selected;
}

gboolean toolbar_is_sort_selected()
{
    CL_TBPRINTF("return [%d]", g_sort_selected);
    return g_sort_selected;
}

gboolean toolbar_is_lock_screen_selected()
{
    if (g_lock_screen_state == iconState_selected)
        return TRUE;
    else
        return FALSE;
}

gboolean toolbar_is_share_selected()
{
    CL_TBPRINTF("return [%d]", g_share_selected);
    return g_share_selected;
}
