/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPBlock.idl
 */

#ifndef __gen_nsISOAPBlock_h__
#define __gen_nsISOAPBlock_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMElement; /* forward declaration */

class nsIVariant; /* forward declaration */

class nsISOAPEncoding; /* forward declaration */

class nsISchemaType; /* forward declaration */

class nsISOAPAttachments; /* forward declaration */


/* starting interface:    nsISOAPBlock */
#define NS_ISOAPBLOCK_IID_STR "843afaa8-1dd2-11b2-8b0d-9b5d16fe64ea"

#define NS_ISOAPBLOCK_IID \
  {0x843afaa8, 0x1dd2, 0x11b2, \
    { 0x8b, 0x0d, 0x9b, 0x5d, 0x16, 0xfe, 0x64, 0xea }}

/**
 * This interface encapsulates an arbitrary block to be used
 * by the soap serialization or protocol.  It presents the
 * namespaceURI, name, encoding, schemaType, and value of the
 * block.  There are two different ways this may be used:
 * <p>1.  When created by a user for serialization, a value is set
 * which is then used to encode the message.  In this case, the
 * user sets the value (or element in the case of a literal
 * block) which is then encoded (unless it is the element) 
 * and incorporated into the document as it is encoded.
 * <p>2.  When requested by the user from a message that is to
 * be decoded.  In this case, an element is set on the block
 * which is automatically decoded whenever the value attribute is
 * accessed (possibly after the user sets the encoding or schemaType,
 * or for literal blocks, the user just accesses the element and
 * no decoding is performed.  For SOAP which attachments, hidden 
 * attachments may also be associated from the message to the block 
 * so that later decoding which relies on the attachments is possible.
 */
