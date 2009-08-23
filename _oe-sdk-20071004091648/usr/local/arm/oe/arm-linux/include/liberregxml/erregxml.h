/**
 *\file 
 *\brief liberreg -  define elements and attributes etc. in registry.xml
 *                          define some xpath macro
 *                          typedef struct regRegistry_t
 *                          function declarations of geting & setting
 *
 *Copyright (C) 2007 iRex Technologies BV.
 *
 */
#ifndef _ER_REG_XML_H_
#define _ER_REG_XML_H_

#include "erreglog.h"

#include <libermanifest/ermanifest.h>

#include "erregtypes.h"
#include "erregxsd.h"

//////////// composite elements's names used in the registry file ////////////
#define EL_REGISTRY_ROOT          "iLiadRegistry"

#define EL_VERSION                "version"
#define EL_USER_PROFILE           "userProfile"
#define EL_USER_SETTING           "userSetting"

#define EL_PC_PROFILES            "pcProfileList"
#define EL_PC_PROFILE             "pcProfile"

#define EL_AUTO_CONNECT           "autoConnect"
#define EL_BACKGOUND              "background"
#define EL_TIMED_IDS              "timedIds"
#define EL_ENABLE_TIMEDIDS        "enable"
#define EL_TIMESET                "timeSet"
#define EL_TIME                   "time"
#define EL_ENABLE_SWUPDATE        "swUpdate"
#define EL_ENABLE_SWITCHOFF       "switchOff"

#define EL_CATEGORIES             "categories"
#define EL_CATEGORY               "category"
#define EL_SORT                   "sort"

#define EL_LASTREADS              "lastReads"
#define EL_LASTREAD               "lastRead"

#define EL_APPLICATIONS           "applications"
#define EL_APPLICATION            "application"
#define EL_SUPPORT_FORMAT         "supportFormat"
#define EL_ARG_MASK               "argMask"

#define EL_EXTENSION_INFOS        "extensionInfos"
#define EL_EXTENSION_INFO         "extensionInfo"

#define EL_START_UP               "startup"

#define EL_NETWORK_PROFILES       "networkProfileList"
#define EL_NETWORK_PROFILE        "networkProfile"
#define EL_WLAN_DETAILS           "wlanDetails"
#define EL_DIALUP_DETAILS         "dialupDetails"
#define EL_IP_STATIC_DETAILS      "ipStaticDetails"
#define EL_PROXY_DETAILS          "proxyDetails"
#define EL_LAST_CONNECT           "lastConnect" 

#define EL_TOOLBAR_ICONS          "toolbarIcons"
#define EL_TOOLBAR_ICON           "toolbarIcon"

//////////////////////////// leaf elements names ////////////////////////////
#define EL_REGISTRY               "registry"

#define EL_NAME                   "name"
#define EL_EMAIL                  "email"
#define EL_PASSWORD               "password"
#define EL_REDIRECT_URL           "redirectUrl"

#define EL_LANGUAGE               "language"

#define EL_SOUND                  "sound"
#define EL_ENABLE                 "enable"
#define EL_VOLUME                 "volume"

#define EL_KEY_BUFFERING          "keyBuffering"
#define EL_NUMBER_OF_KEYS         "numberOfKeys"

#define EL_FLIPBAR_DIRECTION      "flipbarDirection"
#define EL_LONG_KP_TIMEOUT        "longKeyPressTimeout"

#define EL_PROFILE_NAME           "profileName"

#define EL_PC_NAME                "pcName"
#define EL_SHARE_NAME             "shareName"
#define EL_USER_NAME              "userName"
#define EL_WORKGROUP              "workgroup"

#define EL_SERVER_TYPE            "serverType"
#define EL_INTERVAL               "interval"

#define EL_LOCATION               "location"
#define EL_SORT_ASCENDING         "ascending"
#define EL_SORT_FIELD_TYPE        "field"

#define EL_EXPORT_MEMORYTYPE      "usbExport"

#define EL_DOCUMENT_PATH          "documentPath"
#define EL_MANIFEST_PATH          "manifestPath"
#define EL_EXT_NAME               "extName"
#define EL_POSITION               "position"

