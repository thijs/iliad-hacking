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

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 *
 */

// config.h is generated based on the directives placed in the configure.ac 
// file and user input supplied to the configure script

#define _GNU_SOURCE

#include <config.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include <liberregxml/erregapi.h>
#include <libergtk/ergtk.h>
#include <liberipc/eripctoolbar.h>
#include <libermanifest/ermanifest.h>

#include "contentListerLog.h"
#include "erConnect.h"
#include "system.h"
#include "gtkPincodeScreen.h"
#include "control.h"
#include "cursor.h"
#include "displayUpdate.h"
#include "erMdsContent.h"
#include "gtkDistListWnd.h"
#include "control_share.h"
#include "erbusy.h"
#include "sign.h"
#include "system.h"
#include "toolbar.h"
#include "pagebar.h"
#include "languages.h"

// local types
typedef enum
        {
            dl_approve = 0,
            dl_reject,
            dl_undefined
        } dist_list_reason_e;

// local vars
static struct
       {
            int            index;
            gboolean       do_move;
            gboolean       do_sign;
            ContentLister* theContentLister;
       } g_item_to_be_shared;

static const mdsSelectionList_t* g_selectionList = NULL;  // data for selections lists
static erGtkSelectionGroup**     g_selection_groups;


// internally used functions
static gboolean ctrl_send_to_outbox(const gchar* szSourcename, const gboolean bDeleteOriginal);
static gchar*   ctrl_sign_container(const char* szSourcename);
static void     ctrl_sign_pincode_ok(void);
static void     ctrl_sign_pincode_quit(void);
static gboolean ctrl_show_sel_list_wnd(int itemIndex, ContentLister* theContentLister);

// callbacks 



// Send specified item to the outbox directory on the same device
static gboolean ctrl_send_to_outbox(const gchar* szSourcename, const gboolean bDeleteOriginal)
{
    gboolean        bRet = FALSE; // not ok (yet)
    const gchar*    device       = NULL;
    const gchar*    szFilename   = NULL;
    gchar*          szTargetname = NULL;
    int             argc;
    char*           argv[10];
    struct stat     status;
    int             rc;
    int             i;
    char*           cp;

    CL_CONTROLPRINTF("entry: item [%s] delete [%d]", szSourcename, bDeleteOriginal);
    
    // find device on which this item is located
    device = ctrl_is_trusted_path(szSourcename);
    if (device == NULL)
    {
        // no device found
        CL_ERRORPRINTF("Item not on a trusted filesystem [%s]", szSourcename);
    }
    else
    {
        // determine target name
        szFilename = strrchr(szSourcename, '/');
        g_return_val_if_fail((szFilename != NULL), FALSE);
        szFilename++;

        // create outbox if needed
        szTargetname = alloca( strlen(device) + 1 + strlen(OUTBOX_NAME) + 1 + strlen(szFilename) + 3 + 1 );
        g_assert(szTargetname != NULL);
        sprintf(szTargetname, "%s/" OUTBOX_NAME, device);
        (void)mkdir(szTargetname, 0755);
        
        // check target already in outbox
        strcat(szTargetname, "/");
        strcat(szTargetname, szFilename);
        //    add a sequence number, in case of duplicate filenames
        cp = szTargetname + strlen(szTargetname);
        for (i = 1 ; lstat(szTargetname, &status) == 0  && i <= 99 ; i++)
        {
            sprintf(cp, "_%02d", i);
        }
        if (i > 99)
        {
            CL_ERRORPRINTF("Target already exists [%s]", szTargetname);
        }
        else
        {
            // send item to outbox
            if (bDeleteOriginal)
            {
                // move item to outbox
                rc = rename(szSourcename, szTargetname);
                if (rc == 0)
                {
                    sync();
                    bRet = TRUE;  // success
                }
                else
                {
                    CL_ERRORPRINTF( "Cannot rename [%s] to [%s] - error [%d] [%s]",
                                     szSourcename, szTargetname, errno, strerror(errno) );
                }
            }
            else
            {
                // copy item to outbox
                //     command = cp -r <source-container> <target-container>
                argc = 0;
                argv[argc++] = "cp";
                argv[argc++] = "-r";
                //
                cp = alloca( strlen(szSourcename) + 1 );
                g_assert(cp != NULL);
                strcpy( cp, szSourcename);
                argv[argc++] = cp;
                //
                cp = alloca( strlen(szTargetname) + 1 );
                g_assert(cp != NULL);
                strcpy( cp, szTargetname);
                argv[argc++] = cp;
                //
                argv[argc] = NULL;
                g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
                rc = fork_exec(argc, argv);
                if (rc == 0)
                {
                    sync();
                    bRet = TRUE;  // success
                }
                else
                {
                    CL_ERRORPRINTF( "Cannot copy [%s] to [%s] - error [%d] [%s]",
                                     szSourcename, szTargetname, rc, strerror(rc) );
                }
            }
        }
    }

    // report success or not
    return bRet;
}


