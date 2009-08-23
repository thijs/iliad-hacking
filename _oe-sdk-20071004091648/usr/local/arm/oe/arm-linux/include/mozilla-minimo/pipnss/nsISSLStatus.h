/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/security/manager/ssl/public/nsISSLStatus.idl
 */

#ifndef __gen_nsISSLStatus_h__
#define __gen_nsISSLStatus_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIX509Cert; /* forward declaration */


/* starting interface:    nsISSLStatus */
#define NS_ISSLSTATUS_IID_STR "7b2ca1ca-1dd2-11b2-87ec-d217dbe22b85"

#define NS_ISSLSTATUS_IID \
  {0x7b2ca1ca, 0x1dd2, 0x11b2, \
    { 0x87, 0xec, 0xd2, 0x17, 0xdb, 0xe2, 0x2b, 0x85 }}

class NS_NO_VTABLE nsISSLStatus : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISSLSTATUS_IID)

  /* readonly attribute nsIX509Cert serverCert; */
  NS_IMETHOD GetServerCert(nsIX509Cert * *aServerCert) = 0;

  /* readonly attribute string cipherName; */
  NS_IMETHOD GetCipherName(char * *aCipherName) = 0;

  /* readonly attribute unsigned long keyLength; */
  NS_IMETHOD GetKeyLength(PRUint32 *aKeyLength) = 0;

  /* readonly attribute unsigned long secretKeyLength; */
  NS_IMETHOD GetSecretKeyLength(PRUint32 *aSecretKeyLength) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISSLSTATUS \
  NS_IMETHOD GetServerCert(nsIX509Cert * *aServerCert); \
  NS_IMETHOD GetCipherName(char * *aCipherName); \
  NS_IMETHOD GetKeyLength(PRUint32 *aKeyLength); \
  NS_IMETHOD GetSecretKeyLength(PRUint32 *aSecretKeyLength); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISSLSTATUS(_to) \
  NS_IMETHOD GetServerCert(nsIX509Cert * *aServerCert) { return _to GetServerCert(aServerCert); } \
  NS_IMETHOD GetCipherName(char * *aCipherName) { return _to GetCipherName(aCipherName); } \
  NS_IMETHOD GetKeyLength(PRUint32 *aKeyLength) { return _to GetKeyLength(aKeyLength); } \
  NS_IMETHOD GetSecretKeyLength(PRUint32 *aSecretKeyLength) { return _to GetSecretKeyLength(aSecretKeyLength); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISSLSTATUS(_to) \
  NS_IMETHOD GetServerCert(nsIX509Cert * *aServerCert) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetServerCert(aServerCert); } \
  NS_IMETHOD GetCipherName(char * *aCipherName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCipherName(aCipherName); } \
  NS_IMETHOD GetKeyLength(PRUint32 *aKeyLength) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetKeyLength(aKeyLength); } \
  NS_IMETHOD GetSecretKeyLength(PRUint32 *aSecretKeyLength) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSecretKeyLength(aSecretKeyLength); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSSLStatus : public nsISSLStatus
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISSLSTATUS

  nsSSLStatus();

private:
  ~nsSSLStatus();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSSLStatus, nsISSLStatus)

nsSSLStatus::nsSSLStatus()
{
  /* member initializers and constructor code */
}

nsSSLStatus::~nsSSLStatus()
{
  /* destructor code */
}

/* readonly attribute nsIX509Cert serverCert; */
NS_IMETHODIMP nsSSLStatus::GetServerCert(nsIX509Cert * *aServerCert)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string cipherName; */
NS_IMETHODIMP nsSSLStatus::GetCipherName(char * *aCipherName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long keyLength; */
NS_IMETHODIMP nsSSLStatus::GetKeyLength(PRUint32 *aKeyLength)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long secretKeyLength; */
NS_IMETHODIMP nsSSLStatus::GetSecretKeyLength(PRUint32 *aSecretKeyLength)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISSLStatus_h__ */
