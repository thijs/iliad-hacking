/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISchema.idl
 */

#ifndef __gen_nsISchema_h__
#define __gen_nsISchema_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIWebServiceErrorHandler_h__
#include "nsIWebServiceErrorHandler.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISchema; /* forward declaration */

class nsISchemaType; /* forward declaration */

class nsISchemaSimpleType; /* forward declaration */

class nsISchemaBuiltinType; /* forward declaration */

class nsISchemaListType; /* forward declaration */

class nsISchemaUnionType; /* forward declaration */

class nsISchemaRestrictionType; /* forward declaration */

class nsISchemaComplexType; /* forward declaration */

class nsISchemaParticle; /* forward declaration */

class nsISchemaModelGroup; /* forward declaration */

class nsISchemaAnyParticle; /* forward declaration */

class nsISchemaElement; /* forward declaration */

class nsISchemaAttributeComponent; /* forward declaration */

class nsISchemaAttribute; /* forward declaration */

class nsISchemaAttributeGroup; /* forward declaration */

class nsISchemaAnyAttribute; /* forward declaration */

class nsISchemaFacet; /* forward declaration */


/* starting interface:    nsISchemaCollection */
#define NS_ISCHEMACOLLECTION_IID_STR "427c5511-941b-48c0-9abc-8ec9ea5d964b"

#define NS_ISCHEMACOLLECTION_IID \
  {0x427c5511, 0x941b, 0x48c0, \
    { 0x9a, 0xbc, 0x8e, 0xc9, 0xea, 0x5d, 0x96, 0x4b }}

/**
 * The collection of loaded schemas. If a schema references other 
 * schemas (generally through an import), these will be included
 * in the corresponding collection. 
 */
