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
 * \file stack.c
 * \brief - eReader "content keys (newspaper, book, notes,...) history handling                  
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
#include <string.h>
#include <glib.h>

#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "stack.h"


static GArray *g_stacks[TOTAL_STACK_COUNT];

void stackInitAll(void)
{
    int     index;

    CL_STACKPRINTF("entry");

    for (index = 0; index < TOTAL_STACK_COUNT; index++)
    {
        g_stacks[index] = g_array_sized_new(FALSE, TRUE, sizeof(stItem_t), INITIAL_STACK_ARRAY_SIZE);
    }
}

void stackDestroyAll(void)
{
    int     index;

    CL_STACKPRINTF("entry");

    for (index = 0; index < TOTAL_STACK_COUNT; index++)
    {
        if (g_stacks[index])
        {
            stackClear(index);
            g_array_free(g_stacks[index], TRUE);
            g_stacks[index] = NULL;
        }
    }
}

void stackPush(st_ContentType_e contentType, stItem_t * stackItem)
{
    if (stackItem)
    {
        CL_STACKPRINTF("%d-0x%x", contentType, (unsigned int) stackItem);
        g_array_append_val(g_stacks[contentType], *stackItem);
    }
}

void stackPop(st_ContentType_e contentType)
{
    stItem_t *theItem = NULL;

    if (g_stacks[contentType]->len > 0)
    {
        CL_STACKPRINTF("%d-0x%x", contentType,
                       (unsigned int) &g_array_index(g_stacks[contentType], stItem_t,
                                                     (g_stacks[contentType]->len - 1)));

        theItem = &g_array_index(g_stacks[contentType], stItem_t, (g_stacks[contentType]->len - 1));

        // free the memory referenced by this item
        if (theItem)
        {
            if (theItem->dirArray)
            {
                CL_STACKPRINTF("dirArray - 0x%x", (unsigned int) theItem->dirArray);
                g_array_free(theItem->dirArray, TRUE);
                theItem->dirArray = NULL;
            }
        }

        g_array_remove_index(g_stacks[contentType], (g_stacks[contentType]->len - 1));
    }
    return;
}

stItem_t *stackPeek(st_ContentType_e contentType)
{
    stItem_t *theItem = NULL;

    if (g_stacks[contentType]->len > 0)
    {
        CL_STACKPRINTF("%d-0x%x", contentType,
                       (unsigned int) &g_array_index(g_stacks[contentType], stItem_t,
                                                     (g_stacks[contentType]->len - 1)));

        theItem = &g_array_index(g_stacks[contentType], stItem_t, (g_stacks[contentType]->len - 1));
    }

    return theItem;
}

int stackHeight(st_ContentType_e contentType)
{
    CL_STACKPRINTF("%d-%d", contentType, g_stacks[contentType]->len);

    return g_stacks[contentType]->len;
}

void stackClear(st_ContentType_e contentType)
{
    int     index;
    stItem_t *theItem = NULL;

    CL_STACKPRINTF("type %d - length %d", contentType, g_stacks[contentType]->len);

    // clear all reference values
    if (g_stacks[contentType]->len > 0)
    {
        for (index = 0; index < g_stacks[contentType]->len; index++)
        {
            theItem = &g_array_index(g_stacks[contentType], stItem_t, index);

            if (theItem->dirArray)
            {
                CL_STACKPRINTF("dirArray - 0x%x", (unsigned int) theItem->dirArray);
                g_array_free(theItem->dirArray, TRUE);
                theItem->dirArray = NULL;
            }
        }

        // clear the rest of the array
        g_array_remove_range(g_stacks[contentType], 0, g_stacks[contentType]->len);
    }
}

void stackItemDestroy(stItem_t * stackItem)
{
    CL_STACKPRINTF("0x%x", (unsigned int) stackItem);

    if (stackItem)
    {
        if (stackItem->dirArray)
        {
            CL_STACKPRINTF("dirArray - 0x%x", (unsigned int) stackItem->dirArray);
            g_array_free(stackItem->dirArray, TRUE);
        }

        g_free(stackItem);
        stackItem = NULL;
    }
    else
    {
        CL_STACKPRINTF("stackItem ==  NULL");
    }
}

void stackItemDisplay(stItem_t * stackItem)
{
    int     index;
    mdsDirectory_t *directory;

    CL_STACKPRINTF("0x%x", (unsigned int) stackItem);

    if (stackItem)
    {
        if (stackItem->dirArray)
        {
            CL_STACKPRINTF("dirArray - 0x%x", (unsigned int) stackItem->dirArray);

            for (index = 0; index < stackItem->dirArray->len; index++)
            {
                directory = &g_array_index(stackItem->dirArray, mdsDirectory_t, index);
                CL_STACKPRINTF("dir : %s", directory->szFilename);
            }
        }

        CL_STACKPRINTF("sort fieldType %d", stackItem->sort.ft);
        CL_STACKPRINTF("sort ascending %d", stackItem->sort.ascending);
        CL_STACKPRINTF("index %d", stackItem->index);
    }
}

void stackDisplay(st_ContentType_e contentType)
{
    int     index;
    stItem_t *theItem = NULL;

    // clear all reference values
    if (g_stacks[contentType]->len > 0)
    {
        for (index = 0; index < g_stacks[contentType]->len; index++)
        {
            theItem = &g_array_index(g_stacks[contentType], stItem_t, (g_stacks[contentType]->len - 1));
            CL_STACKPRINTF("\n*****************[%d]**********************", index);
            stackItemDisplay(theItem);
            CL_STACKPRINTF("\n*******************************************\n");
        }
    }
}


gboolean stackIsStorage(st_ContentType_e contentType)
{
    if (   (contentType >= st_StorageTypeCF)
        || (contentType <= st_StorageTypeSD)
        || (contentType <= st_StorageTypeUSB)
        || (contentType <= st_StorageTypeMain) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

gboolean stackIsArchive(st_ContentType_e contentType)
{
    if (   (contentType == st_ContentTypeBooks)
        || (contentType == st_ContentTypeNews)
        || (contentType == st_ContentTypeDocs)
        || (contentType == st_ContentTypeNotes) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
