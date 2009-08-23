/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/autocomplete/public/nsIAutoCompleteListener.idl
 */

#ifndef __gen_nsIAutoCompleteListener_h__
#define __gen_nsIAutoCompleteListener_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIAutoCompleteResults_h__
#include "nsIAutoCompleteResults.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
typedef PRInt32 AutoCompleteStatus;


/* starting interface:    nsIAutoCompleteStatus */
#define NS_IAUTOCOMPLETESTATUS_IID_STR "4ba0a180-097c-11d4-a449-cd1a8b47ed7c"

#define NS_IAUTOCOMPLETESTATUS_IID \
  {0x4ba0a180, 0x097c, 0x11d4, \
    { 0xa4, 0x49, 0xcd, 0x1a, 0x8b, 0x47, 0xed, 0x7c }}

class NS_NO_VTABLE nsIAutoCompleteStatus {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAUTOCOMPLETESTATUS_IID)

  enum { failed = -1 };

  enum { noMatch = 0 };

  enum { matchFound = 1 };

  enum { ignored = 2 };

  enum { failureItems = 3 };

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUTOCOMPLETESTATUS \

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUTOCOMPLETESTATUS(_to) \

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUTOCOMPLETESTATUS(_to) \

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAutoCompleteStatus : public nsIAutoCompleteStatus
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAUTOCOMPLETESTATUS

  nsAutoCompleteStatus();

private:
  ~nsAutoCompleteStatus();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAutoCompleteStatus, nsIAutoCompleteStatus)

nsAutoCompleteStatus::nsAutoCompleteStatus()
{
  /* member initializers and constructor code */
}

nsAutoCompleteStatus::~nsAutoCompleteStatus()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIAutoCompleteListener */
#define NS_IAUTOCOMPLETELISTENER_IID_STR "4ba0a181-097c-11d4-a449-cd1a8b47ed7c"

#define NS_IAUTOCOMPLETELISTENER_IID \
  {0x4ba0a181, 0x097c, 0x11d4, \
    { 0xa4, 0x49, 0xcd, 0x1a, 0x8b, 0x47, 0xed, 0x7c }}

class NS_NO_VTABLE nsIAutoCompleteListener : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAUTOCOMPLETELISTENER_IID)

  /* void onStatus (in wstring statusText); */
  NS_IMETHOD OnStatus(const PRUnichar *statusText) = 0;

  /* void onAutoComplete (in nsIAutoCompleteResults result, in AutoCompleteStatus status); */
  NS_IMETHOD OnAutoComplete(nsIAutoCompleteResults *result, AutoCompleteStatus status) = 0;

  /* attribute nsISupports param; */
  NS_IMETHOD GetParam(nsISupports * *aParam) = 0;
  NS_IMETHOD SetParam(nsISupports * aParam) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUTOCOMPLETELISTENER \
  NS_IMETHOD OnStatus(const PRUnichar *statusText); \
  NS_IMETHOD OnAutoComplete(nsIAutoCompleteResults *result, AutoCompleteStatus status); \
  NS_IMETHOD GetParam(nsISupports * *aParam); \
  NS_IMETHOD SetParam(nsISupports * aParam); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUTOCOMPLETELISTENER(_to) \
  NS_IMETHOD OnStatus(const PRUnichar *statusText) { return _to OnStatus(statusText); } \
  NS_IMETHOD OnAutoComplete(nsIAutoCompleteResults *result, AutoCompleteStatus status) { return _to OnAutoComplete(result, status); } \
  NS_IMETHOD GetParam(nsISupports * *aParam) { return _to GetParam(aParam); } \
  NS_IMETHOD SetParam(nsISupports * aParam) { return _to SetParam(aParam); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUTOCOMPLETELISTENER(_to) \
  NS_IMETHOD OnStatus(const PRUnichar *statusText) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStatus(statusText); } \
  NS_IMETHOD OnAutoComplete(nsIAutoCompleteResults *result, AutoCompleteStatus status) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnAutoComplete(result, status); } \
  NS_IMETHOD GetParam(nsISupports * *aParam) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParam(aParam); } \
  NS_IMETHOD SetParam(nsISupports * aParam) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetParam(aParam); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAutoCompleteListener : public nsIAutoCompleteListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAUTOCOMPLETELISTENER

  nsAutoCompleteListener();

private:
  ~nsAutoCompleteListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAutoCompleteListener, nsIAutoCompleteListener)

nsAutoCompleteListener::nsAutoCompleteListener()
{
  /* member initializers and constructor code */
}

nsAutoCompleteListener::~nsAutoCompleteListener()
{
  /* destructor code */
}

/* void onStatus (in wstring statusText); */
NS_IMETHODIMP nsAutoCompleteListener::OnStatus(const PRUnichar *statusText)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onAutoComplete (in nsIAutoCompleteResults result, in AutoCompleteStatus status); */
NS_IMETHODIMP nsAutoCompleteListener::OnAutoComplete(nsIAutoCompleteResults *result, AutoCompleteStatus status)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISupports param; */
NS_IMETHODIMP nsAutoCompleteListener::GetParam(nsISupports * *aParam)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteListener::SetParam(nsISupports * aParam)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIAutoCompleteListener_h__ */
