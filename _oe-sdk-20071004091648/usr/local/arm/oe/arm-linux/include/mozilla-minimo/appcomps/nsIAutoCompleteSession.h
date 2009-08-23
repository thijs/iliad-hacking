/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/autocomplete/public/nsIAutoCompleteSession.idl
 */

#ifndef __gen_nsIAutoCompleteSession_h__
#define __gen_nsIAutoCompleteSession_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include "nsIAutoCompleteResults.h"
#include "nsIAutoCompleteListener.h"
class nsIAutoCompleteResults; /* forward declaration */

class nsIAutoCompleteListener; /* forward declaration */


/* starting interface:    nsIAutoCompleteSession */
#define NS_IAUTOCOMPLETESESSION_IID_STR "e0140b60-0264-11d4-8378-000000000000"

#define NS_IAUTOCOMPLETESESSION_IID \
  {0xe0140b60, 0x0264, 0x11d4, \
    { 0x83, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}

class NS_NO_VTABLE nsIAutoCompleteSession : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAUTOCOMPLETESESSION_IID)

  /* void onStartLookup (in wstring searchString, in nsIAutoCompleteResults previousSearchResult, in nsIAutoCompleteListener listener); */
  NS_IMETHOD OnStartLookup(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener) = 0;

  /* void onStopLookup (); */
  NS_IMETHOD OnStopLookup(void) = 0;

  /* void onAutoComplete (in wstring searchString, in nsIAutoCompleteResults previousSearchResult, in nsIAutoCompleteListener listener); */
  NS_IMETHOD OnAutoComplete(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUTOCOMPLETESESSION \
  NS_IMETHOD OnStartLookup(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener); \
  NS_IMETHOD OnStopLookup(void); \
  NS_IMETHOD OnAutoComplete(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUTOCOMPLETESESSION(_to) \
  NS_IMETHOD OnStartLookup(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener) { return _to OnStartLookup(searchString, previousSearchResult, listener); } \
  NS_IMETHOD OnStopLookup(void) { return _to OnStopLookup(); } \
  NS_IMETHOD OnAutoComplete(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener) { return _to OnAutoComplete(searchString, previousSearchResult, listener); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUTOCOMPLETESESSION(_to) \
  NS_IMETHOD OnStartLookup(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStartLookup(searchString, previousSearchResult, listener); } \
  NS_IMETHOD OnStopLookup(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStopLookup(); } \
  NS_IMETHOD OnAutoComplete(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnAutoComplete(searchString, previousSearchResult, listener); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAutoCompleteSession : public nsIAutoCompleteSession
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAUTOCOMPLETESESSION

  nsAutoCompleteSession();

private:
  ~nsAutoCompleteSession();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAutoCompleteSession, nsIAutoCompleteSession)

nsAutoCompleteSession::nsAutoCompleteSession()
{
  /* member initializers and constructor code */
}

nsAutoCompleteSession::~nsAutoCompleteSession()
{
  /* destructor code */
}

/* void onStartLookup (in wstring searchString, in nsIAutoCompleteResults previousSearchResult, in nsIAutoCompleteListener listener); */
NS_IMETHODIMP nsAutoCompleteSession::OnStartLookup(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onStopLookup (); */
NS_IMETHODIMP nsAutoCompleteSession::OnStopLookup()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onAutoComplete (in wstring searchString, in nsIAutoCompleteResults previousSearchResult, in nsIAutoCompleteListener listener); */
NS_IMETHODIMP nsAutoCompleteSession::OnAutoComplete(const PRUnichar *searchString, nsIAutoCompleteResults *previousSearchResult, nsIAutoCompleteListener *listener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIAutoCompleteSession_h__ */