// Share the specified content item, i.e. copy to outbox
void ctrl_listItem_share(const int* index_tbl, ContentLister* theContentLister)
{
    gboolean         ok;

    CL_CONTROLPRINTF("entry: index [%d]", *index_tbl);
    g_assert(theContentLister != NULL);

    // find out which item must be shared
    const int  index = *index_tbl;  // only one item expected
    g_assert(index >= 0  &&  index < theContentLister->itemCount);

    // and set a pointer to the item details
    clDisplayItem_t* theItem = theContentLister->items + index;

    erbusy_blink();

    // update the stored index value
    mdsSetIndex(theContentLister->currentContentType, index + ((theContentLister->currentPage - 1) * MAX_ITEMS_ON_ONE_PAGE));

    // remember which item we are sharing
    g_item_to_be_shared.index            = index;
    g_item_to_be_shared.do_move          = FALSE;
    g_item_to_be_shared.do_sign          = FALSE;
    g_item_to_be_shared.theContentLister = theContentLister;

    // convert file to container
    if (theItem->fit == mdsFitFile)
    {
        ctrl_before_file_clicked(theItem);
    }

    // show selection list window
    if (theItem->fit == mdsFitContainer)
    {
        display_update_increase_level(MAIN_WINDOW_EXPOSE_LEVEL);
        ok = ctrl_show_sel_list_wnd(index, theContentLister);
        if (ok)
        {
            display_update_request_screen_refresh(MAIN_WINDOW_EXPOSE_LEVEL);
        }
        else
        {
            CL_WARNPRINTF("-- cannot show dist_list_wnd");
            toolbar_setIconState(iconID_share, iconState_normal);
            toolbar_synchronise();
        }
    }
    else
    {
        CL_WARNPRINTF("-- not allowed to share item [%s] fit [%d] ", theItem->szFilename, theItem->fit);
        toolbar_setIconState(iconID_share, iconState_normal);
        toolbar_synchronise();
    }
}

void ctrl_doc_share_now(ContentLister* theContentLister)
{
    CL_CONTROLPRINTF("entry");

    gchar* szContainerName;
    gchar* cp;
    
    if (   g_selectionList           != NULL
        && g_item_to_be_shared.index >= 0   )
    {
        clDisplayItem_t* theItem = theContentLister->items + g_item_to_be_shared.index;

        erbusy_blink();

        // save selection lists, hide screen
        ctrl_hide_sel_list_wnd(theContentLister, TRUE);  

        // sign document, when needed
        if (g_item_to_be_shared.do_sign)
        {
            // show pincode screen
            ctrl_show_pincode_screen(signDocument_t, ctrl_sign_pincode_ok, ctrl_sign_pincode_quit);
        }
        else
        {
            // determine containername
            szContainerName = alloca( strlen(theItem->szManifest) + 1 );
            g_assert(szContainerName != NULL);
            strcpy(szContainerName, theItem->szManifest);
            cp = strrchr(szContainerName, '/');
            g_return_if_fail(cp != NULL);
            *cp = '\0';
            
            // move/copy document to outbox
            ctrl_send_to_outbox(szContainerName, g_item_to_be_shared.do_move);
            if (g_item_to_be_shared.do_move)
            {
                ctrl_rescan_current();
            }

            // no more signing to do
            g_item_to_be_shared.index = -1;
        }
    }
}


