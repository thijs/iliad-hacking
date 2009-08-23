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
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

/**
 * \file selection.cpp
 * \brief browser - API to identify a selection in an e-reader XHTML file and recreate this selection
 *                  when needed.
 *
 *
 *  Restrictions/Remark : - the sequence of the selections in the selection file should be respected
 *                        - selections are stored in a selection file which is located in the same directory as the 
 *                          original XHTML file
 *                        - selections are put in the actual XHTML file by surrounding the TEXT tags with a <Higlight> tag.
 *                          The actual result is determined by the style associated with this <Highlight> tag in the CSS file
 *                                    HighLight
 *                                    {
 *                                           background-color: Black;
 *                                           color:White;
 *                                    }
 *
 *                        - higlighting should not affect font or other size related things since higlights may not affect 
 *                          the actual page layout. 
 *                        - todo : check if this it true => history should be at least 1 in the mozilla js file in 
 *                                                          so the highlights will be displayed
 *
 * <File description>
 *
 */

#include <stdio.h>
#include <glib.h>
#include <glib/gstdio.h>
#include "browserTypes.h"
#include "selection.h"
#include "selectionFileHandler.h"

/* mozilla specific headers */
#include "nsCOMPtr.h"
#include "nsMemory.h"
#include "gtkmozembed_internal.h"
#include "nsString.h"
#include "nsIWebBrowser.h"
#include "nsIDOMNode.h"
#include "nsIDOMNodeList.h"
#include "nsIDOMNode.h"
#include "nsISelection.h"
#include "nsIDOMRange.h"
#include "nsIDOMDocumentRange.h"
#include "nsIDOMNSRange.h"
#include "nsIDOMCharacterData.h"
#include "nsIDOMWindow.h"
#include "nsIDOMDocument.h"
#include "nsIWebNavigation.h"
#include "nsIDOMHTMLElement.h"
#include "browserLog.h"

#ifdef SELECTION_ENABLED

// internally used routines
static void selection_free_list(GSList * theSelectionList);
static GSList *mozilla_retrieve_selections(GtkMozEmbed * b);
static nsISelection *mozilla_get_selection_object(nsIWebBrowser * wb);
static bool mozilla_selection_get_node_index(nsCOMPtr < nsIDOMNode >
                                             theParentNode, nsCOMPtr < nsIDOMNode > theChildNode, int *parentNodeIndex,
                                             int *childNodeIndex);
static bool mozilla_selection_node_is_valid(nsCOMPtr < nsIDOMNode > theNode);
static void mozilla_display_node_info(nsCOMPtr < nsIDOMNode > theNode);
static void mozilla_disect_range(nsCOMPtr < nsIDOMRange > theRange, nsCOMPtr < nsIDOMNode > theContainerNode, GSList ** theSelectionList);
static bool mozilla_text_node_is_real(nsCOMPtr < nsIDOMNode > theNode);
static bool mozilla_set_selection(GtkMozEmbed * b, SelectionID * theSelection);

/*
 * selection_clear_current_page_selections
 * 
 *     remove the associated selection file, if available 
 *     and reload the XHTML page    
 *
 */
bool selection_clear_current_page_selections(GtkMozEmbed * mozEmbed)
{
    if (selection_file_destroy(mozEmbed) == TRUE)
    {
        //reload the current page without the selections
        gtk_moz_embed_reload(GTK_MOZ_EMBED(mozEmbed), GTK_MOZ_EMBED_FLAG_RELOADCHARSETCHANGE);
        return TRUE;
    }
    return FALSE;
}

/*
 * selection_set_current_page_selections
 * 
 *     retrieve the associated selection file content and update the 
 *     currently displayed XHTML page    
 *
 */