class NS_NO_VTABLE nsISOAPBlock : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPBLOCK_IID)

  /**
   *  Initialize the block for additional decoding information.
   *
   * @param aAttachments Attachments in case this refers to them.
   *
   * @param aVersion SOAP version for decoding special header attributes.
   */
  /* void init (in nsISOAPAttachments aAttachments, in unsigned short aVersion); */
  NS_IMETHOD Init(nsISOAPAttachments *aAttachments, PRUint16 aVersion) = 0;

  /**
   *  The namespace URI of the block.  Ignored if name is null.
   *  If this is modified, element is set to null and all 
   *  attributes computed from element revert to previous
   *  uncomputed values.  If element is set, this becomes computed.
   */
  /* attribute AString namespaceURI; */
  NS_IMETHOD GetNamespaceURI(nsAString & aNamespaceURI) = 0;
  NS_IMETHOD SetNamespaceURI(const nsAString & aNamespaceURI) = 0;

  /**
   * The name of the block. If the block is left unnamed, it
   * will be encoded using the element types defined in the SOAP-ENC
   * schema. For example, <code>&lt;SOAP-ENC:int&gt;45&lt;/SOAP-ENC:int&gt;
   * </code>.  If this is modified, element is set to null and all 
   * attributes computed from element revert to previous uncomputed
   * values.  If element is set, this becomes computed.
   */
  /* attribute AString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;
  NS_IMETHOD SetName(const nsAString & aName) = 0;

  /**
   * The encoding that was / will be applied to the
   * block.  If this is blank and element is non-null,
   * it becomes impossible to decode the block when
   * the value is requested.
   */
  /* attribute nsISOAPEncoding encoding; */
  NS_IMETHOD GetEncoding(nsISOAPEncoding * *aEncoding) = 0;
  NS_IMETHOD SetEncoding(nsISOAPEncoding * aEncoding) = 0;

  /**
   * The schema type used to encode or decode the
   *   block.  If this is null, then the default
   *   encoder or decoder may 
   */
  /* attribute nsISchemaType schemaType; */
  NS_IMETHOD GetSchemaType(nsISchemaType * *aSchemaType) = 0;
  NS_IMETHOD SetSchemaType(nsISchemaType * aSchemaType) = 0;

  /**
   *  The element which is the encoded value of this block.
   *  If this is set, value, namespaceURI, and name becomes a 
   *  computed attributes which are produced by decoding this 
   *  element.
   */
  /* attribute nsIDOMElement element; */
  NS_IMETHOD GetElement(nsIDOMElement * *aElement) = 0;
  NS_IMETHOD SetElement(nsIDOMElement * aElement) = 0;

  /**
   *  The native value which is the decoded value of 
   *  this block.  If this is modified, element is set
   *  to null and all attributes computed from element 
   *  revert to previous uncomputed values.  If element 
   *  is set, this becomes computed, relying on the 
   *  value of encoding and schemaType each time it is 
   *  computed.
   */
  /* attribute nsIVariant value; */
  NS_IMETHOD GetValue(nsIVariant * *aValue) = 0;
  NS_IMETHOD SetValue(nsIVariant * aValue) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPBLOCK \
  NS_IMETHOD Init(nsISOAPAttachments *aAttachments, PRUint16 aVersion); \
  NS_IMETHOD GetNamespaceURI(nsAString & aNamespaceURI); \
  NS_IMETHOD SetNamespaceURI(const nsAString & aNamespaceURI); \
  NS_IMETHOD GetName(nsAString & aName); \
  NS_IMETHOD SetName(const nsAString & aName); \
  NS_IMETHOD GetEncoding(nsISOAPEncoding * *aEncoding); \
  NS_IMETHOD SetEncoding(nsISOAPEncoding * aEncoding); \
  NS_IMETHOD GetSchemaType(nsISchemaType * *aSchemaType); \
  NS_IMETHOD SetSchemaType(nsISchemaType * aSchemaType); \
  NS_IMETHOD GetElement(nsIDOMElement * *aElement); \
  NS_IMETHOD SetElement(nsIDOMElement * aElement); \
  NS_IMETHOD GetValue(nsIVariant * *aValue); \
  NS_IMETHOD SetValue(nsIVariant * aValue); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPBLOCK(_to) \
  NS_IMETHOD Init(nsISOAPAttachments *aAttachments, PRUint16 aVersion) { return _to Init(aAttachments, aVersion); } \
  NS_IMETHOD GetNamespaceURI(nsAString & aNamespaceURI) { return _to GetNamespaceURI(aNamespaceURI); } \
  NS_IMETHOD SetNamespaceURI(const nsAString & aNamespaceURI) { return _to SetNamespaceURI(aNamespaceURI); } \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD SetName(const nsAString & aName) { return _to SetName(aName); } \
  NS_IMETHOD GetEncoding(nsISOAPEncoding * *aEncoding) { return _to GetEncoding(aEncoding); } \
  NS_IMETHOD SetEncoding(nsISOAPEncoding * aEncoding) { return _to SetEncoding(aEncoding); } \
  NS_IMETHOD GetSchemaType(nsISchemaType * *aSchemaType) { return _to GetSchemaType(aSchemaType); } \
  NS_IMETHOD SetSchemaType(nsISchemaType * aSchemaType) { return _to SetSchemaType(aSchemaType); } \
  NS_IMETHOD GetElement(nsIDOMElement * *aElement) { return _to GetElement(aElement); } \
  NS_IMETHOD SetElement(nsIDOMElement * aElement) { return _to SetElement(aElement); } \
  NS_IMETHOD GetValue(nsIVariant * *aValue) { return _to GetValue(aValue); } \
  NS_IMETHOD SetValue(nsIVariant * aValue) { return _to SetValue(aValue); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPBLOCK(_to) \
  NS_IMETHOD Init(nsISOAPAttachments *aAttachments, PRUint16 aVersion) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aAttachments, aVersion); } \
  NS_IMETHOD GetNamespaceURI(nsAString & aNamespaceURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNamespaceURI(aNamespaceURI); } \
  NS_IMETHOD SetNamespaceURI(const nsAString & aNamespaceURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetNamespaceURI(aNamespaceURI); } \
  NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD SetName(const nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetName(aName); } \
  NS_IMETHOD GetEncoding(nsISOAPEncoding * *aEncoding) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEncoding(aEncoding); } \
  NS_IMETHOD SetEncoding(nsISOAPEncoding * aEncoding) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetEncoding(aEncoding); } \
  NS_IMETHOD GetSchemaType(nsISchemaType * *aSchemaType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSchemaType(aSchemaType); } \
  NS_IMETHOD SetSchemaType(nsISchemaType * aSchemaType) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSchemaType(aSchemaType); } \
  NS_IMETHOD GetElement(nsIDOMElement * *aElement) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetElement(aElement); } \
  NS_IMETHOD SetElement(nsIDOMElement * aElement) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetElement(aElement); } \
  NS_IMETHOD GetValue(nsIVariant * *aValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetValue(aValue); } \
  NS_IMETHOD SetValue(nsIVariant * aValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetValue(aValue); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPBlock : public nsISOAPBlock
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPBLOCK

  nsSOAPBlock();

private:
  ~nsSOAPBlock();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPBlock, nsISOAPBlock)

nsSOAPBlock::nsSOAPBlock()
{
  /* member initializers and constructor code */
}

nsSOAPBlock::~nsSOAPBlock()
{
  /* destructor code */
}

/* void init (in nsISOAPAttachments aAttachments, in unsigned short aVersion); */
NS_IMETHODIMP nsSOAPBlock::Init(nsISOAPAttachments *aAttachments, PRUint16 aVersion)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute AString namespaceURI; */
NS_IMETHODIMP nsSOAPBlock::GetNamespaceURI(nsAString & aNamespaceURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPBlock::SetNamespaceURI(const nsAString & aNamespaceURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute AString name; */
NS_IMETHODIMP nsSOAPBlock::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPBlock::SetName(const nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISOAPEncoding encoding; */
NS_IMETHODIMP nsSOAPBlock::GetEncoding(nsISOAPEncoding * *aEncoding)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPBlock::SetEncoding(nsISOAPEncoding * aEncoding)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISchemaType schemaType; */
NS_IMETHODIMP nsSOAPBlock::GetSchemaType(nsISchemaType * *aSchemaType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPBlock::SetSchemaType(nsISchemaType * aSchemaType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIDOMElement element; */
NS_IMETHODIMP nsSOAPBlock::GetElement(nsIDOMElement * *aElement)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPBlock::SetElement(nsIDOMElement * aElement)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIVariant value; */
NS_IMETHODIMP nsSOAPBlock::GetValue(nsIVariant * *aValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPBlock::SetValue(nsIVariant * aValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISOAPBlock_h__ */
