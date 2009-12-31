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
 * \file control.c
 * \brief control - handle input and keep state                  
 * 
 * <File description>
 *
 */

/*
 * contentLister - A GTK based content lister (read only) application on
 * the eReader platform 
 * Copyright (C) 2005-2008 iRex Technologies B.V. 
 * All rights reserved.
 */

// config.h is generated based on the directives placed in the
// configure.ac 
// file and user input supplied to the configure script

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/stat.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <liberdm/display.h>
#include <liberdm/connectionMgrConstants.h>
#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>
#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>
#include <libermanifest/ermanifest.h>

#include "Geometry.h"
#include "contentListerLog.h"
#include "erConnect.h"
#include "erMSDisk.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "control_share.h"
#include "programManager.h"
#include "displayUpdate.h"
#include "viewer.h"
#include "cursor.h"
#include "gtkErrorScreen.h"
#include "button.h"
#include "metadataStoreManager.h"
#include "icons.h"
#include "pagebar.h"
#include "history.h"
#include "erbusy.h"
#include "languages.h"
#include "programManager.h"
#include "regValue.h"
#include "toolbar.h"
#include "powerManager.h"
#include "system.h"
#include "gtkMetadataWnd.h"
#include "gtkPincodeScreen.h"
#include "pincodeIdleTime.h"
#include "gtkSearchWnd.h"
#include "gtkSortWnd.h"
#include "gtkDistListWnd.h"
#include "timedids.h"

// local vars
static listerState_t    g_listerState = STATE_NORMAL;
static ContentLister*   gContentLister = NULL;
static ctrlErrorType_e  g_errorTypeDisplayed = ctrlNoError;
static gboolean         g_shutdownPending = FALSE;
static gchar            restoreUaOnTop[UAID_MAX_SIZE] = CONTENTLISTER_UAID;
static connectStruct_t  connectStruct;
static gboolean         g_background_connect_enable = FALSE;
static gboolean         g_new_content_received = FALSE;
static gboolean         g_new_content_acknowledged = FALSE;
static guint            g_idle_check_battery_id = 0;

// pending actions on toolbar_synchronise event
typedef void (*on_tbsync_action_t) (void);
static on_tbsync_action_t on_toolbar_sync_actions[] = { NULL, NULL };

// data for listitem select timer
typedef void on_item_selected_t(const int* index_tbl, ContentLister* theContentLister);
static struct
{
    int                 iconID;           // toolbar icon
    gboolean            confirm_with_icon;// confirm selection by clicking icon again?
    guint               ticks;            // for timer
    guint               timerID;          // for timer
    gboolean            active;           // is selection mode active?
    gboolean            icon_clicked;     // is selected icon clicked again?
    gboolean            item_selected[MAX_ITEMS_ON_ONE_PAGE];
    on_item_selected_t* on_item_selected; // callback on item selected
} g_select = { ICON_ID_NOTUSED, FALSE, 0, 0, FALSE, FALSE, {}, NULL };

// for meta data editor
static struct
{
    clDisplayItem_t*    item;           // current selected item
} g_md_editor_mgr;

// internally used functions
static void             ctrl_init_screen_layout(GtkWidget * topLevelWindow, ContentLister * theContentLister);
static void             ctrl_display_item_view(ContentLister * theContentLister);
static int              ctrl_scan_content(ContentLister * theContentLister, ctrlScanType_e scanType, char *folder);
static void             ctrl_get_content_name(st_ContentType_e theType, char *name);
static int              ctrl_goto_item(const gchar* szFilename);
static gboolean         ctrl_start_viewer(st_ContentType_e theType, clDisplayItem_t * theItem, char *location);
static void             ctrl_start_application(gchar* szExtID, gchar* options);
static void             ctrl_select_stop(gboolean restore_toolbar);
static gboolean         ctrl_file_exist(const char *pathName);
static gboolean         ctrl_check_extension(const char *extension);
static gboolean         ctrl_create_manifest(clDisplayItem_t * theItem, const char *dir, const char *name);
static const char*      ctrl_get_current_location(ContentLister * theContentLister);
static gboolean         ctrl_current_location_is_outbox(void);
static void             ctrl_background_connect_timeout_start_withinterval(gboolean read_settings, gboolean immediate, gint interval);
static gboolean         ctrl_show_search_wnd(gpointer data);
static void             ctrl_show_sort_wnd_trigger(void);
static gboolean         ctrl_show_sort_wnd(gpointer data);
static void             ctrl_hide_sort_wnd(gboolean bSort);
static void             ctrl_on_sort_clicked(int iconState);
static void             ctrl_sort_current(void);
static gint             ctrl_save_sort_settings(void);
static void             ctrl_sort_wnd_setting_init(void);

static void             ctrl_add_on_toolbar_sync_action(on_tbsync_action_t action);
static void             ctrl_show_keyboard();
static void             ctrl_hide_keyboard(int iconState);
static void             ctrl_select_listitem(int iconID, gboolean confirm_with_icon, on_item_selected_t* callback_on_selected);
static void             ctrl_listItem_edit(const int* index_tbl, ContentLister* theContentLister);
static void             ctrl_metadata_editor_close(gboolean bSave);
static void             ctrl_listItem_delete(const int* index_tbl, ContentLister* theContentLister);

static void ctrl_listItem_rate_a(const int* index_tbl, ContentLister* theContentLister);
static void ctrl_listItem_rate_aa(const int* index_tbl, ContentLister* theContentLister);
static void ctrl_listItem_rate_aaa(const int* index_tbl, ContentLister* theContentLister);
static void ctrl_listItem_rate(const int* index_tbl, ContentLister* theContentLister, int rating);

static gboolean         ctrl_not_create_manifest(const char *path);

static gboolean         ctrl_current_location_is_mobipocket(void);
static gboolean         ctrl_current_location_is_empty(void);

// callbacks 
static gboolean hook_screen_refresh_typing(GSignalInvocationHint *ihint, guint n_param_values, const GValue *param_values, gpointer user_data);
static guint    ctrl_listerArea_keypress(GtkWidget * widget, GdkEventKey * event, ContentLister * theContentLister);
static gboolean ctrl_main_window_expose_event(GtkWidget * widget, GdkEventExpose * event, ContentLister * theContentLister);
static void     ctrl_main_window_realize_event(GtkWidget * widget, ContentLister * theContentLister);
static guint    ctrl_errorScreen_buttonpress(GtkWidget * widget, GdkEventButton * event, ContentLister * theContentLister);
static gboolean ctrl_on_idle_show_keyboard(gpointer data);
static gboolean ctrl_on_idle_hide_keyboard(gpointer data);
static gboolean on_idle_connect_start(gpointer user_data);
static void     ctrl_on_search_clicked(int iconState);

// for pincode
static void ctrl_pincode_idle_time_callback();
static void ctrl_pincode_set_toolbar_icon();

static void ctrl_start_setup();
static void ctrl_lock_screen(void);

// user customized start-up behaviour
static gboolean ctrl_on_idle_select_language(gpointer data);
static void ctrl_start_up_behaviour(void);
static gboolean ctrl_open_quick_start_guide(const char *documentPath);
static void ctrl_display_reference_materials(void);
static gboolean ctrl_open_last_read_document(void);
static void ctrl_display_recent_documents();
static gboolean startup_idle_handler(gpointer data);
static gboolean startup_idle_display(gpointer data);

ContentLister *ctrlInit(GtkWidget * topLevelWindow)
{
    ContentLister *theContentLister = NULL;
    guint         signal_id;

    // create one theContentLister struct and fill it with zeros
    theContentLister = g_new0(ContentLister, 1);
    theContentLister->topLevelWindow = topLevelWindow;
    gContentLister = theContentLister;

    // init the connect fsm thread
    erConnectInit(&connectStruct);

    // init the msdisk application
    erMSDiskInit();

    // load the frequently used icons
    icons_init();

    // init the meta data store (including the contenttype history/stack)
    mdsInit();

    // init the history information
    hsInit();

    viewerInstallIpcServer();

    // whether or not the viewers are actually there is not important
    // during the init phase
    viewerInit(BROWSER_APP);

    erbusy_init();
    pagebar_init();
    toolbar_init();

    // power Manager init
    // MvdW: power manager disabled for now, needs more testing...
    // power_init();

    // init content key handling
    button_init(theContentLister);

    // init the program manager
    if (pm_InitProgramManager() != TRUE)
    {
        CL_ERRORPRINTF("Error initializing programManager, this will most likely cause a crash...");
    }

    // init the screen layout
    ctrl_init_screen_layout(topLevelWindow, theContentLister);
    theContentLister->lister = lsInit(theContentLister->listerArea, theContentLister);
    create_content_metadata_wnd(theContentLister->contentEditWnd);
    create_search_wnd(theContentLister->searchWnd);
    create_sort_wnd(theContentLister->sortWnd);
    create_dist_list_wnd(theContentLister->distListWnd);
    ctrlSetScreenTexts();

    // install class-specific screen update handlers
    //   erGtkEntry
    signal_id = g_signal_lookup("screen-refresh", ERGTK_ENTRY_TYPE);
    g_assert(signal_id > 0);
    g_signal_add_emission_hook(signal_id, 0, hook_screen_refresh_typing, (gpointer)TEXTENTRY_CHANGED_LEVEL, NULL);
    //   erGtkToggleButton
    signal_id = g_signal_lookup("toggled", ERGTK_TOGGLE_BUTTON_TYPE);
    g_assert(signal_id > 0);
    g_signal_add_emission_hook(signal_id, 0, hook_screen_refresh_typing, (gpointer)TEXTENTRY_CHANGED_LEVEL, NULL);

    cursor_init(theContentLister);

    theContentLister->itemCount = 0;
    theContentLister->currentContentType = st_ContentTypeUndefined;
    theContentLister->previousContentType = st_ContentTypeUndefined;
    theContentLister->currentFocus = 0;
    theContentLister->pagecount = 0;
    theContentLister->currentPage = 0;
    theContentLister->currentError = ctrlNoError;

    // Re-connect to server when requested
    gboolean do_connect = FALSE;
    int err = scGet_Reconnect(&do_connect);
    if (err == 0 && do_connect)
    {
        display_update_increase_level(NO_DISPLAY_UPDATE_LEVEL);
        g_idle_add(on_idle_connect_start, NULL);
    }
    else if (get_timedids_connect_reason() == connect_after_wakeup)
    {
        // iLiad is waken up by RTC alarm, need to connect to iDS
        display_update_increase_level(NO_DISPLAY_UPDATE_LEVEL);
        g_idle_add(on_timedids_connect_after_wakeup, NULL);
    }
    else
    {
        // Normal startup actions: select language, open QuickRef, etc.
        ctrl_startup();
    }

    return theContentLister;
}

static gboolean hook_screen_refresh_typing(GSignalInvocationHint *ihint, guint n_param_values, 
        const GValue *param_values, gpointer user_data)
{
        int level = (int)user_data;

        CL_WARNPRINTF("entry: level [%d]", level);

        display_update_request_screen_refresh(level);

        return TRUE;  // please call again
}

void ctrlSetScreenTexts()
{
    content_metadata_set_text();
    search_wnd_set_text();
    sort_wnd_set_text();
}

static gboolean on_idle_connect_start(gpointer user_data)
{
    CL_LOGPRINTF("entry");

    ctrl_connect(connectScrMode, connect_after_reboot);
    
    return FALSE;               // don't call again
}

gboolean on_idle_connect_start_background(gpointer p)
{
    CL_LOGPRINTF("entry");

    g_assert(p);

    e_ctrl_connect_reason_t connReason = (e_ctrl_connect_reason_t)(int)p;

    const gchar* uaOnTop = pm_getUaOnTop();

    if (   strcmp(uaOnTop, PROFILES_APP) == 0
        || strcmp(uaOnTop, SETUP_APP   ) == 0
        || listerIsErrorScreenVisible()
        || g_select.active                   )
    {
        // profile editor or manual connect is running
        // or settings applicationf is running
        // or error screen is visible
        //     --> try again after retry interval
        CL_WARNPRINTF("Retry after [%d] minutes", BG_CONNECT_RETRY_INTERVAL);
        if (connReason == connect_timed_ids_background)
        {
            timed_ids_handle_recreate_timer(BG_CONNECT_RETRY_INTERVAL);
        }
        else
        {
            ctrl_background_connect_timeout_start_withinterval(FALSE, FALSE, BG_CONNECT_RETRY_INTERVAL);
        }
    }
    /**Alexis 30nov07 DOWNLOADMGR_APP no longer used for manual connect, now PROFILES_APP **
    else if ( strcmp(uaOnTop, DOWNLOADMGR_APP) == 0 )
    {
        // manual connect busy --> ignore
        CL_WARNPRINTF("Download manager active, no background connect");
    }
    **/
    else
    {
        // no problems -> do background connect to iDS
        CL_WARNPRINTF("Do background connect");

        if (connReason == connect_timed_ids_background)
        {
            set_timedids_connect_reason(connect_when_running);
        }
        
        ctrl_connect(connectScrUnchanged, connReason);
    }

    return FALSE;  // don't call again
}

static void ctrl_background_connect_timeout_start_withinterval(gboolean read_settings, gboolean immediate, gint interval)
{
    CL_LOGPRINTF("entry: read_settings [%d] interval [%d]", read_settings, interval);

    static gint  background_connect_interval = 0;
    static gint  background_timeout_id       = 0;  // 0 = initially no timeout running

    // read background-connect settings from registry
    if (read_settings)
    {
        regAutoConnect_t* autoconnect_settings = NULL;

        // get settings from registry 
        autoconnect_settings = erRegGetAutoConnect();
        if (autoconnect_settings)
        {
            // remember settings
            g_background_connect_enable = autoconnect_settings->backgroundEnable;
            background_connect_interval = autoconnect_settings->backgroundInterval;
            CL_WARNPRINTF("registry read: enable [%d] interval [%d]", g_background_connect_enable, background_connect_interval);
            g_free(autoconnect_settings);
        }
        else
        {
            CL_ERRORPRINTF("no settings from erRegReadAutoconnect");
        }
    }

    // stop time-out if running
    if (background_timeout_id > 0)
    {
        g_source_remove(background_timeout_id);
        background_timeout_id = 0;
    }

    // start new time-out if requested
    if (immediate)
    {
        interval = 0;
    }
    else if (interval <= 0)
    {
        interval = background_connect_interval;
    }
    if (g_background_connect_enable  ||  immediate)
    {
        background_timeout_id = g_timeout_add( 100 + (interval * 60 * 1000),
                                               on_idle_connect_start_background,
                                               (gpointer)connect_background);
        CL_WARNPRINTF("timeout started, interval [%d] id [%d]", interval, background_timeout_id);
    }
}

void ctrl_background_connect_timeout_start(gboolean read_settings)
{
    CL_LOGPRINTF("entry: read_settings [%d]", read_settings);

    ctrl_background_connect_timeout_start_withinterval(read_settings, FALSE, 0);
}

void ctrl_set_connect_icon()
{

    CL_LOGPRINTF("entry");
    
    // update connect-state on toolbar
    if (g_new_content_received)
    {
        if (gContentLister->currentContentType == st_DownloadHistory)
        {
            toolbar_setIconState(iconID_connect, 
                g_new_content_acknowledged ? iconState_IDconnect_newcontent_selected : iconState_IDconnect_newcontent);
        }
        else if (g_new_content_acknowledged)
        {
            toolbar_setIconState(iconID_connect, g_background_connect_enable ? (e_iconState_t)iconState_IDconnect_time : iconState_normal);
            g_new_content_received = FALSE;
        }
        else
        {
            toolbar_setIconState(iconID_connect, iconState_IDconnect_newcontent);
        }
    }
    else
    {
        toolbar_setIconState(iconID_connect, g_background_connect_enable ? (e_iconState_t)iconState_IDconnect_time : iconState_normal);
    }
}

void ctrl_new_content_received()
{
    CL_LOGPRINTF("entry");

    g_new_content_received     = TRUE;
    g_new_content_acknowledged = FALSE;
}

void ctrlDestroy(ContentLister * contentLister)
{
    g_return_if_fail(contentLister != NULL);

    // TODO : store the last read content 

    // destroy the history information
    hsDestroy();

    pm_DestroyProgramManager();

    // store the loggin information 
    // g_free(contentLister);

    // free page content
    // mdsDestroy();

    // free pixmaps
    // icons_destroy();
}

/**
 * update cursor (previously the focus) 
 * @param index : index of the item in the pageitem list (0.., or -1 for none selected
 * @param data  : reference to contentlister structure
 *
 * @returns -
 */
static void ctrl_listItem_focus_no_redraw(int index, gpointer data)
{
    ContentLister *theContentLister = (ContentLister *) data;
    CL_CURSORPRINTF("entry: index [%d]", index);

    // move cursor to specified item
    cursor_set_state(theContentLister->currentFocus, e_cursor_hide);
    cursor_set_state(index, e_cursor_show);

    // remember where the focus is
    theContentLister->currentFocus = index;

    // update the stored index value
    if (index >= 0)
    {
        mdsSetIndex( theContentLister->currentContentType, 
                     (_G.pageItems * (theContentLister->currentPage - 1)) + index );
    }
}

static void ctrl_listItem_focus(int index, gpointer data)
{
    CL_CURSORPRINTF("entry: index [%d]", index);

    ctrl_listItem_focus_no_redraw(index, data);
    cursor_redraw();
}


