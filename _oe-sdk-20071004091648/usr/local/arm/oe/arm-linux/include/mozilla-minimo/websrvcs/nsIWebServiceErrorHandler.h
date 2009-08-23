/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIWebServiceErrorHandler.idl
 */

#ifndef __gen_nsIWebServiceErrorHandler_h__
#define __gen_nsIWebServiceErrorHandler_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIWebServiceErrorHandler */
#define NS_IWEBSERVICEERRORHANDLER_IID_STR "068e20e0-df59-11d8-869f-000393b6661a"

#define NS_IWEBSERVICEERRORHANDLER_IID \
  {0x068e20e0, 0xdf59, 0x11d8, \
    { 0x86, 0x9f, 0x00, 0x03, 0x93, 0xb6, 0x66, 0x1a }}

class NS_NO_VTABLE nsIWebServiceErrorHandler : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWEBSERVICEERRORHANDLER_IID)

  /* void onError (in nsresult status, in AString statusMessage); */
  NS_IMETHOD OnError(nsresult status, const nsAString & statusMessage) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEBSERVICEERRORHANDLER \
  NS_IMETHOD OnError(nsresult status, const nsAString & statusMessage); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEBSERVICEERRORHANDLER(_to) \
  NS_IMETHOD OnError(nsresult status, const nsAString & statusMessage) { return _to OnError(status, statusMessage); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEBSERVICEERRORHANDLER(_to) \
  NS_IMETHOD OnError(nsresult status, const nsAString & statusMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnError(status, statusMessage); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWebServiceErrorHandler : public nsIWebServiceErrorHandler
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWEBSERVICEERRORHANDLER

  nsWebServiceErrorHandler();

private:
  ~nsWebServiceErrorHandler();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWebServiceErrorHandler, nsIWebServiceErrorHandler)

nsWebServiceErrorHandler::nsWebServiceErrorHandler()
{
  /* member initializers and constructor code */
}

nsWebServiceErrorHandler::~nsWebServiceErrorHandler()
{
  /* destructor code */
}

/* void onError (in nsresult status, in AString statusMessage); */
NS_IMETHODIMP nsWebServiceErrorHandler::OnError(nsresult status, const nsAString & statusMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIWebServiceErrorHandler_h__ */
