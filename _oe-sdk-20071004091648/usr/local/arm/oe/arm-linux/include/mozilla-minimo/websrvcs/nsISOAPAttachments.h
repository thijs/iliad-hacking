/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPAttachments.idl
 */

#ifndef __gen_nsISOAPAttachments_h__
#define __gen_nsISOAPAttachments_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsISOAPAttachments */
#define NS_ISOAPATTACHMENTS_IID_STR "6192dcbe-1dd2-11b2-81ad-a4597614c4ae"

#define NS_ISOAPATTACHMENTS_IID \
  {0x6192dcbe, 0x1dd2, 0x11b2, \
    { 0x81, 0xad, 0xa4, 0x59, 0x76, 0x14, 0xc4, 0xae }}

/**
 * This interface permits attachment of SOAP attachments.
 */
class NS_NO_VTABLE nsISOAPAttachments : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPATTACHMENTS_IID)

  /**
   * Get the attachment associated with a particular identifier.
   *
   * @param aIdentifier The identifier of the attachment to be accessed.
   *
   * Appropriate return(s) must be identified.
   */
  /* void getAttachment (in AString aIdentifier); */
  NS_IMETHOD GetAttachment(const nsAString & aIdentifier) = 0;

  /**
   * Attach an attachment to the message.
   *
   * Appropriate argument(s) must be identified.
   *
   * @return The identifier of the attachment, to be referenced in SOAP encoding
   */
  /* AString attach (); */
  NS_IMETHOD Attach(nsAString & _retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPATTACHMENTS \
  NS_IMETHOD GetAttachment(const nsAString & aIdentifier); \
  NS_IMETHOD Attach(nsAString & _retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPATTACHMENTS(_to) \
  NS_IMETHOD GetAttachment(const nsAString & aIdentifier) { return _to GetAttachment(aIdentifier); } \
  NS_IMETHOD Attach(nsAString & _retval) { return _to Attach(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPATTACHMENTS(_to) \
  NS_IMETHOD GetAttachment(const nsAString & aIdentifier) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttachment(aIdentifier); } \
  NS_IMETHOD Attach(nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Attach(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPAttachments : public nsISOAPAttachments
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPATTACHMENTS

  nsSOAPAttachments();

private:
  ~nsSOAPAttachments();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPAttachments, nsISOAPAttachments)

nsSOAPAttachments::nsSOAPAttachments()
{
  /* member initializers and constructor code */
}

nsSOAPAttachments::~nsSOAPAttachments()
{
  /* destructor code */
}

/* void getAttachment (in AString aIdentifier); */
NS_IMETHODIMP nsSOAPAttachments::GetAttachment(const nsAString & aIdentifier)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString attach (); */
NS_IMETHODIMP nsSOAPAttachments::Attach(nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISOAPAttachments_h__ */
