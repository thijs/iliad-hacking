/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/typeaheadfind/public/nsITypeAheadFind.idl
 */

#ifndef __gen_nsITypeAheadFind_h__
#define __gen_nsITypeAheadFind_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_domstubs_h__
#include "domstubs.h"
#endif

#ifndef __gen_nsISupportsPrimitives_h__
#include "nsISupportsPrimitives.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDocShell; /* forward declaration */


/* starting interface:    nsITypeAheadFind */
#define NS_ITYPEAHEADFIND_IID_STR "376da416-e6b3-4bac-98f3-6aa408742751"

#define NS_ITYPEAHEADFIND_IID \
  {0x376da416, 0xe6b3, 0x4bac, \
    { 0x98, 0xf3, 0x6a, 0xa4, 0x08, 0x74, 0x27, 0x51 }}

class NS_NO_VTABLE nsITypeAheadFind : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ITYPEAHEADFIND_IID)

  /* void init (in nsIDocShell aDocShell); */
  NS_IMETHOD Init(nsIDocShell *aDocShell) = 0;

  enum { FIND_FOUND = 0U };

  enum { FIND_NOTFOUND = 1U };

  enum { FIND_WRAPPED = 2U };

  /* unsigned short find (in AString aSearchString, in boolean aLinksOnly); */
  NS_IMETHOD Find(const nsAString & aSearchString, PRBool aLinksOnly, PRUint16 *_retval) = 0;

  /* unsigned short findNext (); */
  NS_IMETHOD FindNext(PRUint16 *_retval) = 0;

  /* unsigned short findPrevious (); */
  NS_IMETHOD FindPrevious(PRUint16 *_retval) = 0;

  /* void setDocShell (in nsIDocShell aDocShell); */
  NS_IMETHOD SetDocShell(nsIDocShell *aDocShell) = 0;

  /* readonly attribute AString searchString; */
  NS_IMETHOD GetSearchString(nsAString & aSearchString) = 0;

  /* attribute boolean focusLinks; */
  NS_IMETHOD GetFocusLinks(PRBool *aFocusLinks) = 0;
  NS_IMETHOD SetFocusLinks(PRBool aFocusLinks) = 0;

  /* attribute boolean caseSensitive; */
  NS_IMETHOD GetCaseSensitive(PRBool *aCaseSensitive) = 0;
  NS_IMETHOD SetCaseSensitive(PRBool aCaseSensitive) = 0;

  /* readonly attribute nsIDOMElement foundLink; */
  NS_IMETHOD GetFoundLink(nsIDOMElement * *aFoundLink) = 0;

  /* readonly attribute nsIDOMWindow currentWindow; */
  NS_IMETHOD GetCurrentWindow(nsIDOMWindow * *aCurrentWindow) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITYPEAHEADFIND \
  NS_IMETHOD Init(nsIDocShell *aDocShell); \
  NS_IMETHOD Find(const nsAString & aSearchString, PRBool aLinksOnly, PRUint16 *_retval); \
  NS_IMETHOD FindNext(PRUint16 *_retval); \
  NS_IMETHOD FindPrevious(PRUint16 *_retval); \
  NS_IMETHOD SetDocShell(nsIDocShell *aDocShell); \
  NS_IMETHOD GetSearchString(nsAString & aSearchString); \
  NS_IMETHOD GetFocusLinks(PRBool *aFocusLinks); \
  NS_IMETHOD SetFocusLinks(PRBool aFocusLinks); \
  NS_IMETHOD GetCaseSensitive(PRBool *aCaseSensitive); \
  NS_IMETHOD SetCaseSensitive(PRBool aCaseSensitive); \
  NS_IMETHOD GetFoundLink(nsIDOMElement * *aFoundLink); \
  NS_IMETHOD GetCurrentWindow(nsIDOMWindow * *aCurrentWindow); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITYPEAHEADFIND(_to) \
  NS_IMETHOD Init(nsIDocShell *aDocShell) { return _to Init(aDocShell); } \
  NS_IMETHOD Find(const nsAString & aSearchString, PRBool aLinksOnly, PRUint16 *_retval) { return _to Find(aSearchString, aLinksOnly, _retval); } \
  NS_IMETHOD FindNext(PRUint16 *_retval) { return _to FindNext(_retval); } \
  NS_IMETHOD FindPrevious(PRUint16 *_retval) { return _to FindPrevious(_retval); } \
  NS_IMETHOD SetDocShell(nsIDocShell *aDocShell) { return _to SetDocShell(aDocShell); } \
  NS_IMETHOD GetSearchString(nsAString & aSearchString) { return _to GetSearchString(aSearchString); } \
  NS_IMETHOD GetFocusLinks(PRBool *aFocusLinks) { return _to GetFocusLinks(aFocusLinks); } \
  NS_IMETHOD SetFocusLinks(PRBool aFocusLinks) { return _to SetFocusLinks(aFocusLinks); } \
  NS_IMETHOD GetCaseSensitive(PRBool *aCaseSensitive) { return _to GetCaseSensitive(aCaseSensitive); } \
  NS_IMETHOD SetCaseSensitive(PRBool aCaseSensitive) { return _to SetCaseSensitive(aCaseSensitive); } \
  NS_IMETHOD GetFoundLink(nsIDOMElement * *aFoundLink) { return _to GetFoundLink(aFoundLink); } \
  NS_IMETHOD GetCurrentWindow(nsIDOMWindow * *aCurrentWindow) { return _to GetCurrentWindow(aCurrentWindow); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITYPEAHEADFIND(_to) \
  NS_IMETHOD Init(nsIDocShell *aDocShell) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aDocShell); } \
  NS_IMETHOD Find(const nsAString & aSearchString, PRBool aLinksOnly, PRUint16 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Find(aSearchString, aLinksOnly, _retval); } \
  NS_IMETHOD FindNext(PRUint16 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->FindNext(_retval); } \
  NS_IMETHOD FindPrevious(PRUint16 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->FindPrevious(_retval); } \
  NS_IMETHOD SetDocShell(nsIDocShell *aDocShell) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDocShell(aDocShell); } \
  NS_IMETHOD GetSearchString(nsAString & aSearchString) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSearchString(aSearchString); } \
  NS_IMETHOD GetFocusLinks(PRBool *aFocusLinks) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFocusLinks(aFocusLinks); } \
  NS_IMETHOD SetFocusLinks(PRBool aFocusLinks) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFocusLinks(aFocusLinks); } \
  NS_IMETHOD GetCaseSensitive(PRBool *aCaseSensitive) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCaseSensitive(aCaseSensitive); } \
  NS_IMETHOD SetCaseSensitive(PRBool aCaseSensitive) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCaseSensitive(aCaseSensitive); } \
  NS_IMETHOD GetFoundLink(nsIDOMElement * *aFoundLink) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFoundLink(aFoundLink); } \
  NS_IMETHOD GetCurrentWindow(nsIDOMWindow * *aCurrentWindow) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCurrentWindow(aCurrentWindow); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsTypeAheadFind : public nsITypeAheadFind
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITYPEAHEADFIND

  nsTypeAheadFind();

