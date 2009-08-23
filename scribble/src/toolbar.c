/*
 * This file is part of scribble.
 *
 * scribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * scribble is distributed in the hope that it will be useful,
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
 * \file toolbar.c
 * \brief scribble - toolbar interaction with scribble
 * \author Jian Shen
 *
 * <File description>
 *
 */

#include <libmb/mb.h>

#include <liberipc/eripc.h>
#include <liberipc/eripctoolbar.h>
#include <liberipc/eripcviewer.h>
#include <liberipc/eripcicons.h>

#include "ScribbleUI.h"
#include "ScribbleIPC.h"
#include "ScribbleLog.h"
#include "ScribbleUtils.h"
#include "displayUpdate.h"

//internal use for iconlist mode
typedef enum {
    ICONS_FLDALL=0,      //folder all icon
    ICONLIST_UNFLDSIZE=1,//unfold the size select
    ICONLIST_UNFLDCOLOR=2,//fold the color select
}ICONLIST_MODE;

static erClientChannel_t toolbarChannel=NULL;

//current operation mode
static  OPERATION_MODE s_OperationMode=MODE_DRAW;
OPERATION_MODE getOperationMode()
{
    return s_OperationMode;
}
void setOperationMode(OPERATION_MODE iOpMode)
{
    s_OperationMode=iOpMode;
}

//current pen color
static HW_COLOR s_PenColor=COLOR_BLACK;
//get color for direct draw on hardware
HW_COLOR getPenColor()
{
    return s_PenColor;
}

void setPenColor(HW_COLOR iHwColor)
{
    s_PenColor=iHwColor;
}

//current pen-size
static PEN_SIZE m_PenSize=SIZE_3PIX;
PEN_SIZE getPenSize()
{
    return m_PenSize;
}
void setPenSize(PEN_SIZE iPenSize)
{
    m_PenSize=iPenSize;
}

//some icon initially.
static const int s_InitIcons[]=
{
    iconID_eraseline, //for eraser
    iconID_npage,//for add new page,
    iconID_trashcan,//for delete page,only add it in first time
};


#define LIST_INDEX_ICON    (0)
#define LIST_INDEX_ICONLOW (1)
#define LIST_INDEX_SETTING (2)

static const int s_PenList[][4]=
{
    {
        iconID_pen1pixel,
        iconID_pen3pixel,
        iconID_pen5pixel,
        iconID_pen7pixel
    },
    {
        iconID_pen1pixelLow,
        iconID_pen3pixelLow,
        iconID_pen5pixelLow,
        iconID_pen7pixelLow
    },
    {
        SIZE_1PIX,
        SIZE_3PIX,
        SIZE_5PIX,
        SIZE_7PIX
    }
};

static const int s_ColorList[][4]=
{
    {
        iconID_penwhite,
        iconID_penlightgrey,//LIGHT_GRAY
        iconID_pendarkgrey,//DARK_GRAY
        iconID_penblack //BLACK
    },
    {
        iconID_penwhiteLow,
        iconID_penlightgreyLow,//LIGHT_GRAY
        iconID_pendarkgreyLow,//DARK_GRAY
        iconID_penblackLow //BLACK
    },
    {
        COLOR_WHITE, 
        COLOR_LIGHT_GRAY, 
        COLOR_DARK_GRAY,//in select clor or pen size
        COLOR_BLACK,
    }
};

static int getIconfromPenSize(PEN_SIZE iPenSize, int iListIndex)
{
    int i;
    for(i=0; i< ARRAYSIZE(s_PenList[LIST_INDEX_SETTING]); i++)
    {
        if( s_PenList[LIST_INDEX_SETTING][i] == iPenSize )
        {
            return s_PenList[iListIndex][i];
        }
    }
    return s_PenList[iListIndex][1]; //default: pen3pixel
}

static PEN_SIZE getPenSizefromIcon(int iIconID)
{
    int i;
    for(i=0; i< ARRAYSIZE(s_PenList[LIST_INDEX_SETTING]); i++)
    {
        if(   s_PenList[LIST_INDEX_ICON   ][i]==iIconID
           || s_PenList[LIST_INDEX_ICONLOW][i]==iIconID )
        {
            return s_PenList[LIST_INDEX_SETTING][i];
        }
    }
    return SIZE_3PIX;//default
}

static int getIconfromPenColor(HW_COLOR iPenColor, int iListIndex)
{
    int i;
    for( i=0; i< ARRAYSIZE(s_ColorList[LIST_INDEX_SETTING]); i++)
    {
        if( s_ColorList[LIST_INDEX_SETTING][i] == iPenColor )
        {
            return s_ColorList[iListIndex][i];
        }
    }
    return s_ColorList[iListIndex][3];  //default: black
}

static HW_COLOR getPenColorfromIcon(int iIconID)
{
    int i;
    for( i=0; i< ARRAYSIZE(s_ColorList[LIST_INDEX_SETTING]); i++)
    {
        if(   s_ColorList[LIST_INDEX_ICON   ][i]==iIconID
           || s_ColorList[LIST_INDEX_ICONLOW][i]==iIconID )
        {
            return s_ColorList[LIST_INDEX_SETTING][i];
        }
    }
    return COLOR_BLACK;//default
}