bool selection_set_current_page_selections(GtkMozEmbed * embed)
{
    FILE   *selectionFile;
    GSList *selectionList;
    unsigned int length;
    SelectionID *theSelection;

    // see if there is a selection file associated with this page

    if (selection_file_available(embed) == TRUE)
    {
        BR_LOGPRINTF("selection file available");
        selectionFile = selection_file_init(embed);

        if (selectionFile)
        {
            // retrieve the list of selections available in this html file
            selectionList = selection_file_get_list(selectionFile);
            selection_file_close(selectionFile);

            // add all these selections to the file
            if (selectionList)
            {
                length = g_slist_length(selectionList);
                BR_LOGPRINTF("selection file contains %d selections", length);

                for (unsigned int index = 0; index < length; index++)
                {
                    theSelection = (SelectionID *) g_slist_nth_data(selectionList, index);
                    mozilla_set_selection(embed, theSelection);

                    g_free(theSelection);
                    theSelection = NULL;
                }

                // free the GSList allocated memory
                g_slist_free(selectionList);
            }
        }
    }
    else
    {
        BR_LOGPRINTF("selection file NOT available");
    }

    return TRUE;
}

/*
 * selection_identify_and_store_selection
 * 
 *		identify in the selected XHTML text a list of selections and store these is the appropriate selection file
 *
 */
bool selection_identify_and_store_selection(GtkMozEmbed * mozEmbed)
{
    SelectionID *theSelection;
    FILE   *selectionFile;
    GSList *theSelectionList;
    unsigned int length;

    theSelectionList = mozilla_retrieve_selections(GTK_MOZ_EMBED(mozEmbed));

    if (theSelectionList)
    {
        BR_LOGPRINTF("init selection file");
        selectionFile = selection_file_init(GTK_MOZ_EMBED(mozEmbed));

        if (selectionFile)
        {
            length = g_slist_length(theSelectionList);
            BR_LOGPRINTF("theSelectionList contains %d selections", length);

            for (unsigned int index = 0; index < length; index++)
            {
                theSelection = (SelectionID *) g_slist_nth_data(theSelectionList, index);
                BR_LOGPRINTF("store selection information [%d]", index);
                selection_file_store(selectionFile, theSelection);
            }
            BR_LOGPRINTF("close selection file");
            selection_file_close(selectionFile);

            //reload the current page with the new selections => history 1 needed ??
            //gtk_moz_embed_reload(GTK_MOZ_EMBED(mozEmbed),GTK_MOZ_EMBED_FLAG_RELOADNORMAL);
            gtk_moz_embed_reload(GTK_MOZ_EMBED(mozEmbed), GTK_MOZ_EMBED_FLAG_RELOADCHARSETCHANGE);

        }
        else
        {
            BR_ERRORPRINTF("Can NOT init selection file");
            return FALSE;
        }

        selection_free_list(theSelectionList);
    }
    return TRUE;
}

/*
 * selection_free_list
 * 
 *		free the memory allocated for the selection list
 *
 */
static void selection_free_list(GSList * theSelectionList)
{
    SelectionID *theSelection;
    unsigned int length;

    if (theSelectionList)
    {
        length = g_slist_length(theSelectionList);

        for (unsigned int index = 0; index < length; index++)
        {
            theSelection = (SelectionID *) g_slist_nth_data(theSelectionList, index);
            g_free(theSelection);
            theSelection = NULL;
        }

        // free the GSList allocated memory
        g_slist_free(theSelectionList);
    }
    return;
}

/*
 * mozilla_set_selection
 * 
 *		Method used to set a selected part in a webpage 
 *      
 *      return value : FALSE when the selection is invalid
 *					   TRUE  when the selection is valid	
 */
