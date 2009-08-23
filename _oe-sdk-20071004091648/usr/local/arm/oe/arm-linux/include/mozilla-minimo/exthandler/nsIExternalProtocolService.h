/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/uriloader/exthandler/nsIExternalProtocolService.idl
 */

#ifndef __gen_nsIExternalProtocolService_h__
#define __gen_nsIExternalProtocolService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIURI; /* forward declaration */

class nsIFile; /* forward declaration */

class nsIPrompt; /* forward declaration */


/* starting interface:    nsIExternalProtocolService */
#define NS_IEXTERNALPROTOCOLSERVICE_IID_STR "a49813a4-98b7-4bdb-998c-8bd9704af0c0"

#define NS_IEXTERNALPROTOCOLSERVICE_IID \
  {0xa49813a4, 0x98b7, 0x4bdb, \
    { 0x99, 0x8c, 0x8b, 0xd9, 0x70, 0x4a, 0xf0, 0xc0 }}

/**
 * The external protocol service is used for finding and launching
 * platform specific applications for particular protocols.
 *
 * You can ask the external protocol service if it has an external
 * handler for a given protocol scheme. And you can ask it to load 
 * the url using the default handler.
 */
class NS_NO_VTABLE nsIExternalProtocolService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IEXTERNALPROTOCOLSERVICE_IID)

  /**
   * Check whether a handler for a specific protocol exists.
   * @param aProtocolScheme The scheme from a url: http, ftp, mailto, etc.
   * @return true if we have a handler and false otherwise.
   */
  /* boolean externalProtocolHandlerExists (in string aProtocolScheme); */
  NS_IMETHOD ExternalProtocolHandlerExists(const char *aProtocolScheme, PRBool *_retval) = 0;

  /**
   * Check whether a handler for a specific protocol is "exposed" as a visible
   * feature of the current application.
   *
   * An exposed protocol handler is one that can be used in all contexts.  A
   * non-exposed protocol handler is one that can only be used internally by the
   * application.  For example, a non-exposed protocol would not be loaded by the
   * application in response to a link click or a X-remote openURL command.
   * Instead, it would be deferred to the system's external protocol handler.
   */
  /* boolean isExposedProtocol (in string aProtocolScheme); */
  NS_IMETHOD IsExposedProtocol(const char *aProtocolScheme, PRBool *_retval) = 0;

  /**
   * Used to load a url via an external protocol handler (if one exists)
   * @param aURL The url to load
   */
  /* void loadUrl (in nsIURI aURL); */
  NS_IMETHOD LoadUrl(nsIURI *aURL) = 0;

  /**
   * Used to load a URI via an external application. Might prompt the user for
   * permission to load the external application. Replaces loadUrl()
   *
   * @param aURI    The URI to load
   * @param aPrompt If null we grab one from windowwatcher if we need it
   */
  /* void loadURI (in nsIURI aURI, in nsIPrompt aPrompt); */
  NS_IMETHOD LoadURI(nsIURI *aURI, nsIPrompt *aPrompt) = 0;

  /**
   * Gets a human-readable description for the application responsible for
   * handling a specific protocol.
   *
   * @param aScheme The scheme to look up. For example, "mms".
   *
   * @throw NS_ERROR_NOT_IMPLEMENTED
   *        If getting descriptions for protocol helpers is not supported
   * @throw NS_ERROR_NOT_AVAILABLE
   *        If no protocol helper exists for this scheme, or if it is not
   *        possible to get a description for it.
   */
  /* AString getApplicationDescription (in AUTF8String aScheme); */
  NS_IMETHOD GetApplicationDescription(const nsACString & aScheme, nsAString & _retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIEXTERNALPROTOCOLSERVICE \
  NS_IMETHOD ExternalProtocolHandlerExists(const char *aProtocolScheme, PRBool *_retval); \
  NS_IMETHOD IsExposedProtocol(const char *aProtocolScheme, PRBool *_retval); \
  NS_IMETHOD LoadUrl(nsIURI *aURL); \
  NS_IMETHOD LoadURI(nsIURI *aURI, nsIPrompt *aPrompt); \
  NS_IMETHOD GetApplicationDescription(const nsACString & aScheme, nsAString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIEXTERNALPROTOCOLSERVICE(_to) \
  NS_IMETHOD ExternalProtocolHandlerExists(const char *aProtocolScheme, PRBool *_retval) { return _to ExternalProtocolHandlerExists(aProtocolScheme, _retval); } \
  NS_IMETHOD IsExposedProtocol(const char *aProtocolScheme, PRBool *_retval) { return _to IsExposedProtocol(aProtocolScheme, _retval); } \
  NS_IMETHOD LoadUrl(nsIURI *aURL) { return _to LoadUrl(aURL); } \
  NS_IMETHOD LoadURI(nsIURI *aURI, nsIPrompt *aPrompt) { return _to LoadURI(aURI, aPrompt); } \
  NS_IMETHOD GetApplicationDescription(const nsACString & aScheme, nsAString & _retval) { return _to GetApplicationDescription(aScheme, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIEXTERNALPROTOCOLSERVICE(_to) \
  NS_IMETHOD ExternalProtocolHandlerExists(const char *aProtocolScheme, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->ExternalProtocolHandlerExists(aProtocolScheme, _retval); } \
  NS_IMETHOD IsExposedProtocol(const char *aProtocolScheme, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsExposedProtocol(aProtocolScheme, _retval); } \
  NS_IMETHOD LoadUrl(nsIURI *aURL) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoadUrl(aURL); } \
  NS_IMETHOD LoadURI(nsIURI *aURI, nsIPrompt *aPrompt) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoadURI(aURI, aPrompt); } \
  NS_IMETHOD GetApplicationDescription(const nsACString & aScheme, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetApplicationDescription(aScheme, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsExternalProtocolService : public nsIExternalProtocolService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIEXTERNALPROTOCOLSERVICE

  nsExternalProtocolService();

private:
  ~nsExternalProtocolService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsExternalProtocolService, nsIExternalProtocolService)

nsExternalProtocolService::nsExternalProtocolService()
{
  /* member initializers and constructor code */
}

nsExternalProtocolService::~nsExternalProtocolService()
{
  /* destructor code */
}

/* boolean externalProtocolHandlerExists (in string aProtocolScheme); */
NS_IMETHODIMP nsExternalProtocolService::ExternalProtocolHandlerExists(const char *aProtocolScheme, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isExposedProtocol (in string aProtocolScheme); */
NS_IMETHODIMP nsExternalProtocolService::IsExposedProtocol(const char *aProtocolScheme, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void loadUrl (in nsIURI aURL); */
NS_IMETHODIMP nsExternalProtocolService::LoadUrl(nsIURI *aURL)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void loadURI (in nsIURI aURI, in nsIPrompt aPrompt); */
NS_IMETHODIMP nsExternalProtocolService::LoadURI(nsIURI *aURI, nsIPrompt *aPrompt)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getApplicationDescription (in AUTF8String aScheme); */
NS_IMETHODIMP nsExternalProtocolService::GetApplicationDescription(const nsACString & aScheme, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIExternalProtocolService_h__ */
