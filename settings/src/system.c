/*
 * This file is part of settings.
 *
 * settings is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * settings is distributed in the hope that it will be useful,
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


#include <glib.h>
#include <config.h>
#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

#include "setupLog.h"
#include "system.h"


#define MAX_ARGS 255

// Internal functions
static const char* get_waveform_type();
static const char* get_waveform_version();

/* Exec a command like the shell would. - func by p.blundell*/
int shell_exec(const char *cmd)
{
    const char *p = cmd;
    char *buf = alloca(strlen(cmd) + 1), *bufp = buf;
    char *argv[MAX_ARGS + 1];
    int nargs = 0;
    int escape = 0, squote = 0, dquote = 0;
    int rc;
    int i;

    if (cmd[0] == 0)
    {
        errno = ENOENT;
        return -1;
    }

    while (*p)
    {
        if (escape)
        {
            *bufp++ = *p;
            escape = 0;
        }
        else
        {
            switch (*p)
            {
                case '\\':
                    escape = 1;
                    break;
                case '"':
                    if (squote)
                        *bufp++ = *p;
                    else
                        dquote = !dquote;
                    break;
                case '\'':
                    if (dquote)
                        *bufp++ = *p;
                    else
                        squote = !squote;
                    break;
                case ' ':
                    if (!squote && !dquote)
                    {
                        *bufp = 0;
                        if (nargs < MAX_ARGS)
                            argv[nargs++] = strdup(buf);
                        bufp = buf;
                        break;
                    }
                default:
                    *bufp++ = *p;
                    break;
            }
        }
        p++;
    }

    if (bufp != buf)
    {
        *bufp = 0;
        if (nargs < MAX_ARGS)
            argv[nargs++] = strdup(buf);
    }

    argv[nargs] = NULL;
    rc = execvp(argv[0], argv);

    for (i = 0; i < nargs; i++)
        free(argv[i]);

    return rc;
}

#define PINCODE_LENGTH          (4*2)
#define PINCODE_HEX_DECORATION  2

#define MAC_LENGTH              12
#define MAC_HEX_DECORATION      2

#define PROCESS_BUFFER_MAX      (2*1024)

const char *get_sysset_macAddress(int offset)
{
    FILE *POUT = 0;
    int nProcessRet = 0;
    char cmd[32];

    static char mac_address[MAC_LENGTH + 6] = "  -  -  -  -  -  ";

    sprintf(cmd, "sysset -r -b -a %d -l 6", offset);
    POUT = popen(cmd, "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read EUI64");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                int nLen;

                // Normal case
                //
                // The string as retrieved from sysset is 15 (MAX_HEX_DECORATION+MAC_LENGTH+1) characters long.
                //   e.g 0x00167c000000\n
                // The first two (MAX_HEX_DECORATION) characters need to be removed
                // and dashes inserted to separate byte values
                //   resulting in: 00-16-7c-00-00-00
                //
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));
                nLen = strlen(szBuffer);
                if (nLen != MAC_LENGTH + 2 + 1)
                {
                    ST_ERRORPRINTF("Wrong length [%d] of MAC address [%s]", nLen, szBuffer);
                }
                else
                {
                    char *cp1 = mac_address;
                    char *cp2 = szBuffer + MAC_HEX_DECORATION;
                    for (index = 0; index < MAC_LENGTH; index += 2)
                    {
                        // m.s. nibble
                        *cp1 = tolower(*cp2);
                        cp1++;
                        cp2++;

                        // l.s. nibble
                        *cp1 = tolower(*cp2);
                        cp1++;
                        cp2++;

                        // skip byte separator
                        cp1++;
                    }
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving MAC", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ST_LOGPRINTF("return [%s]", mac_address);
    return mac_address;
}

const char *get_sysset_product_id(void)
{
    return get_sysset_macAddress(262);
}

const char *get_wireless_macAddress(void)
{
    return get_sysset_macAddress(270);
}

const char *get_wired_macAddress(void)
{
    char *cp;
    FILE *POUT = 0;
    int nProcessRet = 0;

    static char mac_address[MAC_LENGTH + 6] = "  -  -  -  -  -  ";


    POUT = popen("/usr/bin/wired.sh mac | tail -1", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read wired MAC address");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                int nLen;

                // Normal case
                //
                // The string as retrieved from wired.sh 17 (MAC_LENGTH+5+1) characters long.
                //   e.g 00-16-7c-00-00-00\n
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));
                nLen = strlen(szBuffer);
                if (nLen != MAC_LENGTH + 5 + 1)
                {
                    ST_ERRORPRINTF("Wrong length [%d] of MAC address [%s]", nLen, szBuffer);
                }
                else
                {
                    strncpy(mac_address, szBuffer, MAC_LENGTH + 5);
                    for (cp = mac_address ; *cp ; cp++)
                    {
                        *cp = tolower(*cp);
                    }
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving MAC", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ST_LOGPRINTF("return [%s]", mac_address);
    return mac_address;
}

