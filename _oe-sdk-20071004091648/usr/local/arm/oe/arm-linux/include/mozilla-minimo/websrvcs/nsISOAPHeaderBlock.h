/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPHeaderBlock.idl
 */

#ifndef __gen_nsISOAPHeaderBlock_h__
#define __gen_nsISOAPHeaderBlock_h__


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


/* starting interface:    nsISOAPHeaderBlock */
#define NS_ISOAPHEADERBLOCK_IID_STR "063d4a4e-1dd2-11b2-a365-cbaf1651f140"

#define NS_ISOAPHEADERBLOCK_IID \
  {0x063d4a4e, 0x1dd2, 0x11b2, \
    { 0xa3, 0x65, 0xcb, 0xaf, 0x16, 0x51, 0xf1, 0x40 }}

/**
 * This interface encapsulates an arbitrary header block to be used
 * by the soap serialization or protocol.  See the description of the
 * nsISOAPBlock interface for information on how the basics of this
 * interface works.
 */
class NS_NO_VTABLE nsISOAPHeaderBlock : public nsISOAPBlock {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPHEADERBLOCK_IID)

  /**
   * The actor URI of the header block.  If element is set,
   *   then this is a computed value.  If this is modified,
   *   then element  is set to null and all attributes computed 
   *   from element revert to previous uncomputed values.
   */
  /* attribute AString actorURI; */
  NS_IMETHOD GetActorURI(nsAString & aActorURI) = 0;
  NS_IMETHOD SetActorURI(const nsAString & aActorURI) = 0;

  /**
   * Flags that the processor must understand this header.
   *   If element is set, then this is a computed value.
   *   If this is modified, then element is set to null and 
   *   all attributes computed from element revert to
   *   previous uncomputed values.
   */
  /* attribute boolean mustUnderstand; */
  NS_IMETHOD GetMustUnderstand(PRBool *aMustUnderstand) = 0;
  NS_IMETHOD SetMustUnderstand(PRBool aMustUnderstand) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPHEADERBLOCK \
  NS_IMETHOD GetActorURI(nsAString & aActorURI); \
  NS_IMETHOD SetActorURI(const nsAString & aActorURI); \
  NS_IMETHOD GetMustUnderstand(PRBool *aMustUnderstand); \
  NS_IMETHOD SetMustUnderstand(PRBool aMustUnderstand); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPHEADERBLOCK(_to) \
  NS_IMETHOD GetActorURI(nsAString & aActorURI) { return _to GetActorURI(aActorURI); } \
  NS_IMETHOD SetActorURI(const nsAString & aActorURI) { return _to SetActorURI(aActorURI); } \
  NS_IMETHOD GetMustUnderstand(PRBool *aMustUnderstand) { return _to GetMustUnderstand(aMustUnderstand); } \
  NS_IMETHOD SetMustUnderstand(PRBool aMustUnderstand) { return _to SetMustUnderstand(aMustUnderstand); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPHEADERBLOCK(_to) \
  NS_IMETHOD GetActorURI(nsAString & aActorURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetActorURI(aActorURI); } \
  NS_IMETHOD SetActorURI(const nsAString & aActorURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetActorURI(aActorURI); } \
  NS_IMETHOD GetMustUnderstand(PRBool *aMustUnderstand) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMustUnderstand(aMustUnderstand); } \
  NS_IMETHOD SetMustUnderstand(PRBool aMustUnderstand) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetMustUnderstand(aMustUnderstand); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPHeaderBlock : public nsISOAPHeaderBlock
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPHEADERBLOCK

  nsSOAPHeaderBlock();

private:
  ~nsSOAPHeaderBlock();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPHeaderBlock, nsISOAPHeaderBlock)

nsSOAPHeaderBlock::nsSOAPHeaderBlock()
{
  /* member initializers and constructor code */
}

nsSOAPHeaderBlock::~nsSOAPHeaderBlock()
{
  /* destructor code */
}

/* attribute AString actorURI; */
NS_IMETHODIMP nsSOAPHeaderBlock::GetActorURI(nsAString & aActorURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPHeaderBlock::SetActorURI(const nsAString & aActorURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean mustUnderstand; */
NS_IMETHODIMP nsSOAPHeaderBlock::GetMustUnderstand(PRBool *aMustUnderstand)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPHeaderBlock::SetMustUnderstand(PRBool aMustUnderstand)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SOAPHEADERBLOCK_CID                        \
{ /* 5ad0eace-1dd2-11b2-a260-ff42edcaedb3 */        \
  0x5ad0eace, 0x1dd2, 0x11b2,                       \
 {0xa2, 0x60, 0xff, 0x42, 0xed, 0xca, 0xed, 0xb3} }
#define NS_SOAPHEADERBLOCK_CONTRACTID \
"@mozilla.org/xmlextras/soap/headerblock;1"

#endif /* __gen_nsISOAPHeaderBlock_h__ */
