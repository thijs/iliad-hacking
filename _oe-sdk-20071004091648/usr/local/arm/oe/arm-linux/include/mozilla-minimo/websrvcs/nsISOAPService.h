/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPService.idl
 */

#ifndef __gen_nsISOAPService_h__
#define __gen_nsISOAPService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISOAPMessage; /* forward declaration */

class nsISOAPResponseListener; /* forward declaration */


/* starting interface:    nsISOAPService */
#define NS_ISOAPSERVICE_IID_STR "9927fa40-1dd1-11b2-a8d1-857ad21b872c"

#define NS_ISOAPSERVICE_IID \
  {0x9927fa40, 0x1dd1, 0x11b2, \
    { 0xa8, 0xd1, 0x85, 0x7a, 0xd2, 0x1b, 0x87, 0x2c }}

/**
 * This interface describes a service which may be
 *   applied to incoming messages.  The service is
 *   responsible for determining whether the message
 *   is one that it should process and rejecting it
 *   if it is not.  Services may be chained.
 */
class NS_NO_VTABLE nsISOAPService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPSERVICE_IID)

  /**
   * Configuration object that may contain more info on the service
   */
  /* attribute nsISupports configuration; */
  NS_IMETHOD GetConfiguration(nsISupports * *aConfiguration) = 0;
  NS_IMETHOD SetConfiguration(nsISupports * aConfiguration) = 0;

  /**
   * Process an incoming message.
   *
   * @param aMessage message to be processed
   *
   * @param aListener listener to which to report results
   *
   * @return True if the message will be handled, false if
   *   it should be given to some other service or fail.
   *   In case of failure, a more detailed status will be
   *   recorded in the message.
   */
  /* boolean process (in nsISOAPMessage aMessage, in nsISOAPResponseListener aListener); */
  NS_IMETHOD Process(nsISOAPMessage *aMessage, nsISOAPResponseListener *aListener, PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPSERVICE \
  NS_IMETHOD GetConfiguration(nsISupports * *aConfiguration); \
  NS_IMETHOD SetConfiguration(nsISupports * aConfiguration); \
  NS_IMETHOD Process(nsISOAPMessage *aMessage, nsISOAPResponseListener *aListener, PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPSERVICE(_to) \
  NS_IMETHOD GetConfiguration(nsISupports * *aConfiguration) { return _to GetConfiguration(aConfiguration); } \
  NS_IMETHOD SetConfiguration(nsISupports * aConfiguration) { return _to SetConfiguration(aConfiguration); } \
  NS_IMETHOD Process(nsISOAPMessage *aMessage, nsISOAPResponseListener *aListener, PRBool *_retval) { return _to Process(aMessage, aListener, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPSERVICE(_to) \
  NS_IMETHOD GetConfiguration(nsISupports * *aConfiguration) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetConfiguration(aConfiguration); } \
  NS_IMETHOD SetConfiguration(nsISupports * aConfiguration) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetConfiguration(aConfiguration); } \
  NS_IMETHOD Process(nsISOAPMessage *aMessage, nsISOAPResponseListener *aListener, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Process(aMessage, aListener, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPService : public nsISOAPService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPSERVICE

  nsSOAPService();

private:
  ~nsSOAPService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPService, nsISOAPService)

nsSOAPService::nsSOAPService()
{
  /* member initializers and constructor code */
}

nsSOAPService::~nsSOAPService()
{
  /* destructor code */
}

/* attribute nsISupports configuration; */
NS_IMETHODIMP nsSOAPService::GetConfiguration(nsISupports * *aConfiguration)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPService::SetConfiguration(nsISupports * aConfiguration)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean process (in nsISOAPMessage aMessage, in nsISOAPResponseListener aListener); */
NS_IMETHODIMP nsSOAPService::Process(nsISOAPMessage *aMessage, nsISOAPResponseListener *aListener, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SOAPJSSERVICE_CID                            \
{ /* 26a41df2-1dd2-11b2-9f29-909e637afa0e */        \
  0x26a41df2, 0x1dd2, 0x11b2,                       \
 {0x9f, 0x29, 0x90, 0x9e, 0x63, 0x7a, 0xfa, 0x0e} }
#define NS_SOAPJSSERVICE_CONTRACTID \
"@mozilla.org/xmlextras/soap/jsservice;1"

#endif /* __gen_nsISOAPService_h__ */
