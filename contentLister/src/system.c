/*
 * This file is part of contentLister.
 *
 * contentLister is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * contentLister is distributed in the hope that it will be useful,
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


#include <config.h>

#include <alloca.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <glib.h>
#include <gdk/gdk.h>

#include "contentListerLog.h"
#include "system.h"


#define MAX_ARGS 255

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
                        while (*(p+1) == ' ')
                            p++;
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

// Execute specified command with arguments in child process
int fork_exec(const int argc, char *const argv[])
{
    int rc = -1;
    int pid;
    int status;

    CL_LOGPRINTF("entry: [%s]", argv[0]);
    g_assert(argv[argc] == NULL);
    
    // spawn child process
    switch (pid = fork())
    {
        case 0:
            // child process: execute command
            rc = execvp(argv[0], argv);
            CL_ERRORPRINTF("execvp [%s] returns [%d] errno [%d] - %s", argv[0], rc, errno, strerror(errno));
            exit(1);

        case -1:
            // error: fork failed
            CL_ERRORPRINTF("fork returns [%d] errno [%d] - %s", pid, errno, strerror(errno));
            g_assert_not_reached();
            break;

        default:
            // parent process: wait for child and return its exit value
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                rc = WEXITSTATUS(status);
            }
    }

    return rc;
}


int scGet_Reconnect(gboolean* connect_after_reboot)
{
    FILE     *POUT = 0;
    int      nProcessRet = 0;

    int      nRet = -1;              // 0 = ok, -1 = error
    *connect_after_reboot = FALSE;   // safe value


    POUT = popen("sysset -r -a 153 -l 1", "r");
    if (POUT == NULL)
    {
        CL_ERRORPRINTF("Could not read Connect_After_Reboot");
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
        CL_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        gboolean ok = FALSE;
        if (nProcessRet == -1)
        {
            if (errno == ECHILD)
            {
                // child process has vanished,
                // probably because our on_sigchld() catched the signal
                ok = TRUE;
            }
        }
        else if ( WIFEXITED(nProcessRet)  &&  WEXITSTATUS(nProcessRet) == 0 )
        {
            // process has exited with status ok
            ok = TRUE;
        }
        if (ok)
        {
            // The string as retrieved by sysset is 1 + 1 (newline) character long.
            int nLen = strlen(szBuffer);
            if (szBuffer[nLen - 1] == '\n')
            {
                nLen = nLen - 1;
                szBuffer[nLen] = '\0';
            }
            if (nLen != 1)
            {
                CL_ERRORPRINTF("Retrieved Reconnect flag [%s] wrong length [%d]", szBuffer, nLen);
            }
            else
            {
                if (strcmp(szBuffer, "0") == 0)
                {
                    *connect_after_reboot = FALSE;
                    nRet = 0;       // ok
                }
                else if (strcmp(szBuffer, "1") == 0)
                {
                    *connect_after_reboot = TRUE;
                    nRet = 0;       // ok
                }
                else
                {
                    CL_ERRORPRINTF("Retrieved Reconnect flag has illegal value: [%s]", szBuffer);
                }
            }
        }
        else
        {
            CL_ERRORPRINTF("pclose did not exit normally");
        }
    }

    return nRet;
}

gboolean is_battery_charging(void)
{
    gboolean is_charging = FALSE;
    unsigned int battery_current;
    short current;

    int pwr_fd = open(POWERMGMTIFACE, O_RDWR);
    if (pwr_fd < 0)
    {
        CL_ERRORPRINTF("Error opening /dev/battery - %s", strerror(errno));
    }
    else
    {
        if (ioctl(pwr_fd, BATTERY_IOCTL_READ_CURRENT, &battery_current) == -1)  // in milliamps, positive or negative
        {
            CL_ERRORPRINTF("ioctl read current failed - %s", strerror(errno));
        }
        else
        {
            current = battery_current & 0xFFFF; // convert from unsigned int to short
            if (current > 0)
            {
                is_charging = TRUE;
            }
        }

        close(pwr_fd);
    }

    return is_charging;
}


#define PINCODE_LENGTH          (4*2)
#define PINCODE_HEX_DECORATION  2

#define MAC_LENGTH              12
#define MAC_HEX_DECORATION      2

#define PROCESS_BUFFER_MAX      (2*1024)

const char* get_sysset_macAddress(void)
{
    FILE     *POUT = 0;
    int      nProcessRet = 0;
    
    static char mac_address[MAC_LENGTH+6] = "  -  -  -  -  -  ";


    POUT = popen("sysset -r -b -a 262 -l 6", "r");
    if (POUT == NULL)
    {
        CL_ERRORPRINTF("Could not read EUI64");
    }
    else
    {
        int     nChar;
        int     index = 0;
        char    szBuffer[PROCESS_BUFFER_MAX + 1];

        while (((nChar = getc(POUT)) != EOF) && (index < PROCESS_BUFFER_MAX))
        {
            szBuffer[index] = (char) nChar;
            index++;
        }
        szBuffer[index] = '\0';
        CL_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        gboolean ok = FALSE;
        if (nProcessRet == -1)
        {
            if (errno == ECHILD)
            {
                // child process has vanished,
                // probably because our on_sigchld() catched the signal
                ok = TRUE;
            }
        }
        else if ( WIFEXITED(nProcessRet)  &&  WEXITSTATUS(nProcessRet) == 0 )
        {
            // process has exited with status ok
            ok = TRUE;
        }
        if (ok)
        {
            // The string as retrieved from sysset is 15 (MAX_HEX_DECORATION+MAC_LENGTH+1) characters long.
            //   e.g 0x00167c000000\n
            // The first two (MAX_HEX_DECORATION) characters need to be removed
            // and dashes inserted to separate byte values
            //   resulting in: 00-16-7c-00-00-00
            //
            CL_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));
            int nLen = strlen(szBuffer);
            if (nLen != MAC_LENGTH + 2 + 1)
            {
                CL_ERRORPRINTF("Wrong length [%d] of MAC address [%s]", nLen, szBuffer);
            }
            else
            {
                char* cp1 = mac_address;
                char* cp2 = szBuffer + MAC_HEX_DECORATION;
                for (index = 0; index < MAC_LENGTH; index += 2)
                {
                    // m.s. nibble
                    *cp1 = toupper(*cp2);
                    cp1++;
                    cp2++;

                    // l.s. nibble
                    *cp1 = toupper(*cp2);
                    cp1++;
                    cp2++;

                    // skip byte separator
                    cp1++;
                }
            }
        }
        else
        {
            CL_ERRORPRINTF("pclose did not exit normally");
        }
    }

    CL_LOGPRINTF("return [%s]", mac_address);
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
        CL_ERRORPRINTF("Could not read PINCODE on/off");
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
        CL_LOGPRINTF("Read PINCODE onoff- echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        gboolean ok = FALSE;
        if (nProcessRet == -1)
        {
            if (errno == ECHILD)
            {
                // child process has vanished,
                // probably because our on_sigchld() catched the signal
                ok = TRUE;
            }
        }
        else if ( WIFEXITED(nProcessRet)  &&  WEXITSTATUS(nProcessRet) == 0 )
        {
            // process has exited with status ok
            ok = TRUE;
        }
        if (ok)
        {
            int nLen;

            // The string as retrieved by sysset is 1 + 1 (newline) character long.
            CL_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));
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
                CL_ERRORPRINTF("Retrieved pincode on/off [%s] wrong length [%d]", szBuffer, nLen);
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
                    CL_LOGPRINTF("Read PINCODE onoff: %d", *enable);
                }
                else if (strcmp(szBuffer, "1") == 0)
                {
                    *enable = TRUE;
                    nRet = 0;       // ok
                    CL_LOGPRINTF("Read PINCODE onoff: %d", *enable);
                }
                else
                {
                    CL_ERRORPRINTF("Retrieved pincode on/off has illegal value: [%s]", szBuffer);
                }
            }
        }
        else
        {
            CL_ERRORPRINTF("Read PINCODE onoff- pclose did not exit normally");
        }
    }

    CL_LOGPRINTF("Read PINCODE onoff returned %d", nRet);
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
        CL_ERRORPRINTF("Sysset could not read PINCODE");
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
        CL_LOGPRINTF("Read PINCODE echoed %s", szBuffer);
        nProcessRet = pclose(POUT);
        gboolean ok = FALSE;
        if (nProcessRet == -1)
        {
            if (errno == ECHILD)
            {
                // child process has vanished,
                // probably because our on_sigchld() catched the signal
                ok = TRUE;
            }
        }
        else if ( WIFEXITED(nProcessRet)  &&  WEXITSTATUS(nProcessRet) == 0 )
        {
            // process has exited with status ok
            ok = TRUE;
        }
        if (ok)
        {
            // The string as retrieved from sysset is 11(PINCODE_HEX_DECORATION+PINCODE_LENGTH+1) characters long.
            //   e.g 0x85F5E0FF\n
            // "99999999"<--99999999<--"0x05F5E0FF"<--"0x85F5E0FF\n"
            // firstly remove the length of pincode in '0x85F5E0FF\n'
            // then, convert hex string to integer, convert the integer to string 
            //
            CL_LOGPRINTF("Read PINCODE-pclose returned %d", WEXITSTATUS(nProcessRet));
            int nLen = strlen(szBuffer);
            if (nLen != (PINCODE_HEX_DECORATION + PINCODE_LENGTH + 1))
            {
                CL_ERRORPRINTF("Wrong length [%d] of PINCODE address [%s]", nLen, szBuffer);
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
                CL_LOGPRINTF("PINCODE read returned [%s]", pincode);
                nRet = 0;
            }
        }
        else
        {
            CL_ERRORPRINTF("Read PINCODE pclose did not exit normally");
        }
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

gboolean delete_fsitem(const gchar* path)
{
    int    rc;
    char*  cp;
    int    argc;
    char*  argv[10];
    gboolean retVal = FALSE;  // return value

    g_assert(path);

    rc = unlink(path);
    if (rc == 0)
    {
        retVal = TRUE;
    }
    else
    {
        // command = rm -rf <path>
        argc = 0;
        argv[argc++] = "rm";
        argv[argc++] = "-rf";
        //
        cp = alloca( strlen(path) + 1 );
        g_assert(cp != NULL);
        strcpy(cp, path);
        argv[argc++] = cp;
        //
        argv[argc] = NULL;
        g_assert( argc < (sizeof(argv)/sizeof(argv[0])) );
        rc = fork_exec(argc, argv);
        if (rc == 0)
        {
            retVal = TRUE;
        }
        else
        {
            CL_ERRORPRINTF( "Cannot remove path [%s] - error [%d] [%s]",
                             path, rc, strerror(rc) );
        }
    }

    return retVal;
}

