/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/content/base/public/nsIObjectLoadingContent.idl
 */

#ifndef __gen_nsIObjectLoadingContent_h__
#define __gen_nsIObjectLoadingContent_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIObjectFrame; /* forward declaration */


/* starting interface:    nsIObjectLoadingContent */
#define NS_IOBJECTLOADINGCONTENT_IID_STR "3e78c950-24d2-4fcc-b0b9-751686e89b8f"

#define NS_IOBJECTLOADINGCONTENT_IID \
  {0x3e78c950, 0x24d2, 0x4fcc, \
    { 0xb0, 0xb9, 0x75, 0x16, 0x86, 0xe8, 0x9b, 0x8f }}

/**
 * This interface represents a content node that loads objects.
 */
class NS_NO_VTABLE nsIObjectLoadingContent : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IOBJECTLOADINGCONTENT_IID)

  enum { TYPE_LOADING = 0U };

  enum { TYPE_IMAGE = 1U };

  enum { TYPE_PLUGIN = 2U };

  enum { TYPE_DOCUMENT = 3U };

  enum { TYPE_NULL = 4U };

  /**
   * The actual mime type (the one we got back from the network
   * request) for the element.
   */
  /* readonly attribute ACString actualType; */
  NS_IMETHOD GetActualType(nsACString & aActualType) = 0;

  /**
   * Gets the type of the content that's currently loaded. See
   * the constants above for the list of possible values.
   */
  /* readonly attribute unsigned long displayedType; */
  NS_IMETHOD GetDisplayedType(PRUint32 *aDisplayedType) = 0;

  /**
   * For a classid, returns the MIME type that can be used to instantiate
   * a plugin for this ID.
   *
   * @throw NS_ERROR_NOT_AVAILABLE Unsupported class ID.
   */
  /* ACString typeForClassID (in AString aClassID); */
  NS_IMETHOD TypeForClassID(const nsAString & aClassID, nsACString & _retval) = 0;

  /**
   * Tells the content about an associated object frame.
   * This can be called multiple times for different frames.
   *
   * This is noscript because this is an internal method that will go away, and
   * because nsIObjectFrame is unscriptable.
   */
  /* [noscript] void hasNewFrame (in nsIObjectFrame aFrame); */
  NS_IMETHOD HasNewFrame(nsIObjectFrame *aFrame) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIOBJECTLOADINGCONTENT \
  NS_IMETHOD GetActualType(nsACString & aActualType); \
  NS_IMETHOD GetDisplayedType(PRUint32 *aDisplayedType); \
  NS_IMETHOD TypeForClassID(const nsAString & aClassID, nsACString & _retval); \
  NS_IMETHOD HasNewFrame(nsIObjectFrame *aFrame); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIOBJECTLOADINGCONTENT(_to) \
  NS_IMETHOD GetActualType(nsACString & aActualType) { return _to GetActualType(aActualType); } \
  NS_IMETHOD GetDisplayedType(PRUint32 *aDisplayedType) { return _to GetDisplayedType(aDisplayedType); } \
  NS_IMETHOD TypeForClassID(const nsAString & aClassID, nsACString & _retval) { return _to TypeForClassID(aClassID, _retval); } \
  NS_IMETHOD HasNewFrame(nsIObjectFrame *aFrame) { return _to HasNewFrame(aFrame); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIOBJECTLOADINGCONTENT(_to) \
  NS_IMETHOD GetActualType(nsACString & aActualType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetActualType(aActualType); } \
  NS_IMETHOD GetDisplayedType(PRUint32 *aDisplayedType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDisplayedType(aDisplayedType); } \
  NS_IMETHOD TypeForClassID(const nsAString & aClassID, nsACString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->TypeForClassID(aClassID, _retval); } \
  NS_IMETHOD HasNewFrame(nsIObjectFrame *aFrame) { return !_to ? NS_ERROR_NULL_POINTER : _to->HasNewFrame(aFrame); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsObjectLoadingContent : public nsIObjectLoadingContent
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIOBJECTLOADINGCONTENT

  nsObjectLoadingContent();

private:
  ~nsObjectLoadingContent();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsObjectLoadingContent, nsIObjectLoadingContent)

nsObjectLoadingContent::nsObjectLoadingContent()
{
  /* member initializers and constructor code */
}

nsObjectLoadingContent::~nsObjectLoadingContent()
{
  /* destructor code */
}

/* readonly attribute ACString actualType; */
NS_IMETHODIMP nsObjectLoadingContent::GetActualType(nsACString & aActualType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long displayedType; */
NS_IMETHODIMP nsObjectLoadingContent::GetDisplayedType(PRUint32 *aDisplayedType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* ACString typeForClassID (in AString aClassID); */
NS_IMETHODIMP nsObjectLoadingContent::TypeForClassID(const nsAString & aClassID, nsACString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void hasNewFrame (in nsIObjectFrame aFrame); */
NS_IMETHODIMP nsObjectLoadingContent::HasNewFrame(nsIObjectFrame *aFrame)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIObjectLoadingContent_h__ */
