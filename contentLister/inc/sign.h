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
 * Copyright (C) 2005-2008 iRex Technologies B.V.
 * All rights reserved.
 */

#ifndef __ILIAD_SIGN_H__
#define __ILIAD_SIGN_H__

#ifdef __cplusplus
extern  "C" {
#endif  /* __cplusplus */

/* These are the possible return values of the functions below */
#define ERR_SUCCESS                        (0)
#define ERR_INVALID_PARAMETERS             (1)
#define ERR_OUT_OF_MEMORY                  (2)
#define ERR_UNABLE_TO_READ_PRIVATE_KEY     (3)
#define ERR_UNABLE_TO_DECRYPT_PRIVATE_KEY  (4)
#define ERR_UNABLE_TO_READ_SOURCE_FILE     (5)
#define ERR_UNABLE_TO_CREATE_SIGNATURE     (6)
#define ERR_UNABLE_TO_WRITE_OUTPUT_FILE    (7)
#define ERR_UNABLE_TO_READ_PUBLIC_KEY      (8)
#define ERR_UNABLE_TO_READ_INPUT_FILE      (9)
#define ERR_UNABLE_TO_READ_SIGNATURE_FILE  (10)
#define ERR_UNABLE_TO_SET_INPUT_FILTER     (11)
#define ERR_VERIFICATION_FAILED            (99)

/* 
 * Create a SHA1 signature of input_filename and write it to signature_filename.
 * Use the iLiad Private Key, sign it with the appropriate passphrase. 
 */
int iliad_create_signature(char * input_filename, char * signature_filename);

/* 
 * Verify the SHA1 signature of input_filename where the signature is read from
 * signature_filename. This uses the iLiad public key.
 */
int iliad_verify_signature(char * input_filename, char * signature_filename);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
