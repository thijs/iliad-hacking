/*
 * This file is part of displayMgr.
 *
 * displayMgr is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * displayMgr is distributed in the hope that it will be useful,
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

#ifdef __arm__
#include <linux/fb.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#else
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#endif

#include <sys/time.h>
#define MILLION 1000000
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "liberdm/erdm.h"
#include "liberdm/erdmServer.h"
#include "liberdm/erdminternal.h"

#include "config.h"
#include "displayEinkTypes.h"
#include "liberdm/display.h"
#include <liberipc/eripc.h>
#include <liberipc/eripcbusyd.h>
#include <liberipc/eripccontentlister.h>

#define N_FILENAME 1024
#define UD_FBSIZE  786432       // 768*1024*2*4/8 // H x V x bit_per_pixel x upscale_factor_to_fit_X / bits_in_a_byte

#define BUFFERSIZE 1024

#define ERDMG_LOGGING_ON 0
#define ERDMG_WARNING_ON 1
#define ERDMG_ERROR_ON 1

#if (ERDMG_LOGGING_ON)
#define ERDMG_LOGPRINTF(x, args...) fprintf(stderr, "(L)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERDMG_LOGPRINTF(x, args...) do {} while (0)
#endif

#if (ERDMG_WARNING_ON)
#define ERDMG_WARNPRINTF(x, args...) fprintf(stderr, "(W)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERDMG_WARNPRINTF(x, args...) do {} while (0)
#endif

#if (ERDMG_ERROR_ON)
#define ERDMG_ERRORPRINTF(x, args...) fprintf(stderr, "(E)" __FILE__ ":%d,%s() " x "\n", __LINE__, __func__ , ##args)
#else
#define ERDMG_ERRORPRINTF(x, args...) do {} while (0)
#endif

#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))

// <WARNING> Keep the following values identical to the values defined in einkfb.h
#define WAVEFORM_SIZE (128*1024)

#define FBIO_IOCTL_BASE       'v'
#define FBIO_DISPLAY          _IOW(FBIO_IOCTL_BASE, 1, struct display_update_info)
#define FBIO_ERASE_WHITE      _IOW(FBIO_IOCTL_BASE, 2, struct display_update_info)
#define FBIO_DRAWPIXELS       _IOW(FBIO_IOCTL_BASE, 3, struct transfer_buffer)
#define FBIO_DRAW_BRUSH       _IOW(FBIO_IOCTL_BASE, 4, struct brush_draw_info)
#define FBIO_REDRAW           _IOW(FBIO_IOCTL_BASE, 5, struct display_update_info)
#define FBIO_UPLOAD_WAVEFORM  _IOC(_IOC_WRITE, FBIO_IOCTL_BASE, 6, WAVEFORM_SIZE)

// This is only here so the existing code will compile. Partial display updates are disabled.
#define FBIO_DISPLAYPARTIAL   0xfff  
// </WARNING>

typedef struct _dmWaveforms
{
    eDmQuality quality;
    unsigned long einkElapsedTime;
} dmWaveforms_t;

// The array can be indexed with the integer corresponding to the eDmQuality
static dmWaveforms_t waveforms[] = {
    {dmQFull,   1 * MILLION },
    {dmQBW,          100000 },
    {dmQTyping, 1 * MILLION }
};

void displayHelp()
{
    printf("Usage: displayMgr -<option> <argument>\n");
    printf("  no arguments : start server process (local access only)\n");
    printf("  -a : allow messages to be sent from any host (starts server process)\n");
    printf("  -h : display this message\n");
    // printf("  -d <filename> : dump framebuffer to filename\n");
    printf("  -p : disable partial update\n");
    printf("  -w <filename> : load waveform from filename\n");
}

int uploadWaveform(int fbDev, char *szFilename)
{
    FILE   *file = NULL;
    unsigned char *buffer = NULL;
    int     ret = 0;

    file = fopen(szFilename, "rb");
    if (!file)
    {
        ERDMG_ERRORPRINTF("Error opening file %s", szFilename);
        ret = -1;
        goto end;
    }

    buffer = (unsigned char *) malloc(WAVEFORM_SIZE);
    if (!buffer)
    {
        ERDMG_ERRORPRINTF("Out of memory");
        ret = -2;
        goto end;
    }

    if (fread(buffer, 1, WAVEFORM_SIZE, file) != WAVEFORM_SIZE)
    {
        ERDMG_ERRORPRINTF("Error reading file");
        ret = -3;
        goto end;
    }

    if ((ret = ioctl(fbDev, FBIO_UPLOAD_WAVEFORM, buffer)))
    {
        ERDMG_ERRORPRINTF("Error uploading waveform (err=0x%x)", ret);
        ret = -4;
        goto end;
    }

  end:
    if (buffer)
        free(buffer);
    if (file)
        fclose(file);
    return ret;
}


int main(int argc, char **argv)
{
    int     fDump = 0;
    int     fLocal = 1;
    int     fDisablePartialUpdate = 0;
    int     fWaveFile = 0;
    int     nArg = 1;
    char    szDumpFile[N_FILENAME];
    char    szWaveFile[N_FILENAME];
    int     nRet;

    // eink var's
    int     ret;
    int     fbDev;

    int     sockfd;
    struct sockaddr_in clientAdr;
    socklen_t len;
    int     n;

    char    buffer[BUFFERSIZE];
    uDmCommand dmCommand;
    unsigned long sleepTimeUSeconds = 0;

    erClientChannel_t contentListerChannel;
    int               first_update = 1;

    // Options follow the pattern
    // -option <argument>
    // -option
    while (nArg < argc)
    {
        if (argv[nArg][0] == '-')
        {
            switch (argv[nArg][1])
            {
            case 'h':
                displayHelp();
                return 0;
                break;

            case 'a':
                fLocal = 0;
                nArg++;
                break;

            case 'd':
                fDump = 1;
                // To what file ?
                if (++nArg >= argc)
                {
                    // Not enough arguments supplied.
                    ERDMG_ERRORPRINTF("Supply a filename after option -d");
                    displayHelp();
                    return -1;
                }
                else
                {
                    strncpy(szDumpFile, argv[nArg], N_FILENAME);
                    nArg++;
                }
                break;

            case 'p':
                fDisablePartialUpdate = 1;
                nArg++;
                break;

            case 'w':
                fWaveFile = 1;
                // To what file ?
                if (++nArg >= argc)
                {
                    // Not enough arguments supplied.
                    ERDMG_ERRORPRINTF("Supply a filename after option -w");
                    displayHelp();
                    return -1;
                }
                else
                {
                    strncpy(szWaveFile, argv[nArg], N_FILENAME);
                    nArg++;
                }
                break;

            default:
                ERDMG_ERRORPRINTF("Option %s not known.", argv[nArg]);
                displayHelp();
                return -1;
            }
        }
        else
        {
            ERDMG_ERRORPRINTF("Argument supplied not proceded by option.");
            displayHelp();
            return -1;
        }
    }


    ERDMG_WARNPRINTF("ePaper daemon (%s)", PACKAGE_STRING);

    if (fDump == 1)
        ERDMG_WARNPRINTF("File dumping set to file %s", szDumpFile);
    if (fLocal == 1)
        ERDMG_WARNPRINTF("Display Manager is accessible to local processes only.");
    if (fWaveFile == 1)
        ERDMG_WARNPRINTF("Loading waveforms from file %s", szWaveFile);

#ifdef __arm__
    fbDev = open("/dev/fb0", O_RDWR);
    if (fbDev == -1)
    {
        ERDMG_ERRORPRINTF("Error opening framebufferdevice. # mknod /dev/fb0 c 29 0");
        return -1;
    }

    // Load waveforms
    if (fWaveFile == 1)
    {
        nRet = uploadWaveform(fbDev, szWaveFile);
        if (nRet != 0)
        {
            ERDMG_ERRORPRINTF("Failed to upload waveform, continuing anyway");
        }
    }
#endif

    initServer(&sockfd, fLocal);    
	// setup the client channel of contentlister 	
	ret = erIpcStartClient(ER_CONTENTLISTER_CHANNEL, &contentListerChannel);
	if (ret != 0)
        ERDMG_ERRORPRINTF("erIpcStartClient returned %d", ret);
    while (1)
    {
        fd_set  readset;
        struct timeval timeout;
        char    szClientIP[INET_ADDRSTRLEN + 1];
        struct timeval tvStart;
        struct timeval tvEnd;
        long    timediff;
        uDmCommand dmCmdDpy;
        uDmCommand dmCmdDpyPartial;
        eDmCommandCode commandCode;
        struct coordinates coordFinal;
        int     nx1, ny1, nx2, ny2;
        int     x1, y1, x2, y2;
        struct display_update_info displayUpdateInfo;
        struct partial_display_update_info partialDisplayUpdateInfo;

        if (sleepTimeUSeconds > 0)
        {
            // We know the eInk display is busy. We just sleep.
            ERDMG_LOGPRINTF("Going to sleep for %ld micro seconds", sleepTimeUSeconds);
            usleep(sleepTimeUSeconds);
            sleepTimeUSeconds = 0;
        }

        // The first call is a blocking call.
        len = sizeof(clientAdr);
        n = recvfrom(sockfd, buffer, BUFFERSIZE, 0, (struct sockaddr *) &clientAdr, &len);
        buffer[n] = '\0';
        if (inet_ntop(AF_INET, &clientAdr.sin_addr, szClientIP, sizeof(szClientIP)))
        {
            ERDMG_LOGPRINTF("Message %s received from %s.", buffer, szClientIP);
        }
        nRet = dmMessageParser(buffer, &dmCommand);
        if (nRet != 0)
        {
            ERDMG_ERRORPRINTF("Skipping an invalid command");
            continue;
        }

        // Blank display before the first display update,
        // this reduces ghosting from startup screen.
        if (first_update)
        {
            first_update = 0;

            ERDMG_LOGPRINTF("Erase to white for first update");
            displayUpdateInfo.waveform = WAVEFORM_4BPP_IMAGE;
            displayUpdateInfo.sequence = 0;
#ifdef __arm__
            if ((ret = ioctl(fbDev, FBIO_ERASE_WHITE, &displayUpdateInfo)))
            {
                ERDMG_ERRORPRINTF("Error sending FBIO_ERASE_WHITE: %x", ret);
            }
#else
            ERDMG_LOGPRINTF("(Dummy) ioctl(fbdev, FBIO_ERASE_WHITE)");
#endif
        }

        commandCode = dmCommand.dmCmdGeneric.cmd;
        switch (commandCode)
        {
        case dmCcDisplay:
            dmCmdDpy = dmCommand;
            break;

        case dmCcDisplayPartial:
            dmCmdDpyPartial = dmCommand;
            break;

        default:
            ;  // ignore
        }

        // Depending on the priority of the request we adapt the timeout that will be
        // used in the select of the while loop.
        switch (dmCommand.dmCmdGeneric.priority)
        {
        case dmCmdPriorLow:
            timediff = 1500 * 1000;
            break;

        case dmCmdPriorHigh:
            timediff = 100 * 1000;
            break;

        case dmCmdPriorUrgent:
            timediff = 0;
            break;

        case dmCmdPriorNormal:
        default:
            timediff = 500 * 1000;
            break;
        }
        //g_assert(timediff < MILLION);
        
        gettimeofday(&tvEnd, NULL);
        tvEnd.tv_usec += timediff;
        if (tvEnd.tv_usec > MILLION)
        {
            tvEnd.tv_usec -= MILLION;
            tvEnd.tv_sec  += 1;
        }

        while (1)  // loop exits with break statement
        {
            FD_ZERO(&readset);
            FD_SET(sockfd, &readset);

            gettimeofday(&tvStart, NULL);
            if (tvEnd.tv_usec < tvStart.tv_usec)
            {
                tvEnd.tv_usec += MILLION;
                tvEnd.tv_sec  -= 1;
            }
            if (tvEnd.tv_sec < tvStart.tv_sec)
            {
                timeout.tv_usec = 0;
                timeout.tv_sec  = 0;
            }
            else
            {
                timeout.tv_usec = tvEnd.tv_usec - tvStart.tv_usec;
                timeout.tv_sec  = tvEnd.tv_sec  - tvStart.tv_sec;
            }
            ERDMG_LOGPRINTF("timeout [%ld:%06ld]", timeout.tv_sec, timeout.tv_usec);
            
            if ((select(sockfd + 1, &readset, NULL, NULL, &timeout) == -1) && (errno != EINTR))
            {
                perror("Select Failed");
            }
            else
            {
                if (FD_ISSET(sockfd, &readset))
                {
                    len = sizeof(clientAdr);
                    n = recvfrom(sockfd, buffer, BUFFERSIZE, 0, (struct sockaddr *) &clientAdr, &len);
                    buffer[n] = '\0';
                    if (inet_ntop(AF_INET, &clientAdr.sin_addr, szClientIP, sizeof(szClientIP)))
                    {
                        ERDMG_LOGPRINTF("Message %s received from %s.", buffer, szClientIP);
                    }
                    nRet = dmMessageParser(buffer, &dmCommand);
                    if (nRet != 0)
                    {
                        ERDMG_ERRORPRINTF("Skipping an invalid command");
                        continue;
                    }
                    // If we already plan to do a full display update, ignore the partial display update.
                    if (commandCode == dmCcDisplay)
                    {
                        // Update the priority
                        //   A higher value corresponds to a higher priority
                        if ((int) dmCmdDpy.dmCmdDisplay.priority < (int) dmCommand.dmCmdDisplay.priority)
                        {
                            dmCmdDpy.dmCmdDisplay.priority = dmCommand.dmCmdDisplay.priority;
                        }
                        // Update the quality
                        //   A higher value corresponds to a faster waveform (slowest waveforms should take precedence)
                        if ((int) dmCmdDpy.dmCmdDisplay.qual > (int) dmCommand.dmCmdDisplay.qual)
                        {
                            dmCmdDpy.dmCmdDisplay.qual = dmCommand.dmCmdDisplay.qual;
                        }
                    }
                    else if (commandCode == dmCcDisplayPartial)
                    {
                        if (dmCommand.dmCmdGeneric.cmd == dmCcDisplay)
                        {
                            // A full screen update supersedes a partial screen update.
                            commandCode = dmCcDisplay;
                            dmCmdDpy = dmCommand;
                        }
                        else if (dmCommand.dmCmdGeneric.cmd == dmCcDisplayPartial)
                        {
                            // Merge the two rectangles.
                            int     x1Up, y1Up;
                            int     x1Down, y1Down;
                            int     x2Up, y2Up;
                            int     x2Down, y2Down;

                            x1Down = dmCmdDpyPartial.dmCmdDisplayPartial.xDown;
                            y1Down = dmCmdDpyPartial.dmCmdDisplayPartial.yDown;
                            x1Up = dmCmdDpyPartial.dmCmdDisplayPartial.xUp;
                            y1Up = dmCmdDpyPartial.dmCmdDisplayPartial.yUp;

                            x1Up = min(x1Up, x1Down);
                            y1Up = min(y1Up, y1Down);
                            x1Down = max(x1Up, x1Down);
                            y1Down = max(y1Up, y1Down);

                            x2Down = dmCommand.dmCmdDisplayPartial.xDown;
                            y2Down = dmCommand.dmCmdDisplayPartial.yDown;
                            x2Up = dmCommand.dmCmdDisplayPartial.xUp;
                            y2Up = dmCommand.dmCmdDisplayPartial.yUp;

                            x2Up = min(x2Up, x2Down);
                            y2Up = min(y2Up, y2Down);
                            x2Down = max(x2Up, x2Down);
                            y2Down = max(y2Up, y2Down);

                            // The actual merge (TODO: determine qual)
                            commandCode = dmCcDisplayPartial;
                            dmCmdDpyPartial.dmCmdDisplay.cmd = commandCode;
                            dmCmdDpyPartial.dmCmdDisplay.priority = dmCmdPriorNormal;
                            dmCmdDpyPartial.dmCmdDisplay.qual = dmQFull;
                            dmCmdDpyPartial.dmCmdDisplayPartial.xUp = min(x1Up, x2Up);
                            dmCmdDpyPartial.dmCmdDisplayPartial.yUp = min(y1Up, y2Up);
                            dmCmdDpyPartial.dmCmdDisplayPartial.xDown = max(x1Down, x2Down);
                            dmCmdDpyPartial.dmCmdDisplayPartial.yDown = max(y1Down, y2Down);
                        }
                    }
                    else
                    {
                        commandCode = dmCommand.dmCmdGeneric.cmd;
                        switch (commandCode)
                        {
                        case dmCcDisplay:
                            dmCmdDpy = dmCommand;
                            break;

                        case dmCcDisplayPartial:
                            dmCmdDpyPartial = dmCommand;
                            break;
                        default:
                            ;  // ignore
                        }
                    }
                }
                else
                {
                    ERDMG_LOGPRINTF("Nothing more pending.");
                    break;  // exit while (1)
                }
            }
        }

        switch (commandCode)
        {
        case dmCcDisplay:
            dmCommand = dmCmdDpy;
            break;

        case dmCcDisplayPartial:
            dmCommand = dmCmdDpyPartial;
            break;

        default:
            ;  // ignore
        }

        switch (dmCommand.dmCmdGeneric.cmd)
        {
        case dmCcDisplay:
            displayUpdateInfo.sequence = 0;
            switch (dmCommand.dmCmdDisplay.qual)
            {
              case dmQFull:
                displayUpdateInfo.waveform = WAVEFORM_4BPP_IMAGE;
                break;
              case dmQBW:
                displayUpdateInfo.waveform = WAVEFORM_FAST_BLACK_WHITE;
                ERDMG_LOGPRINTF("Selected BW waveform");
                break;
              case dmQTyping:
                displayUpdateInfo.waveform = WAVEFORM_TYPING;
                ERDMG_LOGPRINTF("Selected Typing waveform");
                break;
              default:
                ;  // ignore
            }
            ERDMG_LOGPRINTF("Before ioctl");
            gettimeofday(&tvStart, NULL);
#ifdef __arm__
            // if ((ret = ioctl(fbDev, FBIO_DISPLAY, NULL)))
            if ((ret = ioctl(fbDev, FBIO_DISPLAY, &displayUpdateInfo)))
            {
                ERDMG_ERRORPRINTF("Error sending FBIO_DISPLAY: %x", ret);
            }
#else
            ERDMG_LOGPRINTF("(Dummy) ioctl(fbDev, FBIO_DISPLAY, NULL)");
#endif
            gettimeofday(&tvEnd, NULL);
            timediff = MILLION * (tvEnd.tv_sec - tvStart.tv_sec) + (tvEnd.tv_usec - tvStart.tv_usec);
            ERDMG_LOGPRINTF("ioctl executed for %ld usec", timediff);
            sleepTimeUSeconds = waveforms[(int) dmCommand.dmCmdDisplay.qual].einkElapsedTime;
			// send "clCcDisplayUpdated" signal to contentlister
            clDisplayUpdated(contentListerChannel);			
            break;

        case dmCcDisplayPartial:
            // Transform coordinates
/* 'coord' should be the native coords, while gtk is rotated
 * gtk.x = 768 - native.y
 * gtk.y = native.x
 *
 * native.x = gtk.y
 * native.y = 768 - gtk.x
 */
            if (fDisablePartialUpdate == 1)
            {
                ERDMG_LOGPRINTF("Before ioctl (PARTIAL UPDATE DISABLED: FULL UPDATE INSTEAD)");
                displayUpdateInfo.sequence = 0;
                displayUpdateInfo.waveform = WAVEFORM_4BPP_IMAGE;
                gettimeofday(&tvStart, NULL);
#ifdef __arm__
                // if ((ret = ioctl(fbDev, FBIO_DISPLAY, NULL)))
                if ((ret = ioctl(fbDev, FBIO_DISPLAY, &displayUpdateInfo)))
                {
                    ERDMG_ERRORPRINTF("Error sending FBIO_DISPLAY: %x", ret);
                }
#else
                ERDMG_LOGPRINTF("(Dummy) ioctl(fbDev, FBIO_DISPLAY, NULL)");
#endif
                gettimeofday(&tvEnd, NULL);
                timediff = MILLION * (tvEnd.tv_sec - tvStart.tv_sec) + (tvEnd.tv_usec - tvStart.tv_usec);
                ERDMG_LOGPRINTF("ioctl executed for %ld usec", timediff);
                sleepTimeUSeconds = waveforms[(int) dmCommand.dmCmdDisplay.qual].einkElapsedTime;
            }
            else
            {
                partialDisplayUpdateInfo.sequence = 0;
                partialDisplayUpdateInfo.waveform = WAVEFORM_4BPP_IMAGE;

                nx1 = dmCommand.dmCmdDisplayPartial.yUp;
                ny1 = SCREEN_WIDTH - dmCommand.dmCmdDisplayPartial.xUp;
                nx2 = dmCommand.dmCmdDisplayPartial.yDown;
                ny2 = SCREEN_WIDTH - dmCommand.dmCmdDisplayPartial.xDown;

                // Have the rectangle properly defined. 
                // (x1,y1) = upper left in native coordinates
                // (x2,y2) = lower right in native coordinates
                x1 = min(nx1, nx2);
                y1 = min(ny1, ny2);
                x2 = max(nx1, nx2);
                y2 = max(ny1, ny2);

                // Make everything a multiple of four. The rectangle should not become smaller. (Except if it is adjacent to border)
                // Avoid partial redrawing adjacent to border (don't know whether we need this..)
                x2 += 4;
                y2 += 4;

                x1 = (x1 < 4) ? 4 : (x1 - (x1 % 4));
                y1 = (y1 < 4) ? 4 : (y1 - (y1 % 4));
                x2 = (x2 > (SCREEN_HEIGHT - 4)) ? (SCREEN_HEIGHT - 4) : (x2 - (x2 % 4));
                y2 = (y2 > (SCREEN_WIDTH - 4)) ? (SCREEN_WIDTH - 4) : (y2 - (y2 % 4));

                // Here the type becomes unsigned.
                coordFinal.x1 = x1;
                coordFinal.y1 = y1;
                coordFinal.x2 = x2;
                coordFinal.y2 = y2;

                partialDisplayUpdateInfo.coord = coordFinal;

                ERDMG_LOGPRINTF("Before ioctl. PartialUpdate((%d,%d):(%d,%d))", coordFinal.x1, coordFinal.y1, coordFinal.x2, coordFinal.y2);
                gettimeofday(&tvStart, NULL);
#ifdef __arm__
                // if ((ret = ioctl(fbDev, FBIO_DISPLAYPARTIAL, &coordFinal)))
                if ((ret = ioctl(fbDev, FBIO_DISPLAYPARTIAL, &partialDisplayUpdateInfo)))
                {
                    ERDMG_ERRORPRINTF("Error sending FBIO_DISPLAYPARTIAL: %x", ret);
                }
#else
                ERDMG_LOGPRINTF("(Dummy) ioctl(fbdev, FBIO_DISPLAYPARTIAL, &coordFinal)");
#endif
                gettimeofday(&tvEnd, NULL);
                timediff = MILLION * (tvEnd.tv_sec - tvStart.tv_sec) + (tvEnd.tv_usec - tvStart.tv_usec);
                ERDMG_LOGPRINTF("ioctl executed for %ld usec", timediff);
                sleepTimeUSeconds = waveforms[(int) dmCommand.dmCmdDisplay.qual].einkElapsedTime;
            }
            break;

        case dmCcDump:
            ERDMG_ERRORPRINTF("%d not implemented.", (int) dmCommand.dmCmdGeneric.cmd);
            sleepTimeUSeconds = 0;
            break;

        case dmCcEraseToWhite:
            displayUpdateInfo.waveform = WAVEFORM_4BPP_IMAGE;
            displayUpdateInfo.sequence = 0;
            ERDMG_LOGPRINTF("erase to white");
#ifdef __arm__
            if ((ret = ioctl(fbDev, FBIO_ERASE_WHITE, &displayUpdateInfo)))
            {
                ERDMG_ERRORPRINTF("Error sending FBIO_ERASE_WHITE: %x", ret);
            }
#else
            ERDMG_LOGPRINTF("(Dummy) ioctl(fbdev, FBIO_ERASE_WHITE)");
#endif
            sleepTimeUSeconds = 0;
            break;

        default:
            ;  // ignore
        }
    }

    return 0;
}
