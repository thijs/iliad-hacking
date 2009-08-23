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
 * \file mozilla_api.cpp
 * \brief browser - API for retrieving info on the loaded XHTML page 
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */
#include "browserTypes.h"
#include "indexFileHandler.h"
#include "browserLog.h"
#include "mozilla_api.h"
#include <sys/stat.h>
#include <ctype.h>

#include "nsCOMPtr.h"
#include "nsMemory.h"
#include "gtkmozembed_internal.h"
#include "nsIWebBrowser.h"
#include "nsIDOMDocument.h"
#include "nsIWebNavigation.h"
#include "nsIDOMNodeList.h"
#include "nsIDOMNode.h"
#include "nsIDOMHTMLMetaElement.h"
#include "nsIWebBrowserPersist.h"
#include "nsILocalFile.h"
#include "nsIScrollable.h"
#include "nsIViewManager.h"
#include "nsIPresShell.h"
#include "nsIBaseWindow.h"
#include "nsIDeviceContext.h"
#include "nsIDOMHTMLLinkElement.h"
#include "nsIDocShell.h"
#include "nsIDOMHTMLDocument.h"
#include "nsIDOMHTMLCollection.h"
#include "nsIWebBrowserFocus.h"
#include "nsIDOMHTMLAnchorElement.h"
#include "nsIDOMWindow.h"
#include "nsNetUtil.h"
#include "nsITextScroll.h"
#include "nsIScrollableView.h"

// internally used routines
static float GetWebPagePixelScale(nsCOMPtr < nsIWebBrowser > web_browser);
static gulong GetWebPageWindowSize(nsCOMPtr < nsIWebBrowser > web_browser);
static gboolean mozilla_link_update(GtkMozEmbed * b, gint type);
static nsIDOMWindow *GetDOMOWindowObject(nsIWebBrowser * wb);
static gboolean mozilla_link_module_goto_rel(GtkMozEmbed * b, char *value);

gboolean mozilla_get_page_type(GtkMozEmbed * b, gchar ** type)
{
    nsCOMPtr < nsIWebBrowser > wb;
    nsAutoString metaTypeName;
    nsAutoString metaTypeContent;
    nsAutoString metaTagName;
    nsresult result;
    PRUint32 numnodes = 0;
    char   *typeValue = NULL;

    metaTagName.AssignWithConversion("meta");
    metaTypeName.AssignWithConversion("type");

    g_return_val_if_fail(b != NULL, FALSE);
    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (!wb)
        return (FALSE);
    // retrieve the list of meta nodes
    nsCOMPtr < nsIWebNavigation > nav(do_QueryInterface(wb));
    nsCOMPtr < nsIDOMDocument > domDoc;
    nav->GetDocument(getter_AddRefs(domDoc));
    if (!domDoc)
        return (FALSE);

    nsCOMPtr < nsIDOMNodeList > nodeList;
    result = domDoc->GetElementsByTagName(metaTagName, getter_AddRefs(nodeList));

    BR_PAGEBARPRINTF("domDoc->GetElementsByTagName returned %d", result);

    nodeList->GetLength(&numnodes);

    if (numnodes != 0)
    {
        BR_PAGEBARPRINTF("page does contain %d meta tags", numnodes);

        // find the next page meta data tag
        for (unsigned int i = 0; i < numnodes; i++)
        {
            nsCOMPtr < nsIDOMNode > node;
            nodeList->Item(i, getter_AddRefs(node));

            nsCOMPtr < nsIDOMHTMLMetaElement > metaElement = do_QueryInterface(node);

            if (metaElement)
            {
                nsCOMPtr < nsIDOMElement > domElement = do_QueryInterface(node);
                if (!domElement)
                    return (FALSE);

                domElement->GetAttribute(metaTypeName, metaTypeContent);

                typeValue = ToNewCString(metaTypeContent);
                *type = g_strdup(typeValue);
                nsMemory::Free(typeValue);

                BR_PAGEBARPRINTF("node [%d] is meta data with type [%s]", i, (**type == NULL) ? "none" : *type);

                if (**type != NULL)
                {
                    // leave for loop  
                    break;
                }
            }                   // (meta Element)
        }                       // for loop

        if (**type == NULL)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }


    }
    else
    {
        BR_PAGEBARPRINTF("page does not contain any meta data");
        return FALSE;
    }                           // (numnodes != 0)
    return FALSE;
}