// Sign container and return the path of the signed container
static gchar* ctrl_sign_container(const char* szSourceContainer)
{
    int         n;
    int         rc;
    erManifest  manifest;
    struct stat status;
    char*       cp;
    const char* ccp;
    int         argc;
    char*       argv[10];
    time_t      t_time;
    struct tm*  tm_time;

    gchar* szDateTime = NULL;
    gchar* szSourceName = NULL;
    gchar* szSourceManifest = NULL;
    gchar* szSourceManifestOld = NULL;
    gchar* szTargetContainer = NULL;
    gchar* szTargetTarfile = NULL;
    gchar* szTargetSignature = NULL;
    regUserProfile_t* theUserProfile = NULL;


    CL_CONTROLPRINTF("entry: source [%s]", szSourceContainer);

    // get current date-time string
    n = 19 + 1;
    szDateTime = alloca(n);
    g_assert(szDateTime != NULL);
    t_time  = time(NULL);
    tm_time = localtime( &t_time );
    snprintf( szDateTime,
              n,
              "%04d-%02d-%02dT%02d:%02d:%02d",
                       tm_time->tm_year + 1900,
                            tm_time->tm_mon + 1,
                                 tm_time->tm_mday,
                                      tm_time->tm_hour,
                                           tm_time->tm_min,
                                                tm_time->tm_sec );

    // get derived file-/directory names
    //     source container name
    szSourceName = strrchr(szSourceContainer, '/');
    if (szSourceName == NULL)
    {
        CL_ERRORPRINTF("Illegal container name [%s]", szSourceContainer);
        return NULL;
    }
    szSourceName++;
    //     target container name
    n = strlen(szSourceContainer) + 1 + 8 + strlen(szDateTime) + 1;
    szTargetContainer = alloca(n);
    g_assert(szTargetContainer != NULL);
    strcpy(szTargetContainer, szSourceContainer);
    cp= strrchr(szTargetContainer, '/');
    g_assert(cp != NULL);
    strcpy(cp, "/Signed_");
    strcat(cp, szDateTime);
    while ( (cp = strchr(szTargetContainer, ':')) != NULL)
    {
        *cp = '-';
    }
    //     target tarfile name
    n = strlen(szTargetContainer) + 1 + strlen(szSourceName) + strlen(TARFILE_EXTENSION) + 1;
    szTargetTarfile = alloca(n);
    g_assert(szTargetTarfile != NULL);
    snprintf(szTargetTarfile, n, "%s/%s%s", szTargetContainer, szSourceName, TARFILE_EXTENSION);
    //     target signature filename
    n = strlen(szTargetContainer) + 1 + strlen(szSourceName) + strlen(SIGNATURE_EXTENSION) + 1;
    szTargetSignature = alloca(n);
    g_assert(szTargetSignature != NULL);
    snprintf(szTargetSignature, n, "%s/%s%s", szTargetContainer, szSourceName, SIGNATURE_EXTENSION);

    // remove target container, if any
    if (lstat(szTargetContainer, &status) == 0)
    {
        CL_WARNPRINTF("-- container already present [%s]", szTargetContainer);
        // command = rm -rf <target-container>
        argc = 0;
        argv[argc++] = "rm";
        argv[argc++] = "-rf";
        //
        cp = alloca( strlen(szTargetContainer) + 1 );
        g_assert(cp != NULL);
        strcpy(cp, szTargetContainer);
        argv[argc++] = cp;
        //
        argv[argc] = NULL;
        g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
        rc = fork_exec(argc, argv);
        if (rc != 0)
        {
            CL_ERRORPRINTF( "Cannot remove container [%s] - error [%d] [%s]",
                             szTargetContainer, rc, strerror(rc) );
            return NULL;
        }
    }

    // update manifest file:
    //     open manifest file
    if (RET_OK != ermXmlOpenManifest(szSourceContainer, &manifest))
    {
        CL_ERRORPRINTF("Cannot open manifest file in [%s]", szSourceContainer);
        return NULL;
    }
    //     remember source manifest filename
    ccp = ermXmlGetFileName(&manifest);
    szSourceManifest = alloca( strlen(ccp) + 1 );
    g_assert(szSourceManifest != NULL);
    strcpy(szSourceManifest, ccp);
    //     save original manifest file
    n = strlen(szSourceManifest) + 4 + 1;
    szSourceManifestOld = alloca(n);
    g_assert(szSourceManifestOld != NULL);
    snprintf(szSourceManifestOld, n, "%s.old", szSourceManifest);
    rc = ermXmlSaveAs(&manifest, szSourceManifestOld);
    if (rc != RET_OK)
    {
        CL_ERRORPRINTF("Cannot save manifest file [%s]", szSourceManifestOld);
        return NULL;
    }
    //     remove existing section document-signing, if any
    if (RET_OK == ermXmlExist(&manifest, XMLNODE_DOCUMENT_SIGNING))
    {
        CL_WARNPRINTF("-- document-signing info already present in [%s]", szSourceManifest);
        ermXmlRemoveNode(&manifest, XMLNODE_DOCUMENT_SIGNING);
    }
    //     add document signing info
    ermXmlNewString(&manifest, XMLNODE_DOCUMENT_SIGNING_PARENT, XMLNODE_DOCUMENT_SIGNING_NODENAME, "");
    ermXmlNewString(&manifest, XMLNODE_DOCUMENT_SIGNING,        "signed-by",                       "");
    ermXmlNewString(&manifest, XMLNODE_DOCUMENT_SIGNING,        "iliad-mac-address",               "");
    ermXmlNewString(&manifest, XMLNODE_DOCUMENT_SIGNING,        "date",                            "");
    //         set user's email address
    theUserProfile = erRegGetUserProfile();
    if (theUserProfile)
    {
        if (theUserProfile->email)
        {
            CL_CONTROLPRINTF("email [%s]", theUserProfile->email);
            ermXmlSetString(&manifest, XMLNODE_DOCUMENT_SIGNING "/signed-by", theUserProfile->email);
        }
        erRegFreeUserProfile(theUserProfile);
        theUserProfile = NULL;
    }
    //         set iLiad's MAC address
    CL_CONTROLPRINTF("MAC address [%s]", get_sysset_macAddress());
    ermXmlSetString(&manifest, XMLNODE_DOCUMENT_SIGNING "/iliad-mac-address", get_sysset_macAddress());
    //         set data-time stamp
    CL_CONTROLPRINTF("date [%s]", szDateTime);
    ermXmlSetString(&manifest, XMLNODE_DOCUMENT_SIGNING "/date", szDateTime);
    //     save manifest file
    rc = ermXmlSaveAs(&manifest, szSourceManifest);
    if (rc != RET_OK)
    {
        CL_ERRORPRINTF("Cannot save manifest file [%s]", szSourceManifest);
        (void)rename(szSourceManifestOld, szSourceManifest);
        return NULL;
    }
    ermXmlClose(&manifest);

    // create target container
    rc = mkdir(szTargetContainer, 0755);
    if (rc != 0)
    {
        CL_ERRORPRINTF("Cannot not create folder [%s] - error [%d] [%s]", szTargetContainer, errno, strerror(errno));
        (void)rename(szSourceManifestOld, szSourceManifest);
        return NULL;
    }

    // combine source container into a tarball in target container
    rc = chdir(szSourceContainer);
    if (rc != 0)
    {
        CL_ERRORPRINTF("Cannot chdir to [%s] - error [%d] [%s]", szSourceContainer, errno, strerror(errno));
        (void)rename(szSourceManifestOld, szSourceManifest);
        return NULL;
    }
    rc = chdir("..");
    if (rc != 0)
    {
        CL_ERRORPRINTF("Cannot chdir to [%s/..] - error [%d] [%s]", szSourceContainer, errno, strerror(errno));
        (void)rename(szSourceManifestOld, szSourceManifest);
        return NULL;
    }
    //     command = tar -czf <target-tarfile> <source-directory-name>
    argc = 0;
    argv[argc++] = "tar";
    argv[argc++] = "-czf";
    //
    cp = alloca( strlen(szTargetTarfile) + 1 );
    g_assert(cp != NULL);
    strcpy(cp, szTargetTarfile);
    argv[argc++] = cp;
    //
    cp = alloca( strlen(szSourceName) + 1 );
    g_assert(cp != NULL);
    strcpy(cp, szSourceName);
    argv[argc++] = cp;
    //
    argv[argc] = NULL;
    g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
    rc = fork_exec(argc, argv);
    if (rc == 0)
    {
        sync();
    }
    else
    {
        CL_ERRORPRINTF( "Cannot create tarfile [%s] - error [%d] [%s]",
                         szTargetTarfile, rc, strerror(rc) );
        (void)unlink(szTargetTarfile);
        (void)rmdir(szTargetContainer);
        (void)rename(szSourceManifestOld, szSourceManifest);
        return NULL;
    }

    // add and verify signature to target container
    rc = iliad_create_signature(szTargetTarfile, szTargetSignature);
    if (rc == ERR_SUCCESS)
    {
        rc = iliad_verify_signature(szTargetTarfile, szTargetSignature);
    }
    if (rc != ERR_SUCCESS)
    {
        CL_ERRORPRINTF( "Cannot sign, iliad_create_signature returns [%d] on [%s]", rc, szTargetSignature);
        (void)unlink(szTargetSignature);
        (void)unlink(szTargetTarfile);
        (void)rmdir(szTargetContainer);
        (void)rename(szSourceManifestOld, szSourceManifest);
        return NULL;
    }

    // add manifest file to target container
    //     command = cp <source-manifest> <target-container>
    argc = 0;
    argv[argc++] = "cp";
    //
    cp = alloca( strlen(szSourceManifest) + 1 );
    g_assert(cp != NULL);
    strcpy( cp, szSourceManifest);
    argv[argc++] = cp;
    //
    cp = alloca( strlen(szTargetContainer) + 1 );
    g_assert(cp != NULL);
    strcpy( cp, szTargetContainer);
    argv[argc++] = cp;
    //
    argv[argc] = NULL;
    g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
    rc = fork_exec(argc, argv);
    if (rc == 0)
    {
        sync();

        // remove start-page entry from target manifest
        //     open manifest file
        if (RET_OK != ermXmlOpenManifest(szTargetContainer, &manifest))
        {
            CL_ERRORPRINTF("Cannot open manifest file in [%s]", szTargetContainer);
        }
        else
        {
            if (RET_OK == ermXmlExist(&manifest, XMLNODE_STARTPAGE))
            {
                ermXmlRemoveNode(&manifest, XMLNODE_STARTPAGE);
            }
            //     save manifest file
            rc = ermXmlSaveAndClose(&manifest);
            if (rc != RET_OK)
            {
                CL_ERRORPRINTF("Cannot save manifest file in [%s]", szTargetContainer);
            }
        }
    }
    else
    {
        CL_ERRORPRINTF( "Cannot copy [%s] to [%s] - error [%d] [%s]",
                         szSourceManifest, szTargetContainer, rc, strerror(rc) );
    }

    // report what happened
    return strdup(szTargetContainer);
}


