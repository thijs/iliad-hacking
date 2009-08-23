/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPResponseListener.idl
 */

#ifndef __gen_nsISOAPResponseListener_h__
#define __gen_nsISOAPResponseListener_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISOAPCall; /* forward declaration */

class nsISOAPResponse; /* forward declaration */


/* starting interface:    nsISOAPResponseListener */
#define NS_ISOAPRESPONSELISTENER_IID_STR "99ec6692-535f-11d4-9a58-000064657374"

#define NS_ISOAPRESPONSELISTENER_IID \
  {0x99ec6692, 0x535f, 0x11d4, \
    { 0x9a, 0x58, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

/**
 * This interface represents a response handler to be invoked whenever
 * a response of a particular call is recieved and when no more
 * responses are expected.
 */
class NS_NO_VTABLE nsISOAPResponseListener : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPRESPONSELISTENER_IID)

  /**
   * This method is invoked when we receive an asynchronous response to 
   * a SOAP message. The listener is registered as part of the original
   * asynchronous call invocation. 
   *
   * @param aResponse The decoded version of the response.  If an
   *   error occurred transmitting the response, the status field
   *   of the response will contain an error code.  The last call
   *   to the listener may contain a null response, which should
   *   only be interpreted as an error if your call expected more 
   *   results than it got.  If the service or the transport
   *   do not know whether to expect more results, then setting
   *   the last parameter true may only be possible after the
   *   last response has already been delivered.
   *
   * @param aLast True if this is the last call to the listener.
   *
   * @return True to make this the last call to the listener, even
   *   if last was not true.  Calls which expect a single response
   *   should return true upon receiving that response to avoid
   *   possibly recieving another callback with a null response
   *   indicating that the last response was already sent.
   */
  /* boolean handleResponse (in nsISOAPResponse aResponse, in nsISOAPCall aCall, in unsigned long status, in boolean aLast); */
  NS_IMETHOD HandleResponse(nsISOAPResponse *aResponse, nsISOAPCall *aCall, PRUint32 status, PRBool aLast, PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPRESPONSELISTENER \
  NS_IMETHOD HandleResponse(nsISOAPResponse *aResponse, nsISOAPCall *aCall, PRUint32 status, PRBool aLast, PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPRESPONSELISTENER(_to) \
  NS_IMETHOD HandleResponse(nsISOAPResponse *aResponse, nsISOAPCall *aCall, PRUint32 status, PRBool aLast, PRBool *_retval) { return _to HandleResponse(aResponse, aCall, status, aLast, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPRESPONSELISTENER(_to) \
  NS_IMETHOD HandleResponse(nsISOAPResponse *aResponse, nsISOAPCall *aCall, PRUint32 status, PRBool aLast, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleResponse(aResponse, aCall, status, aLast, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPResponseListener : public nsISOAPResponseListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPRESPONSELISTENER

  nsSOAPResponseListener();

private:
  ~nsSOAPResponseListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPResponseListener, nsISOAPResponseListener)

nsSOAPResponseListener::nsSOAPResponseListener()
{
  /* member initializers and constructor code */
}

nsSOAPResponseListener::~nsSOAPResponseListener()
{
  /* destructor code */
}

/* boolean handleResponse (in nsISOAPResponse aResponse, in nsISOAPCall aCall, in unsigned long status, in boolean aLast); */
NS_IMETHODIMP nsSOAPResponseListener::HandleResponse(nsISOAPResponse *aResponse, nsISOAPCall *aCall, PRUint32 status, PRBool aLast, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISOAPResponseListener_h__ */
