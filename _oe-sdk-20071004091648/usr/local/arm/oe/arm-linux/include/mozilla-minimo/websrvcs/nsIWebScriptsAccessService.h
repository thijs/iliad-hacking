/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIWebScriptsAccessService.idl
 */

#ifndef __gen_nsIWebScriptsAccessService_h__
#define __gen_nsIWebScriptsAccessService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIURI; /* forward declaration */


/* starting interface:    nsIWebScriptsAccessService */
#define NS_IWEBSCRIPTSACCESSSERVICE_IID_STR "57e2860b-4266-4a85-bfde-ae39d945b014"

#define NS_IWEBSCRIPTSACCESSSERVICE_IID \
  {0x57e2860b, 0x4266, 0x4a85, \
    { 0xbf, 0xde, 0xae, 0x39, 0xd9, 0x45, 0xb0, 0x14 }}

class NS_NO_VTABLE nsIWebScriptsAccessService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWEBSCRIPTSACCESSSERVICE_IID)

  /**
   * Call this method to check script access...
   * aTransportURI - The web service URI.
   * aType - Type requested by the script.
   * return TRUE if access granted else FALSE
   */
  /* boolean canAccess (in nsIURI aTransportURI, in AString aType); */
  NS_IMETHOD CanAccess(nsIURI *aTransportURI, const nsAString & aType, PRBool *_retval) = 0;

  /**
   * This method will invalidate the cached entry for the transport uri.
   * Also one can clear the _entire_ cache by passing in a null string.
   */
  /* void invalidateCache (in string aTransportURI); */
  NS_IMETHOD InvalidateCache(const char *aTransportURI) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEBSCRIPTSACCESSSERVICE \
  NS_IMETHOD CanAccess(nsIURI *aTransportURI, const nsAString & aType, PRBool *_retval); \
  NS_IMETHOD InvalidateCache(const char *aTransportURI); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEBSCRIPTSACCESSSERVICE(_to) \
  NS_IMETHOD CanAccess(nsIURI *aTransportURI, const nsAString & aType, PRBool *_retval) { return _to CanAccess(aTransportURI, aType, _retval); } \
  NS_IMETHOD InvalidateCache(const char *aTransportURI) { return _to InvalidateCache(aTransportURI); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEBSCRIPTSACCESSSERVICE(_to) \
  NS_IMETHOD CanAccess(nsIURI *aTransportURI, const nsAString & aType, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CanAccess(aTransportURI, aType, _retval); } \
  NS_IMETHOD InvalidateCache(const char *aTransportURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->InvalidateCache(aTransportURI); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWebScriptsAccessService : public nsIWebScriptsAccessService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWEBSCRIPTSACCESSSERVICE

  nsWebScriptsAccessService();

private:
  ~nsWebScriptsAccessService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWebScriptsAccessService, nsIWebScriptsAccessService)

nsWebScriptsAccessService::nsWebScriptsAccessService()
{
  /* member initializers and constructor code */
}

nsWebScriptsAccessService::~nsWebScriptsAccessService()
{
  /* destructor code */
}

/* boolean canAccess (in nsIURI aTransportURI, in AString aType); */
NS_IMETHODIMP nsWebScriptsAccessService::CanAccess(nsIURI *aTransportURI, const nsAString & aType, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void invalidateCache (in string aTransportURI); */
NS_IMETHODIMP nsWebScriptsAccessService::InvalidateCache(const char *aTransportURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_WEBSCRIPTSACCESSSERVICE_CID              \
{ /* 57E2860B-4266-4a85-BFDE-AE39D945B014 */        \
  0x57e2860b, 0x4266, 0x4a85,                       \
  { 0xbf, 0xde, 0xae, 0x39, 0xd9, 0x45, 0xb0, 0x14 }\
}
#define NS_WEBSCRIPTSACCESSSERVICE_CONTRACTID \
"@mozilla.org/xmlextras/securityservice;1"

#endif /* __gen_nsIWebScriptsAccessService_h__ */