#define EL_APP_CATEGORY           "appCategory"
#define EL_X_RES_NAME             "xResourceName"
#define EL_IPC_CHANNEL            "ipcChannel"
#define EL_APP_ICON_PATH          "appIconPath"
#define EL_EXECUTE                "execute"

#define EL_ASSOCIATE_APP          "associateApp"
#define EL_ASSOCIATE_ORDER        "associateOrder"
#define EL_EXT_ICON               "extIcon"

#define EL_BEHAVIOUR              "behaviour"

#define EL_NETWORK_TYPE           "networkType"

#define EL_WLAN_SSID              "wlanSsid"
#define EL_WLAN_ENCRYPTION_TYPE   "wlanEncryptionType"
#define EL_WLAN_ENCRYPTION_KEY    "wlanEncryptionKey"
#define EL_WLAN_MODE              "wlanMode"
#define EL_WLAN_CHANNEL           "wlanChannel"
#define EL_WLAN_BROADCAST         "wlanBroadcast"
//#define ENCRYPTIONTYPE_OLDKEY     "wepmode"
//#define ENCRYPTIONKEY_OLDKEY      "wepkey"

#define EL_DIALUP_PHONE           "dialupPhone"
#define EL_DIALUP_USER            "dialupUser"  
#define EL_DIALUP_PASSWORD        "dialupPassword"

#define EL_USE_DHCP               "useDhcp"
#define EL_IP_ADDRESS             "ipAddress"
#define EL_IP_NETMASK             "ipNetmask"
#define EL_IP_DNS                 "ipNameServer"
#define EL_IP_GATEWAY             "ipGateway"

#define EL_USE_PROXY              "useProxy"
#define EL_PROXY_SERVER           "server"
#define EL_PROXY_PORT             "port"

#define EL_TIMESTAMP              "timestamp"

#define EL_PROFILE_CONNECTED_IDS   "profileConnectedIDS"
#define EL_PROFILE_CONNECTED_PC    "profileConnectedPC"
#define EL_MANUAL_CONNECT_TYPE     "manualConnectType"

#define EL_AUTO_CONNECT_WLAN       "autoConnectWlan"

#define EL_ICON_BASE_PATH          "iconBasePath"
#define EL_ICON_FILE_NAME          "iconFileName"

#define EL_ICON_BASE_PATH         "iconBasePath"
#define EL_ICON_FILE_NAME         "iconFileName"

////////////////////////////// attribute names //////////////////////////////
#define ATTR_PROFILE_ID           "profileID"
#define ATTR_TYPE                 "type"
#define ATTR_NAME                 "name"
#define ATTR_ID                   "id"

///////////////////////// enumeration names for elements /////////////////////////
#define IDS                        "IDS"
#define PC                         "PC"

#define AUTHOR                     "AUTHOR"
#define FIELDFILE                  "FILE"  
#define FSNAME                     "FSNAME"
#define EXTENSION                  "EXTENSION"
#define SIZE                       "SIZE"
#define DATE                       "DATE"
#define TITLE                      "TITLE"
#define DESCRIPTION                "DESCRIPTION"

// startup behaviour
#define SPECIFIC_DOCUMENT          "SPECIFIC_DOCUMENT"
#define REFERENCE_MATERIALS        "REFERENCE_MATERIALS"
#define LAST_READ_DOCUMENT         "LAST_READ_DOCUMENT"
#define RECENT_DOCUMENTS           "RECENT_DOCUMENTS"
#define DEVICE_MANAGER             "DEVICE_MANAGER"
#define NEWS_FOLDER                "NEWS_FOLDER"
#define BOOKS_FOLDER               "BOOKS_FOLDER"
#define DOCS_FOLDER                "DOCS_FOLDER"
#define NOTES_FOLDER               "NOTES_FOLDER"

#define WIRED                      "WIRED"
#define WIRELESS                   "WIRELESS"
#define DIALUP                     "DIALUP"

#define ENCR_NONE                  "ENCR_NONE"
#define ENCR_WEP                   "ENCR_WEP"
#define ENCR_WPA                   "ENCR_WPA"