static void ctrl_listItem_delete(const int* index_tbl, ContentLister* theContentLister)
{
    clDisplayItem_t* theItem;
    int              itemIndex = 0;
    int              index;
    const int*       indexPtr;
    gboolean         deleted = FALSE;
    char*            argv[10];
    int              argc;
    char*            dir = NULL;
    char*            cp;

    CL_CONTROLPRINTF("entry: index [%d]", index_tbl[0]);

    for (indexPtr = index_tbl ; *indexPtr >= 0 ; indexPtr++)
    {
        index = *indexPtr;

        if (index >= 0  &&  index < theContentLister->itemCount)
        {
            CL_CURSORPRINTF("index [%d] [%s] itemCount [%d] trashcan [%d]",
                                    index,
                                         theContentLister->items[index].szFilename,
                                                        theContentLister->itemCount,
                                                                      toolbar_is_trashcan_selected() );

            // Trashcan mode: delete the item
            erbusy_blink();
            theItem = &theContentLister->items[index];

            if (theItem->modifyEnable.bDelete == FALSE)
            {
                CL_WARNPRINTF("Delete disabled by manifest: item [%s]", theItem->szFilename);
            }
            else
            {
                // update the stored index value
                itemIndex = _G.pageItems * (theContentLister->currentPage - 1) + index;
                mdsSetIndex(theContentLister->currentContentType, itemIndex);
                switch (theItem->fit)
                {
                    case mdsFitFolder:
                        CL_WARNPRINTF("-- deleting folder [%s]", theItem->szFilename);
                        argc = 0;
                        argv[argc++] = "rm";
                        argv[argc++] = "-rf";
                        argv[argc++] = theItem->szFilename;
                        argv[argc] = NULL;
                        g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
                        fork_exec(argc, argv);
                        deleted = TRUE;
                        break;

                    case mdsFitContainer:
                    case mdsFitManifestDirectory:
                        CL_WARNPRINTF("-- deleting container [%s]", theItem->szManifest);
                        dir = (char *)alloca( strlen(theItem->szManifest) + 1 );
                        g_assert(dir != NULL);
                        strcpy(dir, theItem->szManifest);
                        cp = strrchr(dir, '/');
                        if (cp)
                        {
                            *cp = '\0';
                            argc = 0;
                            argv[argc++] = "rm";
                            argv[argc++] = "-rf";
                            argv[argc++] = dir;
                            argv[argc] = NULL;
                            g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
                            fork_exec(argc, argv);
                            deleted = TRUE;
                        }
                        else
                        {
                            CL_ERRORPRINTF("-- invalid container [%s]", theItem->szFilename);
                        }
                        break;

                    case mdsFitFile:
                        CL_WARNPRINTF("-- deleting file [%s]", theItem->szFilename);
                        unlink(theItem->szFilename);
                        deleted = TRUE;
                        break;

                    case mdsFitApplication:
                    case mdsFitStorage:
                    case mdsFitSymlink:
                        CL_ERRORPRINTF("-- not allowed to delete item [%s] fit [%d] ", theItem->szFilename, theItem->fit);
                        break;

                    default:
                        CL_ERRORPRINTF("-- unknown fit [%d] item [%s]", theItem->fit, theItem->szFilename);
                        break;
                }
            }
        }
    }

    if (deleted)
    {
        sync();
        ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
    }
}


static void ctrl_listItem_rate_a(const int* index_tbl, ContentLister* theContentLister) {
  ctrl_listItem_rate(index_tbl, theContentLister, 1);
}
static void ctrl_listItem_rate_aa(const int* index_tbl, ContentLister* theContentLister) {
  ctrl_listItem_rate(index_tbl, theContentLister, 2);
}
static void ctrl_listItem_rate_aaa(const int* index_tbl, ContentLister* theContentLister) {
  ctrl_listItem_rate(index_tbl, theContentLister, 3);
}

static void ctrl_listItem_rate(const int* index_tbl, ContentLister* theContentLister, int rating) {

    clDisplayItem_t* theItem;
    int              itemIndex = 0;
    int              index;
    const int*       indexPtr;
    char*            argv[10];
    int              argc;
    char*            dir = NULL;
    char*            cp;

    CL_CONTROLPRINTF("entry: index [%d]", index_tbl[0]);

    for (indexPtr = index_tbl ; *indexPtr >= 0 ; indexPtr++) {
        index = *indexPtr;

        if (index >= 0  &&  index < theContentLister->itemCount) {

            CL_CURSORPRINTF("index [%d] [%s] itemCount [%d] rating [%d]",
                            index,
                            theContentLister->items[index].szFilename,
                            theContentLister->itemCount,
                            rating );

            // rating mode: run rating script
            erbusy_blink();
            theItem = &theContentLister->items[index];

            // update the stored index value
            itemIndex = MAX_ITEMS_ON_ONE_PAGE * (theContentLister->currentPage - 1) + index;
            mdsSetIndex(theContentLister->currentContentType, itemIndex);

            switch (theItem->fit) {
                    case mdsFitFile:
                        CL_WARNPRINTF("-- rating file [%s]", theItem->szFilename);

                        dir = alloca( strlen(theItem->szManifest) + 1 );
                        g_assert(dir != NULL);
                        strcpy(dir, theItem->szManifest);
                        cp = strrchr(dir, '/');
                        if (cp) {
                            *cp = '\0';
                            argc = 0;
                            argv[argc++] = "/home/intent/ratealt";
                            switch (rating) {
                              case 1:
                                argv[argc++] = "1";
                                break;
                              case 2:
                                argv[argc++] = "2";
                                break;
                              case 3:
                                argv[argc++] = "3";
                                break;
                            }
                            argv[argc++] = dir;
                            argv[argc] = NULL;
                            g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
                            fork_exec(argc, argv);
                        }
                        break;

                    default:
                        CL_ERRORPRINTF("-- unknown fit [%d] item [%s]", theItem->fit, theItem->szFilename);
                        break;
            }
        }
    }
}


void ctrl_listItem_clicked(int index, gpointer data)
{
    ContentLister    *theContentLister = (ContentLister *) data;
    clDisplayItem_t  *theItem;
    int              itemIndex = 0;
    st_ContentType_e storageType;
    gboolean         ret;

    CL_CONTROLPRINTF("Entry: index [%d]", index);

    // action required only when content lister in normal state and visible
    if (   g_listerState != STATE_NORMAL
        || strcmp(pm_getUaOnTop(), CONTENTLISTER_UAID) != 0)
    {
        CL_WARNPRINTF("Ignore because contentLister in special state or not visible");
        return;
    }

    if (g_select.active)
    {
        // select mode: blink or hide cursor for this item
        if (g_select.item_selected[index])
        {
            g_select.item_selected[index] = FALSE;
            cursor_set_state(index, e_cursor_hide);
            cursor_redraw();
        }
        else
        {
            g_select.item_selected[index] = TRUE;
            cursor_set_state(index, e_cursor_blink);
            cursor_redraw();

            if ( !g_select.confirm_with_icon )
            {
                g_select.icon_clicked = TRUE;
            }
        }

        // restart the select timeout
        g_select.ticks = ITEM_SELECT_TIMEOUT_TICKS;
    }
    else if ( !ctrl_current_location_is_outbox() )
    {
        // Not in select mode: open the item
        erbusy_blink();

        theItem = &theContentLister->items[index];

        // position cursor on the selected item
        ctrl_listItem_focus(index, theContentLister);

        // update the stored index value
        itemIndex = ctrl_get_item_index(theContentLister);
        mdsSetIndex(theContentLister->currentContentType, itemIndex);

        // now act on the selected item
        switch (theItem->fit)
        {
            case mdsFitFolder:
                ctrl_scan_content(theContentLister, ctrlScanNext, theItem->szFilename);
                break;

            case mdsFitApplication:
                CL_CONTROLPRINTF("mdsFitApplication");
                // before enter setup, ask user input pincode
                if (0 == strcmp(theItem->szExtID, SETUP_APP))
                {
                    gboolean enable = FALSE;
                    gchar pincode[PINCODE_MAX_LENGTH + 1];

                    gtk_pincode_screen_get_pincode_settings(theContentLister->pincodeScreen, &enable, pincode);
                    if (FALSE == enable || 0 == strlen(pincode))
                    {
                        ctrl_start_application(theItem->szExtID, NULL);
                    }
                    else
                    {
                        ctrl_show_pincode_screen(enterSettings_t, ctrl_start_setup, NULL);
                    }
                }
                else
                {
                    ctrl_start_application(theItem->szExtID, NULL);
                }
                break;

            case mdsFitManifestDirectory:
                CL_CONTROLPRINTF("mdsFitManifestDirectory -> %s", theItem->szFilename);
                ctrl_scan_content(theContentLister, ctrlScanNext, theItem->szFilename);
                break;

            case mdsFitStorage:
                storageType = ctrl_get_storageType(theItem->szExtID);

                if (stackIsStorage(storageType) == TRUE)
                {
                    CL_CONTROLPRINTF("scan storage type %d", storageType);

                    // update the contentType
                    theContentLister->previousContentType = theContentLister->currentContentType;
                    theContentLister->currentContentType = storageType;
                    ctrl_scan_content(theContentLister, ctrlScanCurrent, 0);
                }
                else
                {
                    CL_ERRORPRINTF("invalid storage type");
                }
                break;
                
            case mdsFitContainer:
                ret = ctrl_start_viewer(theContentLister->currentContentType, theItem, NULL);
                if (ret == FALSE)
                {
                    // Not busy!
                    erbusy_off();
                }
                break;
                
            case mdsFitFile:
                ctrl_before_file_clicked(theItem);
                ret = ctrl_start_viewer(theContentLister->currentContentType, theItem, NULL);
                if (ret == FALSE)
                {
                    // Not busy!
                    erbusy_off();
                }
                break;

            case mdsFitSymlink:
            default:
                CL_ERRORPRINTF("theItem->fit %d - UNKNOWN", theItem->fit);
        }
    }
}

void ctrl_storage_changed(st_ContentType_e storageType, gboolean storagePresent)
{
    ContentLister *theContentLister = gContentLister;
    
    CL_CONTROLPRINTF("Entry: storageType [%d] Present [%d]", storageType, storagePresent);

    if (storagePresent)
    {
        // if the archive locations are redirected to the storage device,
        // check the archive folders exits or not, if not create them
        mdsRecreateArchives(storageType);
    }

    if (theContentLister != NULL 
        && (theContentLister->currentContentType == storageType 
            || mdsArcLocIsChanged(theContentLister->currentContentType, storageType)) )
    {
        if (!storagePresent)
        {
            ctrl_hide_misc_screen();
        }
        ctrl_select_stop(TRUE);

        if (stackIsStorage(storageType) == TRUE)
        {
            CL_CONTROLPRINTF("scan storage type %d", storageType);
            stackClear(theContentLister->currentContentType);
            ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
        }
        else
        {
            CL_ERRORPRINTF("invalid storage type");
        }
    }

    erMSDisk_storage_changed(storageType, storagePresent);
}

/**
 * start - if needed - the viewer for the specified contentype to view the specified item
 * and update the history data 
 *
 * @param theType 
 * @param theItem - viewed item 
 * @param location - last viewed location of the item (can be NULL)
 *
 * @returns TRUE when the viewer started with succes
 */
static gboolean ctrl_start_viewer(st_ContentType_e theType, clDisplayItem_t * theItem, char *location)
{
    char *viewer_id;
    gboolean returnValue = TRUE;
    int viewerReturn = 0;
    gboolean isActive = FALSE;
    st_ContentType_e ContentTypeUsingViewer;
    gboolean viewerStartNeeded = TRUE;

    g_assert(theItem != NULL);
    CL_CONTROLPRINTF("Entry: contentType [%d] file [%s]", theType, theItem->szFilename);

    viewer_id = pm_getUaID(theItem->szFileExt);
    if (NULL != viewer_id)
    {
        // => only request a refresh on the main window expose event
        CL_SCREENPRINTF("");

        display_update_increase_level(TOOLBAR_UPDATE_LEVEL);

        // check if the viewer is already active
        isActive = pm_IsActive(viewer_id);

        CL_HISTORYPRINTF("viewer %s is active = %s", viewer_id, (isActive ? "TRUE" : "FALSE"));

        if (isActive)
        {
            ContentTypeUsingViewer = hsGetContentType(viewer_id);
            CL_HISTORYPRINTF(" ContentTypeUsingViewer = %d", ContentTypeUsingViewer);

            if ((ContentTypeUsingViewer == theType) && (stackIsArchive(ContentTypeUsingViewer)))
            {
                //viewer is alreay active for this archive content type
                CL_HISTORYPRINTF("Viewer still active => RaiseViewer %s", viewer_id);
                //pm_RaiseViewer(viewer_id);
                //viewerStartNeeded = FALSE;
            }
            else
            {
                //viewer is active for an archive key 
                if (stackIsArchive(ContentTypeUsingViewer))
                {
                    CL_HISTORYPRINTF("viewerStore viewer [id=%s] ", viewer_id);
                    viewerStore(viewer_id, ContentTypeUsingViewer);
                }
            }
        }

        if (viewerStartNeeded)
        {
            CL_HISTORYPRINTF("viewerStartNeeded (loc = %s)", (location == NULL) ? "NULL" : location);

            viewerReturn = pm_RunViewer(viewer_id, theItem->szFilename, theItem->szManifest, NULL, 0);

            if (viewerReturn == ERR_OK)
            {
                // keep track of which content type is using which viewer
                hsSetActiveViewer(theType, viewer_id);

                // keep track of the last viewed content Item
                hsStoreItem(theType, theItem);

                //clear the last viewed location
                hsRemoveLocation(theType);
                
                // start progress indication (knight rider)
                start_progress_indicator();
            }
            else
            {
                CL_ERRORPRINTF("pmRunViewer returned %d", viewerReturn);
                returnValue = FALSE;
            }
        }                   //(viewerStartNeeded)  
    }
    else
    {
        CL_ERRORPRINTF("theItem->szFileExt [%s] - UNHANDLED FILE TYPE", theItem->szFileExt);
        returnValue = FALSE;
    }

    return returnValue;
}

/**
 * Scan the current/next or previous directory of the specified content Type 
 * Display the first page of the result and update the relevant contentLister 
 * struct data fields  
 *
 * @param contentLister reference contentLister data 
 * @param scanType next, previous or current stack position 
 * @param folder next folder name 
 *
 * NOTE : - The folder parameter is only used for a "next" directory content scan
 *
 * @returns the scan result return value
 */
static int ctrl_scan_content(ContentLister * theContentLister, ctrlScanType_e scanType, char *folder)
{
    int totalItemCount;
    int result = ERMDS_CONTENT_SCAN_FAILED;
    int req_cnt = _G.pageItems;
    int itemIndex = 0;
    int startindex;

    CL_CONTROLPRINTF("Entry: folder [%s] type [%d]", folder, theContentLister->currentContentType);

    if (   theContentLister->currentContentType <  0
        || theContentLister->currentContentType >= st_ContentTypeUndefined)
    {
        return ERMDS_CONTENT_SCAN_FAILED;
    }

    // remove dangling symlinks
    if (   theContentLister->currentContentType == st_RecentDocuments
        || theContentLister->currentContentType == st_DownloadHistory )
    {
        char dirname[ERMDS_MAX_FILENAME_SIZE];
        if ( mdsGetRootLocation(theContentLister->currentContentType, dirname, sizeof(dirname)) > 0 )
        {
            if (getListerState() != STATE_USB_CONNECTED)
            {
                hsCleanupSymlinks(dirname);
            }
            else
            {
                CL_WARNPRINTF("Not cleaning up history when listerState is STATE_USB_CONNECTED");
            }
        }
    }

    // update toolbar icon "connect"
    ctrl_set_connect_icon();
    
    // scan the specified type and location
    switch (scanType)
    {
        case ctrlScanCurrent:
            result = mdsScanCurrent(theContentLister->currentContentType);
            itemIndex = mdsGetIndex(theContentLister->currentContentType);
            CL_CURSORPRINTF("ctrlScanCurrent itemIndex %d", itemIndex);
            break;
        case ctrlScanNext:
            CL_CONTROLPRINTF("scan folder %s", folder);
            itemIndex = 0;
            CL_CURSORPRINTF("ctrlScanNext itemIndex %d", itemIndex);
            result = mdsScanNext(theContentLister->currentContentType, folder, 0);
            break;
        case ctrlScanPrevious:
            result = mdsScanPrevious(theContentLister->currentContentType);
            itemIndex = mdsGetIndex(theContentLister->currentContentType);
            CL_CURSORPRINTF("ctrlScanCurrent itemIndex %d", itemIndex);
            break;
    }

    if ((ERMDS_CONTENT_SCAN_OK == result) || (ERMDS_CONTENT_SCAN_TOO_MANY_ITEMS == result))
    {
        // possibly second time in the loop => undo error indication
        theContentLister->currentError = ctrlNoError;

        totalItemCount = mdsGetItemCount();
        if (itemIndex >= totalItemCount)
        {
            itemIndex = totalItemCount - 1;
        }

        if (totalItemCount > 0)
        {
            theContentLister->pagecount = (totalItemCount + (_G.pageItems - 1)) / _G.pageItems;
            theContentLister->currentPage = (itemIndex / _G.pageItems) + 1;
            theContentLister->currentFocus = itemIndex % _G.pageItems;
            CL_CURSORPRINTF("page %d focus %d", theContentLister->currentPage, theContentLister->currentFocus);

            startindex = _G.pageItems * (theContentLister->currentPage - 1);
            mdsSetPageContent(theContentLister->items, startindex, &req_cnt);
            theContentLister->itemCount = req_cnt;

        }
        else
        {
            theContentLister->itemCount = 0;
            theContentLister->pagecount = 0;
            theContentLister->currentPage = 0;
            theContentLister->currentFocus = 0;
        }

        ctrl_display_item_view(theContentLister);
    }
    else
    {
        theContentLister->currentError = ctrlScanFailedError;

        if (scanType == ctrlScanNext)
        {
            // restore the stack
            stackPop(theContentLister->currentContentType);
        }

        listerShowErrorScreen(ctrlScanFailedError);
    }
    return result;
}

// convert the ID from the manifest file to the known storage types
st_ContentType_e ctrl_get_storageType(gchar * szExtID)
{
    st_ContentType_e theType = st_ContentTypeUndefined;

    if (strncmp(szExtID, CF_ID, ERMDS_MAX_EXTENSION_ID_SIZE) == 0)
    {
        theType = st_StorageTypeCF;
    }
    else if (strncmp(szExtID, SD_ID, ERMDS_MAX_EXTENSION_ID_SIZE) == 0)
    {
        theType = st_StorageTypeSD;
    }
    else if (strncmp(szExtID, USB_ID, ERMDS_MAX_EXTENSION_ID_SIZE) == 0)
    {
        theType = st_StorageTypeUSB;
    }
    else if (strncmp(szExtID, MAIN_MEMORY_ID, ERMDS_MAX_EXTENSION_ID_SIZE) == 0)
    {
        theType = st_StorageTypeMain;
    }
    else if (strncmp(szExtID, RECENT_DOCS_ID, ERMDS_MAX_EXTENSION_ID_SIZE) == 0)
    {
        theType = st_RecentDocuments;
    }

    return theType;
}

unsigned int ctrl_get_item_index(ContentLister * theContentLister)
{
    unsigned int itemIndex;
    int pageIndex;

    pageIndex = (theContentLister->currentPage - 1);
    if (pageIndex < 0)
    {
        pageIndex = 0;
    }

    itemIndex = (pageIndex * _G.pageItems);
    if (itemIndex >= 0)
    {
        itemIndex += theContentLister->currentFocus;
    }

    CL_CURSORPRINTF ("item index %d (current page %d, pageindex %d, currentFocus %d)",
                                 itemIndex,       theContentLister->currentPage,
                                                                pageIndex,       theContentLister->currentFocus);

    return itemIndex;
}

