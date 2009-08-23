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
 * \file icons.c
 * \brief contentLister - icon handling
 *
 * <File description>
 *
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */
#include <stdio.h>

#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "erMdsContent.h"
#include "icons.h"

#include "Geometry.h"

static GdkPixbuf *g_icons[ICON_COUNT];


void icons_init()
{
    int W=_G.icon.w-11; int H=_G.icon.h-11;
    GError *error = NULL;

    g_icons[clFolderIcon] = icons_load(DATA_DIR "/Icon_folder.png");
    if (NULL == g_icons[clFolderIcon])
    {
        CL_ERRORPRINTF("clFolderIcon - error %s", error->message);
    }

    g_icons[clUnknownIcon] = icons_load(DATA_DIR "/Icon_unknown.png");
    if (NULL == g_icons[clUnknownIcon])
    {
        CL_ERRORPRINTF("clUnknownIcon - error %s", error->message);
    }

    g_icons[clBookIcon] = icons_load(DATA_DIR "/Icon_books.png");
    if (NULL == g_icons[clBookIcon])
    {
        CL_ERRORPRINTF("clBookIcon - error %s", error->message);
    }

    g_icons[clDocumentIcon] = icons_load(DATA_DIR "/Icon_documents.png");
    if (NULL == g_icons[clDocumentIcon])
    {
        CL_ERRORPRINTF("clDocumentIcon - error %s", error->message);
    }

    g_icons[clMagazineIcon] = icons_load(DATA_DIR "/Icon_magazines.png");
    if (NULL == g_icons[clMagazineIcon])
    {
        CL_ERRORPRINTF("clMagazineIcon - error %s", error->message);
    }

    g_icons[clMiscIcon] = icons_load(DATA_DIR "/Icon_misc.png");
    if (NULL == g_icons[clMiscIcon])
    {
        CL_ERRORPRINTF("clMiscIcon - error %s", error->message);
    }

    g_icons[clMp3Icon] = icons_load(DATA_DIR "/Icon_mp3.png");
    if (NULL == g_icons[clMp3Icon])
    {
        CL_ERRORPRINTF("clMp3Icon - error %s", error->message);
    }

    g_icons[clNewspaperIcon] = icons_load(DATA_DIR "/Icon_newspapers.png");
    if (NULL == g_icons[clNewspaperIcon])
    {
        CL_ERRORPRINTF("clNewspaperIcon - error %s", error->message);
    }

    g_icons[clNoteIcon] = icons_load(DATA_DIR "/Icon_notes.png");
    if (NULL == g_icons[clNoteIcon])
    {
        CL_ERRORPRINTF("clNoteIcon - error %s", error->message);
    }

    g_icons[clPhotoIcon] = icons_load(DATA_DIR "/Icon_photos.png");
    if (NULL == g_icons[clPhotoIcon])
    {
        CL_ERRORPRINTF("clPhotoIcon - error %s", error->message);
    }

    g_icons[clReviewIcon] = icons_load(DATA_DIR "/Icon_reviews.png");
    if (NULL == g_icons[clReviewIcon])
    {
        CL_ERRORPRINTF("clReviewIcon - error %s", error->message);
    }

    g_icons[clOutboxIcon] = icons_load(DATA_DIR "/Icon_outbox.png");
    if (NULL == g_icons[clReviewIcon])
    {
        CL_ERRORPRINTF("clReviewIcon - error %s", error->message);
    }
}


void icons_destroy()
{
    int     index;

    CL_LOGPRINTF("entry");

    for (index = 0; index < ICON_COUNT; index++)
    {
        if (g_icons[index])
        {
            g_object_unref(g_icons[index]);
            g_icons[index] = NULL;
        }
    }

    CL_LOGPRINTF("exit");
}


GdkPixbuf *icons_get(unsigned int Icon_ID)
{
    if ((Icon_ID < ICON_COUNT))
    {
        return g_icons[Icon_ID];
    }
    else
    {
        return NULL;
    }
}

GdkPixbuf *icons_load(const char *location)
{
    GdkPixbuf *image = NULL;
    GError *error = NULL;

    if (location)
    {
        int W=_G.icon.w-11; int H=_G.icon.h-11;
        image = gdk_pixbuf_new_from_file_at_size(location, W, H, &error);

        if (NULL == image)
        {
            CL_WARNPRINTF("location %s - error %s", location, error->message);
        }
    }
    return image;
}

void icons_unload(GdkPixbuf *image)
{
    if (image)
    {
        g_object_unref(G_OBJECT(image));
    }
}