#define INFRASTRUCTURE             "INFRASTRUCTURE"
#define ADHOC                      "ADHOC"

///////////////////// enumeration names for attributes /////////////////////
#define BOOKS_CATEGORY             "BOOKS"
#define NEWSPAPERS_CATEGORY        "NEWSPAPERS"
#define NOTES_CATEGORY             "NOTES"
#define DOCUMENTS_CATEGORY         "DOCUMENTS"
#define CF_CATEGORY                "CF"
#define MAIN_CATEGORY              "MAIN"
#define SD_CATEGORY                "SDMMC"
#define USB_CATEGORY               "USB"
#define MODE_CATEGORY              "MODE"
#define DOWNLOAD_HISTORY           "DOWNLOADHISTORY"
#define SEARCH_RESULT              "SEARCHRESULT"
#define RECENTDOCUMENTS            "RECENTDOCUMENTS"

#define BROWSER_APP                "BROWSER"
#define IPDF_APP                   "IPDF"
#define EBA_APP                    "EBA"
#define SETUP_APP                  "SETUP"
#define PROFILES_APP               "PROFILES"
#define APABIMOBI_APP              "APABIMOBI"
#define IMAGEVIEWSER_APP           "IMAGEVIEWER"
#define DOWNLOADMGR_APP            "DOWNLOADMGR"

#define NORMAL                     "NORMAL"
#define GREY                       "GREY"
#define SELECTED                   "SELECTED"

/////////////// elements in device configuration xml file ////////////////
#define EL_CONFIG_ROOT      "iLiadConfig"

#define EL_DEVICE_DETAILS   "deviceDetails"
#define EL_SOFTWARE         "software"
#define EL_BUILD_NAME       "buildName"
#define EL_BUILD_NUM        "buildNum"
#define EL_MAC_OF_WIRED     "macOfWired"
#define EL_MAC_OF_WIRELESS  "macOfWireless"
#define EL_PUBLIC_KEY       "publicKey"

#define BUF_LEN              1024

typedef struct
{
    char *version;
    char *userProfile;
    char *userSetting;
    char *pcProfiles;
    char *pcProfile;
    char *autoConnect;
    char *category;
    char *usbExport;
    char *lastRead;
    char *application;
    char *extensionInfos;
    char *extensionInfo;
    char *startup;
    char *networkProfiles;
    char *networkProfile;
    char *lastConnect;
    char *autoConnectWlan;
    char *toolbarIcons;
    char *toolbarIcon;
    char *deviceDetails;
} xpathList_t;

typedef struct
{
    regSection_t section;
    gboolean     changed;       // registry data in xmlCxt has changed
    erManifest   xmlCxt;
    regSchema_t  xsdCxt;
    xpathList_t *xpaths;        // store the xpaths which is often used and referred
} regRegistry_t;

