/*
 * This file is part of liberregxml.
 *
 * liberregxml is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * liberregxml is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Copyright (C) 2007 iRex Technologies B.V.
 * All rights reserved.
 */

#include <glib.h>
#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

#include "erreglog.h"
#include "system.h"

#define MAC_LENGTH              12
#define MAC_HEX_DECORATION      2

#define PROCESS_BUFFER_MAX      (2*1024)

const char *get_sysset_macAddress(void)
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    static char mac_address[MAC_LENGTH + 6] = "00-00-00-00-00-00";

    POUT = popen("sysset -r -b -a 262 -l 6", "r");
    if (POUT == NULL)
    {
        ERREG_ERRORPRINTF("Could not read EUI64");
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
        ERREG_LOGPRINTF("echoed %s", szBuffer);

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
                ERREG_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));
                nLen = strlen(szBuffer);
                if (nLen != MAC_LENGTH + 2 + 1)
                {
                    ERREG_ERRORPRINTF("Wrong length [%d] of MAC address [%s]", nLen, szBuffer);
                }
                else
                {
                    char *cp1 = mac_address;
                    char *cp2 = szBuffer + MAC_HEX_DECORATION;
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
                ERREG_ERRORPRINTF("pclose returned %d. Error retrieving MAC", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ERREG_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ERREG_LOGPRINTF("return [%s]", mac_address);
    return mac_address;
}

const char *get_wired_macAddress(void)
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    static char mac_address[MAC_LENGTH + 6] = "00-00-00-00-00-00";


    POUT = popen("/usr/bin/wired.sh mac | tail -1", "r");
    if (POUT == NULL)
    {
        ERREG_ERRORPRINTF("Could not read wired MAC address");
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
        ERREG_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                int nLen;

                // Normal case
                //
                // The string as retrieved from wirede.sh 17 (MAC_LENGTH+5+1) characters long.
                //   e.g 00-16-7c-00-00-00\n
                ERREG_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));
                nLen = strlen(szBuffer);
                if (nLen != MAC_LENGTH + 5 + 1)
                {
                    ERREG_ERRORPRINTF("Wrong length [%d] of MAC address [%s]", nLen, szBuffer);
                }
                else
                {
                    strncpy(mac_address, szBuffer, MAC_LENGTH + 5);
                }
            }
            else
            {
                ERREG_ERRORPRINTF("pclose returned %d. Error retrieving MAC", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ERREG_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ERREG_LOGPRINTF("return [%s]", mac_address);
    return mac_address;
}

// TODO
const char *get_wireless_macAddress(void)
{
    FILE *POUT = 0;
    int nProcessRet = 0;

    static char mac_address[MAC_LENGTH + 6] = "00-00-00-00-00-00";


    POUT = popen("/usr/bin/wireless.sh mac | tail -1", "r");
    if (POUT == NULL)
    {
        ERREG_ERRORPRINTF("Could not read wireless MAC address");
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
        ERREG_LOGPRINTF("echoed %s", szBuffer);

        nProcessRet = pclose(POUT);
        if (WIFEXITED(nProcessRet))
        {
            if (WEXITSTATUS(nProcessRet) == 0)
            {
                int nLen;

                // Normal case
                //
                // The string as retrieved from wireless.sh 17 (MAC_LENGTH+5+1) characters long.
                //   e.g 00-16-7c-00-00-00\n
                ERREG_LOGPRINTF("pclose returned %d", WEXITSTATUS(nProcessRet));
                nLen = strlen(szBuffer);
                if (nLen != MAC_LENGTH + 5 + 1)
                {
                    ERREG_ERRORPRINTF("Wrong length [%d] of MAC address [%s]", nLen, szBuffer);
                }
                else
                {
                    strncpy(mac_address, szBuffer, MAC_LENGTH + 5);
                }
            }
            else
            {
                ERREG_ERRORPRINTF("pclose returned %d. Error retrieving MAC", WEXITSTATUS(nProcessRet));
            }
        }
        else
        {
            ERREG_ERRORPRINTF("pclose did not exit normally");
        }
    }

    ERREG_LOGPRINTF("return [%s]", mac_address);
    return mac_address;
}

#define WLAN_MAC_FILE "/mnt/free/wlan_mac.txt"

const char *get_wlanmactxt_macAddress(void)
{
    static char mac_address[MAC_LENGTH + 6] = "00-00-00-00-00-00";
 
    FILE *fd = fopen(WLAN_MAC_FILE, "r");
    if (fd)
    {
        // read the content of file into buffer
        char* tmp = NULL;
        fseek(fd, 0, SEEK_END);
        int len = ftell(fd);
       
        tmp = malloc(len+1);
        if (tmp)
        {
            fseek(fd, 0, SEEK_SET);
            fread(tmp, sizeof(char), len, fd);
            tmp[len] = '\0';

            // strip out the first line, the remained part is mac address
            char *new_line = strchr(tmp, '\n');
            if (new_line)
            {
                snprintf(mac_address, sizeof(mac_address), "%s", new_line+1);
            }
        
            free(tmp);
        }

        fclose(fd);

        ERREG_LOGPRINTF("software mac_address [%s]", mac_address);
    }
    
    return mac_address;
}

#define VERSION_FILE "/etc/er_version"
#define VERSION_COMMERCIAL_FILE "/etc/er_version_commercial"

#define VERSION_LENGTH 40

const char *get_software_version(void)
{
    static char version[VERSION_LENGTH] = "";

    FILE *fd = fopen(VERSION_FILE, "r");
    if (fd)
    {
        fgets(version, sizeof(version)-1, fd);
        fclose(fd);

        char *new_line = strchr(version, '\n');
        if (new_line)
        {
            *new_line = '\0';
        }
    
        ERREG_LOGPRINTF("software version [%s]", version);
    }

    return version;
}

const char *get_software_version_commercial(void)
{
    static char version[VERSION_LENGTH] = "";

    FILE *fd = fopen(VERSION_COMMERCIAL_FILE, "r");
    if (fd)
    {
        fgets(version, sizeof(version)-1, fd);
        fclose(fd);

        char *new_line = strchr(version, '\n');
        if (new_line)
        {
            *new_line = '\0';
        }
    
        ERREG_LOGPRINTF("software version [%s]", version);
    }

    return version;
}

#define PUBLIC_KEY_FILE "/proc/keystore/public"

#define MAX_PUBKEY_SIZE     (1024)

static inline int is_pubkey_char(unsigned char c)
{
      return (isalnum(c) || (c == '+') || (c == '/') || (c == '-') || (c == '\n') || (c == ' '));
}

int get_public_key(char **pubkey)
{
    int bytes = 0;
    int i;
    FILE *f = fopen(PUBLIC_KEY_FILE, "r");
    if (f == NULL) 
    {
        return -1; // -1 means file opening failed
    }

    *pubkey = (char *) malloc(MAX_PUBKEY_SIZE);
    if (*pubkey == NULL)
    {
        ERREG_ERRORPRINTF("Error: cannot allocate %d bytes for public key", MAX_PUBKEY_SIZE);
        return 1;
    }

    if ((bytes = fread(*pubkey, 1, MAX_PUBKEY_SIZE, f)) == MAX_PUBKEY_SIZE)
    {
        ERREG_ERRORPRINTF("Error reading Public Key (too big)");
        free(*pubkey);
        *pubkey = NULL;
        return -2;
    }

    if (!feof(f))
    {
        ERREG_ERRORPRINTF("Error reading Public Key (not EOF)");
        free(*pubkey);
        *pubkey = NULL;
        return -3;
    }
    
    (*pubkey)[bytes] = '\0';
    
    // Test to see if this looks like a public key
    for (i = 0; i < bytes; i++)
    {
        if (!is_pubkey_char((*pubkey)[i]))
        {
            ERREG_ERRORPRINTF("Error: this does not look like a public key!");
            free(*pubkey);
            *pubkey = NULL;
            return -5;
        }
    }

    ERREG_LOGPRINTF("software publicKey [%s]", *pubkey);
    
    fclose(f);
    return bytes;
}

/* discard this function, the reason is shown as below
char* get_public_key(void)
{
    char *publicKey = NULL;

    FILE *fd = fopen(PUBLIC_KEY_FILE, "r");
    if (fd)
    {
        fseek(fd, 0, SEEK_END);
        int len = ftell(fd);// getting the length by this method is not correct! The length is always 37.
        
        publicKey = malloc(len+1);
        if (publicKey)
        {
            fseek(fd, 0, SEEK_SET);
            fread(publicKey, sizeof(char), len, fd);
        }
        publicKey[len] = '\0';

        fclose(fd);
        
        ERREG_LOGPRINTF("software publicKey [%s]", publicKey);
    }

    return publicKey;
}
*/
