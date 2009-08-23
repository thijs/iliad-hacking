/*
 * This file is part of browser.
 *
 * browser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * browser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file toolbar.cpp
 * \brief browser - toolbar interaction with browser application
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>
#include <liberipc/eripcviewer.h>
#include <liberipc/eripcicons.h>
#include "browserTypes.h"
#include "toolbar.h"
#include "pagebar.h"
#include "browserSettings.h"
#include "mozilla_api.h"
#include "find.h"
#include "browserLog.h"
#include "displayUpdate.h"
#include "erbusy.h"

static erClientChannel_t toolbarChannel;

void toolbar_init()
{
    int     returnValue;

    // TODO: check this function, it seems broken...
    // MvdW, 2006-03-13

    BR_IPCPRINTF("entry");

    // create/init the channel to communicate with the toolbar service
    returnValue = erIpcStartClient(ER_TOOLBAR_CHANNEL, &toolbarChannel);

    if (returnValue != 0)
    {
        BR_WARNPRINTF("erIpcStartClient returned %d", returnValue);
        return;
    }

    // tell toolbar which icons set to display
    tbSelectIconSet(toolbarChannel, ER_XHTML_VIEWER_UA_ID);
    
    // clear the toolbar
    tbClearIconSet(toolbarChannel, ER_XHTML_VIEWER_UA_ID);

    // add the zoom/unzoom icon to the toolbar
    tbAppendPlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, iconID_fontlarger,  ccVwrToolbar);
    tbAppendPlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, iconID_fontsmaller, ccVwrToolbar);
    //tbAppendPlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, ER_IC_JUMP_TO_PAGE);
    //tbAppendPlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, ER_IC_JUMP_TO_PAGE);
    //tbAppendPlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, ER_IC_JUMP_TO_PAGE);
    
    // In the browser, the keyboard is enabled
    tbSetStatePlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, iconID_keyboard, iconState_normal);

}


void toolbar_key_selected(int iconID, int iconState, Ereader * browser)
{
    float   zoomSize;
    gint    offset;

    BR_IPCPRINTF("iconID %d - iconState %d - browser 0x%x \n", iconID, iconState, browser);
    
    // Busy indication when a toolbar icon is clicked
    erbusy_blink();

    switch (iconID)
    {
        case iconID_fontlarger:
            zoomSize = browser_settings_get_zoom_factor();
            BR_IPCPRINTF("ZOOM IN %f + 0,1", zoomSize);
            zoomSize = zoomSize + ZOOM_STEPSIZE;

            if (zoomSize <= MAX_ZOOM_IN)
            {
                if (zoomSize + ZOOM_STEPSIZE > MAX_ZOOM_IN)
                {
                    tbSetStatePlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, iconID_fontlarger, iconState_grey);
                }
                tbSetStatePlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, iconID_fontsmaller, iconState_normal);

                mozilla_zoom(GTK_MOZ_EMBED(browser->mozEmbed), zoomSize);
                
                // Pagebar offset needs to be updated after fontchange
                offset = mozilla_determine_page_offset(GTK_MOZ_EMBED(browser->mozEmbed));
                pagebar_set_offset(offset);
                pagebar_redraw();
                
                display_update_request_screen_refresh(MOZEMBED_UPDATE_LEVEL, NULL);
                browser_settings_set_zoom_factor(zoomSize);
            }
            else
            {
                BR_IPCPRINTF("new zoom size %f > %f", zoomSize, MAX_ZOOM_IN);
                erbusy_off(); // No longer busy
            }
            break;

        case iconID_fontsmaller:
            zoomSize = browser_settings_get_zoom_factor();
            BR_IPCPRINTF("ZOOM OUT %f - 0,1", zoomSize);
            zoomSize = zoomSize - ZOOM_STEPSIZE;

            if (zoomSize > MAX_ZOOM_OUT)
            {
                if (zoomSize - ZOOM_STEPSIZE <= MAX_ZOOM_OUT)
                {
                    tbSetStatePlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, iconID_fontsmaller, iconState_grey);
                }
                tbSetStatePlatformIcon(toolbarChannel, ER_XHTML_VIEWER_UA_ID, iconID_fontlarger, iconState_normal);

                mozilla_zoom(GTK_MOZ_EMBED(browser->mozEmbed), zoomSize);
                if (mozilla_scrollable(pageDown_t, GTK_MOZ_EMBED(browser->mozEmbed)) == false)
                {
                   // We are at the end of the page. Testing if scrolling down restores
                   // normal view --> at the end of document instead of showing black screen
                   mozilla_scroll(1, GTK_MOZ_EMBED(browser->mozEmbed));
                }
                
                // Pagebar offset needs to be updated after fontchange
                offset = mozilla_determine_page_offset(GTK_MOZ_EMBED(browser->mozEmbed));
                pagebar_set_offset(offset);
                pagebar_redraw();
                
                display_update_request_screen_refresh(MOZEMBED_UPDATE_LEVEL, NULL);
                browser_settings_set_zoom_factor(zoomSize);
            }
            else
            {
                BR_IPCPRINTF("new zoom size %f < %f", zoomSize, MAX_ZOOM_OUT);
                erbusy_off();  // No longer busy
            }
            break;

        /**
        case ccVwrInvokeFindDialog:
            BR_IPCPRINTF("INVOKE FIND DIALOG");
            browser_find_dialog_create(browser);
            break;
        **/

        default:
            BR_IPCPRINTF("Unknown iconID [%d]", iconID);
            erbusy_off();  // No longer busy
            break;
    }

    return;
}