class NS_NO_VTABLE nsISchemaCollection : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMACOLLECTION_IID)

  /* nsISchema getSchema (in AString targetNamespace); */
  NS_IMETHOD GetSchema(const nsAString & targetNamespace, nsISchema **_retval) = 0;

  /* nsISchemaElement getElement (in AString name, in AString aNamespace); */
  NS_IMETHOD GetElement(const nsAString & name, const nsAString & aNamespace, nsISchemaElement **_retval) = 0;

  /* nsISchemaAttribute getAttribute (in AString name, in AString aNamespace); */
  NS_IMETHOD GetAttribute(const nsAString & name, const nsAString & aNamespace, nsISchemaAttribute **_retval) = 0;

  /* nsISchemaType getType (in AString name, in AString aNamespace); */
  NS_IMETHOD GetType(const nsAString & name, const nsAString & aNamespace, nsISchemaType **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMACOLLECTION \
  NS_IMETHOD GetSchema(const nsAString & targetNamespace, nsISchema **_retval); \
  NS_IMETHOD GetElement(const nsAString & name, const nsAString & aNamespace, nsISchemaElement **_retval); \
  NS_IMETHOD GetAttribute(const nsAString & name, const nsAString & aNamespace, nsISchemaAttribute **_retval); \
  NS_IMETHOD GetType(const nsAString & name, const nsAString & aNamespace, nsISchemaType **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMACOLLECTION(_to) \
  NS_IMETHOD GetSchema(const nsAString & targetNamespace, nsISchema **_retval) { return _to GetSchema(targetNamespace, _retval); } \
  NS_IMETHOD GetElement(const nsAString & name, const nsAString & aNamespace, nsISchemaElement **_retval) { return _to GetElement(name, aNamespace, _retval); } \
  NS_IMETHOD GetAttribute(const nsAString & name, const nsAString & aNamespace, nsISchemaAttribute **_retval) { return _to GetAttribute(name, aNamespace, _retval); } \
  NS_IMETHOD GetType(const nsAString & name, const nsAString & aNamespace, nsISchemaType **_retval) { return _to GetType(name, aNamespace, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMACOLLECTION(_to) \
  NS_IMETHOD GetSchema(const nsAString & targetNamespace, nsISchema **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSchema(targetNamespace, _retval); } \
  NS_IMETHOD GetElement(const nsAString & name, const nsAString & aNamespace, nsISchemaElement **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetElement(name, aNamespace, _retval); } \
  NS_IMETHOD GetAttribute(const nsAString & name, const nsAString & aNamespace, nsISchemaAttribute **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttribute(name, aNamespace, _retval); } \
  NS_IMETHOD GetType(const nsAString & name, const nsAString & aNamespace, nsISchemaType **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(name, aNamespace, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaCollection : public nsISchemaCollection
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMACOLLECTION

  nsSchemaCollection();

private:
  ~nsSchemaCollection();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaCollection, nsISchemaCollection)

nsSchemaCollection::nsSchemaCollection()
{
  /* member initializers and constructor code */
}

nsSchemaCollection::~nsSchemaCollection()
{
  /* destructor code */
}

/* nsISchema getSchema (in AString targetNamespace); */
NS_IMETHODIMP nsSchemaCollection::GetSchema(const nsAString & targetNamespace, nsISchema **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaElement getElement (in AString name, in AString aNamespace); */
NS_IMETHODIMP nsSchemaCollection::GetElement(const nsAString & name, const nsAString & aNamespace, nsISchemaElement **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaAttribute getAttribute (in AString name, in AString aNamespace); */
NS_IMETHODIMP nsSchemaCollection::GetAttribute(const nsAString & name, const nsAString & aNamespace, nsISchemaAttribute **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaType getType (in AString name, in AString aNamespace); */
NS_IMETHODIMP nsSchemaCollection::GetType(const nsAString & name, const nsAString & aNamespace, nsISchemaType **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaComponent */
#define NS_ISCHEMACOMPONENT_IID_STR "5caaa64e-e191-11d8-842a-000393b6661a"

#define NS_ISCHEMACOMPONENT_IID \
  {0x5caaa64e, 0xe191, 0x11d8, \
    { 0x84, 0x2a, 0x00, 0x03, 0x93, 0xb6, 0x66, 0x1a }}

class NS_NO_VTABLE nsISchemaComponent : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMACOMPONENT_IID)

  /* readonly attribute AString targetNamespace; */
  NS_IMETHOD GetTargetNamespace(nsAString & aTargetNamespace) = 0;

  /* void resolve (in nsIWebServiceErrorHandler aErrorHandler); */
  NS_IMETHOD Resolve(nsIWebServiceErrorHandler *aErrorHandler) = 0;

  /* void clear (); */
  NS_IMETHOD Clear(void) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMACOMPONENT \
  NS_IMETHOD GetTargetNamespace(nsAString & aTargetNamespace); \
  NS_IMETHOD Resolve(nsIWebServiceErrorHandler *aErrorHandler); \
  NS_IMETHOD Clear(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMACOMPONENT(_to) \
  NS_IMETHOD GetTargetNamespace(nsAString & aTargetNamespace) { return _to GetTargetNamespace(aTargetNamespace); } \
  NS_IMETHOD Resolve(nsIWebServiceErrorHandler *aErrorHandler) { return _to Resolve(aErrorHandler); } \
  NS_IMETHOD Clear(void) { return _to Clear(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMACOMPONENT(_to) \
  NS_IMETHOD GetTargetNamespace(nsAString & aTargetNamespace) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTargetNamespace(aTargetNamespace); } \
  NS_IMETHOD Resolve(nsIWebServiceErrorHandler *aErrorHandler) { return !_to ? NS_ERROR_NULL_POINTER : _to->Resolve(aErrorHandler); } \
  NS_IMETHOD Clear(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Clear(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaComponent : public nsISchemaComponent
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMACOMPONENT

  nsSchemaComponent();

private:
  ~nsSchemaComponent();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaComponent, nsISchemaComponent)

nsSchemaComponent::nsSchemaComponent()
{
  /* member initializers and constructor code */
}

nsSchemaComponent::~nsSchemaComponent()
{
  /* destructor code */
}

/* readonly attribute AString targetNamespace; */
NS_IMETHODIMP nsSchemaComponent::GetTargetNamespace(nsAString & aTargetNamespace)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void resolve (in nsIWebServiceErrorHandler aErrorHandler); */
NS_IMETHODIMP nsSchemaComponent::Resolve(nsIWebServiceErrorHandler *aErrorHandler)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void clear (); */
NS_IMETHODIMP nsSchemaComponent::Clear()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchema */
#define NS_ISCHEMA_IID_STR "3c14a021-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMA_IID \
  {0x3c14a021, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchema : public nsISchemaComponent {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMA_IID)

  /* readonly attribute AString schemaNamespace; */
  NS_IMETHOD GetSchemaNamespace(nsAString & aSchemaNamespace) = 0;

  /* readonly attribute PRUint32 typeCount; */
  NS_IMETHOD GetTypeCount(PRUint32 *aTypeCount) = 0;

  /* nsISchemaType getTypeByIndex (in PRUint32 index); */
  NS_IMETHOD GetTypeByIndex(PRUint32 index, nsISchemaType **_retval) = 0;

  /* nsISchemaType getTypeByName (in AString name); */
  NS_IMETHOD GetTypeByName(const nsAString & name, nsISchemaType **_retval) = 0;

  /* readonly attribute PRUint32 attributeCount; */
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount) = 0;

  /* nsISchemaAttribute getAttributeByIndex (in PRUint32 index); */
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttribute **_retval) = 0;

  /* nsISchemaAttribute getAttributeByName (in AString name); */
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttribute **_retval) = 0;

  /* readonly attribute PRUint32 elementCount; */
  NS_IMETHOD GetElementCount(PRUint32 *aElementCount) = 0;

  /* nsISchemaElement getElementByIndex (in PRUint32 index); */
  NS_IMETHOD GetElementByIndex(PRUint32 index, nsISchemaElement **_retval) = 0;

  /* nsISchemaElement getElementByName (in AString name); */
  NS_IMETHOD GetElementByName(const nsAString & name, nsISchemaElement **_retval) = 0;

  /* readonly attribute PRUint32 attributeGroupCount; */
  NS_IMETHOD GetAttributeGroupCount(PRUint32 *aAttributeGroupCount) = 0;

  /* nsISchemaAttributeGroup getAttributeGroupByIndex (in PRUint32 index); */
  NS_IMETHOD GetAttributeGroupByIndex(PRUint32 index, nsISchemaAttributeGroup **_retval) = 0;

  /* nsISchemaAttributeGroup getAttributeGroupByName (in AString name); */
  NS_IMETHOD GetAttributeGroupByName(const nsAString & name, nsISchemaAttributeGroup **_retval) = 0;

  /* readonly attribute PRUint32 modelGroupCount; */
  NS_IMETHOD GetModelGroupCount(PRUint32 *aModelGroupCount) = 0;

  /* nsISchemaModelGroup getModelGroupByIndex (in PRUint32 index); */
  NS_IMETHOD GetModelGroupByIndex(PRUint32 index, nsISchemaModelGroup **_retval) = 0;

  /* nsISchemaModelGroup getModelGroupByName (in AString name); */
  NS_IMETHOD GetModelGroupByName(const nsAString & name, nsISchemaModelGroup **_retval) = 0;

  /* readonly attribute nsISchemaCollection collection; */
  NS_IMETHOD GetCollection(nsISchemaCollection * *aCollection) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMA \
  NS_IMETHOD GetSchemaNamespace(nsAString & aSchemaNamespace); \
  NS_IMETHOD GetTypeCount(PRUint32 *aTypeCount); \
  NS_IMETHOD GetTypeByIndex(PRUint32 index, nsISchemaType **_retval); \
  NS_IMETHOD GetTypeByName(const nsAString & name, nsISchemaType **_retval); \
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount); \
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttribute **_retval); \
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttribute **_retval); \
  NS_IMETHOD GetElementCount(PRUint32 *aElementCount); \
  NS_IMETHOD GetElementByIndex(PRUint32 index, nsISchemaElement **_retval); \
  NS_IMETHOD GetElementByName(const nsAString & name, nsISchemaElement **_retval); \
  NS_IMETHOD GetAttributeGroupCount(PRUint32 *aAttributeGroupCount); \
  NS_IMETHOD GetAttributeGroupByIndex(PRUint32 index, nsISchemaAttributeGroup **_retval); \
  NS_IMETHOD GetAttributeGroupByName(const nsAString & name, nsISchemaAttributeGroup **_retval); \
  NS_IMETHOD GetModelGroupCount(PRUint32 *aModelGroupCount); \
  NS_IMETHOD GetModelGroupByIndex(PRUint32 index, nsISchemaModelGroup **_retval); \
  NS_IMETHOD GetModelGroupByName(const nsAString & name, nsISchemaModelGroup **_retval); \
  NS_IMETHOD GetCollection(nsISchemaCollection * *aCollection); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMA(_to) \
  NS_IMETHOD GetSchemaNamespace(nsAString & aSchemaNamespace) { return _to GetSchemaNamespace(aSchemaNamespace); } \
  NS_IMETHOD GetTypeCount(PRUint32 *aTypeCount) { return _to GetTypeCount(aTypeCount); } \
  NS_IMETHOD GetTypeByIndex(PRUint32 index, nsISchemaType **_retval) { return _to GetTypeByIndex(index, _retval); } \
  NS_IMETHOD GetTypeByName(const nsAString & name, nsISchemaType **_retval) { return _to GetTypeByName(name, _retval); } \
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount) { return _to GetAttributeCount(aAttributeCount); } \
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttribute **_retval) { return _to GetAttributeByIndex(index, _retval); } \
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttribute **_retval) { return _to GetAttributeByName(name, _retval); } \
  NS_IMETHOD GetElementCount(PRUint32 *aElementCount) { return _to GetElementCount(aElementCount); } \
  NS_IMETHOD GetElementByIndex(PRUint32 index, nsISchemaElement **_retval) { return _to GetElementByIndex(index, _retval); } \
  NS_IMETHOD GetElementByName(const nsAString & name, nsISchemaElement **_retval) { return _to GetElementByName(name, _retval); } \
  NS_IMETHOD GetAttributeGroupCount(PRUint32 *aAttributeGroupCount) { return _to GetAttributeGroupCount(aAttributeGroupCount); } \
  NS_IMETHOD GetAttributeGroupByIndex(PRUint32 index, nsISchemaAttributeGroup **_retval) { return _to GetAttributeGroupByIndex(index, _retval); } \
  NS_IMETHOD GetAttributeGroupByName(const nsAString & name, nsISchemaAttributeGroup **_retval) { return _to GetAttributeGroupByName(name, _retval); } \
  NS_IMETHOD GetModelGroupCount(PRUint32 *aModelGroupCount) { return _to GetModelGroupCount(aModelGroupCount); } \
  NS_IMETHOD GetModelGroupByIndex(PRUint32 index, nsISchemaModelGroup **_retval) { return _to GetModelGroupByIndex(index, _retval); } \
  NS_IMETHOD GetModelGroupByName(const nsAString & name, nsISchemaModelGroup **_retval) { return _to GetModelGroupByName(name, _retval); } \
  NS_IMETHOD GetCollection(nsISchemaCollection * *aCollection) { return _to GetCollection(aCollection); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMA(_to) \
  NS_IMETHOD GetSchemaNamespace(nsAString & aSchemaNamespace) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSchemaNamespace(aSchemaNamespace); } \
  NS_IMETHOD GetTypeCount(PRUint32 *aTypeCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTypeCount(aTypeCount); } \
  NS_IMETHOD GetTypeByIndex(PRUint32 index, nsISchemaType **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTypeByIndex(index, _retval); } \
  NS_IMETHOD GetTypeByName(const nsAString & name, nsISchemaType **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTypeByName(name, _retval); } \
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeCount(aAttributeCount); } \
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttribute **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeByIndex(index, _retval); } \
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttribute **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeByName(name, _retval); } \
  NS_IMETHOD GetElementCount(PRUint32 *aElementCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetElementCount(aElementCount); } \
  NS_IMETHOD GetElementByIndex(PRUint32 index, nsISchemaElement **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetElementByIndex(index, _retval); } \
  NS_IMETHOD GetElementByName(const nsAString & name, nsISchemaElement **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetElementByName(name, _retval); } \
  NS_IMETHOD GetAttributeGroupCount(PRUint32 *aAttributeGroupCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeGroupCount(aAttributeGroupCount); } \
  NS_IMETHOD GetAttributeGroupByIndex(PRUint32 index, nsISchemaAttributeGroup **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeGroupByIndex(index, _retval); } \
  NS_IMETHOD GetAttributeGroupByName(const nsAString & name, nsISchemaAttributeGroup **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeGroupByName(name, _retval); } \
  NS_IMETHOD GetModelGroupCount(PRUint32 *aModelGroupCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetModelGroupCount(aModelGroupCount); } \
  NS_IMETHOD GetModelGroupByIndex(PRUint32 index, nsISchemaModelGroup **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetModelGroupByIndex(index, _retval); } \
  NS_IMETHOD GetModelGroupByName(const nsAString & name, nsISchemaModelGroup **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetModelGroupByName(name, _retval); } \
  NS_IMETHOD GetCollection(nsISchemaCollection * *aCollection) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCollection(aCollection); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchema : public nsISchema
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMA

  nsSchema();

