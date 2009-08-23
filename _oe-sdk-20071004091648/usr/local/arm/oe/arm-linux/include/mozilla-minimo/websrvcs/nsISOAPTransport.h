/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPTransport.idl
 */

#ifndef __gen_nsISOAPTransport_h__
#define __gen_nsISOAPTransport_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISOAPTransportListener; /* forward declaration */

class nsISOAPCall; /* forward declaration */

class nsISOAPResponse; /* forward declaration */

class nsISOAPResponseListener; /* forward declaration */

class nsISOAPCallCompletion; /* forward declaration */


/* starting interface:    nsISOAPTransport */
#define NS_ISOAPTRANSPORT_IID_STR "99ec6695-535f-11d4-9a58-000064657374"

#define NS_ISOAPTRANSPORT_IID \
  {0x99ec6695, 0x535f, 0x11d4, \
    { 0x9a, 0x58, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISOAPTransport : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPTRANSPORT_IID)

  /**
   * Send the specified message to the specified destination.
   * This will fail if synchronous calls are not supported or if there is any
   * failure in the actual message exchange.  Failure of the call itself will be
   * contained in the response.
   *
   * @param aCall Actual message to be sent.
   *
   * @param aResponse Message to be recieved.  Calling synchronously assumes that 
   *   exactly one response is expected.
   */
  /* void syncCall (in nsISOAPCall aCall, in nsISOAPResponse aResponse); */
  NS_IMETHOD SyncCall(nsISOAPCall *aCall, nsISOAPResponse *aResponse) = 0;

  /**
   * Send the specified message to the specified destination synchronously waiting 
   * for completion and any response.
   * This will fail if there is any failure in the setup of the message exchange.
   * Later errors will only be known through the response listener.  Failures of the
   * call itself will be contained in the response passed to the response listener.
   *
   * @param aCall Actual message to be sent.
   *
   * @param aListener Handler to be invoked (single threaded) as each response is 
   *  received and finally with null.  If specified as null, no responses are returned.
   *
   * @param response Message to recieve response and be handled by listener.  May be 
   *   null if listener is null.
   */
  /* nsISOAPCallCompletion asyncCall (in nsISOAPCall aCall, in nsISOAPResponseListener aListener, in nsISOAPResponse aResponse); */
  NS_IMETHOD AsyncCall(nsISOAPCall *aCall, nsISOAPResponseListener *aListener, nsISOAPResponse *aResponse, nsISOAPCallCompletion **_retval) = 0;

  /**
   * Add listener for unsolicited messages arriving on the transport.  Listeners
   *  are provided with the opportunity to accept and process messages.  Typically
   *  a listener will be a service dispatcher.  Listeners will be invoked in the
   *  reverse order of declaration, allowing more local service dispatchers to
   *  temporarily override permanent service dispatchers.  This will fail if the
   *  desired listener was already added to the transport with the specified
   *  capture flag or if the transport does not support incoming messages.
   *
   * @param aListener The listener to recieve unsolicited messages from the
   *   transport.
   *
   * @param aCapture True if the listener should capture the message before
   *   later-declared services.
   */
  /* void addListener (in nsISOAPTransportListener aListener, in boolean aCapture); */
  NS_IMETHOD AddListener(nsISOAPTransportListener *aListener, PRBool aCapture) = 0;

  /**
   * Remove listener for unsolicited messages arriving on the transport.  This
   *  will fail if the specified listener was not added with the specified
   *  capture setting.
   *
   * @param aListener The listener to stop from recieving unsolicited messages 
   *  from the transport.
   *
   * @param aCapture True if the listener was added to capture the message before
   *   later-declared services (must be specified to remove, since a listener
   *   may be registered as both).
   */
  /* void removeListener (in nsISOAPTransportListener aListener, in boolean aCapture); */
  NS_IMETHOD RemoveListener(nsISOAPTransportListener *aListener, PRBool aCapture) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPTRANSPORT \
  NS_IMETHOD SyncCall(nsISOAPCall *aCall, nsISOAPResponse *aResponse); \
  NS_IMETHOD AsyncCall(nsISOAPCall *aCall, nsISOAPResponseListener *aListener, nsISOAPResponse *aResponse, nsISOAPCallCompletion **_retval); \
  NS_IMETHOD AddListener(nsISOAPTransportListener *aListener, PRBool aCapture); \
  NS_IMETHOD RemoveListener(nsISOAPTransportListener *aListener, PRBool aCapture); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPTRANSPORT(_to) \
  NS_IMETHOD SyncCall(nsISOAPCall *aCall, nsISOAPResponse *aResponse) { return _to SyncCall(aCall, aResponse); } \
  NS_IMETHOD AsyncCall(nsISOAPCall *aCall, nsISOAPResponseListener *aListener, nsISOAPResponse *aResponse, nsISOAPCallCompletion **_retval) { return _to AsyncCall(aCall, aListener, aResponse, _retval); } \
  NS_IMETHOD AddListener(nsISOAPTransportListener *aListener, PRBool aCapture) { return _to AddListener(aListener, aCapture); } \
  NS_IMETHOD RemoveListener(nsISOAPTransportListener *aListener, PRBool aCapture) { return _to RemoveListener(aListener, aCapture); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPTRANSPORT(_to) \
  NS_IMETHOD SyncCall(nsISOAPCall *aCall, nsISOAPResponse *aResponse) { return !_to ? NS_ERROR_NULL_POINTER : _to->SyncCall(aCall, aResponse); } \
  NS_IMETHOD AsyncCall(nsISOAPCall *aCall, nsISOAPResponseListener *aListener, nsISOAPResponse *aResponse, nsISOAPCallCompletion **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->AsyncCall(aCall, aListener, aResponse, _retval); } \
  NS_IMETHOD AddListener(nsISOAPTransportListener *aListener, PRBool aCapture) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddListener(aListener, aCapture); } \
  NS_IMETHOD RemoveListener(nsISOAPTransportListener *aListener, PRBool aCapture) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveListener(aListener, aCapture); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPTransport : public nsISOAPTransport
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPTRANSPORT

  nsSOAPTransport();

private:
  ~nsSOAPTransport();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPTransport, nsISOAPTransport)

