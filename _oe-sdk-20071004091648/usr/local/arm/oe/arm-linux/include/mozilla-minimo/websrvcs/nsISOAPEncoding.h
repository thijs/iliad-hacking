/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPEncoding.idl
 */

#ifndef __gen_nsISOAPEncoding_h__
#define __gen_nsISOAPEncoding_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISchemaType; /* forward declaration */

class nsIDOMElement; /* forward declaration */

class nsISOAPEncoder; /* forward declaration */

class nsISOAPDecoder; /* forward declaration */

class nsISOAPMessage; /* forward declaration */

class nsIVariant; /* forward declaration */

class nsISOAPAttachments; /* forward declaration */

class nsISchemaCollection; /* forward declaration */

class nsISOAPEncoding; /* forward declaration */


/* starting interface:    nsISOAPEncodingRegistry */
#define NS_ISOAPENCODINGREGISTRY_IID_STR "9fe91a61-3048-40e3-99ef-e39ab946ae0b"

#define NS_ISOAPENCODINGREGISTRY_IID \
  {0x9fe91a61, 0x3048, 0x40e3, \
    { 0x99, 0xef, 0xe3, 0x9a, 0xb9, 0x46, 0xae, 0x0b }}

/** 
 * This interface provides access to a collection of SOAP encodings
 *   and schemas. The registry is owned by an encoding.
 */
class NS_NO_VTABLE nsISOAPEncodingRegistry : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPENCODINGREGISTRY_IID)

  /**
   * Get an associated encoding for a different encoding style.  If
   *   there is no default associated encoding available for the
   *   specified encoding style, then if aCreateIf is specified, one
   *   is created, otherwise a null is returned.  Associated encodings
   *   are the set of encodings that may be used and modified together
   *   as a set.
   *
   * @param aStyleURI The style URI of the associated encoding.
   *
   * @param aCreateIf If true, then create the associated encoding if it
   *   does not already exist, otherwise return the existing encoding.
   *
   * @return The associated encoding which corresponds to the
   *   specified styleURI, or null if the spefied alternative encoding
   *   does not exist and it was not requested that it be created.
   */
  /* nsISOAPEncoding getAssociatedEncoding (in AString aStyleURI, in boolean aCreateIf); */
  NS_IMETHOD GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval) = 0;

  /**
    * The schema collection used by the owning encoding and all 
    *   associated encodings.
    */
  /* attribute nsISchemaCollection schemaCollection; */
  NS_IMETHOD GetSchemaCollection(nsISchemaCollection * *aSchemaCollection) = 0;
  NS_IMETHOD SetSchemaCollection(nsISchemaCollection * aSchemaCollection) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPENCODINGREGISTRY \
  NS_IMETHOD GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval); \
  NS_IMETHOD GetSchemaCollection(nsISchemaCollection * *aSchemaCollection); \
  NS_IMETHOD SetSchemaCollection(nsISchemaCollection * aSchemaCollection); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPENCODINGREGISTRY(_to) \
  NS_IMETHOD GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval) { return _to GetAssociatedEncoding(aStyleURI, aCreateIf, _retval); } \
  NS_IMETHOD GetSchemaCollection(nsISchemaCollection * *aSchemaCollection) { return _to GetSchemaCollection(aSchemaCollection); } \
  NS_IMETHOD SetSchemaCollection(nsISchemaCollection * aSchemaCollection) { return _to SetSchemaCollection(aSchemaCollection); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPENCODINGREGISTRY(_to) \
  NS_IMETHOD GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAssociatedEncoding(aStyleURI, aCreateIf, _retval); } \
  NS_IMETHOD GetSchemaCollection(nsISchemaCollection * *aSchemaCollection) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSchemaCollection(aSchemaCollection); } \
  NS_IMETHOD SetSchemaCollection(nsISchemaCollection * aSchemaCollection) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSchemaCollection(aSchemaCollection); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPEncodingRegistry : public nsISOAPEncodingRegistry
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPENCODINGREGISTRY

  nsSOAPEncodingRegistry();

private:
  ~nsSOAPEncodingRegistry();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPEncodingRegistry, nsISOAPEncodingRegistry)

nsSOAPEncodingRegistry::nsSOAPEncodingRegistry()
{
  /* member initializers and constructor code */
}

nsSOAPEncodingRegistry::~nsSOAPEncodingRegistry()
{
  /* destructor code */
}

