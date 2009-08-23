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
 * \file mozilla_api.h
 * \brief browser - API for retrieving info on the loaded XHTML page 
 *
 * <File description>
 *
 */

/* browser - A mozilla based browser application on the eReader platform
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 *
 */

#ifndef __MOZILLA_API__
#define __MOZILLA_API__

#define NEXT_LINK		0
#define PREVIOUS_LINK	1

typedef enum 
{
	pageUp_t,
	pageDown_t,
} direction_t;


/**
 * Method used to retrieve the type of a page (attribute of meta TAG) 
 *
 * @param embed reference to the MozEmbed widget
 * @param type  return the type information (allocated memory should be cleared by the calling routine
 *
 * @returns TRUE on succes
 */
gboolean mozilla_get_page_type(GtkMozEmbed *b, char** type);

/**
 * Method used to save a webpage -- only used for debug reasons 
 *
 * @param embed reference to the MozEmbed widget
 * @param filename where to save
 * @param all
 *
 * @returns TRUE on succes
 */
gboolean mozilla_save(GtkMozEmbed *b, gchar *file_name, gint all);

/**
 * Method used to select the next link of the currently loaded XHTML page 
 * when no "next" link is available the first link on th epage will be selected
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns TRUE on succes
 */
gboolean mozilla_next_link(GtkMozEmbed *b);

/**
 * Method used to select the currently focussed link
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns TRUE on succes
 */
gboolean mozilla_previous_link(GtkMozEmbed *b);

/**
 * Method used to select the previous link of the currently loaded XHTML page 
 * when no "previous" link is available the last link on the page will be selected
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns TRUE on succes
 */
gboolean mozilla_handle_link(GtkMozEmbed *b);

/**
 * Method used to select the INDEX link of the currently loaded XHTML page 
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns TRUE on succes
 */
gboolean mozilla_overview_page (GtkMozEmbed *b);

/**
 * Method used to go to the next page
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns TRUE on succes
 */
gboolean mozilla_next_page(GtkMozEmbed *b);

/**
 * Method used to go to the previous page
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns TRUE on succes
 */
gboolean mozilla_previous_page(GtkMozEmbed *b);

/**
 * Method used to select the ALT link of the currently loaded XHTML page 
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns TRUE on succes
 */
gboolean mozilla_alt_page(GtkMozEmbed * b);

/**
 * Method used to go one step back in the browser history
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns TRUE on succes
 */
gboolean mozilla_back(GtkMozEmbed * b);

/**
 * Method used to check if it possible to scroll on page
 * down or up  in the currently displayed XHTML page
 *
 * @param embed reference to the MozEmbed widget
 * @param direction
 *
 * @returns TRUE when possible
 */
gboolean mozilla_scrollable(direction_t direction, GtkMozEmbed * b);

/**
 * Method to scroll on page up or down
 * in the currently displayed XHTML page
 *
 * @param embed reference to the MozEmbed widget
 * @param count number of pages to scroll (positive values move down in the view)
 *
 * @returns TRUE when possible
 */
gboolean mozilla_scroll(int count, GtkMozEmbed * b);
/**
 * Method used to zoom the currently loaded page
 *
 * @param embed reference to the MozEmbed widget
 * @param size zoom factor
 *
 * @returns TRUE on succes
 */
gboolean mozilla_zoom(GtkMozEmbed *b, float size);

/**
 * Method used to retrieve the scrolled offset of the currently loaded "one page" XHTML page 
 *
 * @param embed reference to the MozEmbed widget
 *
 * @returns offset (DEFAULT_PAGE_OFFSET when errors occured, or non-scrollable page)
 */
gint mozilla_determine_page_offset(GtkMozEmbed * b);

/**
 * Method used to convert from uri to file system path.
 * 
 * @param uri reference to the input uri
 *
 * @return the path in file system, NULL if fail
 *
 */
char * mozilla_uri_to_path(const char * uri);

int raw_url_decode(char *str, int len);

#endif //__MOZILLA_API__
