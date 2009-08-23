/*
 * This file is part of libergtk.
 *
 * libergtk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libergtk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file erGtkEntryFilter.c
 * \brief ereader gtk library - filters for erGtkEntry object
 *                              ipv4 filter and integer filter
 *
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

#include <ctype.h>
#include "erGtkEntryFilter.h"

static int is_ipv4(const char *ip)
{
    int i;
    int n;
    int field;
    int ok = TRUE;
    const char *cp;

    cp = ip;
    for (field = 0 ; field < 4 && ok ; field++)
    {
        i = 0;
        n = 0;
        while (isdigit(*cp)  &&  n < 4)
        {
            i = (10 * i) + (*cp - '0');
            n = n + 1;
            cp++;
        }
        if (n < 1  ||  n > 3  ||  i > 255)
        {
            ok = FALSE;
        }

        if (field < 3)
        {
            if (*cp == '.')
            {
                cp++;
                if (!isdigit(*cp))
                {
                    ok = FALSE;
                }
            }
            else
            {
                ok = FALSE;
            }
        }
        else
        {
            if (*cp)
            {
                ok = FALSE;
            }
        }
    }

    return ok;
}

static int is_ipv4_incomplete(const char *ip)
{
    int i;
    int n;
    int field;
    int ok = TRUE;
    const char *cp;

    cp = ip;
    for (field = 0 ; field < 4 && *cp && ok ; field++)
    {

        i = 0;
        n = 0;
        while (isdigit(*cp)  &&  n < 4)
        {
            i = (10 * i) + (*cp - '0');
            n = n + 1;
            cp++;
        }
        if (n < 1  ||  n > 3  ||  i > 255)
        {
            ok = FALSE;
        }

        if (*cp)
        {
            if (field < 3)
            {
                if (*cp == '.')
                {
                    cp++;
                }
                else
                {
                    ok = FALSE;
                }
            }
            else
            {
                if (*cp)
                {
                    ok = FALSE;
                }
            }
        }
    }

    return ok;
}

ipAddress_t ipv4_filter_check_address (const gchar *text)
{
    if (!text)
    {
        return ipv4Incomplete_e;
    }

    if (is_ipv4(text))
    {
        return ipv4Complete_e;
    }

    if (is_ipv4_incomplete(text))
    {
        return ipv4Incomplete_e;
    }
    
    return ipv4Error_e;
}

static gboolean check_string (gint filter, const gchar *str)
{
    ipAddress_t ret;
    
    ret = ipv4_filter_check_address (str);
    if ((ret == ipv4Complete_e) || (ret == ipv4Incomplete_e))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void insert_filter (GtkEditable *editable, 
                           const gchar *text, 
                           gint length, 
                           gint *pos, 
                           gpointer data)
{
    gint   filter;
    gchar *str, *pre, *post;

    filter = GPOINTER_TO_INT (data);
    pre  = gtk_editable_get_chars (editable, 0, *pos);
    post = gtk_editable_get_chars (editable, *pos, -1);

    str = g_strconcat (pre, text, post, NULL);

    if (!check_string (filter, str))
    {
        g_signal_stop_emission_by_name (G_OBJECT (editable), "insert-text");
    }

    g_free (pre);
    g_free (post);
    g_free (str);
}

static void delete_filter (GtkEditable *editable, 
                           gint start, 
                           gint end, 
                           gpointer data)
{
    gint   filter;
    gchar *str, *pre, *post;

    filter = GPOINTER_TO_INT (data);
    pre  = gtk_editable_get_chars (editable, 0, start);
    post = gtk_editable_get_chars (editable, end, -1);

    str = g_strconcat (pre, post, NULL);

    if (!check_string (filter, str))
    {
        g_signal_stop_emission_by_name (G_OBJECT (editable), "delete-text");
    }

    g_free (pre);
    g_free (post);
    g_free (str);
}

void ipv4_filter_init(GtkEntry *entry)
{
    g_signal_connect (G_OBJECT (entry), "insert-text",
                    G_CALLBACK (insert_filter), NULL);

    g_signal_connect (G_OBJECT (entry), "delete-text",
                    G_CALLBACK (delete_filter), NULL);

}

static void insert_integer_filter(GtkEditable *editable, 
                                const gchar *text, 
                                gint text_length, 
                                gint *pos, 
                                gpointer data)
{
     int i, j; 
     gchar *result;

     if (text_length > 0)
     {
         // make sure the text is a digit 
         result = g_new( gchar, text_length ); 
         for ( i = 0, j = 0; i < text_length; ++i )
         {
             if ( isdigit(text[i]) )  
             {
                 result[j++] = text[i]; 
             }
         }
         result[j] = '\0'; 

         if ( j > 0 ) 
         { 
             gtk_signal_handler_block_by_func( GTK_OBJECT(editable), 
                     GTK_SIGNAL_FUNC(insert_integer_filter), data ); 
             gtk_editable_insert_text( editable, result, j, pos ); 
             gtk_signal_handler_unblock_by_func( GTK_OBJECT(editable), 
                     GTK_SIGNAL_FUNC(insert_integer_filter), data ); 
         } 
         gtk_signal_emit_stop_by_name( GTK_OBJECT(editable), 
                                       "insert-text" ); 
      
         g_free(result); 
     }
}


void integer_filter_init(GtkEntry *entry)
{
    g_signal_connect (G_OBJECT (entry), "insert-text",
                    G_CALLBACK (insert_integer_filter), NULL);
}

