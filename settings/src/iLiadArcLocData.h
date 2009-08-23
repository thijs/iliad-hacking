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

/**
 * \file iLiadArcLocData.h
 * \brief settings - Archive Location Data
 *
 * Providing archive location data functions. 
 * 
 */

#ifndef __ILIAD_ARC_LOC_DATA_H__
#define __ILIAD_ARC_LOC_DATA_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    arcNews = 0,
    arcBooks,
    arcDocs,
    arcNotes,
    arcTypeUndefined
} arcType_e;

typedef enum
{
    arcLocMain = 0,
    arcLocCard,
    arcLocUSB,
    arcLocCF,
    arcLocUndefined
} arcLocType_e;

typedef struct
{
    arcLocType_e newsLoc;
    arcLocType_e booksLoc;
    arcLocType_e docsLoc;
    arcLocType_e notesLoc;
} arcLocData_t;

typedef struct
{
    char* mainPath;
    char* cardPath;
    char* cfPath;
    char* usbPath;
} mediaPath_t;

typedef struct
{
    char* newsPath;
    char* booksPath;
    char* docsPath;
    char* notesPath;
}arcPath_t;

void iLiad_archive_location_data_init(void);

void iLiad_archive_location_data_destroy(void);

void iLiad_archive_location_data_store(void);

void iLiad_archive_location_data_set(arcType_e arc, arcLocType_e location);
void iLiad_export_location_data_set(arcLocType_e location);

arcLocType_e iLiad_archive_location_data_get(arcType_e arc);
arcLocType_e iLiad_export_location_data_get();

#ifdef __cplusplus
}
#endif

#endif                          /* __ILIAD_ARC_LOC_DATA_H__ */