static void ctrl_init_screen_layout(GtkWidget * topLevelWindow, ContentLister * theContentLister)
{
    GtkWidget *topLevelVBox;

    // object hierarchy:
    //     topLevelWindow
    //       |-- topLevelVBox
    //             |
    topLevelVBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(topLevelWindow), topLevelVBox);
    //             |
    //             |-- theContentLister->listerScreen
    //             |     |-- alignment
    //             |           |-- vbox
    //             |                 |
    theContentLister->listerScreen = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(theContentLister->listerScreen), "lister_background");
    gtk_box_pack_start(GTK_BOX(topLevelVBox), theContentLister->listerScreen, TRUE, TRUE, 0);
    gtk_widget_show(theContentLister->listerScreen);
    //
    GtkWidget* alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_widget_show(alignment);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), _G.listerBorder, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(theContentLister->listerScreen), alignment);
    // 
    GtkWidget *vbox = gtk_vbox_new(FALSE, _G.itemSpacing);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    gtk_widget_show(vbox);
    //             |                 |-- theContentLister->listerArea (event box)
    //             |                 |
    theContentLister->listerArea = gtk_event_box_new();
    gtk_widget_set_name(GTK_WIDGET(theContentLister->listerArea), "lister_background");
    GTK_WIDGET_SET_FLAGS(GTK_WIDGET(theContentLister->listerArea), GTK_CAN_FOCUS);
    gtk_box_pack_start(GTK_BOX(vbox), theContentLister->listerArea, FALSE, FALSE, 0);
    gtk_widget_show(theContentLister->listerArea);
    //             |                 |
    //             |                 |-- alignment
    //             |                       |-- vbox
    //             |                             |
    alignment = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
    gtk_alignment_set_padding(GTK_ALIGNMENT(alignment), 0, 0, _G.listerBorder, 0);
    gtk_box_pack_start(GTK_BOX(vbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show(alignment);
    // 
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(alignment), vbox);
    gtk_widget_show(vbox);
    //             |                             |
    //             |                             |-- theContentLister->contentEditWnd (event box)
    theContentLister->contentEditWnd = gtk_event_box_new();
    GTK_WIDGET_SET_FLAGS(GTK_WIDGET(theContentLister->contentEditWnd), GTK_CAN_FOCUS);
    gtk_widget_set_name(GTK_WIDGET(theContentLister->contentEditWnd), "lister_background");
    gtk_box_pack_start(GTK_BOX(vbox), theContentLister->contentEditWnd, FALSE, FALSE, METADATA_WND_SPACE_ABOVE);
    //             |                             |
    //             |                             |-- theContentLister->searchWnd (event box)
    theContentLister->searchWnd = gtk_event_box_new();
    GTK_WIDGET_SET_FLAGS(GTK_WIDGET(theContentLister->searchWnd), GTK_CAN_FOCUS);
    gtk_widget_set_name(GTK_WIDGET(theContentLister->searchWnd), "lister_background");
    gtk_box_pack_start(GTK_BOX(vbox), theContentLister->searchWnd, FALSE, FALSE, SEARCH_WND_SPACE_ABOVE);
    //             |                             |
    //             |                             |-- theContentLister->sortWnd (event box)
    theContentLister->sortWnd = gtk_event_box_new();
    GTK_WIDGET_SET_FLAGS(GTK_WIDGET(theContentLister->sortWnd), GTK_CAN_FOCUS);
    gtk_widget_set_name(GTK_WIDGET(theContentLister->sortWnd), "lister_background");
    gtk_box_pack_start(GTK_BOX(vbox), theContentLister->sortWnd, FALSE, FALSE, 0);
    //             |                             |
    //             |                             |-- theContentLister->distListWnd (event box)
    //             |
    theContentLister->distListWnd = gtk_event_box_new();
    GTK_WIDGET_SET_FLAGS(GTK_WIDGET(theContentLister->distListWnd), GTK_CAN_FOCUS);
    gtk_widget_set_name(GTK_WIDGET(theContentLister->distListWnd), "lister_background");
    gtk_box_pack_start(GTK_BOX(vbox), theContentLister->distListWnd, FALSE, FALSE, 0);
    //             |
    //             |-- theContentLister->errorScreen (GtkErrorScreen)
    //             |
    theContentLister->errorScreen = gtk_error_screen_new();
    GTK_WIDGET_SET_FLAGS(GTK_WIDGET(theContentLister->errorScreen), GTK_CAN_FOCUS);
    gtk_box_pack_start(GTK_BOX(topLevelVBox), theContentLister->errorScreen, TRUE, TRUE, 0);
    //             |
    //             |-- theContentLister->pincodeScreen (GtkPincodeScreen)
    //
    theContentLister->pincodeScreen = gtk_pincode_screen_new();
    GTK_WIDGET_SET_FLAGS(GTK_WIDGET(theContentLister->pincodeScreen), GTK_CAN_FOCUS);
    gtk_box_pack_start(GTK_BOX(topLevelVBox), theContentLister->pincodeScreen, TRUE, TRUE, 0);

    gtk_widget_show(topLevelVBox);

    g_signal_connect(G_OBJECT(theContentLister->listerArea),
                     "key_press_event", G_CALLBACK(ctrl_listerArea_keypress), theContentLister);
    g_signal_connect(G_OBJECT(theContentLister->errorScreen),
                     "button_press_event", G_CALLBACK(ctrl_errorScreen_buttonpress), theContentLister);

    g_signal_connect(G_OBJECT(topLevelWindow), "expose-event",
                     G_CALLBACK(ctrl_main_window_expose_event), theContentLister);
    g_signal_connect(G_OBJECT(topLevelWindow), "realize", G_CALLBACK(ctrl_main_window_realize_event), theContentLister);
}

/**
 * Update the page content and the pagebar with the updated 
 * contentlister data: 
 *
 * @param contentLister reference contentLister data 
 *
 * @returns -
 */
static void ctrl_display_item_view(ContentLister * theContentLister)
{
    char page_title[ERCL_MAX_TITLE_SIZE];

    CL_CONTROLPRINTF("entry");

    if (theContentLister)
    {
        switch (theContentLister->currentContentType)
        {
            case st_ContentTypeBooks:
            case st_ContentTypeNews:
            case st_ContentTypeDocs:
            case st_ContentTypeNotes:
            case st_StorageTypeMain:        // MAIN MEMORY
            case st_StorageTypeCF:      // Should check if filesystem is readonly, but how?
            case st_StorageTypeSD:      // Should check if filesystem is readonly, but how?
            case st_StorageTypeUSB:     // Should check if filesystem is readonly, but how?
                toolbar_disableUpdate();
                // Custom toolbar states for certain special folders
                if ( ctrl_current_location_is_empty() )
                {
                    toolbar_setIconState(iconID_trashcan, iconState_grey);
                }
                else
                {
                    toolbar_setIconState(iconID_trashcan, iconState_normal);
                }

                if ( ctrl_current_location_is_mobipocket() 
                        || ctrl_current_location_is_outbox() 
                        || ctrl_current_location_is_empty() )
                {
                    toolbar_setIconState(iconID_share,    iconState_grey  );
                    toolbar_setIconState(iconID_tagging,  iconState_grey  );
                }
                else
                {
                    toolbar_setIconState(iconID_share,    iconState_normal);
                    toolbar_setIconState(iconID_tagging,  iconState_normal);
                }
                
                if ( ctrl_current_location_is_outbox() || ctrl_current_location_is_empty() ) 
                {
                    toolbar_setIconState(iconID_search,   iconState_grey  );
                    toolbar_setIconState(iconID_sort,     iconState_grey  );

                    toolbar_setIconState(iconID_rate_a,   iconState_grey  );
                    toolbar_setIconState(iconID_rate_aa,  iconState_grey  );
                    toolbar_setIconState(iconID_rate_aaa, iconState_grey  );
                }
                else
                {
                    toolbar_setIconState(iconID_search,   iconState_normal);
                    toolbar_setIconState(iconID_sort,     iconState_normal);

                    toolbar_setIconState(iconID_rate_a,   iconState_normal);
                    toolbar_setIconState(iconID_rate_aa,  iconState_normal);
                    toolbar_setIconState(iconID_rate_aaa, iconState_normal);
                }
                
                toolbar_enableUpdate();
                break;
                
            case st_MenuTypeMode:
            case st_DownloadHistory:
            case st_RecentDocuments:
                toolbar_disableUpdate();
                toolbar_setIconState(iconID_search,   iconState_grey);
                toolbar_setIconState(iconID_sort,     iconState_grey);
                toolbar_setIconState(iconID_share,    iconState_grey);
                toolbar_setIconState(iconID_tagging,  iconState_grey);
                toolbar_setIconState(iconID_trashcan, iconState_grey);

                toolbar_setIconState(iconID_rate_a,   iconState_grey  );
                toolbar_setIconState(iconID_rate_aa,  iconState_grey  );
                toolbar_setIconState(iconID_rate_aaa, iconState_grey  );

                toolbar_enableUpdate();
                break;

            case st_SearchResult:
                toolbar_disableUpdate();
                toolbar_setIconState(iconID_trashcan,    iconState_grey  );
                toolbar_setIconState(iconID_tagging,     iconState_grey  );
                if ( ctrl_current_location_is_empty() )
                {
                    toolbar_setIconState(iconID_search,      iconState_grey);
                }
                else
                {
                    toolbar_setIconState(iconID_search,      iconState_normal);
                }
                toolbar_setIconState(iconID_sort,        iconState_grey);
                toolbar_setIconState(iconID_share,       iconState_grey  );

                toolbar_setIconState(iconID_rate_a,   iconState_grey  );
                toolbar_setIconState(iconID_rate_aa,  iconState_grey  );
                toolbar_setIconState(iconID_rate_aaa, iconState_grey  );

                toolbar_enableUpdate();
                break;

            default:
                CL_ERRORPRINTF("Unknown currentContentType: %d", theContentLister->currentContentType);
                toolbar_disableUpdate();
                toolbar_setIconState(iconID_search,   iconState_grey);
                toolbar_setIconState(iconID_sort,     iconState_grey);
                toolbar_setIconState(iconID_share,    iconState_grey);
                toolbar_setIconState(iconID_tagging,  iconState_grey);
                toolbar_setIconState(iconID_trashcan, iconState_grey);

                toolbar_setIconState(iconID_rate_a,   iconState_grey  );
                toolbar_setIconState(iconID_rate_aa,  iconState_grey  );
                toolbar_setIconState(iconID_rate_aaa, iconState_grey  );

                toolbar_enableUpdate();
        }

        // hide cursor for outbox
        if (ctrl_current_location_is_outbox())
        {
            ctrl_listItem_focus(-1, theContentLister);
        }

        ctrl_get_content_name(theContentLister->currentContentType, page_title);

        // get current location information. display location in title bar
        const char* location;
        if (theContentLister->currentContentType == st_DownloadHistory)
        {
            location = "";
        }
        else
        {
            location = ctrl_get_current_location(theContentLister);
        }
        lsUpdatePage(theContentLister->lister, theContentLister->items,
                     theContentLister->itemCount, theContentLister->currentFocus, page_title, location);

        pagebar_reset();        // a little overkill, but this ensures the 
        // pageBar always has the correct
        // currentApp
        pagebar_set_pagecount(theContentLister->pagecount);
        pagebar_goto_page(theContentLister->currentPage);
        pagebar_redraw();
    }
    else
    {
        CL_ERRORPRINTF("theContentLister == NULL");
    }
}

static guint ctrl_listerArea_keypress(GtkWidget * widget, GdkEventKey * event, ContentLister * theContentLister)
{
    int newPage;
    int returnValue = 0;        // return FALSE => default gtk
    // handling
    int newFocus;

    CL_CONTROLPRINTF("entry: keyval [0x%04X]", event->keyval);

    CL_TIMEDISPLAY("keypress handling");

    // ignore all keys during select or rename action
    if (   g_select.active
        || toolbar_is_tagging_selected() )
    {
        erbusy_off();
        return returnValue;
    }

    switch (event->keyval)
    {
        case GDK_Page_Down:
            newPage = theContentLister->currentPage + 1;
            ctrl_goto_page(newPage);

            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_F1:
            // long PAGETURN_FORWARD keypress
            newPage =
                ((theContentLister->currentPage + 5) <= theContentLister->pagecount) ? (theContentLister->currentPage + 5)
                                                                                     : theContentLister->pagecount;
            ctrl_goto_page(newPage);

            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_Page_Up:
            newPage = theContentLister->currentPage - 1;
            ctrl_goto_page(newPage);

            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_F2:
            // long PAGETURN_BACK keypress
            newPage = ((theContentLister->currentPage - 5) < 1) ? (1) : (theContentLister->currentPage - 5);
            ctrl_goto_page(newPage);

            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_Home:
            // long press
            CL_CONTROLPRINTF("jumping back to root (Home)");
            stackClear(theContentLister->currentContentType);
            ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_F5:
            // OVERVIEW keypress
            CL_CONTROLPRINTF("jumping one level up (F5)");
            if ( (st_StorageTypeCF   == theContentLister->currentContentType ||
                  st_StorageTypeSD   == theContentLister->currentContentType ||
                  st_StorageTypeUSB  == theContentLister->currentContentType ||
                  st_StorageTypeMain == theContentLister->currentContentType ||
                  st_RecentDocuments == theContentLister->currentContentType   ) &&
                0 >= stackHeight(theContentLister->currentContentType))
            {
                // goto device manager
                theContentLister->currentContentType = st_MenuTypeMode;
                ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
            }
            else if (st_SearchResult == theContentLister->currentContentType
                     && 0 >= stackHeight(theContentLister->currentContentType))
            {
                CL_WARNPRINTF("    -- Search result: return to previous");
                theContentLister->currentContentType = theContentLister->previousContentType;
                theContentLister->previousContentType = st_SearchResult;
                ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
            }
            else
            {
                ctrl_scan_content(theContentLister, ctrlScanPrevious, NULL);
            }
            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_Return:
        case GDK_F6:
            if (theContentLister->itemCount > 0)
            {
                CL_CONTROLPRINTF("jumping to focused item %d", theContentLister->currentFocus);

                // find the item that has the focus
                if (theContentLister->currentFocus >= 0 && theContentLister->currentFocus < _G.pageItems)
                {
                    ctrl_listItem_clicked(theContentLister->currentFocus, theContentLister);
                }
            }
            else
            {
                erbusy_off();
            }
            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_Up:
            if ( !ctrl_current_location_is_outbox() )
            {
                if (theContentLister->itemCount > 0)
                {
                    CL_CONTROLPRINTF("prev item -- current %d itemCount %d",
                                     theContentLister->currentFocus, theContentLister->itemCount);
                    if (theContentLister->currentFocus == -1)
                    {
                        newFocus = 0;
                    }
                    else if (theContentLister->currentFocus == 0)
                    {
                        newFocus = theContentLister->itemCount - 1;
                    }
                    else
                    {
                        newFocus = theContentLister->currentFocus - 1;
                    }
                    ctrl_listItem_focus(newFocus, theContentLister);
                }
            }
            erbusy_off();
            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_F4:
            // long PREVIOUS_LINK_BUTTON keypress
            CL_CURSORPRINTF("long prev item -- current %d", theContentLister->currentFocus);
            if ( !ctrl_current_location_is_outbox() )
            {
                if (theContentLister->currentFocus != 0)
                {
                    ctrl_listItem_focus(0, theContentLister);
                }
            }
            erbusy_off();
            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_Down:
            if ( !ctrl_current_location_is_outbox() )
            {
                if (theContentLister->itemCount > 0)
                {
                    CL_CONTROLPRINTF("next item -- current %d itemCount %d",
                                     theContentLister->currentFocus, theContentLister->itemCount);
                    newFocus = theContentLister->currentFocus + 1;
                    if (newFocus >= theContentLister->itemCount)
                    {
                        newFocus = 0;
                    }
                    ctrl_listItem_focus(newFocus, theContentLister);
                }
            }
            erbusy_off();
            // return TRUE => stop event handling 
            returnValue = 1;
            break;

        case GDK_F3:
            // long NEXT_LINK_BUTTON keypress
            CL_CURSORPRINTF("long next item -- current %d", theContentLister->currentFocus);
            if ( !ctrl_current_location_is_outbox() )
            {
                if (theContentLister->currentFocus != (theContentLister->itemCount - 1))
                {
                    ctrl_listItem_focus(theContentLister->itemCount - 1, theContentLister);
                }
            }
            erbusy_off();
            // return TRUE => stop event handling 
            returnValue = 1;
            break;

#ifdef COMMAND_LINE_INPUT
        case GDK_F12:
            // test case 
            ctrl_contentType_button_clicked(st_ContentTypeBooks);
            break;
        case GDK_F11:
            // test case 
            ctrl_contentType_button_clicked(st_ContentTypeNews);
            break;
        case GDK_F10:
            // test case 
            ctrl_contentType_button_clicked(st_ContentTypeDocs);
            break;
        case GDK_F9:
            // test case 
            ctrl_contentType_button_clicked(st_ContentTypeNotes);
            break;
        case GDK_F8:
            // test case 
            ctrl_mode_button_clicked();
            break;
        case GDK_F7:
            // test case 
            ctrl_display_download_history();
            break;
#endif // COMMAND_LINE_INPUT

    }                           // end switch

    return returnValue;
}

void ctrl_goto_page(int pagenumber)
{
    CL_SCREENPRINTF("entry: pagenum [%d]", pagenumber);

    int startindex;
    int req_cnt = _G.pageItems;
    ContentLister *theContentLister = gContentLister;

    if (   getListerState() == STATE_MISC
        && GTK_WIDGET_VISIBLE(theContentLister->distListWnd))
    {
        dist_list_wnd_goto_page(pagenumber);
    }
    else if ((pagenumber > 0) && (pagenumber != theContentLister->currentPage)
             && (pagenumber <= theContentLister->pagecount))
    {
        theContentLister->currentPage = pagenumber;
        startindex = _G.pageItems * (pagenumber - 1);

        ctrl_select_stop(TRUE);

        // retrieve new page content
        mdsSetPageContent(theContentLister->items, startindex, &req_cnt);

        // update the stored index value
        mdsSetIndex(theContentLister->currentContentType, startindex);

        theContentLister->itemCount = req_cnt;
        theContentLister->currentFocus = 0;
        ctrl_display_item_view(theContentLister);
    }
    else
    {
        erbusy_off();
    }
}


