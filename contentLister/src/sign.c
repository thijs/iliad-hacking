/*****
 * OpenSSL signatures for iLiad 
 *
 * Copyright 2006-2008 iRex Technologies B.V.
 * All rights reserved.
 * Written by Matthijs van de Water
 *
 */

/* Large parts of this program are based on the OpenSSL example 
 *  demos/sign/sign.c  which is
 * Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 */

#include <stdio.h>
#include <string.h>

#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include "gethwid.h"

#include "sign.h"

#undef BUFSIZE
#define BUFSIZE (1024 * 8)

static char keyfile[] = "/proc/keystore/private";
static char pubkey[]  = "/proc/keystore/public";


int iliad_create_signature(char * infile, char * outfile)
{
    int nRet = ERR_INVALID_PARAMETERS;

    int err;
    unsigned int len;
    int i;
    unsigned char *data;
    char * password;
    char pwbuf[16];
    EVP_MD_CTX      *md_ctx;
    EVP_PKEY *      pkey;
    const EVP_MD   *md = NULL;
    BIO *           key = NULL;
    BIO *           out = NULL;
    BIO *           bmd = NULL;
    BIO *           in = NULL;
    BIO *           inp = NULL;

    /* Parameter checking */
    if (infile == NULL || outfile == NULL)
    {
        nRet = ERR_INVALID_PARAMETERS;
        goto error_exit;
    }

    /* Allocate memory buffer for the input file */
    if ((data = (unsigned char *) OPENSSL_malloc(BUFSIZE)) == NULL)
    {
        nRet = ERR_OUT_OF_MEMORY;
        goto error_exit;
    }

    /* Get the HW ID from the DOCFLASH,
    * and use it as the passphrase */
    password = (char*) malloc(sizeof(char) * 33);
    get_hw_id(pwbuf);
    for(i = 0; i < 16; i++)
    {
        sprintf(&password[i*2], "%02X", pwbuf[i]);
    }
    //  fprintf(stderr, "Using password: %s\n", password);

    /* Load the crypto library error strings */
    ERR_load_crypto_strings();

    /* Load support for all known algorithms, including
       RSA PrivateKey with 3DES encryption */
    OpenSSL_add_all_algorithms();

    /* Init SHA1 */
    md = EVP_get_digestbyname("SHA1");
    bmd = BIO_new(BIO_f_md());
    in = BIO_new(BIO_s_file());

    /* Read private key */
    key = BIO_new(BIO_s_file());
    if (BIO_read_filename(key, keyfile) <= 0)
    {
        nRet = ERR_UNABLE_TO_READ_PRIVATE_KEY;
        goto error_exit;
    }

    pkey = PEM_read_bio_PrivateKey(key, NULL, 0, password);
    BIO_free(key); 
    free(password);

    if (pkey == NULL)
    { 
        ERR_print_errors_fp (stderr);
        nRet = ERR_UNABLE_TO_DECRYPT_PRIVATE_KEY;
        goto error_exit;
    }

    /* We use md as a filter, reading from in */
    if (!BIO_set_md(bmd, md)) 
    {
        nRet = ERR_UNABLE_TO_SET_INPUT_FILTER;
        goto error_exit;
    }
    inp = BIO_push(bmd, in);

    /* Open the input file  */
    if (BIO_read_filename(in, infile) <= 0) 
    {
        nRet = ERR_UNABLE_TO_READ_INPUT_FILE;
        goto error_exit;
    }

    /* Read file */
    for (;;)
    {
        i = BIO_read(inp, (char *) data, BUFSIZE); 

        if (i < 0)
        {
            ERR_print_errors(BIO_new_fp(stderr,BIO_NOCLOSE));
            nRet = ERR_UNABLE_TO_READ_INPUT_FILE;
            goto error_exit;
        }

        if (i == 0)
        {
            break;
        }
    }

    /* Do the signature */
    BIO_get_md_ctx(inp, &md_ctx);
    err = EVP_SignFinal(md_ctx, data, (unsigned int *)&len, pkey);

    if (err != 1)
    {
        ERR_print_errors_fp(stderr);
        nRet = ERR_UNABLE_TO_CREATE_SIGNATURE;
        goto error_exit;
    }

    EVP_PKEY_free (pkey);

    /* 
    * Write signature to file 
    */
    out = BIO_new_file(outfile, "wb");

    i = BIO_write(out, data, len);
    if (i == len)
    {
        fprintf (stderr, "Written signature to file: %s (%d bytes)\n", outfile, len);
        nRet = ERR_SUCCESS;
    }
    else
    {
        fprintf(stderr, "BIO_write returned [%d], expected [%d]\n", i, len);
        nRet = ERR_UNABLE_TO_WRITE_OUTPUT_FILE;
        goto error_exit;
    }

error_exit:
    if (in)  { BIO_free(in);      }
    if (out) { BIO_free_all(out); }
    if (bmd) { BIO_free(bmd);     }

    return nRet;
}


