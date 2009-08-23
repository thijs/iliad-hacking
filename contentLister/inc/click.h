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
 * \file click.h
 * \brief content lister - eReader key-click handling                  
 * 
 * Generate keys-click on request
 */

/* contentLister - A GTK based content lister (read only) application on the eReader platform
 * Copyright (C) 2006-2008  iRex Technologies B.V.
 */

 
#ifndef __CL_CLICK__H__
#define __CL_CLICK__H__


#define AUDIO_DEVICE            "/dev/dsp"

#define FILENAME_KEY_CLICK      (DATA_DIR "/clicked_key.wav")
#define FILENAME_KEY_CLICK_DISCARDED      (DATA_DIR "/clicked_key_discarded.wav")
#define FILENAME_PEN_CLICK      (DATA_DIR "/clicked_pen.wav")

typedef enum
{
    playClickedKey = 0,
    playClickedKeyDiscarded,
    playClickedPen,
//  playClickedPenDiscarded, 
    playUndefined
} playSoundType_e;

typedef struct
{
    const char* wav_file_path;
    int         wav_file_size;
    char*       waveform;
} wavFile_t;

gboolean click_init();
void handle_sound_settings_changed(int volume);

/**
 * play click key sound 
 */
void click_key();

/**
 * play click key discarded sound 
 */
void click_key_discard();

/**
 * play click pen sound 
 */
void click_pen();

#endif  //__CL_CLICK__H__
