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
#include <glib.h>

#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "stack.h"


int main(int argc, char *argv[])
{
    stItem_t *stackItem_A;
    stItem_t *stackItem_B;

    stItem_t *stackItem_1;
    stItem_t *stackItem_2;

    mdsDirectory_t dir_1;
    mdsDirectory_t dir_2;
    mdsDirectory_t dir_3;
    mdsDirectory_t dir_4;
    mdsDirectory_t dir_5;

    stackInitAll();

    strcpy(dir_1.szFilename, "/home/ann/contentExample/E200581216");
    strcpy(dir_2.szFilename, "/home/root");
    strcpy(dir_3.szFilename, "/home/ann");

    strcpy(dir_4.szFilename, "/home/ann/projects");
    strcpy(dir_5.szFilename, "/test");

    // create a stack item  
    stackItem_A = g_new0(stItem_t, 1);
    stackItem_A->dirArray = g_array_sized_new(FALSE, TRUE, ERMDS_MAX_FILENAME_SIZE, INITIAL_DIR_COUNT_ARRAY_SIZE);
    CL_STACKPRINTF("stackItem_A->dirArray - 0x%x", (unsigned int) stackItem_A->dirArray);
    g_array_append_val(stackItem_A->dirArray, dir_1);
    g_array_append_val(stackItem_A->dirArray, dir_2);
    g_array_append_val(stackItem_A->dirArray, dir_3);
    
    stackItem_A->sort.ascending = 1;
    stackItem_A->sort.ft = mdsFieldFile;
    stackPush(st_ContentTypeBooks, stackItem_A);

    // create a stack item  
    stackItem_B = g_new0(stItem_t, 1);
    stackItem_B->dirArray = g_array_sized_new(FALSE, TRUE, ERMDS_MAX_FILENAME_SIZE, INITIAL_DIR_COUNT_ARRAY_SIZE);
    CL_STACKPRINTF("stackItem_B->dirArray - 0x%x", (unsigned int) stackItem_B->dirArray);
    g_array_append_val(stackItem_B->dirArray, dir_4);
    g_array_append_val(stackItem_B->dirArray, dir_5);
    
    stackItem_B->sort.ascending = 0;
    stackItem_B->sort.ft = mdsFieldFile;
    stackPush(st_ContentTypeBooks, stackItem_B);

    CL_STACKPRINTF("height %d", stackHeight(st_ContentTypeBooks));;

    stackItem_2 = stackPeek(st_ContentTypeBooks);
    stackItemDisplay(stackItem_2);
    stackPop(st_ContentTypeBooks);

    stackItem_1 = stackPeek(st_ContentTypeBooks);
    stackItemDisplay(stackItem_1);
    stackPop(st_ContentTypeBooks);

    g_free(stackItem_A);
    g_free(stackItem_B);

    stackDestroyAll();

    return 0;
}
