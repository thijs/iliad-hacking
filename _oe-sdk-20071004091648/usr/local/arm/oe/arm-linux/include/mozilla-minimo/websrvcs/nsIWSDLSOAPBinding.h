/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIWSDLSOAPBinding.idl
 */

#ifndef __gen_nsIWSDLSOAPBinding_h__
#define __gen_nsIWSDLSOAPBinding_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIWSDL_h__
#include "nsIWSDL.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIWSDLSOAPBinding */
#define NS_IWSDLSOAPBINDING_IID_STR "bb76b72e-d143-4738-804a-53b311466dd9"

#define NS_IWSDLSOAPBINDING_IID \
  {0xbb76b72e, 0xd143, 0x4738, \
    { 0x80, 0x4a, 0x53, 0xb3, 0x11, 0x46, 0x6d, 0xd9 }}

class NS_NO_VTABLE nsIWSDLSOAPBinding : public nsIWSDLBinding {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWSDLSOAPBINDING_IID)

  enum { STYLE_RPC = 1U };

  enum { STYLE_DOCUMENT = 2U };

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWSDLSOAPBINDING \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWSDLSOAPBINDING(_to) \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWSDLSOAPBINDING(_to) \

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWSDLSOAPBinding : public nsIWSDLSOAPBinding
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWSDLSOAPBINDING

  nsWSDLSOAPBinding();

private:
  ~nsWSDLSOAPBinding();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWSDLSOAPBinding, nsIWSDLSOAPBinding)

nsWSDLSOAPBinding::nsWSDLSOAPBinding()
{
  /* member initializers and constructor code */
}

nsWSDLSOAPBinding::~nsWSDLSOAPBinding()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISOAPPortBinding */
#define NS_ISOAPPORTBINDING_IID_STR "0458dac7-65de-11d5-9b42-00104bdf5339"

#define NS_ISOAPPORTBINDING_IID \
  {0x0458dac7, 0x65de, 0x11d5, \
    { 0x9b, 0x42, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39 }}

class NS_NO_VTABLE nsISOAPPortBinding : public nsIWSDLSOAPBinding {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPPORTBINDING_IID)

  enum { SOAP_VERSION_1_1 = 0U };

  enum { SOAP_VERSION_1_2 = 1U };

  enum { SOAP_VERSION_UNKNOWN = 32767U };

  /* readonly attribute AString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;

  /* readonly attribute AString address; */
  NS_IMETHOD GetAddress(nsAString & aAddress) = 0;

  /* readonly attribute unsigned short style; */
  NS_IMETHOD GetStyle(PRUint16 *aStyle) = 0;

  /* readonly attribute AString transport; */
  NS_IMETHOD GetTransport(nsAString & aTransport) = 0;

  /* readonly attribute unsigned short soapVersion; */
  NS_IMETHOD GetSoapVersion(PRUint16 *aSoapVersion) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPPORTBINDING \
  NS_IMETHOD GetName(nsAString & aName); \
  NS_IMETHOD GetAddress(nsAString & aAddress); \
  NS_IMETHOD GetStyle(PRUint16 *aStyle); \
  NS_IMETHOD GetTransport(nsAString & aTransport); \
  NS_IMETHOD GetSoapVersion(PRUint16 *aSoapVersion); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPPORTBINDING(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD GetAddress(nsAString & aAddress) { return _to GetAddress(aAddress); } \
  NS_IMETHOD GetStyle(PRUint16 *aStyle) { return _to GetStyle(aStyle); } \
  NS_IMETHOD GetTransport(nsAString & aTransport) { return _to GetTransport(aTransport); } \
  NS_IMETHOD GetSoapVersion(PRUint16 *aSoapVersion) { return _to GetSoapVersion(aSoapVersion); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPPORTBINDING(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetAddress(nsAString & aAddress) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAddress(aAddress); } \
  NS_IMETHOD GetStyle(PRUint16 *aStyle) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStyle(aStyle); } \
  NS_IMETHOD GetTransport(nsAString & aTransport) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTransport(aTransport); } \
  NS_IMETHOD GetSoapVersion(PRUint16 *aSoapVersion) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSoapVersion(aSoapVersion); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPPortBinding : public nsISOAPPortBinding
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPPORTBINDING

  nsSOAPPortBinding();

