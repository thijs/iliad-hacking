/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPResponse.idl
 */

#ifndef __gen_nsISOAPResponse_h__
#define __gen_nsISOAPResponse_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsISOAPCall_h__
#include "nsISOAPCall.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISOAPParameter; /* forward declaration */

class nsISOAPFault; /* forward declaration */


/* starting interface:    nsISOAPResponse */
#define NS_ISOAPRESPONSE_IID_STR "99ec6691-535f-11d4-9a58-000064657374"

#define NS_ISOAPRESPONSE_IID \
  {0x99ec6691, 0x535f, 0x11d4, \
    { 0x9a, 0x58, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISOAPResponse : public nsISOAPMessage {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPRESPONSE_IID)

  /**
 * This is an extension of a message which contains extra functions
 * such as tracking, where appropriate, the original call that
 * produced the response message, identifying the fault, if any,
 * and supplying the return value.
 */
/**
   * The fault returned in the response, if one was generated. NULL
   * if there was no fault.  This does not rely on the response
   * parameters having been deserialized.
   */
  /* readonly attribute nsISOAPFault fault; */
  NS_IMETHOD GetFault(nsISOAPFault * *aFault) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPRESPONSE \
  NS_IMETHOD GetFault(nsISOAPFault * *aFault); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPRESPONSE(_to) \
  NS_IMETHOD GetFault(nsISOAPFault * *aFault) { return _to GetFault(aFault); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPRESPONSE(_to) \
  NS_IMETHOD GetFault(nsISOAPFault * *aFault) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFault(aFault); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPResponse : public nsISOAPResponse
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPRESPONSE

  nsSOAPResponse();

private:
  ~nsSOAPResponse();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPResponse, nsISOAPResponse)

nsSOAPResponse::nsSOAPResponse()
{
  /* member initializers and constructor code */
}

nsSOAPResponse::~nsSOAPResponse()
{
  /* destructor code */
}

/* readonly attribute nsISOAPFault fault; */
NS_IMETHODIMP nsSOAPResponse::GetFault(nsISOAPFault * *aFault)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SOAPRESPONSE_CID                         \
{ /* 87d21ec3-539d-11d4-9a59-00104bdf5339 */        \
  0x87d21ec3, 0x539d, 0x11d4,                       \
 {0x9a, 0x59, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39} }
#define NS_SOAPRESPONSE_CONTRACTID \
"@mozilla.org/xmlextras/soap/response;1"

#endif /* __gen_nsISOAPResponse_h__ */
