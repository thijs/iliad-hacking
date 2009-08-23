/* ------------------------------------------------------------------------- */ 
/*      Copyright (c) 2005 Koninklijke Philips Electronics N.V.              */ 
/*                             All rights reserved.                          */ 
/*   This source code and any compilation or derivative thereof is the       */ 
/*   proprietary information of Koninklijke Philips Electronics N.V. and     */ 
/*   is confidential in nature.                                              */ 
/*                                                                           */ 
/*   Under no circumstances is this software to be combined with any Open    */ 
/*   Source Software in any way or placed under an Open Source License of    */ 
/*   any type without the express written permission of                      */ 
/*   Koninklijke Philips Electronics N.V.                                    */ 
/* ------------------------------------------------------------------------- */
#ifndef ERLOG_H
#define ERLOG_H

#ifdef __cplusplus
extern  "C"
{
#endif                          /* __cplusplus */

typedef enum {
     power_on    = 0x01,
     power_off   = 0x02,
     auto_off    = 0x03,
     connect     = 0x04,
     reset       = 0x05,
     wake_up     = 0x06
} system_event_t, *psystem_event_t;


void erlog_write_start (void);
void erlog_event_system (system_event_t system_event);
void erlog_event_button (int key_number); 
void erlog_event_pen_click (int x, int y, int z);
void erlog_event_open (char *uri); 
void erlog_event_follow_link (char *on_page_uri, char *link_uri);
void erlog_splice_log (char *infile_name, char *outfile_name);
void erlog_event_user_change (int user_id);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif
