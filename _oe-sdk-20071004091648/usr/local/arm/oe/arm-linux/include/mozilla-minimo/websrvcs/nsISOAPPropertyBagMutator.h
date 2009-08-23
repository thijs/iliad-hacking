/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPPropertyBagMutator.idl
 */

#ifndef __gen_nsISOAPPropertyBagMutator_h__
#define __gen_nsISOAPPropertyBagMutator_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIPropertyBag; /* forward declaration */

class nsIVariant; /* forward declaration */


/* starting interface:    nsISOAPPropertyBagMutator */
#define NS_ISOAPPROPERTYBAGMUTATOR_IID_STR "f34cb3c8-1dd1-11b2-8a18-a93a99d92c08"

#define NS_ISOAPPROPERTYBAGMUTATOR_IID \
  {0xf34cb3c8, 0x1dd1, 0x11b2, \
    { 0x8a, 0x18, 0xa9, 0x3a, 0x99, 0xd9, 0x2c, 0x08 }}

/**
 * This permits construction by native or
 * script code a property bag that is also
 * usable by other xpconnect systems.
 */
class NS_NO_VTABLE nsISOAPPropertyBagMutator : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPPROPERTYBAGMUTATOR_IID)

  /**
   * The property bag that is being constructed.
   * After the mutator is destroyed, this is
   * read-only.  This will never be null.
   */
  /* readonly attribute nsIPropertyBag propertyBag; */
  NS_IMETHOD GetPropertyBag(nsIPropertyBag * *aPropertyBag) = 0;

  /**
   * Add or replace a property to the 
   * newly-constructed property bag.
   *
   * @param aName The name of the property.
   *
   * @param aValue The value of the property.
   */
  /* void addProperty (in AString aName, in nsIVariant aValue); */
  NS_IMETHOD AddProperty(const nsAString & aName, nsIVariant *aValue) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPPROPERTYBAGMUTATOR \
  NS_IMETHOD GetPropertyBag(nsIPropertyBag * *aPropertyBag); \
  NS_IMETHOD AddProperty(const nsAString & aName, nsIVariant *aValue); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPPROPERTYBAGMUTATOR(_to) \
  NS_IMETHOD GetPropertyBag(nsIPropertyBag * *aPropertyBag) { return _to GetPropertyBag(aPropertyBag); } \
  NS_IMETHOD AddProperty(const nsAString & aName, nsIVariant *aValue) { return _to AddProperty(aName, aValue); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPPROPERTYBAGMUTATOR(_to) \
  NS_IMETHOD GetPropertyBag(nsIPropertyBag * *aPropertyBag) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPropertyBag(aPropertyBag); } \
  NS_IMETHOD AddProperty(const nsAString & aName, nsIVariant *aValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddProperty(aName, aValue); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPPropertyBagMutator : public nsISOAPPropertyBagMutator
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPPROPERTYBAGMUTATOR

  nsSOAPPropertyBagMutator();

private:
  ~nsSOAPPropertyBagMutator();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPPropertyBagMutator, nsISOAPPropertyBagMutator)

nsSOAPPropertyBagMutator::nsSOAPPropertyBagMutator()
{
  /* member initializers and constructor code */
}

nsSOAPPropertyBagMutator::~nsSOAPPropertyBagMutator()
{
  /* destructor code */
}

/* readonly attribute nsIPropertyBag propertyBag; */
NS_IMETHODIMP nsSOAPPropertyBagMutator::GetPropertyBag(nsIPropertyBag * *aPropertyBag)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addProperty (in AString aName, in nsIVariant aValue); */
NS_IMETHODIMP nsSOAPPropertyBagMutator::AddProperty(const nsAString & aName, nsIVariant *aValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SOAPPROPERTYBAGMUTATOR_CID               \
{ /* c63d2762-1dd1-11b2-b028-8f244edaa8a1 */        \
  0xc63d2762, 0x1dd1, 0x11b2,                       \
 {0xb0, 0x28, 0x8f, 0x24, 0x4e, 0xda, 0xa8, 0xa1} }
#define NS_SOAPPROPERTYBAGMUTATOR_CONTRACTID \
"@mozilla.org/xmlextras/soap/propertybagmutator;1"

#endif /* __gen_nsISOAPPropertyBagMutator_h__ */
