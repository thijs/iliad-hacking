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

//
//
// WARNING : keep these types in sync with einkfb.h!
//
//
#ifndef DISPLAY_EINK_TYPES_H
#define DISPLAY_EINK_TYPES_H

#define WAVEFORM_4BPP_IMAGE       1
#define WAVEFORM_DRAW             4
#define WAVEFORM_FAST_BLACK_WHITE 6
#define WAVEFORM_TYPING           9

struct coordinates {
	unsigned short x1;
	unsigned short y1;
	unsigned short x2;
	unsigned short y2;
};

struct pixel {
	unsigned short x;
	unsigned short y;
	unsigned int   size;
	unsigned int   color;
};

struct pixelInfo {
	unsigned int count;
	struct pixel* pixels;
};

struct display_update_info {
    int waveform;
    int sequence;
};
 
struct partial_display_update_info {
    struct coordinates coord;
    int waveform;
    int sequence;
};

#endif // DISPLAY_EINK_TYPES_H
