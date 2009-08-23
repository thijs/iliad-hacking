/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPParameter.idl
 */

#ifndef __gen_nsISOAPParameter_h__
#define __gen_nsISOAPParameter_h__


#ifndef __gen_nsISOAPBlock_h__
#include "nsISOAPBlock.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMElement; /* forward declaration */

class nsIVariant; /* forward declaration */

class nsISOAPEncoding; /* forward declaration */

class nsISchemaType; /* forward declaration */


/* starting interface:    nsISOAPParameter */
#define NS_ISOAPPARAMETER_IID_STR "99ec6690-535f-11d4-9a58-000064657374"

#define NS_ISOAPPARAMETER_IID \
  {0x99ec6690, 0x535f, 0x11d4, \
    { 0x9a, 0x58, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

/**
 * This interface encapsulates an arbitrary parameter to be used
 * by the soap serialization or protocol.  See the nsISOAPBlock
 * interface for particulars about how this interface works.
 */
class NS_NO_VTABLE nsISOAPParameter : public nsISOAPBlock {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPPARAMETER_IID)

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPPARAMETER \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPPARAMETER(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPPARAMETER(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPParameter : public nsISOAPParameter
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPPARAMETER

  nsSOAPParameter();

private:
  ~nsSOAPParameter();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPParameter, nsISOAPParameter)

nsSOAPParameter::nsSOAPParameter()
{
  /* member initializers and constructor code */
}

nsSOAPParameter::~nsSOAPParameter()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif

#define NS_SOAPPARAMETER_CID                        \
{ /* 87d21ec2-539d-11d4-9a59-00104bdf5339 */        \
  0x87d21ec2, 0x539d, 0x11d4,                       \
 {0x9a, 0x59, 0x00, 0x10, 0x4b, 0xdf, 0x53, 0x39} }
#define NS_SOAPPARAMETER_CONTRACTID \
"@mozilla.org/xmlextras/soap/parameter;1"

#endif /* __gen_nsISOAPParameter_h__ */