gboolean mozilla_save(GtkMozEmbed * b, gchar * file_name, gint all)
{
#ifdef SELECTION_ENABLED
    nsCOMPtr < nsIWebBrowser > wb;
    gint    i = 0;
    gchar  *relative_path = NULL;

    g_return_val_if_fail(b != NULL, FALSE);


    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (!wb)
        return (FALSE);

    nsCOMPtr < nsIWebNavigation > nav(do_QueryInterface(wb));

    nsCOMPtr < nsIDOMDocument > domDoc;
    nav->GetDocument(getter_AddRefs(domDoc));

    if (!domDoc)
        return (FALSE);
    nsCOMPtr < nsIWebBrowserPersist > persist(do_QueryInterface(wb));

    if (persist)
    {
        nsCOMPtr < nsILocalFile > file;
        nsCOMPtr < nsILocalFile > relative = nsnull;
        if (all)
        {

            relative_path = g_strdup(file_name);
            relative_path = g_strconcat(relative_path, "_files/", NULL);

            for (i = strlen(relative_path) - 1; i >= 0; --i)
                if (relative_path[i] == '/')
                {
                    relative_path[i] = '\0';
                    break;
                }

            mkdir(relative_path, 0755);

            nsAutoString s;

            s.AssignWithConversion(relative_path);
            NS_NewLocalFile(s, PR_TRUE, getter_AddRefs(relative));
        }

        nsAutoString s;

        s.AssignWithConversion(file_name);
        NS_NewLocalFile(s, PR_TRUE, getter_AddRefs(file));

        if (file)
            persist->SaveDocument(domDoc, file, relative, nsnull, 0, 0);

        if (all)
            g_free(relative_path);

        return (TRUE);
    }
#endif //SELECTION_ENABLED
    return (FALSE);
}



gboolean mozilla_next_link(GtkMozEmbed * embed)
{
    return mozilla_link_update(embed, NEXT_LINK);
}

gboolean mozilla_previous_link(GtkMozEmbed * embed)
{
    return mozilla_link_update(embed, PREVIOUS_LINK);
}

/*
 * mozilla_link_update: Method used to hightlight the previous/next link
 *
 */
static gboolean mozilla_link_update(GtkMozEmbed * b, gint type)
{
    static PRInt32 currentFocus = (-1);

    nsCOMPtr < nsIWebBrowser > wb;
    PRUint32 numnodes = 0;

    g_return_val_if_fail(b != NULL, FALSE);
    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (!wb)
        return (FALSE);

    // retrieve the list of link nodes
    nsCOMPtr < nsIWebNavigation > nav(do_QueryInterface(wb));
    nsCOMPtr < nsIDOMDocument > domDoc;
    nav->GetDocument(getter_AddRefs(domDoc));
    if (!domDoc)
        return (FALSE);

    nsCOMPtr < nsIDOMHTMLDocument > htmlDoc = do_QueryInterface(domDoc);
    if (!htmlDoc)
        return (FALSE);

    nsCOMPtr < nsIDOMHTMLCollection > links;
    htmlDoc->GetLinks(getter_AddRefs(links)); // GetAnchors returns nothing
    nsCOMPtr < nsIDOMNode > link;
    links->GetLength(&numnodes);

    if (numnodes != 0)
    {
        // see if there is a focused element
        nsCOMPtr < nsIWebBrowserFocus > focus = do_QueryInterface(wb);
        if (!focus)
            return (FALSE);
        nsCOMPtr < nsIDOMElement > focusElement;
        focus->GetFocusedElement(getter_AddRefs(focusElement));

        if (focusElement)
        {
            BR_MOZILLA_DOCPRINTF("there is a focused element %d mode to %d\n", currentFocus, (currentFocus + numnodes - 1) % numnodes);

            if (type == PREVIOUS_LINK)
            {
                currentFocus = (currentFocus + numnodes - 1) % numnodes;
            }
            else
            {
                currentFocus = (currentFocus + 1) % numnodes;
            }

            links->Item(currentFocus, getter_AddRefs(link));
            nsCOMPtr < nsIDOMHTMLAnchorElement > anchor = do_QueryInterface(link);
            if (!anchor)
                return (FALSE);
            if (anchor)
            {
                BR_MOZILLA_DOCPRINTF("node is anchor");
                anchor->Focus();
            }                   // (anchor)
        }
        else
        {
            // set the focus to the first link element
            links->Item(0, getter_AddRefs(link));

            nsCOMPtr < nsIDOMHTMLAnchorElement > anchor = do_QueryInterface(link);
            if (!anchor)
                return (FALSE);
            if (anchor)
            {
                BR_MOZILLA_DOCPRINTF("node is anchor");
                anchor->Focus();
                currentFocus = 0;
            }                   // (anchor)

        }                       // (focusElement)
    }
    else
    {
        BR_MOZILLA_DOCPRINTF("page does not contain any links");
        return FALSE;
    }
    return TRUE;
}

