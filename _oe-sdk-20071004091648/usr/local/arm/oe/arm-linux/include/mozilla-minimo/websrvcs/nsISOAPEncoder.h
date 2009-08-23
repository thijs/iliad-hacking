/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPEncoder.idl
 */

#ifndef __gen_nsISOAPEncoder_h__
#define __gen_nsISOAPEncoder_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISchemaType; /* forward declaration */

class nsISOAPEncoding; /* forward declaration */

class nsIVariant; /* forward declaration */

class nsIDOMElement; /* forward declaration */

class nsISOAPAttachments; /* forward declaration */


/* starting interface:    nsISOAPEncoder */
#define NS_ISOAPENCODER_IID_STR "fc33ffd6-1dd1-11b2-8750-fa62430a38b4"

#define NS_ISOAPENCODER_IID \
  {0xfc33ffd6, 0x1dd1, 0x11b2, \
    { 0x87, 0x50, 0xfa, 0x62, 0x43, 0x0a, 0x38, 0xb4 }}

/**
 * This interface permits encoding of variants.
 */
class NS_NO_VTABLE nsISOAPEncoder : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPENCODER_IID)

  /**
   * Encode the source variant.
   *
   * @param aEncodings The encodings to be used.
   *
   * @param aEncodingStyleURI The encoding style
   *
   * @param aSource The variant to be encoded.
   *
   * @param aNamespaceURI The namespace of the thing being coded
   *
   * @param aName The name of the thing being coded
   *
   * @param aSchemaType The schema type of the thing being encoded
   *
   * @param aDestination The node scope, if any, where the result 
   *   will live.  If this is null, then the result must be 
   *   explicitly attached to the message.
   *
   * @return element which was inserted.
   *
   * @param aAttachments Accumulates any attachments.
   */
  /* nsIDOMElement encode (in nsISOAPEncoding aEncoding, in nsIVariant aSource, in AString aNamespaceURI, in AString aName, in nsISchemaType aSchemaType, in nsISOAPAttachments aAttachments, in nsIDOMElement aDestination); */
  NS_IMETHOD Encode(nsISOAPEncoding *aEncoding, nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPENCODER \
  NS_IMETHOD Encode(nsISOAPEncoding *aEncoding, nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPENCODER(_to) \
  NS_IMETHOD Encode(nsISOAPEncoding *aEncoding, nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval) { return _to Encode(aEncoding, aSource, aNamespaceURI, aName, aSchemaType, aAttachments, aDestination, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPENCODER(_to) \
  NS_IMETHOD Encode(nsISOAPEncoding *aEncoding, nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Encode(aEncoding, aSource, aNamespaceURI, aName, aSchemaType, aAttachments, aDestination, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPEncoder : public nsISOAPEncoder
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPENCODER

  nsSOAPEncoder();

private:
  ~nsSOAPEncoder();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPEncoder, nsISOAPEncoder)

nsSOAPEncoder::nsSOAPEncoder()
{
  /* member initializers and constructor code */
}

nsSOAPEncoder::~nsSOAPEncoder()
{
  /* destructor code */
}

/* nsIDOMElement encode (in nsISOAPEncoding aEncoding, in nsIVariant aSource, in AString aNamespaceURI, in AString aName, in nsISchemaType aSchemaType, in nsISOAPAttachments aAttachments, in nsIDOMElement aDestination); */
NS_IMETHODIMP nsSOAPEncoder::Encode(nsISOAPEncoding *aEncoding, nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISOAPEncoder_h__ */
