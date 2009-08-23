/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPCall.idl
 */

#ifndef __gen_nsISOAPCall_h__
#define __gen_nsISOAPCall_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsISOAPMessage_h__
#include "nsISOAPMessage.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISOAPResponse; /* forward declaration */

class nsISOAPResponseListener; /* forward declaration */

class nsISOAPCallCompletion; /* forward declaration */


/* starting interface:    nsISOAPCall */
#define NS_ISOAPCALL_IID_STR "a8fefe40-52bc-11d4-9a57-000064657374"

#define NS_ISOAPCALL_IID \
  {0xa8fefe40, 0x52bc, 0x11d4, \
    { 0x9a, 0x57, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

/**
 * This interface is a convenience extension of the basic SOAP message,
 * which handles common patterns of calling, such as providing an
 * action URI in the HTTP header, locating and invoking the appropriate
 * transport based upon the protocol of the transportURI, and
 * automatically recieving the result in a new nsISOAPResponse object
 * which recieves an XML message.
 */
class NS_NO_VTABLE nsISOAPCall : public nsISOAPMessage {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPCALL_IID)

  /**
   * The URI to which the message will be sent, identifying the
   *   transport and transport-specific information about the
   *   destination.
   * This does not have to match the <code>targetObjectURI</code>.
   */
  /* attribute AString transportURI; */
  NS_IMETHOD GetTransportURI(nsAString & aTransportURI) = 0;
  NS_IMETHOD SetTransportURI(const nsAString & aTransportURI) = 0;

  /**
   * Enables alternative security model which may be available
   *   from participating services.  Securely adds a 
   *   "verifySource" header to the outgoing message with 
   *   "mustUnderstand" enabled, which permits the server to 
   *   decide whether the call should be honored from that 
   *   particular source.  i
   * <p>When this verification header is enabled, the 
   *   calling script is released from the draconion 
   *   security checks of unverified SOAP calls.  But
   *   the service being invoked must not reject the
   *   message, which is the prescribed action if a SOAP
   *   server receives a header of type "mustUnderstand" that
   *   it does not understand.
   * <p>Servers which accept "verified" messages
   *   containing this header relieve the user of having to
   *   configure and trust the domain of the web page never 
   *   to exploit behind his firewall, because the
   *   responsibility is assumed by the service.  If the
   *   service is not behind a firewall, then merely
   *   ignoring the packet is enough to free all
   *   users of this facility to call the server whatever
   *   their security configurations for unverified calls
   *   may be.  But it only works with services that 
   *   accept these verifySource headers.
   * <p>It is possible for a user to disable even verified
   *   SOAP calls, but this is not the default setting.
   */
  /* attribute boolean verifySourceHeader; */
  NS_IMETHOD GetVerifySourceHeader(PRBool *aVerifySourceHeader) = 0;
  NS_IMETHOD SetVerifySourceHeader(PRBool aVerifySourceHeader) = 0;

  /**
   * Synchronously invoke the call. The method returns only when 
   * we receive a response (or an error occurs).  The
   * <code>transportURI</code> must have been set, the 
   * parameter list (even if empty) must have been encoded,
   * and the transportURI must use some known protocol.  A
   * synchronous call assumes that there will be exactly one
   * response per call.
   *
   * If not, an error is returned in the status of the response.
   *
   * @returns The SOAP response
   */
  /* nsISOAPResponse invoke (); */
  NS_IMETHOD Invoke(nsISOAPResponse **_retval) = 0;

  /**
   * Asynchronously invoke the call. At this point, the document
   * rooted by the Envelope element is encoded to form the body
   * of the SOAP message. The method returns immediately, and the
   * listener is invoked when we eventually receive a response
   * (or error or successful completion). The
   * <code>transportURI</code> must have been set, the 
   * parameter list (even if empty) must have been encoded,
   * and the transportURI must use some known protocol.
   *
   * If not, an error is returned in the status of the response.
   *
   * @param aListener Handler to be invoked asynchronously after the
   *  response is recieved.  Should be null if no response is
   *  expected.
   */
  /* nsISOAPCallCompletion asyncInvoke (in nsISOAPResponseListener aListener); */
  NS_IMETHOD AsyncInvoke(nsISOAPResponseListener *aListener, nsISOAPCallCompletion **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPCALL \
  NS_IMETHOD GetTransportURI(nsAString & aTransportURI); \
  NS_IMETHOD SetTransportURI(const nsAString & aTransportURI); \
  NS_IMETHOD GetVerifySourceHeader(PRBool *aVerifySourceHeader); \
  NS_IMETHOD SetVerifySourceHeader(PRBool aVerifySourceHeader); \
  NS_IMETHOD Invoke(nsISOAPResponse **_retval); \
  NS_IMETHOD AsyncInvoke(nsISOAPResponseListener *aListener, nsISOAPCallCompletion **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPCALL(_to) \
  NS_IMETHOD GetTransportURI(nsAString & aTransportURI) { return _to GetTransportURI(aTransportURI); } \
  NS_IMETHOD SetTransportURI(const nsAString & aTransportURI) { return _to SetTransportURI(aTransportURI); } \
  NS_IMETHOD GetVerifySourceHeader(PRBool *aVerifySourceHeader) { return _to GetVerifySourceHeader(aVerifySourceHeader); } \
  NS_IMETHOD SetVerifySourceHeader(PRBool aVerifySourceHeader) { return _to SetVerifySourceHeader(aVerifySourceHeader); } \
  NS_IMETHOD Invoke(nsISOAPResponse **_retval) { return _to Invoke(_retval); } \
  NS_IMETHOD AsyncInvoke(nsISOAPResponseListener *aListener, nsISOAPCallCompletion **_retval) { return _to AsyncInvoke(aListener, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPCALL(_to) \
  NS_IMETHOD GetTransportURI(nsAString & aTransportURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTransportURI(aTransportURI); } \
  NS_IMETHOD SetTransportURI(const nsAString & aTransportURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetTransportURI(aTransportURI); } \
  NS_IMETHOD GetVerifySourceHeader(PRBool *aVerifySourceHeader) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetVerifySourceHeader(aVerifySourceHeader); } \
  NS_IMETHOD SetVerifySourceHeader(PRBool aVerifySourceHeader) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetVerifySourceHeader(aVerifySourceHeader); } \
  NS_IMETHOD Invoke(nsISOAPResponse **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Invoke(_retval); } \
  NS_IMETHOD AsyncInvoke(nsISOAPResponseListener *aListener, nsISOAPCallCompletion **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->AsyncInvoke(aListener, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPCall : public nsISOAPCall
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPCALL

  nsSOAPCall();

private:
  ~nsSOAPCall();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPCall, nsISOAPCall)

nsSOAPCall::nsSOAPCall()
{
  /* member initializers and constructor code */
}

nsSOAPCall::~nsSOAPCall()
{
  /* destructor code */
}

/* attribute AString transportURI; */
NS_IMETHODIMP nsSOAPCall::GetTransportURI(nsAString & aTransportURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPCall::SetTransportURI(const nsAString & aTransportURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean verifySourceHeader; */
NS_IMETHODIMP nsSOAPCall::GetVerifySourceHeader(PRBool *aVerifySourceHeader)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPCall::SetVerifySourceHeader(PRBool aVerifySourceHeader)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISOAPResponse invoke (); */
NS_IMETHODIMP nsSOAPCall::Invoke(nsISOAPResponse **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISOAPCallCompletion asyncInvoke (in nsISOAPResponseListener aListener); */
NS_IMETHODIMP nsSOAPCall::AsyncInvoke(nsISOAPResponseListener *aListener, nsISOAPCallCompletion **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SOAPCALL_CID                             \
{ /* 87d21ec0-539d-11d4-9a59-00104bdf5339 */        \
  0x87d21ec0, 0x539d, 0x11d4,                       \
 {0x9a, 0x59, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39} }
#define NS_SOAPCALL_CONTRACTID \
"@mozilla.org/xmlextras/soap/call;1"

#endif /* __gen_nsISOAPCall_h__ */