////////////////////////// functions declarations ////////////////////////// 

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */
    
    gboolean regValidate(regRegistry_t* pRegistry);
    
    regRegistry_t *regParseFiles(const char *szXmlFile, const char *szXsdFile);
    gboolean regStore(regRegistry_t* pRegistry, const char *szFilename);
    void regDestroy(regRegistry_t *pRegistry);

    regVersion_t *regGetVersion(const regRegistry_t *pRegistry);
    gboolean regSetVersion(const regVersion_t *theVersion, regRegistry_t *pRegistry);

    regUserProfile_t *regGetUserProfile(const regRegistry_t *pRegistry);
    gboolean regSetUserProfile(const regUserProfile_t *theUserProfile, regRegistry_t *pRegistry);

    regUserSetting_t *regGetUserSetting(const regRegistry_t *pRegistry);
    gboolean regSetUserSetting(const regUserSetting_t *theUserSetting, regRegistry_t *pRegistry);

    regPCConfig_t *regGetPCConfig(const regRegistry_t *pRegistry);
    gboolean regSetPCConfig(const regPCConfig_t *thePCConfig, regRegistry_t *pRegistry);
    
    regPCProfile_t *regGetPCProfile(const char *ID, const regRegistry_t *pRegistry);
    gboolean regSetPCProfile(const char *ID, const regPCProfile_t *thePCProfile, regRegistry_t *pRegistry);
    
    gboolean regRemovePCProfile(const char *ID, regRegistry_t *pRegistry);
    gboolean regRemovePCConfig(regRegistry_t *pRegistry);

    void regGetPCProfiles(regPCConfig_t **pPCConfig, regPCProfile_t *** pPCs, const regRegistry_t* pRegistry);
    gboolean regSetPCProfiles(const regPCConfig_t *pPCConfig, const regPCProfile_t **pPCs, regRegistry_t* pRegistry);

    regAutoConnect_t *regGetAutoConnect(const regRegistry_t *pRegistry);
    gboolean regSetAutoConnect(const regAutoConnect_t *theAutoConnect, regRegistry_t *pRegistry);

    regTimedIds_t *regGetTimedIds(const regRegistry_t *pRegistry);
    gboolean regSetTimedIds(regTimedIds_t *theTimedIds, regRegistry_t *pRegistry);
    
    regContentCategory_t *regGetContentCategory(const char *ID, const regRegistry_t *pRegistry);
    gboolean regSetContentCategory(const char *ID, const regContentCategory_t *theContentCategory, regRegistry_t *pRegistry);

    regExportMemType_t *regGetExportMemType(const regRegistry_t *pRegistry);
    gboolean regSetExportMemType(const regExportMemType_t *theExportMemType, regRegistry_t *pRegistry);

    regLastRead_t *regGetLastRead(const char *ID, regRegistry_t *pRegistry);
    gboolean regSetLastRead(const char *ID, const regLastRead_t *theLastRead, regRegistry_t *pRegistry);

    regUserAppList_t *regGetUserAppList(const regRegistry_t *pRegistry);
    regUserApp_t *regGetUserApp(const char *uaID, const regRegistry_t *pRegistry);
    gboolean regSetUserApp(const char *uaID, const regUserApp_t *theUserApp, regRegistry_t *pRegistry);

    regExtInfoList_t *regGetExtInfoList(const char *extension, const regRegistry_t * pRegistry);
    //gboolean regSetExtInfo(const char *extension, const regExtInfo_t * theExtInfo, regRegistry_t * pRegistry);

    regStartUp_t *regGetStartUp(const regRegistry_t* pRegistry);
    gboolean regSetStartUp(const regStartUp_t* theStartUp, regRegistry_t* pRegistry);

    regNetworkConfig_t *regGetNetworkConfig(const regRegistry_t *pRegistry);
    gboolean regSetNetworkConfig(const regNetworkConfig_t *theNetworkConfig, regRegistry_t *pRegistry);
    
    regNetworkProfile_t *regGetNetworkProfile(const char *ID, const regRegistry_t *pRegistry);
    gboolean regSetNetworkProfile(const char *ID, const regNetworkProfile_t *theNetworkProfile, regRegistry_t *pRegistry);

    gboolean regRemoveTimeSet(regRegistry_t *pRegistry);
    gboolean regRemoveNetworkProfile(const char *ID, regRegistry_t *pRegistry);
    gboolean regRemoveNetworkConfig(regRegistry_t *pRegistry);

    void regGetNetworkProfiles(regNetworkConfig_t **pNetworkConfig, regNetworkProfile_t *** pNetworks, const regRegistry_t* pRegistry);
    gboolean regSetNetworkProfiles(const regNetworkConfig_t *pNetworkConfig, const regNetworkProfile_t **pNetworks, regRegistry_t* pRegistry);

    regLastConnect_t *regGetLastConnect(const regRegistry_t* pRegistry);
    gboolean regSetLastConnect(const regLastConnect_t* theLastConnect, regRegistry_t* pRegistry);

    gboolean regGetAutoConnectWlan(const regRegistry_t *pRegistry);
    gboolean regSetAutoConnectWlan(const gboolean enable, regRegistry_t *pRegistry);

    regIconSet_t *regGetIconSet(const regRegistry_t *pRegistry);

    deviceDetails_t* regGetDeviceDetails(const regRegistry_t *pRegistry);
    gboolean regSetDeviceDetails(const deviceDetails_t* theDeviceDetails, regRegistry_t* pRegistry);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif                          // _ER_REG_XML_H_
