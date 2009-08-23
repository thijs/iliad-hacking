/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/docshell/base/nsIGlobalHistory2.idl
 */

#ifndef __gen_nsIGlobalHistory2_h__
#define __gen_nsIGlobalHistory2_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIURI; /* forward declaration */

// nsIObserver topic to fire when you add new visited URIs to the history;
// the nsIURI is the subject
#define NS_LINK_VISITED_EVENT_TOPIC "link-visited"
// This is NOT part of the interface! It could change.
#define NS_GECKO_FLAG_NEEDS_VERTICAL_SCROLLBAR (1 << 0)

/* starting interface:    nsIGlobalHistory2 */
#define NS_IGLOBALHISTORY2_IID_STR "fbcfa5ca-5d05-4726-b325-d0e6563a1715"

#define NS_IGLOBALHISTORY2_IID \
  {0xfbcfa5ca, 0x5d05, 0x4726, \
    { 0xb3, 0x25, 0xd0, 0xe6, 0x56, 0x3a, 0x17, 0x15 }}

class NS_NO_VTABLE nsIGlobalHistory2 : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IGLOBALHISTORY2_IID)

  /**
     * Add a URI to global history
     *
     * @param aURI      the URI of the page
     * @param aRedirect whether the URI was redirected to another location;
     *                  this is 'true' for the original URI which is
     *                  redirected.
     * @param aToplevel whether the URI is loaded in a top-level window
     * @param aReferrer the URI of the referring page
     *
     * @note  Docshell will not filter out URI schemes like chrome: data:
     *        about: and view-source:.  Embedders should consider filtering out
     *        these schemes and others, e.g. mailbox: for the main URI and the
     *        referrer.
     */
  /* void addURI (in nsIURI aURI, in boolean aRedirect, in boolean aToplevel, in nsIURI aReferrer); */
  NS_IMETHOD AddURI(nsIURI *aURI, PRBool aRedirect, PRBool aToplevel, nsIURI *aReferrer) = 0;

  /**
     * Checks to see whether the given URI is in history.
     *
     * @param aURI the uri to the page
     * @return true if a URI has been visited
     */
  /* boolean isVisited (in nsIURI aURI); */
  NS_IMETHOD IsVisited(nsIURI *aURI, PRBool *_retval) = 0;

  /**
     * Set the page title for the given uri. URIs that are not already in
     * global history will not be added.
     *
     * @param aURI    the URI for which to set to the title
     * @param aTitle  the page title
     */
  /* void setPageTitle (in nsIURI aURI, in AString aTitle); */
  NS_IMETHOD SetPageTitle(nsIURI *aURI, const nsAString & aTitle) = 0;

  /**
     * Get the Gecko flags for this URI. These flags are used by Gecko as hints
     * to optimize page loading. Not all histories have them; this need not be
     * supported (just return NS_ERROR_NOT_IMPLEMENTED. These flags are opaque
     * and should not be interpreted by the history engine.
     */
  /* unsigned long getURIGeckoFlags (in nsIURI aURI); */
  NS_IMETHOD GetURIGeckoFlags(nsIURI *aURI, PRUint32 *_retval) = 0;

  /**
     * Set the Gecko flags for this URI. May fail if the history entry
     * doesn't have any flags or if there is no entry for the URI.
     */
  /* void setURIGeckoFlags (in nsIURI aURI, in unsigned long aFlags); */
  NS_IMETHOD SetURIGeckoFlags(nsIURI *aURI, PRUint32 aFlags) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIGLOBALHISTORY2 \
  NS_IMETHOD AddURI(nsIURI *aURI, PRBool aRedirect, PRBool aToplevel, nsIURI *aReferrer); \
  NS_IMETHOD IsVisited(nsIURI *aURI, PRBool *_retval); \
  NS_IMETHOD SetPageTitle(nsIURI *aURI, const nsAString & aTitle); \
  NS_IMETHOD GetURIGeckoFlags(nsIURI *aURI, PRUint32 *_retval); \
  NS_IMETHOD SetURIGeckoFlags(nsIURI *aURI, PRUint32 aFlags); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIGLOBALHISTORY2(_to) \
  NS_IMETHOD AddURI(nsIURI *aURI, PRBool aRedirect, PRBool aToplevel, nsIURI *aReferrer) { return _to AddURI(aURI, aRedirect, aToplevel, aReferrer); } \
  NS_IMETHOD IsVisited(nsIURI *aURI, PRBool *_retval) { return _to IsVisited(aURI, _retval); } \
  NS_IMETHOD SetPageTitle(nsIURI *aURI, const nsAString & aTitle) { return _to SetPageTitle(aURI, aTitle); } \
  NS_IMETHOD GetURIGeckoFlags(nsIURI *aURI, PRUint32 *_retval) { return _to GetURIGeckoFlags(aURI, _retval); } \
  NS_IMETHOD SetURIGeckoFlags(nsIURI *aURI, PRUint32 aFlags) { return _to SetURIGeckoFlags(aURI, aFlags); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIGLOBALHISTORY2(_to) \
  NS_IMETHOD AddURI(nsIURI *aURI, PRBool aRedirect, PRBool aToplevel, nsIURI *aReferrer) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddURI(aURI, aRedirect, aToplevel, aReferrer); } \
  NS_IMETHOD IsVisited(nsIURI *aURI, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsVisited(aURI, _retval); } \
  NS_IMETHOD SetPageTitle(nsIURI *aURI, const nsAString & aTitle) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPageTitle(aURI, aTitle); } \
  NS_IMETHOD GetURIGeckoFlags(nsIURI *aURI, PRUint32 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetURIGeckoFlags(aURI, _retval); } \
  NS_IMETHOD SetURIGeckoFlags(nsIURI *aURI, PRUint32 aFlags) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetURIGeckoFlags(aURI, aFlags); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsGlobalHistory2 : public nsIGlobalHistory2
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIGLOBALHISTORY2

  nsGlobalHistory2();

private:
  ~nsGlobalHistory2();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsGlobalHistory2, nsIGlobalHistory2)

nsGlobalHistory2::nsGlobalHistory2()
{
  /* member initializers and constructor code */
}

nsGlobalHistory2::~nsGlobalHistory2()
{
  /* destructor code */
}

/* void addURI (in nsIURI aURI, in boolean aRedirect, in boolean aToplevel, in nsIURI aReferrer); */
NS_IMETHODIMP nsGlobalHistory2::AddURI(nsIURI *aURI, PRBool aRedirect, PRBool aToplevel, nsIURI *aReferrer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isVisited (in nsIURI aURI); */
NS_IMETHODIMP nsGlobalHistory2::IsVisited(nsIURI *aURI, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setPageTitle (in nsIURI aURI, in AString aTitle); */
NS_IMETHODIMP nsGlobalHistory2::SetPageTitle(nsIURI *aURI, const nsAString & aTitle)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* unsigned long getURIGeckoFlags (in nsIURI aURI); */
NS_IMETHODIMP nsGlobalHistory2::GetURIGeckoFlags(nsIURI *aURI, PRUint32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setURIGeckoFlags (in nsIURI aURI, in unsigned long aFlags); */
NS_IMETHODIMP nsGlobalHistory2::SetURIGeckoFlags(nsIURI *aURI, PRUint32 aFlags)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIGlobalHistory2_h__ */