gboolean mozilla_next_page(GtkMozEmbed * b)
{
    return mozilla_link_module_goto_rel(b, "NEXT");
}

gboolean mozilla_previous_page(GtkMozEmbed * b)
{
    return mozilla_link_module_goto_rel(b, "PREV");
}

gboolean mozilla_overview_page(GtkMozEmbed * b)
{
    return mozilla_link_module_goto_rel(b, "INDEX");
}

gboolean mozilla_alt_page(GtkMozEmbed * b)
{
    return mozilla_link_module_goto_rel(b, "ALT");
}

/*
 * mozilla_link_module_goto_rel: 
 *		Method used to find the URL in the XHTML "Link module" section
 *		associated with the "value" rel attribute (see e-reader format spec) 
 *
 */
static gboolean mozilla_link_module_goto_rel(GtkMozEmbed * b, char *value)
{
    nsCOMPtr < nsIWebBrowser > wb;
    PRUint32 numnodes = 0;
    nsresult result;
    nsAutoString aHref;
    nsAutoString linkTagName;
    nsAutoString linkRelNameContent;
    char   *urlValue = NULL;

    linkTagName.AssignWithConversion("link");
    //NS_NAMED_LITERAL_STRING(linkTagName,"link");

    g_return_val_if_fail(b != NULL, FALSE);

    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (!wb)
        return (FALSE);

    BR_MOZILLA_DOCPRINTF("entry %s", value);

    // retrieve the list of link nodes
    nsCOMPtr < nsIWebNavigation > nav(do_QueryInterface(wb));
    nsCOMPtr < nsIDOMDocument > domDoc;
    nav->GetDocument(getter_AddRefs(domDoc));
    if (!domDoc)
        return (FALSE);

    nsCOMPtr < nsIDOMNodeList > nodeList;
    result = domDoc->GetElementsByTagName(linkTagName, getter_AddRefs(nodeList));
    //result = domDoc->GetElementsByTagName(NS_LITERAL_STRING("link"), getter_AddRefs(nodeList));

    BR_MOZILLA_DOCPRINTF("domDoc->GetElementsByTagName returned %d", result);

    nodeList->GetLength(&numnodes);

    if (numnodes != 0)
    {
        BR_MOZILLA_DOCPRINTF("page does contain %d links\n", numnodes);

        // find the next page link
        for (unsigned int i = 0; i < numnodes; i++)
        {
            nsCOMPtr < nsIDOMNode > node;
            nodeList->Item(i, getter_AddRefs(node));

            nsCOMPtr < nsIDOMHTMLLinkElement > linkElement = do_QueryInterface(node);
            if (!linkElement)
                return (FALSE);

            linkElement->GetRel(linkRelNameContent);

            if (linkRelNameContent.EqualsIgnoreCase(value))
            {
                linkElement->GetHref(aHref);
                // return a new |char| buffer you must free with |nsMemory::Free|
                urlValue = ToNewCString(aHref);

                BR_MOZILLA_DOCPRINTF("node is linkelement with url %s", urlValue);
                gtk_moz_embed_load_url(GTK_MOZ_EMBED(b), urlValue);

                nsMemory::Free(urlValue);
                return TRUE;
            }
            else
            {
                BR_MOZILLA_DOCPRINTF("node is linkelement with rel != %s", value);
            }
        }
    }
    else
    {
        BR_MOZILLA_DOCPRINTF("page does not contain any links");
    }
    return FALSE;
}