////////////////////////////////////////////////////////////////////////////////
// pincode in sysset
// 
// Pin code on/off      s 58 1 1 "1"/"0" = on/off           "0" "0"
// Pin code         b 59 4 4 binary integer 0 … 99999999 0    0x00
//
gint sysset_read_pincode_onoff(gboolean * enable)
{
    FILE *POUT = 0;
    int nProcessRet = 0;
    int nRet = -1;              // 0 = ok, -1 = error

    if (NULL == enable)
        return nRet;
    *enable = FALSE;

    POUT = popen("sysset -r -a 58 -l 1", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read PINCODE on/off");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[100];

        while (((nChar = getc(POUT)) != EOF) && (index < sizeof(szBuffer) - 1))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("Read PINCODE onoff- echoed %s", szBuffer);
        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                int nLen;

                // The string as retrieved by sysset is 1 + 1 (newline) character long.
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));
                nLen = strlen(szBuffer);
                // it's old sysset version, treat binary 0 as "0"
                if (0 == nLen)
                {
                    szBuffer[0] = '0';
                    szBuffer[1] = '\n';
                    szBuffer[2] = '\0';
                    nLen = 2;
                }
                if (nLen != 2)
                {
                    ST_ERRORPRINTF("Retrieved pincode on/off [%s] wrong length [%d]", szBuffer, nLen);
                }
                else
                {
                    // remove '\n'
                    if (szBuffer[nLen - 1] == '\n')
                    {
                        nLen = nLen - 1;
                        szBuffer[nLen] = '\0';
                    }

                    if (strcmp(szBuffer, "0") == 0)
                    {
                        *enable = FALSE;
                        nRet = 0;       // ok
                        ST_LOGPRINTF("Read PINCODE onoff: %d", *enable);
                    }
                    else if (strcmp(szBuffer, "1") == 0)
                    {
                        *enable = TRUE;
                        nRet = 0;       // ok
                        ST_LOGPRINTF("Read PINCODE onoff: %d", *enable);
                    }
                    else
                    {
                        ST_ERRORPRINTF("Retrieved pincode on/off has illegal value: [%s]", szBuffer);
                    }
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving pincode on/off", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("Read PINCODE onoff- pclose did not exit normally");
        }
    }

    ST_LOGPRINTF("Read PINCODE onoff returned %d", nRet);
    return nRet;
}

gint sysset_write_pincode_onoff(gboolean enable)
{
    char szCommand[32] = "";
    int nProcessRet = 0;
    int nRet;

    if (TRUE == enable)
        strcpy(szCommand, "sysset -w -a 58 -l 1 -v 1");
    else
        strcpy(szCommand, "sysset -w -a 58 -l 1 -v 0");
    ST_LOGPRINTF("Calling: %s", szCommand);
    nProcessRet = system(szCommand);
    nRet = WEXITSTATUS(nProcessRet);
    ST_LOGPRINTF("Write PINCODE onoff returned: %d", nRet);
    return nRet;
}

