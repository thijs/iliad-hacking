/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/autocomplete/public/nsIAutoCompleteController.idl
 */

#ifndef __gen_nsIAutoCompleteController_h__
#define __gen_nsIAutoCompleteController_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIAutoCompleteInput; /* forward declaration */


/* starting interface:    nsIAutoCompleteController */
#define NS_IAUTOCOMPLETECONTROLLER_IID_STR "cf2aca0c-4fb1-42e4-8a54-23e832cb2a98"

#define NS_IAUTOCOMPLETECONTROLLER_IID \
  {0xcf2aca0c, 0x4fb1, 0x42e4, \
    { 0x8a, 0x54, 0x23, 0xe8, 0x32, 0xcb, 0x2a, 0x98 }}

class NS_NO_VTABLE nsIAutoCompleteController : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAUTOCOMPLETECONTROLLER_IID)

  enum { STATUS_NONE = 1U };

  enum { STATUS_SEARCHING = 2U };

  enum { STATUS_COMPLETE_NO_MATCH = 3U };

  enum { STATUS_COMPLETE_MATCH = 4U };

  enum { KEY_UP = 1U };

  enum { KEY_DOWN = 2U };

  enum { KEY_LEFT = 3U };

  enum { KEY_RIGHT = 4U };

  enum { KEY_PAGE_UP = 5U };

  enum { KEY_PAGE_DOWN = 6U };

  enum { KEY_HOME = 7U };

  enum { KEY_END = 8U };

  /* attribute nsIAutoCompleteInput input; */
  NS_IMETHOD GetInput(nsIAutoCompleteInput * *aInput) = 0;
  NS_IMETHOD SetInput(nsIAutoCompleteInput * aInput) = 0;

  /* readonly attribute unsigned short searchStatus; */
  NS_IMETHOD GetSearchStatus(PRUint16 *aSearchStatus) = 0;

  /* readonly attribute unsigned long matchCount; */
  NS_IMETHOD GetMatchCount(PRUint32 *aMatchCount) = 0;

  /* void startSearch (in AString searchString); */
  NS_IMETHOD StartSearch(const nsAString & searchString) = 0;

  /* void handleText (in boolean aIgnoreSelection); */
  NS_IMETHOD HandleText(PRBool aIgnoreSelection) = 0;

  /* boolean handleEnter (); */
  NS_IMETHOD HandleEnter(PRBool *_retval) = 0;

  /* boolean handleEscape (); */
  NS_IMETHOD HandleEscape(PRBool *_retval) = 0;

  /* void handleStartComposition (); */
  NS_IMETHOD HandleStartComposition(void) = 0;

  /* void handleEndComposition (); */
  NS_IMETHOD HandleEndComposition(void) = 0;

  /* void handleTab (); */
  NS_IMETHOD HandleTab(void) = 0;

  /* boolean handleKeyNavigation (in unsigned short key); */
  NS_IMETHOD HandleKeyNavigation(PRUint16 key, PRBool *_retval) = 0;

  /* boolean handleDelete (); */
  NS_IMETHOD HandleDelete(PRBool *_retval) = 0;

  /* AString getValueAt (in long index); */
  NS_IMETHOD GetValueAt(PRInt32 index, nsAString & _retval) = 0;

  /* AString getCommentAt (in long index); */
  NS_IMETHOD GetCommentAt(PRInt32 index, nsAString & _retval) = 0;

  /* AString getStyleAt (in long index); */
  NS_IMETHOD GetStyleAt(PRInt32 index, nsAString & _retval) = 0;

  /* void setSearchString (in AString aSearchString); */
  NS_IMETHOD SetSearchString(const nsAString & aSearchString) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUTOCOMPLETECONTROLLER \
  NS_IMETHOD GetInput(nsIAutoCompleteInput * *aInput); \
  NS_IMETHOD SetInput(nsIAutoCompleteInput * aInput); \
  NS_IMETHOD GetSearchStatus(PRUint16 *aSearchStatus); \
  NS_IMETHOD GetMatchCount(PRUint32 *aMatchCount); \
  NS_IMETHOD StartSearch(const nsAString & searchString); \
  NS_IMETHOD HandleText(PRBool aIgnoreSelection); \
  NS_IMETHOD HandleEnter(PRBool *_retval); \
  NS_IMETHOD HandleEscape(PRBool *_retval); \
  NS_IMETHOD HandleStartComposition(void); \
  NS_IMETHOD HandleEndComposition(void); \
  NS_IMETHOD HandleTab(void); \
  NS_IMETHOD HandleKeyNavigation(PRUint16 key, PRBool *_retval); \
  NS_IMETHOD HandleDelete(PRBool *_retval); \
  NS_IMETHOD GetValueAt(PRInt32 index, nsAString & _retval); \
  NS_IMETHOD GetCommentAt(PRInt32 index, nsAString & _retval); \
  NS_IMETHOD GetStyleAt(PRInt32 index, nsAString & _retval); \
  NS_IMETHOD SetSearchString(const nsAString & aSearchString); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUTOCOMPLETECONTROLLER(_to) \
  NS_IMETHOD GetInput(nsIAutoCompleteInput * *aInput) { return _to GetInput(aInput); } \
  NS_IMETHOD SetInput(nsIAutoCompleteInput * aInput) { return _to SetInput(aInput); } \
  NS_IMETHOD GetSearchStatus(PRUint16 *aSearchStatus) { return _to GetSearchStatus(aSearchStatus); } \
  NS_IMETHOD GetMatchCount(PRUint32 *aMatchCount) { return _to GetMatchCount(aMatchCount); } \
  NS_IMETHOD StartSearch(const nsAString & searchString) { return _to StartSearch(searchString); } \
  NS_IMETHOD HandleText(PRBool aIgnoreSelection) { return _to HandleText(aIgnoreSelection); } \
  NS_IMETHOD HandleEnter(PRBool *_retval) { return _to HandleEnter(_retval); } \
  NS_IMETHOD HandleEscape(PRBool *_retval) { return _to HandleEscape(_retval); } \
  NS_IMETHOD HandleStartComposition(void) { return _to HandleStartComposition(); } \
  NS_IMETHOD HandleEndComposition(void) { return _to HandleEndComposition(); } \
  NS_IMETHOD HandleTab(void) { return _to HandleTab(); } \
  NS_IMETHOD HandleKeyNavigation(PRUint16 key, PRBool *_retval) { return _to HandleKeyNavigation(key, _retval); } \
  NS_IMETHOD HandleDelete(PRBool *_retval) { return _to HandleDelete(_retval); } \
  NS_IMETHOD GetValueAt(PRInt32 index, nsAString & _retval) { return _to GetValueAt(index, _retval); } \
  NS_IMETHOD GetCommentAt(PRInt32 index, nsAString & _retval) { return _to GetCommentAt(index, _retval); } \
  NS_IMETHOD GetStyleAt(PRInt32 index, nsAString & _retval) { return _to GetStyleAt(index, _retval); } \
  NS_IMETHOD SetSearchString(const nsAString & aSearchString) { return _to SetSearchString(aSearchString); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUTOCOMPLETECONTROLLER(_to) \
  NS_IMETHOD GetInput(nsIAutoCompleteInput * *aInput) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInput(aInput); } \
  NS_IMETHOD SetInput(nsIAutoCompleteInput * aInput) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetInput(aInput); } \
  NS_IMETHOD GetSearchStatus(PRUint16 *aSearchStatus) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSearchStatus(aSearchStatus); } \
  NS_IMETHOD GetMatchCount(PRUint32 *aMatchCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMatchCount(aMatchCount); } \
  NS_IMETHOD StartSearch(const nsAString & searchString) { return !_to ? NS_ERROR_NULL_POINTER : _to->StartSearch(searchString); } \
  NS_IMETHOD HandleText(PRBool aIgnoreSelection) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleText(aIgnoreSelection); } \
  NS_IMETHOD HandleEnter(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleEnter(_retval); } \
  NS_IMETHOD HandleEscape(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleEscape(_retval); } \
  NS_IMETHOD HandleStartComposition(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleStartComposition(); } \
  NS_IMETHOD HandleEndComposition(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleEndComposition(); } \
  NS_IMETHOD HandleTab(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleTab(); } \
  NS_IMETHOD HandleKeyNavigation(PRUint16 key, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleKeyNavigation(key, _retval); } \
  NS_IMETHOD HandleDelete(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleDelete(_retval); } \
  NS_IMETHOD GetValueAt(PRInt32 index, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetValueAt(index, _retval); } \
  NS_IMETHOD GetCommentAt(PRInt32 index, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCommentAt(index, _retval); } \
  NS_IMETHOD GetStyleAt(PRInt32 index, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStyleAt(index, _retval); } \
  NS_IMETHOD SetSearchString(const nsAString & aSearchString) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSearchString(aSearchString); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAutoCompleteController : public nsIAutoCompleteController
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAUTOCOMPLETECONTROLLER

  nsAutoCompleteController();