nsSOAPTransport::nsSOAPTransport()
{
  /* member initializers and constructor code */
}

nsSOAPTransport::~nsSOAPTransport()
{
  /* destructor code */
}

/* void syncCall (in nsISOAPCall aCall, in nsISOAPResponse aResponse); */
NS_IMETHODIMP nsSOAPTransport::SyncCall(nsISOAPCall *aCall, nsISOAPResponse *aResponse)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISOAPCallCompletion asyncCall (in nsISOAPCall aCall, in nsISOAPResponseListener aListener, in nsISOAPResponse aResponse); */
NS_IMETHODIMP nsSOAPTransport::AsyncCall(nsISOAPCall *aCall, nsISOAPResponseListener *aListener, nsISOAPResponse *aResponse, nsISOAPCallCompletion **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addListener (in nsISOAPTransportListener aListener, in boolean aCapture); */
NS_IMETHODIMP nsSOAPTransport::AddListener(nsISOAPTransportListener *aListener, PRBool aCapture)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeListener (in nsISOAPTransportListener aListener, in boolean aCapture); */
NS_IMETHODIMP nsSOAPTransport::RemoveListener(nsISOAPTransportListener *aListener, PRBool aCapture)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SOAPTRANSPORT_CONTRACTID  \
"@mozilla.org/xmlextras/soap/transport;1"
#define NS_SOAPTRANSPORT_CONTRACTID_PREFIX   NS_SOAPTRANSPORT_CONTRACTID "?protocol="

#endif /* __gen_nsISOAPTransport_h__ */