gboolean mozilla_handle_link(GtkMozEmbed * b)
{
    nsCOMPtr < nsIWebBrowser > wb;
    char   *urlValue;
    nsAutoString aHref;

    g_return_val_if_fail(b != NULL, FALSE);
    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (!wb)
        return (FALSE);

    // see if there is a focused element
    nsCOMPtr < nsIWebBrowserFocus > focus = do_QueryInterface(wb);
    nsCOMPtr < nsIDOMElement > focusElement;
    focus->GetFocusedElement(getter_AddRefs(focusElement));

    if (focusElement)
    {
        BR_MOZILLA_DOCPRINTF("There is a focused element");

        nsCOMPtr < nsIDOMHTMLAnchorElement > anchor = do_QueryInterface(focusElement);
        if (anchor)
        {
            anchor->GetHref(aHref);

            // return a new |char| buffer you must free with |nsMemory::Free|
            urlValue = ToNewCString(aHref);

            BR_MOZILLA_DOCPRINTF("node is anchor with url %s", urlValue);
            gtk_moz_embed_load_url(GTK_MOZ_EMBED(b), urlValue);

            nsMemory::Free(urlValue);
            return TRUE;
        }
        else
        {
            BR_MOZILLA_DOCPRINTF("node is NO anchor !!!!!");
        }                       // (anchor)
    }
    else
    {
        BR_MOZILLA_DOCPRINTF("page does not contain a focussed element");
    }
    return FALSE;
}


gboolean mozilla_back(GtkMozEmbed * b)
{
    g_return_val_if_fail(b != NULL, FALSE);

    if (gtk_moz_embed_can_go_back(GTK_MOZ_EMBED(b)))
    {
        gtk_moz_embed_go_back(GTK_MOZ_EMBED(b));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

gboolean mozilla_zoom(GtkMozEmbed * b, float size)
{
    nsCOMPtr < nsIWebBrowser > wb;
    nsCOMPtr < nsIDOMWindow > activeWindow;

    g_return_val_if_fail(b != NULL, FALSE);
    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (!wb)
        return (FALSE);

    BR_LOGPRINTF("mozilla_zoom -- GetDOMOWindowObject");

    activeWindow = GetDOMOWindowObject(wb);

    if ((activeWindow) && (size > 0))
    {
        activeWindow->SetTextZoom(size);
        return TRUE;
    }
    return FALSE;
}

/*
 * GetDOMOWindowObject: 
 *		retrieve refercne to the DOMWindow
 *
 */
static nsIDOMWindow *GetDOMOWindowObject(nsIWebBrowser * wb)
{
    nsCOMPtr < nsIDOMWindow > oDomWindow;
    nsresult rv;

    rv = wb->GetContentDOMWindow(getter_AddRefs(oDomWindow));

    BR_LOGPRINTF("GetContentDOMWindow rv %d", rv);

    if (!oDomWindow)
    {
        BR_WARNPRINTF("Cannot create Dom Window Object");
    }
    return oDomWindow;
}

/*
gboolean mozilla_scrollable(direction_t direction, GtkMozEmbed * b)
{
    nsCOMPtr < nsIWebBrowser > wb;
    nsCOMPtr < nsIScrollable > scrollableInterface;
	int32 curPos, minPos, maxPos;
	
    gboolean  scrollable = FALSE;

    g_return_val_if_fail(b != NULL, scrollable);
	
    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (wb) 
	{
	    scrollableInterface = do_QueryInterface(wb);

    	if (scrollableInterface)
    	{
			scrollableInterface->GetScrollRange(scrollableInterface->ScrollOrientation_Y, &minPos, &maxPos);
            scrollableInterface->GetCurScrollPos(scrollableInterface->ScrollOrientation_Y, &curPos);
			
			BR_PAGEBARPRINTF("minPos %d maxPos %d curPos %d",minPos,maxPos,curPos);
			
			switch (direction)
			{
				case pageDown_t:
					if (curPos < maxPos) scrollable = TRUE;
					break;
				case pageUp_t :
					if (curPos > minPos) scrollable = TRUE;
					break;
			}
		}
    }
	
	BR_PAGEBARPRINTF("return %s",(scrollable == TRUE) ? "TRUE":"FALSE");
    return scrollable;	
}
*/

gboolean mozilla_scrollable(direction_t direction, GtkMozEmbed * b)
{
    PRBool  scrollable = PR_FALSE;

    nsCOMPtr < nsIWebBrowser > wb;
    nsCOMPtr < nsIDocShell > doc_shell;
    nsCOMPtr < nsIPresShell > presShell;
    //nsCOMPtr < nsIViewManager > vm;
    //nsCOMPtr < nsIScrollableView > scrollview;

    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));

    if (wb)
    {
        doc_shell = do_GetInterface(wb);

        if (doc_shell)
        {
            doc_shell->GetPresShell(getter_AddRefs(presShell));

            if (presShell)
            {
                nsIViewManager *vm = presShell->GetViewManager();

                if (vm)
                {
                    nsIScrollableView *scrollableView = nsnull;

                    vm->GetRootScrollableView(&scrollableView);

                    if (scrollableView)
                    {
                        switch (direction)
                        {
                        case pageDown_t:
                            scrollableView->CanScroll(PR_FALSE, PR_TRUE, scrollable);
                            break;
                        case pageUp_t:
                            scrollableView->CanScroll(PR_FALSE, PR_FALSE, scrollable);
                            break;
                        }

                    }
                }

            }
        }
    }

    BR_PAGEBARPRINTF("return %s", (scrollable == PR_TRUE) ? "TRUE" : "FALSE");

    return scrollable;
}

