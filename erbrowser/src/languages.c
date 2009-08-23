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

//
// Description: internationalistation
//
// Author: ann <ann@localhost>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <stdio.h>
#include <glib.h>
#include <locale.h>
#include <libintl.h>
#include "languages.h"
#include "browserLog.h"

void languagesInit()
{
    // look at setup application for howto integrate the 
    // language support with automake
/*	
	char* result;
	
	// " " => the local names will be set from the values of the environment variables with 
	// the same name as the categories or from "LANG"
	//setlocale(LC_ALL,"");
	
	// tryout
	setlocale(LC_ALL,"nl_NL");
	
	BR_LOGPRINTF("LCALL: %s",setlocale(LC_ALL,NULL));

	result = bindtextdomain("browser","../data/");
	
	BR_LOGPRINTF("bindtextdomain: %s",result);
	
	//bind_textdomain_codeset ("messages", "UTF-8");
	textdomain("browser");
*/
}
