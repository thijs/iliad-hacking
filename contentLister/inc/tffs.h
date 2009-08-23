/***********************************************************************************/
/*           Copyright (C) M-Systems Flash Disk Pioneers Ltd. 1995-2006            */
/*                         All Rights Reserved                                     */
/***********************************************************************************/

/* 
 * Custom TFFS include file for iRex Technologies B.V. contentLister
 *
 * Support file for gethwid function
 *
 */

#ifndef TFFS_H
#define TFFS_H

#include <sys/ioctl.h>

#define FL_IOCTL_START (SIOCDEVPRIVATE + 1)

/* In every call to flIOctl function, the irFlags field in the structure
IOreq should hold one of the following: */
typedef enum{FL_IOCTL_GET_INFO = FL_IOCTL_START,
  FL_IOCTL_DEFRAGMENT,
  FL_IOCTL_NO_LONGER_SUPPORTED_0,	/* FL_IOCTL_WRITE_PROTECT             */
  FL_IOCTL_MOUNT_VOLUME,
  FL_IOCTL_NO_LONGER_SUPPORTED_1,	/* FL_IOCTL_FORMAT_VOLUME             */
  FL_IOCTL_BDK_OPERATION,
  FL_IOCTL_DELETE_SECTORS,
  FL_IOCTL_READ_SECTORS,
  FL_IOCTL_WRITE_SECTORS,
  FL_IOCTL_NO_LONGER_SUPPORTED_2,	/* FL_IOCTL_FORMAT_PHYSICAL_DRIVE     */
  FL_IOCTL_NO_LONGER_SUPPORTED_3,	/* FL_IOCTL_FORMAT_LOGICAL_DRIVE      */
  FL_IOCTL_BDTL_HW_PROTECTION,
  FL_IOCTL_BINARY_HW_PROTECTION,
  FL_IOCTL_OTP,
  FL_IOCTL_CUSTOMER_ID,
  FL_IOCTL_UNIQUE_ID,
  FL_IOCTL_NUMBER_OF_PARTITIONS,
  FL_IOCTL_NO_LONGER_SUPPORTED_4,	/* FL_IOCTL_INQUIRE_CAPABILITIES      */ 
  FL_IOCTL_NO_LONGER_SUPPORTED_5,	/* FL_IOCTL_SET_ENVIRONMENT_VARIABLES */
  FL_IOCTL_NO_LONGER_SUPPORTED_6,	/* FL_IOCTL_PLACE_EXB_BY_BUFFER       */
  FL_IOCTL_NO_LONGER_SUPPORTED_7,	/* FL_IOCTL_WRITE_IPL                 */
  FL_IOCTL_DEEP_POWER_DOWN_MODE,
  FL_IOCTL_EXTENDED_ENVIRONMENT_VARIABLES, 
  FL_IOCTL_VERIFY_VOLUME,	 
  FL_IOCTL_SET_ACCESS_ROUTINE,
  FL_IOCTL_GET_ACCESS_ROUTINE,
  FL_IOCTL_NO_LONGER_SUPPORTED_8,	/* FL_IOCTL_EXTENDED_WRITE_IPL        */  
  FL_IOCTL_NO_LONGER_SUPPORTED_9,	/* FL_IOCTL_OPEN_TRANSACTION          */
  FL_IOCTL_NO_LONGER_SUPPORTED_10,	/* FL_IOCTL_COMMIT_TRANSACTION        */
  FL_IOCTL_NO_LONGER_SUPPORTED_11,	/* FL_IOCTL_IS_VOLUME_RUGGEDIZED      */
  FL_IOCTL_IS_QUICK_MOUNT_VALID,      
  FL_IOCTL_WRITE_QUICK_MOUNT_INFO,
  FL_IOCTL_CLEAR_QUICK_MOUNT_INFO,
  FL_IOCTL_NO_LONGER_SUPPORTED_12,	/* FL_IOCTL_COMPLETE_OPERATION        */
  FL_IOCTL_FLASH_FORMAT, /* Replacing FL_IOCTL_FORMAT_PHYSICAL_DRIVE       */
  FL_IOCTL_ERASE_BD,                  /* Added in TrueFFS 6.3.0            */
  FL_IOCTL_FLASH_UNFORMAT,            /* Added in TrueFFS 6.3.0            */
  FL_IOCTL_WRITE_IPL,                 /* Changed in TrueFFS 6.3.0          */
  FL_IOCTL_MCOTP
  ,FL_IOCTL_PASS_THROUGH
} flIOctlFunctionNo;