gboolean mozilla_scroll(int count, GtkMozEmbed * b)
{
    gboolean returnValue = FALSE;

    nsCOMPtr < nsIWebBrowser > wb;
    nsCOMPtr < nsITextScroll > textscrollInterface;

    g_return_val_if_fail(b != NULL, FALSE);

    BR_PAGEBARPRINTF("count %d", count);

    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (wb)
    {
        textscrollInterface = do_QueryInterface(wb);

        if (textscrollInterface)
        {
            // prevents scrolling off the end of the view
            textscrollInterface->ScrollByPages(count);
            returnValue = TRUE;
        }
    }
    return returnValue;
}

gint mozilla_determine_page_offset(GtkMozEmbed * b)
{
    nsCOMPtr < nsIWebBrowser > wb;
    nsCOMPtr < nsIScrollable > scrollable;
    gint    offset = DEFAULT_PAGE_OFFSET;

    BR_PAGEBARPRINTF("mozilla_determine_page_offset -- entry\n");

    g_return_val_if_fail(b != NULL, DEFAULT_PAGE_OFFSET);

    BR_PAGEBARPRINTF("mozilla_determine_page_offset -- b != NULL\n");

    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (!wb)
    {
        BR_WARNPRINTF("mozilla_determine_page_offset -- wb == null\n");
        return (DEFAULT_PAGE_OFFSET);
    }

    // determine web page size
    scrollable = do_QueryInterface(wb);

    if (scrollable)
    {
        gint    minPos;
        gint    maxPos;
        gint    curPos;
        int     pixelscale, htmlSize, htmlPos;
        gulong  theWindowSize;

        scrollable->GetScrollRange(scrollable->ScrollOrientation_Y, &minPos, &maxPos);
        BR_PAGEBARPRINTF("scrollable->getScrollRange returned [%d,%d]\n", minPos, maxPos);
        scrollable->GetCurScrollPos(scrollable->ScrollOrientation_Y, &curPos);
        BR_PAGEBARPRINTF("scrollable->getCurScrollPos returned %d\n", curPos);

        // determine window size (in pixels) 
        theWindowSize = GetWebPageWindowSize(wb);
        BR_PAGEBARPRINTF("theWindowSize=%d\n", theWindowSize);

        // determine pixel scale to convert from twips to pixels
        // scaled to avoid using floats
        pixelscale = (int) (100 / GetWebPagePixelScale(wb));
        BR_PAGEBARPRINTF("100/pixelscale=%d\n", pixelscale);

        // Sizes in pixels, already scaled for percentage to avoid
        // using floats
        htmlSize = (maxPos - minPos);
        htmlPos = curPos * 100 + theWindowSize * pixelscale;

        // Calculate offset in percentage -- rounded to integer value
        offset = (gint) (htmlPos / htmlSize) + 1;
        if (offset > 100)
        {
            offset = 100;
        }
        BR_PAGEBARPRINTF("CALCULATED OFFSET = %d\n", offset);

    }

    return offset;
}