private:
  ~nsSchema();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchema, nsISchema)

nsSchema::nsSchema()
{
  /* member initializers and constructor code */
}

nsSchema::~nsSchema()
{
  /* destructor code */
}

/* readonly attribute AString schemaNamespace; */
NS_IMETHODIMP nsSchema::GetSchemaNamespace(nsAString & aSchemaNamespace)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 typeCount; */
NS_IMETHODIMP nsSchema::GetTypeCount(PRUint32 *aTypeCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaType getTypeByIndex (in PRUint32 index); */
NS_IMETHODIMP nsSchema::GetTypeByIndex(PRUint32 index, nsISchemaType **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaType getTypeByName (in AString name); */
NS_IMETHODIMP nsSchema::GetTypeByName(const nsAString & name, nsISchemaType **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 attributeCount; */
NS_IMETHODIMP nsSchema::GetAttributeCount(PRUint32 *aAttributeCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaAttribute getAttributeByIndex (in PRUint32 index); */
NS_IMETHODIMP nsSchema::GetAttributeByIndex(PRUint32 index, nsISchemaAttribute **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaAttribute getAttributeByName (in AString name); */
NS_IMETHODIMP nsSchema::GetAttributeByName(const nsAString & name, nsISchemaAttribute **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 elementCount; */
NS_IMETHODIMP nsSchema::GetElementCount(PRUint32 *aElementCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaElement getElementByIndex (in PRUint32 index); */
NS_IMETHODIMP nsSchema::GetElementByIndex(PRUint32 index, nsISchemaElement **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaElement getElementByName (in AString name); */
NS_IMETHODIMP nsSchema::GetElementByName(const nsAString & name, nsISchemaElement **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 attributeGroupCount; */
NS_IMETHODIMP nsSchema::GetAttributeGroupCount(PRUint32 *aAttributeGroupCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaAttributeGroup getAttributeGroupByIndex (in PRUint32 index); */
NS_IMETHODIMP nsSchema::GetAttributeGroupByIndex(PRUint32 index, nsISchemaAttributeGroup **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaAttributeGroup getAttributeGroupByName (in AString name); */
NS_IMETHODIMP nsSchema::GetAttributeGroupByName(const nsAString & name, nsISchemaAttributeGroup **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 modelGroupCount; */
NS_IMETHODIMP nsSchema::GetModelGroupCount(PRUint32 *aModelGroupCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaModelGroup getModelGroupByIndex (in PRUint32 index); */
NS_IMETHODIMP nsSchema::GetModelGroupByIndex(PRUint32 index, nsISchemaModelGroup **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaModelGroup getModelGroupByName (in AString name); */
NS_IMETHODIMP nsSchema::GetModelGroupByName(const nsAString & name, nsISchemaModelGroup **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISchemaCollection collection; */
NS_IMETHODIMP nsSchema::GetCollection(nsISchemaCollection * *aCollection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaType */
#define NS_ISCHEMATYPE_IID_STR "3c14a022-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMATYPE_IID \
  {0x3c14a022, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaType : public nsISchemaComponent {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMATYPE_IID)

  enum { SCHEMA_TYPE_SIMPLE = 1U };

  enum { SCHEMA_TYPE_COMPLEX = 2U };

  enum { SCHEMA_TYPE_PLACEHOLDER = 3U };

  /* readonly attribute AString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;

  /* readonly attribute unsigned short schemaType; */
  NS_IMETHOD GetSchemaType(PRUint16 *aSchemaType) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMATYPE \
  NS_IMETHOD GetName(nsAString & aName); \
  NS_IMETHOD GetSchemaType(PRUint16 *aSchemaType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMATYPE(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD GetSchemaType(PRUint16 *aSchemaType) { return _to GetSchemaType(aSchemaType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMATYPE(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetSchemaType(PRUint16 *aSchemaType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSchemaType(aSchemaType); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaType : public nsISchemaType
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMATYPE

  nsSchemaType();

private:
  ~nsSchemaType();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaType, nsISchemaType)

nsSchemaType::nsSchemaType()
{
  /* member initializers and constructor code */
}

nsSchemaType::~nsSchemaType()
{
  /* destructor code */
}

/* readonly attribute AString name; */
NS_IMETHODIMP nsSchemaType::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short schemaType; */
NS_IMETHODIMP nsSchemaType::GetSchemaType(PRUint16 *aSchemaType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaSimpleType */
#define NS_ISCHEMASIMPLETYPE_IID_STR "3c14a023-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMASIMPLETYPE_IID \
  {0x3c14a023, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaSimpleType : public nsISchemaType {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMASIMPLETYPE_IID)

  enum { SIMPLE_TYPE_BUILTIN = 1U };

  enum { SIMPLE_TYPE_LIST = 2U };

  enum { SIMPLE_TYPE_UNION = 3U };

  enum { SIMPLE_TYPE_RESTRICTION = 4U };

  /* readonly attribute unsigned short simpleType; */
  NS_IMETHOD GetSimpleType(PRUint16 *aSimpleType) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMASIMPLETYPE \
  NS_IMETHOD GetSimpleType(PRUint16 *aSimpleType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMASIMPLETYPE(_to) \
  NS_IMETHOD GetSimpleType(PRUint16 *aSimpleType) { return _to GetSimpleType(aSimpleType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMASIMPLETYPE(_to) \
  NS_IMETHOD GetSimpleType(PRUint16 *aSimpleType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSimpleType(aSimpleType); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaSimpleType : public nsISchemaSimpleType
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMASIMPLETYPE

  nsSchemaSimpleType();

private:
  ~nsSchemaSimpleType();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaSimpleType, nsISchemaSimpleType)

nsSchemaSimpleType::nsSchemaSimpleType()
{
  /* member initializers and constructor code */
}

nsSchemaSimpleType::~nsSchemaSimpleType()
{
  /* destructor code */
}

/* readonly attribute unsigned short simpleType; */
NS_IMETHODIMP nsSchemaSimpleType::GetSimpleType(PRUint16 *aSimpleType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaBuiltinType */
#define NS_ISCHEMABUILTINTYPE_IID_STR "3c14a024-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMABUILTINTYPE_IID \
  {0x3c14a024, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaBuiltinType : public nsISchemaSimpleType {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMABUILTINTYPE_IID)

  enum { BUILTIN_TYPE_ANYTYPE = 1U };

  enum { BUILTIN_TYPE_STRING = 2U };

  enum { BUILTIN_TYPE_NORMALIZED_STRING = 3U };

  enum { BUILTIN_TYPE_TOKEN = 4U };

  enum { BUILTIN_TYPE_BYTE = 5U };

  enum { BUILTIN_TYPE_UNSIGNEDBYTE = 6U };

  enum { BUILTIN_TYPE_BASE64BINARY = 7U };

  enum { BUILTIN_TYPE_HEXBINARY = 8U };

  enum { BUILTIN_TYPE_INTEGER = 9U };

  enum { BUILTIN_TYPE_POSITIVEINTEGER = 10U };

  enum { BUILTIN_TYPE_NEGATIVEINTEGER = 11U };

  enum { BUILTIN_TYPE_NONNEGATIVEINTEGER = 12U };

  enum { BUILTIN_TYPE_NONPOSITIVEINTEGER = 13U };

  enum { BUILTIN_TYPE_INT = 14U };

  enum { BUILTIN_TYPE_UNSIGNEDINT = 15U };

  enum { BUILTIN_TYPE_LONG = 16U };

  enum { BUILTIN_TYPE_UNSIGNEDLONG = 17U };

  enum { BUILTIN_TYPE_SHORT = 18U };

  enum { BUILTIN_TYPE_UNSIGNEDSHORT = 19U };

  enum { BUILTIN_TYPE_DECIMAL = 20U };

  enum { BUILTIN_TYPE_FLOAT = 21U };

  enum { BUILTIN_TYPE_DOUBLE = 22U };

  enum { BUILTIN_TYPE_BOOLEAN = 23U };

  enum { BUILTIN_TYPE_TIME = 24U };

  enum { BUILTIN_TYPE_DATETIME = 25U };

  enum { BUILTIN_TYPE_DURATION = 26U };

  enum { BUILTIN_TYPE_DATE = 27U };

  enum { BUILTIN_TYPE_GMONTH = 28U };

  enum { BUILTIN_TYPE_GYEAR = 29U };

  enum { BUILTIN_TYPE_GYEARMONTH = 30U };

  enum { BUILTIN_TYPE_GDAY = 31U };

  enum { BUILTIN_TYPE_GMONTHDAY = 32U };

  enum { BUILTIN_TYPE_NAME = 33U };

  enum { BUILTIN_TYPE_QNAME = 34U };

  enum { BUILTIN_TYPE_NCNAME = 35U };

  enum { BUILTIN_TYPE_ANYURI = 36U };

  enum { BUILTIN_TYPE_LANGUAGE = 37U };

  enum { BUILTIN_TYPE_ID = 38U };

  enum { BUILTIN_TYPE_IDREF = 39U };

  enum { BUILTIN_TYPE_IDREFS = 40U };

  enum { BUILTIN_TYPE_ENTITY = 41U };

  enum { BUILTIN_TYPE_ENTITIES = 42U };

  enum { BUILTIN_TYPE_NOTATION = 43U };

  enum { BUILTIN_TYPE_NMTOKEN = 44U };

  enum { BUILTIN_TYPE_NMTOKENS = 45U };

  /* readonly attribute unsigned short builtinType; */
  NS_IMETHOD GetBuiltinType(PRUint16 *aBuiltinType) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMABUILTINTYPE \
  NS_IMETHOD GetBuiltinType(PRUint16 *aBuiltinType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMABUILTINTYPE(_to) \
  NS_IMETHOD GetBuiltinType(PRUint16 *aBuiltinType) { return _to GetBuiltinType(aBuiltinType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMABUILTINTYPE(_to) \
  NS_IMETHOD GetBuiltinType(PRUint16 *aBuiltinType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBuiltinType(aBuiltinType); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaBuiltinType : public nsISchemaBuiltinType
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMABUILTINTYPE

  nsSchemaBuiltinType();

private:
  ~nsSchemaBuiltinType();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaBuiltinType, nsISchemaBuiltinType)

nsSchemaBuiltinType::nsSchemaBuiltinType()
{
  /* member initializers and constructor code */
}

nsSchemaBuiltinType::~nsSchemaBuiltinType()
{
  /* destructor code */
}

/* readonly attribute unsigned short builtinType; */
NS_IMETHODIMP nsSchemaBuiltinType::GetBuiltinType(PRUint16 *aBuiltinType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaListType */
#define NS_ISCHEMALISTTYPE_IID_STR "3c14a025-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMALISTTYPE_IID \
  {0x3c14a025, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaListType : public nsISchemaSimpleType {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMALISTTYPE_IID)

  /* readonly attribute nsISchemaSimpleType listType; */
  NS_IMETHOD GetListType(nsISchemaSimpleType * *aListType) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMALISTTYPE \
  NS_IMETHOD GetListType(nsISchemaSimpleType * *aListType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMALISTTYPE(_to) \
  NS_IMETHOD GetListType(nsISchemaSimpleType * *aListType) { return _to GetListType(aListType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMALISTTYPE(_to) \
  NS_IMETHOD GetListType(nsISchemaSimpleType * *aListType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetListType(aListType); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaListType : public nsISchemaListType
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMALISTTYPE

  nsSchemaListType();

private:
  ~nsSchemaListType();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaListType, nsISchemaListType)

nsSchemaListType::nsSchemaListType()
{
  /* member initializers and constructor code */
}

nsSchemaListType::~nsSchemaListType()
{
  /* destructor code */
}

/* readonly attribute nsISchemaSimpleType listType; */
NS_IMETHODIMP nsSchemaListType::GetListType(nsISchemaSimpleType * *aListType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaUnionType */
#define NS_ISCHEMAUNIONTYPE_IID_STR "3c14a026-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAUNIONTYPE_IID \
  {0x3c14a026, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaUnionType : public nsISchemaSimpleType {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAUNIONTYPE_IID)

  /* readonly attribute PRUint32 unionTypeCount; */
  NS_IMETHOD GetUnionTypeCount(PRUint32 *aUnionTypeCount) = 0;

  /* nsISchemaSimpleType getUnionType (in PRUint32 index); */
  NS_IMETHOD GetUnionType(PRUint32 index, nsISchemaSimpleType **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAUNIONTYPE \
  NS_IMETHOD GetUnionTypeCount(PRUint32 *aUnionTypeCount); \
  NS_IMETHOD GetUnionType(PRUint32 index, nsISchemaSimpleType **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAUNIONTYPE(_to) \
  NS_IMETHOD GetUnionTypeCount(PRUint32 *aUnionTypeCount) { return _to GetUnionTypeCount(aUnionTypeCount); } \
  NS_IMETHOD GetUnionType(PRUint32 index, nsISchemaSimpleType **_retval) { return _to GetUnionType(index, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAUNIONTYPE(_to) \
  NS_IMETHOD GetUnionTypeCount(PRUint32 *aUnionTypeCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetUnionTypeCount(aUnionTypeCount); } \
  NS_IMETHOD GetUnionType(PRUint32 index, nsISchemaSimpleType **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetUnionType(index, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaUnionType : public nsISchemaUnionType
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAUNIONTYPE

  nsSchemaUnionType();

private:
  ~nsSchemaUnionType();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaUnionType, nsISchemaUnionType)

nsSchemaUnionType::nsSchemaUnionType()
{
  /* member initializers and constructor code */
}

nsSchemaUnionType::~nsSchemaUnionType()
{
  /* destructor code */
}

/* readonly attribute PRUint32 unionTypeCount; */
NS_IMETHODIMP nsSchemaUnionType::GetUnionTypeCount(PRUint32 *aUnionTypeCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaSimpleType getUnionType (in PRUint32 index); */
NS_IMETHODIMP nsSchemaUnionType::GetUnionType(PRUint32 index, nsISchemaSimpleType **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaRestrictionType */
#define NS_ISCHEMARESTRICTIONTYPE_IID_STR "3c14a027-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMARESTRICTIONTYPE_IID \
  {0x3c14a027, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaRestrictionType : public nsISchemaSimpleType {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMARESTRICTIONTYPE_IID)

  /* readonly attribute nsISchemaSimpleType baseType; */
  NS_IMETHOD GetBaseType(nsISchemaSimpleType * *aBaseType) = 0;

  /* readonly attribute PRUint32 facetCount; */
  NS_IMETHOD GetFacetCount(PRUint32 *aFacetCount) = 0;

  /* nsISchemaFacet getFacet (in PRUint32 index); */
  NS_IMETHOD GetFacet(PRUint32 index, nsISchemaFacet **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMARESTRICTIONTYPE \
  NS_IMETHOD GetBaseType(nsISchemaSimpleType * *aBaseType); \
  NS_IMETHOD GetFacetCount(PRUint32 *aFacetCount); \
  NS_IMETHOD GetFacet(PRUint32 index, nsISchemaFacet **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMARESTRICTIONTYPE(_to) \
  NS_IMETHOD GetBaseType(nsISchemaSimpleType * *aBaseType) { return _to GetBaseType(aBaseType); } \
  NS_IMETHOD GetFacetCount(PRUint32 *aFacetCount) { return _to GetFacetCount(aFacetCount); } \
  NS_IMETHOD GetFacet(PRUint32 index, nsISchemaFacet **_retval) { return _to GetFacet(index, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMARESTRICTIONTYPE(_to) \
  NS_IMETHOD GetBaseType(nsISchemaSimpleType * *aBaseType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBaseType(aBaseType); } \
  NS_IMETHOD GetFacetCount(PRUint32 *aFacetCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFacetCount(aFacetCount); } \
  NS_IMETHOD GetFacet(PRUint32 index, nsISchemaFacet **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFacet(index, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaRestrictionType : public nsISchemaRestrictionType
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMARESTRICTIONTYPE

  nsSchemaRestrictionType();

private:
  ~nsSchemaRestrictionType();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaRestrictionType, nsISchemaRestrictionType)

nsSchemaRestrictionType::nsSchemaRestrictionType()
{
  /* member initializers and constructor code */
}

nsSchemaRestrictionType::~nsSchemaRestrictionType()
{
  /* destructor code */
}

/* readonly attribute nsISchemaSimpleType baseType; */
NS_IMETHODIMP nsSchemaRestrictionType::GetBaseType(nsISchemaSimpleType * *aBaseType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 facetCount; */
NS_IMETHODIMP nsSchemaRestrictionType::GetFacetCount(PRUint32 *aFacetCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaFacet getFacet (in PRUint32 index); */
NS_IMETHODIMP nsSchemaRestrictionType::GetFacet(PRUint32 index, nsISchemaFacet **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaComplexType */
#define NS_ISCHEMACOMPLEXTYPE_IID_STR "3c14a028-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMACOMPLEXTYPE_IID \
  {0x3c14a028, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaComplexType : public nsISchemaType {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMACOMPLEXTYPE_IID)

  enum { CONTENT_MODEL_EMPTY = 1U };

  enum { CONTENT_MODEL_SIMPLE = 2U };

  enum { CONTENT_MODEL_ELEMENT_ONLY = 3U };

  enum { CONTENT_MODEL_MIXED = 4U };

  enum { DERIVATION_EXTENSION_SIMPLE = 1U };

  enum { DERIVATION_RESTRICTION_SIMPLE = 2U };

  enum { DERIVATION_EXTENSION_COMPLEX = 3U };

  enum { DERIVATION_RESTRICTION_COMPLEX = 4U };

  enum { DERIVATION_SELF_CONTAINED = 5U };

  /* readonly attribute unsigned short contentModel; */
  NS_IMETHOD GetContentModel(PRUint16 *aContentModel) = 0;

  /* readonly attribute unsigned short derivation; */
  NS_IMETHOD GetDerivation(PRUint16 *aDerivation) = 0;

  /* readonly attribute nsISchemaType baseType; */
  NS_IMETHOD GetBaseType(nsISchemaType * *aBaseType) = 0;

  /* readonly attribute nsISchemaSimpleType simpleBaseType; */
  NS_IMETHOD GetSimpleBaseType(nsISchemaSimpleType * *aSimpleBaseType) = 0;

  /* readonly attribute nsISchemaModelGroup modelGroup; */
  NS_IMETHOD GetModelGroup(nsISchemaModelGroup * *aModelGroup) = 0;

  /* readonly attribute PRUint32 attributeCount; */
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount) = 0;

  /* nsISchemaAttributeComponent getAttributeByIndex (in PRUint32 index); */
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval) = 0;

  /* nsISchemaAttributeComponent getAttributeByName (in AString name); */
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval) = 0;

  /* readonly attribute boolean abstract; */
  NS_IMETHOD GetAbstract(PRBool *aAbstract) = 0;

  /* readonly attribute boolean isArray; */
  NS_IMETHOD GetIsArray(PRBool *aIsArray) = 0;

  /* readonly attribute nsISchemaType arrayType; */
  NS_IMETHOD GetArrayType(nsISchemaType * *aArrayType) = 0;

  /* readonly attribute PRUint32 arrayDimension; */
  NS_IMETHOD GetArrayDimension(PRUint32 *aArrayDimension) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMACOMPLEXTYPE \
  NS_IMETHOD GetContentModel(PRUint16 *aContentModel); \
  NS_IMETHOD GetDerivation(PRUint16 *aDerivation); \
  NS_IMETHOD GetBaseType(nsISchemaType * *aBaseType); \
  NS_IMETHOD GetSimpleBaseType(nsISchemaSimpleType * *aSimpleBaseType); \
  NS_IMETHOD GetModelGroup(nsISchemaModelGroup * *aModelGroup); \
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount); \
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval); \
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval); \
  NS_IMETHOD GetAbstract(PRBool *aAbstract); \
  NS_IMETHOD GetIsArray(PRBool *aIsArray); \
  NS_IMETHOD GetArrayType(nsISchemaType * *aArrayType); \
  NS_IMETHOD GetArrayDimension(PRUint32 *aArrayDimension); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMACOMPLEXTYPE(_to) \
  NS_IMETHOD GetContentModel(PRUint16 *aContentModel) { return _to GetContentModel(aContentModel); } \
  NS_IMETHOD GetDerivation(PRUint16 *aDerivation) { return _to GetDerivation(aDerivation); } \
  NS_IMETHOD GetBaseType(nsISchemaType * *aBaseType) { return _to GetBaseType(aBaseType); } \
  NS_IMETHOD GetSimpleBaseType(nsISchemaSimpleType * *aSimpleBaseType) { return _to GetSimpleBaseType(aSimpleBaseType); } \
  NS_IMETHOD GetModelGroup(nsISchemaModelGroup * *aModelGroup) { return _to GetModelGroup(aModelGroup); } \
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount) { return _to GetAttributeCount(aAttributeCount); } \
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval) { return _to GetAttributeByIndex(index, _retval); } \
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval) { return _to GetAttributeByName(name, _retval); } \
  NS_IMETHOD GetAbstract(PRBool *aAbstract) { return _to GetAbstract(aAbstract); } \
  NS_IMETHOD GetIsArray(PRBool *aIsArray) { return _to GetIsArray(aIsArray); } \
  NS_IMETHOD GetArrayType(nsISchemaType * *aArrayType) { return _to GetArrayType(aArrayType); } \
  NS_IMETHOD GetArrayDimension(PRUint32 *aArrayDimension) { return _to GetArrayDimension(aArrayDimension); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMACOMPLEXTYPE(_to) \
  NS_IMETHOD GetContentModel(PRUint16 *aContentModel) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetContentModel(aContentModel); } \
  NS_IMETHOD GetDerivation(PRUint16 *aDerivation) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDerivation(aDerivation); } \
  NS_IMETHOD GetBaseType(nsISchemaType * *aBaseType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBaseType(aBaseType); } \
  NS_IMETHOD GetSimpleBaseType(nsISchemaSimpleType * *aSimpleBaseType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSimpleBaseType(aSimpleBaseType); } \
  NS_IMETHOD GetModelGroup(nsISchemaModelGroup * *aModelGroup) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetModelGroup(aModelGroup); } \
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeCount(aAttributeCount); } \
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeByIndex(index, _retval); } \
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeByName(name, _retval); } \
  NS_IMETHOD GetAbstract(PRBool *aAbstract) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAbstract(aAbstract); } \
  NS_IMETHOD GetIsArray(PRBool *aIsArray) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsArray(aIsArray); } \
  NS_IMETHOD GetArrayType(nsISchemaType * *aArrayType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetArrayType(aArrayType); } \
  NS_IMETHOD GetArrayDimension(PRUint32 *aArrayDimension) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetArrayDimension(aArrayDimension); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaComplexType : public nsISchemaComplexType
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMACOMPLEXTYPE

  nsSchemaComplexType();

private:
  ~nsSchemaComplexType();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaComplexType, nsISchemaComplexType)

nsSchemaComplexType::nsSchemaComplexType()
{
  /* member initializers and constructor code */
}

nsSchemaComplexType::~nsSchemaComplexType()
{
  /* destructor code */
}

/* readonly attribute unsigned short contentModel; */
NS_IMETHODIMP nsSchemaComplexType::GetContentModel(PRUint16 *aContentModel)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short derivation; */
NS_IMETHODIMP nsSchemaComplexType::GetDerivation(PRUint16 *aDerivation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISchemaType baseType; */
NS_IMETHODIMP nsSchemaComplexType::GetBaseType(nsISchemaType * *aBaseType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISchemaSimpleType simpleBaseType; */
NS_IMETHODIMP nsSchemaComplexType::GetSimpleBaseType(nsISchemaSimpleType * *aSimpleBaseType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISchemaModelGroup modelGroup; */
NS_IMETHODIMP nsSchemaComplexType::GetModelGroup(nsISchemaModelGroup * *aModelGroup)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 attributeCount; */
NS_IMETHODIMP nsSchemaComplexType::GetAttributeCount(PRUint32 *aAttributeCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaAttributeComponent getAttributeByIndex (in PRUint32 index); */
NS_IMETHODIMP nsSchemaComplexType::GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaAttributeComponent getAttributeByName (in AString name); */
NS_IMETHODIMP nsSchemaComplexType::GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean abstract; */
NS_IMETHODIMP nsSchemaComplexType::GetAbstract(PRBool *aAbstract)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isArray; */
NS_IMETHODIMP nsSchemaComplexType::GetIsArray(PRBool *aIsArray)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISchemaType arrayType; */
NS_IMETHODIMP nsSchemaComplexType::GetArrayType(nsISchemaType * *aArrayType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 arrayDimension; */
NS_IMETHODIMP nsSchemaComplexType::GetArrayDimension(PRUint32 *aArrayDimension)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaParticle */
#define NS_ISCHEMAPARTICLE_IID_STR "3c14a029-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAPARTICLE_IID \
  {0x3c14a029, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaParticle : public nsISchemaComponent {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAPARTICLE_IID)

  enum { PARTICLE_TYPE_ELEMENT = 1U };

  enum { PARTICLE_TYPE_MODEL_GROUP = 2U };

  enum { PARTICLE_TYPE_ANY = 3U };

  enum { OCCURRENCE_UNBOUNDED = 4294967295U };

  /* readonly attribute AString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;

  /* readonly attribute unsigned short particleType; */
  NS_IMETHOD GetParticleType(PRUint16 *aParticleType) = 0;

  /* readonly attribute PRUint32 minOccurs; */
  NS_IMETHOD GetMinOccurs(PRUint32 *aMinOccurs) = 0;

  /* readonly attribute PRUint32 maxOccurs; */
  NS_IMETHOD GetMaxOccurs(PRUint32 *aMaxOccurs) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAPARTICLE \
  NS_IMETHOD GetName(nsAString & aName); \
  NS_IMETHOD GetParticleType(PRUint16 *aParticleType); \
  NS_IMETHOD GetMinOccurs(PRUint32 *aMinOccurs); \
  NS_IMETHOD GetMaxOccurs(PRUint32 *aMaxOccurs); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAPARTICLE(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD GetParticleType(PRUint16 *aParticleType) { return _to GetParticleType(aParticleType); } \
  NS_IMETHOD GetMinOccurs(PRUint32 *aMinOccurs) { return _to GetMinOccurs(aMinOccurs); } \
  NS_IMETHOD GetMaxOccurs(PRUint32 *aMaxOccurs) { return _to GetMaxOccurs(aMaxOccurs); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAPARTICLE(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetParticleType(PRUint16 *aParticleType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParticleType(aParticleType); } \
  NS_IMETHOD GetMinOccurs(PRUint32 *aMinOccurs) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMinOccurs(aMinOccurs); } \
  NS_IMETHOD GetMaxOccurs(PRUint32 *aMaxOccurs) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMaxOccurs(aMaxOccurs); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaParticle : public nsISchemaParticle
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAPARTICLE

  nsSchemaParticle();

private:
  ~nsSchemaParticle();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaParticle, nsISchemaParticle)

nsSchemaParticle::nsSchemaParticle()
{
  /* member initializers and constructor code */
}

nsSchemaParticle::~nsSchemaParticle()
{
  /* destructor code */
}

/* readonly attribute AString name; */
NS_IMETHODIMP nsSchemaParticle::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short particleType; */
NS_IMETHODIMP nsSchemaParticle::GetParticleType(PRUint16 *aParticleType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 minOccurs; */
NS_IMETHODIMP nsSchemaParticle::GetMinOccurs(PRUint32 *aMinOccurs)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 maxOccurs; */
NS_IMETHODIMP nsSchemaParticle::GetMaxOccurs(PRUint32 *aMaxOccurs)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaModelGroup */
#define NS_ISCHEMAMODELGROUP_IID_STR "3c14a02a-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAMODELGROUP_IID \
  {0x3c14a02a, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaModelGroup : public nsISchemaParticle {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAMODELGROUP_IID)

  enum { COMPOSITOR_ALL = 1U };

  enum { COMPOSITOR_SEQUENCE = 2U };

  enum { COMPOSITOR_CHOICE = 3U };

  /* readonly attribute unsigned short compositor; */
  NS_IMETHOD GetCompositor(PRUint16 *aCompositor) = 0;

  /* readonly attribute PRUint32 particleCount; */
  NS_IMETHOD GetParticleCount(PRUint32 *aParticleCount) = 0;

  /* nsISchemaParticle getParticle (in PRUint32 index); */
  NS_IMETHOD GetParticle(PRUint32 index, nsISchemaParticle **_retval) = 0;

  /* nsISchemaElement getElementByName (in AString name); */
  NS_IMETHOD GetElementByName(const nsAString & name, nsISchemaElement **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAMODELGROUP \
  NS_IMETHOD GetCompositor(PRUint16 *aCompositor); \
  NS_IMETHOD GetParticleCount(PRUint32 *aParticleCount); \
  NS_IMETHOD GetParticle(PRUint32 index, nsISchemaParticle **_retval); \
  NS_IMETHOD GetElementByName(const nsAString & name, nsISchemaElement **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAMODELGROUP(_to) \
  NS_IMETHOD GetCompositor(PRUint16 *aCompositor) { return _to GetCompositor(aCompositor); } \
  NS_IMETHOD GetParticleCount(PRUint32 *aParticleCount) { return _to GetParticleCount(aParticleCount); } \
  NS_IMETHOD GetParticle(PRUint32 index, nsISchemaParticle **_retval) { return _to GetParticle(index, _retval); } \
  NS_IMETHOD GetElementByName(const nsAString & name, nsISchemaElement **_retval) { return _to GetElementByName(name, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAMODELGROUP(_to) \
  NS_IMETHOD GetCompositor(PRUint16 *aCompositor) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCompositor(aCompositor); } \
  NS_IMETHOD GetParticleCount(PRUint32 *aParticleCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParticleCount(aParticleCount); } \
  NS_IMETHOD GetParticle(PRUint32 index, nsISchemaParticle **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParticle(index, _retval); } \
  NS_IMETHOD GetElementByName(const nsAString & name, nsISchemaElement **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetElementByName(name, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaModelGroup : public nsISchemaModelGroup
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAMODELGROUP

  nsSchemaModelGroup();

private:
  ~nsSchemaModelGroup();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaModelGroup, nsISchemaModelGroup)

nsSchemaModelGroup::nsSchemaModelGroup()
{
  /* member initializers and constructor code */
}

nsSchemaModelGroup::~nsSchemaModelGroup()
{
  /* destructor code */
}

/* readonly attribute unsigned short compositor; */
NS_IMETHODIMP nsSchemaModelGroup::GetCompositor(PRUint16 *aCompositor)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 particleCount; */
NS_IMETHODIMP nsSchemaModelGroup::GetParticleCount(PRUint32 *aParticleCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaParticle getParticle (in PRUint32 index); */
NS_IMETHODIMP nsSchemaModelGroup::GetParticle(PRUint32 index, nsISchemaParticle **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaElement getElementByName (in AString name); */
NS_IMETHODIMP nsSchemaModelGroup::GetElementByName(const nsAString & name, nsISchemaElement **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaAnyParticle */
#define NS_ISCHEMAANYPARTICLE_IID_STR "3c14a02b-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAANYPARTICLE_IID \
  {0x3c14a02b, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaAnyParticle : public nsISchemaParticle {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAANYPARTICLE_IID)

  enum { PROCESS_STRICT = 1U };

  enum { PROCESS_SKIP = 2U };

  enum { PROCESS_LAX = 3U };

  /* readonly attribute unsigned short process; */
  NS_IMETHOD GetProcess(PRUint16 *aProcess) = 0;

  /* readonly attribute AString namespace; */
  NS_IMETHOD GetNamespace(nsAString & aNamespace) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAANYPARTICLE \
  NS_IMETHOD GetProcess(PRUint16 *aProcess); \
  NS_IMETHOD GetNamespace(nsAString & aNamespace); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAANYPARTICLE(_to) \
  NS_IMETHOD GetProcess(PRUint16 *aProcess) { return _to GetProcess(aProcess); } \
  NS_IMETHOD GetNamespace(nsAString & aNamespace) { return _to GetNamespace(aNamespace); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAANYPARTICLE(_to) \
  NS_IMETHOD GetProcess(PRUint16 *aProcess) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetProcess(aProcess); } \
  NS_IMETHOD GetNamespace(nsAString & aNamespace) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNamespace(aNamespace); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaAnyParticle : public nsISchemaAnyParticle
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAANYPARTICLE

  nsSchemaAnyParticle();

private:
  ~nsSchemaAnyParticle();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaAnyParticle, nsISchemaAnyParticle)

nsSchemaAnyParticle::nsSchemaAnyParticle()
{
  /* member initializers and constructor code */
}

nsSchemaAnyParticle::~nsSchemaAnyParticle()
{
  /* destructor code */
}

/* readonly attribute unsigned short process; */
NS_IMETHODIMP nsSchemaAnyParticle::GetProcess(PRUint16 *aProcess)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString namespace; */
NS_IMETHODIMP nsSchemaAnyParticle::GetNamespace(nsAString & aNamespace)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaElement */
#define NS_ISCHEMAELEMENT_IID_STR "3c14a02c-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAELEMENT_IID \
  {0x3c14a02c, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaElement : public nsISchemaParticle {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAELEMENT_IID)

  /* readonly attribute nsISchemaType type; */
  NS_IMETHOD GetType(nsISchemaType * *aType) = 0;

  /* readonly attribute AString defaultValue; */
  NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue) = 0;

  /* readonly attribute AString fixedValue; */
  NS_IMETHOD GetFixedValue(nsAString & aFixedValue) = 0;

  /* readonly attribute boolean nillable; */
  NS_IMETHOD GetNillable(PRBool *aNillable) = 0;

  /* readonly attribute boolean abstract; */
  NS_IMETHOD GetAbstract(PRBool *aAbstract) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAELEMENT \
  NS_IMETHOD GetType(nsISchemaType * *aType); \
  NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue); \
  NS_IMETHOD GetFixedValue(nsAString & aFixedValue); \
  NS_IMETHOD GetNillable(PRBool *aNillable); \
  NS_IMETHOD GetAbstract(PRBool *aAbstract); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAELEMENT(_to) \
  NS_IMETHOD GetType(nsISchemaType * *aType) { return _to GetType(aType); } \
  NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue) { return _to GetDefaultValue(aDefaultValue); } \
  NS_IMETHOD GetFixedValue(nsAString & aFixedValue) { return _to GetFixedValue(aFixedValue); } \
  NS_IMETHOD GetNillable(PRBool *aNillable) { return _to GetNillable(aNillable); } \
  NS_IMETHOD GetAbstract(PRBool *aAbstract) { return _to GetAbstract(aAbstract); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAELEMENT(_to) \
  NS_IMETHOD GetType(nsISchemaType * *aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultValue(aDefaultValue); } \
  NS_IMETHOD GetFixedValue(nsAString & aFixedValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFixedValue(aFixedValue); } \
  NS_IMETHOD GetNillable(PRBool *aNillable) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNillable(aNillable); } \
  NS_IMETHOD GetAbstract(PRBool *aAbstract) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAbstract(aAbstract); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaElement : public nsISchemaElement
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAELEMENT

  nsSchemaElement();

private:
  ~nsSchemaElement();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaElement, nsISchemaElement)

nsSchemaElement::nsSchemaElement()
{
  /* member initializers and constructor code */
}

nsSchemaElement::~nsSchemaElement()
{
  /* destructor code */
}

/* readonly attribute nsISchemaType type; */
NS_IMETHODIMP nsSchemaElement::GetType(nsISchemaType * *aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString defaultValue; */
NS_IMETHODIMP nsSchemaElement::GetDefaultValue(nsAString & aDefaultValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString fixedValue; */
NS_IMETHODIMP nsSchemaElement::GetFixedValue(nsAString & aFixedValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean nillable; */
NS_IMETHODIMP nsSchemaElement::GetNillable(PRBool *aNillable)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean abstract; */
NS_IMETHODIMP nsSchemaElement::GetAbstract(PRBool *aAbstract)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaAttributeComponent */
#define NS_ISCHEMAATTRIBUTECOMPONENT_IID_STR "3c14a02d-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAATTRIBUTECOMPONENT_IID \
  {0x3c14a02d, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaAttributeComponent : public nsISchemaComponent {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAATTRIBUTECOMPONENT_IID)

  enum { COMPONENT_TYPE_ATTRIBUTE = 1U };

  enum { COMPONENT_TYPE_GROUP = 2U };

  enum { COMPONENT_TYPE_ANY = 3U };

  /* readonly attribute AString name; */
  NS_IMETHOD GetName(nsAString & aName) = 0;

  /* readonly attribute unsigned short componentType; */
  NS_IMETHOD GetComponentType(PRUint16 *aComponentType) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAATTRIBUTECOMPONENT \
  NS_IMETHOD GetName(nsAString & aName); \
  NS_IMETHOD GetComponentType(PRUint16 *aComponentType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAATTRIBUTECOMPONENT(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD GetComponentType(PRUint16 *aComponentType) { return _to GetComponentType(aComponentType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAATTRIBUTECOMPONENT(_to) \
  NS_IMETHOD GetName(nsAString & aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetComponentType(PRUint16 *aComponentType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetComponentType(aComponentType); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaAttributeComponent : public nsISchemaAttributeComponent
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAATTRIBUTECOMPONENT

  nsSchemaAttributeComponent();

private:
  ~nsSchemaAttributeComponent();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaAttributeComponent, nsISchemaAttributeComponent)

nsSchemaAttributeComponent::nsSchemaAttributeComponent()
{
  /* member initializers and constructor code */
}

nsSchemaAttributeComponent::~nsSchemaAttributeComponent()
{
  /* destructor code */
}

/* readonly attribute AString name; */
NS_IMETHODIMP nsSchemaAttributeComponent::GetName(nsAString & aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short componentType; */
NS_IMETHODIMP nsSchemaAttributeComponent::GetComponentType(PRUint16 *aComponentType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaAttribute */
#define NS_ISCHEMAATTRIBUTE_IID_STR "3c14a02e-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAATTRIBUTE_IID \
  {0x3c14a02e, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaAttribute : public nsISchemaAttributeComponent {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAATTRIBUTE_IID)

  enum { USE_OPTIONAL = 1U };

  enum { USE_PROHIBITED = 2U };

  enum { USE_REQUIRED = 3U };

  /* readonly attribute nsISchemaSimpleType type; */
  NS_IMETHOD GetType(nsISchemaSimpleType * *aType) = 0;

  /* readonly attribute AString defaultValue; */
  NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue) = 0;

  /* readonly attribute AString fixedValue; */
  NS_IMETHOD GetFixedValue(nsAString & aFixedValue) = 0;

  /* readonly attribute unsigned short use; */
  NS_IMETHOD GetUse(PRUint16 *aUse) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAATTRIBUTE \
  NS_IMETHOD GetType(nsISchemaSimpleType * *aType); \
  NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue); \
  NS_IMETHOD GetFixedValue(nsAString & aFixedValue); \
  NS_IMETHOD GetUse(PRUint16 *aUse); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAATTRIBUTE(_to) \
  NS_IMETHOD GetType(nsISchemaSimpleType * *aType) { return _to GetType(aType); } \
  NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue) { return _to GetDefaultValue(aDefaultValue); } \
  NS_IMETHOD GetFixedValue(nsAString & aFixedValue) { return _to GetFixedValue(aFixedValue); } \
  NS_IMETHOD GetUse(PRUint16 *aUse) { return _to GetUse(aUse); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAATTRIBUTE(_to) \
  NS_IMETHOD GetType(nsISchemaSimpleType * *aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_IMETHOD GetDefaultValue(nsAString & aDefaultValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultValue(aDefaultValue); } \
  NS_IMETHOD GetFixedValue(nsAString & aFixedValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFixedValue(aFixedValue); } \
  NS_IMETHOD GetUse(PRUint16 *aUse) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetUse(aUse); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaAttribute : public nsISchemaAttribute
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAATTRIBUTE

  nsSchemaAttribute();

private:
  ~nsSchemaAttribute();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaAttribute, nsISchemaAttribute)

nsSchemaAttribute::nsSchemaAttribute()
{
  /* member initializers and constructor code */
}

nsSchemaAttribute::~nsSchemaAttribute()
{
  /* destructor code */
}

/* readonly attribute nsISchemaSimpleType type; */
NS_IMETHODIMP nsSchemaAttribute::GetType(nsISchemaSimpleType * *aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString defaultValue; */
NS_IMETHODIMP nsSchemaAttribute::GetDefaultValue(nsAString & aDefaultValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString fixedValue; */
NS_IMETHODIMP nsSchemaAttribute::GetFixedValue(nsAString & aFixedValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short use; */
NS_IMETHODIMP nsSchemaAttribute::GetUse(PRUint16 *aUse)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaAttributeGroup */
#define NS_ISCHEMAATTRIBUTEGROUP_IID_STR "3c14a02f-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAATTRIBUTEGROUP_IID \
  {0x3c14a02f, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaAttributeGroup : public nsISchemaAttributeComponent {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAATTRIBUTEGROUP_IID)

  /* readonly attribute PRUint32 attributeCount; */
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount) = 0;

  /* nsISchemaAttributeComponent getAttributeByIndex (in PRUint32 index); */
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval) = 0;

  /* nsISchemaAttributeComponent getAttributeByName (in AString name); */
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAATTRIBUTEGROUP \
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount); \
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval); \
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAATTRIBUTEGROUP(_to) \
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount) { return _to GetAttributeCount(aAttributeCount); } \
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval) { return _to GetAttributeByIndex(index, _retval); } \
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval) { return _to GetAttributeByName(name, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAATTRIBUTEGROUP(_to) \
  NS_IMETHOD GetAttributeCount(PRUint32 *aAttributeCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeCount(aAttributeCount); } \
  NS_IMETHOD GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeByIndex(index, _retval); } \
  NS_IMETHOD GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttributeByName(name, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaAttributeGroup : public nsISchemaAttributeGroup
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAATTRIBUTEGROUP

  nsSchemaAttributeGroup();

private:
  ~nsSchemaAttributeGroup();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaAttributeGroup, nsISchemaAttributeGroup)

nsSchemaAttributeGroup::nsSchemaAttributeGroup()
{
  /* member initializers and constructor code */
}

nsSchemaAttributeGroup::~nsSchemaAttributeGroup()
{
  /* destructor code */
}

/* readonly attribute PRUint32 attributeCount; */
NS_IMETHODIMP nsSchemaAttributeGroup::GetAttributeCount(PRUint32 *aAttributeCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaAttributeComponent getAttributeByIndex (in PRUint32 index); */
NS_IMETHODIMP nsSchemaAttributeGroup::GetAttributeByIndex(PRUint32 index, nsISchemaAttributeComponent **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISchemaAttributeComponent getAttributeByName (in AString name); */
NS_IMETHODIMP nsSchemaAttributeGroup::GetAttributeByName(const nsAString & name, nsISchemaAttributeComponent **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaAnyAttribute */
#define NS_ISCHEMAANYATTRIBUTE_IID_STR "3c14a030-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAANYATTRIBUTE_IID \
  {0x3c14a030, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaAnyAttribute : public nsISchemaAttributeComponent {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAANYATTRIBUTE_IID)

  enum { PROCESS_STRICT = 1U };

  enum { PROCESS_SKIP = 2U };

  enum { PROCESS_LAX = 3U };

  /* readonly attribute unsigned short process; */
  NS_IMETHOD GetProcess(PRUint16 *aProcess) = 0;

  /* readonly attribute AString namespace; */
  NS_IMETHOD GetNamespace(nsAString & aNamespace) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAANYATTRIBUTE \
  NS_IMETHOD GetProcess(PRUint16 *aProcess); \
  NS_IMETHOD GetNamespace(nsAString & aNamespace); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAANYATTRIBUTE(_to) \
  NS_IMETHOD GetProcess(PRUint16 *aProcess) { return _to GetProcess(aProcess); } \
  NS_IMETHOD GetNamespace(nsAString & aNamespace) { return _to GetNamespace(aNamespace); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAANYATTRIBUTE(_to) \
  NS_IMETHOD GetProcess(PRUint16 *aProcess) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetProcess(aProcess); } \
  NS_IMETHOD GetNamespace(nsAString & aNamespace) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNamespace(aNamespace); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaAnyAttribute : public nsISchemaAnyAttribute
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAANYATTRIBUTE

  nsSchemaAnyAttribute();

private:
  ~nsSchemaAnyAttribute();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaAnyAttribute, nsISchemaAnyAttribute)

nsSchemaAnyAttribute::nsSchemaAnyAttribute()
{
  /* member initializers and constructor code */
}

nsSchemaAnyAttribute::~nsSchemaAnyAttribute()
{
  /* destructor code */
}

/* readonly attribute unsigned short process; */
NS_IMETHODIMP nsSchemaAnyAttribute::GetProcess(PRUint16 *aProcess)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString namespace; */
NS_IMETHODIMP nsSchemaAnyAttribute::GetNamespace(nsAString & aNamespace)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsISchemaFacet */
#define NS_ISCHEMAFACET_IID_STR "3c14a031-6f4e-11d5-9b46-000064657374"

#define NS_ISCHEMAFACET_IID \
  {0x3c14a031, 0x6f4e, 0x11d5, \
    { 0x9b, 0x46, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsISchemaFacet : public nsISchemaComponent {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCHEMAFACET_IID)

  enum { FACET_TYPE_LENGTH = 1U };

  enum { FACET_TYPE_MINLENGTH = 2U };

  enum { FACET_TYPE_MAXLENGTH = 3U };

  enum { FACET_TYPE_PATTERN = 4U };

  enum { FACET_TYPE_ENUMERATION = 5U };

  enum { FACET_TYPE_WHITESPACE = 6U };

  enum { FACET_TYPE_MAXINCLUSIVE = 7U };

  enum { FACET_TYPE_MININCLUSIVE = 8U };

  enum { FACET_TYPE_MAXEXCLUSIVE = 9U };

  enum { FACET_TYPE_MINEXCLUSIVE = 10U };

  enum { FACET_TYPE_TOTALDIGITS = 11U };

  enum { FACET_TYPE_FRACTIONDIGITS = 12U };

  enum { WHITESPACE_PRESERVE = 1U };

  enum { WHITESPACE_REPLACE = 1U };

  enum { WHITESPACE_COLLAPSE = 1U };

  /* readonly attribute unsigned short facetType; */
  NS_IMETHOD GetFacetType(PRUint16 *aFacetType) = 0;

  /* readonly attribute AString value; */
  NS_IMETHOD GetValue(nsAString & aValue) = 0;

  /* readonly attribute PRUint32 lengthValue; */
  NS_IMETHOD GetLengthValue(PRUint32 *aLengthValue) = 0;

  /* readonly attribute PRUint32 digitsValue; */
  NS_IMETHOD GetDigitsValue(PRUint32 *aDigitsValue) = 0;

  /* readonly attribute unsigned short whitespaceValue; */
  NS_IMETHOD GetWhitespaceValue(PRUint16 *aWhitespaceValue) = 0;

  /* readonly attribute boolean isfixed; */
  NS_IMETHOD GetIsfixed(PRBool *aIsfixed) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCHEMAFACET \
  NS_IMETHOD GetFacetType(PRUint16 *aFacetType); \
  NS_IMETHOD GetValue(nsAString & aValue); \
  NS_IMETHOD GetLengthValue(PRUint32 *aLengthValue); \
  NS_IMETHOD GetDigitsValue(PRUint32 *aDigitsValue); \
  NS_IMETHOD GetWhitespaceValue(PRUint16 *aWhitespaceValue); \
  NS_IMETHOD GetIsfixed(PRBool *aIsfixed); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCHEMAFACET(_to) \
  NS_IMETHOD GetFacetType(PRUint16 *aFacetType) { return _to GetFacetType(aFacetType); } \
  NS_IMETHOD GetValue(nsAString & aValue) { return _to GetValue(aValue); } \
  NS_IMETHOD GetLengthValue(PRUint32 *aLengthValue) { return _to GetLengthValue(aLengthValue); } \
  NS_IMETHOD GetDigitsValue(PRUint32 *aDigitsValue) { return _to GetDigitsValue(aDigitsValue); } \
  NS_IMETHOD GetWhitespaceValue(PRUint16 *aWhitespaceValue) { return _to GetWhitespaceValue(aWhitespaceValue); } \
  NS_IMETHOD GetIsfixed(PRBool *aIsfixed) { return _to GetIsfixed(aIsfixed); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCHEMAFACET(_to) \
  NS_IMETHOD GetFacetType(PRUint16 *aFacetType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFacetType(aFacetType); } \
  NS_IMETHOD GetValue(nsAString & aValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetValue(aValue); } \
  NS_IMETHOD GetLengthValue(PRUint32 *aLengthValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLengthValue(aLengthValue); } \
  NS_IMETHOD GetDigitsValue(PRUint32 *aDigitsValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDigitsValue(aDigitsValue); } \
  NS_IMETHOD GetWhitespaceValue(PRUint16 *aWhitespaceValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetWhitespaceValue(aWhitespaceValue); } \
  NS_IMETHOD GetIsfixed(PRBool *aIsfixed) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsfixed(aIsfixed); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSchemaFacet : public nsISchemaFacet
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCHEMAFACET

  nsSchemaFacet();

private:
  ~nsSchemaFacet();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSchemaFacet, nsISchemaFacet)

nsSchemaFacet::nsSchemaFacet()
{
  /* member initializers and constructor code */
}

nsSchemaFacet::~nsSchemaFacet()
{
  /* destructor code */
}

/* readonly attribute unsigned short facetType; */
NS_IMETHODIMP nsSchemaFacet::GetFacetType(PRUint16 *aFacetType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString value; */
NS_IMETHODIMP nsSchemaFacet::GetValue(nsAString & aValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 lengthValue; */
NS_IMETHODIMP nsSchemaFacet::GetLengthValue(PRUint32 *aLengthValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 digitsValue; */
NS_IMETHODIMP nsSchemaFacet::GetDigitsValue(PRUint32 *aDigitsValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short whitespaceValue; */
NS_IMETHODIMP nsSchemaFacet::GetWhitespaceValue(PRUint16 *aWhitespaceValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isfixed; */
NS_IMETHODIMP nsSchemaFacet::GetIsfixed(PRBool *aIsfixed)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISchema_h__ */