static bool mozilla_set_selection(GtkMozEmbed * b, SelectionID * theSelection)
{
    nsAutoString parentTagName;

    nsCOMPtr < nsIWebBrowser > wb;
    nsCOMPtr < nsIDOMDocument > domDoc;
    nsCOMPtr < nsIDOMNodeList > nodeList;
    nsCOMPtr < nsIDOMNodeList > childNodeList;
    nsCOMPtr < nsIDOMNode > parentNode;
    nsCOMPtr < nsIDOMNode > childNode;
    nsCOMPtr < nsIWebNavigation > nav;
    nsCOMPtr < nsIDOMDocumentRange > domDocRange;
    nsCOMPtr < nsIDOMRange > theRange;

    BR_LOGPRINTF("entry");

    if (theSelection)
    {
        g_return_val_if_fail(b != NULL, FALSE);
        gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
        if (!wb)
            return (FALSE);

        nav = do_QueryInterface(wb);
        nav->GetDocument(getter_AddRefs(domDoc));
        if (!domDoc)
            return (FALSE);

        //get the parent node
        parentTagName.AssignWithConversion(theSelection->parentNodeTagName);
        domDoc->GetElementsByTagName(parentTagName, getter_AddRefs(nodeList));
        nodeList->Item(theSelection->parentNodeIndex, getter_AddRefs(parentNode));
        BR_LOGPRINTF("retrieved parent node");
        mozilla_display_node_info(parentNode);

        //get the child nodes
        parentNode->GetChildNodes(getter_AddRefs(childNodeList));
        childNodeList->Item(theSelection->nodeIndex, getter_AddRefs(childNode));

        BR_LOGPRINTF("retrieved child node");
        mozilla_display_node_info(childNode);

        // Create Range object for Selection 
        domDocRange = do_QueryInterface(domDoc);
        domDocRange->CreateRange(getter_AddRefs(theRange));

        theRange->SetStart(childNode, theSelection->selectionStart);
        theRange->SetEnd(childNode, theSelection->selectionStart + theSelection->selectionLength);

        BR_LOGPRINTF("created range");

        nsIDOMElement *newNode;
        nsAutoString tag;

        tag.AssignWithConversion("HIGHLIGHT");
        domDoc->CreateElement(tag, &newNode);
        theRange->SurroundContents(newNode);

        BR_LOGPRINTF("highlight range");
    }
    return FALSE;
}

/*
 * mozilla_retrieve_selections
 * 
 *		Method used to disect the selected part of the webpage and split it up in selection items
 *      
 *      NOTE : an e-reader selection can only contain one RANGE !
 *
 *      returns a list of the valid selections	
 */
static GSList *mozilla_retrieve_selections(GtkMozEmbed * b)
{
    nsCOMPtr < nsIWebBrowser > wb;
    nsCOMPtr < nsISelection > selection;
    nsCOMPtr < nsIDOMRange > theRange;
    nsCOMPtr < nsIDOMNode > commonAncestorContainer;
    PRInt32 rangecount = 0;
    nsresult rv;
    GSList *theSelectionList;

    // check input parameter
    if (b == NULL)
    {
        BR_ERRORPRINTF("invalid input parameter");
        return NULL;
    }

    // init the selection list
    theSelectionList = NULL;

    // retrieve the webbrowser interface    
    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));

    if (wb)
    {
        selection = mozilla_get_selection_object(wb);

        if (selection)
        {
            // retrieve the range count 
            rv = selection->GetRangeCount(&rangecount);
            BR_LOGPRINTF("nsISelection::GetRangeCount() returned %d count = %d", rv, rangecount);

            if (rangecount > 1)
            {
                BR_ERRORPRINTF("invalid RangeCount");
            }
            else
            {
                // retrieve the range, first range has index 0
                rv = selection->GetRangeAt(0, getter_AddRefs(theRange));
                BR_LOGPRINTF("nsISelection::GetRangeAt() for index 0 returned %d", rv);

                if (!theRange)
                {
                    BR_ERRORPRINTF("Not able to get nsIDOMRange object");
                }
                else
                {
                    theRange->GetCommonAncestorContainer(getter_AddRefs(commonAncestorContainer));

                    if (commonAncestorContainer)
                    {
                        // find the text nodes that are intersecting with this range
                        mozilla_disect_range(theRange, commonAncestorContainer, &theSelectionList);

                        // remove the higlights
                        selection->RemoveAllRanges();
                        //release the DOM from keeping track of the ranges boundary points
                        theRange->Detach();
                    }
                    else
                    {
                        BR_ERRORPRINTF("Not able to get commonAncestorContainer object");
                    }

                }               // (!theRange)

            }                   // (rangecount > 1)   

        }                       //(selection)

    }                           // (wb != 0)

    return theSelectionList;
}