/*
 * GetWebPagePixelScale: 
 *		Method used to retrieve the pixelscale page count in a "one_page" type HTML page 
 *
*/
static float GetWebPagePixelScale(nsCOMPtr < nsIWebBrowser > web_browser)
{
    nsCOMPtr < nsIDocShell > doc_shell;
    nsCOMPtr < nsIPresShell > presShell;
    nsCOMPtr < nsIViewManager > vm;

    nsIDeviceContext *dx = NULL;
    float   pixelscale = 0;

    doc_shell = do_GetInterface(web_browser);

    if (doc_shell)
    {
        doc_shell->GetPresShell(getter_AddRefs(presShell));

        if (presShell)
        {
            // What about the "nsCOMPtr" story here ??

            //try out....   
            //nsCOMPtr <nsIDocument> dm;
            //dm = presShell->GetDocument();

            // try out....
            //presShell->SetPreferenceStyleRules(TRUE);

            vm = presShell->GetViewManager();
            //presShell->GetViewManager(getter_AddRefs(vm));

            vm->GetDeviceContext(dx);
            pixelscale = dx->TwipsToDevUnits();

//            PRINT_TRACE(("dx->TwipsToDevUnits returned %f\n", pixelscale));
        }
    }
    return pixelscale;

}

/*
 * GetWebPageWindowSize: 
 *		Method used to retrieve the (gecko) clientwindow size in pixels 
 *
*/
static gulong GetWebPageWindowSize(nsCOMPtr < nsIWebBrowser > web_browser)
{
    nsCOMPtr < nsIBaseWindow > baseWindow;
    gulong  theWindowSize = 0;

    // determine window size (in pixels) 
    baseWindow = do_QueryInterface(web_browser);

    if (baseWindow)
    {
        int     x, y, cx, cy;

        baseWindow->GetPositionAndSize(&x, &y, &cx, &cy);
//        PRINT_TRACE(("baseWindow->GetPositionAndSize x %d - y %d - cx %d - cy %d\n", x, y, cx, cy));

        theWindowSize = cy - y;
//        PRINT_TRACE(("windowSize = %d - %d = %d\n", cy, y, theWindowSize));
    }
    return theWindowSize;
}

// ****************************************************************
//
//  THESE FUNCTIONS ARE NOT USED ANYMORE, KEPT HERE FOR REFERENCE
//
// ****************************************************************

/**
 * Method used to retrieve the page count of the currently loaded "one page" XHTML page 
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns pagecount (DEFAULT_PAGE when errors occured)
 */
