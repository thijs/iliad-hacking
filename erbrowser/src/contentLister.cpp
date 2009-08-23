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
 * \file contentLister.cpp
 * \brief browser - contentlister interaction with browser application
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#include <liberipc/eripc.h>
#include <liberipc/eripcviewer.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>
#include "browserTypes.h"
#include "contentLister.h"
#include "browserLog.h"
#include "mozilla_api.h"

static erClientChannel_t contentListerChannel;

static void contentlister_store_item_location(int category, char *item, char *location);

void contentlister_init()
{
    BR_IPCPRINTF("entry");
    erIpcStartClient(ER_CONTENTLISTER_CHANNEL, &contentListerChannel);
}


void contentlister_request(erIpcCmd_t command, Ereader * browser)
{
    char   *item = NULL;
    int     category;
    char   *location = NULL;
    int     index;

    BR_IPCPRINTF("command %d", command.cc);

    switch (command.cc)
    {
    case ccVwrStore:
        BR_IPCPRINTF("STORE - nArg %d", command.nArg);
        //check the argument just to besure
        if (command.nArg == 3)
        {
            for (index = 0; index < command.nArg; index++)
            {
                BR_IPCPRINTF("command.arg[%d]=%s", index, command.arg[index]);
            }

            category = atoi(command.arg[1]);
            item = command.arg[2];

            if ((browser) && (browser->mozEmbed))
            {
                BR_IPCPRINTF("gtk_moz_embed_get_location");
                location = gtk_moz_embed_get_location(GTK_MOZ_EMBED(browser->mozEmbed));

                if (location)
                {
                    BR_IPCPRINTF("category=%d item=%s location=%s", category, item, location);
                    contentlister_store_item_location(category, item, location);
                    // gtk_moz_embed_get_location used g_strdup
                    g_free(location);
                }
            }
        }
        else
        {
            BR_WARNPRINTF("ccVwrStore invalid argument count %d", command.nArg);
        }
        break;
    default:
        BR_IPCPRINTF("UNHANDLED COMMAND %d", command.cc);
        break;
    }
    return;
}

/**
 * Send location of the specified category to the contentlister
 *
 * @param category unique id
 * @param item - unique id
 * @param location 
 *
 * @returns -
 */
void contentlister_store_item_location(int category, char *item, char *location)
{
    BR_IPCPRINTF("clStore");
    clStore(contentListerChannel, ER_XHTML_VIEWER_UA_ID, category, item, location);
}