/*
 * GetSelectionObject
 * 
 *		retrieve the nsISelection object 	
 */
static nsISelection *mozilla_get_selection_object(nsIWebBrowser * wb)
{
    nsresult rv;

    nsCOMPtr < nsISelection > oNsSelection;
    nsCOMPtr < nsIDOMWindow > oDomWindow;

    // Get the nsIDOMWindow interface in order to get nsISelection 
    rv = wb->GetContentDOMWindow(getter_AddRefs(oDomWindow));

    BR_LOGPRINTF("GetSelectionObject -- nsIWebBrowser::GetContentDOMWindow() rv %d", rv);

    if (!oDomWindow)
    {
        BR_ERRORPRINTF("GetSelectionObject -- Cannot get the  nsIDOMNode Object");
        return NULL;
    }

    // Now get the nsISelection interface
    rv = oDomWindow->GetSelection(getter_AddRefs(oNsSelection));

    BR_LOGPRINTF("GetSelectionObject -- nsIDOMWindow::GetContentDOMWindow()' rv %d", rv);

    if (!oNsSelection)
    {
        BR_LOGPRINTF("GetSelectionObject -- Cannot get the nsISelection Object");
        return NULL;
    }
    return oNsSelection;
}

/*
 * mozilla_disect_range
 * 
 *		parse (recursivly) through all children of theNode (starting with the commonAncestorContainer of the range)
 *      and check if the node is a TEXT node that itersects with the range and determine how to generate the correct
 *      selection identification info.
 *
 *      This routine returns a list of valid selections
 * 	
 */