/* nsISOAPEncoding getAssociatedEncoding (in AString aStyleURI, in boolean aCreateIf); */
NS_IMETHODIMP nsSOAPEncodingRegistry::GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISchemaCollection schemaCollection; */
NS_IMETHODIMP nsSOAPEncodingRegistry::GetSchemaCollection(nsISchemaCollection * *aSchemaCollection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPEncodingRegistry::SetSchemaCollection(nsISchemaCollection * aSchemaCollection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISOAPEncoding */
#define NS_ISOAPENCODING_IID_STR "9ae49600-1dd1-11b2-877f-e62f620c5e92"

#define NS_ISOAPENCODING_IID \
  {0x9ae49600, 0x1dd1, 0x11b2, \
    { 0x87, 0x7f, 0xe6, 0x2f, 0x62, 0x0c, 0x5e, 0x92 }}

class NS_NO_VTABLE nsISOAPEncoding : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPENCODING_IID)

  /**
 * This interface keeps track of the current encoding style and
 *   how objects should be encoded or decoded, as well as all
 *   associated encodings.
 *   <p>While two different nsSOAPCalls may have an encoding
 *   for the same styleURI, by default the encoding object and all
 *   associated encoding objects are local do not affect other
 *   calls because the encodings are local and not associated, 
 *   unless the same or associated encodings are explicitly set
 *   on multiple calls.  When a call is invoked, the same encoding 
 *   object is automatically set on the response message so that
 *   decoders registered on the call encoding are automatically
 *   applied to the response.
 */
/**
   * The name of the encoding as it is known to SOAP.
   */
  /* readonly attribute AString styleURI; */
  NS_IMETHOD GetStyleURI(nsAString & aStyleURI) = 0;

  /**
   * Get an associated encoding for a different encoding style.  If
   *   there is no default associated encoding available for the
   *   specified encoding style, then if aCreateIf is specified, one
   *   is created, otherwise a null is returned.  Associated encodings
   *   are the set of encodings that may be used and modified together
   *   as a set.
   *
   * @param aStyleURI The style URI of the associated encoding.
   *
   * @param aCreateIf If true, then create the associated encoding if it
   *   does not already exist, otherwise return the existing encoding.
   *
   * @return The associated encoding which corresponds to the
   *   specified styleURI, or null if the spefied alternative encoding
   *   does not exist and it was not requested that it be created.
   */
  /* nsISOAPEncoding getAssociatedEncoding (in AString aStyleURI, in boolean aCreateIf); */
  NS_IMETHOD GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval) = 0;

  /**
    * Set an encoder in the encoding.
    *
    * @param aKey The key to be associated with the encoder.
    *
    * @param aEncoder The encoder to be specified or null to eliminate
    *   the encoder.
    */
  /* void setEncoder (in AString aKey, in nsISOAPEncoder aEncoder); */
  NS_IMETHOD SetEncoder(const nsAString & aKey, nsISOAPEncoder *aEncoder) = 0;

  /**
    * Get an encoder from the encoding.
    *
    * @param aKey The key to be used to look up the encoder.
    *
    * @return The encoder.
    */
  /* nsISOAPEncoder getEncoder (in AString aKey); */
  NS_IMETHOD GetEncoder(const nsAString & aKey, nsISOAPEncoder **_retval) = 0;

  /**
    * Set a decoder in the encoding.
    *
    * @param aKey The key to be associated with the decoder.
    *
    * @param aDecoder The decoder to be specified or null to eliminate
    *   the decoder.
    */
  /* void setDecoder (in AString aKey, in nsISOAPDecoder aDecoder); */
  NS_IMETHOD SetDecoder(const nsAString & aKey, nsISOAPDecoder *aDecoder) = 0;

  /**
    * Get a decoder from the encoding.
    *
    * @param aKey The key to be used to look up the decoder.
    *
    * @return The decoder.
    */
  /* nsISOAPDecoder getDecoder (in AString aKey); */
  NS_IMETHOD GetDecoder(const nsAString & aKey, nsISOAPDecoder **_retval) = 0;

  /**
   * The default encoder invoked by all encoding calls. Appropriate calls
   * to more-specific encoders, if any, must be dispatched by this default 
   * encoder.  The default encoder typically looks up encoders by known 
   * information such as explicit or implicit types.
   */
  /* attribute nsISOAPEncoder defaultEncoder; */
  NS_IMETHOD GetDefaultEncoder(nsISOAPEncoder * *aDefaultEncoder) = 0;
  NS_IMETHOD SetDefaultEncoder(nsISOAPEncoder * aDefaultEncoder) = 0;

  /**
   * The default decoder invoked by all encoding calls. Appropriate calls
   * to more-specific decoders, if any, must be dispatched by this default 
   * decoder.  The default decoder typically looks up encoders by known 
   * information such as explicit or implicit types.
   */
  /* attribute nsISOAPDecoder defaultDecoder; */
  NS_IMETHOD GetDefaultDecoder(nsISOAPDecoder * *aDefaultDecoder) = 0;
  NS_IMETHOD SetDefaultDecoder(nsISOAPDecoder * aDefaultDecoder) = 0;

  /**
    * The schema collection used by this and all associated encodings.
    */
  /* attribute nsISchemaCollection schemaCollection; */
  NS_IMETHOD GetSchemaCollection(nsISchemaCollection * *aSchemaCollection) = 0;
  NS_IMETHOD SetSchemaCollection(nsISchemaCollection * aSchemaCollection) = 0;

  /**
   * Encode the source variant
   *
   * @param aEncodings The encodings to be used.
   *
   * @param aEncodingStyleURI The encoding style
   *
   * @param aSource The variant to be encoded, soon to become a variant
   *
   * @param aNamespaceURI The namespace of the thing being coded
   *
   * @param aName The name of the thing being coded
   *
   * @param aSchemaType The schema type of the thing being encoded
   *
   * @param aDestination The node scope where the result will live.
   *
   * @param aAttachments Accumulates any attachments.
   *
   * @return The element which was inserted and encoded.
   */
  /* nsIDOMElement encode (in nsIVariant aSource, in AString aNamespaceURI, in AString aName, in nsISchemaType aSchemaType, in nsISOAPAttachments aAttachments, in nsIDOMElement aDestination); */
  NS_IMETHOD Encode(nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval) = 0;

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
   * @return The decoded variant, soon to become a variant, which is null if 
   *   the operation failed or did not return a result.
   */
  /* nsIVariant decode (in nsIDOMElement aSource, in nsISchemaType aSchemaType, in nsISOAPAttachments aAttachments); */
  NS_IMETHOD Decode(nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval) = 0;

  /**
   * Map an external schema URI to an internal one.  In an
   * ideal world, we don't need this, but schemas change URIs,
   * and we wind up with inconsistencies between WSDL and
   * exchanged documents.  The external URI must not be
   * currently mapped and if output mapping is true, then
   * the internal URI must be unique for output mapping.
   *
   * @param aExternalURI the external URI to be mapped.
   *
   * @param aInternalURI the internal URI to be mapped.
   *
   * @param aOutput If true, then re-map to the output
   *   URI when outputting.
   *
   * @return boolean true if succeeded, false if the
   *   mapping was not unique as required.
   */
  /* boolean mapSchemaURI (in AString aExternalURI, in AString aInternalURI, in boolean aOutput); */
  NS_IMETHOD MapSchemaURI(const nsAString & aExternalURI, const nsAString & aInternalURI, PRBool aOutput, PRBool *_retval) = 0;

  /**
   * Unmap an external schema URI as mapped by
   * mapSchemaURI.
   *
   * @param aExternalURI the external URI to be mapped.
   *
   * @return boolean true if succeeded, false if the
   *   mapping did not exist.
   */
  /* boolean unmapSchemaURI (in AString aExternalURI); */
  NS_IMETHOD UnmapSchemaURI(const nsAString & aExternalURI, PRBool *_retval) = 0;

  /**
   * Get an internal schema URI from an external one.
   *
   * @param aExternalURI the external URI which may
   *   have been mapped.
   *
   * @return The corresponding internal URI (the
   *   original is returned if it has not been mapped).
   */
  /* AString getInternalSchemaURI (in AString aExternalURI); */
  NS_IMETHOD GetInternalSchemaURI(const nsAString & aExternalURI, nsAString & _retval) = 0;

  /**
   * Get an external schema URI from an internal one.
   *
   * @param aExternalURI the external URI which may
   *   have been mapped.
   *
   * @return The corresponding internal URI (the
   *   original is returned if it has not been mapped).
   */
  /* AString getExternalSchemaURI (in AString aInternalURI); */
  NS_IMETHOD GetExternalSchemaURI(const nsAString & aInternalURI, nsAString & _retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPENCODING \
  NS_IMETHOD GetStyleURI(nsAString & aStyleURI); \
  NS_IMETHOD GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval); \
  NS_IMETHOD SetEncoder(const nsAString & aKey, nsISOAPEncoder *aEncoder); \
  NS_IMETHOD GetEncoder(const nsAString & aKey, nsISOAPEncoder **_retval); \
  NS_IMETHOD SetDecoder(const nsAString & aKey, nsISOAPDecoder *aDecoder); \
  NS_IMETHOD GetDecoder(const nsAString & aKey, nsISOAPDecoder **_retval); \
  NS_IMETHOD GetDefaultEncoder(nsISOAPEncoder * *aDefaultEncoder); \
  NS_IMETHOD SetDefaultEncoder(nsISOAPEncoder * aDefaultEncoder); \
  NS_IMETHOD GetDefaultDecoder(nsISOAPDecoder * *aDefaultDecoder); \
  NS_IMETHOD SetDefaultDecoder(nsISOAPDecoder * aDefaultDecoder); \
  NS_IMETHOD GetSchemaCollection(nsISchemaCollection * *aSchemaCollection); \
  NS_IMETHOD SetSchemaCollection(nsISchemaCollection * aSchemaCollection); \
  NS_IMETHOD Encode(nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval); \
  NS_IMETHOD Decode(nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval); \
  NS_IMETHOD MapSchemaURI(const nsAString & aExternalURI, const nsAString & aInternalURI, PRBool aOutput, PRBool *_retval); \
  NS_IMETHOD UnmapSchemaURI(const nsAString & aExternalURI, PRBool *_retval); \
  NS_IMETHOD GetInternalSchemaURI(const nsAString & aExternalURI, nsAString & _retval); \
  NS_IMETHOD GetExternalSchemaURI(const nsAString & aInternalURI, nsAString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPENCODING(_to) \
  NS_IMETHOD GetStyleURI(nsAString & aStyleURI) { return _to GetStyleURI(aStyleURI); } \
  NS_IMETHOD GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval) { return _to GetAssociatedEncoding(aStyleURI, aCreateIf, _retval); } \
  NS_IMETHOD SetEncoder(const nsAString & aKey, nsISOAPEncoder *aEncoder) { return _to SetEncoder(aKey, aEncoder); } \
  NS_IMETHOD GetEncoder(const nsAString & aKey, nsISOAPEncoder **_retval) { return _to GetEncoder(aKey, _retval); } \
  NS_IMETHOD SetDecoder(const nsAString & aKey, nsISOAPDecoder *aDecoder) { return _to SetDecoder(aKey, aDecoder); } \
  NS_IMETHOD GetDecoder(const nsAString & aKey, nsISOAPDecoder **_retval) { return _to GetDecoder(aKey, _retval); } \
  NS_IMETHOD GetDefaultEncoder(nsISOAPEncoder * *aDefaultEncoder) { return _to GetDefaultEncoder(aDefaultEncoder); } \
  NS_IMETHOD SetDefaultEncoder(nsISOAPEncoder * aDefaultEncoder) { return _to SetDefaultEncoder(aDefaultEncoder); } \
  NS_IMETHOD GetDefaultDecoder(nsISOAPDecoder * *aDefaultDecoder) { return _to GetDefaultDecoder(aDefaultDecoder); } \
  NS_IMETHOD SetDefaultDecoder(nsISOAPDecoder * aDefaultDecoder) { return _to SetDefaultDecoder(aDefaultDecoder); } \
  NS_IMETHOD GetSchemaCollection(nsISchemaCollection * *aSchemaCollection) { return _to GetSchemaCollection(aSchemaCollection); } \
  NS_IMETHOD SetSchemaCollection(nsISchemaCollection * aSchemaCollection) { return _to SetSchemaCollection(aSchemaCollection); } \
  NS_IMETHOD Encode(nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval) { return _to Encode(aSource, aNamespaceURI, aName, aSchemaType, aAttachments, aDestination, _retval); } \
  NS_IMETHOD Decode(nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval) { return _to Decode(aSource, aSchemaType, aAttachments, _retval); } \
  NS_IMETHOD MapSchemaURI(const nsAString & aExternalURI, const nsAString & aInternalURI, PRBool aOutput, PRBool *_retval) { return _to MapSchemaURI(aExternalURI, aInternalURI, aOutput, _retval); } \
  NS_IMETHOD UnmapSchemaURI(const nsAString & aExternalURI, PRBool *_retval) { return _to UnmapSchemaURI(aExternalURI, _retval); } \
  NS_IMETHOD GetInternalSchemaURI(const nsAString & aExternalURI, nsAString & _retval) { return _to GetInternalSchemaURI(aExternalURI, _retval); } \
  NS_IMETHOD GetExternalSchemaURI(const nsAString & aInternalURI, nsAString & _retval) { return _to GetExternalSchemaURI(aInternalURI, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPENCODING(_to) \
  NS_IMETHOD GetStyleURI(nsAString & aStyleURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStyleURI(aStyleURI); } \
  NS_IMETHOD GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAssociatedEncoding(aStyleURI, aCreateIf, _retval); } \
  NS_IMETHOD SetEncoder(const nsAString & aKey, nsISOAPEncoder *aEncoder) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetEncoder(aKey, aEncoder); } \
  NS_IMETHOD GetEncoder(const nsAString & aKey, nsISOAPEncoder **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEncoder(aKey, _retval); } \
  NS_IMETHOD SetDecoder(const nsAString & aKey, nsISOAPDecoder *aDecoder) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDecoder(aKey, aDecoder); } \
  NS_IMETHOD GetDecoder(const nsAString & aKey, nsISOAPDecoder **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDecoder(aKey, _retval); } \
  NS_IMETHOD GetDefaultEncoder(nsISOAPEncoder * *aDefaultEncoder) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultEncoder(aDefaultEncoder); } \
  NS_IMETHOD SetDefaultEncoder(nsISOAPEncoder * aDefaultEncoder) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDefaultEncoder(aDefaultEncoder); } \
  NS_IMETHOD GetDefaultDecoder(nsISOAPDecoder * *aDefaultDecoder) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultDecoder(aDefaultDecoder); } \
  NS_IMETHOD SetDefaultDecoder(nsISOAPDecoder * aDefaultDecoder) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDefaultDecoder(aDefaultDecoder); } \
  NS_IMETHOD GetSchemaCollection(nsISchemaCollection * *aSchemaCollection) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSchemaCollection(aSchemaCollection); } \
  NS_IMETHOD SetSchemaCollection(nsISchemaCollection * aSchemaCollection) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSchemaCollection(aSchemaCollection); } \
  NS_IMETHOD Encode(nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Encode(aSource, aNamespaceURI, aName, aSchemaType, aAttachments, aDestination, _retval); } \
  NS_IMETHOD Decode(nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Decode(aSource, aSchemaType, aAttachments, _retval); } \
  NS_IMETHOD MapSchemaURI(const nsAString & aExternalURI, const nsAString & aInternalURI, PRBool aOutput, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->MapSchemaURI(aExternalURI, aInternalURI, aOutput, _retval); } \
  NS_IMETHOD UnmapSchemaURI(const nsAString & aExternalURI, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->UnmapSchemaURI(aExternalURI, _retval); } \
  NS_IMETHOD GetInternalSchemaURI(const nsAString & aExternalURI, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInternalSchemaURI(aExternalURI, _retval); } \
  NS_IMETHOD GetExternalSchemaURI(const nsAString & aInternalURI, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetExternalSchemaURI(aInternalURI, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPEncoding : public nsISOAPEncoding
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPENCODING

  nsSOAPEncoding();

private:
  ~nsSOAPEncoding();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPEncoding, nsISOAPEncoding)

nsSOAPEncoding::nsSOAPEncoding()
{
  /* member initializers and constructor code */
}

nsSOAPEncoding::~nsSOAPEncoding()
{
  /* destructor code */
}

/* readonly attribute AString styleURI; */
NS_IMETHODIMP nsSOAPEncoding::GetStyleURI(nsAString & aStyleURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISOAPEncoding getAssociatedEncoding (in AString aStyleURI, in boolean aCreateIf); */
NS_IMETHODIMP nsSOAPEncoding::GetAssociatedEncoding(const nsAString & aStyleURI, PRBool aCreateIf, nsISOAPEncoding **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setEncoder (in AString aKey, in nsISOAPEncoder aEncoder); */
NS_IMETHODIMP nsSOAPEncoding::SetEncoder(const nsAString & aKey, nsISOAPEncoder *aEncoder)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISOAPEncoder getEncoder (in AString aKey); */
NS_IMETHODIMP nsSOAPEncoding::GetEncoder(const nsAString & aKey, nsISOAPEncoder **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setDecoder (in AString aKey, in nsISOAPDecoder aDecoder); */
NS_IMETHODIMP nsSOAPEncoding::SetDecoder(const nsAString & aKey, nsISOAPDecoder *aDecoder)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISOAPDecoder getDecoder (in AString aKey); */
NS_IMETHODIMP nsSOAPEncoding::GetDecoder(const nsAString & aKey, nsISOAPDecoder **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISOAPEncoder defaultEncoder; */
NS_IMETHODIMP nsSOAPEncoding::GetDefaultEncoder(nsISOAPEncoder * *aDefaultEncoder)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPEncoding::SetDefaultEncoder(nsISOAPEncoder * aDefaultEncoder)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISOAPDecoder defaultDecoder; */
NS_IMETHODIMP nsSOAPEncoding::GetDefaultDecoder(nsISOAPDecoder * *aDefaultDecoder)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPEncoding::SetDefaultDecoder(nsISOAPDecoder * aDefaultDecoder)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISchemaCollection schemaCollection; */
NS_IMETHODIMP nsSOAPEncoding::GetSchemaCollection(nsISchemaCollection * *aSchemaCollection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPEncoding::SetSchemaCollection(nsISchemaCollection * aSchemaCollection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMElement encode (in nsIVariant aSource, in AString aNamespaceURI, in AString aName, in nsISchemaType aSchemaType, in nsISOAPAttachments aAttachments, in nsIDOMElement aDestination); */
NS_IMETHODIMP nsSOAPEncoding::Encode(nsIVariant *aSource, const nsAString & aNamespaceURI, const nsAString & aName, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIDOMElement *aDestination, nsIDOMElement **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIVariant decode (in nsIDOMElement aSource, in nsISchemaType aSchemaType, in nsISOAPAttachments aAttachments); */
NS_IMETHODIMP nsSOAPEncoding::Decode(nsIDOMElement *aSource, nsISchemaType *aSchemaType, nsISOAPAttachments *aAttachments, nsIVariant **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean mapSchemaURI (in AString aExternalURI, in AString aInternalURI, in boolean aOutput); */
NS_IMETHODIMP nsSOAPEncoding::MapSchemaURI(const nsAString & aExternalURI, const nsAString & aInternalURI, PRBool aOutput, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean unmapSchemaURI (in AString aExternalURI); */
NS_IMETHODIMP nsSOAPEncoding::UnmapSchemaURI(const nsAString & aExternalURI, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getInternalSchemaURI (in AString aExternalURI); */
NS_IMETHODIMP nsSOAPEncoding::GetInternalSchemaURI(const nsAString & aExternalURI, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getExternalSchemaURI (in AString aInternalURI); */
NS_IMETHODIMP nsSOAPEncoding::GetExternalSchemaURI(const nsAString & aInternalURI, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SOAPENCODING_CID                            \
{ /* db612ec6-1dd1-11b2-a8bc-e6777bf70a30 */        \
  0xdb612ec6, 0x1dd1, 0x11b2,                       \
 {0xa8, 0xbc, 0xe6, 0x77, 0x7b, 0xf7, 0x0a, 0x30} }
#define NS_SOAPENCODING_CONTRACTID \
"@mozilla.org/xmlextras/soap/encoding;1"
#define NS_SOAPENCODING_CONTRACTID_PREFIX \
NS_SOAPENCODING_CONTRACTID "?uri="
#define NS_DEFAULTSOAPENCODING_1_1_CID                            \
{ /* 06fb035c-1dd2-11b2-bc30-f6d8e314d6b9 */        \
  0x06fb035c, 0x1dd2, 0x11b2,                       \
 {0xbc, 0x30, 0xf6, 0xd8, 0xe3, 0x14, 0xd6, 0xb9} }
#define NS_DEFAULTSOAPENCODING_1_1_CONTRACTID \
NS_SOAPENCODING_CONTRACTID_PREFIX "http://schemas.xmlsoap.org/soap/encoding/"
#define NS_DEFAULTSOAPENCODING_1_2_CID                            \
{ /* e0ee4044-1dd1-11b2-9d7e-8899d4d89648 */        \
  0xe0ee4044, 0x1dd1, 0x11b2,                       \
 {0x9d, 0x7e, 0x88, 0x99, 0xd4, 0xd8, 0x96, 0x48} }
#define NS_DEFAULTSOAPENCODING_1_2_CONTRACTID \
NS_SOAPENCODING_CONTRACTID_PREFIX "http://www.w3.org/2001/09/soap-encoding"

#endif /* __gen_nsISOAPEncoding_h__ */
