/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/webshell/public/nsIRefreshURI.idl
 */

#ifndef __gen_nsIRefreshURI_h__
#define __gen_nsIRefreshURI_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIURI_h__
#include "nsIURI.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIChannel; /* forward declaration */


/* starting interface:    nsIRefreshURI */
#define NS_IREFRESHURI_IID_STR "69efc430-2efe-11d2-9e5d-006008bf092e"

#define NS_IREFRESHURI_IID \
  {0x69efc430, 0x2efe, 0x11d2, \
    { 0x9e, 0x5d, 0x00, 0x60, 0x08, 0xbf, 0x09, 0x2e }}

class NS_NO_VTABLE nsIRefreshURI : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IREFRESHURI_IID)

  /**
      * Load a uri after waiting for aMillis milliseconds. If the docshell
      * is busy loading a page currently, the refresh request will be
      * queued and executed when the current load finishes. 
      *
      * @param aUri The uri to refresh.
      * @param aMillis The number of milliseconds to wait.
      * @param aRepeat Flag to indicate if the uri is to be 
      *                repeatedly refreshed every aMillis milliseconds.
      * @parem aMetaRefresh Flag to indicate if this is a Meta refresh.
      */
  /* void refreshURI (in nsIURI aURI, in long aMillis, in boolean aRepeat, in boolean aMetaRefresh); */
  NS_IMETHOD RefreshURI(nsIURI *aURI, PRInt32 aMillis, PRBool aRepeat, PRBool aMetaRefresh) = 0;

  /**
      * Checks the passed in channel to see if there is a refresh header, 
      * if there is, will setup a timer to refresh the uri found
      * in the header. If docshell is busy loading a page currently, the
      * request will be queued and executed when the current page 
      * finishes loading. 
      *
      * @param aChannel The channel to be parsed. 
      */
  /* void setupRefreshURI (in nsIChannel aChannel); */
  NS_IMETHOD SetupRefreshURI(nsIChannel *aChannel) = 0;

  /**
      * Parses the passed in header string and sets up a refreshURI if
      * a "refresh" header is found. If docshell is busy loading a page 
      * currently, the request will be queued and executed when 
      * the current page finishes loading. 
      *
      * @param aBaseURI base URI to resolve refresh uri with.
      * @param aHeader  The meta refresh header string.
      */
  /* void setupRefreshURIFromHeader (in nsIURI aBaseURI, in ACString aHeader); */
  NS_IMETHOD SetupRefreshURIFromHeader(nsIURI *aBaseURI, const nsACString & aHeader) = 0;

  /**
      * Cancels all timer loads.
      */
  /* void cancelRefreshURITimers (); */
  NS_IMETHOD CancelRefreshURITimers(void) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIREFRESHURI \
  NS_IMETHOD RefreshURI(nsIURI *aURI, PRInt32 aMillis, PRBool aRepeat, PRBool aMetaRefresh); \
  NS_IMETHOD SetupRefreshURI(nsIChannel *aChannel); \
  NS_IMETHOD SetupRefreshURIFromHeader(nsIURI *aBaseURI, const nsACString & aHeader); \
  NS_IMETHOD CancelRefreshURITimers(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIREFRESHURI(_to) \
  NS_IMETHOD RefreshURI(nsIURI *aURI, PRInt32 aMillis, PRBool aRepeat, PRBool aMetaRefresh) { return _to RefreshURI(aURI, aMillis, aRepeat, aMetaRefresh); } \
  NS_IMETHOD SetupRefreshURI(nsIChannel *aChannel) { return _to SetupRefreshURI(aChannel); } \
  NS_IMETHOD SetupRefreshURIFromHeader(nsIURI *aBaseURI, const nsACString & aHeader) { return _to SetupRefreshURIFromHeader(aBaseURI, aHeader); } \
  NS_IMETHOD CancelRefreshURITimers(void) { return _to CancelRefreshURITimers(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIREFRESHURI(_to) \
  NS_IMETHOD RefreshURI(nsIURI *aURI, PRInt32 aMillis, PRBool aRepeat, PRBool aMetaRefresh) { return !_to ? NS_ERROR_NULL_POINTER : _to->RefreshURI(aURI, aMillis, aRepeat, aMetaRefresh); } \
  NS_IMETHOD SetupRefreshURI(nsIChannel *aChannel) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetupRefreshURI(aChannel); } \
  NS_IMETHOD SetupRefreshURIFromHeader(nsIURI *aBaseURI, const nsACString & aHeader) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetupRefreshURIFromHeader(aBaseURI, aHeader); } \
  NS_IMETHOD CancelRefreshURITimers(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->CancelRefreshURITimers(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsRefreshURI : public nsIRefreshURI
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIREFRESHURI

  nsRefreshURI();

private:
  ~nsRefreshURI();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsRefreshURI, nsIRefreshURI)

nsRefreshURI::nsRefreshURI()
{
  /* member initializers and constructor code */
}

nsRefreshURI::~nsRefreshURI()
{
  /* destructor code */
}

/* void refreshURI (in nsIURI aURI, in long aMillis, in boolean aRepeat, in boolean aMetaRefresh); */
NS_IMETHODIMP nsRefreshURI::RefreshURI(nsIURI *aURI, PRInt32 aMillis, PRBool aRepeat, PRBool aMetaRefresh)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setupRefreshURI (in nsIChannel aChannel); */
NS_IMETHODIMP nsRefreshURI::SetupRefreshURI(nsIChannel *aChannel)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setupRefreshURIFromHeader (in nsIURI aBaseURI, in ACString aHeader); */
NS_IMETHODIMP nsRefreshURI::SetupRefreshURIFromHeader(nsIURI *aBaseURI, const nsACString & aHeader)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void cancelRefreshURITimers (); */
NS_IMETHODIMP nsRefreshURI::CancelRefreshURITimers()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIRefreshURI_h__ */
