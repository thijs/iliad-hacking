/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPTransportListener.idl
 */

#ifndef __gen_nsISOAPTransportListener_h__
#define __gen_nsISOAPTransportListener_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMDocument; /* forward declaration */

class nsISOAPMessage; /* forward declaration */


/* starting interface:    nsISOAPTransportListener */
#define NS_ISOAPTRANSPORTLISTENER_IID_STR "99ec6696-535f-11d4-9a58-000064657374"

#define NS_ISOAPTRANSPORTLISTENER_IID \
  {0x99ec6696, 0x535f, 0x11d4, \
    { 0x9a, 0x58, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

/**
 * This interface recieves control when an unsolicited transport
 * is recieved on a transport.
 */
class NS_NO_VTABLE nsISOAPTransportListener : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPTRANSPORTLISTENER_IID)

  /**
   * This method is invoked when an unsolicited message is
   *   recieved.  First all listeners are tried in the order declared
   *   with the capture flag set.  Then all listeners are tried in
   *   the reverse order declared with the capture flag clear.
   *
   * @param aMessage Actual message.
   *
   * @param aCapture True if the listener is being permitted to gain
   *   control before all later-added listeners.
   *
   * @return true if message is handled, false if it was not
   */
  /* boolean handleMessage (in nsISOAPMessage aMessage, in boolean aCapture); */
  NS_IMETHOD HandleMessage(nsISOAPMessage *aMessage, PRBool aCapture, PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPTRANSPORTLISTENER \
  NS_IMETHOD HandleMessage(nsISOAPMessage *aMessage, PRBool aCapture, PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPTRANSPORTLISTENER(_to) \
  NS_IMETHOD HandleMessage(nsISOAPMessage *aMessage, PRBool aCapture, PRBool *_retval) { return _to HandleMessage(aMessage, aCapture, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPTRANSPORTLISTENER(_to) \
  NS_IMETHOD HandleMessage(nsISOAPMessage *aMessage, PRBool aCapture, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleMessage(aMessage, aCapture, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPTransportListener : public nsISOAPTransportListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPTRANSPORTLISTENER

  nsSOAPTransportListener();

private:
  ~nsSOAPTransportListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPTransportListener, nsISOAPTransportListener)

nsSOAPTransportListener::nsSOAPTransportListener()
{
  /* member initializers and constructor code */
}

nsSOAPTransportListener::~nsSOAPTransportListener()
{
  /* destructor code */
}

/* boolean handleMessage (in nsISOAPMessage aMessage, in boolean aCapture); */
NS_IMETHODIMP nsSOAPTransportListener::HandleMessage(nsISOAPMessage *aMessage, PRBool aCapture, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISOAPTransportListener_h__ */
