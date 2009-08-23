/**
 * \file erregapi.h 
 * \brief liberreg - erregxml APIs header file
 *                   applications which calls library erregxmlshould include this header file                   
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _ERREG_API__H__
#define _ERREG_API__H__

// notes: copied from erregxml.h
#define BOOKS_CATEGORY             "BOOKS"         ///< the ID of books archive
#define NEWSPAPERS_CATEGORY        "NEWSPAPERS"    ///< the ID of news archive
#define NOTES_CATEGORY             "NOTES"         ///< the ID of notes archive
#define DOCUMENTS_CATEGORY         "DOCUMENTS"     ///< the ID of documents archive
#define CF_CATEGORY                "CF"            ///< the ID of CF card
#define MAIN_CATEGORY              "MAIN"          ///< the ID of main memory
#define SD_CATEGORY                "SDMMC"         ///< the ID of cards(SD or MMC) 
#define USB_CATEGORY               "USB"           ///< the ID of USB
#define MODE_CATEGORY              "MODE"          ///< the ID of mode(device manager)
#define DOWNLOAD_HISTORY           "DOWNLOADHISTORY" ///< the ID of download histroy
#define SEARCH_RESULT              "SEARCHRESULT"    ///< the ID of search result
#define RECENTDOCUMENTS            "RECENTDOCUMENTS" ///< the ID of recent documents

#define BROWSER_APP                "BROWSER"      ///< the ID of browser(minimo)
#define IPDF_APP                   "IPDF"         ///< the ID of ipdf viewer
#define EBA_APP                    "EBA"          ///< the ID of eba viewer
#define SETUP_APP                  "SETUP"        ///< the ID of setup(settings)   
#define PROFILES_APP               "PROFILES"     ///< the ID of connectionMgr
#define APABIMOBI_APP              "APABIMOBI"    ///< the ID of apabiMobi viewer
#define IMAGEVIEWSER_APP           "IMAGEVIEWER"  ///< the ID of image viewer(scribble)
#define DOWNLOADMGR_APP            "DOWNLOADMGR"  ///< the ID of downloadMgr

#define AUTHOR                     "AUTHOR"       ///< sort field string - author
#define FIELDFILE                  "FILE"         ///< sort field string - file
#define FSNAME                     "FSNAME"       ///< sort field string - file name
#define EXTENSION                  "EXTENSION"    ///< sort field string - extension name
#define SIZE                       "SIZE"         ///< sort field string - content size 
#define DATE                       "DATE"         ///< sort field string - date 
#define TITLE                      "TITLE"        ///< sort field string - title
#define DESCRIPTION                "DESCRIPTION"  ///< sort field string - decription

#include "erregrwlock.h"
#include "erregtypes.h"
#include "erreg.h"
#include "erregcfg.h"

#endif                          //_ERREG_API__H__
