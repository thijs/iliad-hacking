/*
 * This file is part of pcshareMgr.
 *
 * pcshareMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * pcshareMgr is distributed in the hope that it will be useful,
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
 * \file view.h
 * \brief view - GUI of the downloadManager application                  
 * 
 * <File description>
 *
 */

#ifndef __VIEW_MODULE__H__
#define __VIEW_MODULE__H__

#include <liberdm/display.h>

#define VIEW_TEXT_SPACING               10
#define VIEW_MAX_INF0_STRING_SIZE       128
#define VIEW_MAX_PROGRESS_STRING_SIZE   128
#define VIEW_MAX_DOTS                   5

// spacing from text area border to text
#define HBOX_TEXT_BORDER                6

// spacing between info and error fields
#define VBOX_SPACING                    4

// info-text and progress-text
#define TEXT_BG_WIDTH                   708
#define TEXT_BG_HEIGHT                  56

#define INFO_TEXT_WIDTH                 600

// error-image and error-text
#define ERROR_BG_WIDTH                  708
#define ERROR_BG_HEIGHT                 115

#define ERRORICON_BG_WIDTH              91
#define ERRORICON_BG_HEIGHT             115

#define ERRORICON_IMG_WIDTH             69
#define ERRORICON_IMG_HEIGHT            93

#define ERRORCONFIRM_HEIGHT             23
#define ERRORTEXT_HEIGHT                (ERROR_BG_HEIGHT - ERRORCONFIRM_HEIGHT - 2*HBOX_TEXT_BORDER)
#define ERRORTEXT_WIDTH                 (ERROR_BG_WIDTH  - ERRORICON_BG_WIDTH  - 3*HBOX_TEXT_BORDER)
#define ERRORCONFIRM_WIDTH              (ERRORTEXT_WIDTH - ERRORICON_BG_WIDTH  -   HBOX_TEXT_BORDER)


// text area: info/progress + error
#define TEXT_BORDER_LEFT                ((SCREEN_WIDTH - TEXT_BG_WIDTH)/2) 
#define TEXT_BORDER_RIGHT               (SCREEN_WIDTH - TEXT_BG_WIDTH - TEXT_BORDER_LEFT) 
#define TEXT_BORDER_TOP                 840
#define TEXT_BORDER_BOTTOM              0


#ifdef __cplusplus
extern  "C"
{
#endif /* __cplusplus */

// create and show downloadMgr screen
void viewInit(GtkWidget* topLevelWindow);
void viewSetText(void);

// show/hide the information
void viewShowInfo(char* info);
void viewHideInfo(void);

// show/hide the progress ifnforamtion
void viewShowProgress(void);
void viewHideProgress(void);

// show/hide the error message
void viewShowError(const char* msg);
void viewHideError(void);

typedef void on_error_confirmed_t(void);
void view_callback_on_error_confirmed(on_error_confirmed_t* callback);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__VIEW_MODULE__H__