/* 
gint mozilla_determine_page_count(GtkMozEmbed * b)
{
    nsCOMPtr < nsIWebBrowser > wb;
    nsCOMPtr < nsIScrollable > scrollable;
    gulong  htmlSize = 0;
    gulong  theWindowSize = 0;
    gulong  pagecount = 0;

    PRINT_TRACE(("mozilla_determine_page_count -- entry\n"));

    g_return_val_if_fail(b != NULL, DEFAULT_PAGE);

    PRINT_TRACE(("mozilla_determine_page_count -- b != NULL\n"));

    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (!wb)
    {
        PRINT_ERROR(("mozilla_determine_page_count -- wb == null\n"));
        return (DEFAULT_PAGE);
    }

    // determine web page size (in pixels)
    scrollable = do_QueryInterface(wb);

    if (scrollable)
    {
        gint    minPos;
        gint    maxPos;
        float   pixelscale;

        scrollable->GetScrollRange(scrollable->ScrollOrientation_Y, &minPos, &maxPos);
        PRINT_TRACE(("scrollable->getScrollRange returned [%d,%d]\n", minPos, maxPos));

        pixelscale = GetWebPagePixelScale(wb);

        // converion from Twip to device units (= pixels)
        htmlSize = (maxPos - minPos) * pixelscale;

        // What about decimal digits ??
        PRINT_TRACE(("height (%d) * pixelscale (%f)  = %d => HTML page size\n", (maxPos - minPos), pixelscale, htmlSize));
    }
    else
    {
        PRINT_TRACE(("mozilla_determine_page_count -- scrollable == NULL\n"));
    }

    // determine window size (in pixels) 
    theWindowSize = GetWebPageWindowSize(wb);

    // calculate page count 
    if (theWindowSize)
    {
        pagecount = htmlSize / theWindowSize;
        PRINT_TRACE(("pagecount (%d) = %d/%d\n", pagecount, htmlSize, theWindowSize));

    }
    return pagecount;
}
*/

/*
gint mozilla_get_current_page(GtkMozEmbed * b)
{
    nsCOMPtr < nsIWebBrowser > wb;
    nsCOMPtr < nsIScrollable > scrollable;
    gulong  htmlPosition = 0;
    gulong  theWindowSize = 0;
    gulong  page = DEFAULT_PAGE;

    g_return_val_if_fail(b != NULL, page);
    gtk_moz_embed_get_nsIWebBrowser(b, getter_AddRefs(wb));
    if (!wb)
        return (page);

    scrollable = do_QueryInterface(wb);

    if (scrollable)
    {
        gint    curPos;
        float   pixelscale;

        scrollable->GetCurScrollPos(scrollable->ScrollOrientation_Y, &curPos);
        pixelscale = GetWebPagePixelScale(wb);

        // converion from Twip to device units (=pixels)
        htmlPosition = curPos * pixelscale;
        PRINT_TRACE(("scrollable->GetCurScrollPos returned %d => htmlPosition %d\n", curPos, htmlPosition));
    }

    // determine window size (in pixels) 
    theWindowSize = GetWebPageWindowSize(wb);

    // calculate current page count 
    if (theWindowSize)
    {
        page = htmlPosition / theWindowSize;
        PRINT_TRACE(("page (%d) = %d/%d\n", page, htmlPosition, theWindowSize));
    }

    return page;
}
*/

/* Code taken from the PHP project, non-restrictive license */
/* Copyright (c) 1999 - 2006 The PHP Group. */
static int htoi(char *s)
{
	int value;
	int c;

	c = ((unsigned char *)s)[0];
	if (isupper(c))
		c = tolower(c);
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = ((unsigned char *)s)[1];
	if (isupper(c))
		c = tolower(c);
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	return (value);
}

int raw_url_decode(char *str, int len)
{
	char *dest = str;
	char *data = str;

	while (len--) {
		if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) 
			&& isxdigit((int) *(data + 2))) {
			*dest = (char) htoi(data + 1);
			data += 2;
			len -= 2;
		} else {
			*dest = *data;
		}
		data++;
		dest++;
	}
	*dest = '\0';
	return dest - str;
}
/* End code taken from PHP */


char * mozilla_uri_to_path(const char * aUri)
{
    // in fact, we should use nsIURI to get path from uri
    // just remove file:// prefix if there is any
    char * protocol = "file://";
    if (NULL == aUri)  return NULL;
    unsigned int min = strlen(protocol);

    if (strlen(aUri) <= min) return NULL;
    if (aUri != strstr(aUri, protocol))
    {
        return NULL;
    }
    
    char * path = strdup((char *) &aUri[min]);
    
    // we should handle the %20 %2f...
    raw_url_decode(path, strlen(path));
    return path;
    
    // the following code does not work, I don't know the reason, it once works
    /*
    nsresult rv;
    nsCOMPtr<nsIURI> theURI;
    rv = NS_NewURI(getter_AddRefs(theURI), aUri);
    if (NS_FAILED(rv)) return NULL;
    nsCAutoString  path;
    theURI->GetPath(path);
    return ToNewCString(path);
    */    
}    	