// need to deal with the cases below
// 1) pincode is empty, in sysset 0x00000000
// 2) pincode = "0000" pincode = "000000"
// as 99999999 is 0x05F5E0FF, there's one half of byte unoccupied in the pincode area in syset
// we can use it to store the length of pincode because the max length of pincode is 8, less than 15
gint sysset_read_pincode_string(gchar * pincode)
{
    FILE *POUT = 0;
    int nProcessRet = 0;
    char string[PINCODE_MAX_LENGTH + 1] = "";
    int nRet = -1;              // 0 = ok, -1 = error

    if (NULL == pincode)
        return nRet;

    POUT = popen("sysset -r -b -a 59 -l 4", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Sysset could not read PINCODE");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("Read PINCODE echoed %s", szBuffer);
        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                int nLen;
                //
                // The string as retrieved from sysset is 11(PINCODE_HEX_DECORATION+PINCODE_LENGTH+1) characters long.
                //   e.g 0x85F5E0FF\n
                // "99999999"<--99999999<--"0x05F5E0FF"<--"0x85F5E0FF\n"
                // firstly remove the length of pincode in '0x85F5E0FF\n'
                // then, convert hex string to integer, convert the integer to string 
                //
                ST_LOGPRINTF("Read PINCODE-pclose returned %d", WEXITSTATUS(nProcessRet));
                nLen = strlen(szBuffer);
                if (nLen != (PINCODE_HEX_DECORATION + PINCODE_LENGTH + 1))
                {
                    ST_ERRORPRINTF("Wrong length [%d] of PINCODE address [%s]", nLen, szBuffer);
                }
                else
                {
                    char ch[2];
                    int len;
                    int integer = 0;
                    // remove '\n'
                    if (szBuffer[nLen - 1] == '\n')
                    {
                        nLen = nLen - 1;
                        szBuffer[nLen] = '\0';
                    }
                    // get the length of pincode
                    ch[0] = szBuffer[strlen("0x8") - 1];        // ch='8'
                    ch[1] = '\0';
                    len = strtoul(ch, NULL, 16);
                    if (len > PINCODE_MAX_LENGTH)
                        len = PINCODE_MAX_LENGTH;
                    // remove the lenght of pincode
                    szBuffer[strlen("0x8") - 1] = '0';
                    // convert hex string to integer
                    integer = strtoul(szBuffer, NULL, 16);
                    // convert integer to string
                    sprintf(string, "%0*d", len, integer);
                    // deal with some special cases, treat 0x00000000 as 0x40000000
                    if (0 == len && 0 == integer)
                    {
                        strcpy(string, "0000");
                    }
                    strcpy(pincode, string);
                    ST_LOGPRINTF("PINCODE read returned [%s]", pincode);
                    nRet = 0;
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving PINCODE", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("Read PINCODE pclose did not exit normally");
        }
    }
    return nRet;
}

gint sysset_write_pincode_string(const gchar * pincode)
{
    int nLen;
    unsigned int n, integer = 0;
    char string[32];

    char szCommand[32] = "";
    int nProcessRet = 0;
    int nRet = -1;              // -1 = error

    if (NULL == pincode)
        return nRet;
    nLen = strlen(pincode);
    if (0 == nLen || (nLen >= PINCODE_MIN_LENGTH && nLen <= PINCODE_MAX_LENGTH))
    {
        if (0 == nLen)          // empty pincode
        {
            integer = 0;
        }
        else                    // (nLen >= PINCODE_MIN_LENGTH && nLen <= PINCODE_MAX_LENGTH)
        {
            // pincode ="99999999"--> integer=99999999=0x05F5E0FF-->0x85F5E0FF=2247483647
            integer = atoi(pincode);    // integer = 99999999
            sprintf(string, "0x%d0000000", nLen);       // string = "0x80000000"
            n = strtoul(string, NULL, 16);      // n = 2147483648
            integer += n;       // 2247483647 (0x85F5E0FF)
        }
        sprintf(szCommand, "sysset -w -b -a 59 -l 4 -v 0x%08X", integer);
        ST_LOGPRINTF("Calling: %s", szCommand);
        nProcessRet = system(szCommand);
        nRet = WEXITSTATUS(nProcessRet);
    }
    else
    {
        ST_ERRORPRINTF("Write PINCODE wrong length %d", nLen);
    }
    return nRet;
}

gboolean sysset_is_pincode_empty()
{
    gchar pincode[PINCODE_MAX_LENGTH + 1];
    int nLen;
    gboolean empty = FALSE;

    sysset_read_pincode_string(pincode);
    nLen = strlen(pincode);
    if (0 == nLen)
        empty = TRUE;
    return empty;
}


// Other sysset fields

// Get FA model-id from sysset
const char *get_sysset_fa_model(void)
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    int  i;
    static char fa_model[33] = "";

    POUT = popen("sysset -r -a 16 -l 32", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read FA model-id");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));

                // store FA model-id
                snprintf(fa_model, sizeof(fa_model), "%s", szBuffer);

                // strip trailing spaces
                for ( i = strlen(fa_model) - 1 ;
                      i > 0  &&  fa_model[i] == ' ' ;
                      i-- )
                {
                    fa_model[i] = '\0';
                }  
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving FA model-id", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ST_LOGPRINTF("return [%s]", fa_model);
    return fa_model;
}

// Get FA region-code from sysset
int get_sysset_fa_region(void)
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    static int fa_region = 0;

    POUT = popen("sysset -r -b -a 50 -l 2", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read FA region-code");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));

                // store FA region-code
                if (szBuffer[0] == '0'  &&  szBuffer[1] == 'x')
                {
                    fa_region = atoi(szBuffer + 2);
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving FA region-code", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("pclose did not exit normally");
        }
    }
    return fa_region;
}