private:
  ~nsSOAPPortBinding();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPPortBinding, nsISOAPPortBinding)

nsSOAPPortBinding::nsSOAPPortBinding()
{
  /* member initializers and constructor code */
}

nsSOAPPortBinding::~nsSOAPPortBinding()
{
  /* destructor code */
}

/* readonly attribute AString name; */
NS_IMETHODIMP nsSOAPPortBinding::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString address; */
NS_IMETHODIMP nsSOAPPortBinding::GetAddress(nsAString & aAddress)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short style; */
NS_IMETHODIMP nsSOAPPortBinding::GetStyle(PRUint16 *aStyle)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString transport; */
NS_IMETHODIMP nsSOAPPortBinding::GetTransport(nsAString & aTransport)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short soapVersion; */
NS_IMETHODIMP nsSOAPPortBinding::GetSoapVersion(PRUint16 *aSoapVersion)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISOAPOperationBinding */
#define NS_ISOAPOPERATIONBINDING_IID_STR "0458dac8-65de-11d5-9b42-00104bdf5339"

#define NS_ISOAPOPERATIONBINDING_IID \
  {0x0458dac8, 0x65de, 0x11d5, \
    { 0x9b, 0x42, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39 }}

class NS_NO_VTABLE nsISOAPOperationBinding : public nsIWSDLSOAPBinding {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPOPERATIONBINDING_IID)

  /* readonly attribute unsigned short style; */
  NS_IMETHOD GetStyle(PRUint16 *aStyle) = 0;

  /* readonly attribute AString soapAction; */
  NS_IMETHOD GetSoapAction(nsAString & aSoapAction) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPOPERATIONBINDING \
  NS_IMETHOD GetStyle(PRUint16 *aStyle); \
  NS_IMETHOD GetSoapAction(nsAString & aSoapAction); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPOPERATIONBINDING(_to) \
  NS_IMETHOD GetStyle(PRUint16 *aStyle) { return _to GetStyle(aStyle); } \
  NS_IMETHOD GetSoapAction(nsAString & aSoapAction) { return _to GetSoapAction(aSoapAction); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPOPERATIONBINDING(_to) \
  NS_IMETHOD GetStyle(PRUint16 *aStyle) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStyle(aStyle); } \
  NS_IMETHOD GetSoapAction(nsAString & aSoapAction) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSoapAction(aSoapAction); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPOperationBinding : public nsISOAPOperationBinding
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPOPERATIONBINDING

  nsSOAPOperationBinding();

private:
  ~nsSOAPOperationBinding();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPOperationBinding, nsISOAPOperationBinding)

nsSOAPOperationBinding::nsSOAPOperationBinding()
{
  /* member initializers and constructor code */
}

nsSOAPOperationBinding::~nsSOAPOperationBinding()
{
  /* destructor code */
}

/* readonly attribute unsigned short style; */
NS_IMETHODIMP nsSOAPOperationBinding::GetStyle(PRUint16 *aStyle)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString soapAction; */
NS_IMETHODIMP nsSOAPOperationBinding::GetSoapAction(nsAString & aSoapAction)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISOAPMessageBinding */
#define NS_ISOAPMESSAGEBINDING_IID_STR "c3c4614c-301a-44af-ad70-936f9fd1ba5c"

#define NS_ISOAPMESSAGEBINDING_IID \
  {0xc3c4614c, 0x301a, 0x44af, \
    { 0xad, 0x70, 0x93, 0x6f, 0x9f, 0xd1, 0xba, 0x5c }}

class NS_NO_VTABLE nsISOAPMessageBinding : public nsIWSDLSOAPBinding {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPMESSAGEBINDING_IID)

  /* readonly attribute AString namespace; */
  NS_IMETHOD GetNamespace(nsAString & aNamespace) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPMESSAGEBINDING \
  NS_IMETHOD GetNamespace(nsAString & aNamespace); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPMESSAGEBINDING(_to) \
  NS_IMETHOD GetNamespace(nsAString & aNamespace) { return _to GetNamespace(aNamespace); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPMESSAGEBINDING(_to) \
  NS_IMETHOD GetNamespace(nsAString & aNamespace) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNamespace(aNamespace); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPMessageBinding : public nsISOAPMessageBinding
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPMESSAGEBINDING

  nsSOAPMessageBinding();

private:
  ~nsSOAPMessageBinding();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPMessageBinding, nsISOAPMessageBinding)

