/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/webshell/public/nsIDocumentLoaderFactory.idl
 */

#ifndef __gen_nsIDocumentLoaderFactory_h__
#define __gen_nsIDocumentLoaderFactory_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIChannel; /* forward declaration */

class nsIContentViewer; /* forward declaration */

class nsIStreamListener; /* forward declaration */

class nsIDocument; /* forward declaration */

class nsILoadGroup; /* forward declaration */


/* starting interface:    nsIDocumentLoaderFactory */
#define NS_IDOCUMENTLOADERFACTORY_IID_STR "df15f850-5d98-11d4-9f4d-0010a4053fd0"

#define NS_IDOCUMENTLOADERFACTORY_IID \
  {0xdf15f850, 0x5d98, 0x11d4, \
    { 0x9f, 0x4d, 0x00, 0x10, 0xa4, 0x05, 0x3f, 0xd0 }}

/**
 * To get a component that implements nsIDocumentLoaderFactory
 * for a given mimetype, use nsICategoryManager to find an entry
 * with the mimetype as its name in the category "Gecko-Content-Viewers".
 * The value of the entry is the contractid of the component.
 * The component is a service, so use GetService, not CreateInstance to get it.
 */
class NS_NO_VTABLE nsIDocumentLoaderFactory : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IDOCUMENTLOADERFACTORY_IID)

  /* nsIContentViewer createInstance (in string aCommand, in nsIChannel aChannel, in nsILoadGroup aLoadGroup, in string aContentType, in nsISupports aContainer, in nsISupports aExtraInfo, out nsIStreamListener aDocListenerResult); */
  NS_IMETHOD CreateInstance(const char *aCommand, nsIChannel *aChannel, nsILoadGroup *aLoadGroup, const char *aContentType, nsISupports *aContainer, nsISupports *aExtraInfo, nsIStreamListener **aDocListenerResult, nsIContentViewer **_retval) = 0;

  /* nsIContentViewer createInstanceForDocument (in nsISupports aContainer, in nsIDocument aDocument, in string aCommand); */
  NS_IMETHOD CreateInstanceForDocument(nsISupports *aContainer, nsIDocument *aDocument, const char *aCommand, nsIContentViewer **_retval) = 0;

  /* nsIDocument createBlankDocument (in nsILoadGroup aLoadGroup); */
  NS_IMETHOD CreateBlankDocument(nsILoadGroup *aLoadGroup, nsIDocument **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOCUMENTLOADERFACTORY \
  NS_IMETHOD CreateInstance(const char *aCommand, nsIChannel *aChannel, nsILoadGroup *aLoadGroup, const char *aContentType, nsISupports *aContainer, nsISupports *aExtraInfo, nsIStreamListener **aDocListenerResult, nsIContentViewer **_retval); \
  NS_IMETHOD CreateInstanceForDocument(nsISupports *aContainer, nsIDocument *aDocument, const char *aCommand, nsIContentViewer **_retval); \
  NS_IMETHOD CreateBlankDocument(nsILoadGroup *aLoadGroup, nsIDocument **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOCUMENTLOADERFACTORY(_to) \
  NS_IMETHOD CreateInstance(const char *aCommand, nsIChannel *aChannel, nsILoadGroup *aLoadGroup, const char *aContentType, nsISupports *aContainer, nsISupports *aExtraInfo, nsIStreamListener **aDocListenerResult, nsIContentViewer **_retval) { return _to CreateInstance(aCommand, aChannel, aLoadGroup, aContentType, aContainer, aExtraInfo, aDocListenerResult, _retval); } \
  NS_IMETHOD CreateInstanceForDocument(nsISupports *aContainer, nsIDocument *aDocument, const char *aCommand, nsIContentViewer **_retval) { return _to CreateInstanceForDocument(aContainer, aDocument, aCommand, _retval); } \
  NS_IMETHOD CreateBlankDocument(nsILoadGroup *aLoadGroup, nsIDocument **_retval) { return _to CreateBlankDocument(aLoadGroup, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOCUMENTLOADERFACTORY(_to) \
  NS_IMETHOD CreateInstance(const char *aCommand, nsIChannel *aChannel, nsILoadGroup *aLoadGroup, const char *aContentType, nsISupports *aContainer, nsISupports *aExtraInfo, nsIStreamListener **aDocListenerResult, nsIContentViewer **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateInstance(aCommand, aChannel, aLoadGroup, aContentType, aContainer, aExtraInfo, aDocListenerResult, _retval); } \
  NS_IMETHOD CreateInstanceForDocument(nsISupports *aContainer, nsIDocument *aDocument, const char *aCommand, nsIContentViewer **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateInstanceForDocument(aContainer, aDocument, aCommand, _retval); } \
  NS_IMETHOD CreateBlankDocument(nsILoadGroup *aLoadGroup, nsIDocument **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateBlankDocument(aLoadGroup, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDocumentLoaderFactory : public nsIDocumentLoaderFactory
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOCUMENTLOADERFACTORY

  nsDocumentLoaderFactory();

private:
  ~nsDocumentLoaderFactory();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDocumentLoaderFactory, nsIDocumentLoaderFactory)

nsDocumentLoaderFactory::nsDocumentLoaderFactory()
{
  /* member initializers and constructor code */
}

nsDocumentLoaderFactory::~nsDocumentLoaderFactory()
{
  /* destructor code */
}

/* nsIContentViewer createInstance (in string aCommand, in nsIChannel aChannel, in nsILoadGroup aLoadGroup, in string aContentType, in nsISupports aContainer, in nsISupports aExtraInfo, out nsIStreamListener aDocListenerResult); */
NS_IMETHODIMP nsDocumentLoaderFactory::CreateInstance(const char *aCommand, nsIChannel *aChannel, nsILoadGroup *aLoadGroup, const char *aContentType, nsISupports *aContainer, nsISupports *aExtraInfo, nsIStreamListener **aDocListenerResult, nsIContentViewer **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIContentViewer createInstanceForDocument (in nsISupports aContainer, in nsIDocument aDocument, in string aCommand); */
NS_IMETHODIMP nsDocumentLoaderFactory::CreateInstanceForDocument(nsISupports *aContainer, nsIDocument *aDocument, const char *aCommand, nsIContentViewer **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDocument createBlankDocument (in nsILoadGroup aLoadGroup); */
NS_IMETHODIMP nsDocumentLoaderFactory::CreateBlankDocument(nsILoadGroup *aLoadGroup, nsIDocument **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDocumentLoaderFactory_h__ */
