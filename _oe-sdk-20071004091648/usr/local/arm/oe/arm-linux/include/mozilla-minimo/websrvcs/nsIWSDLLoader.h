/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIWSDLLoader.idl
 */

#ifndef __gen_nsIWSDLLoader_h__
#define __gen_nsIWSDLLoader_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIWebServiceErrorHandler_h__
#include "nsIWebServiceErrorHandler.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIWSDLPort; /* forward declaration */

class nsIWSDLLoadListener; /* forward declaration */


/* starting interface:    nsIWSDLLoader */
#define NS_IWSDLLOADER_IID_STR "0458dac5-65de-11d5-9b42-00104bdf5339"

#define NS_IWSDLLOADER_IID \
  {0x0458dac5, 0x65de, 0x11d5, \
    { 0x9b, 0x42, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39 }}

class NS_NO_VTABLE nsIWSDLLoader : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWSDLLOADER_IID)

  /* nsIWSDLPort load (in AString wsdlURI, in AString portName); */
  NS_IMETHOD Load(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLPort **_retval) = 0;

  /* void loadAsync (in AString wsdlURI, in AString portName, in nsIWSDLLoadListener listener); */
  NS_IMETHOD LoadAsync(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLLoadListener *listener) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWSDLLOADER \
  NS_IMETHOD Load(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLPort **_retval); \
  NS_IMETHOD LoadAsync(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLLoadListener *listener); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWSDLLOADER(_to) \
  NS_IMETHOD Load(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLPort **_retval) { return _to Load(wsdlURI, portName, _retval); } \
  NS_IMETHOD LoadAsync(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLLoadListener *listener) { return _to LoadAsync(wsdlURI, portName, listener); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWSDLLOADER(_to) \
  NS_IMETHOD Load(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLPort **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Load(wsdlURI, portName, _retval); } \
  NS_IMETHOD LoadAsync(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLLoadListener *listener) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoadAsync(wsdlURI, portName, listener); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWSDLLoader : public nsIWSDLLoader
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWSDLLOADER

  nsWSDLLoader();

private:
  ~nsWSDLLoader();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWSDLLoader, nsIWSDLLoader)

nsWSDLLoader::nsWSDLLoader()
{
  /* member initializers and constructor code */
}

nsWSDLLoader::~nsWSDLLoader()
{
  /* destructor code */
}

/* nsIWSDLPort load (in AString wsdlURI, in AString portName); */
NS_IMETHODIMP nsWSDLLoader::Load(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLPort **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void loadAsync (in AString wsdlURI, in AString portName, in nsIWSDLLoadListener listener); */
NS_IMETHODIMP nsWSDLLoader::LoadAsync(const nsAString & wsdlURI, const nsAString & portName, nsIWSDLLoadListener *listener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWSDLLoadListener */
#define NS_IWSDLLOADLISTENER_IID_STR "c3681210-e191-11d8-949e-000393b6661a"

#define NS_IWSDLLOADLISTENER_IID \
  {0xc3681210, 0xe191, 0x11d8, \
    { 0x94, 0x9e, 0x00, 0x03, 0x93, 0xb6, 0x66, 0x1a }}

class NS_NO_VTABLE nsIWSDLLoadListener : public nsIWebServiceErrorHandler {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWSDLLOADLISTENER_IID)

  /* void onLoad (in nsIWSDLPort port); */
  NS_IMETHOD OnLoad(nsIWSDLPort *port) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWSDLLOADLISTENER \
  NS_IMETHOD OnLoad(nsIWSDLPort *port); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWSDLLOADLISTENER(_to) \
  NS_IMETHOD OnLoad(nsIWSDLPort *port) { return _to OnLoad(port); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWSDLLOADLISTENER(_to) \
  NS_IMETHOD OnLoad(nsIWSDLPort *port) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnLoad(port); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWSDLLoadListener : public nsIWSDLLoadListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWSDLLOADLISTENER

  nsWSDLLoadListener();

private:
  ~nsWSDLLoadListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWSDLLoadListener, nsIWSDLLoadListener)

nsWSDLLoadListener::nsWSDLLoadListener()
{
  /* member initializers and constructor code */
}

nsWSDLLoadListener::~nsWSDLLoadListener()
{
  /* destructor code */
}

/* void onLoad (in nsIWSDLPort port); */
NS_IMETHODIMP nsWSDLLoadListener::OnLoad(nsIWSDLPort *port)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_WSDLLOADER_CID                             \
{ /* 0a5af577-a61e-4492-ba0e-dd3c7b657e18 */          \
 0x0a5af577, 0xa61e, 0x4492,                          \
 {0xba, 0x0e, 0xdd, 0x3c, 0x7b, 0x65, 0x7e, 0x18}}
#define NS_WSDLLOADER_CONTRACTID "@mozilla.org/xmlextras/wsdl/wsdlloader;1"
#define NS_ERROR_WSDL_NOT_WSDL_ELEMENT          NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 1)
#define NS_ERROR_WSDL_SCHEMA_PROCESSING_ERROR   NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 2)
#define NS_ERROR_WSDL_BINDING_NOT_FOUND         NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 3)
#define NS_ERROR_WSDL_UNKNOWN_SCHEMA_COMPONENT  NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 4)
#define NS_ERROR_WSDL_UNKNOWN_WSDL_COMPONENT    NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 5)
#define NS_ERROR_WSDL_LOADING_ERROR             NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 6)
#define NS_ERROR_WSDL_RECURSIVE_IMPORT          NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 7)
#define NS_ERROR_WSDL_NOT_ENABLED               NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 8)

#endif /* __gen_nsIWSDLLoader_h__ */
