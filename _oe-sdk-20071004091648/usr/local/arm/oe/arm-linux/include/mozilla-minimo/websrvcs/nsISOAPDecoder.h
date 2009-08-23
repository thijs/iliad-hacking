/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPDecoder.idl
 */

#ifndef __gen_nsISOAPDecoder_h__
#define __gen_nsISOAPDecoder_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISchemaType; /* forward declaration */

class nsISOAPEncoding; /* forward declaration */

class nsIDOMElement; /* forward declaration */

class nsIVariant; /* forward declaration */

class nsISOAPAttachments; /* forward declaration */


/* starting interface:    nsISOAPDecoder */
#define NS_ISOAPDECODER_IID_STR "4c2e02ae-1dd2-11b2-b1cd-c79dea3d46db"

#define NS_ISOAPDECODER_IID \
  {0x4c2e02ae, 0x1dd2, 0x11b2, \
    { 0xb1, 0xcd, 0xc7, 0x9d, 0xea, 0x3d, 0x46, 0xdb }}

/**
 * This interface supplies decoding of a specific
 * part of a message XML DOM into appropriate objects
 * for the script or application.
 */
class NS_NO_VTABLE nsISOAPDecoder : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPDECODER_IID)

  /**
   * Decode the source DOM node
   *
   * @param aEncodings The encodings used to decode
   *
   * @param aEncodingStyleURI The encoding style
   *
   * @param aSource The DOM node to be decoded.
   *
   * @param aSchemaType The schema type of the source DOM node
   *
   * @param aAttachments Dispenses any attachments.
   *
   * @return The decoded variant, which is null if 
   *   the operation failed or did not return a result.
   */
  /* nsIVariant decode (in nsISOAPEncoding aEncoding, in nsIDOMElement aSource, in nsISchemaType aSchemaType, in nsISOAPAttachments aAttachments); */
  NS_IMETHOD Decode(nsISOAPEncoding *aEncoding, nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPDECODER \
  NS_IMETHOD Decode(nsISOAPEncoding *aEncoding, nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPDECODER(_to) \
  NS_IMETHOD Decode(nsISOAPEncoding *aEncoding, nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval) { return _to Decode(aEncoding, aSource, aSchemaType, aAttachments, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPDECODER(_to) \
  NS_IMETHOD Decode(nsISOAPEncoding *aEncoding, nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Decode(aEncoding, aSource, aSchemaType, aAttachments, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPDecoder : public nsISOAPDecoder
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPDECODER

  nsSOAPDecoder();

private:
  ~nsSOAPDecoder();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPDecoder, nsISOAPDecoder)

nsSOAPDecoder::nsSOAPDecoder()
{
  /* member initializers and constructor code */
}

nsSOAPDecoder::~nsSOAPDecoder()
{
  /* destructor code */
}

/* nsIVariant decode (in nsISOAPEncoding aEncoding, in nsIDOMElement aSource, in nsISchemaType aSchemaType, in nsISOAPAttachments aAttachments); */
NS_IMETHODIMP nsSOAPDecoder::Decode(nsISOAPEncoding *aEncoding, nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISOAPDecoder_h__ */