void mozilla_disect_range(nsCOMPtr < nsIDOMRange > theRange, nsCOMPtr < nsIDOMNode > theNode, GSList ** theSelectionList)
{
    nsCOMPtr < nsIDOMNSRange > theNSRange;
    PRBool  HasChildren;

    nsCOMPtr < nsIDOMNode > theChild;
    nsCOMPtr < nsIDOMNode > childNode;
    PRBool  inRange = FALSE;
    PRUint16 retval;

    nsCOMPtr < nsIDOMNodeList > childNodeList;
    unsigned int childNodeListLength = 0;
    SelectionID *theSelection;

    nsCOMPtr < nsIDOMNode > theParentNode;
    int     parentNodeIndex;
    int     childNodeIndex;
    unsigned int selectionLength;

    nsCOMPtr < nsIDOMCharacterData > charData;
    nsString theParentNodeName;

    nsCOMPtr < nsIDOMNode > startContainer;
    nsCOMPtr < nsIDOMNode > endContainer;
    int     startOffset;
    int     endOffset;

    // retrieve some range info
    theRange->GetStartContainer(getter_AddRefs(startContainer));
    theRange->GetEndContainer(getter_AddRefs(endContainer));
    theRange->GetStartOffset(&startOffset);
    theRange->GetEndOffset(&endOffset);

    theNSRange = do_QueryInterface(theRange);

    if (theNSRange)
    {
        BR_LOGPRINTF("---> got one nsIDOMNSRange object");

        // go througth all the descendants of the theContainerNode to find the text nodes that
        // are part of the selection [text nodes do not have children]

        if (mozilla_selection_node_is_valid(theNode))
        {
            theNSRange->IntersectsNode(theNode, &inRange);

            if (inRange)
            {
                // get the node info to identify the selection                                          

                theNode->GetParentNode(getter_AddRefs(theParentNode));
                theParentNode->GetNodeName(theParentNodeName);
                mozilla_selection_get_node_index(theParentNode, theNode, &parentNodeIndex, &childNodeIndex);

                charData = (do_QueryInterface(theNode));
                charData->GetLength(&selectionLength);

                // create a new selection and fill in the known values                          
                theSelection = g_new0(SelectionID, 1);
                theSelection->nodeIndex = childNodeIndex;
                theSelection->parentNodeIndex = parentNodeIndex;

                char   *tagname = ToNewCString(theParentNodeName);

                strcpy(theSelection->parentNodeTagName, tagname);
                nsMemory::Free(tagname);

                mozilla_display_node_info(theNode);

                // find out some details about the intersection with the range to determine the length and offset of
                // the selection 
                theNSRange->CompareNode(theNode, &retval);

                switch (retval)
                {
                case nsIDOMNSRange::NODE_INSIDE:
                    {
                        BR_LOGPRINTF("######## child  -- NODE_INSIDE #######");
                        theSelection->selectionLength = selectionLength;
                        theSelection->selectionStart = 0;

                    }
                    break;
                case nsIDOMNSRange::NODE_BEFORE:
                    {
                        BR_LOGPRINTF("######## child -- NODE_BEFORE ########");

                        if (theNode == startContainer)
                        {
                            theSelection->selectionStart = startOffset;
                        }
                        else
                        {
                            theSelection->selectionStart = 0;
                        }
                        theSelection->selectionLength = selectionLength - theSelection->selectionStart;

                    }
                    break;
                case nsIDOMNSRange::NODE_AFTER:
                    {
                        if (theNode == endContainer)
                        {
                            theSelection->selectionLength = endOffset;
                        }
                        else
                        {
                            theSelection->selectionLength = selectionLength;
                        }
                        theSelection->selectionStart = 0;

                    }
                    break;
                case nsIDOMNSRange::NODE_BEFORE_AND_AFTER:
                    {
                        BR_LOGPRINTF("######## NODE_BEFORE_AND_AFTER -- ########");
                        // only one node selected
                        theSelection->selectionLength = endOffset - startOffset;
                        theSelection->selectionStart = startOffset;
                    }
                    break;
                }               // switch retval

                // add this selection to the list
                //
                // IMPORTANT REMARK :
                // ------------------
                // g_slist_prepend takes care of the fact that the last identified selection in the range
                // is added first to the selection list. This way, the selection identification (indexes)
                // is not influenced by other selections in this same range that appear earlier in the XHTML file
                // but that still need te be identified.  

                BR_LOGPRINTF("add this selection to the list");
                *theSelectionList = g_slist_prepend(*theSelectionList, theSelection);

            }                   // (inRange)
        }
        else
        {
            theNode->HasChildNodes(&HasChildren);

            if (HasChildren)
            {
                theNode->GetChildNodes(getter_AddRefs(childNodeList));

                if (childNodeList)
                {
                    childNodeList->GetLength(&childNodeListLength);

                    for (unsigned int index = 0; index < childNodeListLength; index++)
                    {
                        childNodeList->Item(index, getter_AddRefs(childNode));
                        mozilla_disect_range(theRange, childNode, theSelectionList);
                    }

                }               // (childNodeList)

            }                   // (HasChildren)        

        }                       // mozilla_selection_node_is_valid 

    }
    else
    {
        BR_ERRORPRINTF("no nsIDOMNSRange object");
    }
    return;
}

