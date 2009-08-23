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
#include <glib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <libermanifest/ermanifest.h>

#include "stack.h"
#include "lister.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "metadataStoreManager.h"

#define MAX_DESC        80

#define PREVIOUS        0x0001
#define NEXT            0x0002
#define CURRENT         0x0003
#define DISPLAY_STACK   0x0004
#define MENU_QUIT       0x0400

static int lastCmd = -1;

struct command
{
    char    key;
    char   *desc;
    int     code;
};

struct command commands[] = {
    {'1', "Scan using the current content information", CURRENT},
    {'2', "Scan NEXT", NEXT},
    {'3', "Scan PREVIOUS", PREVIOUS},
    {'4', "Display Stack", DISPLAY_STACK},
    {0, NULL, 0},
    {'Q', "Quit", MENU_QUIT}
};

int     menu(char *mode);

int main(int argc, char *argv[])
{
    int     result;
    int     command;
    char    commandDesc[MAX_DESC];
    char    mode[80];
    mdsDirectory_t dir_1;

    //int index;

    // Initialize some variables
    strcpy(commandDesc, "NO DESCRIPTION");

    mdsInit();

    do
    {
        command = menu(mode);

        switch (command)
        {
        case CURRENT:
            result = mdsScanCurrent(st_ContentTypeBooks);

            if (result != ERMDS_CONTENT_SCAN_OK)
            {
                printf("mdsScanCurrent returned %d\n", result);
            }
            else
            {
                displayScanResult();
            }
            break;

        case PREVIOUS:
            result = mdsScanPrevious(st_ContentTypeBooks);

            if (result != ERMDS_CONTENT_SCAN_OK)
            {
                printf("mdsScanPrevious returned %d\n", result);
            }
            else
            {
                displayScanResult();
            }
            break;

        case NEXT:
            strcpy(dir_1.szFilename, "/home/ann/contentExample/test_root/dir_1");

            printf("folder %s\n", dir_1.szFilename);

            result = mdsScanNext(st_ContentTypeBooks, dir_1.szFilename, 0);

            if (result != ERMDS_CONTENT_SCAN_OK)
            {
                printf("mdsScanPrevious returned %d\n", result);
            }
            else
            {
                displayScanResult();
            }
            break;

        case DISPLAY_STACK:
            stackDisplay(st_ContentTypeBooks);
            break;
        }

    }
    while (command != MENU_QUIT);

    mdsDestroy();

    exit(0);
}

int menu(char *mode)
{
    int     i;
    char    ch;

    printf("\n\nMenu: \n");

    if (lastCmd != -1)
        printf("Last selected command = '%s'\n\n", commands[lastCmd].desc);
    else
        printf("\n\n");

    for (i = 0; i < sizeof(commands) / sizeof(*commands); i++)
    {
        if (commands[i].key == 0)
            printf("\n");
        else
            printf(" %c - %s\n", commands[i].key, commands[i].desc);
    }

    printf("\nCommand: ");
    fflush(stdout);

    while (1)
    {
        ch = toupper(getchar());

        for (i = 0; i < sizeof(commands) / sizeof(*commands); i++)
        {
            if (ch && ch == commands[i].key)
            {
                printf("%c\n\n", ch);
                lastCmd = i;
                return commands[i].code;
            }
        }
    }
}
