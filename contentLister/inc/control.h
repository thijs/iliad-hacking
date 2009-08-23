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
 * \file control.h
 * \brief control - handle input and keep state                  
 * 
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#ifndef __CONTROL_MODULE__H__
#define __CONTROL_MODULE__H__

#include "lister.h"
#include "gtkErrorScreen.h"
#include "metadataStoreManager.h"

#ifdef __cplusplus
extern  "C"
{
#endif  /* __cplusplus */

#define CATEGORIES_LIST        (-1)

//contentlister identification of the memory extensions (should map with the manifest content)
#define CF_ID                "CF"
#define SD_ID                "SD"
#define USB_ID               "USB"
#define MAIN_MEMORY_ID       "MAIN"
#define RECENT_DOCS_ID       "RECENT"

// time-out in seconds before "listitem select" mode exits automatically
#define ITEM_SELECT_TIMEOUT             6       // seconds
#define ITEM_SELECT_TIMER_INTERVAL      500     // milliseconds
#define ITEM_SELECT_TIMEOUT_TICKS       ((ITEM_SELECT_TIMEOUT * 1000) / ITEM_SELECT_TIMER_INTERVAL)

// retry interval for background connect (in minutes)
#define BG_CONNECT_RETRY_INTERVAL  5

#define PINCODE_MAX_IDLE_TIME (5*60*1000)       // in milliseconds

// Illegal filename characters on FAT32 partition
// Used for search and rename
#define ILLEGAL_FILENAME_CHARS "*?/\\\"<>:|";


#define CONTENTLISTER_GROUP     "CONTENTLISTER"
#define DO_SHARE_KEY            "doc_sharing"
#define DO_SIGN_KEY             "doc_signing"

#define MODE_MENU_REFERENCE_MATERIALS_DIRNAME "_reference/"

#define ERROR_STRING_BUFLEN     256

typedef enum
{
    ctrlScanCurrent = 0,
    ctrlScanPrevious = 1,
    ctrlScanNext = 2,
} ctrlScanType_e;

typedef struct _contentLister
{
    lsLister_t*       lister;                        // reference to the lister widget, where the items are displayed
    GtkWidget*        topLevelWindow;                // reference to top-level window

    GtkWidget*        listerScreen;                  // reference to lister screen
    GtkWidget*        listerArea;                    // reference to lister area within lister screen
    GtkWidget*        contentEditWnd;                // reference to content edit window
    GtkWidget*        searchWnd;                     // reference to the search screen
    GtkWidget*        sortWnd;                       // reference to the sort screen
    GtkWidget*        distListWnd;                   // reference to the distribution list screen

    GtkWidget*        errorScreen;                   // reference to the error screen
    ctrlErrorType_e   currentError;

    GtkWidget*        pincodeScreen;                 // reference to pincodeScreen

    clDisplayItem_t   items[MAX_ITEMS_ON_ONE_PAGE];  // lister page items, the data associated with the currently displayed items
    int               itemCount;                     // number of items in the above array 
    st_ContentType_e  currentContentType;            // the category of the currently displayed items
    st_ContentType_e  previousContentType;           // the category of the previously displayed items
    int               currentFocus;                  // currently focused List item widget  
    int               pagecount;                     // number of pages in this category 
    int               currentPage;                   // currently selected page
} ContentLister;

typedef enum {
    STATE_NORMAL = 0,
    STATE_USB_CONNECTED,
    STATE_DOWNLOADMGR,
    STATE_MISC,// combine rename, search, sort states into one state STATE_MISC
    STATE_PINCODE,
    STATE_LANG_SEL,
    STATE_LAST
} listerState_t;

// reasons to call ctrl_connect() function
typedef enum
{
    connect_manual,
    connect_background,
    connect_from_pincode,
    connect_after_reboot,
    connect_timed_ids_with_sw,
    connect_timed_ids_content_only,
    connect_timed_ids_background
} e_ctrl_connect_reason_t;

/**
 * Init contentlister modules/data : - init meta data store (logging information)
 *                                   - init screen layout
 *                                   - init button handling
 *                                   - init history data  
 *                                   - init IPC communication
 *                                   - allocate ContentLister, for maintaining state       
 *
 * @param window reference to the top level window 
 *
 * @returns reference to contentLister structure, NULL in case of failure
 */
ContentLister *ctrlInit(GtkWidget * topLevelWindow);

/**
 * Set fixed screen texts
 */
void ctrlSetScreenTexts(void);

/**
 * destroy contentlister modules - clean up allocated memory and
 * store the loggin information in Flash
 *
 * @param contentLister reference contentLister data 
 *
 * @returns -
 */
void ctrlDestroy(ContentLister * contentLister);

/**
 * Gtk list item clicked callback routine
 *
 * @param data index of the item in the pageitem list 
 * @param reference passed during lsInit
 *
 * @returns -
 */
void ctrl_listItem_clicked(int index, gpointer data);

// convert the ID from the manifest file to the known storage types
st_ContentType_e ctrl_get_storageType(gchar* szExtID);

/**
 * A storage module has been inserted or removed
 *
 * @param storage module identification
 * @param 1/0 = module present yes/no
 *
 * @returns -
 */
void ctrl_storage_changed(st_ContentType_e storageType, gboolean storagePresent);

/**
 * category button selected
 *
 * @param data - reference contentLister data 
 *
 * @returns -
 */
void ctrl_contentType_button_clicked(st_ContentType_e type);

/**
 * Jump to the specified page number, in case the page number is invalid keep the current status
 * and update the status information
 *
 * @param pagenumber the requested page number
 *
 * @returns --
 */
void ctrl_goto_page(int pagenumber);

void ctrl_connect_button_clicked(void);
void ctrl_disconnect(connectScr_e screen);
void ctrl_disconnect_wait(void);
void ctrl_background_connect_timeout_start(gboolean read_settings);
void ctrl_set_connect_icon(void);
void ctrl_new_content_received(void);
void ctrl_mode_button_long_clicked(void);
void ctrl_usbmode_keypress(void);

//rescan current contentype and dir,for application quited initiatively.(Jian)
void ctrl_rescan_current(void);

unsigned int ctrl_get_item_index(ContentLister * theContentLister);
int  ctrl_before_file_clicked(clDisplayItem_t * theItem);

void ctrl_mode_button_clicked(void);
void ctrl_display_download_history(void);
void ctrl_cleanup_download_history(void);
void ctrl_battery_low_screen(gint percentage, gint time_left, gboolean shutdown);
void ctrl_on_icon_clicked(int iconID, int iconState);
void ctrl_on_lock_screen_clicked(int iconState);
void ctrl_on_toolbar_synchronised();

void ctrl_show_lister_area(void);
void ctrl_hide_lister_area(void);

void ctrl_hide_misc_screen(void);

listerState_t getListerState();
void setListerState(listerState_t newstate);
gboolean listerIsErrorScreenVisible(void);
void listerHideErrorScreen(void);
void listerShowErrorScreen(ctrlErrorType_e errorType);
void listerShowUSBScreen(void);
void listerHideUSBScreen(void);

void ctrl_update_pagebar(void);
void ctrl_shutdown(void);

void ctrl_display_search_result(void);
void ctrl_hide_search_wnd(void);

// show absolute current location information 
const char * ctrl_get_abscurr_location(char* sPath,int iSize);
gboolean ctrl_location_is_outbox(const char* location);

// for pincode 
void ctrl_pincode_setting_init(void);

void ctrl_show_pincode_screen(showPincodeReason_t reason, on_passed_callback_t* on_passed_callback, on_cancel_callback_t* on_cancel_callback);
void ctrl_hide_pincode_screen (void);

gboolean ctrl_pincode_is_for_locking(void);
void ctrl_pincode_overview_button_clicked(void);
void ctrl_pincode_connect_button_clicked (void);
void ctrl_pincode_button_clicked(void);

gboolean ctrl_is_connect_from_pincode (void);
void ctrl_connect_back_to_pincode_screen(void);

void ctrl_startup(void);

void ctrl_export_config(void);
void ctrl_handle_import_config(void);

gboolean on_idle_connect_start_background(gpointer p);
void ctrl_connect(connectScr_e screen, e_ctrl_connect_reason_t reason);

#ifdef __cplusplus
}
#endif  //* __cplusplus */

#endif //__CONTROL_MODULE__H__
