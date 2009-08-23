/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISchemaLoader.idl
 */

#ifndef __gen_nsISchemaLoader_h__
#define __gen_nsISchemaLoader_h__


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
class nsISchema; /* forward declaration */

class nsISchemaType; /* forward declaration */

class nsIDOMElement; /* forward declaration */

class nsISchemaLoadListener; /* forward declaration */


/* starting interface:    nsISchemaLoader */
#define NS_ISCHEMALOADER_IID_STR "9b2f0b4a-8f00-4a78-961a-7e84ed49b0b6"

#define NS_ISCHEMALOADER_IID \
  {0x9b2f0b4a, 0x8f00, 0x4a78, \
    { 0x96, 0x1a, 0x7e, 0x84, 0xed, 0x49, 0xb0, 0xb6 }}

class NS_NO_VTABLE nsISchemaLoader : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMALOADER_IID)

  /* nsISchema load (in AString schemaURI); */
  NS_IMETHOD Load(const nsAString & schemaURI, nsISchema **_retval) = 0;

  /* void loadAsync (in AString schemaURI, in nsISchemaLoadListener listener); */
  NS_IMETHOD LoadAsync(const nsAString & schemaURI, nsISchemaLoadListener *listener) = 0;

  /* nsISchema processSchemaElement (in nsIDOMElement element, in nsIWebServiceErrorHandler aErrorHandler); */
  NS_IMETHOD ProcessSchemaElement(nsIDOMElement *element, nsIWebServiceErrorHandler *aErrorHandler, nsISchema **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMALOADER \
  NS_IMETHOD Load(const nsAString & schemaURI, nsISchema **_retval); \
  NS_IMETHOD LoadAsync(const nsAString & schemaURI, nsISchemaLoadListener *listener); \
  NS_IMETHOD ProcessSchemaElement(nsIDOMElement *element, nsIWebServiceErrorHandler *aErrorHandler, nsISchema **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMALOADER(_to) \
  NS_IMETHOD Load(const nsAString & schemaURI, nsISchema **_retval) { return _to Load(schemaURI, _retval); } \
  NS_IMETHOD LoadAsync(const nsAString & schemaURI, nsISchemaLoadListener *listener) { return _to LoadAsync(schemaURI, listener); } \
  NS_IMETHOD ProcessSchemaElement(nsIDOMElement *element, nsIWebServiceErrorHandler *aErrorHandler, nsISchema **_retval) { return _to ProcessSchemaElement(element, aErrorHandler, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMALOADER(_to) \
  NS_IMETHOD Load(const nsAString & schemaURI, nsISchema **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Load(schemaURI, _retval); } \
  NS_IMETHOD LoadAsync(const nsAString & schemaURI, nsISchemaLoadListener *listener) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoadAsync(schemaURI, listener); } \
  NS_IMETHOD ProcessSchemaElement(nsIDOMElement *element, nsIWebServiceErrorHandler *aErrorHandler, nsISchema **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->ProcessSchemaElement(element, aErrorHandler, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaLoader : public nsISchemaLoader
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMALOADER

  nsSchemaLoader();

private:
  ~nsSchemaLoader();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaLoader, nsISchemaLoader)

nsSchemaLoader::nsSchemaLoader()
{
  /* member initializers and constructor code */
}

nsSchemaLoader::~nsSchemaLoader()
{
  /* destructor code */
}

/* nsISchema load (in AString schemaURI); */
NS_IMETHODIMP nsSchemaLoader::Load(const nsAString & schemaURI, nsISchema **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void loadAsync (in AString schemaURI, in nsISchemaLoadListener listener); */
NS_IMETHODIMP nsSchemaLoader::LoadAsync(const nsAString & schemaURI, nsISchemaLoadListener *listener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchema processSchemaElement (in nsIDOMElement element, in nsIWebServiceErrorHandler aErrorHandler); */
NS_IMETHODIMP nsSchemaLoader::ProcessSchemaElement(nsIDOMElement *element, nsIWebServiceErrorHandler *aErrorHandler, nsISchema **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaLoadListener */
#define NS_ISCHEMALOADLISTENER_IID_STR "8d9aa9ce-e191-11d8-9f31-000393b6661a"

#define NS_ISCHEMALOADLISTENER_IID \
  {0x8d9aa9ce, 0xe191, 0x11d8, \
    { 0x9f, 0x31, 0x00, 0x03, 0x93, 0xb6, 0x66, 0x1a }}

class NS_NO_VTABLE nsISchemaLoadListener : public nsIWebServiceErrorHandler {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMALOADLISTENER_IID)

  /* void onLoad (in nsISchema schema); */
  NS_IMETHOD OnLoad(nsISchema *schema) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMALOADLISTENER \
  NS_IMETHOD OnLoad(nsISchema *schema); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMALOADLISTENER(_to) \
  NS_IMETHOD OnLoad(nsISchema *schema) { return _to OnLoad(schema); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMALOADLISTENER(_to) \
  NS_IMETHOD OnLoad(nsISchema *schema) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnLoad(schema); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaLoadListener : public nsISchemaLoadListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMALOADLISTENER

  nsSchemaLoadListener();

private:
  ~nsSchemaLoadListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaLoadListener, nsISchemaLoadListener)

nsSchemaLoadListener::nsSchemaLoadListener()
{
  /* member initializers and constructor code */
}

nsSchemaLoadListener::~nsSchemaLoadListener()
{
  /* destructor code */
}

/* void onLoad (in nsISchema schema); */
NS_IMETHODIMP nsSchemaLoadListener::OnLoad(nsISchema *schema)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SCHEMALOADER_CID                           \
{ /* 5adc10f0-74e1-11d5-9b49-00104bdf5339 */          \
 0x5adc10f0, 0x74e1, 0x11d5,                          \
{0x9b, 0x49, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39}}     
#define NS_SCHEMALOADER_CONTRACTID "@mozilla.org/xmlextras/schemas/schemaloader;1"
#define NS_BUILTINSCHEMACOLLECTION_CID                \
{ /* a83b0823-f10e-4235-9859-88a5abd2ed54 */          \
 0xa83b0823, 0xf10e, 0x4235,                          \
{0x98, 0x59, 0x88, 0xa5, 0xab, 0xd2, 0xed, 0x54}}
#define NS_BUILTINSCHEMACOLLECTION_CONTRACTID "@mozilla.org/xmlextras/schemas/builtinschemacollection;1"
#define NS_ERROR_SCHEMA_NOT_SCHEMA_ELEMENT NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 1)
#define NS_ERROR_SCHEMA_UNKNOWN_TARGET_NAMESPACE NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 2)
#define NS_ERROR_SCHEMA_UNKNOWN_TYPE NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 3)
#define NS_ERROR_SCHEMA_UNKNOWN_PREFIX NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 4)
#define NS_ERROR_SCHEMA_INVALID_STRUCTURE NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 5)
#define NS_ERROR_SCHEMA_INVALID_TYPE_USAGE NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 6)
#define NS_ERROR_SCHEMA_MISSING_TYPE NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 7)
#define NS_ERROR_SCHEMA_FACET_VALUE_ERROR NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 8)
#define NS_ERROR_SCHEMA_LOADING_ERROR NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_GENERAL, 9)

#endif /* __gen_nsISchemaLoader_h__ */