int ctrl_goto_item(const gchar * szFilename)
{
    CL_SCREENPRINTF("entry: filename [%s]", szFilename);

    int startindex;
    int req_cnt = _G.pageItems;
    ContentLister *theContentLister = gContentLister;

    // find index for specified file
    int index = mdsGetItemIndex(szFilename);
    if (index < 0)
    {
        CL_WARNPRINTF("Cannot find filename [%s]", szFilename);
        index = 0;
    }
    // display and select the specified file
    int pagenumber = (index / _G.pageItems) + 1;
    int pageindex = index % _G.pageItems;
    if ((pagenumber > 0) && (pagenumber <= theContentLister->pagecount))
    {
        // select the correct page
        theContentLister->currentPage = pagenumber;
        startindex = _G.pageItems * (pagenumber - 1);

        // retrieve new page content
        mdsSetPageContent(theContentLister->items, startindex, &req_cnt);
        theContentLister->itemCount = req_cnt;

        // select the correct item on this page
        g_assert(pageindex < theContentLister->itemCount);
        theContentLister->currentFocus = pageindex;
    }
    // and finally display this page
    ctrl_display_item_view(theContentLister);
    return 0;
}

static gboolean ctrl_main_window_expose_event(GtkWidget * widget, GdkEventExpose * event, ContentLister * theContentLister)
{
    CL_SCREENPRINTF("entry");

    display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL);
    return FALSE;
}

// an X window for this main window widget was created
static void ctrl_main_window_realize_event(GtkWidget * widget, ContentLister * theContentLister)
{
    Window xwin;
    static int count = 0;

    CL_SCREENPRINTF("entry");

    if (count == 0)
    {
        // init the program manager
        xwin = GDK_WINDOW_XID(widget->window);
        CL_SCREENPRINTF("Toplevel XWindow: 0x%x", (int)xwin);

        pm_SetMyWindow(xwin);

        count = 1;
    }
}

// rescan current contentype and dir,for application quited initiatively.
void ctrl_rescan_current()
{
    CL_CONTROLPRINTF("entry: currentError [%d]", gContentLister->currentError);

    if (gContentLister->currentError == ctrlNoError)
    {
        CL_CONTROLPRINTF("rescan folder only, current content type = %d", gContentLister->currentContentType);
        ctrl_scan_content(gContentLister, ctrlScanCurrent, NULL);
    }
}


void ctrl_contentType_button_clicked(st_ContentType_e theContentType)
{
    ContentLister *theContentLister = gContentLister;
    clDisplayItem_t theItem;
    const gboolean cl_OnTop = (strcmp(pm_getUaOnTop(), CONTENTLISTER_UAID) == 0) ? TRUE : FALSE;
    const gboolean dm_OnTop = (strcmp(pm_getUaOnTop(), PROFILES_APP      ) == 0) ? TRUE : FALSE;
    char *location;

    // check whether contentlister is visible
    CL_CONTROLPRINTF("entry: theContentLister [%p] type [%d] UaOnTop [%s]", theContentLister, theContentType, pm_getUaOnTop());
    if ( !cl_OnTop )
    {
        // contentLister not "On Top" => scan current content and show contentlister 
        CL_CONTROLPRINTF("CL NOT on top - current type was = %d", theContentLister->currentContentType);

        theContentLister->previousContentType = theContentLister->currentContentType;
        theContentLister->currentContentType = theContentType;
        // change acording to design, do not record history
        stackClear(theContentType);

        pm_RaiseContentLister();

        if (dm_OnTop)
        {
            // This scan_content only needed after download manager,
            // because the sigchld handler already rescans (see programMgr.c)
            ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
        }
    }
    else
    {
        // contentLister is "On Top"
        if (theContentType != theContentLister->currentContentType)
        {
            // contentLister "On Top" but different contentType => scan
            // new contentType 
            CL_CONTROLPRINTF
                ("CL on top - current type (%d) != theContentType (%d)",
                 theContentLister->currentContentType, theContentType);

            if (getListerState() == STATE_MISC)
            {
                ctrl_hide_misc_screen();
            }
            ctrl_select_stop(TRUE);

            theContentLister->previousContentType = theContentLister->currentContentType;
            theContentLister->currentContentType  = theContentType;
            // change acording to design, do not record history
            stackClear(theContentType);

            ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
        }
        else if (theContentLister->currentError != ctrlNoError)
        {
            CL_CONTROLPRINTF("currentError %d", theContentLister->currentError);
            ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
        }
        else if (g_select.active)
        {
            ctrl_select_stop(TRUE);
            toolbar_synchronise();
        }
        else if (getListerState() == STATE_MISC)
        {
            ctrl_hide_misc_screen();
        }
        else
        {
            CL_CONTROLPRINTF("CL on top - current type (%d) == theContentType", theContentLister->currentContentType);

            CL_HISTORYPRINTF("Goto last read content of the contentType %d", theContentType);

            hsGetItem(theContentType, &theItem);
            location = hsGetLocation(theContentType);

            CL_HISTORYPRINTF("hsGetLocation (for %d) returned %s",
                             theContentType, (location == NULL) ? "NULL" : location);

            if (ctrl_start_viewer(theContentLister->currentContentType, &theItem, location) == FALSE)
            {
                CL_CONTROLPRINTF("ctrl_start_viewer returned FALSE");
                ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
            }

            if (location)
            {
                g_free(location);
                location = NULL;
            }
        }
    }
}

void ctrl_connect_button_clicked(void)
{
    CL_LOGPRINTF("entry");

    // stop background connect, to avoid conflicts
    ctrl_disconnect_wait();

    // connect to iDS
    ctrl_connect(connectScrDownloadHistory, connect_manual);
}

void ctrl_connect(connectScr_e screen, e_ctrl_connect_reason_t reason)
{
    CL_LOGPRINTF("entry: reason [%d]", reason);

    gboolean use_last_connected  = FALSE;
    gboolean background          = FALSE;
    gboolean content_only        = FALSE;
    gchar*   content_uuid        = NULL;
    gboolean from_pincode_screen = FALSE;
    gboolean force_ids           = FALSE;
    gboolean after_reboot        = FALSE;

    switch (reason)
    {
        case connect_manual:
            break;
        case connect_background:
            use_last_connected  = TRUE;
            background          = TRUE;
            content_only        = TRUE;
            break;
        case connect_from_pincode:
            use_last_connected  = TRUE;
            content_only        = TRUE;
            content_uuid        = "reset-pin-code";
            from_pincode_screen = TRUE;
            force_ids           = TRUE;
            break;
        case connect_after_reboot:
            use_last_connected  = TRUE;
            force_ids           = TRUE;
            after_reboot        = TRUE;
            break;
        case connect_timed_ids_with_sw:
            force_ids           = TRUE;
            break;
        case connect_timed_ids_content_only:
            content_only        = TRUE;
            force_ids           = TRUE;
            break;
        case connect_timed_ids_background:
            use_last_connected  = TRUE;
            background          = TRUE;
            content_only        = TRUE;
            force_ids           = TRUE;
            break;
        default:
            CL_ERRORPRINTF("Illegal reason [%d]", reason);
            return;
    }

    if (    gContentLister->currentError == ctrlNoError
         && connectStruct.state          == connectStateIdle )
    {
        // Show toolbar icon "connected"
        toolbar_setIconState(iconID_connect, iconState_selected);

        // Put contentlister on top
        if ( background )
        {
            toolbar_synchronise();
        }
        else
        {
            const char* UaOnTop = pm_getUaOnTop();
            CL_CONTROLPRINTF("UaOnTop = %s", UaOnTop);
            if ((strncmp(UaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0))
            {
                // contentLister not "On Top" => show contentlister
                CL_CONTROLPRINTF("CL NOT on top - current type was = %d", gContentLister->currentContentType);
                pm_RaiseContentLister();
            }
            setListerState(STATE_DOWNLOADMGR);
            pm_setUaOnTop(PROFILES_APP);  // set connection mgr as application-on-top, so key events will be sent here
            toolbar_restore();
        }

        // Block USB connect while connectionMgr running
        erStopMSDiskApp();

        // Start the connectionMgr, 
        // who connects to the IDS server
        // end then starts the downloadMgr
        snprintf( connectStruct.szCommand,
                  sizeof(connectStruct.szCommand),
                  "%s%s%s%s%s%s%s%s",
                  CONNECTIONMGR_EXECUTABLE,
                  use_last_connected ? " --use-last-connected"   : "",
                  background         ? " --background"           : "",
                  content_only       ? " --content-only"         : "",
                  content_uuid       ? " --content-uuid "        : "",
                  content_uuid       ?   content_uuid            : "",
                  force_ids          ? " --connect-ids"          : "",
                  after_reboot       ? " --connect-after-reboot" : ""  );

        connectStruct.screen               = screen;
        connectStruct.background           = background;
        connectStruct.connect_from_pincode = from_pincode_screen;
        connectStruct.connect_after_reboot = after_reboot;
        connectStruct.command              = connectCmdStart;
        pthread_mutex_unlock( &(connectStruct.mutex) );
    }
    else
    {
        CL_SCREENPRINTF("cannot connect now");
        display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL);
    }
}

void ctrl_disconnect(connectScr_e screen)
{
    CL_LOGPRINTF("entry: screen [%d]", screen);
    
    if (connectStruct.state == connectStateConnected)
    {
        connectStruct.screen  = screen;
        connectStruct.command = connectCmdStop;
    }
}

void ctrl_disconnect_wait(void)
{
    CL_LOGPRINTF("entry");
    
    ctrl_disconnect(connectStruct.screen);

    while (connectStruct.state != connectStateIdle)
    {
        CL_WARNPRINTF("Waiting for connectStateIdle [%d], current state [%d]", connectStateIdle, connectStruct.state);
        sleep(1);
    }
}


void ctrl_mode_button_long_clicked(void)
{
    ctrl_display_recent_documents();
}

