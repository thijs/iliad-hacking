/**
 * \file erregtypes.h 
 * \brief liberreg -  datatypes definition of registry
 *
 * Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _ER_TYPES_REGISTRY_H_
#define _ER_TYPES_REGISTRY_H_
#include <glib.h>

#define TIME_OPTIONS 3

typedef enum
{
    ids_t = 0,
    pc_t
} connectTo_t;

typedef enum 
{
    specificDocument_t = 0,
    referenceMaterials_t,
    lastReadDocument_t,
    recentDocuments_t,
    deviceManager_t,
    newsFolder_t,
    booksFolder_t,
    docsFolder_t,
    notesFolder_t,
    behaviourUndefined_t
} startup_behaviour_t;

typedef enum
{
    wired_t,
    wireless_t,
    dialup_t,
    connection_undefined_t
} connection_t;

typedef enum
{
    static_t,
    dhcp_t,
    ip_address_mode_undefined_t
} ip_address_mode_t;

typedef enum
{
    encr_none_t = 0,
    encr_wep_t,
    encr_wpa_t,
    encr_undefined_t = 99
} wireless_encr_type_t;

typedef struct
{
    char *registry;
} regVersion_t;

typedef struct
{
    char *name;
    char *email;
    char *password;
    char *redirectUrl;
} regUserProfile_t;

typedef struct
{
    char *language;
    int volume;
    int longkeypressTimeout;
    int keyBuffering;
    gboolean flipbarLeftIsDown; ///< TRUE: left is page down, FALSE: right is page down.
} regUserSetting_t;

typedef struct
{
    char *name; ///< pc profile name
    char *pcname;
    char *sharename;
    char *workgroup;
    char *username;
    char *password;
} regPCProfile_t;

typedef struct
{
    char **pcList;              ///< array of available PC IDs
    int size;                   ///< number of IDs in the PC list
} regPCConfig_t;

typedef struct
{
    gboolean backgroundEnable;  ///< do/don't enable background connect 
    connectTo_t backgroundConnectTo;    ///< target to connect to during background connect: 0 = iDS, 1.. = PCshare
    int backgroundInterval;     ///< interval in minutes background connect
} regAutoConnect_t;

typedef struct
{
    gboolean enable;            ///< do/don't enable timed-iDS connection
    int timeCnt;                ///< number of valid items in timeSet[TIME_OPTIONS], could be 0,1...TIME_OPTIONS
    int timeSet[TIME_OPTIONS];  ///< time settings
    gboolean swUpdate;          ///< do/don't include software update each time connected to iDS
    gboolean switchOff;         ///< do/don't switch off after waking iLiad up
} regTimedIds_t;

typedef struct
{
    char *location;
    gboolean sortAscending;
    char *sortFieldType;        ///< author - date - description - file - fsname - title - none    
} regContentCategory_t;

typedef struct
{
    char *location;
} regExportMemType_t;

typedef struct
{
    char *documentPath;         ///< path to last read item
    char *manifestPath;         ///< path to manifest file
    char *extension;            ///< file extension of path: determines viewer 
    char *position;             ///< where was I in this item? (Could be a pagenumber, a URL, ....) format depends upon viewer
} regLastRead_t;

typedef struct
{
    char **extensionArray;      ///< list of file extensions supported by this viewer 
    gsize extensionArraySize;
    char *xResName;             ///< XWindow resource name
    char *szExec;               ///< executable of the user application
    char *szArgMask;            ///< use <FILENAME> to specify the filename, <MANIFEST> to specify the manifest
    char *szCategory;           ///< viewer, setup, downloadMgr, UserApp are the currently known categories
    int ipcChannel;             ///< IPC channel used by this viewer
    char *appIconPath;
} regUserApp_t;

typedef struct
{
    char **uaIDArray;           ///< list of applications (setup, downloadMgr, viewers...) available on the ereader 
    gsize uaIDArraySize;
} regUserAppList_t;

typedef struct
{
    char *extName;              ///< the extension name e.g "pdf"
    char *associateApp;         ///< the application associated with the extension e.g "ipdf"
    char *iconLocation;         ///< the extension icon
    int  associateOrder;        ///< the order in which to use associated Apps if more than one association for an extension exists
} regExtInfo_t;

typedef struct
{
    regExtInfo_t * extArray;    ///< The pointer of first regExtInfo_t object.
    gsize extArraySize;         ///< The size of the array.
}
regExtInfoList_t;

typedef struct 
{
    startup_behaviour_t behaviour;
    char*               documentPath;    // store the document path want to open immediately after finishing start-up
} regStartUp_t;

typedef struct
{
    char *SSID;
    wireless_encr_type_t encrType;
    char *encrKey;
    gboolean broadcast;
} regWirelessSetting_t;

typedef struct
{
    char* phone;
    char* user;
    char* password;
} regDialupSetting_t;

typedef struct
{
    char *address;
    char *port;
} regProxySetting_t;

typedef struct
{
    char *address;
    char *netmask;
    char *dns;
    char *gateway;
} regIpSetting_t;

typedef struct
{
    unsigned int         timestamp;         ///< timestamp(in microseconds) of the last successful connection
    char                 *name;             ///< network profile name
    connection_t         connection;
    gboolean             proxy;
    ip_address_mode_t    addressMode;
    regWirelessSetting_t *wirelessSettings;///< NULL in case of a wired/dialup connection
    regDialupSetting_t   *dialupSettings;
    regProxySetting_t    *proxySettings;   ///< NULL in case no proxy
    regIpSetting_t       *ipSettings;      ///< NULL in case of dhcp
} regNetworkProfile_t;

typedef struct
{
    char **networkList;         ///< array of available network ID's
    int size;                   ///< number of ID's in the network list
} regNetworkConfig_t;

typedef struct
{
    int profileConnectedIDS;
    int profileConnectedPC;
    connectTo_t  manualConnectType; 
} regLastConnect_t;

typedef struct
{
    char *iconName;
    char **iconPaths;           ///< Currently the first path is the 'normal' icon and the second path is the 'dimmed' icon
    gsize iconPathsSize;
} regIcon_t;

typedef struct
{
    regIcon_t *iconArray;
    int nIcons;
} regIconSet_t;

typedef struct
{
    char *szBuildName;
    char *szBuildNum;
} software_t;

typedef struct
{
    software_t *software;
    char *szMacOfWired;
    char *szMacOfWireless;
    char *szPublicKey;
} deviceDetails_t;

#endif // _ER_TYPES_REGISTRY_H_
