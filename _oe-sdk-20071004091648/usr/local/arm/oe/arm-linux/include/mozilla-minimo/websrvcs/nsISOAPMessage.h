/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPMessage.idl
 */

#ifndef __gen_nsISOAPMessage_h__
#define __gen_nsISOAPMessage_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMDocument; /* forward declaration */

class nsIDOMElement; /* forward declaration */

class nsISOAPEncoding; /* forward declaration */

class nsISOAPHeaderBlock; /* forward declaration */

class nsISOAPParameter; /* forward declaration */

class nsIVariant; /* forward declaration */


/* starting interface:    nsISOAPMessage */
#define NS_ISOAPMESSAGE_IID_STR "3970815e-1dd2-11b2-a475-db4dac6826f1"

#define NS_ISOAPMESSAGE_IID \
  {0x3970815e, 0x1dd2, 0x11b2, \
    { 0xa4, 0x75, 0xdb, 0x4d, 0xac, 0x68, 0x26, 0xf1 }}

/**
 * This interface controls all SOAP messages. It permits easy
 * construction of a message, typically through encoding of
 * parameters and certain properties settable on this interface
 * or through deserialization of a transport stream.  It
 * permits easy processing of a message typically through
 * decoding of parameters and certain properties available
 * on this interface.  It also encapsulates protocol information
 * interpreted by the transport.
 */