void ctrl_mode_button_clicked(void)
{
    ContentLister *theContentLister = gContentLister;
    char *UaOnTop;

    CL_CONTROLPRINTF("entry 0x%x", (unsigned int)theContentLister);

    UaOnTop = pm_getUaOnTop();

    CL_CONTROLPRINTF("UaOnTop = %s", UaOnTop);

    if ((strncmp(UaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0))
    {
        // contentLister not "On Top" => scan mode directory content and
        // show contentlister 
        CL_CONTROLPRINTF("CL NOT on top");
        theContentLister->previousContentType = theContentLister->currentContentType;
        theContentLister->currentContentType = st_MenuTypeMode;
        // reset the st_MenuTypeMode stack
        stackClear(theContentLister->currentContentType);

        pm_RaiseContentLister();

        // Disabled this scan_content, because the signal handler already
        // rescans (see programMgr.c)
        // ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
        CL_CONTROLPRINTF("Not scanning content, this will be done in the sigchld handler");
    }
    else
    {
        // contentLister "On Top" => scan new contentType 
        CL_CONTROLPRINTF("CL on top - current type (%d)", theContentLister->currentContentType);

        if ((theContentLister->currentError != ctrlNoError) && (stackIsStorage(theContentLister->currentContentType)))
        {
            CL_CONTROLPRINTF("Error screen for storage (%d)", theContentLister->currentContentType);
            ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
        }
        else
        {
            // back to the mode menu 
            CL_CONTROLPRINTF("Back to mode menu");
            ctrl_hide_misc_screen();
            theContentLister->previousContentType = theContentLister->currentContentType;
            theContentLister->currentContentType = st_MenuTypeMode;
            // reset the st_MenuType1 stack
            stackClear(theContentLister->currentContentType);
            ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
        }
    }
}

void ctrl_display_download_history()
{
    ContentLister *theContentLister = gContentLister;
    char *UaOnTop;

    CL_CONTROLPRINTF("entry 0x%x", (unsigned int)theContentLister);

    // back to the download history menu 
    CL_CONTROLPRINTF("Back to the download history menu");
    theContentLister->previousContentType = theContentLister->currentContentType;
    theContentLister->currentContentType = st_DownloadHistory;
    if (g_new_content_received)
    {
        g_new_content_acknowledged = TRUE;
    }

    ctrl_cleanup_download_history();

    // reset the st_DownloadHistory stack
    stackClear(theContentLister->currentContentType);
    // always display the first page
    mdsSetIndex(theContentLister->currentContentType, 0);
    ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);
    UaOnTop = pm_getUaOnTop();
    CL_CONTROLPRINTF("UaOnTop = %s", UaOnTop);
    if ((strncmp(UaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0))
    {
        // contentLister not "On Top" => show contentlister 
        CL_CONTROLPRINTF("CL NOT on top");
        pm_RaiseContentLister();
    }
}

void ctrl_cleanup_download_history(void)
{
    char   dirname[ERMDS_MAX_FILENAME_SIZE];

    if ( mdsGetRootLocation(st_DownloadHistory, dirname, sizeof(dirname)) > 0 )
    {
        // remove dangling symlinks
        hsCleanupSymlinks(dirname);

        // restrict the number of symlinks
        hsLimitSymlinks(dirname, MAX_DOWNLOAD_ITEMS);
    }
}

static void ctrl_start_application(gchar * szExtID, gchar* options)
{
    // avoid conflict between profile-editor/settings and background connection
    if (   strcmp(szExtID, PROFILES_APP) == 0
        || strcmp(szExtID, SETUP_APP)    == 0 )
    {
        ctrl_disconnect_wait();
        toolbar_setIconState(iconID_connect, iconState_grey);
    }

    // start application
    if (pm_RunViewer(szExtID, NULL, NULL, options, 0) != ERR_OK)
    {
        CL_ERRORPRINTF("Error running application [%s]", szExtID);
        erbusy_off();
    }
}

// temp used routine thes values should come from ???? in the future
static void ctrl_get_content_name(st_ContentType_e theType, char *name)
{
    if (name)
    {
        switch (theType)
        {
            case st_ContentTypeBooks:
                strcpy(name, _("Books"));
                break;
            case st_ContentTypeNews:
                strcpy(name, _("Newspapers"));
                break;
            case st_ContentTypeDocs:
                strcpy(name, _("Documents"));
                break;
            case st_ContentTypeNotes:
                strcpy(name, _("Notes"));
                break;
            case st_StorageTypeCF:
                strcpy(name, _("CF"));
                break;
            case st_StorageTypeSD:
                strcpy(name, _("MMC"));
                break;
            case st_StorageTypeUSB:
                strcpy(name, _("USB"));
                break;
            case st_StorageTypeMain:
                strcpy(name, _("Internal Memory"));
                break;
            case st_MenuTypeMode:
                strcpy(name, _("Device Manager"));
                break;
            case st_DownloadHistory:
                strcpy(name, _("Download History"));
                break;
            case st_SearchResult:
                strcpy(name, _("Search Results"));
                break;
            case st_RecentDocuments:
                strcpy(name, _("Recent Documents"));
                break;
            default:
                strcpy(name, _("Unknown"));
                break;
        }
    }
}


listerState_t getListerState()
{
    CL_CONTROLPRINTF("return [%d]", g_listerState);
    return g_listerState;
}

void setListerState(listerState_t newstate)
{
    CL_CONTROLPRINTF("entry: state old [%d] new [%d]", g_listerState, newstate);

    // need to rescan if state changes from STATE_USB_CONNECTED to normal
    if (STATE_USB_CONNECTED == g_listerState && STATE_NORMAL == newstate)
    {
        // set new state and rescan
        g_listerState = newstate; // set new state before rescan (ref. BT 802)
        CL_CONTROLPRINTF("rescan after disconnected from PC");
        ctrl_scan_content(gContentLister, ctrlScanCurrent, 0);
    }
    else
    {
        g_listerState = newstate;
    }
}

void listerShowUSBScreen()
{
    CL_CONTROLPRINTF("entry");

    if (get_exported_memory_type() == get_memory_type_used())
    {
        listerShowErrorScreen(ctrlConnectedUSBError);
    }
    else
    {
        listerShowErrorScreen(ctrlExportingDevUnavailError);
    }
}

void listerHideUSBScreen()
{
    CL_CONTROLPRINTF("entry");

    if (gContentLister)
    {
        if (gContentLister->currentError == ctrlNoError)
        {
            listerHideErrorScreen();
        }
        else
        {
            listerShowErrorScreen(gContentLister->currentError);
        }
    }
}

void ctrl_hide_misc_screen()
{
    CL_CONTROLPRINTF("entry");

    if (getListerState() == STATE_MISC)
    {
        ctrl_metadata_editor_close(FALSE);
        ctrl_hide_search_wnd();
        ctrl_hide_sort_wnd(FALSE);
        ctrl_hide_sel_list_wnd(gContentLister, FALSE);
    }
    else
    {
        ctrl_select_stop(TRUE);
    }
}

void ctrl_battery_low_screen(gint percentage, gint time_left, gboolean shutdown)
{
    static int ignore_count = 0;

    CL_CONTROLPRINTF("entry: percentage [%d] time_left [%d]", percentage, time_left);

    // We don't do anything with the battery status at the moment...

    if (shutdown && g_errorTypeDisplayed == ctrlBatteryLowError)
    {
        ctrl_shutdown();
    }
    else if (   ctrl_is_connect_from_pincode()
             && ignore_count < 2               ) 
    {
        ignore_count++;
    }
    else
    {
        ignore_count = 0;
        listerShowErrorScreen(ctrlBatteryLowError);
    }
}

gboolean listerIsErrorScreenVisible()
{
    gboolean ret = FALSE;

    if (gContentLister->errorScreen)
    {
        ret = GTK_WIDGET_VISIBLE(gContentLister->errorScreen);
    }

    CL_CONTROLPRINTF("return [%d]", ret);
    return ret;
}

void ctrl_usbmode_keypress(void)
{
    CL_CONTROLPRINTF("entry");

    ctrl_errorScreen_buttonpress(NULL, NULL, gContentLister);
}

static guint ctrl_errorScreen_buttonpress(GtkWidget*      widget_notused,
                                          GdkEventButton* event_notused,
                                          ContentLister*  theContentLister_notused)
{
    CL_CONTROLPRINTF("entry");

    if (   g_errorTypeDisplayed == ctrlConnectedUSBError
        || g_errorTypeDisplayed == ctrlSystemShutdownError )
    {
        CL_WARNPRINTF("Ignore button-press while USB or shutdown screen shown");
        return 1;               // TRUE => no further processing
    }

    erbusy_blink();

    if (g_shutdownPending && g_errorTypeDisplayed == ctrlBatteryLowError && is_battery_charging() == FALSE)
    {
        ctrl_shutdown();
    }
    else if (g_listerState == STATE_USB_CONNECTED)
    {
        listerShowErrorScreen(ctrlConnectedUSBError);
    }
    else
    {
        listerHideErrorScreen();
    }

    return 0;                   // FALSE => default GTK handling
}


gboolean idle_CheckBattery_HideErrorScreen(gpointer data)
{
    if ( is_battery_charging() )
    {
        ctrl_errorScreen_buttonpress(NULL, NULL, NULL);
        g_idle_check_battery_id = 0;
        return FALSE;  // stop idle handler
    }
    else
    {
        return TRUE;  // continue idle handler
    }
}

// analyse current implemention:
// 1.there's no way to show pincode screen when error screen is visible,
//   for error screen is full and we have no automatic screen lock yet
// 2.but it's possible to show error screen when pincode screen is visible
// 3.and no way to change contentLister state when pincode screen is visible except users 
//   press CONNECT LONG. for pincode is visible, no way to connect USB and no way to search or rename.
//
// how to deal with error screen together with pincode screen as below:
// 1. in ctrl_shutdown, first ctrl_hide_pincode_screen, then listerShowErrorScreen()
// 2. in listerShowErrorScreen, hide pincode screen before show error screen
// 3. in listerHideErrorScreen, check if connect from pincode screen, if yes we set contentLister state to STATE_PINCODE 
//    after that, check if contentLister state is STATE_PINCODE, if yes, back to pincode screen, otherwise show listerArea
// 4. in button_handler, 
//    if it's in download state, if launched connect from pincode, block all keys 
//    if it's in pincode state, first hide error screen,  afterwards block all keys except CONNECT LONG or OVERVIEW
//
void listerShowErrorScreen(ctrlErrorType_e errorType)
{
    char *UaOnTop;

    CL_SCREENPRINTF("entry: errorType [%d]", errorType);

    if (gContentLister)
    {
        toolbar_disableUpdate();

        // abort miscellaneous contentLister windows
        ctrl_hide_misc_screen();
        toolbar_setIconState(iconID_search,   iconState_grey);
        toolbar_setIconState(iconID_sort,     iconState_grey);
        toolbar_setIconState(iconID_share,    iconState_grey);
        toolbar_setIconState(iconID_tagging,  iconState_grey);
        toolbar_setIconState(iconID_trashcan, iconState_grey);

        toolbar_setIconState(iconID_rate_a,   iconState_grey  );
        toolbar_setIconState(iconID_rate_aa,  iconState_grey  );
        toolbar_setIconState(iconID_rate_aaa, iconState_grey  );

        // hide keyboard
        ctrl_hide_keyboard(iconState_grey);
        toolbar_enableUpdate();

        // bring contentlister on top
        UaOnTop = pm_getUaOnTop();
        strncpy(restoreUaOnTop, UaOnTop, sizeof(restoreUaOnTop));
        if ((strncmp(UaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0))
        {
            display_update_increase_level(NO_DISPLAY_UPDATE_LEVEL);
            CL_SCREENPRINTF("restoreUaOnTop=%s", restoreUaOnTop);
            pm_RaiseContentLister();
        }

        const char* card_name = NULL;
        char buf[ERROR_STRING_BUFLEN];
        switch (errorType)
        {
            case ctrlConnectedUSBError:
                gtk_error_screen_set_text(GTK_ERROR_SCREEN(gContentLister->errorScreen), _("Connected to PC..."));
                gtk_error_screen_set_image(GTK_ERROR_SCREEN(gContentLister->errorScreen), ctrlDontDisconnectNWIcon);
                break;

            case ctrlExportingDevUnavailError:
                switch (get_exported_memory_type())
                {
                    case expMMC:
                        card_name = _("MMC card");
                        break;
                    case expUSB:
                        card_name = _("USB stick");
                        break;
                    case expCF:
                        card_name = _("CF card");
                        break;
                    default:
                        break;
                }
                snprintf(buf, ERROR_STRING_BUFLEN, _("Connected to PC.\nThe %s storage is not available, Internal Memory is exported instead."), card_name);

                gtk_error_screen_set_text(GTK_ERROR_SCREEN(gContentLister->errorScreen), buf);
                gtk_error_screen_set_image(GTK_ERROR_SCREEN(gContentLister->errorScreen), ctrlWarningIcon);
                break;

            case ctrlBatteryLowError:
                gtk_error_screen_set_text(GTK_ERROR_SCREEN(gContentLister->errorScreen),
                        _("Your battery is almost empty, please connect the power adapter to keep working."));
                gtk_error_screen_set_image(GTK_ERROR_SCREEN(gContentLister->errorScreen), ctrlBatteryLowIcon);
                g_idle_check_battery_id = g_idle_add(idle_CheckBattery_HideErrorScreen, NULL);
                break;

            case ctrlSystemShutdownError:
                gtk_error_screen_set_text(GTK_ERROR_SCREEN(gContentLister->errorScreen),
                        _("The iLiad is preparing for power down."));
                gtk_error_screen_set_image(GTK_ERROR_SCREEN(gContentLister->errorScreen), ctrlSystemShutdownIcon);
                break;

            case ctrlScanTooManyItemsError:
                gtk_error_screen_set_text(GTK_ERROR_SCREEN(gContentLister->errorScreen),
                        _("There are too many items to display. Only the first 1000 items will be listed."));
                gtk_error_screen_set_image(GTK_ERROR_SCREEN(gContentLister->errorScreen), ctrlWarningIcon);
                break;

            case ctrlScanFailedError:
                gtk_error_screen_set_text(GTK_ERROR_SCREEN(gContentLister->errorScreen),
                        _("Reading of content has failed due to an unknown reason."));
                gtk_error_screen_set_image(GTK_ERROR_SCREEN(gContentLister->errorScreen), ctrlErrorIcon);
                break;

            case ctrlContentNotAvailableError:
                gtk_error_screen_set_text(GTK_ERROR_SCREEN(gContentLister->errorScreen),
                        _("Content isn't accessible any more."));
                gtk_error_screen_set_image(GTK_ERROR_SCREEN(gContentLister->errorScreen), ctrlInfoIcon);
                break;

            case ctrlNoNetworkConfiguredError:
                gtk_error_screen_set_text(GTK_ERROR_SCREEN(gContentLister->errorScreen),
                        _("No network configured."));
                gtk_error_screen_set_image(GTK_ERROR_SCREEN(gContentLister->errorScreen), ctrlErrorIcon);
                break;

            default:
                gtk_error_screen_set_text(GTK_ERROR_SCREEN(gContentLister->errorScreen),
                        _("An unknown event occurred."));
                gtk_error_screen_set_image(GTK_ERROR_SCREEN(gContentLister->errorScreen), ctrlInfoIcon);
        }
        g_errorTypeDisplayed = errorType;

        if (listerIsErrorScreenVisible() == FALSE)
        {
            // => only request a refresh on the main window expose event
            CL_SCREENPRINTF("Display error screen");
            if (display_update_get_level() == NO_DISPLAY_UPDATE_LEVEL)
            {
                display_update_decrease_level(MAIN_WINDOW_EXPOSE_LEVEL);
            }
            else
            {
                display_update_increase_level(MAIN_WINDOW_EXPOSE_LEVEL);
            }
            gtk_window_fullscreen(GTK_WINDOW(gContentLister->topLevelWindow));
            gtk_widget_hide(GTK_WIDGET(gContentLister->listerScreen ));
            gtk_widget_show(GTK_WIDGET(gContentLister->errorScreen  ));
            gtk_widget_hide(GTK_WIDGET(gContentLister->pincodeScreen));
            gtk_widget_grab_focus(GTK_WIDGET(gContentLister->errorScreen));
        }
    }
}

void listerHideErrorScreen()
{
    CL_CONTROLPRINTF("entry");

    if (gContentLister && listerIsErrorScreenVisible())
    {
        // hide error screen
        // => only request a refresh on the main window expose event
        CL_SCREENPRINTF("");
        display_update_increase_level(MAIN_WINDOW_EXPOSE_LEVEL);
        
        if (ctrl_is_connect_from_pincode())
        {
            setListerState(STATE_PINCODE);
        }

        if (STATE_PINCODE == getListerState())
        {
            CL_WARNPRINTF("hide error, show pincode");
            gtk_window_fullscreen(GTK_WINDOW(gContentLister->topLevelWindow));
            gtk_widget_hide(GTK_WIDGET(gContentLister->listerScreen ));
            gtk_widget_hide(GTK_WIDGET(gContentLister->errorScreen  ));
            gtk_widget_show(GTK_WIDGET(gContentLister->pincodeScreen));
            gtk_widget_grab_focus(GTK_WIDGET(gContentLister->pincodeScreen));
        }
        else
        {
            CL_WARNPRINTF("hide error, show listArea");
            gtk_window_unfullscreen(GTK_WINDOW(gContentLister->topLevelWindow));
            gtk_widget_show(GTK_WIDGET(gContentLister->listerScreen ));
            gtk_widget_hide(GTK_WIDGET(gContentLister->errorScreen  ));
            gtk_widget_hide(GTK_WIDGET(gContentLister->pincodeScreen));
            gtk_widget_grab_focus(GTK_WIDGET(gContentLister->listerArea));
        }
        g_errorTypeDisplayed = ctrlNoError;
        
        if ((strncmp(restoreUaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0))
        {
            pm_RaiseUserApp(restoreUaOnTop);
            strncpy(restoreUaOnTop, CONTENTLISTER_UAID, sizeof(restoreUaOnTop));
        }

        if (g_idle_check_battery_id > 0)
        {
            g_source_remove(g_idle_check_battery_id);
            g_idle_check_battery_id = 0;
        }
    }
}


/////// select one of the currently displayed contentLister items //////////
//
static void ctrl_select_stop(gboolean restore_toolbar)
{
    int      i;
    gboolean selected;

    CL_CONTROLPRINTF("entry");

    if (g_select.active)
    {
        // stop time-out
        gtk_timeout_remove(g_select.timerID);
        g_select.active = FALSE;

        // hide all cursors, stop blink
        cursor_hide_all();

        if (restore_toolbar)
        {
            // if nothing selected show cursor on first item
            selected = FALSE;
            for (i = 0 ; !selected && i < _G.pageItems; i++)
            {
                if (g_select.item_selected[i])
                {
                    ctrl_listItem_focus_no_redraw(i, gContentLister);
                    selected = TRUE;
                }
            }
            if ( !selected )
            {
                ctrl_listItem_focus_no_redraw(0, gContentLister);
            }

            // update toolbar
            toolbar_disableUpdate();
            if ( ctrl_current_location_is_empty() )
            {
                toolbar_setIconState(iconID_search,      iconState_grey);
                toolbar_setIconState(iconID_sort,        iconState_grey);
                toolbar_setIconState(iconID_share,       iconState_grey);
                toolbar_setIconState(iconID_tagging,     iconState_grey);
                toolbar_setIconState(iconID_trashcan,    iconState_grey);

                toolbar_setIconState(iconID_rate_a,   iconState_grey  );
                toolbar_setIconState(iconID_rate_aa,  iconState_grey  );
                toolbar_setIconState(iconID_rate_aaa, iconState_grey  );
            }
            else
            {
                if (ctrl_current_location_is_outbox())
                {
                    toolbar_setIconState(iconID_search,   iconState_grey  );
                    toolbar_setIconState(iconID_sort,     iconState_grey  );
                    toolbar_setIconState(iconID_share,    iconState_grey  );
                    toolbar_setIconState(iconID_tagging,  iconState_grey  );
                    toolbar_setIconState(iconID_trashcan, iconState_normal);

                    toolbar_setIconState(iconID_rate_a,   iconState_grey  );
                    toolbar_setIconState(iconID_rate_aa,  iconState_grey  );
                    toolbar_setIconState(iconID_rate_aaa, iconState_grey  );
                }
                else
                {
                    toolbar_setIconState(iconID_search,   iconState_normal);
                    toolbar_setIconState(iconID_sort,     iconState_normal);
                    toolbar_setIconState(iconID_share,    iconState_normal);
                    toolbar_setIconState(iconID_tagging,  iconState_normal);
                    toolbar_setIconState(iconID_trashcan, iconState_normal);

                    toolbar_setIconState(iconID_rate_a,   iconState_normal);
                    toolbar_setIconState(iconID_rate_aa,  iconState_normal);
                    toolbar_setIconState(iconID_rate_aaa, iconState_normal);
                }
            }
            toolbar_enableUpdate();
            toolbar_synchronise();
        }
    }
}

static gboolean ctrl_on_select_timer(gpointer data)
{
    int      i;
    int      item_idx[MAX_ITEMS_ON_ONE_PAGE + 1];
    int*     ip;
    gboolean item_selected = FALSE;
    gboolean call_me_again = FALSE;

    CL_CONTROLPRINTF("entry");

    if (g_select.active)
    {
        if (g_select.icon_clicked)
        {
            // listitem select procedure ends
            if (g_select.on_item_selected != NULL)
            {
                // report the currently selected item(s)
                ip = item_idx;
                for (i = _G.pageItems - 1 ; i >= 0 ; i--)
                {
                    if (g_select.item_selected[i])
                    {
                        item_selected = TRUE;
                        *ip = i;
                        ip++;
                    }
                }
                *ip = -1;
                if (item_selected)
                {
                    // start the registered function, i.e. callback function
                    g_select.on_item_selected(item_idx, gContentLister);
                    ctrl_select_stop(g_select.confirm_with_icon);
                }
                else
                {
                    // no items selected: abort select mode
                    ctrl_select_stop(TRUE);
                    toolbar_synchronise();
                }
            }
            else
            {
                // no callback registered: abort select mode
                ctrl_select_stop(TRUE);
                toolbar_synchronise();
            }
        }
        else if (g_select.ticks > 0)
        {
            // decrement time-out counter
            g_select.ticks--;
            call_me_again = TRUE;
        }
        else
        {
            // abort select mode and show cursor at the top-most contentlister item
            ctrl_select_stop(TRUE);
            toolbar_synchronise();
        }
    }

    return call_me_again;
}

void ctrl_select_listitem(int iconID, gboolean confirm_with_icon, on_item_selected_t* callback_on_selected)
{
    int i;

    CL_CONTROLPRINTF("entry: iconID [%d]", iconID);
    g_assert(callback_on_selected != NULL);

    // abort (background) connect, as this may interfere
    ctrl_disconnect_wait();

    // remember the settings
    g_select.iconID            = iconID;
    g_select.on_item_selected  = callback_on_selected;
    g_select.confirm_with_icon = confirm_with_icon;

    // update toolbar
    toolbar_disableUpdate();
    toolbar_setIconState(iconID_search,   iconState_grey);
    toolbar_setIconState(iconID_sort,     iconState_grey);
    toolbar_setIconState(iconID_share,    iconState_grey);
    toolbar_setIconState(iconID_tagging,  iconState_grey);
    toolbar_setIconState(iconID_trashcan, iconState_grey);

    toolbar_setIconState(iconID_rate_a,   iconState_grey);
    toolbar_setIconState(iconID_rate_aa,  iconState_grey);
    toolbar_setIconState(iconID_rate_aaa, iconState_grey);

    toolbar_setIconState(iconID, iconState_selected);
    toolbar_enableUpdate();
    
    // de-select contentlister items
    ctrl_listItem_focus(-1, gContentLister);
    for (i = 0 ; i < _G.pageItems; i++)
    {
        g_select.item_selected[i] = FALSE;
    }
    
    // start time-out
    g_select.ticks        = ITEM_SELECT_TIMEOUT_TICKS;
    g_select.timerID      = g_timeout_add(500, ctrl_on_select_timer, 0);
    g_select.icon_clicked = FALSE;
    g_select.active       = TRUE;

    // update screen when toolbar icons ok
    toolbar_synchronise();
}

//////// end select ////////


void ctrl_on_icon_clicked(int iconID, int iconState)
{
    CL_TBPRINTF("entry: iconID [%d] iconState [%d]", iconID, iconState);

    if (   g_select.active
        && iconID    == g_select.iconID
        && iconState == iconState_selected)
    {
        // icon clicked again -> listitem select procedure ends
        // Note: ipc messages like toolbar icon clicked are received on a separate thread, therefore the current
        //       function is NOT called on the GTK thread, so the order of events cannot be guaranteed.
        //       More precisely: pen-click listitem, pen-click toolbar icon sometimes is received in the inverse order.
        //       To avoid these issues we set a flag, which is checked by the timer function who then ends the select.
        g_select.icon_clicked = TRUE;
    }
    else
    {
        // icon actions regardless the icon state
        switch (iconID)
        {
            case iconID_search:
                ctrl_on_search_clicked(iconState);
                break;

            case iconID_sort:
                ctrl_on_sort_clicked(iconState);
                break;

            case iconID_locked:
                switch (iconState)
                {
                    case iconState_normal:
                        toolbar_setIconState(iconID_locked, iconState_selected);
                        toolbar_synchronise();
                        break;
                    case iconState_selected:
                        toolbar_setIconState(iconID_locked, iconState_normal);
                        toolbar_synchronise();
                        break;
                    case iconState_grey:
                        // ignore
                        break;
                    default:
                        CL_ERRORPRINTF("iconID_locked: state [%d] unknown", iconState);
                }
                break;

            case iconID_lock_screen:
                ctrl_on_lock_screen_clicked(iconState);
                break;

            case iconID_connect:
                switch (iconState)
                {
                    case iconState_normal:
                    case iconState_IDconnect_time:
                        ctrl_background_connect_timeout_start_withinterval(FALSE, TRUE, 0);
                        break;
                    case iconState_selected:
                        ctrl_disconnect(connectScrUnchanged);
                        break;
                    case iconState_IDconnect_newcontent:
                        ctrl_display_download_history();
                        break;
                    case iconState_IDconnect_newcontent_selected:
                        g_new_content_received = FALSE;
                        ctrl_set_connect_icon();
                        toolbar_synchronise();
                        break;
                    default:
                        CL_ERRORPRINTF("iconID_connect state [%d] unknown", iconState);
                }
                break;

            default:
                break; //ignore
        }

        // icon actions for icon state normal
        if (iconState == iconState_normal)
        {
            switch (iconID)
            {
                case iconID_share:
                    ctrl_select_listitem(iconID_share, FALSE, ctrl_listItem_share);
                    break;

                case iconID_tagging:
                    ctrl_select_listitem(iconID_tagging, FALSE, ctrl_listItem_edit);
                    break;
                
                case iconID_trashcan:
                    ctrl_select_listitem(iconID_trashcan, TRUE, ctrl_listItem_delete);
                    break;

                case iconID_rate_a:
                    ctrl_select_listitem(iconID_rate_a, TRUE, ctrl_listItem_rate_a);
                    break;
                case iconID_rate_aa:
                    ctrl_select_listitem(iconID_rate_aa, TRUE, ctrl_listItem_rate_aa);
                    break;
                case iconID_rate_aaa:
                    ctrl_select_listitem(iconID_rate_aaa, TRUE, ctrl_listItem_rate_aaa);
                    break;

                default:
                    break;  // ignore
            }
        }

        // icon actions for icon state selected
        if (iconState == iconState_selected)
        {
            switch (iconID)
            {
                case iconID_share:
                    ctrl_doc_share_now(gContentLister);
                    break;

                case iconID_tagging:
                    ctrl_metadata_editor_close(TRUE);
                    break;

                default:
                    break;  // ignore
            }
        }
    }
}


static void ctrl_add_on_toolbar_sync_action(on_tbsync_action_t action)
{
    int i;

    CL_LOGPRINTF("entry");
    g_assert(action != NULL);

    for (i = 0; i < sizeof(on_toolbar_sync_actions) / sizeof(on_toolbar_sync_actions[0]); i++)
    {
        if (on_toolbar_sync_actions[i] == NULL)
        {
            on_toolbar_sync_actions[i] = action;
            return;
        }
    }

    g_assert_not_reached();
}

void ctrl_on_toolbar_synchronised()
{
    int i;

    CL_IPCPRINTF("entry");

    display_update_request_screen_refresh(TOOLBAR_UPDATE_LEVEL);

    // call functions for registered on_toolbar_sync_actions
    for (i = 0; i < sizeof(on_toolbar_sync_actions) / sizeof(on_toolbar_sync_actions[0]); i++)
    {
        if (on_toolbar_sync_actions[i] != NULL)
        {
            on_toolbar_sync_actions[i] ();
            on_toolbar_sync_actions[i] = NULL;
        }
    }
}

void ctrl_show_lister_area(void)
{
    if (gContentLister->listerArea)
    {
        gtk_widget_show(gContentLister->listerArea);
    }
}

void ctrl_hide_lister_area(void)
{
    if (gContentLister->listerArea)
    {
        gtk_widget_hide(gContentLister->listerArea);
    }
}


// ////////////////////////////////////////////////////////////////////////
// keyboard handling

// show keyboard:
// wait for GTK objects updated then show keyboard
// block display updates while updating GTK objects, rely on keyboard
// display update
static void ctrl_show_keyboard()
{
    erbusy_blink();

    // block screen updates until keyboard shows
    display_update_increase_level(KEYBOARD_SHOW_HIDE_LEVEL);

    // update GTK objects then show keyboard, rely on display update by
    // keyboard
    g_idle_add(ctrl_on_idle_show_keyboard, NULL);
}

static gboolean ctrl_on_idle_show_keyboard(gpointer data)
{
    // request keyboard but first disable keyboard,
    // this makes sure the keyboard (re-)appears and does a display update
    toolbar_setIconState(iconID_keyboard, iconState_normal);
    toolbar_setIconState(iconID_keyboard, iconState_selected);

    // allow display updates again
    display_update_decrease_level(LOWEST_LEVEL);

    return FALSE;  // don't call me again
}

// hide keyboard:
// wait for GTK objects updated then show keyboard
// block display updates while updating GTK objects, rely on keyboard
// display update
static void ctrl_hide_keyboard(int iconState)
{
    g_assert((iconState == iconState_grey) || (iconState == iconState_normal));

    erbusy_blink();

    // block screen updates until keyboard hides
    display_update_increase_level(KEYBOARD_SHOW_HIDE_LEVEL);

    // update GTK objects then hide keyboard, rely on display update by
    // keyboard
    g_idle_add(ctrl_on_idle_hide_keyboard, (gpointer) iconState);
}

static gboolean ctrl_on_idle_hide_keyboard(gpointer data)
{
    int iconState = (int)data;

    // hide keyboard but first show keyboard,
    // this makes sure the keyboard (re-)disappears and does a display
    // update
    toolbar_setIconState(iconID_keyboard, iconState_selected);
    toolbar_setIconState(iconID_keyboard, iconState);

    display_update_request_screen_refresh(KEYBOARD_SHOW_HIDE_LEVEL);

    return FALSE;  // don't call me again
}


// ////////////////////////////////////////////////////////////////////////
// for content metadata editor

// close metadata edtior, save them if bSave == TRUE
// otherwise ignore change
static void ctrl_metadata_editor_close(gboolean bSave)
{
    CL_CONTROLPRINTF("entry");

    gchar szContainerDir[ERMDS_MAX_FILENAME_SIZE];

    if ( (getListerState() == STATE_MISC) 
          && (gContentLister->contentEditWnd)
          && GTK_WIDGET_VISIBLE(gContentLister->contentEditWnd) )
    {
        erbusy_blink();
        ctrl_hide_keyboard(iconState_grey);

        // store modified metadata
        if (bSave)
        {
            // rename container as needed
            content_metadata_rename_container(g_md_editor_mgr.item->szManifest, szContainerDir, sizeof(szContainerDir));

            // update manifest file
            content_metadata_write_manifest(szContainerDir);

            // show updated item on screen
            ctrl_rescan_current();
            ctrl_goto_item(szContainerDir);

            // update the stored index value
            int itemIndex = ctrl_get_item_index(gContentLister);
            mdsSetIndex(gContentLister->currentContentType, itemIndex);
        }
        else
        {
            // re-display all items
            ctrl_rescan_current();
        }

        // show contentlister page
        gtk_widget_hide(GTK_WIDGET(gContentLister->contentEditWnd));
        gtk_widget_grab_focus(GTK_WIDGET(gContentLister->listerArea));
        setListerState(STATE_NORMAL);

        toolbar_synchronise();
    }

    CL_CONTROLPRINTF("leave");
}

// edit content attributes
static void ctrl_listItem_edit(const int* index_tbl, ContentLister* theContentLister)
{
    const int  index = *index_tbl;  // only one item expected

    CL_CONTROLPRINTF("entry");

    if (getListerState() == STATE_MISC)
    {
        // already editing an item: ignore
        return;
    }

    clDisplayItem_t *theItem = &theContentLister->items[index];
    if (theItem->modifyEnable.bTagging == FALSE)
    {
        CL_WARNPRINTF("Rename disabled by manifest: item [%s]", theItem->szFilename);
        goto error_exit;
    }

    // check current item state 
    switch (theItem->fit)
    {
        case mdsFitFile:
            CL_CONTROLPRINTF("File [%s]", theItem->szFilename);
            int i = ctrl_before_file_clicked(theItem);  // create manifest file for it
            if (i != 0)
            {
                CL_ERRORPRINTF("ctrl_before_file_clicked failed for [%s]", theItem->szFilename);
                toolbar_setIconState(iconID_tagging, iconState_normal);
                toolbar_synchronise();
                return;
            }
            break;

        case mdsFitContainer:
            CL_CONTROLPRINTF("Container [%s]", theItem->szFilename);
            break;

        case mdsFitFolder:
        case mdsFitApplication:
        case mdsFitManifestDirectory:
        case mdsFitStorage:
        default:
            CL_WARNPRINTF("Cannot rename item [%s]", theItem->szFilename);
            goto error_exit;
    }

    // update toolbar, show keyboard
    toolbar_setIconState(iconID_tagging, iconState_selected);
    ctrl_show_keyboard();

    // remember the item we are editing
    g_md_editor_mgr.item = &(theContentLister->items[index]);

    // get manifest data
    content_metadata_read_manifest(theItem->szManifest);

    // update pagebar. maybe it should be moved into gtkMetadataWnd, multi 
    // pages? 
    pagebar_set_pagecount(1);
    pagebar_goto_page(1);
    pagebar_redraw();

    // hide not-selected lister items, then show edit screen
    int i;
    GtkWidget *listItem;
    for (i = 0; i < _G.pageItems; i++)
    {
        if (i != index)
        {
            listItem = lsGetListerItem(theContentLister->lister, i);
            gtk_widget_hide(listItem);
        }
    }
    gtk_widget_show(GTK_WIDGET(theContentLister->contentEditWnd));

    // remember contentLister state
    setListerState(STATE_MISC);
    return;

error_exit:
    ctrl_select_stop(TRUE);
    return;
}


// ////////////////////////////////////////////////////////////////////////
// for searching
// 
void ctrl_show_search_wnd_trigger(void)
{
    g_timeout_add(500, ctrl_show_search_wnd, NULL);
}

static gboolean ctrl_show_search_wnd(gpointer data)
{
    gboolean nRet = FALSE;      // don't me call again

    CL_SEARCHPRINTF("entry");

    if (getListerState() == STATE_MISC)
    {
        // already searching: ignore
        return nRet;
    }
    // update state and ui
    erbusy_blink();
    setListerState(STATE_MISC);

    // update toolbar, show keyboard
    toolbar_disableUpdate();
    toolbar_setIconState(iconID_search,   iconState_selected);
    toolbar_setIconState(iconID_sort,     iconState_grey    );
    toolbar_setIconState(iconID_share,    iconState_grey    );
    toolbar_setIconState(iconID_tagging,  iconState_grey    );
    toolbar_setIconState(iconID_trashcan, iconState_grey    );
    ctrl_show_keyboard();
    toolbar_enableUpdate();

    // update pagebar. maybe it should be moved into gtkSearchWnd, multi
    // pages? 
    pagebar_set_pagecount(1);
    pagebar_goto_page(1);
    pagebar_redraw();

    // hide lister items, then show edit screen
    int i;
    GtkWidget *listItem;
    for (i = 0; i < _G.pageItems; i++)
    {
        listItem = lsGetListerItem(gContentLister->lister, i);
        gtk_widget_hide(listItem);
    }
    gtk_widget_show(GTK_WIDGET(gContentLister->searchWnd));

    return nRet;
}

void ctrl_hide_search_wnd(void)
{
    CL_SEARCHPRINTF("entry");

    if ( (getListerState() == STATE_MISC) 
         && (gContentLister->searchWnd)
         && GTK_WIDGET_VISIBLE(gContentLister->searchWnd) )
    {
        // update toolbar, hide keyboard
        toolbar_disableUpdate();
        toolbar_setIconState(iconID_search,   iconState_normal);
        toolbar_setIconState(iconID_sort,     iconState_normal);
        toolbar_setIconState(iconID_share,    iconState_normal);
        toolbar_setIconState(iconID_tagging,  iconState_normal);
        toolbar_setIconState(iconID_trashcan, iconState_normal);
        ctrl_hide_keyboard(iconState_grey);
        toolbar_enableUpdate();

        // show contentlister page
        gtk_widget_hide(GTK_WIDGET(gContentLister->searchWnd));
        gtk_widget_grab_focus(GTK_WIDGET(gContentLister->listerArea));
        setListerState(STATE_NORMAL);
        ctrl_rescan_current();
    }
}

void ctrl_on_search_clicked(int iconState)
{
    switch (iconState)
    {
        case iconState_normal:
            if (getListerState() == STATE_NORMAL)
            {
                // de-select contentlister items
                ctrl_listItem_focus(-1, gContentLister);

                // update toolbar, show search window after toolbar updated
                toolbar_disableUpdate();
                toolbar_setIconState(iconID_search,   iconState_selected);
                toolbar_setIconState(iconID_sort,     iconState_grey    );
                toolbar_setIconState(iconID_share,    iconState_grey    );
                toolbar_setIconState(iconID_tagging,  iconState_grey    );
                toolbar_setIconState(iconID_trashcan, iconState_grey    );
                ctrl_add_on_toolbar_sync_action(ctrl_show_search_wnd_trigger);
                toolbar_enableUpdate();
                toolbar_synchronise();
            }
            break;

        case iconState_selected:
            ctrl_hide_search_wnd();
            break;

        default:
            /*
             * ignore 
             */ ;
    }
}

void ctrl_display_search_result()
{
    CL_SEARCHPRINTF("entry");

    gContentLister->previousContentType = gContentLister->currentContentType;
    gContentLister->currentContentType = st_SearchResult;

    // reset the st_SearchResult stack, always display the first page
    stackClear(gContentLister->currentContentType);
    mdsSetIndex(gContentLister->currentContentType, 0);
    ctrl_scan_content(gContentLister, ctrlScanCurrent, NULL);

    char *UaOnTop = pm_getUaOnTop();
    CL_SEARCHPRINTF("UaOnTop = %s", UaOnTop);
    if ((strncmp(UaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0))
    {
        // contentLister not "On Top" => show contentlister 
        CL_SEARCHPRINTF("CL NOT on top");
        pm_RaiseContentLister();
    }
}

// //////////////end searching///////////////////////////////////////////


// internal helper function
gboolean ctrl_file_exist(const char *pathName)
{
    struct stat statbuf;
    if (0 != stat(pathName, &statbuf))
    {
        return FALSE;
    }
    return TRUE;
}

// Called before a file item is to be clicked. 
// Change a simple file to a container
// 1. retrieve extension name, file name and dir name from item
// 2. check extension
// 3. create dir if necessary
// 4. create default manifest file for it.
// 5. change item to mdsFitContainer
// 2006-2008-07-27. dir name is changed, use filename as dirname
int ctrl_before_file_clicked(clDisplayItem_t * theItem)
{
    CL_LOGPRINTF("entry: szFilename [%s] szFileExt [%s]", theItem->szFilename, theItem->szFileExt);

    int ret = -1;               // -1 = error, 0 = ok

    if (strlen(theItem->szFilename) >= ERMDS_MAX_FILENAME_SIZE)
    {
        CL_ERRORPRINTF("Filename too long: [%s]", theItem->szFilename);
        return ret;
    }

    // maximum tries for file rename, arbitary value
    static const int MAX_COUNT = 5;
    char * viewer_id;

    int  i;
    char dirname [    ERMDS_MAX_FILENAME_SIZE            ];
    char filename[    ERMDS_MAX_FILENAME_SIZE            ];
    char tmp_file[    ERMDS_MAX_FILENAME_SIZE + MAX_COUNT];
    char new_dir [    ERMDS_MAX_FILENAME_SIZE            ];
    char new_file[2 * ERMDS_MAX_FILENAME_SIZE            ];
    char new_fil2[    ERMDS_MAX_FILENAME_SIZE            ];

    // 1. get extname, filename and dirname    
    char *name      = strrchr(theItem->szFilename, '/');
    char *extension = theItem->szFileExt;    
    if (name == NULL  || extension == NULL  || strlen(name) <= strlen(extension))
    {
        // it must be a fatal error, ignore this file
        CL_ERRORPRINTF("No extension or file name error!");
        return ret;
    }
    name++;

    i = strlen(theItem->szFilename) - strlen(name);
    strncpy(dirname, theItem->szFilename, i);
    dirname[i] = '\0';

    // verify if the path is trusted, otherwise we will *not* create the
    // manifest
    if (ctrl_is_trusted_path(dirname) == NULL)
    {
        CL_ERRORPRINTF("The fully qualified path [%s] is not trusted, not creating manifest", dirname);
        return ret;
    }

    // check if for this path we cannot create manifest folders
    if (ctrl_not_create_manifest(dirname))
    {
        CL_ERRORPRINTF("Configured to not create manifest folder for [%s]", dirname);
        return ret;
    }

    strcpy(filename, name);     // including extension

    // 2. check. we only handle the file we can handle
    if (!ctrl_check_extension(extension))
    {
        CL_ERRORPRINTF("Invalid extension: [%s]", extension);
        return ret;
    }
    
    // 3. create directory and move file if needed
    // Determine new directory and filename
    sprintf(new_dir,  "%s%s",    dirname, filename);
    sprintf(new_file, "%s%s/%s", dirname, filename, filename);
    if (strlen(new_file) >= ERMDS_MAX_FILENAME_SIZE)
    {
        CL_ERRORPRINTF("Filename too long to make it a container: [%s]", theItem->szFilename);
        return ret;
    }
    // before we create the dir, we need to change the file name
    // otherwise the dir will not be created successfully
    strcpy(tmp_file, theItem->szFilename);
    for(i = 0; i < MAX_COUNT; ++i)
    {
        strcat(tmp_file, "_");
        if (!ctrl_file_exist(tmp_file))
        {
            rename(theItem->szFilename, tmp_file);
            break;
        }
    }
    if (MAX_COUNT == i)
    {
        // could not change filename
        CL_ERRORPRINTF("Could not change name to %s", tmp_file);
        return ret;
    }
    //
    // create dir now                
    if (0 != mkdir(new_dir, 0755))
    {
        CL_ERRORPRINTF("Can not create directory for this content!");             
        rename(tmp_file, theItem->szFilename);
        return ret;
    }
    if (0 != rename(tmp_file, new_file))
    {
        // restore the file name and remove dir
        rmdir(new_dir);
        rename(tmp_file, theItem->szFilename);
        CL_ERRORPRINTF("Can not move file!");             
        return ret;
    }

    // if this is Mobipocket, check if there is a *.mbp file with the same name
    viewer_id = pm_getUaID(extension);
    if (viewer_id  &&  strcmp(viewer_id, "MOBIPOCKET") == 0)
    {
        struct stat file_stat;

        // strip extension
        char * tmp;
        char * fname = strdup(filename);
        tmp = strrchr(fname, '.');
        if (tmp) *tmp = '\0';

        snprintf(tmp_file, sizeof(tmp_file), "%s%s.%s", dirname, fname, "mbp");
        snprintf(new_fil2, sizeof(new_fil2), "%s%s/%s.%s", dirname, filename, fname, "mbp");
        free(fname);
        if (stat(tmp_file, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
        {
            CL_CONTROLPRINTF("Going to rename %s to %s", tmp_file, new_fil2);
            if (rename(tmp_file, new_fil2) != 0)
            {
                unlink(tmp_file);
                CL_ERRORPRINTF("Could not move %s, deleted it instead!", tmp_file);
            }
        }
    }
	
    // update the file name, so that it can be open successfully
    strncpy(theItem->szFilename, new_file, ERMDS_MAX_FILENAME_SIZE);
    
    // 4. create the manifest file 
    ctrl_create_manifest(theItem, new_dir, filename);
    
    // 5. change item to mdsFitContainer
    theItem->fit = mdsFitContainer;

    ret = 0;
    return ret;
}


gboolean ctrl_check_extension(const char *extension)
{
    // for the following file, we can handle them directly. temp solution
    static const char *ext_table[] = {
        "pdf",
        "xeb",
        "eba",
        "prc",
        "pdb",
        "mobi"
    };

    const int size = sizeof(ext_table) / sizeof(ext_table[0]);

    int i = 0;
    for (; i < size; ++i)
    {
        if (0 == strcasecmp(extension, ext_table[i]))
            return TRUE;
    }
    return FALSE;
}

// create a default manifest file for the item.
// if necessary, this function can be moved into ermanifest lib.
// read information from system register
// get type from ext
// get icon from ext
// get .... from ext
gboolean ctrl_create_manifest(clDisplayItem_t * theItem, const char *dir, const char *name)
{
    char pathName[ERMDS_MAX_FILENAME_SIZE] = {0};
        
    // remember manifest name
    int  nPathLength = g_snprintf(pathName, ERMDS_MAX_FILENAME_SIZE, "%s/" MANIFEST_FILENAME, dir);
    if (nPathLength > ERMDS_MAX_FILENAME_SIZE)
    {
        CL_ERRORPRINTF("manifest path name is too long: %d\n", nPathLength);
        return FALSE;
    }
    strcpy(theItem->szManifest, pathName);

    // call lib/ermanifest to create a xml file
    erManifest manifest;
    int ret = RET_ERR;
    ret = ermXmlCreateManifest(dir, &manifest);
    if (ret == RET_ERR)
    {
        CL_ERRORPRINTF("Could not create xml manifest file!\n");
        return FALSE;
    }

    ermXmlNewString(&manifest, "/", "package", "");
    ermXmlNewString(&manifest, "/package", "metadata", "");

    // dc-metadata
    ermXmlNewString(&manifest, "/package/metadata", "dc-metadata", "");
    ermXmlNewString(&manifest, "/package/metadata/dc-metadata", "Title", name);
    ermXmlNewString(&manifest, "/package/metadata/dc-metadata", "Description", "");
    ermXmlNewString(&manifest, "/package/metadata/dc-metadata", "Date", theItem->szDate);

    // y-metadata 
    ermXmlNewString(&manifest, "/package/metadata", "y-metadata", "");

    // startpage serves as an entry point for app.
    ermXmlNewString(&manifest, "/package/metadata/y-metadata", "startpage", name);

    ermXmlNewString(&manifest, "/package/metadata/y-metadata", "version", "000");

    // save
    ermXmlSaveAndClose(&manifest);

    return TRUE;
}

// Check to see if this folder is an exception to the rule of moving
// files into a manifest folder. 
// If this function returns TRUE, a manifest folder is not created.
static gboolean ctrl_not_create_manifest(const char *path)
{
    int i;

    // Folders that can be matched.
    // Must be full path names _including_ trailing slash
    static const char *path_table[] = {
        "/mnt/free"   MOBIPOCKET_FOLDER "/",
        "/mnt/cf"     MOBIPOCKET_FOLDER "/",
        "/media/card" MOBIPOCKET_FOLDER "/",
        "/mnt/usb"    MOBIPOCKET_FOLDER "/"
    };

    const int size = sizeof(path_table) / sizeof(path_table[0]);

    for (i = 0; i < size; i++)
    {
        if (strcasecmp(path, path_table[i]) == 0)
            return TRUE;
    }
    return FALSE;
}

void ctrl_shutdown()
{
    CL_WARNPRINTF("entry");

    g_shutdownPending = TRUE;

    // keep busy led on
    erbusy_shutdown();

    // block all keys
    button_block_all_keys(TRUE);
    
    // hide pincode screen
    ctrl_hide_pincode_screen();

    // show a "System will shutdown now message"
    listerShowErrorScreen(ctrlSystemShutdownError);

    // store last view content logging data before killing the
    // contentLister process
    ctrlDestroy(gContentLister);

    // re-create RTC alarm if needed
    timed_ids_final();

    // stop usb export of filesystem
    erStopMSDiskApp();

    // and stop the system
    system("halt");

    CL_WARNPRINTF("end");
}

// show location information 
const char *ctrl_get_current_location(ContentLister * theContentLister)
{
    CL_CONTROLPRINTF("entry: contentType [%d]", theContentLister->currentContentType);

    char root[ERMDS_MAX_FILENAME_SIZE] = { 0 };
    if (0 >= mdsGetRootLocation(theContentLister->currentContentType, root, ERMDS_MAX_FILENAME_SIZE))
    {
        return NULL;
    }

    stItem_t *pItem = stackPeek(theContentLister->currentContentType);
    if (NULL == pItem)
    {
        // no stack item -> report top directory
        return "";
    }
    else
    {
        // stack item found -> report its path
        mdsDirectory_t *directory = &g_array_index(pItem->dirArray, mdsDirectory_t, 0);
        if (directory)
        {
            if (strstr(directory->szFilename, root))
            {
                return (directory->szFilename + strlen(root));
            }
        }
    }
    return NULL;
}

// show absolute current location information 
const char *ctrl_get_abscurr_location(char *sPath, int iSize)
{
    memset(sPath, 0, iSize);
    if (0 >= mdsGetRootLocation(gContentLister->currentContentType, sPath, iSize))
    {
        return sPath;
    }

    stItem_t *pItem = stackPeek(gContentLister->currentContentType);
    if (NULL != pItem)
    {
        mdsDirectory_t *directory = &g_array_index(pItem->dirArray, mdsDirectory_t, 0);
        if (directory)
        {
            return directory->szFilename;
        }
    }
    return sPath;
}

gboolean ctrl_location_is_outbox(const char* location)
{
    if ( location != NULL && strncmp(location, "/" OUTBOX_NAME, 1 + strlen(OUTBOX_NAME)) == 0 )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

gboolean ctrl_current_location_is_outbox()
{
    const char* location = ctrl_get_current_location(gContentLister);
    
    return ctrl_location_is_outbox(location);
}


// Exception folder(s) in which all manifest-related functionality is disabled
// At the moment used for Mobipocket to disable share/rename
gboolean ctrl_current_location_is_mobipocket()
{
    char location[ERMDS_MAX_FILENAME_SIZE];
    char * cp;
    
    location[0] = '\0';
    stItem_t *pItem = stackPeek(gContentLister->currentContentType);
    if (pItem != NULL)
    {
        mdsDirectory_t *directory = &g_array_index(pItem->dirArray, mdsDirectory_t, 0);
        if (directory)
        {
            strncpy(location, directory->szFilename, sizeof(location));
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

    cp = &location[0];
    
    CL_CONTROLPRINTF("check location: [%s]", cp);
    
    // TODO: We could use a simple
    // return ctrl_not_create_manifest(location);
    // here. Is this what we want?
    
    if (strlen(location) > strlen(MOBIPOCKET_FOLDER))
    {
        cp += (strlen(location) - strlen(MOBIPOCKET_FOLDER));
    }
    
    CL_CONTROLPRINTF("found: [%s]", cp);

    if (strcmp(cp, MOBIPOCKET_FOLDER) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// detect whether the current content location is empty or not
// returns: TRUE is empty, FALSE is not empty
static gboolean ctrl_current_location_is_empty(void)
{
    CL_CONTROLPRINTF("empty");

    if (gContentLister && gContentLister->itemCount > 0)
    {
        return FALSE;
    }
    else
    {
        CL_CONTROLPRINTF("Current location is empty.");
        return TRUE;
    }
}

// ///////////////////////////////////////////////////////////////////////////////
// for pincode
//

// re-read the values of pincode from sysset and re-set the lock screen icon on toolbar
void ctrl_pincode_setting_init()
{
    gboolean enable = FALSE;
    gchar pincode[PINCODE_MAX_LENGTH + 1];
    ContentLister *theContentLister = gContentLister;

    g_return_if_fail(NULL != theContentLister);
    g_return_if_fail(NULL != theContentLister->pincodeScreen);
    sysset_read_pincode_onoff(&enable);
    sysset_read_pincode_string(pincode);
    
    gtk_pincode_screen_set_pincode_settings(theContentLister->pincodeScreen, enable, pincode);
    ctrl_pincode_set_toolbar_icon();
}

void ctrl_show_pincode_screen(showPincodeReason_t reason, on_passed_callback_t* on_passed_callback, on_cancel_callback_t* on_cancel_callback)
{
    CL_LOGPRINTF("PINCODE screen show");

    ContentLister *theContentLister = gContentLister;
    char *UaOnTop = NULL;

    g_return_if_fail(theContentLister != NULL);
    g_return_if_fail(theContentLister->pincodeScreen != NULL);
    g_return_if_fail(g_listerState != STATE_PINCODE);

    if (reason != startup_t)
    {
        // abort miscellaneous contentLister windows
        ctrl_hide_misc_screen();

        // bring contentlister on top
        UaOnTop = pm_getUaOnTop ();
        CL_CONTROLPRINTF ("%s on top", UaOnTop);
        strncpy (restoreUaOnTop, UaOnTop, sizeof (restoreUaOnTop)); 
        if ((strncmp (UaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0)) 
        { 
            // contentLister not "On Top" => show contentlister 
            CL_CONTROLPRINTF ("CL NOT on top");
            display_update_increase_level(NO_DISPLAY_UPDATE_LEVEL);
            pm_RaiseContentLister (); 
        }
    }

    if (GTK_WIDGET_VISIBLE(theContentLister->pincodeScreen) == FALSE)
    {
        // => only request a refresh on the main window expose event
        CL_SCREENPRINTF("Display pincode screen");
        if (display_update_get_level() == NO_DISPLAY_UPDATE_LEVEL)
        {
            display_update_decrease_level(MAIN_WINDOW_EXPOSE_LEVEL);
        }
        else
        {
            display_update_increase_level(MAIN_WINDOW_EXPOSE_LEVEL);
        }
        gtk_window_fullscreen(GTK_WINDOW(theContentLister->topLevelWindow));
        gtk_widget_hide(GTK_WIDGET(theContentLister->listerScreen ));
        gtk_widget_hide(GTK_WIDGET(theContentLister->errorScreen  ));
        gtk_widget_show(GTK_WIDGET(theContentLister->pincodeScreen));
        gtk_widget_grab_focus(GTK_WIDGET(theContentLister->pincodeScreen));
    }
    // it's possible user change or reset pincode after we create gtkPincdeScreen
    ctrl_pincode_setting_init();

    // when show pincode screen
    // and disable usb connect with pc
    // and begin to accumulate idle time 
    // and reset the pincode passed

    // attention: when launched downloadMgr, will change contentlister status to STATE_DOWNLOADMGR
    setListerState(STATE_PINCODE);
    erStopMSDiskApp();
   
    accumulate_idle_time_set_callback(ctrl_pincode_idle_time_callback);
    accumulate_idle_time_set_threshold(PINCODE_MAX_IDLE_TIME);
    gtk_pincode_screen_layout_reset(theContentLister->pincodeScreen);
    gtk_pincode_screen_data_reset(theContentLister->pincodeScreen);
    gtk_pincode_screen_set_reason(theContentLister->pincodeScreen, reason);
    gtk_pincode_screen_set_on_passed_callback(theContentLister->pincodeScreen, on_passed_callback);

    if (ctrl_pincode_is_for_locking())
    {
        gtk_pincode_screen_set_on_cancel_callback(theContentLister->pincodeScreen, NULL);
    }
    else
    {
        gtk_pincode_screen_set_on_cancel_callback(theContentLister->pincodeScreen, on_cancel_callback);
    }
}

void ctrl_hide_pincode_screen()
{
    showPincodeReason_t reason;
    CL_LOGPRINTF("PINCODE screen hide");

    ContentLister *theContentLister = gContentLister;
    if (g_listerState != STATE_PINCODE)
    {
        return;
    }

    if (theContentLister && theContentLister->pincodeScreen && GTK_WIDGET_VISIBLE(theContentLister->pincodeScreen))
    {
        // => only request a refresh on the main window expose event
        CL_SCREENPRINTF("");
        display_update_increase_level(MAIN_WINDOW_EXPOSE_LEVEL);

        gtk_window_unfullscreen(GTK_WINDOW(theContentLister->topLevelWindow));
        gtk_widget_show(GTK_WIDGET(theContentLister->listerScreen ));
        gtk_widget_hide(GTK_WIDGET(theContentLister->errorScreen  ));
        gtk_widget_hide(GTK_WIDGET(theContentLister->pincodeScreen));
        gtk_widget_grab_focus(GTK_WIDGET(theContentLister->listerArea));

        reason = gtk_pincode_screen_get_reason(theContentLister->pincodeScreen);
        if (reason != startup_t)
        {
            if ((strncmp (restoreUaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0)) 
            { 
                pm_RaiseUserApp (restoreUaOnTop);
                strncpy (restoreUaOnTop, CONTENTLISTER_UAID, sizeof(restoreUaOnTop)); 
            } 
        }
        if (   reason != startup_t
            && reason != enterSettings_t)
        {
            erStartMSDiskApp();
        }
    }

    // when hide pincode screen, 
    // and set contentlister to be normal state
    // and enable usb connect with pc
    // and end to accumulate idle time
    setListerState(STATE_NORMAL);
    accumulate_idle_time_end();
}

gboolean ctrl_pincode_is_for_locking()
{
    gboolean bIsForLocking=TRUE;
    ContentLister *theContentLister;
    showPincodeReason_t reason;
    
    theContentLister = gContentLister;
    if (NULL == theContentLister)
        return bIsForLocking;
    if (NULL == theContentLister->pincodeScreen)
        return bIsForLocking;
    reason = (showPincodeReason_t)gtk_pincode_screen_get_reason(theContentLister->pincodeScreen);
    switch (reason)
    {
        case startup_t:
        case lockScreen_t:
            bIsForLocking = TRUE;
        break;
        
        case signDocument_t:
        case enterSettings_t:
        case importSettings_t:
            bIsForLocking = FALSE;
        break;
        
        default:
            bIsForLocking = TRUE;
    }
    return bIsForLocking;
}

// when pincode screen for locking purpose, we need to lock all keys except 'connect long'
// when pincode screen for anthorizing purpose, we need to lock all keys except 'up key'
//
// cancel the pincode screen
//
void ctrl_pincode_overview_button_clicked(void)
{
   showPincodeReason_t reason;
   gboolean bIsForLocking;

    accumulate_idle_time_reset();

    bIsForLocking = ctrl_pincode_is_for_locking();
    if (bIsForLocking)
    {
        erbusy_off();
        return;
    }

    ContentLister *theContentLister = gContentLister;
    reason = gtk_pincode_screen_get_reason(theContentLister->pincodeScreen);
    ctrl_hide_pincode_screen();
    gtk_pincode_screen_on_cancel_callback(theContentLister->pincodeScreen);
}

// want to reset pincode, launch connectMgr, pincode screen is at the background
void ctrl_pincode_connect_button_clicked()
{
    gboolean bIsForLocking;

    accumulate_idle_time_reset();

    bIsForLocking = ctrl_pincode_is_for_locking();
    if (!bIsForLocking)
    {
        erbusy_off();
        return;
    }
    
    CL_LOGPRINTF("Connect from PINCODE...");
    // when downloading, we should remove accumulate_idle_time
    // to prevent shutdown device due to idle
    accumulate_idle_time_end();

    ctrl_connect(connectScrUnchanged, connect_from_pincode);
}

void ctrl_pincode_button_clicked(void)
{
    accumulate_idle_time_reset();
    erbusy_off();
}

gboolean ctrl_is_connect_from_pincode(void)
{
    gboolean bRet = FALSE;
   
    if (   getListerState() == STATE_DOWNLOADMGR
        && connectStruct.connect_from_pincode   )
    {
        bRet = TRUE;
    }
    CL_LOGPRINTF("return %d", bRet);
    return bRet;
}

void ctrl_connect_back_to_pincode_screen()
{
    ContentLister *theContentLister = gContentLister;
    char *UaOnTop = NULL;
    showPincodeReason_t reason;

    g_return_if_fail(NULL != theContentLister);
    g_return_if_fail(NULL != theContentLister->pincodeScreen);

    // set contentliser to STATE_PINCODE
    setListerState(STATE_PINCODE);

    // when finishing downloading reset pincode,  we should continue accumulate_idle_time

    // re-read pincode from sysset
    // from pincode screen user can connect to iDS and download a pincode reset package
    ctrl_pincode_setting_init();

    reason = gtk_pincode_screen_get_reason(theContentLister->pincodeScreen);
    if (reason != startup_t)
    {
        UaOnTop = pm_getUaOnTop();
        CL_CONTROLPRINTF("UaOnTop = %s", UaOnTop);
        if ((strncmp(UaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0))
        {
            // contentLister not "On Top" => show contentlister 
            CL_CONTROLPRINTF("CL NOT on top");
            pm_RaiseContentLister();
        }
    }

    display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL);
}

void ctrl_on_lock_screen_clicked(int iconState)
{
    CL_IPCPRINTF("entry: iconState [%d]", iconState);

    switch (iconState)
    {
    case iconState_normal:
    // change icon to "selected",
//        toolbar_setIconState(iconID_lock_screen, iconState_selected);
//        toolbar_synchronise();
    ctrl_lock_screen();
    break;

//    case iconState_selected:
    // change icon to "normal",
//        toolbar_setIconState(iconID_lock_screen, iconState_normal);
//        toolbar_synchronise();
    break;

    default:
        /*
        * ignore 
        */ ;
    }
}

static void ctrl_lock_screen(void)
{
    if (g_listerState != STATE_DOWNLOADMGR)
    {
        erbusy_blink();
        ctrl_show_pincode_screen(lockScreen_t, NULL, NULL);
    }
}

static void ctrl_pincode_idle_time_callback()
{
    CL_LOGPRINTF("system shutdown due to PINCODE screen idle");
    ctrl_hide_pincode_screen();
    ctrl_shutdown();
}

static void ctrl_pincode_set_toolbar_icon()
{
    ContentLister *theContentLister = NULL;
    gboolean enable = FALSE;
    gchar pincode[PINCODE_MAX_LENGTH + 1];          

    theContentLister = gContentLister;
    if (theContentLister != NULL && theContentLister->pincodeScreen)
    {
        gtk_pincode_screen_get_pincode_settings(theContentLister->pincodeScreen, &enable, pincode);
        if (FALSE == enable || 0 == strlen(pincode))
        {
            toolbar_setIconState(iconID_lock_screen, iconState_grey);
        }
        else
        {
            toolbar_setIconState(iconID_lock_screen, iconState_normal);
        }
    }
}

static void ctrl_start_setup()
{
    ctrl_start_application(SETUP_APP, NULL);
}

////////////////////////////////////////////////////////////////////////////////
// for sorting
//
static void ctrl_show_sort_wnd_trigger(void)
{
    g_timeout_add(500, ctrl_show_sort_wnd, NULL);
}

static gboolean ctrl_show_sort_wnd(gpointer data)
{
    gboolean bRet = FALSE;      // don't me call again

    CL_LOGPRINTF("entry");

    if (getListerState() == STATE_MISC)
    {
        CL_WARNPRINTF("already sorting: igore");
        return bRet;
    }
   
    // update state and ui
    erbusy_blink();
    setListerState(STATE_MISC);
    
    // update toolbar
    toolbar_disableUpdate();
    toolbar_setIconState(iconID_search,   iconState_grey    );
    toolbar_setIconState(iconID_sort,     iconState_selected);
    toolbar_setIconState(iconID_share,    iconState_grey    );
    toolbar_setIconState(iconID_tagging,  iconState_grey    );
    toolbar_setIconState(iconID_trashcan, iconState_grey    );
    toolbar_enableUpdate();

    // update pagebar
    pagebar_set_pagecount(1);
    pagebar_goto_page(1);
    pagebar_redraw();

    // hide lister items, then show edit screen
    int i;
    GtkWidget *listItem;
    for (i = 0; i < _G.pageItems; i++)
    {
        listItem = lsGetListerItem(gContentLister->lister, i);
        gtk_widget_hide(listItem);
    }
   
    // increase level to prevent display from updating in 'ctrl_sort_wnd_setting_init()'
    display_update_increase_level(LISTER_EXPOSE_LEVEL);
    // 
    ctrl_sort_wnd_setting_init();

    gtk_widget_show(GTK_WIDGET(gContentLister->sortWnd));

    return bRet;
}

void ctrl_hide_sort_wnd(gboolean bSort)
{
    CL_LOGPRINTF("entry");

    if ( (getListerState() == STATE_MISC) 
         && (gContentLister->sortWnd)
         && GTK_WIDGET_VISIBLE(gContentLister->sortWnd) )
    {
        // update toolbar
        toolbar_disableUpdate();
        toolbar_setIconState(iconID_search,   iconState_normal);
        toolbar_setIconState(iconID_sort,     iconState_normal);
        toolbar_setIconState(iconID_share,    iconState_normal);
        toolbar_setIconState(iconID_tagging,  iconState_normal);
        toolbar_setIconState(iconID_trashcan, iconState_normal);
        ctrl_hide_keyboard(iconState_grey);
        toolbar_enableUpdate();
        
        if (bSort)
        {
           ctrl_sort_current();
        }
        else
        {
            ctrl_rescan_current();
        }
        
        // show contentlister page
        gtk_widget_hide(GTK_WIDGET(gContentLister->sortWnd));
        gtk_widget_grab_focus(GTK_WIDGET(gContentLister->listerArea));
        setListerState(STATE_NORMAL);   
    }
}

static void ctrl_on_sort_clicked(int iconState)
{
    switch (iconState)
    {
        case iconState_normal:
            if (getListerState() == STATE_NORMAL)
            {
                // de-select contentlister items
                ctrl_listItem_focus(-1, gContentLister);

                // update toolbar, show sort window after toolbar updated
                toolbar_disableUpdate();
                toolbar_setIconState(iconID_search,   iconState_grey    );
                toolbar_setIconState(iconID_sort,     iconState_selected);
                toolbar_setIconState(iconID_share,    iconState_grey    );
                toolbar_setIconState(iconID_tagging,  iconState_grey    ); 
                toolbar_setIconState(iconID_trashcan, iconState_grey    );
                ctrl_add_on_toolbar_sync_action(ctrl_show_sort_wnd_trigger);
                toolbar_enableUpdate();
                toolbar_synchronise();
            }
            break;

        case iconState_selected:
            ctrl_hide_sort_wnd(TRUE);
            break;

        default:
            /*
             * ignore 
             */ ;
    }
}

static void ctrl_sort_current(void)
{
    CL_LOGPRINTF("entry");
    gint nSave = -1;
 
    erbusy_blink();

    nSave = ctrl_save_sort_settings();
    if (1 == nSave)
    {
        int      itemIndex;
        gchar    szFilename[ERMDS_MAX_FILENAME_SIZE];
        gboolean ret;

        // remember the last selected item position
        itemIndex = mdsGetIndex(gContentLister->currentContentType);
        ret = mdsGetItemFilename(itemIndex, szFilename, ERMDS_MAX_FILENAME_SIZE);
        
        // rescan the content after changing sort settings
        ctrl_rescan_current();
        if (ret)
        {
            // go to the item which the last cursor position pointed to
            ctrl_goto_item(szFilename);

            // update the stored index value
            itemIndex = ctrl_get_item_index(gContentLister);
            mdsSetIndex(gContentLister->currentContentType, itemIndex);
        }
    }
    else
    {
        ctrl_rescan_current();
    }
}

// comparing new sort settings with the old ones, 
// if not equals, save to registry. otherwise, return immeditately
// return : -1 error, 0 needn't save, 1 saved
static gint ctrl_save_sort_settings(void)
{
    CL_LOGPRINTF("entry");
    
    mdsSortStruct_t  sortStruct;
    mdsSortStruct_t* oldSortStruct = NULL;
    gint             nSave = -1;

    // get the new sort settings from sortwnd
    sort_wnd_get_sort_settings(&sortStruct);
    
    // get the old sort settings from g_rootInfo
    oldSortStruct = mdsGetSortStructOfRootInfo(gContentLister->currentContentType);
    
    if (oldSortStruct)
    {
        if ((oldSortStruct->ft != sortStruct.ft) || (oldSortStruct->ascending != sortStruct.ascending))
        {
            gchar *szID = NULL;
            regContentCategory_t *theContentCategory = NULL;

            szID = getContentCategoryID(gContentLister->currentContentType);
            if (NULL != szID && strlen(szID) != 0)
            {
                prepare_registry_write();

                theContentCategory = erRegGetContentCategory(szID);
                if (theContentCategory)
                {
                    char* sortString = getFieldString(sortStruct.ft);
                    gboolean ascending = ((1 == sortStruct.ascending) ? TRUE : FALSE);
                    // free the old memory firstly
                    if (theContentCategory->sortFieldType)
                    {
                        free(theContentCategory->sortFieldType);
                        theContentCategory->sortFieldType = NULL;
                    }
                    theContentCategory->sortFieldType = g_strdup(sortString);
                    theContentCategory->sortAscending = ascending;

                    erRegSetContentCategory(szID, theContentCategory);
                    erRegFreeContentCategory(theContentCategory);
                    
                    // updating the sortStruct of rootinfo
                    mdsSetSortStructOfRootInfo(gContentLister->currentContentType, &sortStruct);
                    nSave = 1;
                }
                else
                {
                    CL_ERRORPRINTF("CATEGORY_GROUP [%s] registry info missing", szID);
                }

                do_registry_write();
            }
            else
            {
                CL_ERRORPRINTF("can't get the content category ID");
            }
        }
        else
        {
            CL_LOGPRINTF("the new sort settings are the same as the old ones.");
            nSave = 0;
        }
    }
    else
    {
        CL_ERRORPRINTF("can't get the old sort settings.");
    }
    // return
    return nSave;
}

static void ctrl_sort_wnd_setting_init(void)
{
    mdsSortStruct_t        *sortStruct = NULL;

    // get the sort settings
    sortStruct = mdsGetSortStructOfRootInfo(gContentLister->currentContentType);    

    // initialize the elements of sortwnd
    if (sortStruct)
    {
        sort_wnd_set_sort_settings(sortStruct);
    }
    else
    {
        CL_ERRORPRINTF("can't get the sort settings");
    }
}

////////////////end sorting///////////////////////////////////////////


void ctrl_startup(void)
{
    CL_CONTROLPRINTF("entry");
    
    gboolean      enable = FALSE;
    gchar         pincode[PINCODE_MAX_LENGTH + 1];   
    ContentLister *theContentLister = gContentLister;

    static gboolean firsttime = TRUE;

    g_return_if_fail(theContentLister != NULL);
    g_return_if_fail(g_shutdownPending == FALSE);

    // If user hasn't set language yet or the current language hasn't been installed,
    // run 'setup --select-language-only' to let user set language.
    // Do this only once, user may exit setup without selecting a language.
    if (firsttime  &&  !languageSelected())
    {
        setListerState(STATE_LANG_SEL);
        display_update_increase_level(NO_DISPLAY_UPDATE_LEVEL);
        gtk_idle_add(ctrl_on_idle_select_language, (gpointer)FALSE);
    }
    else
    {
        // read the pincode settings
        sysset_read_pincode_onoff(&enable);
        sysset_read_pincode_string(pincode);

        gtk_pincode_screen_set_pincode_settings(theContentLister->pincodeScreen, enable, pincode);
        if (enable  &&  strlen(pincode) > 0)
        {
            CL_LOGPRINTF("PINCODE on-show pincode screen");
            ctrl_show_pincode_screen(startup_t, ctrl_start_up_behaviour, NULL);
        }
        else
        {
            CL_LOGPRINTF("PINCODE off or empty - user customized start-up behaviour");
            ctrl_pincode_set_toolbar_icon();
            ctrl_start_up_behaviour();
        }
    }

    firsttime = FALSE;
    CL_CONTROLPRINTF("leave");
}

// start settings application with language-select screen
static gboolean ctrl_on_idle_select_language(gpointer data)
{
    // start settings application
    ctrl_start_application(SETUP_APP, "--select-language-only");

    // lower display level, but no display update now
    startup_idle_display(FALSE);

    return FALSE;  // return FALSE to remove handler
}


// user customized start-up behaviour
static void ctrl_start_up_behaviour(void)
{
    CL_CONTROLPRINTF("entry");

    regStartUp_t* theStartUp = erRegGetStartUp();

    // postpone startup actions until all initilisation done
    display_update_increase_level(NO_DISPLAY_UPDATE_LEVEL);
    gtk_idle_add(startup_idle_handler, (gpointer)theStartUp);

    CL_CONTROLPRINTF("leave");
}

static gboolean startup_idle_handler(gpointer data)
{
    CL_CONTROLPRINTF("entry");

    startup_behaviour_t arg_startup   = main_get_startup_behaviour();
    regStartUp_t*       theStartUp    = (regStartUp_t*) data;
    gboolean            displayUpdate = TRUE;
    gboolean            allow_usb     = TRUE;  // allow USB connect
    gboolean            b;

    if (arg_startup >= 0  &&  arg_startup < behaviourUndefined_t)
    {
        // startup location specified on command-line:
        //      discard old startup and use this one
        if (theStartUp)
        {
            erRegFreeStartUp(theStartUp);
            theStartUp = NULL;
        }

        theStartUp = g_new0(regStartUp_t, 1);
        if (theStartUp == NULL) { CL_ERRORPRINTF("malloc failed"); exit(0); }

        theStartUp->behaviour = arg_startup;
    }

    if (theStartUp)
    {
        CL_CONTROLPRINTF("behaviour [%d] document [%s]", theStartUp->behaviour, theStartUp->documentPath);
        switch (theStartUp->behaviour)
        {
            case specificDocument_t:
                b = ctrl_open_quick_start_guide(theStartUp->documentPath);
                displayUpdate = b ? FALSE : TRUE;
                if (b)
                {
                    allow_usb = FALSE;
                }
                break;
            case referenceMaterials_t:
                ctrl_display_reference_materials();
                break;
            case lastReadDocument_t:
                b = ctrl_open_last_read_document();
                displayUpdate = b ? FALSE : TRUE;
                if (b)
                {
                    allow_usb = FALSE;
                }
                break;
            case recentDocuments_t:
                ctrl_display_recent_documents();
                break;
            case deviceManager_t:
                ctrl_mode_button_clicked();
                break;
            case newsFolder_t:
                ctrl_contentType_button_clicked(st_ContentTypeNews);
                break;
            case booksFolder_t:
                ctrl_contentType_button_clicked(st_ContentTypeBooks);
                break;
            case docsFolder_t:
                ctrl_contentType_button_clicked(st_ContentTypeDocs);
                break;
            case notesFolder_t:
                ctrl_contentType_button_clicked(st_ContentTypeNotes);
                break;
            case downloadHistory_t:
                ctrl_display_download_history();
                break;
            default:
                CL_WARNPRINTF("Invalid start-up behaviour.");
                ctrl_display_recent_documents();
                break;
        }

        erRegFreeStartUp(theStartUp);
        theStartUp = NULL;
    }
    else
    {
        CL_WARNPRINTF("No START_UP section specified.");
        ctrl_display_recent_documents();
    }

    if (allow_usb)
    {
        erStartMSDiskApp();
    }
 
    // Start background-connect
    ctrl_background_connect_timeout_start(TRUE);
    ctrl_set_connect_icon();
    
    // postpone display update until all initilisation done
    gtk_idle_add(startup_idle_display, (gpointer)displayUpdate);

    CL_CONTROLPRINTF("leave");
    return FALSE;  // return FALSE to remove handler
}

static gboolean startup_idle_display(gpointer data)
{
    gboolean  displayUpdate = (gboolean) data;

    CL_CONTROLPRINTF("entry: displayUpdate [%d]", displayUpdate);

    display_update_decrease_level(LOWEST_LEVEL);
    if (displayUpdate)
    {
        display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL);
    }

    CL_CONTROLPRINTF("leave");
    return FALSE;  // return FALSE to remove handler
}

static gboolean ctrl_open_quick_start_guide(const char *documentPath)
{
    CL_CONTROLPRINTF("entry");

    char          dirname[ERMDS_MAX_FILENAME_SIZE];
    char          szLinkName[ERMDS_MAX_FILENAME_SIZE];
    struct stat   statbuf;
    gboolean      done = FALSE;
    gboolean      bRet = TRUE;

    // create symlinks for quick start guide in recent documents
    if (   documentPath != NULL
        && stat(documentPath, &statbuf) == 0 )
    {
        // QuickRef file exists
        if ( mdsGetRootLocation(st_RecentDocuments, dirname, sizeof(dirname)) > 0 )
        {
            g_snprintf(szLinkName, ERMDS_MAX_FILENAME_SIZE, "%s/%s", dirname, "ZZZZZZ");
            unlink(szLinkName);
            if (symlink(documentPath, szLinkName)  == 0)
            {
                CL_LOGPRINTF("Created symlink [%s] -> [%s]", szLinkName, documentPath);
                done = TRUE;
            }
            else
            {
                CL_ERRORPRINTF( "Error [%s] on create symlink [%s] -> [%s]", 
                                strerror(errno), szLinkName, documentPath   );
            }
        }
    }

    if (done)
    {
        // open the last read document, it's Quick Start Guide.
        bRet = ctrl_open_last_read_document();
    }
    else
    {
        CL_WARNPRINTF("Can't open Quick Start Guide [%s], display recent documents", documentPath);
        ctrl_display_recent_documents();
        bRet = FALSE;
    }

    CL_CONTROLPRINTF("leave: bRet [%d]", bRet);
    return bRet;
}

static void ctrl_display_reference_materials(void)
{
    CL_CONTROLPRINTF("entry");
    int index;

    ContentLister *theContentLister = gContentLister;
    
    // first scan mode
    ctrl_mode_button_clicked();

    if (theContentLister->itemCount > 0)
    {
        for (index = 0; index < theContentLister->itemCount; index++)
        {
            if (strstr(theContentLister->items[index].szManifest, MODE_MENU_REFERENCE_MATERIALS_DIRNAME) != NULL)
            {
                CL_WARNPRINTF("The index of reference materials is %d", index);
                // scan reference materials
                ctrl_listItem_clicked(index, theContentLister);
                break;
            }
        }
    }
    
    // If not found, do nothing

    CL_CONTROLPRINTF("leave");
}

static gboolean ctrl_open_last_read_document(void)
{
    CL_CONTROLPRINTF("entry");

    gboolean      bRet = TRUE;
    ContentLister *theContentLister = gContentLister;

    // scan the recent documents
    ctrl_display_recent_documents();

    if (theContentLister->itemCount > 0)
    {
        // open the first item in recent documents
        ctrl_listItem_clicked(0, theContentLister);
    }
    else
    {
        bRet = FALSE;
        CL_WARNPRINTF("User hasn't read any documents yet.");
    }

    CL_CONTROLPRINTF("leave: bRet [%d]", bRet);
    return bRet;
}

static void ctrl_display_recent_documents()
{
    CL_CONTROLPRINTF("entry");

    ContentLister *theContentLister = gContentLister;
    char *UaOnTop = NULL;

    g_return_if_fail(NULL != theContentLister);
   
    // Start in recent-documents
    theContentLister->previousContentType = st_MenuTypeMode;
    theContentLister->currentContentType  = st_RecentDocuments;

    ctrl_scan_content(theContentLister, ctrlScanCurrent, NULL);

    UaOnTop = pm_getUaOnTop();

    CL_CONTROLPRINTF("UaOnTop = %s", UaOnTop);

    if ( (NULL != UaOnTop) && (0 != strlen(UaOnTop))
            && (strncmp(UaOnTop, CONTENTLISTER_UAID, UAID_MAX_SIZE) != 0))
    {
        // contentLister not "On Top" => show contentlister 
        CL_CONTROLPRINTF("CL NOT on top");
        pm_RaiseContentLister();
    }

    CL_CONTROLPRINTF("leave");
}

// Mvdw: I've disabled this export/import functionality for now, because it has not been
// fully tested and there is no iLiad Companion Software support for it.
// Adding it to a release right now is IMO too big a risk.
#ifdef ENABLE_CONFIG_EXPORT_IMPORT
void ctrl_export_config(void)
{
    if (erRegReadLock())
    {
        erRegExportConfig();
        erRegUnlock();
    }
}

static void ctrl_import_config(void)
{
    if (erRegWriteLock())
    {
        erRegImportConfig();
        erRegUnlock();
    }
}

void ctrl_handle_import_config(void)
{
    // need to import configuration or not 
    if (erRegCfgFileExist())
    {
         gboolean enable = FALSE;
         gchar pincode[PINCODE_MAX_LENGTH + 1];

         gtk_pincode_screen_get_pincode_settings(gContentLister->pincodeScreen, &enable, pincode);
         if (FALSE == enable || 0 == strlen(pincode))
         {
             ctrl_import_config();
         }
         else
         {
             // show pincode screen before importing
             ctrl_show_pincode_screen(importSettings_t, ctrl_import_config, NULL);
         }
         // TODO wyh mdsInit etc...
    }
}
#endif
