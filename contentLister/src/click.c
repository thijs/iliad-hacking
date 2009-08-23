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
 * \file click.c
 * \brief content lister - eReader key/pen click handling   
 *
 * Generate key/pen clicks on request
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2006-2008  iRex Technologies B.V.
 * All rights reserved.
 */

#include <config.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

// UGLY HACK, this should be in linux/soundcard.h
#ifndef SNDCTL_POWER_OFF
#define SNDCTL_POWER_OFF       _SIO('P', 99)
#define SNDCTL_POWER_ON        _SIO('P', 100)
#endif

#include <glib.h>

#include <liberregxml/erregapi.h>

#include "contentListerLog.h"
#include "click.h"

#define MIXER               "/dev/mixer"

// global data
static int g_volume_settings = 0;

static wavFile_t g_wav_files[playUndefined] = 
{
    {FILENAME_KEY_CLICK, 0, NULL},
    {FILENAME_KEY_CLICK_DISCARDED, 0, NULL},
    {FILENAME_PEN_CLICK, 0, NULL}
};

static pthread_mutex_t g_click_mutex;

static playSoundType_e g_play_sound;    // indicate what sound type played

// local functions
static void *click_thread (void *arg);

// Destroy all waveforms
static void destroy_waveforms()
{
    int i;
    for (i=0; i<playUndefined; i++)
    {
        if (g_wav_files[i].wav_file_size)
        {
            g_free(g_wav_files[i].waveform);
            g_wav_files[i].waveform = NULL;
            g_wav_files[i].wav_file_size = 0;
        }
    }
}

// Read waveforms for key click, pen click, and key-click-discarded
static gboolean read_waveforms()
{
    int i, n, fd;
    for (i=0; i<playUndefined; i++)
    {
        fd = open(g_wav_files[i].wav_file_path, O_RDONLY);
        if (fd <= 0)
        {
            CL_ERRORPRINTF ("Open error [%d] file [%s]", errno, g_wav_files[i].wav_file_path);
            goto Error;
        }

        g_wav_files[i].wav_file_size = lseek(fd, 0, SEEK_END);
        g_wav_files[i].waveform = g_new(char, g_wav_files[i].wav_file_size);
        if (NULL == g_wav_files[i].waveform)
        {
            CL_ERRORPRINTF("Error allocating memory for waveforms.");
            close(fd);
            goto Error;
        }

        lseek (fd, 0, SEEK_SET);
        n = read (fd, g_wav_files[i].waveform, g_wav_files[i].wav_file_size);
        if (n != g_wav_files[i].wav_file_size)
        {
            CL_ERRORPRINTF("Read error [%d] file [%s]", errno, g_wav_files[i].wav_file_path);
            close(fd);
            goto Error;
        }

        // Successfully read g_wav_files[i]
        close(fd);
    }

    return TRUE;

Error:
    destroy_waveforms();
    return FALSE;
}

static void enable_audio_device(gboolean bEnable)
{
    int audio_fd = open(AUDIO_DEVICE, O_RDWR);
    g_assert(audio_fd >= 0);
    int result = ioctl(audio_fd, bEnable ? SNDCTL_POWER_ON : SNDCTL_POWER_OFF, 0);
    g_assert (result != -1);
    close(audio_fd);
}

static void set_volume(int volume)
{
    int mixer_fd = open (MIXER, O_RDWR);
    g_assert (mixer_fd >= 0);
    volume = (volume << 8) | volume;   // both channels equally loud
    int result = ioctl (mixer_fd, SOUND_MIXER_WRITE_VOLUME, &volume);
    g_assert (result != -1);
    close (mixer_fd);
}

// initialise: read click waveform, start click-thread
// return TRUE when ok
gboolean click_init ()
{
    int err;
    pthread_t click_tid;
    regUserSetting_t *theUserSetting = NULL;
    
    // Retrieve user setting from registry
    // Setup the mixer volumes according to the users wishes
    theUserSetting = erRegGetUserSetting();
    if (theUserSetting)
    {
        g_volume_settings = theUserSetting->volume;
        erRegFreeUserSetting (theUserSetting);
    }
    else
    {
        // default is on
        g_volume_settings = 75;
    }

    // Turn on/off the audio device according to the settings
    enable_audio_device(g_volume_settings>0 ? TRUE : FALSE);

    if (g_volume_settings > 0)
    {
        // The sound is on, set the volume
        set_volume(g_volume_settings);
    }

    // Second, read the waveforms
    if (!read_waveforms())
    {
        CL_ERRORPRINTF("Error reading waveforms.");
        return FALSE;
    }

    // Initialize mutex
    err = pthread_mutex_init (&g_click_mutex, NULL);
    if (err != 0)
    {
        CL_ERRORPRINTF ("Mutex init error [%d]", err);
        destroy_waveforms();
        return FALSE;
    }

    // start click-thread
    err = pthread_create (&click_tid, NULL, click_thread, NULL);
    if (err != 0)
    {
        CL_ERRORPRINTF ("start click-thread error [%d]", err);
        destroy_waveforms();
        return FALSE;
    }

    // get here, success
    return TRUE;
}

void handle_sound_settings_changed(int volume)
{
    g_volume_settings = volume;
    
    // Turn on/off the audio device according to the settings
    enable_audio_device(g_volume_settings>0 ? TRUE : FALSE);

    if (g_volume_settings > 0)
    {
        // The sound is on, set the volume
        set_volume(g_volume_settings);
    }
}

// play sound,  e.g click sound or click discard sound
static void play_sound(playSoundType_e sound)
{
    CL_LOGPRINTF("click: play %d sound",  g_play_sound);

    if (0 == g_volume_settings)
    {
        // The sound is disabled, do nothing
        return;
    }

    if ( (sound >= playClickedKey) && (sound < playUndefined) )
    {
        g_play_sound = sound;
    }
    else
    {
        g_play_sound = playClickedKey;
    }
   
    pthread_mutex_unlock (&g_click_mutex); 
}

void click_key()
{
    play_sound(playClickedKey);
}

void click_key_discard()
{
    play_sound(playClickedKeyDiscarded);
}

void click_pen()
{
    play_sound(playClickedPen);
}

static void *click_thread (void *arg)
{
    int fd;   
 
    pthread_mutex_lock (&g_click_mutex);
    while (1)
    {
        char* waveform = NULL;
        int size=0;
       
        // wait for mutex
        pthread_mutex_lock (&g_click_mutex);

        // click once
        waveform = g_wav_files[g_play_sound].waveform;
        size     = g_wav_files[g_play_sound].wav_file_size;

        if (waveform != NULL && size != 0)
        {
            fd = open (AUDIO_DEVICE, O_WRONLY);
            if (fd <= 0)
            {
                CL_ERRORPRINTF("Open error [%d] device [%s]", errno, AUDIO_DEVICE);
            }
            else
            {
                write (fd, waveform, size);
                close (fd);
            }
        }    
    }
}

