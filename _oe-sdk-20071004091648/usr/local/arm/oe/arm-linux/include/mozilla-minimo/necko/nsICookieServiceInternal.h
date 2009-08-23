/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/netwerk/cookie/public/nsICookieServiceInternal.idl
 */

#ifndef __gen_nsICookieServiceInternal_h__
#define __gen_nsICookieServiceInternal_h__


#ifndef __gen_nsICookieService_h__
#include "nsICookieService.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsICookieServiceInternal */
#define NS_ICOOKIESERVICEINTERNAL_IID_STR "7cf4b3d9-bdc6-4763-b912-47c526aacc37"

#define NS_ICOOKIESERVICEINTERNAL_IID \
  {0x7cf4b3d9, 0xbdc6, 0x4763, \
    { 0xb9, 0x12, 0x47, 0xc5, 0x26, 0xaa, 0xcc, 0x37 }}

/**
 * Set and get cookie values using the security policy.
 *
 * @status INTERNAL  This interface is not frozen and is subject to change
 *                   at any time.
 */
class NS_NO_VTABLE nsICookieServiceInternal : public nsICookieService {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICOOKIESERVICEINTERNAL_IID)

  /**
   * Set a cookie name/value pair, performing privacy checks as appropriate.
   *
   * @see nsICookieService.setCookieString
   * @see nsICookieManager2.add
   */
  /* void setCookieValue (in nsIURI aURI, in nsIChannel aChannel, in AUTF8String aDomain, in AUTF8String aPath, in ACString aName, in ACString aValue, in boolean aIsSession, in PRInt64 aExpiry); */
  NS_IMETHOD SetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aDomain, const nsACString & aPath, const nsACString & aName, const nsACString & aValue, PRBool aIsSession, PRInt64 aExpiry) = 0;

  /**
   * Get a single cookie associated with the URI. This returns the first
   * cookie which matches, if there are multiple cookies with the same name.
   *
   * @param aURI
   *        the URI of the document for which cookies are being queried.
   * @param aChannel
   *        the channel used to load the document.  this parameter may be null,
   *        but it is strongly recommended that a non-null value be provided to
   *        ensure that the cookie privacy preferences are honored.
   *
   * @return the resulting cookie value
   * @throws NS_ERROR_NOT_AVAILABLE if no cookie with this name has been set.
   */
  /* ACString getCookieValue (in nsIURI aURI, in nsIChannel aChannel, in ACString aCookieName); */
  NS_IMETHOD GetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aCookieName, nsACString & _retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICOOKIESERVICEINTERNAL \
  NS_IMETHOD SetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aDomain, const nsACString & aPath, const nsACString & aName, const nsACString & aValue, PRBool aIsSession, PRInt64 aExpiry); \
  NS_IMETHOD GetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aCookieName, nsACString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICOOKIESERVICEINTERNAL(_to) \
  NS_IMETHOD SetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aDomain, const nsACString & aPath, const nsACString & aName, const nsACString & aValue, PRBool aIsSession, PRInt64 aExpiry) { return _to SetCookieValue(aURI, aChannel, aDomain, aPath, aName, aValue, aIsSession, aExpiry); } \
  NS_IMETHOD GetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aCookieName, nsACString & _retval) { return _to GetCookieValue(aURI, aChannel, aCookieName, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICOOKIESERVICEINTERNAL(_to) \
  NS_IMETHOD SetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aDomain, const nsACString & aPath, const nsACString & aName, const nsACString & aValue, PRBool aIsSession, PRInt64 aExpiry) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCookieValue(aURI, aChannel, aDomain, aPath, aName, aValue, aIsSession, aExpiry); } \
  NS_IMETHOD GetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aCookieName, nsACString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCookieValue(aURI, aChannel, aCookieName, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsCookieServiceInternal : public nsICookieServiceInternal
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICOOKIESERVICEINTERNAL

  nsCookieServiceInternal();

private:
  ~nsCookieServiceInternal();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsCookieServiceInternal, nsICookieServiceInternal)

nsCookieServiceInternal::nsCookieServiceInternal()
{
  /* member initializers and constructor code */
}

nsCookieServiceInternal::~nsCookieServiceInternal()
{
  /* destructor code */
}

/* void setCookieValue (in nsIURI aURI, in nsIChannel aChannel, in AUTF8String aDomain, in AUTF8String aPath, in ACString aName, in ACString aValue, in boolean aIsSession, in PRInt64 aExpiry); */
NS_IMETHODIMP nsCookieServiceInternal::SetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aDomain, const nsACString & aPath, const nsACString & aName, const nsACString & aValue, PRBool aIsSession, PRInt64 aExpiry)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* ACString getCookieValue (in nsIURI aURI, in nsIChannel aChannel, in ACString aCookieName); */
NS_IMETHODIMP nsCookieServiceInternal::GetCookieValue(nsIURI *aURI, nsIChannel *aChannel, const nsACString & aCookieName, nsACString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsICookieServiceInternal_h__ */