void mozilla_display_node_info(nsCOMPtr < nsIDOMNode > theNode)
{
    nsString theParentNodeName;
    nsString theChildNodeName;
    nsString theParentNodeValue;
    nsString theChildNodeValue;
    nsString theChildClassName;

    nsCOMPtr < nsIDOMNode > theParentNode;
    nsCOMPtr < nsIDOMNodeList > childNodeList;
    nsCOMPtr < nsIDOMNodeList > parentNodeList;
    unsigned int childNodeListLength = 0;
    unsigned parentNodeListLength = 0;
    int     parentNodeIndex;
    int     childNodeIndex;
    unsigned short childtype;
    unsigned short parenttype;
    PRBool  HasChildren;

    nsCOMPtr < nsIDOMHTMLElement > element;

    //theNode->Normalize();

    theNode->GetNodeName(theChildNodeName);
    theNode->GetNodeValue(theChildNodeValue);
    theNode->GetNodeType(&childtype);
    theNode->GetParentNode(getter_AddRefs(theParentNode));

    theNode->HasChildNodes(&HasChildren);

    if (HasChildren)
    {
        theNode->GetChildNodes(getter_AddRefs(childNodeList));

        if (childNodeList)
        {
            childNodeList->GetLength(&childNodeListLength);
        }
    }

    char   *childname = ToNewCString(theChildNodeName);
    char   *childvalue = ToNewCString(theChildNodeValue);

    mozilla_selection_get_node_index(theParentNode, theNode, &parentNodeIndex, &childNodeIndex);

    BR_LOGPRINTF("--------- mozilla_display_node_info ----------------");
    BR_LOGPRINTF("--> node[%d] = %s \n--> (value = %s) \n--> (children = %d) \n--> (type = %d)\n", childNodeIndex, childname, childvalue,
                 childNodeListLength, childtype);

    nsMemory::Free(childvalue);
    nsMemory::Free(childname);

    theParentNode->GetNodeName(theParentNodeName);
    theParentNode->GetNodeValue(theParentNodeValue);
    theParentNode->GetNodeType(&parenttype);

    char   *parentname = ToNewCString(theParentNodeName);
    char   *parentvalue = ToNewCString(theParentNodeValue);

    theParentNode->HasChildNodes(&HasChildren);

    if (HasChildren)
    {
        theParentNode->GetChildNodes(getter_AddRefs(parentNodeList));

        if (parentNodeList)
        {
            parentNodeList->GetLength(&parentNodeListLength);
        }
    }

    BR_LOGPRINTF("--> parentNode[%d] = %s \n--> (value = %s) \n--> (children = %d) \n--> (type = %d)\n", parentNodeIndex, parentname,
                 parentvalue, parentNodeListLength, parenttype);
    BR_LOGPRINTF("--------- mozilla_display_node_info ----------------");


    nsMemory::Free(parentvalue);
    nsMemory::Free(parentname);

}

/*
 * mozilla_selection_node_is_valid
 * 
 *		Until proven wrong the selected nodes should be TEXT NODES with a 
 *      ELEMENT NODE as a valid parent.
 *
 */
bool mozilla_selection_node_is_valid(nsCOMPtr < nsIDOMNode > theNode)
{
    unsigned short childNodeType = 0;
    unsigned short theParentNodeType = 0;

    nsCOMPtr < nsIDOMNode > theParentNode;
    nsString theNodeValue;
    nsAutoString HighLightTag;
    nsString theParentNodeName;

    theNode->GetNodeType(&childNodeType);

    if (childNodeType == nsIDOMNode::TEXT_NODE)
    {
        BR_LOGPRINTF("theNode = TEXT_NODE");

        // check if this text node has an actual value

        if (mozilla_text_node_is_real(theNode) == TRUE)
        {
            theNode->GetParentNode(getter_AddRefs(theParentNode));

            // check if the selection does not interfear with a previous selection
            theParentNode->GetNodeName(theParentNodeName);
            HighLightTag.AssignWithConversion("HIGHLIGHT");

            if (theParentNodeName.Equals(HighLightTag) == FALSE)
            {
                theParentNode->GetNodeType(&theParentNodeType);

                if (theParentNodeType == nsIDOMNode::ELEMENT_NODE)
                {
                    BR_LOGPRINTF("theParentNode = ELEMENT_NODE");
                    BR_LOGPRINTF("VALID SELECTION");
                    return TRUE;
                }
                else
                {
                    BR_LOGPRINTF("theParentNode = %d", childNodeType);
                }
            }
            else
            {
                BR_LOGPRINTF("interfears with other selection !!!");
            }
        }
        else
        {
            BR_LOGPRINTF("no real text NODE");
        }
    }
    else
    {
        BR_LOGPRINTF("theNode = %d", childNodeType);
    }

    BR_LOGPRINTF("INVALID SELECTION");
    return FALSE;
}

/*
 * mozilla_text_node_is_real
 * 
 *   Until proven wrong nodes only containing spaces and '\n' are invalid
 *   text nodes for the selection
 *
 */