class NS_NO_VTABLE nsISOAPMessage : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPMESSAGE_IID)

  enum { VERSION_1_1 = 0U };

  enum { VERSION_1_2 = 1U };

  enum { VERSION_UNKNOWN = 65535U };

  /**
   * The document which captures the message, if any.  A simple 
   * sending application passes parameters to the method
   * encodeSOAPParameters, which calls SOAP encoders 
   * to construct this document along with all contained elements.  
   *
   * But an application may create and set the message directly
   * instead of invoking encodeSOAPParameters to use encoders
   * or access and manipulate the message after it has been 
   * constructed by encodeSOAPParameters.  If the message has 
   * not been set, invoking a call will fail.  A message reciever 
   * may also use this accessor to get the document to avoid using 
   * decoders.
   */
  /* attribute nsIDOMDocument message; */
  NS_IMETHOD GetMessage(nsIDOMDocument * *aMessage) = 0;
  NS_IMETHOD SetMessage(nsIDOMDocument * aMessage) = 0;

  /**
   * A convenience attribute to obtain the DOM element representing the
   * SOAP envelope from the document.  DOM methods may be used to 
   * access, add, or modify attributes of the envelope.
   *
   * If the message attribute is null or is not a document containing
   * a root soap envelope element, then this will be null.
   */
  /* readonly attribute nsIDOMElement envelope; */
  NS_IMETHOD GetEnvelope(nsIDOMElement * *aEnvelope) = 0;

  /**
   * A convenience attribute to obtain the SOAP version number, if
   * it is known, from the envelope.
   *
   * If the message attribute is null or is not a document containing
   * a root soap envelope element, then this will be VERSION_UNKNOWN.
   */
  /* readonly attribute unsigned short version; */
  NS_IMETHOD GetVersion(PRUint16 *aVersion) = 0;

  /**
   * A convenience attribute to obtain the DOM element representing the
   * SOAP header from the envelope.  DOM methods may be used to 
   * access, add, or modify attributes or elements of the header.
   *
   * If the envelope attribute is null or does not contain a SOAP header
   * element type, then this will be null.
   */
  /* readonly attribute nsIDOMElement header; */
  NS_IMETHOD GetHeader(nsIDOMElement * *aHeader) = 0;

  /**
   * A convenience attribute to obtain the DOM element representing the
   * SOAP body from the envelope.  DOM methods may be used to 
   * access, add, or modify attributes or elements of the body.
   *
   * If the envelope attribute is null or does not contain a SOAP body
   * element type, then this will be null.
   */
  /* readonly attribute nsIDOMElement body; */
  NS_IMETHOD GetBody(nsIDOMElement * *aBody) = 0;

  /**
   * The name of the method being invoked. The methodName is set
   * during encoding as the tagname of the single child of body
   * of RPC-style messages.  When there is no encoded message
   * this will be null.  The value of this attribute for
   * document-style messages may be non-null but should be
   * ignored.  It is up to the application to know whether the
   * message is RPC-style or document style because the SOAP
   * specification makes it difficult to tell which way a
   * message was encoded.
   */
  /* readonly attribute AString methodName; */
  NS_IMETHOD GetMethodName(nsAString & aMethodName) = 0;

  /**
   * The target object on which the method is being invoked. This URI
   * is set during encoding as the namespace to qualify the tagname 
   * of the single child of body of RPC-style messages.  When there
   * is no encoded message, this will be null.  The value of this
   * attribute for document-style messages may be non-null but should
   * be ignored.  It is up to the application to know whether the
   * message is RPC-style or document style because the SOAP
   * specification makes it difficult to tell which way a
   * message was encoded.
   */
  /* readonly attribute AString targetObjectURI; */
  NS_IMETHOD GetTargetObjectURI(nsAString & aTargetObjectURI) = 0;

  /**
   * Encodes the specified parameters into this message, if
   * this message type supports it.
   *
   * @param aMethodName The name of the method being invoked
   * for rpc-style messages.  For document-style messages,
   * this must be null.
   *
   * @param aTargetObjectURI The name of the target object
   * for rpc-style messages.  For document-style messages,
   * this must be null.
   *
   * @param aHeaderBlockCount Number of header blocks in array to be
   *   encoded.  Must be 0 if header block array is null.
   *
   * @param aHeaderBlocks Array of header blocks to be encoded, which
   *   may be null if there are no header blocks.
   *
   * @param aParameterCount Number of parameters in array 
   *   to be encoded.  Must be 0 if parameter array is null.
   *
   * @param aParameters An array of parameters to be 
   *   encoded, which may null if there are no parameters.
   */
  /* void encode (in unsigned short aVersion, in AString aMethodName, in AString aTargetObjectURI, in unsigned long aHeaderBlockCount, [array, size_is (aHeaderBlockCount)] in nsISOAPHeaderBlock aHeaderBlocks, in unsigned long aParameterCount, [array, size_is (aParameterCount)] in nsISOAPParameter aParameters); */
  NS_IMETHOD Encode(PRUint16 aVersion, const nsAString & aMethodName, const nsAString & aTargetObjectURI, PRUint32 aHeaderBlockCount, nsISOAPHeaderBlock **aHeaderBlocks, PRUint32 aParameterCount, nsISOAPParameter **aParameters) = 0;

  /**
   * Gathers the header blocks of a message so that they can be
   *   accessed by a recipient.
   *
   * @param aCount Integer to receive the length of the list
   *   of header blocks.
   *
   * @return Array of header blocks found in the message.
   */
  /* void getHeaderBlocks (out unsigned long aCount, [array, size_is (aCount), retval] out nsISOAPHeaderBlock aHeaderBlocks); */
  NS_IMETHOD GetHeaderBlocks(PRUint32 *aCount, nsISOAPHeaderBlock ***aHeaderBlocks) = 0;

  /**
   * Gathers the parameters of a message so that they can be
   *   accessed by a recipient.
   *
   * @param aDocumentStyle If true, then the parameters
   *   are looked for treating the message as a document 
   *   style message, otherwise it treated as an RPC-style 
   *   message.
   *
   * @param aCount Integer to receive the length of the list
   *   of parameters.
   *
   * @return Array of parameters found in the message.
   */
  /* void getParameters (in boolean aDocumentStyle, out unsigned long aCount, [array, size_is (aCount), retval] out nsISOAPParameter aParameters); */
  NS_IMETHOD GetParameters(PRBool aDocumentStyle, PRUint32 *aCount, nsISOAPParameter ***aParameters) = 0;

  /**
   * The primary encoding of the message, which is established
   *   at the envelope and used unless overridden.  By default,
   *   this is the SOAP encoding, which may be locally modified
   *   or used to obtain alternative encodings, which may be
   *   locally modified, but it may be set to an encoding that
   *   is shared, or it may be set to null, in which case all
   *   non-literal header blocks and parameters must specify an
   *   encoding.
   */
  /* attribute nsISOAPEncoding encoding; */
  NS_IMETHOD GetEncoding(nsISOAPEncoding * *aEncoding) = 0;
  NS_IMETHOD SetEncoding(nsISOAPEncoding * aEncoding) = 0;

  /**
   * An optional URI that can be used to add a SOAPAction HTTP
   * header field. If this attribute is NULL (the default case),
   * no SOAPAction header will be added.
   */
  /* attribute AString actionURI; */
  NS_IMETHOD GetActionURI(nsAString & aActionURI) = 0;
  NS_IMETHOD SetActionURI(const nsAString & aActionURI) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPMESSAGE \
  NS_IMETHOD GetMessage(nsIDOMDocument * *aMessage); \
  NS_IMETHOD SetMessage(nsIDOMDocument * aMessage); \
  NS_IMETHOD GetEnvelope(nsIDOMElement * *aEnvelope); \
  NS_IMETHOD GetVersion(PRUint16 *aVersion); \
  NS_IMETHOD GetHeader(nsIDOMElement * *aHeader); \
  NS_IMETHOD GetBody(nsIDOMElement * *aBody); \
  NS_IMETHOD GetMethodName(nsAString & aMethodName); \
  NS_IMETHOD GetTargetObjectURI(nsAString & aTargetObjectURI); \
  NS_IMETHOD Encode(PRUint16 aVersion, const nsAString & aMethodName, const nsAString & aTargetObjectURI, PRUint32 aHeaderBlockCount, nsISOAPHeaderBlock **aHeaderBlocks, PRUint32 aParameterCount, nsISOAPParameter **aParameters); \
  NS_IMETHOD GetHeaderBlocks(PRUint32 *aCount, nsISOAPHeaderBlock ***aHeaderBlocks); \
  NS_IMETHOD GetParameters(PRBool aDocumentStyle, PRUint32 *aCount, nsISOAPParameter ***aParameters); \
  NS_IMETHOD GetEncoding(nsISOAPEncoding * *aEncoding); \
  NS_IMETHOD SetEncoding(nsISOAPEncoding * aEncoding); \
  NS_IMETHOD GetActionURI(nsAString & aActionURI); \
  NS_IMETHOD SetActionURI(const nsAString & aActionURI); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPMESSAGE(_to) \
  NS_IMETHOD GetMessage(nsIDOMDocument * *aMessage) { return _to GetMessage(aMessage); } \
  NS_IMETHOD SetMessage(nsIDOMDocument * aMessage) { return _to SetMessage(aMessage); } \
  NS_IMETHOD GetEnvelope(nsIDOMElement * *aEnvelope) { return _to GetEnvelope(aEnvelope); } \
  NS_IMETHOD GetVersion(PRUint16 *aVersion) { return _to GetVersion(aVersion); } \
  NS_IMETHOD GetHeader(nsIDOMElement * *aHeader) { return _to GetHeader(aHeader); } \
  NS_IMETHOD GetBody(nsIDOMElement * *aBody) { return _to GetBody(aBody); } \
  NS_IMETHOD GetMethodName(nsAString & aMethodName) { return _to GetMethodName(aMethodName); } \
  NS_IMETHOD GetTargetObjectURI(nsAString & aTargetObjectURI) { return _to GetTargetObjectURI(aTargetObjectURI); } \
  NS_IMETHOD Encode(PRUint16 aVersion, const nsAString & aMethodName, const nsAString & aTargetObjectURI, PRUint32 aHeaderBlockCount, nsISOAPHeaderBlock **aHeaderBlocks, PRUint32 aParameterCount, nsISOAPParameter **aParameters) { return _to Encode(aVersion, aMethodName, aTargetObjectURI, aHeaderBlockCount, aHeaderBlocks, aParameterCount, aParameters); } \
  NS_IMETHOD GetHeaderBlocks(PRUint32 *aCount, nsISOAPHeaderBlock ***aHeaderBlocks) { return _to GetHeaderBlocks(aCount, aHeaderBlocks); } \
  NS_IMETHOD GetParameters(PRBool aDocumentStyle, PRUint32 *aCount, nsISOAPParameter ***aParameters) { return _to GetParameters(aDocumentStyle, aCount, aParameters); } \
  NS_IMETHOD GetEncoding(nsISOAPEncoding * *aEncoding) { return _to GetEncoding(aEncoding); } \
  NS_IMETHOD SetEncoding(nsISOAPEncoding * aEncoding) { return _to SetEncoding(aEncoding); } \
  NS_IMETHOD GetActionURI(nsAString & aActionURI) { return _to GetActionURI(aActionURI); } \
  NS_IMETHOD SetActionURI(const nsAString & aActionURI) { return _to SetActionURI(aActionURI); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPMESSAGE(_to) \
  NS_IMETHOD GetMessage(nsIDOMDocument * *aMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMessage(aMessage); } \
  NS_IMETHOD SetMessage(nsIDOMDocument * aMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetMessage(aMessage); } \
  NS_IMETHOD GetEnvelope(nsIDOMElement * *aEnvelope) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEnvelope(aEnvelope); } \
  NS_IMETHOD GetVersion(PRUint16 *aVersion) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetVersion(aVersion); } \
  NS_IMETHOD GetHeader(nsIDOMElement * *aHeader) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHeader(aHeader); } \
  NS_IMETHOD GetBody(nsIDOMElement * *aBody) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBody(aBody); } \
  NS_IMETHOD GetMethodName(nsAString & aMethodName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMethodName(aMethodName); } \
  NS_IMETHOD GetTargetObjectURI(nsAString & aTargetObjectURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTargetObjectURI(aTargetObjectURI); } \
  NS_IMETHOD Encode(PRUint16 aVersion, const nsAString & aMethodName, const nsAString & aTargetObjectURI, PRUint32 aHeaderBlockCount, nsISOAPHeaderBlock **aHeaderBlocks, PRUint32 aParameterCount, nsISOAPParameter **aParameters) { return !_to ? NS_ERROR_NULL_POINTER : _to->Encode(aVersion, aMethodName, aTargetObjectURI, aHeaderBlockCount, aHeaderBlocks, aParameterCount, aParameters); } \
  NS_IMETHOD GetHeaderBlocks(PRUint32 *aCount, nsISOAPHeaderBlock ***aHeaderBlocks) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHeaderBlocks(aCount, aHeaderBlocks); } \
  NS_IMETHOD GetParameters(PRBool aDocumentStyle, PRUint32 *aCount, nsISOAPParameter ***aParameters) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParameters(aDocumentStyle, aCount, aParameters); } \
  NS_IMETHOD GetEncoding(nsISOAPEncoding * *aEncoding) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEncoding(aEncoding); } \
  NS_IMETHOD SetEncoding(nsISOAPEncoding * aEncoding) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetEncoding(aEncoding); } \
  NS_IMETHOD GetActionURI(nsAString & aActionURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetActionURI(aActionURI); } \
  NS_IMETHOD SetActionURI(const nsAString & aActionURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetActionURI(aActionURI); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPMessage : public nsISOAPMessage
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPMESSAGE

  nsSOAPMessage();

private:
  ~nsSOAPMessage();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPMessage, nsISOAPMessage)

