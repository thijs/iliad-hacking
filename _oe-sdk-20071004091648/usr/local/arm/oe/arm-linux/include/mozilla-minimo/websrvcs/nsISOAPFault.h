/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPFault.idl
 */

#ifndef __gen_nsISOAPFault_h__
#define __gen_nsISOAPFault_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMElement; /* forward declaration */


/* starting interface:    nsISOAPFault */
#define NS_ISOAPFAULT_IID_STR "99ec6694-535f-11d4-9a58-000064657374"

#define NS_ISOAPFAULT_IID \
  {0x99ec6694, 0x535f, 0x11d4, \
    { 0x9a, 0x58, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

/**
 * This interface conveniently interprets information about a fault 
 * that has been returned in a response message.
 *
 */
class NS_NO_VTABLE nsISOAPFault : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPFAULT_IID)

  /**
   * The DOM element representing the fault in the response SOAP message.
   * This must be set for the rest of the interface to function correctly.
   */
  /* attribute nsIDOMElement element; */
  NS_IMETHOD GetElement(nsIDOMElement * *aElement) = 0;
  NS_IMETHOD SetElement(nsIDOMElement * aElement) = 0;

  /**
   * The namespaceURI of the fault code.
   */
  /* readonly attribute AString faultNamespaceURI; */
  NS_IMETHOD GetFaultNamespaceURI(nsAString & aFaultNamespaceURI) = 0;

  /**
   * The fault code
   */
  /* readonly attribute AString faultCode; */
  NS_IMETHOD GetFaultCode(nsAString & aFaultCode) = 0;

  /**
   * The fault string
   */
  /* readonly attribute AString faultString; */
  NS_IMETHOD GetFaultString(nsAString & aFaultString) = 0;

  /**
   * The fault actor if one was specified.
   */
  /* readonly attribute AString faultActor; */
  NS_IMETHOD GetFaultActor(nsAString & aFaultActor) = 0;

  /**
   * The DOM element representing the fault details
   */
  /* readonly attribute nsIDOMElement detail; */
  NS_IMETHOD GetDetail(nsIDOMElement * *aDetail) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPFAULT \
  NS_IMETHOD GetElement(nsIDOMElement * *aElement); \
  NS_IMETHOD SetElement(nsIDOMElement * aElement); \
  NS_IMETHOD GetFaultNamespaceURI(nsAString & aFaultNamespaceURI); \
  NS_IMETHOD GetFaultCode(nsAString & aFaultCode); \
  NS_IMETHOD GetFaultString(nsAString & aFaultString); \
  NS_IMETHOD GetFaultActor(nsAString & aFaultActor); \
  NS_IMETHOD GetDetail(nsIDOMElement * *aDetail); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPFAULT(_to) \
  NS_IMETHOD GetElement(nsIDOMElement * *aElement) { return _to GetElement(aElement); } \
  NS_IMETHOD SetElement(nsIDOMElement * aElement) { return _to SetElement(aElement); } \
  NS_IMETHOD GetFaultNamespaceURI(nsAString & aFaultNamespaceURI) { return _to GetFaultNamespaceURI(aFaultNamespaceURI); } \
  NS_IMETHOD GetFaultCode(nsAString & aFaultCode) { return _to GetFaultCode(aFaultCode); } \
  NS_IMETHOD GetFaultString(nsAString & aFaultString) { return _to GetFaultString(aFaultString); } \
  NS_IMETHOD GetFaultActor(nsAString & aFaultActor) { return _to GetFaultActor(aFaultActor); } \
  NS_IMETHOD GetDetail(nsIDOMElement * *aDetail) { return _to GetDetail(aDetail); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPFAULT(_to) \
  NS_IMETHOD GetElement(nsIDOMElement * *aElement) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetElement(aElement); } \
  NS_IMETHOD SetElement(nsIDOMElement * aElement) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetElement(aElement); } \
  NS_IMETHOD GetFaultNamespaceURI(nsAString & aFaultNamespaceURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFaultNamespaceURI(aFaultNamespaceURI); } \
  NS_IMETHOD GetFaultCode(nsAString & aFaultCode) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFaultCode(aFaultCode); } \
  NS_IMETHOD GetFaultString(nsAString & aFaultString) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFaultString(aFaultString); } \
  NS_IMETHOD GetFaultActor(nsAString & aFaultActor) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFaultActor(aFaultActor); } \
  NS_IMETHOD GetDetail(nsIDOMElement * *aDetail) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDetail(aDetail); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPFault : public nsISOAPFault
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPFAULT

  nsSOAPFault();

private:
  ~nsSOAPFault();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPFault, nsISOAPFault)

nsSOAPFault::nsSOAPFault()
{
  /* member initializers and constructor code */
}

nsSOAPFault::~nsSOAPFault()
{
  /* destructor code */
}

/* attribute nsIDOMElement element; */
NS_IMETHODIMP nsSOAPFault::GetElement(nsIDOMElement * *aElement)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPFault::SetElement(nsIDOMElement * aElement)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString faultNamespaceURI; */
NS_IMETHODIMP nsSOAPFault::GetFaultNamespaceURI(nsAString & aFaultNamespaceURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString faultCode; */
NS_IMETHODIMP nsSOAPFault::GetFaultCode(nsAString & aFaultCode)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString faultString; */
NS_IMETHODIMP nsSOAPFault::GetFaultString(nsAString & aFaultString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString faultActor; */
NS_IMETHODIMP nsSOAPFault::GetFaultActor(nsAString & aFaultActor)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement detail; */
NS_IMETHODIMP nsSOAPFault::GetDetail(nsIDOMElement * *aDetail)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SOAPFAULT_CID                            \
{ /* 87d21ec1-539d-11d4-9a59-00104bdf5339 */        \
  0x87d21ec1, 0x539d, 0x11d4,                       \
 {0x9a, 0x59, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39} }
#define NS_SOAPFAULT_CONTRACTID \
"@mozilla.org/xmlextras/soap/fault;1"

#endif /* __gen_nsISOAPFault_h__ */