private:
  ~nsTypeAheadFind();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsTypeAheadFind, nsITypeAheadFind)

nsTypeAheadFind::nsTypeAheadFind()
{
  /* member initializers and constructor code */
}

nsTypeAheadFind::~nsTypeAheadFind()
{
  /* destructor code */
}

/* void init (in nsIDocShell aDocShell); */
NS_IMETHODIMP nsTypeAheadFind::Init(nsIDocShell *aDocShell)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* unsigned short find (in AString aSearchString, in boolean aLinksOnly); */
NS_IMETHODIMP nsTypeAheadFind::Find(const nsAString & aSearchString, PRBool aLinksOnly, PRUint16 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* unsigned short findNext (); */
NS_IMETHODIMP nsTypeAheadFind::FindNext(PRUint16 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* unsigned short findPrevious (); */
NS_IMETHODIMP nsTypeAheadFind::FindPrevious(PRUint16 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setDocShell (in nsIDocShell aDocShell); */
NS_IMETHODIMP nsTypeAheadFind::SetDocShell(nsIDocShell *aDocShell)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString searchString; */
NS_IMETHODIMP nsTypeAheadFind::GetSearchString(nsAString & aSearchString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean focusLinks; */
NS_IMETHODIMP nsTypeAheadFind::GetFocusLinks(PRBool *aFocusLinks)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsTypeAheadFind::SetFocusLinks(PRBool aFocusLinks)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean caseSensitive; */
NS_IMETHODIMP nsTypeAheadFind::GetCaseSensitive(PRBool *aCaseSensitive)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsTypeAheadFind::SetCaseSensitive(PRBool aCaseSensitive)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement foundLink; */
NS_IMETHODIMP nsTypeAheadFind::GetFoundLink(nsIDOMElement * *aFoundLink)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMWindow currentWindow; */
NS_IMETHODIMP nsTypeAheadFind::GetCurrentWindow(nsIDOMWindow * *aCurrentWindow)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsITypeAheadFind_h__ */
