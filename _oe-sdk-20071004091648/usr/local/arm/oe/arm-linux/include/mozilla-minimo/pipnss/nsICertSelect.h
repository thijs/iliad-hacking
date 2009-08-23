/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/security/manager/ssl/public/nsICertSelect.idl
 */

#ifndef __gen_nsICertSelect_h__
#define __gen_nsICertSelect_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIX509Cert; /* forward declaration */

class nsIChannelSecurityInfo; /* forward declaration */


/* starting interface:    nsICertSelect */
#define NS_ICERTSELECT_IID_STR "3cac403c-edb3-11d4-998b-00b0d02354a0"

#define NS_ICERTSELECT_IID \
  {0x3cac403c, 0xedb3, 0x11d4, \
    { 0x99, 0x8b, 0x00, 0xb0, 0xd0, 0x23, 0x54, 0xa0 }}

class NS_NO_VTABLE nsICertSelect : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICERTSELECT_IID)

  /* nsIX509Cert selectClientAuthCert (in nsIChannelSecurityInfo socketInfo, in unsigned long count, [array, size_is (count)] in nsIX509Cert certs); */
  NS_IMETHOD SelectClientAuthCert(nsIChannelSecurityInfo *socketInfo, PRUint32 count, nsIX509Cert **certs, nsIX509Cert **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICERTSELECT \
  NS_IMETHOD SelectClientAuthCert(nsIChannelSecurityInfo *socketInfo, PRUint32 count, nsIX509Cert **certs, nsIX509Cert **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICERTSELECT(_to) \
  NS_IMETHOD SelectClientAuthCert(nsIChannelSecurityInfo *socketInfo, PRUint32 count, nsIX509Cert **certs, nsIX509Cert **_retval) { return _to SelectClientAuthCert(socketInfo, count, certs, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICERTSELECT(_to) \
  NS_IMETHOD SelectClientAuthCert(nsIChannelSecurityInfo *socketInfo, PRUint32 count, nsIX509Cert **certs, nsIX509Cert **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->SelectClientAuthCert(socketInfo, count, certs, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsCertSelect : public nsICertSelect
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICERTSELECT

  nsCertSelect();

private:
  ~nsCertSelect();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsCertSelect, nsICertSelect)

nsCertSelect::nsCertSelect()
{
  /* member initializers and constructor code */
}

nsCertSelect::~nsCertSelect()
{
  /* destructor code */
}

/* nsIX509Cert selectClientAuthCert (in nsIChannelSecurityInfo socketInfo, in unsigned long count, [array, size_is (count)] in nsIX509Cert certs); */
NS_IMETHODIMP nsCertSelect::SelectClientAuthCert(nsIChannelSecurityInfo *socketInfo, PRUint32 count, nsIX509Cert **certs, nsIX509Cert **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsICertSelect_h__ */