private:
  ~nsAutoCompleteController();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAutoCompleteController, nsIAutoCompleteController)

nsAutoCompleteController::nsAutoCompleteController()
{
  /* member initializers and constructor code */
}

nsAutoCompleteController::~nsAutoCompleteController()
{
  /* destructor code */
}

/* attribute nsIAutoCompleteInput input; */
NS_IMETHODIMP nsAutoCompleteController::GetInput(nsIAutoCompleteInput * *aInput)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteController::SetInput(nsIAutoCompleteInput * aInput)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short searchStatus; */
NS_IMETHODIMP nsAutoCompleteController::GetSearchStatus(PRUint16 *aSearchStatus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long matchCount; */
NS_IMETHODIMP nsAutoCompleteController::GetMatchCount(PRUint32 *aMatchCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void startSearch (in AString searchString); */
NS_IMETHODIMP nsAutoCompleteController::StartSearch(const nsAString & searchString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void handleText (in boolean aIgnoreSelection); */
NS_IMETHODIMP nsAutoCompleteController::HandleText(PRBool aIgnoreSelection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean handleEnter (); */
NS_IMETHODIMP nsAutoCompleteController::HandleEnter(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean handleEscape (); */
NS_IMETHODIMP nsAutoCompleteController::HandleEscape(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void handleStartComposition (); */
NS_IMETHODIMP nsAutoCompleteController::HandleStartComposition()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void handleEndComposition (); */
NS_IMETHODIMP nsAutoCompleteController::HandleEndComposition()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void handleTab (); */
NS_IMETHODIMP nsAutoCompleteController::HandleTab()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean handleKeyNavigation (in unsigned short key); */
NS_IMETHODIMP nsAutoCompleteController::HandleKeyNavigation(PRUint16 key, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean handleDelete (); */
NS_IMETHODIMP nsAutoCompleteController::HandleDelete(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getValueAt (in long index); */
NS_IMETHODIMP nsAutoCompleteController::GetValueAt(PRInt32 index, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getCommentAt (in long index); */
NS_IMETHODIMP nsAutoCompleteController::GetCommentAt(PRInt32 index, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getStyleAt (in long index); */
NS_IMETHODIMP nsAutoCompleteController::GetStyleAt(PRInt32 index, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setSearchString (in AString aSearchString); */
NS_IMETHODIMP nsAutoCompleteController::SetSearchString(const nsAString & aSearchString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIAutoCompleteController_h__ */