/* FLStatus struct */
typedef enum {                         
         flOK                      = 0,
         flBadFunction             = 1,
         flFileNotFound            = 2,
         flPathNotFound            = 3,
         flTooManyOpenFiles        = 4,
         flNoWriteAccess           = 5,
         flBadFileHandle           = 6,
         flDriveNotAvailable       = 9,
         flNonFATformat            = 10,
         flFormatNotSupported      = 11,
         flNoMoreFiles             = 18,
         flWriteProtect            = 19,
         flBadDriveHandle          = 20,
         flDriveNotReady           = 21,
         flUnknownCmd              = 22,
         flBadFormat               = 23,
         flBadLength               = 24,
         flDataError               = 25,
         flUnknownMedia            = 26,
         flSectorNotFound          = 27,
         flOutOfPaper              = 28,
         flWriteFault              = 29,
         flReadFault               = 30,
         flGeneralFailure          = 31,
         flDiskChange              = 34,
         flVppFailure              = 50,
         flBadParameter            = 51,
         flNoSpaceInVolume         = 52,
         flInvalidFATchain         = 53,
         flRootDirectoryFull       = 54,
         flNotMounted              = 55,
         flPathIsRootDirectory     = 56,
         flNotADirectory           = 57,
         flDirectoryNotEmpty       = 58,
         flFileIsADirectory        = 59,
         flAdapterNotFound         = 60,
         flFormattingError         = 62,
         flNotEnoughMemory         = 63,
         flVolumeTooSmall          = 64,
         flBufferingError          = 65,
         flFileAlreadyExists       = 80,
         flForceDismount           = 85,
         flIncomplete              = 100,
         flTimedOut                = 101,
         flTooManyComponents       = 102,
         flTooManyDrives           = 103,
         flTooManyBinaryPartitions = 104,
         flPartitionNotFound       = 105,
         flFeatureNotSupported     = 106,
         flWrongVersion            = 107,
         flTooManyBadBlocks        = 108,
         flNotProtected            = 109,
         flUnchangeableProtection  = 110,
         flBadDownload             = 111,
         flBadBBT                  = 112,
         flInterleaveError         = 113,
         flWrongKey                = 114,
         flHWProtection            = 115,
         flLeftForCompetability    = 116,
         flMultiDocContradiction   = 117,
         flCanNotFold              = 118,
         flBadIPLBlock             = 119,
         flIOCommandBlocked        = 120,
         flBadTransaction          = 121,
         flTooManyTransactions     = 122,
         flUnitIsNotErased         = 123,
         flSequenceError           = 124,
         flFIFOTimeOut             = 125,
         flUnitIsBad               = 126,
         flFlashIsVirgin           = 127,
         flQuickMountNotValid      = 128,

         flSuspendModeDetected     = 129,
         flDmaFailed     		   = 130,
		 flSuspendDueToHWFailure   = 131,

         flCannotCreateShortName   = 150,
         flInvalidCharSet	       = 151,
         flOverwriteExistingEntry  = 152,
         flBufferTooShort	       = 153,
         flPathTooLong		       = 154,
         flBadPartitionSize        = 155,
         flTooManyPartitions       = 156,
         flFSO_IDnotFound	       = 157,
         flFSO_IDisBusy	     	   = 158
} FLStatus;

typedef unsigned char  FLByte;           /* 8  bit unsigned variable */

/* Backward competible x86 pointer definition */
#ifndef FAR1
#define FAR1
#endif /* FAR1 */

/* In every call to flIOctl function, the irData field in the structure
IOreq should point to the structure defined below. The fields
inputRecord and outputRecord should point to structures which are
specific to each IOctl function as defined in this file. */
typedef struct {
	void FAR1 *inputRecord;
	void FAR1 *outputRecord;
} flIOctlRecord;

/* Read unique ID (FL_IOCTL_UNIQUE_ID) */
/* Input record: NULL */
/* Output record: */
typedef struct {
	FLByte  id[16];
	FLStatus status;
} flUniqueIdOutput;

#endif /* TFFS_H */