// Pincode accepted, now approve/reject the document
static void ctrl_sign_pincode_ok()
{
    CL_CONTROLPRINTF("entry");

    ContentLister* theContentLister = g_item_to_be_shared.theContentLister;
    g_assert(theContentLister != NULL);

    int itemIndex = g_item_to_be_shared.index;
    g_assert(itemIndex >= 0  &&  itemIndex < theContentLister->itemCount);

    clDisplayItem_t* theItem = theContentLister->items + itemIndex;
    g_assert(theItem->fit == mdsFitContainer);

    gchar* szContainer       = NULL;
    gchar* szSignedContainer = NULL;
    gchar* cp;
    int    rc;
    int    argc;
    char*  argv[10];


    erbusy_blink();

    // determine containername
    szContainer = alloca( strlen(theItem->szManifest) + 1 );
    g_assert(szContainer != NULL);
    strcpy(szContainer, theItem->szManifest);
    cp = strrchr(szContainer, '/');
    g_return_if_fail(cp != NULL);
    *cp = '\0';

    // sign document
    szSignedContainer = ctrl_sign_container(szContainer);
    if (szSignedContainer)
    {
        // move signed container to outbox
        ctrl_send_to_outbox(szSignedContainer, TRUE);
        free(szSignedContainer);

        // remove original, when needed
        if (g_item_to_be_shared.do_move)
        {
            // command = rm -rf <container>
            argc = 0;
            argv[argc++] = "rm";
            argv[argc++] = "-rf";
            //
            cp = alloca( strlen(szContainer) + 1 );
            g_assert(cp != NULL);
            strcpy(cp, szContainer);
            argv[argc++] = cp;
            //
            argv[argc] = NULL;
            g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
            rc = fork_exec(argc, argv);
            if (rc != 0)
            {
                CL_ERRORPRINTF( "Cannot remove container [%s] - error [%d] [%s]",
                                 szContainer, rc, strerror(rc) );
            }
        }
    }
    else
    {
        CL_ERRORPRINTF("Failed to sign container [%s]", szContainer);
    }

    // no more signing to do
    g_item_to_be_shared.index = -1;

    // show new situation
    ctrl_rescan_current();
}