nsSOAPMessage::nsSOAPMessage()
{
  /* member initializers and constructor code */
}

nsSOAPMessage::~nsSOAPMessage()
{
  /* destructor code */
}

/* attribute nsIDOMDocument message; */
NS_IMETHODIMP nsSOAPMessage::GetMessage(nsIDOMDocument * *aMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPMessage::SetMessage(nsIDOMDocument * aMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement envelope; */
NS_IMETHODIMP nsSOAPMessage::GetEnvelope(nsIDOMElement * *aEnvelope)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short version; */
NS_IMETHODIMP nsSOAPMessage::GetVersion(PRUint16 *aVersion)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement header; */
NS_IMETHODIMP nsSOAPMessage::GetHeader(nsIDOMElement * *aHeader)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement body; */
NS_IMETHODIMP nsSOAPMessage::GetBody(nsIDOMElement * *aBody)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString methodName; */
NS_IMETHODIMP nsSOAPMessage::GetMethodName(nsAString & aMethodName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString targetObjectURI; */
NS_IMETHODIMP nsSOAPMessage::GetTargetObjectURI(nsAString & aTargetObjectURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void encode (in unsigned short aVersion, in AString aMethodName, in AString aTargetObjectURI, in unsigned long aHeaderBlockCount, [array, size_is (aHeaderBlockCount)] in nsISOAPHeaderBlock aHeaderBlocks, in unsigned long aParameterCount, [array, size_is (aParameterCount)] in nsISOAPParameter aParameters); */
NS_IMETHODIMP nsSOAPMessage::Encode(PRUint16 aVersion, const nsAString & aMethodName, const nsAString & aTargetObjectURI, PRUint32 aHeaderBlockCount, nsISOAPHeaderBlock **aHeaderBlocks, PRUint32 aParameterCount, nsISOAPParameter **aParameters)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getHeaderBlocks (out unsigned long aCount, [array, size_is (aCount), retval] out nsISOAPHeaderBlock aHeaderBlocks); */
NS_IMETHODIMP nsSOAPMessage::GetHeaderBlocks(PRUint32 *aCount, nsISOAPHeaderBlock ***aHeaderBlocks)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getParameters (in boolean aDocumentStyle, out unsigned long aCount, [array, size_is (aCount), retval] out nsISOAPParameter aParameters); */
NS_IMETHODIMP nsSOAPMessage::GetParameters(PRBool aDocumentStyle, PRUint32 *aCount, nsISOAPParameter ***aParameters)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISOAPEncoding encoding; */
NS_IMETHODIMP nsSOAPMessage::GetEncoding(nsISOAPEncoding * *aEncoding)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPMessage::SetEncoding(nsISOAPEncoding * aEncoding)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute AString actionURI; */
NS_IMETHODIMP nsSOAPMessage::GetActionURI(nsAString & aActionURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPMessage::SetActionURI(const nsAString & aActionURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISOAPMessage_h__ */
