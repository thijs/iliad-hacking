#ifndef SCBMACRO_H_
#define SCBMACRO_H_

//////////////////////////////////////////////////////////////////////////
// scribble library macro 
// In this file, we will define some macroes, so that callers can reuse 
// them in their application.
// The reason of using macro instead of function is that
// we do not want to bind to specified gui library, such as gtk or X11
//////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
extern "C"
{
#endif

//////////////////////////////////////////////////////////////////////////
// using gtk to draw page
// all the parameters are pointers
#define SCB_GTK_DRAW_PAGE(doc, pageId, drawable, gc)\
{\
    if (doc)\
    {\
        ScbPagePtr SCB_page = scb_doc_get_page(doc, pageId);\
        if (SCB_page)\
        {\
            ScbStrokesPtr SCB_strokes = scb_page_get_strokes(SCB_page);\
            if (SCB_strokes)\
            {\
                ScbStrokePtr SCB_stroke = NULL;\
                GdkColor GDK_color;\
                ScbColor SCB_color;\
                GdkLineStyle GDK_lineStyle = GDK_LINE_SOLID; \
                GList *SCB_ptr = g_list_first(SCB_strokes->strokes);\
                while (SCB_ptr)\
                {\
                    SCB_stroke = (ScbStrokePtr)SCB_ptr->data;\
                    if (SCB_stroke)\
                    {\
                        scb_dev_color_to_color(&SCB_color, SCB_stroke->style.color);\
                        GDK_color.pixel = SCB_color.pixel;\
                        GDK_color.red = SCB_color.red;\
                        GDK_color.green = SCB_color.green;\
                        GDK_color.blue = SCB_color.blue;\
                        gdk_gc_set_rgb_fg_color(gc, &GDK_color);\
                        gdk_gc_set_line_attributes (gc, \
                            SCB_stroke->style.penSize, \
                            GDK_lineStyle, \
                            GDK_CAP_PROJECTING, \
                            GDK_JOIN_MITER);\
                        gdk_draw_lines(drawable, \
                            gc, \
                            (GdkPoint *)scb_stroke_get_point_data(SCB_stroke), \
                            scb_stroke_get_point_count(SCB_stroke));\
                    }\
                    SCB_ptr = g_list_next(SCB_ptr);\
                }\
            }\
        }\
    }\
}


//////////////////////////////////////////////////////////////////////////
// macro for toolbar
//////////////////////////////////////////////////////////////////////////

// ScbDocPtr doc 
// erClientChannel_t channel
// int/enum appId
#define SCB_TOOLBAR_INIT(doc, channel, id)\
{\
    doc->context.appId = id;\
    SCB_TOOLBAR_UPDATE(doc, channel);\
}

#define SCB_TOOLBAR_MAP(doc, s, i)\
{\
    ScbTBSItem item;\
    item.state  = s;\
    item.iconId = i;\
    scb_doc_add_map_item(doc, &item);\
}

#define SCB_TOOLBAR_UPDATE(doc, channel)\
{\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_pen);\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_pen1pixel);\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_pen3pixel);\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_pen5pixel);\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_pen7pixel);\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_pencolorbl);\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_pencolordg);\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_pencolorlg);\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_pencolorwh);\
    tbRemovePlatformIcon(channel, doc->context.appId, iconID_eraseline);\
    int __tbi;\
    ScbTBSItemPtr __tbitem = (ScbTBSItemPtr)doc->context.table->data;\
    for(__tbi = 0; __tbi < doc->context.table->len; ++__tbi)\
    {\
        if (SCB_TBS_SCRIBBLE == __tbitem->state)\
        {\
            tbAppendPlatformIcon(channel, doc->context.appId, __tbitem->iconId, ccVwrToolbar);\
        }\
        ++__tbitem;\
    }\
    if (SCB_TBS_SCRIBBLE == doc->context.curState)\
    {\
        tbSetStatePlatformIcon(channel, doc->context.appId, iconID_pen, iconState_selected);\
    }\
    int __tbIconIndex = (int)iconID_pen1pixel + doc->context.curStrokeStyle.penSize/2;\
    __tbitem = (ScbTBSItemPtr)doc->context.table->data;\
    if (SCB_TBS_SELECT_PEN_SIZE == doc->context.curState)\
    {\
        for(__tbi = 0; __tbi < doc->context.table->len; ++__tbi)\
        {\
            if (SCB_TBS_SELECT_PEN_SIZE == __tbitem->state)\
            {\
                tbAppendPlatformIcon(channel, doc->context.appId, __tbitem->iconId, ccVwrToolbar);\
            }\
            ++__tbitem;\
        }\
        tbSetStatePlatformIcon(channel, doc->context.appId, __tbIconIndex, iconState_selected);\
    }\
    else\
    {\
        for(__tbi = 0; __tbi < doc->context.table->len; ++__tbi)\
        {\
            if (__tbIconIndex == __tbitem->iconId)\
            {\
                tbAppendPlatformIcon(channel, doc->context.appId, __tbitem->iconId, ccVwrToolbar);\
                break;\
            }\
            ++__tbitem;\
        }\
    }\
    __tbIconIndex = (int)iconID_pencolorbl + (int)SCB_DEV_COLOR_BLACK - (int)doc->context.curStrokeStyle.color;\
    __tbitem = (ScbTBSItemPtr)doc->context.table->data;\
    if (SCB_TBS_SELECT_LINE_COLOR == doc->context.curState)\
    {\
        for(__tbi = 0; __tbi < doc->context.table->len; ++__tbi)\
        {\
            if (SCB_TBS_SELECT_LINE_COLOR == __tbitem->state)\
            {\
                tbAppendPlatformIcon(channel, doc->context.appId, __tbitem->iconId, ccVwrToolbar);\
            }\
            ++__tbitem;\
        }\
        tbSetStatePlatformIcon(channel, doc->context.appId, __tbIconIndex, iconState_selected);\
    }\
    else\
    {\
        for(__tbi = 0; __tbi < doc->context.table->len; ++__tbi)\
        {\
            if (__tbIconIndex == __tbitem->iconId)\
            {\
                tbAppendPlatformIcon(channel, doc->context.appId, __tbitem->iconId, ccVwrToolbar);\
                break;\
            }\
            ++__tbitem;\
        }\
    }\
    __tbitem = (ScbTBSItemPtr)doc->context.table->data;\
    for(__tbi = 0; __tbi < doc->context.table->len; ++__tbi)\
    {\
        if (SCB_TBS_ERASE == __tbitem->state)\
        {\
            tbAppendPlatformIcon(channel, doc->context.appId, __tbitem->iconId, ccVwrToolbar);\
        }\
        ++__tbitem;\
    }\
    if (SCB_TBS_ERASE == doc->context.curState)\
    {\
        tbSetStatePlatformIcon(channel, doc->context.appId, iconID_eraseline, iconState_selected);\
    }\
}

