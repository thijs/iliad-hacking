/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/modules/plugin/base/public/nsIHTTPHeaderListener.idl
 */

#ifndef __gen_nsIHTTPHeaderListener_h__
#define __gen_nsIHTTPHeaderListener_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIHTTPHeaderListener */
#define NS_IHTTPHEADERLISTENER_IID_STR "8b246748-1dd2-11b2-9512-9dc84a95fc2f"

#define NS_IHTTPHEADERLISTENER_IID \
  {0x8b246748, 0x1dd2, 0x11b2, \
    { 0x95, 0x12, 0x9d, 0xc8, 0x4a, 0x95, 0xfc, 0x2f }}

/**
 * The nsIHTTPHeaderListener interface allows plugin authors to
 * access HTTP Response headers after issuing an
 * nsIPluginManager::{GetURL,PostURL}() call. <P>

 * IMPORTANT NOTE: The plugin author must provide an instance to
 * {GetURL,PostURL}() that implements both nsIPluginStreamListener and
 * nsIHTTPHeaderListener.  This instance is passed in through
 * {GetURL,PostURL}()'s streamListener parameter.  The browser will then
 * QI thi streamListener to see if it implements
 * nsIHTTPHeaderListener.
 */
class NS_NO_VTABLE nsIHTTPHeaderListener : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IHTTPHEADERLISTENER_IID)

  /**
   * Called for each HTTP Response header.
   * NOTE: You must copy the values of the params.  
   */
  /* void newResponseHeader (in string headerName, in string headerValue); */
  NS_IMETHOD NewResponseHeader(const char *headerName, const char *headerValue) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHTTPHEADERLISTENER \
  NS_IMETHOD NewResponseHeader(const char *headerName, const char *headerValue); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHTTPHEADERLISTENER(_to) \
  NS_IMETHOD NewResponseHeader(const char *headerName, const char *headerValue) { return _to NewResponseHeader(headerName, headerValue); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHTTPHEADERLISTENER(_to) \
  NS_IMETHOD NewResponseHeader(const char *headerName, const char *headerValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->NewResponseHeader(headerName, headerValue); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHTTPHeaderListener : public nsIHTTPHeaderListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHTTPHEADERLISTENER

  nsHTTPHeaderListener();

private:
  ~nsHTTPHeaderListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHTTPHeaderListener, nsIHTTPHeaderListener)

nsHTTPHeaderListener::nsHTTPHeaderListener()
{
  /* member initializers and constructor code */
}

nsHTTPHeaderListener::~nsHTTPHeaderListener()
{
  /* destructor code */
}

/* void newResponseHeader (in string headerName, in string headerValue); */
NS_IMETHODIMP nsHTTPHeaderListener::NewResponseHeader(const char *headerName, const char *headerValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIHTTPHeaderListener_h__ */