//add icons acoording to icons array
static void addIcons(const int *pIcons,unsigned int iSize)
{
    unsigned int i;
    for( i=0;i<iSize;i++)
    {
        tbAppendPlatformIcon(toolbarChannel,ER_SCRIBBLE_UA_ID,
                             pIcons[i],ccVwrToolbar);
    }
}

//add one icon
static void addIcon(const int iconID)
{
    addIcons(&iconID,1);
}

//wait confirm deleting?
gboolean toolbar_is_waitconfirmdel()
{
    return ( MODE_DELPAGE==s_OperationMode );
}

static gboolean ispagedel_confirmed=FALSE;
//set page-delelted confirmed.
void toolbar_confirm_delpage()
{
    ispagedel_confirmed=TRUE;
}

//in mode_erase?
gboolean toolbar_is_erasing()
{
    return (MODE_ERASE== s_OperationMode);
}

//can draw point
gboolean toolbar_is_drawing()
{
    return(    MODE_DRAW     ==s_OperationMode 
            || MODE_SELSIZE  ==s_OperationMode
            || MODE_SELCOLOR ==s_OperationMode);
}

void activeIcon( int iconID )
{
    tbSetStatePlatformIcon(toolbarChannel, ER_SCRIBBLE_UA_ID, 
                           iconID,iconState_selected);
}

void disableIcon( int iconID)
{
    tbSetStatePlatformIcon(toolbarChannel, ER_SCRIBBLE_UA_ID, 
                           iconID,iconState_grey);
}

void normalIcon( int iconID)
{
    tbSetStatePlatformIcon(toolbarChannel, ER_SCRIBBLE_UA_ID, 
                           iconID,iconState_normal);
}

//add icons according to the icon list mode
void addIconsByListMode(const ICONLIST_MODE iListMode)
{
    addIcon( iconID_pen );//normal
    
    //to keep the icons order.
    if( ICONLIST_UNFLDSIZE==iListMode )
    {
        addIcons(s_PenList[LIST_INDEX_ICONLOW], ARRAYSIZE(s_PenList[LIST_INDEX_ICONLOW]));
    }
    else
    {
        addIcon( getIconfromPenSize(getPenSize(), LIST_INDEX_ICON) );  
    }
        
    if( ICONLIST_UNFLDCOLOR==iListMode )
    {
        addIcons(s_ColorList[LIST_INDEX_ICONLOW], ARRAYSIZE(s_ColorList[LIST_INDEX_ICONLOW]));
    }
    else
    {
        addIcon( getIconfromPenColor(getPenColor(), LIST_INDEX_ICON) );
    }
    
    addIcons(s_InitIcons,ARRAYSIZE(s_InitIcons));
    //always disable keyboard and tagging currently
    disableIcon(iconID_keyboard);
    disableIcon(iconID_tagging);
}

//draw icons and set status by current mode.   
void drawIconsByMode(int iTotalPage)
{
    // freeze toolbar 
    tbDisableUpdate(toolbarChannel, ER_SCRIBBLE_UA_ID);
    
    // clear the toolbar
    tbClearIconSet(toolbarChannel, ER_SCRIBBLE_UA_ID);
    switch ( getOperationMode() )
    {
        case MODE_INVALID:
            addIconsByListMode(ICONS_FLDALL);
            break;
        case MODE_DRAW:
            addIconsByListMode(ICONS_FLDALL);
            activeIcon( iconID_pen );           
            break;
        case MODE_SELSIZE:
            addIconsByListMode(ICONLIST_UNFLDSIZE);
            activeIcon( getIconfromPenSize(getPenSize(), LIST_INDEX_ICONLOW) ); 
            activeIcon( iconID_pen );          
            break;
        case MODE_SELCOLOR:
            addIconsByListMode(ICONLIST_UNFLDCOLOR);
            activeIcon( getIconfromPenColor(getPenColor(), LIST_INDEX_ICONLOW) );
            activeIcon( iconID_pen );           
            break;
        case MODE_ERASE:
            addIconsByListMode(ICONS_FLDALL);
            activeIcon( iconID_eraseline );           
            break;
        case MODE_DELPAGE:
            addIconsByListMode(ICONS_FLDALL);
            activeIcon( iconID_trashcan );           
            break;
        default:
            SB_ERRORPRINTF("Error Mode[%d]\n", getOperationMode());
            return;
    }
    if( iTotalPage<=1 )
    {
        disableIcon(iconID_trashcan);
    }
    
    // redraw toolbar now
    tbEnableUpdate(toolbarChannel, ER_SCRIBBLE_UA_ID);
}

//clear the toolbar to make
void toolbar_clearall()
{
    tbClearIconSet(toolbarChannel, ER_SCRIBBLE_UA_ID);
}

//first init the toolbar,set it to MODE_DRAW
void toolbar_firstinit(int iTotalPage)
{
    setOperationMode(MODE_DRAW);
    drawIconsByMode(iTotalPage);
}