// Get FA oem-code from sysset
const char *get_sysset_fa_oem_code(void)
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    int  n;
    static char fa_oem_code[4] = "";

    POUT = popen("sysset -r -a 52 -l 3", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read FA oem-code");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));

                // strip trailing spaces and new-line
                n = strlen(szBuffer);
                while ( n > 0  &&  isspace( szBuffer[n-1] ) )
                {
                    n--;
                    szBuffer[n] = '\0';
                }

                // store FA oem-code
                if (n == 0)
                {
                    snprintf(fa_oem_code, sizeof(fa_oem_code), "000");
                }
                else if (n == 3)
                {
                    snprintf(fa_oem_code, sizeof(fa_oem_code), "%s", szBuffer);
                }
                else
                {
                    ST_ERRORPRINTF("invalid fa_oem_code from sysset [%s]", szBuffer);
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving FA oem-code", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ST_LOGPRINTF("return [%s]", fa_oem_code);
    return fa_oem_code;
}

// Get FA country-code from sysset
const char *get_sysset_fa_country_code(void)
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    int  n;
    static char fa_country_code[4] = "";

    POUT = popen("sysset -r -a 55 -l 3", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read FA country-code");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));

                // strip trailing spaces and new-line
                n = strlen(szBuffer);
                while ( n > 0  &&  isspace( szBuffer[n-1] ) )
                {
                    n--;
                    szBuffer[n] = '\0';
                }

                // store FA country-code
                if (n == 0)
                {
                    snprintf(fa_country_code, sizeof(fa_country_code), "000");
                }
                else if (n == 3)
                {
                    snprintf(fa_country_code, sizeof(fa_country_code), "%s", szBuffer);
                }
                else
                {
                    ST_ERRORPRINTF("invalid fa_country_code from sysset [%s]", szBuffer);
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving FA country-code", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ST_LOGPRINTF("return [%s]", fa_country_code);
    return fa_country_code;
}

// Get PA Board-revision from sysset
int get_sysset_pa_board_revision(void)
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    static int pa_board_revision = 0;

    POUT = popen("sysset -r -b -a 260 -l 1", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read PA Board-revision");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));

                // store PA Board-revision
                if (szBuffer[0] == '0'  &&  szBuffer[1] == 'x')
                {
                    pa_board_revision = atoi(szBuffer + 2);
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving PA board-revision", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("pclose did not exit normally");
        }
    }
    return pa_board_revision;
}

// Get DM waveform type
const char *get_waveform_type()
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    int  n;
    static char dm_waveform_type[7] = "";

    POUT = popen("sysset -r -a 186 -l 6", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read DM waveform type");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));

                // strip trailing spaces and new-line
                n = strlen(szBuffer);
                while ( n > 0  &&  isspace( szBuffer[n-1] ) )
                {
                    n--;
                    szBuffer[n] = '\0';
                }

                // store DM waveform type
                if (n == 0)
                {
                    snprintf(dm_waveform_type, sizeof(dm_waveform_type), "000000");
                }
                else if (n == 6)
                {
                    snprintf(dm_waveform_type, sizeof(dm_waveform_type), "%s", szBuffer);
                }
                else
                {
                    ST_ERRORPRINTF("invalid DM waveform type from sysset [%s]", szBuffer);
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving DM waveform type", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ST_LOGPRINTF("return [%s]", dm_waveform_type);
    return dm_waveform_type;
}

// Get DM waveform version
const char *get_waveform_version()
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    int  n;
    static char dm_waveform_version[3] = "";

    POUT = popen("sysset -r -a 194 -l 2", "r");
    if (POUT == NULL)
    {
        ST_ERRORPRINTF("Could not read DM waveform version");
    }
    else
    {
        int nChar;
        int index = 0;
        char szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char)nChar;
            index++;
        }
        szBuffer[index] = '\0';
        ST_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                ST_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));

                // strip trailing spaces and new-line
                n = strlen(szBuffer);
                while ( n > 0  &&  isspace( szBuffer[n-1] ) )
                {
                    n--;
                    szBuffer[n] = '\0';
                }

                // store DM waveform version
                if (n == 0)
                {
                    snprintf(dm_waveform_version, sizeof(dm_waveform_version), "00");
                }
                else if (n == 2)
                {
                    snprintf(dm_waveform_version, sizeof(dm_waveform_version), "%s", szBuffer);
                }
                else
                {
                    ST_ERRORPRINTF("invalid dm_waveform_version from sysset [%s]", szBuffer);
                }
            }
            else
            {
                ST_ERRORPRINTF("pclose returned %d. Error retrieving DM waveform version", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ST_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ST_LOGPRINTF("return [%s]", dm_waveform_version);
    return dm_waveform_version;
}

const char *get_sysset_epd_id(void)
{
    static char dm_epd_id[9];

    snprintf(dm_epd_id, 9, "%s%s", get_waveform_type(), get_waveform_version());
    return dm_epd_id;
}