//////////////////////////////////////////////////////////////////////////
// take action when current state is scribble
// action: !scribble
// shrink all icons
// set icon(only scribble) state to selected
//////////////////////////////////////////////////////////////////////////
#define SCB_ACTION_SCRIBBLE(doc, channel, iconId)\
{\
    if (SCB_TBS_SCRIBBLE == doc->context.curState)\
    {\
        doc->context.curState = SCB_TBS_NONE;\
    }\
    else\
    {\
        doc->context.curState = SCB_TBS_SCRIBBLE;\
    }\
    SCB_TOOLBAR_UPDATE(doc, channel)\
}

//////////////////////////////////////////////////////////////////////////
// take action when current state is selecting pen size
//////////////////////////////////////////////////////////////////////////
#define SCB_ACTION_SELECT_PEN_SIZE(doc, channel, iconId)\
{\
    if (SCB_TBS_SELECT_PEN_SIZE != doc->context.curState)\
    {\
        doc->context.curState = SCB_TBS_SELECT_PEN_SIZE;\
    }\
    else\
    {\
        doc->context.curState = SCB_TBS_SCRIBBLE;\
        doc->context.curStrokeStyle.penSize = ((int)iconId - (int)iconID_pen1pixel) * 2 + 1;\
    }\
    SCB_TOOLBAR_UPDATE(doc, channel)\
}    

//////////////////////////////////////////////////////////////////////////
// take action when current state is selecting line color
//////////////////////////////////////////////////////////////////////////
#define SCB_ACTION_SELECT_LINE_COLOR(doc, channel, iconId)\
{\
    if (SCB_TBS_SELECT_LINE_COLOR != doc->context.curState)\
    {\
        doc->context.curState = SCB_TBS_SELECT_LINE_COLOR;\
    }\
    else\
    {\
        doc->context.curState = SCB_TBS_SCRIBBLE;\
        doc->context.curStrokeStyle.color = SCB_DEV_COLOR_BLACK - ((int)iconId - (int)iconID_pencolorbl);\
    }\
    SCB_TOOLBAR_UPDATE(doc, channel)\
}

//////////////////////////////////////////////////////////////////////////
// take action when current state is erasing
//////////////////////////////////////////////////////////////////////////
#define SCB_ACTION_ERASE(doc, channel, iconId)\
{\
    if (SCB_TBS_ERASE != doc->context.curState)\
    {\
        doc->context.curState = SCB_TBS_ERASE;\
    }\
    else\
    {\
        doc->context.curState = SCB_TBS_SCRIBBLE;\
    }\
    SCB_TOOLBAR_UPDATE(doc, channel)\
}

//////////////////////////////////////////////////////////////////////////
// receive command and change state according to current state and input state
//////////////////////////////////////////////////////////////////////////
#define SCB_TOOLBAR_ON_RECEIVE_MSG(doc, channel, iconId)\
{\
    if (iconID_pen == iconId)\
    {\
        SCB_ACTION_SCRIBBLE(doc, channel, iconId);\
    }\
    else if ((int)iconID_pen1pixel <= iconId && (int)iconID_pen7pixel >= iconId)\
    {\
        SCB_ACTION_SELECT_PEN_SIZE(doc, channel, iconId);\
    }\
    else if ((int)iconID_pencolorbl <= iconId && (int)iconID_pencolorwh >= iconId)\
    {\
        SCB_ACTION_SELECT_LINE_COLOR(doc, channel, iconId);\
    }\
    else if (iconID_eraseline == iconId)\
    {\
        SCB_ACTION_ERASE(doc, channel, iconId);\
    }\
}


#ifdef __cplusplus
}
#endif

#endif 