nsSOAPMessageBinding::nsSOAPMessageBinding()
{
  /* member initializers and constructor code */
}

nsSOAPMessageBinding::~nsSOAPMessageBinding()
{
  /* destructor code */
}

/* readonly attribute AString namespace; */
NS_IMETHODIMP nsSOAPMessageBinding::GetNamespace(nsAString & aNamespace)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISOAPPartBinding */
#define NS_ISOAPPARTBINDING_IID_STR "0458dac9-65de-11d5-9b42-00104bdf5339"

#define NS_ISOAPPARTBINDING_IID \
  {0x0458dac9, 0x65de, 0x11d5, \
    { 0x9b, 0x42, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39 }}

class NS_NO_VTABLE nsISOAPPartBinding : public nsIWSDLSOAPBinding {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPPARTBINDING_IID)

  enum { LOCATION_BODY = 1U };

  enum { LOCATION_HEADER = 2U };

  enum { LOCATION_FAULT = 3U };

  enum { USE_LITERAL = 1U };

  enum { USE_ENCODED = 2U };

  /* readonly attribute unsigned short location; */
  NS_IMETHOD GetLocation(PRUint16 *aLocation) = 0;

  /* readonly attribute unsigned short use; */
  NS_IMETHOD GetUse(PRUint16 *aUse) = 0;

  /* readonly attribute AString encodingStyle; */
  NS_IMETHOD GetEncodingStyle(nsAString & aEncodingStyle) = 0;

  /* readonly attribute AString namespace; */
  NS_IMETHOD GetNamespace(nsAString & aNamespace) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPPARTBINDING \
  NS_IMETHOD GetLocation(PRUint16 *aLocation); \
  NS_IMETHOD GetUse(PRUint16 *aUse); \
  NS_IMETHOD GetEncodingStyle(nsAString & aEncodingStyle); \
  NS_IMETHOD GetNamespace(nsAString & aNamespace); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPPARTBINDING(_to) \
  NS_IMETHOD GetLocation(PRUint16 *aLocation) { return _to GetLocation(aLocation); } \
  NS_IMETHOD GetUse(PRUint16 *aUse) { return _to GetUse(aUse); } \
  NS_IMETHOD GetEncodingStyle(nsAString & aEncodingStyle) { return _to GetEncodingStyle(aEncodingStyle); } \
  NS_IMETHOD GetNamespace(nsAString & aNamespace) { return _to GetNamespace(aNamespace); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPPARTBINDING(_to) \
  NS_IMETHOD GetLocation(PRUint16 *aLocation) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLocation(aLocation); } \
  NS_IMETHOD GetUse(PRUint16 *aUse) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetUse(aUse); } \
  NS_IMETHOD GetEncodingStyle(nsAString & aEncodingStyle) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEncodingStyle(aEncodingStyle); } \
  NS_IMETHOD GetNamespace(nsAString & aNamespace) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNamespace(aNamespace); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPPartBinding : public nsISOAPPartBinding
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPPARTBINDING

  nsSOAPPartBinding();

private:
  ~nsSOAPPartBinding();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPPartBinding, nsISOAPPartBinding)

nsSOAPPartBinding::nsSOAPPartBinding()
{
  /* member initializers and constructor code */
}

nsSOAPPartBinding::~nsSOAPPartBinding()
{
  /* destructor code */
}

/* readonly attribute unsigned short location; */
NS_IMETHODIMP nsSOAPPartBinding::GetLocation(PRUint16 *aLocation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short use; */
NS_IMETHODIMP nsSOAPPartBinding::GetUse(PRUint16 *aUse)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString encodingStyle; */
NS_IMETHODIMP nsSOAPPartBinding::GetEncodingStyle(nsAString & aEncodingStyle)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString namespace; */
NS_IMETHODIMP nsSOAPPartBinding::GetNamespace(nsAString & aNamespace)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIWSDLSOAPBinding_h__ */