// Pincode quit, now re-display original screen
static void ctrl_sign_pincode_quit()
{
    CL_CONTROLPRINTF("entry");

    // no more signing to do
    g_item_to_be_shared.index = -1;

    // show new situation
    ctrl_rescan_current();
}


// show selection-list window
static gboolean ctrl_show_sel_list_wnd(int itemIndex, ContentLister* theContentLister)
{
    CL_CONTROLPRINTF("entry");

    g_assert(theContentLister != NULL);
    g_assert(itemIndex >= 0  &&  itemIndex < theContentLister->itemCount);
    clDisplayItem_t* theItem = theContentLister->items + itemIndex;

    gboolean found_list_to_display = FALSE;  // return value

    int   i;
    int   sel_num;
    int   sel_idx;
    int   item_num;
    int   item_idx;
    
    const gchar**             item_tbl  = NULL;
    const mdsSelection_t*     selection = NULL;
    const mdsSelectionItem_t* item      = NULL;
    erGtkSelectionGroup*      sel_group = NULL;
    GtkToggleButton*          button    = NULL;
    GtkWidget*                list_item = NULL;;


    if (getListerState() == STATE_MISC)
    {
        // already handling an item: ignore
        CL_ERRORPRINTF("getListerState returns STATE_MISC");
        return FALSE;  // error
    }
    if (g_selectionList)
    {
        CL_ERRORPRINTF("g_selectionList present [%p]", g_selectionList);
        return FALSE;  // error
    }

    setListerState(STATE_MISC);
    erbusy_blink();

    // read distributionlist from manifest
    g_selectionList = erClXmlFetchSelectionList(theItem->szManifest);
    g_assert(g_selectionList != NULL);
    CL_LOGPRINTF("erClXmlFetchSelectionList returns [%d] items", g_selectionList->num_selections);

    // get special values from selection lists
    erClXmlGetSelectionListItemState(g_selectionList, SELECTION_TRANSFER_MODE,    ITEM_TRANSFER_MOVE, &g_item_to_be_shared.do_move);
    erClXmlGetSelectionListItemState(g_selectionList, SELECTION_DOCUMENT_SIGNING, ITEM_SIGN_YES,      &g_item_to_be_shared.do_sign);

    // increase level to prevent display from updating in checkBtn 
    display_update_increase_level(LISTER_EXPOSE_LEVEL);

    // show selection lists as read from manifest
    sel_num   = g_selectionList->num_selections;
    selection = g_selectionList->selections;
    
    g_selection_groups = g_new0(erGtkSelectionGroup*, sel_num + 2);
    g_assert(g_selection_groups);

    for (sel_idx = 0 ; sel_idx < sel_num ; sel_idx++, selection++)
    {
        if (selection->hide == FALSE)
        {
            found_list_to_display = TRUE;

            // build table with item names
            item_num = selection->num_items;
            item_tbl = alloca((item_num + 1) * sizeof(gchar*));
            item     = selection->items;
            for (item_idx = 0 ; item_idx < item_num ; item_idx++, item++)
            {
                item_tbl[item_idx] = item->name;
            }
            item_tbl[item_idx] = NULL;

            // add this selection to screen layout
            sel_group = dist_list_wnd_add_list( selection->display_as,
                                                selection->title,
                                                selection->instruction,
                                                item_tbl );
            g_selection_groups[sel_idx] = sel_group;

            // set buttons state
            // Note: default is not active, so we only set the active buttons
            item = selection->items;
            for (item_idx = 0 ; item_idx < item_num ; item_idx++, item++)
            {
                if (item->state)
                {
                    button = ergtk_selection_group_get_button(sel_group, item_idx);
                    if (button)
                    {
                        gtk_toggle_button_set_active(button, TRUE);
                    }
                }
            }

            // set selectiongroup details
            if (selection->constant)
            {
                ergtk_selection_group_freeze_buttons(sel_group, TRUE);
            }
            else
            {
                ergtk_selection_group_set_details( sel_group, selection->min_selected, selection->max_selected);
            }
        }
    }

    item_tbl    = alloca(sizeof(gchar*));
    item_tbl[0] = NULL;
    sel_group = dist_list_wnd_add_list("default", "", _("Click the 'Share' icon to add this document to your outbox."), item_tbl);
    ergtk_selection_list_show_titlebar(ERGTK_SELECTION_LIST(sel_group), FALSE);
        
    dist_list_wnd_goto_page(1);

    // hide not-selected lister items, then show distribution list screen
    for (i = 0 ; i < MAX_ITEMS_ON_ONE_PAGE ; i++)
    {
        if (i != itemIndex)
        {
            list_item = lsGetListerItem(theContentLister->lister, i);
            gtk_widget_hide(list_item);
        }
    }
    gtk_widget_show(theContentLister->distListWnd);

    return found_list_to_display;
}

