/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/security/manager/ssl/public/nsIX509CertDB2.idl
 */

#ifndef __gen_nsIX509CertDB2_h__
#define __gen_nsIX509CertDB2_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIX509CertDB2 */
#define NS_IX509CERTDB2_IID_STR "dedec2ca-f941-4638-a9c0-32e02ff83d5b"

#define NS_IX509CERTDB2_IID \
  {0xdedec2ca, 0xf941, 0x4638, \
    { 0xa9, 0xc0, 0x32, 0xe0, 0x2f, 0xf8, 0x3d, 0x5b }}

/**
 * This represents a service to access and manipulate 
 * X.509 certificates stored in a database through methods
 * not in nsIX509CertDB, which is frozen
 *
 */
class NS_NO_VTABLE nsIX509CertDB2 : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IX509CERTDB2_IID)

  /* void addCertFromBase64 (in string base64, in string aTrust, in string aName); */
  NS_IMETHOD AddCertFromBase64(const char *base64, const char *aTrust, const char *aName) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIX509CERTDB2 \
  NS_IMETHOD AddCertFromBase64(const char *base64, const char *aTrust, const char *aName); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIX509CERTDB2(_to) \
  NS_IMETHOD AddCertFromBase64(const char *base64, const char *aTrust, const char *aName) { return _to AddCertFromBase64(base64, aTrust, aName); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIX509CERTDB2(_to) \
  NS_IMETHOD AddCertFromBase64(const char *base64, const char *aTrust, const char *aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddCertFromBase64(base64, aTrust, aName); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsX509CertDB2 : public nsIX509CertDB2
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIX509CERTDB2

  nsX509CertDB2();

private:
  ~nsX509CertDB2();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsX509CertDB2, nsIX509CertDB2)

nsX509CertDB2::nsX509CertDB2()
{
  /* member initializers and constructor code */
}

nsX509CertDB2::~nsX509CertDB2()
{
  /* destructor code */
}

/* void addCertFromBase64 (in string base64, in string aTrust, in string aName); */
NS_IMETHODIMP nsX509CertDB2::AddCertFromBase64(const char *base64, const char *aTrust, const char *aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIX509CertDB2_h__ */
