/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIWSDL.idl
 */

#ifndef __gen_nsIWSDL_h__
#define __gen_nsIWSDL_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsISchema_h__
#include "nsISchema.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIWSDLBinding; /* forward declaration */

class nsIWSDLPort; /* forward declaration */

class nsIWSDLOperation; /* forward declaration */

class nsIWSDLMessage; /* forward declaration */

class nsIWSDLPart; /* forward declaration */

class nsIDOMElement; /* forward declaration */


/* starting interface:    nsIWSDLPort */
#define NS_IWSDLPORT_IID_STR "0458dac1-65de-11d5-9b42-00104bdf5339"

#define NS_IWSDLPORT_IID \
  {0x0458dac1, 0x65de, 0x11d5, \
    { 0x9b, 0x42, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39 }}

class NS_NO_VTABLE nsIWSDLPort : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWSDLPORT_IID)

  /* readonly attribute AString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;

  /* readonly attribute nsIDOMElement documentation; */
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) = 0;

  /* readonly attribute nsIWSDLBinding binding; */
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) = 0;

  /* readonly attribute PRUint32 operationCount; */
  NS_IMETHOD GetOperationCount(PRUint32 *aOperationCount) = 0;

  /* nsIWSDLOperation getOperation (in PRUint32 index); */
  NS_IMETHOD GetOperation(PRUint32 index, nsIWSDLOperation **_retval) = 0;

  /* nsIWSDLOperation getOperationByName (in AString name); */
  NS_IMETHOD GetOperationByName(const nsAString & name, nsIWSDLOperation **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWSDLPORT \
  NS_IMETHOD GetName(nsAString & aName); \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation); \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding); \
  NS_IMETHOD GetOperationCount(PRUint32 *aOperationCount); \
  NS_IMETHOD GetOperation(PRUint32 index, nsIWSDLOperation **_retval); \
  NS_IMETHOD GetOperationByName(const nsAString & name, nsIWSDLOperation **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWSDLPORT(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) { return _to GetDocumentation(aDocumentation); } \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) { return _to GetBinding(aBinding); } \
  NS_IMETHOD GetOperationCount(PRUint32 *aOperationCount) { return _to GetOperationCount(aOperationCount); } \
  NS_IMETHOD GetOperation(PRUint32 index, nsIWSDLOperation **_retval) { return _to GetOperation(index, _retval); } \
  NS_IMETHOD GetOperationByName(const nsAString & name, nsIWSDLOperation **_retval) { return _to GetOperationByName(name, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWSDLPORT(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDocumentation(aDocumentation); } \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBinding(aBinding); } \
  NS_IMETHOD GetOperationCount(PRUint32 *aOperationCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOperationCount(aOperationCount); } \
  NS_IMETHOD GetOperation(PRUint32 index, nsIWSDLOperation **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOperation(index, _retval); } \
  NS_IMETHOD GetOperationByName(const nsAString & name, nsIWSDLOperation **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOperationByName(name, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWSDLPort : public nsIWSDLPort
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWSDLPORT

  nsWSDLPort();

private:
  ~nsWSDLPort();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWSDLPort, nsIWSDLPort)

nsWSDLPort::nsWSDLPort()
{
  /* member initializers and constructor code */
}

nsWSDLPort::~nsWSDLPort()
{
  /* destructor code */
}

/* readonly attribute AString name; */
NS_IMETHODIMP nsWSDLPort::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement documentation; */
NS_IMETHODIMP nsWSDLPort::GetDocumentation(nsIDOMElement * *aDocumentation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIWSDLBinding binding; */
NS_IMETHODIMP nsWSDLPort::GetBinding(nsIWSDLBinding * *aBinding)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 operationCount; */
NS_IMETHODIMP nsWSDLPort::GetOperationCount(PRUint32 *aOperationCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIWSDLOperation getOperation (in PRUint32 index); */
NS_IMETHODIMP nsWSDLPort::GetOperation(PRUint32 index, nsIWSDLOperation **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIWSDLOperation getOperationByName (in AString name); */
NS_IMETHODIMP nsWSDLPort::GetOperationByName(const nsAString & name, nsIWSDLOperation **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWSDLOperation */
#define NS_IWSDLOPERATION_IID_STR "0458dac2-65de-11d5-9b42-00104bdf5339"

#define NS_IWSDLOPERATION_IID \
  {0x0458dac2, 0x65de, 0x11d5, \
    { 0x9b, 0x42, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39 }}

class NS_NO_VTABLE nsIWSDLOperation : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWSDLOPERATION_IID)

  /* readonly attribute AString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;

  /* readonly attribute nsIDOMElement documentation; */
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) = 0;

  /* readonly attribute nsIWSDLBinding binding; */
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) = 0;

  /* readonly attribute nsIWSDLMessage input; */
  NS_IMETHOD GetInput(nsIWSDLMessage * *aInput) = 0;

  /* readonly attribute nsIWSDLMessage output; */
  NS_IMETHOD GetOutput(nsIWSDLMessage * *aOutput) = 0;

  /* readonly attribute PRUint32 faultCount; */
  NS_IMETHOD GetFaultCount(PRUint32 *aFaultCount) = 0;

  /* nsIWSDLMessage getFault (in PRUint32 index); */
  NS_IMETHOD GetFault(PRUint32 index, nsIWSDLMessage **_retval) = 0;

  /* readonly attribute PRUint32 parameterOrderCount; */
  NS_IMETHOD GetParameterOrderCount(PRUint32 *aParameterOrderCount) = 0;

  /* AString getParameter (in PRUint32 index); */
  NS_IMETHOD GetParameter(PRUint32 index, nsAString & _retval) = 0;

  /* PRUint32 getParameterIndex (in AString name); */
  NS_IMETHOD GetParameterIndex(const nsAString & name, PRUint32 *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWSDLOPERATION \
  NS_IMETHOD GetName(nsAString & aName); \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation); \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding); \
  NS_IMETHOD GetInput(nsIWSDLMessage * *aInput); \
  NS_IMETHOD GetOutput(nsIWSDLMessage * *aOutput); \
  NS_IMETHOD GetFaultCount(PRUint32 *aFaultCount); \
  NS_IMETHOD GetFault(PRUint32 index, nsIWSDLMessage **_retval); \
  NS_IMETHOD GetParameterOrderCount(PRUint32 *aParameterOrderCount); \
  NS_IMETHOD GetParameter(PRUint32 index, nsAString & _retval); \
  NS_IMETHOD GetParameterIndex(const nsAString & name, PRUint32 *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWSDLOPERATION(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) { return _to GetDocumentation(aDocumentation); } \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) { return _to GetBinding(aBinding); } \
  NS_IMETHOD GetInput(nsIWSDLMessage * *aInput) { return _to GetInput(aInput); } \
  NS_IMETHOD GetOutput(nsIWSDLMessage * *aOutput) { return _to GetOutput(aOutput); } \
  NS_IMETHOD GetFaultCount(PRUint32 *aFaultCount) { return _to GetFaultCount(aFaultCount); } \
  NS_IMETHOD GetFault(PRUint32 index, nsIWSDLMessage **_retval) { return _to GetFault(index, _retval); } \
  NS_IMETHOD GetParameterOrderCount(PRUint32 *aParameterOrderCount) { return _to GetParameterOrderCount(aParameterOrderCount); } \
  NS_IMETHOD GetParameter(PRUint32 index, nsAString & _retval) { return _to GetParameter(index, _retval); } \
  NS_IMETHOD GetParameterIndex(const nsAString & name, PRUint32 *_retval) { return _to GetParameterIndex(name, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWSDLOPERATION(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDocumentation(aDocumentation); } \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBinding(aBinding); } \
  NS_IMETHOD GetInput(nsIWSDLMessage * *aInput) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInput(aInput); } \
  NS_IMETHOD GetOutput(nsIWSDLMessage * *aOutput) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOutput(aOutput); } \
  NS_IMETHOD GetFaultCount(PRUint32 *aFaultCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFaultCount(aFaultCount); } \
  NS_IMETHOD GetFault(PRUint32 index, nsIWSDLMessage **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFault(index, _retval); } \
  NS_IMETHOD GetParameterOrderCount(PRUint32 *aParameterOrderCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParameterOrderCount(aParameterOrderCount); } \
  NS_IMETHOD GetParameter(PRUint32 index, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParameter(index, _retval); } \
  NS_IMETHOD GetParameterIndex(const nsAString & name, PRUint32 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParameterIndex(name, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWSDLOperation : public nsIWSDLOperation
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWSDLOPERATION

  nsWSDLOperation();

private:
  ~nsWSDLOperation();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWSDLOperation, nsIWSDLOperation)

nsWSDLOperation::nsWSDLOperation()
{
  /* member initializers and constructor code */
}

nsWSDLOperation::~nsWSDLOperation()
{
  /* destructor code */
}

/* readonly attribute AString name; */
NS_IMETHODIMP nsWSDLOperation::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement documentation; */
NS_IMETHODIMP nsWSDLOperation::GetDocumentation(nsIDOMElement * *aDocumentation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIWSDLBinding binding; */
NS_IMETHODIMP nsWSDLOperation::GetBinding(nsIWSDLBinding * *aBinding)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIWSDLMessage input; */
NS_IMETHODIMP nsWSDLOperation::GetInput(nsIWSDLMessage * *aInput)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIWSDLMessage output; */
NS_IMETHODIMP nsWSDLOperation::GetOutput(nsIWSDLMessage * *aOutput)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 faultCount; */
NS_IMETHODIMP nsWSDLOperation::GetFaultCount(PRUint32 *aFaultCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIWSDLMessage getFault (in PRUint32 index); */
NS_IMETHODIMP nsWSDLOperation::GetFault(PRUint32 index, nsIWSDLMessage **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 parameterOrderCount; */
NS_IMETHODIMP nsWSDLOperation::GetParameterOrderCount(PRUint32 *aParameterOrderCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getParameter (in PRUint32 index); */
NS_IMETHODIMP nsWSDLOperation::GetParameter(PRUint32 index, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint32 getParameterIndex (in AString name); */
NS_IMETHODIMP nsWSDLOperation::GetParameterIndex(const nsAString & name, PRUint32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWSDLMessage */
#define NS_IWSDLMESSAGE_IID_STR "0458dac3-65de-11d5-9b42-00104bdf5339"

#define NS_IWSDLMESSAGE_IID \
  {0x0458dac3, 0x65de, 0x11d5, \
    { 0x9b, 0x42, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39 }}

class NS_NO_VTABLE nsIWSDLMessage : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWSDLMESSAGE_IID)

  /* readonly attribute AString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;

  /* readonly attribute nsIDOMElement documentation; */
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) = 0;

  /* readonly attribute nsIWSDLBinding binding; */
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) = 0;

  /* readonly attribute PRUint32 partCount; */
  NS_IMETHOD GetPartCount(PRUint32 *aPartCount) = 0;

  /* nsIWSDLPart getPart (in PRUint32 index); */
  NS_IMETHOD GetPart(PRUint32 index, nsIWSDLPart **_retval) = 0;

  /* nsIWSDLPart getPartByName (in AString name); */
  NS_IMETHOD GetPartByName(const nsAString & name, nsIWSDLPart **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWSDLMESSAGE \
  NS_IMETHOD GetName(nsAString & aName); \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation); \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding); \
  NS_IMETHOD GetPartCount(PRUint32 *aPartCount); \
  NS_IMETHOD GetPart(PRUint32 index, nsIWSDLPart **_retval); \
  NS_IMETHOD GetPartByName(const nsAString & name, nsIWSDLPart **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWSDLMESSAGE(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) { return _to GetDocumentation(aDocumentation); } \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) { return _to GetBinding(aBinding); } \
  NS_IMETHOD GetPartCount(PRUint32 *aPartCount) { return _to GetPartCount(aPartCount); } \
  NS_IMETHOD GetPart(PRUint32 index, nsIWSDLPart **_retval) { return _to GetPart(index, _retval); } \
  NS_IMETHOD GetPartByName(const nsAString & name, nsIWSDLPart **_retval) { return _to GetPartByName(name, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWSDLMESSAGE(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDocumentation(aDocumentation); } \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBinding(aBinding); } \
  NS_IMETHOD GetPartCount(PRUint32 *aPartCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPartCount(aPartCount); } \
  NS_IMETHOD GetPart(PRUint32 index, nsIWSDLPart **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPart(index, _retval); } \
  NS_IMETHOD GetPartByName(const nsAString & name, nsIWSDLPart **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPartByName(name, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWSDLMessage : public nsIWSDLMessage
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWSDLMESSAGE

  nsWSDLMessage();

private:
  ~nsWSDLMessage();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWSDLMessage, nsIWSDLMessage)

nsWSDLMessage::nsWSDLMessage()
{
  /* member initializers and constructor code */
}

nsWSDLMessage::~nsWSDLMessage()
{
  /* destructor code */
}

/* readonly attribute AString name; */
NS_IMETHODIMP nsWSDLMessage::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement documentation; */
NS_IMETHODIMP nsWSDLMessage::GetDocumentation(nsIDOMElement * *aDocumentation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIWSDLBinding binding; */
NS_IMETHODIMP nsWSDLMessage::GetBinding(nsIWSDLBinding * *aBinding)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 partCount; */
NS_IMETHODIMP nsWSDLMessage::GetPartCount(PRUint32 *aPartCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIWSDLPart getPart (in PRUint32 index); */
NS_IMETHODIMP nsWSDLMessage::GetPart(PRUint32 index, nsIWSDLPart **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIWSDLPart getPartByName (in AString name); */
NS_IMETHODIMP nsWSDLMessage::GetPartByName(const nsAString & name, nsIWSDLPart **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWSDLPart */
#define NS_IWSDLPART_IID_STR "0458dac4-65de-11d5-9b42-00104bdf5339"

#define NS_IWSDLPART_IID \
  {0x0458dac4, 0x65de, 0x11d5, \
    { 0x9b, 0x42, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39 }}

class NS_NO_VTABLE nsIWSDLPart : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWSDLPART_IID)

  /* readonly attribute AString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;

  /* readonly attribute nsIWSDLBinding binding; */
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) = 0;

  /* readonly attribute AString type; */
  NS_IMETHOD GetType(nsAString & aType) = 0;

  /* readonly attribute AString elementName; */
  NS_IMETHOD GetElementName(nsAString & aElementName) = 0;

  /**
   * The schema component that corresponds to this part. If the
   * type attribute is used, this is a nsISchemaType. If the 
   * elementName attribute is used, it is a nsISchemaElement.
   */
  /* readonly attribute nsISchemaComponent schemaComponent; */
  NS_IMETHOD GetSchemaComponent(nsISchemaComponent * *aSchemaComponent) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWSDLPART \
  NS_IMETHOD GetName(nsAString & aName); \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding); \
  NS_IMETHOD GetType(nsAString & aType); \
  NS_IMETHOD GetElementName(nsAString & aElementName); \
  NS_IMETHOD GetSchemaComponent(nsISchemaComponent * *aSchemaComponent); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWSDLPART(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) { return _to GetBinding(aBinding); } \
  NS_IMETHOD GetType(nsAString & aType) { return _to GetType(aType); } \
  NS_IMETHOD GetElementName(nsAString & aElementName) { return _to GetElementName(aElementName); } \
  NS_IMETHOD GetSchemaComponent(nsISchemaComponent * *aSchemaComponent) { return _to GetSchemaComponent(aSchemaComponent); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWSDLPART(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetBinding(nsIWSDLBinding * *aBinding) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBinding(aBinding); } \
  NS_IMETHOD GetType(nsAString & aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_IMETHOD GetElementName(nsAString & aElementName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetElementName(aElementName); } \
  NS_IMETHOD GetSchemaComponent(nsISchemaComponent * *aSchemaComponent) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSchemaComponent(aSchemaComponent); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWSDLPart : public nsIWSDLPart
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWSDLPART

  nsWSDLPart();

private:
  ~nsWSDLPart();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWSDLPart, nsIWSDLPart)

nsWSDLPart::nsWSDLPart()
{
  /* member initializers and constructor code */
}

nsWSDLPart::~nsWSDLPart()
{
  /* destructor code */
}

/* readonly attribute AString name; */
NS_IMETHODIMP nsWSDLPart::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIWSDLBinding binding; */
NS_IMETHODIMP nsWSDLPart::GetBinding(nsIWSDLBinding * *aBinding)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString type; */
NS_IMETHODIMP nsWSDLPart::GetType(nsAString & aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString elementName; */
NS_IMETHODIMP nsWSDLPart::GetElementName(nsAString & aElementName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISchemaComponent schemaComponent; */
NS_IMETHODIMP nsWSDLPart::GetSchemaComponent(nsISchemaComponent * *aSchemaComponent)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWSDLBinding */
#define NS_IWSDLBINDING_IID_STR "0458dac0-65de-11d5-9b42-00104bdf5339"

#define NS_IWSDLBINDING_IID \
  {0x0458dac0, 0x65de, 0x11d5, \
    { 0x9b, 0x42, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39 }}

class NS_NO_VTABLE nsIWSDLBinding : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWSDLBINDING_IID)

  /* readonly attribute AString protocol; */
  NS_IMETHOD GetProtocol(nsAString & aProtocol) = 0;

  /* readonly attribute nsIDOMElement documentation; */
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWSDLBINDING \
  NS_IMETHOD GetProtocol(nsAString & aProtocol); \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWSDLBINDING(_to) \
  NS_IMETHOD GetProtocol(nsAString & aProtocol) { return _to GetProtocol(aProtocol); } \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) { return _to GetDocumentation(aDocumentation); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWSDLBINDING(_to) \
  NS_IMETHOD GetProtocol(nsAString & aProtocol) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetProtocol(aProtocol); } \
  NS_IMETHOD GetDocumentation(nsIDOMElement * *aDocumentation) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDocumentation(aDocumentation); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWSDLBinding : public nsIWSDLBinding
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWSDLBINDING

  nsWSDLBinding();

private:
  ~nsWSDLBinding();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWSDLBinding, nsIWSDLBinding)

nsWSDLBinding::nsWSDLBinding()
{
  /* member initializers and constructor code */
}

nsWSDLBinding::~nsWSDLBinding()
{
  /* destructor code */
}

/* readonly attribute AString protocol; */
NS_IMETHODIMP nsWSDLBinding::GetProtocol(nsAString & aProtocol)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement documentation; */
NS_IMETHODIMP nsWSDLBinding::GetDocumentation(nsIDOMElement * *aDocumentation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIWSDL_h__ */