// close selection-list window
void ctrl_hide_sel_list_wnd(ContentLister* theContentLister, gboolean bSave)
{
    CL_CONTROLPRINTF("entry bSave[%d]", bSave);

    const mdsSelection_t*  selection = NULL;
    erGtkSelectionGroup*   sel_group;

    int  sel_num;   // number of selections
    int  sel_idx;   // selection index
    int  item_num;  // number of items
    int  item_idx;  // item index

    int        button_idx;
    gint*      button_ids    = NULL;
    gboolean*  button_states = NULL;


    if ( (getListerState() == STATE_MISC) 
          && (theContentLister->distListWnd)
          && GTK_WIDGET_VISIBLE(theContentLister->distListWnd) )
    {
        // save states from selection lists
        if (bSave  &&  g_selectionList  &&  g_selection_groups)
        {
            sel_num   = g_selectionList->num_selections;
            selection = g_selectionList->selections;
            for (sel_idx = 0 ; sel_idx < sel_num ; sel_idx++, selection++)
            {
                item_num  = selection->num_items;
                sel_group = g_selection_groups[sel_idx];

                // update XML item states from button states
                if (selection->constant == FALSE  &&  sel_group)
                {
                    button_ids    = alloca(item_num * sizeof(gint)    );
                    button_states = alloca(item_num * sizeof(gboolean));
                    g_assert(button_ids    != NULL);
                    g_assert(button_states != NULL);
                    ergtk_selection_group_get_selected_buttons(ERGTK_SELECTION_GROUP(sel_group), button_ids, item_num);

                    // set item state
                    // Note: button_ids[] sorted chronologically, NOT on button_id value
                    //   set all items to FALSE
                    for (item_idx = 0 ; item_idx < item_num ; item_idx++)
                    {
                        button_states[item_idx] = FALSE;
                    }
                    //   set selected items to TRUE
                    for (button_idx = 0 ; button_idx < item_num ; button_idx++)
                    {
                        item_idx = button_ids[button_idx];
                        if (item_idx >= 0  &&  item_idx < item_num)
                        {
                            button_states[item_idx] = TRUE;
                        }
                    }
                    //   update XML according to states
                    for (item_idx = 0 ; item_idx < item_num ; item_idx++)
                    {
                        erClXmlSetSelectionItem(g_selectionList, sel_idx, item_idx, button_states[item_idx]);
                    }
                }
            }

            // save updated states to manifest file
            erClXmlSaveSelectionList(g_selectionList);
        }

        // get special values from selection lists
        erClXmlGetSelectionListItemState(g_selectionList, SELECTION_TRANSFER_MODE,    ITEM_TRANSFER_MOVE, &g_item_to_be_shared.do_move);
        erClXmlGetSelectionListItemState(g_selectionList, SELECTION_DOCUMENT_SIGNING, ITEM_SIGN_YES,      &g_item_to_be_shared.do_sign);

        // hide distribution list window
        dist_list_wnd_clear();
        gtk_widget_hide(theContentLister->distListWnd);

        // re-display all items
        ctrl_rescan_current();

        // show contentlister page
        gtk_widget_show(theContentLister->listerArea);
        gtk_widget_grab_focus(theContentLister->listerArea);
        setListerState(STATE_NORMAL);
    }

    // free g_selectionList
    erClXmlFreeSelectionList(g_selectionList);
    g_selectionList = NULL;

    // free g_selection_groups
    g_free(g_selection_groups);
    g_selection_groups = NULL;

    CL_CONTROLPRINTF("leave");
}