static bool mozilla_text_node_is_real(nsCOMPtr < nsIDOMNode > theNode)
{
    nsCOMPtr < nsIDOMCharacterData > charData;
    nsString theCharDataValue;
    nsString theNodeValue;
    char   *charValue;
    unsigned int index;
    bool    returnValue = FALSE;
    unsigned int selectionLength;

    //theNode->Normalize();

    theNode->GetNodeValue(theNodeValue);

    if (!theNodeValue.IsEmpty())
    {
        charData = (do_QueryInterface(theNode));
        charData->GetLength(&selectionLength);

        if (selectionLength)
        {
            charData->GetData(theCharDataValue);
            charValue = ToNewUTF8String(theCharDataValue);

            for (index = 0; index < selectionLength; index++)
            {
                if ((charValue[index] != '\n') && (charValue[index] != ' '))
                {
                    returnValue = TRUE;
                    break;
                }
            }

            // free the allocated memory
            nsMemory::Free(charValue);
        }
        else
        {
            BR_LOGPRINTF("TEXT_NODE has length zero");
        }

    }
    else
    {
        BR_LOGPRINTF("TEXT_NODE theNode has no value");
    }

    return returnValue;
}

/*
 * mozilla_selection_get_node_index
 * 
 *		Retrieve the index of the child node and the parent node
 *
 *      REMARK : index values < 0 when no value was found
 *
 */
bool
mozilla_selection_get_node_index(nsCOMPtr < nsIDOMNode > theParentNode,
                                 nsCOMPtr < nsIDOMNode > theChildNode, int *parentNodeIndex, int *childNodeIndex)
{
    nsCOMPtr < nsIDOMDocument > domDoc;
    nsString theParentNodeName;

    nsCOMPtr < nsIDOMNodeList > documentNodeList;
    nsCOMPtr < nsIDOMNodeList > childNodeList;
    unsigned int documentNodeListLength = 0;
    unsigned int childNodeListLength = 0;

    nsCOMPtr < nsIDOMNode > node;
    nsCOMPtr < nsIDOMNode > childNode;

    BR_LOGPRINTF("ENTRY");

    if (theParentNode && theChildNode && parentNodeIndex && childNodeIndex)
    {
        // init index values
        *childNodeIndex = -1;
        *parentNodeIndex = -1;

        theParentNode->GetOwnerDocument(getter_AddRefs(domDoc));

        if (domDoc)
        {
            theParentNode->GetNodeName(theParentNodeName);
            domDoc->GetElementsByTagName(theParentNodeName, getter_AddRefs(documentNodeList));
            documentNodeList->GetLength(&documentNodeListLength);

            BR_LOGPRINTF("DOM document parent tag name nodeListLength %d", documentNodeListLength);

            for (unsigned int index = 0; index < documentNodeListLength; index++)
            {
                documentNodeList->Item(index, getter_AddRefs(node));

                if (node == theParentNode)
                {
                    *parentNodeIndex = index;
                    BR_LOGPRINTF("selection ParentNodeIndex %d", *parentNodeIndex);
                    //leave the for loop
                    break;
                }
            }

            theParentNode->GetChildNodes(getter_AddRefs(childNodeList));
            childNodeList->GetLength(&childNodeListLength);

            for (unsigned int index = 0; index < childNodeListLength; index++)
            {
                childNodeList->Item(index, getter_AddRefs(childNode));
                if (childNode == theChildNode)
                {
                    *childNodeIndex = index;
                    BR_LOGPRINTF("selection childNodeIndex %d", *childNodeIndex);
                    //leave the for loop
                    break;
                }
            }

            BR_LOGPRINTF("EXIT");
            return TRUE;
        }
        else
        {
            BR_ERRORPRINTF("domDoc == NULL");
        }                       //(domDoc)
    }
    else
    {
        BR_ERRORPRINTF("invalid input parameters");
    }                           // (parentNode) && (childNode)

    BR_LOGPRINTF("EXIT");
    return FALSE;
}

#endif //SELECTION_ENABLED