//reinit the toolbar,reset all the status,include the pages
void toolbar_reinit(int iTotalPage)
{
    //change the current mode only if current is mode_draw or erase.
    if( MODE_DELPAGE==getOperationMode() )
    {
        setOperationMode(MODE_DRAW);
    }
    drawIconsByMode(iTotalPage);
}

void toolbar_init()
{
    int     returnValue;

    SB_IPCPRINTF("\n\n\nentry,ER_TOOLBAR_CHANNEL=%d\n\n",
                 ER_TOOLBAR_CHANNEL);
    toolbarChannel=NULL;
    // create/init channel to communicate with the toolbar service
    returnValue = erIpcStartClient(ER_TOOLBAR_CHANNEL, &toolbarChannel);
    if (returnValue != 0)
    {
        SB_ERRORPRINTF("erIpcStartClient returned %d\n", returnValue);
        return;
    }
    // tell toolbar which icons set to display
    tbSelectIconSet(toolbarChannel, ER_SCRIBBLE_UA_ID);
}

void toolbar_key_selected(int iconID, int iconState,PScribbleUICtx pScribbleUICtx)
{
    if( iconState_grey==iconState )
    {
        return;//donothing.
    }
    
    SB_IPCPRINTF("\n====iconID=%d[mode=%d,pensize=%d,color=%d]\n",
                 iconID,getOperationMode(),getPenSize(),getPenColor());
  
    gboolean bNeedFlushWithFullQlt=FALSE;
    //need full update when return from erase mode.
    if( getOperationMode()== MODE_ERASE)
    {
        //every erase stroke ,we do it, so it's useless
        //core_redrawBgAndStrks(pScribbleUICtx);
        bNeedFlushWithFullQlt=TRUE;
    }		
    //we should add deal to pageup/pagedown
    switch (iconID)
    {
        case iconID_pen:
            //we switch by whether or not user is drawing
            if( !toolbar_is_drawing() ) 
            {
                setOperationMode(MODE_DRAW);
            }
            else 
            {
                setOperationMode(MODE_INVALID);
            } 
            break;
        case iconID_pen1pixel:
        case iconID_pen3pixel:
        case iconID_pen5pixel:
        case iconID_pen7pixel:
            //enter select mode
            setOperationMode(MODE_SELSIZE);
            break;
        case iconID_pen1pixelLow:
        case iconID_pen3pixelLow:
        case iconID_pen5pixelLow:
        case iconID_pen7pixelLow:
            //confirm the select                
            setPenSize(getPenSizefromIcon(iconID));
            setOperationMode(MODE_DRAW);
            break;
        case iconID_penwhite://WHITE
        case iconID_penlightgrey://LIGHT_GRAY
        case iconID_pendarkgrey://DARK_GRAY
        case iconID_penblack://BLACK
            //enter select mode
            setOperationMode(MODE_SELCOLOR);
            break;
        case iconID_penwhiteLow://WHITE
        case iconID_penlightgreyLow://LIGHT_GRAY
        case iconID_pendarkgreyLow://DARK_GRAY
        case iconID_penblackLow://BLACK
            //confirm the select                
            setPenColor(getPenColorfromIcon(iconID));
            setOperationMode(MODE_DRAW);
            break;
        case iconID_eraseline:
            if( MODE_ERASE==getOperationMode())
            {
                setOperationMode(MODE_DRAW);
            }
            else
            {
                setOperationMode(MODE_ERASE);
            }             
            break;
        case iconID_trashcan://delete page
            if(MODE_DELPAGE==getOperationMode())
            {                
                setOperationMode(MODE_DRAW);
                if(ispagedel_confirmed)
                {
		            //delete page
                    delOnepage(pScribbleUICtx);   
                    ispagedel_confirmed=FALSE;                    
                    return;//tool bar will be updated when jumping
                }  
                ispagedel_confirmed=FALSE;                  
            }
            else
            {
                SB_IPCPRINTF("enter MODE_DELPAGE\n");
                setOperationMode(MODE_DELPAGE);
                ispagedel_confirmed=FALSE;
            }
            break;
        case iconID_npage:
            addOnepage(pScribbleUICtx);
            return;//tool bar will be updated when jumping
        default:
            SB_IPCPRINTF("Unknown iconID [%d]\n", iconID);
            return;
    }
    
    drawIconsByMode(getTotalPage(&pScribbleUICtx->fileManager));
    //flush the pixmap to gdk_buffer,but now every stroke is flush,so it's useless.
    //flushPixMap(pScribbleUICtx);
    if(bNeedFlushWithFullQlt)
    {
        display_update_request_screen_refresh
                (MAIN_WINDOW_EXPOSE_LEVEL,(gpointer)dmQFull);
    }
    else
    {
        display_update_request_screen_refresh
                (MAIN_WINDOW_EXPOSE_LEVEL,(gpointer)dmQTyping);
    }
    SB_IPCPRINTF("\n===end===[mode=%d,pensize=%d,color=%d]\n",
                 getOperationMode(),getPenSize(),getPenColor());
    return;
}