int iliad_verify_signature(char * infile, char * sigfile)
{
    int nRet = ERR_SUCCESS;

    int             err;
    int             i;
    unsigned int    siglen;
    unsigned char  *sigbuf = NULL;
    unsigned char  *data = NULL;
    EVP_MD_CTX     *md_ctx = NULL;
    EVP_PKEY       *pkey = NULL;
    const EVP_MD   *md = NULL;
    BIO            *key = NULL;
    BIO            *sigbio = NULL;
    BIO            *in = NULL, *inp = NULL;
    BIO            *bmd = NULL;

    /* Parameter checking */
    if (infile == NULL || sigfile == NULL)
    {
        nRet = ERR_INVALID_PARAMETERS;
        goto error_exit;
    }

    if ((data = (unsigned char *) OPENSSL_malloc(BUFSIZE)) == NULL)
    {
        nRet = ERR_OUT_OF_MEMORY;
        goto error_exit;
    }

    /* Load the crypto library error strings */
    ERR_load_crypto_strings();

    /* Load support for all known algorithms, including
    RSA PrivateKey with 3DES encryption */
    OpenSSL_add_all_algorithms();

    /* Use SHA1 hashing */  
    md = EVP_get_digestbyname("SHA1");
    bmd = BIO_new(BIO_f_md());
    in = BIO_new(BIO_s_file());

    /* Read public key */
    key = BIO_new(BIO_s_file());
    if (BIO_read_filename(key, pubkey) <= 0)
    {
        nRet = ERR_UNABLE_TO_READ_PUBLIC_KEY;
        goto error_exit;
    }

    pkey = PEM_read_bio_PUBKEY(key, NULL, NULL, NULL);
    BIO_free(key);

    if (pkey == NULL)
    {
        ERR_print_errors_fp (stderr);
        nRet = ERR_UNABLE_TO_READ_PUBLIC_KEY;
        goto error_exit;
    }

    /* We use md as a filter, reading from in */
    if (!BIO_set_md(bmd, md))
    {
        nRet = ERR_UNABLE_TO_SET_INPUT_FILTER;
        goto error_exit;
    }
    inp = BIO_push(bmd, in);

    /* Open the file to be verified */
    if (BIO_read_filename(in, infile) <= 0)
    {
        nRet = ERR_UNABLE_TO_READ_INPUT_FILE;
        goto error_exit;
    }

    /* Read file */
    for (;;)
    {
        i = BIO_read(inp, (char *) data, BUFSIZE); 

        if (i < 0)
        {
            nRet = ERR_UNABLE_TO_READ_INPUT_FILE;
            goto error_exit;
        }

        if (i == 0)
        {
            break;
        }
    }

    /* Read the signature from file */
    sigbio = BIO_new_file(sigfile, "rb");
    siglen = EVP_PKEY_size(pkey);
    sigbuf = OPENSSL_malloc(siglen);
    if(!sigbio)
    {
        nRet = ERR_UNABLE_TO_READ_SIGNATURE_FILE;
        goto error_exit;
    }

    siglen = BIO_read(sigbio, sigbuf, siglen);
    BIO_free(sigbio);
    if(siglen <= 0)
    {
        nRet = ERR_UNABLE_TO_READ_SIGNATURE_FILE;
        goto error_exit;
    }

    //  fprintf(stderr, "Signature (%d): %s\n", siglen, sigbuf);

    /*
    * Verify the signature 
    */
    BIO_get_md_ctx(inp, &md_ctx);
    err = EVP_VerifyFinal (md_ctx, sigbuf, siglen, pkey);


    if (err <= 0)
    {
        ERR_print_errors_fp (stderr);
        nRet = ERR_VERIFICATION_FAILED;
    }
    else
    {
        fprintf (stderr, "Signature for %s Verified OK.\n", infile);
        nRet = ERR_SUCCESS;
    }

error_exit:
    if (pkey) EVP_PKEY_free (pkey);
    if (data) { OPENSSL_cleanse(data, BUFSIZE); OPENSSL_free(data); }
    if (sigbuf) OPENSSL_free(sigbuf);
    if (in) BIO_free(in);
    if (bmd) BIO_free(bmd);

    return nRet;
}

