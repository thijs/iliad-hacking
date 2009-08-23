/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/autocomplete/public/nsIAutoCompleteInput.idl
 */

#ifndef __gen_nsIAutoCompleteInput_h__
#define __gen_nsIAutoCompleteInput_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIAutoCompleteController_h__
#include "nsIAutoCompleteController.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIAutoCompletePopup; /* forward declaration */


/* starting interface:    nsIAutoCompleteInput */
#define NS_IAUTOCOMPLETEINPUT_IID_STR "e312267f-8f57-43e8-a904-ff9b5d3f5aef"

#define NS_IAUTOCOMPLETEINPUT_IID \
  {0xe312267f, 0x8f57, 0x43e8, \
    { 0xa9, 0x04, 0xff, 0x9b, 0x5d, 0x3f, 0x5a, 0xef }}

class NS_NO_VTABLE nsIAutoCompleteInput : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAUTOCOMPLETEINPUT_IID)

  /* readonly attribute nsIAutoCompletePopup popup; */
  NS_IMETHOD GetPopup(nsIAutoCompletePopup * *aPopup) = 0;

  /* readonly attribute nsIAutoCompleteController controller; */
  NS_IMETHOD GetController(nsIAutoCompleteController * *aController) = 0;

  /* attribute boolean popupOpen; */
  NS_IMETHOD GetPopupOpen(PRBool *aPopupOpen) = 0;
  NS_IMETHOD SetPopupOpen(PRBool aPopupOpen) = 0;

  /* attribute boolean disableAutoComplete; */
  NS_IMETHOD GetDisableAutoComplete(PRBool *aDisableAutoComplete) = 0;
  NS_IMETHOD SetDisableAutoComplete(PRBool aDisableAutoComplete) = 0;

  /* attribute boolean completeDefaultIndex; */
  NS_IMETHOD GetCompleteDefaultIndex(PRBool *aCompleteDefaultIndex) = 0;
  NS_IMETHOD SetCompleteDefaultIndex(PRBool aCompleteDefaultIndex) = 0;

  /* attribute boolean completeSelectedIndex; */
  NS_IMETHOD GetCompleteSelectedIndex(PRBool *aCompleteSelectedIndex) = 0;
  NS_IMETHOD SetCompleteSelectedIndex(PRBool aCompleteSelectedIndex) = 0;

  /* attribute boolean forceComplete; */
  NS_IMETHOD GetForceComplete(PRBool *aForceComplete) = 0;
  NS_IMETHOD SetForceComplete(PRBool aForceComplete) = 0;

  /* attribute unsigned long minResultsForPopup; */
  NS_IMETHOD GetMinResultsForPopup(PRUint32 *aMinResultsForPopup) = 0;
  NS_IMETHOD SetMinResultsForPopup(PRUint32 aMinResultsForPopup) = 0;

  /* attribute unsigned long maxRows; */
  NS_IMETHOD GetMaxRows(PRUint32 *aMaxRows) = 0;
  NS_IMETHOD SetMaxRows(PRUint32 aMaxRows) = 0;

  /* attribute unsigned long showCommentColumn; */
  NS_IMETHOD GetShowCommentColumn(PRUint32 *aShowCommentColumn) = 0;
  NS_IMETHOD SetShowCommentColumn(PRUint32 aShowCommentColumn) = 0;

  /* attribute unsigned long timeout; */
  NS_IMETHOD GetTimeout(PRUint32 *aTimeout) = 0;
  NS_IMETHOD SetTimeout(PRUint32 aTimeout) = 0;

  /* attribute AString searchParam; */
  NS_IMETHOD GetSearchParam(nsAString & aSearchParam) = 0;
  NS_IMETHOD SetSearchParam(const nsAString & aSearchParam) = 0;

  /* readonly attribute unsigned long searchCount; */
  NS_IMETHOD GetSearchCount(PRUint32 *aSearchCount) = 0;

  /* ACString getSearchAt (in unsigned long index); */
  NS_IMETHOD GetSearchAt(PRUint32 index, nsACString & _retval) = 0;

  /* attribute AString textValue; */
  NS_IMETHOD GetTextValue(nsAString & aTextValue) = 0;
  NS_IMETHOD SetTextValue(const nsAString & aTextValue) = 0;

  /* readonly attribute long selectionStart; */
  NS_IMETHOD GetSelectionStart(PRInt32 *aSelectionStart) = 0;

  /* readonly attribute long selectionEnd; */
  NS_IMETHOD GetSelectionEnd(PRInt32 *aSelectionEnd) = 0;

  /* void selectTextRange (in long startIndex, in long endIndex); */
  NS_IMETHOD SelectTextRange(PRInt32 startIndex, PRInt32 endIndex) = 0;

  /* void onSearchComplete (); */
  NS_IMETHOD OnSearchComplete(void) = 0;

  /* boolean onTextEntered (); */
  NS_IMETHOD OnTextEntered(PRBool *_retval) = 0;

  /* boolean onTextReverted (); */
  NS_IMETHOD OnTextReverted(PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUTOCOMPLETEINPUT \
  NS_IMETHOD GetPopup(nsIAutoCompletePopup * *aPopup); \
  NS_IMETHOD GetController(nsIAutoCompleteController * *aController); \
  NS_IMETHOD GetPopupOpen(PRBool *aPopupOpen); \
  NS_IMETHOD SetPopupOpen(PRBool aPopupOpen); \
  NS_IMETHOD GetDisableAutoComplete(PRBool *aDisableAutoComplete); \
  NS_IMETHOD SetDisableAutoComplete(PRBool aDisableAutoComplete); \
  NS_IMETHOD GetCompleteDefaultIndex(PRBool *aCompleteDefaultIndex); \
  NS_IMETHOD SetCompleteDefaultIndex(PRBool aCompleteDefaultIndex); \
  NS_IMETHOD GetCompleteSelectedIndex(PRBool *aCompleteSelectedIndex); \
  NS_IMETHOD SetCompleteSelectedIndex(PRBool aCompleteSelectedIndex); \
  NS_IMETHOD GetForceComplete(PRBool *aForceComplete); \
  NS_IMETHOD SetForceComplete(PRBool aForceComplete); \
  NS_IMETHOD GetMinResultsForPopup(PRUint32 *aMinResultsForPopup); \
  NS_IMETHOD SetMinResultsForPopup(PRUint32 aMinResultsForPopup); \
  NS_IMETHOD GetMaxRows(PRUint32 *aMaxRows); \
  NS_IMETHOD SetMaxRows(PRUint32 aMaxRows); \
  NS_IMETHOD GetShowCommentColumn(PRUint32 *aShowCommentColumn); \
  NS_IMETHOD SetShowCommentColumn(PRUint32 aShowCommentColumn); \
  NS_IMETHOD GetTimeout(PRUint32 *aTimeout); \
  NS_IMETHOD SetTimeout(PRUint32 aTimeout); \
  NS_IMETHOD GetSearchParam(nsAString & aSearchParam); \
  NS_IMETHOD SetSearchParam(const nsAString & aSearchParam); \
  NS_IMETHOD GetSearchCount(PRUint32 *aSearchCount); \
  NS_IMETHOD GetSearchAt(PRUint32 index, nsACString & _retval); \
  NS_IMETHOD GetTextValue(nsAString & aTextValue); \
  NS_IMETHOD SetTextValue(const nsAString & aTextValue); \
  NS_IMETHOD GetSelectionStart(PRInt32 *aSelectionStart); \
  NS_IMETHOD GetSelectionEnd(PRInt32 *aSelectionEnd); \
  NS_IMETHOD SelectTextRange(PRInt32 startIndex, PRInt32 endIndex); \
  NS_IMETHOD OnSearchComplete(void); \
  NS_IMETHOD OnTextEntered(PRBool *_retval); \
  NS_IMETHOD OnTextReverted(PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUTOCOMPLETEINPUT(_to) \
  NS_IMETHOD GetPopup(nsIAutoCompletePopup * *aPopup) { return _to GetPopup(aPopup); } \
  NS_IMETHOD GetController(nsIAutoCompleteController * *aController) { return _to GetController(aController); } \
  NS_IMETHOD GetPopupOpen(PRBool *aPopupOpen) { return _to GetPopupOpen(aPopupOpen); } \
  NS_IMETHOD SetPopupOpen(PRBool aPopupOpen) { return _to SetPopupOpen(aPopupOpen); } \
  NS_IMETHOD GetDisableAutoComplete(PRBool *aDisableAutoComplete) { return _to GetDisableAutoComplete(aDisableAutoComplete); } \
  NS_IMETHOD SetDisableAutoComplete(PRBool aDisableAutoComplete) { return _to SetDisableAutoComplete(aDisableAutoComplete); } \
  NS_IMETHOD GetCompleteDefaultIndex(PRBool *aCompleteDefaultIndex) { return _to GetCompleteDefaultIndex(aCompleteDefaultIndex); } \
  NS_IMETHOD SetCompleteDefaultIndex(PRBool aCompleteDefaultIndex) { return _to SetCompleteDefaultIndex(aCompleteDefaultIndex); } \
  NS_IMETHOD GetCompleteSelectedIndex(PRBool *aCompleteSelectedIndex) { return _to GetCompleteSelectedIndex(aCompleteSelectedIndex); } \
  NS_IMETHOD SetCompleteSelectedIndex(PRBool aCompleteSelectedIndex) { return _to SetCompleteSelectedIndex(aCompleteSelectedIndex); } \
  NS_IMETHOD GetForceComplete(PRBool *aForceComplete) { return _to GetForceComplete(aForceComplete); } \
  NS_IMETHOD SetForceComplete(PRBool aForceComplete) { return _to SetForceComplete(aForceComplete); } \
  NS_IMETHOD GetMinResultsForPopup(PRUint32 *aMinResultsForPopup) { return _to GetMinResultsForPopup(aMinResultsForPopup); } \
  NS_IMETHOD SetMinResultsForPopup(PRUint32 aMinResultsForPopup) { return _to SetMinResultsForPopup(aMinResultsForPopup); } \
  NS_IMETHOD GetMaxRows(PRUint32 *aMaxRows) { return _to GetMaxRows(aMaxRows); } \
  NS_IMETHOD SetMaxRows(PRUint32 aMaxRows) { return _to SetMaxRows(aMaxRows); } \
  NS_IMETHOD GetShowCommentColumn(PRUint32 *aShowCommentColumn) { return _to GetShowCommentColumn(aShowCommentColumn); } \
  NS_IMETHOD SetShowCommentColumn(PRUint32 aShowCommentColumn) { return _to SetShowCommentColumn(aShowCommentColumn); } \
  NS_IMETHOD GetTimeout(PRUint32 *aTimeout) { return _to GetTimeout(aTimeout); } \
  NS_IMETHOD SetTimeout(PRUint32 aTimeout) { return _to SetTimeout(aTimeout); } \
  NS_IMETHOD GetSearchParam(nsAString & aSearchParam) { return _to GetSearchParam(aSearchParam); } \
  NS_IMETHOD SetSearchParam(const nsAString & aSearchParam) { return _to SetSearchParam(aSearchParam); } \
  NS_IMETHOD GetSearchCount(PRUint32 *aSearchCount) { return _to GetSearchCount(aSearchCount); } \
  NS_IMETHOD GetSearchAt(PRUint32 index, nsACString & _retval) { return _to GetSearchAt(index, _retval); } \
  NS_IMETHOD GetTextValue(nsAString & aTextValue) { return _to GetTextValue(aTextValue); } \
  NS_IMETHOD SetTextValue(const nsAString & aTextValue) { return _to SetTextValue(aTextValue); } \
  NS_IMETHOD GetSelectionStart(PRInt32 *aSelectionStart) { return _to GetSelectionStart(aSelectionStart); } \
  NS_IMETHOD GetSelectionEnd(PRInt32 *aSelectionEnd) { return _to GetSelectionEnd(aSelectionEnd); } \
  NS_IMETHOD SelectTextRange(PRInt32 startIndex, PRInt32 endIndex) { return _to SelectTextRange(startIndex, endIndex); } \
  NS_IMETHOD OnSearchComplete(void) { return _to OnSearchComplete(); } \
  NS_IMETHOD OnTextEntered(PRBool *_retval) { return _to OnTextEntered(_retval); } \
  NS_IMETHOD OnTextReverted(PRBool *_retval) { return _to OnTextReverted(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUTOCOMPLETEINPUT(_to) \
  NS_IMETHOD GetPopup(nsIAutoCompletePopup * *aPopup) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPopup(aPopup); } \
  NS_IMETHOD GetController(nsIAutoCompleteController * *aController) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetController(aController); } \
  NS_IMETHOD GetPopupOpen(PRBool *aPopupOpen) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPopupOpen(aPopupOpen); } \
  NS_IMETHOD SetPopupOpen(PRBool aPopupOpen) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPopupOpen(aPopupOpen); } \
  NS_IMETHOD GetDisableAutoComplete(PRBool *aDisableAutoComplete) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDisableAutoComplete(aDisableAutoComplete); } \
  NS_IMETHOD SetDisableAutoComplete(PRBool aDisableAutoComplete) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDisableAutoComplete(aDisableAutoComplete); } \
  NS_IMETHOD GetCompleteDefaultIndex(PRBool *aCompleteDefaultIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCompleteDefaultIndex(aCompleteDefaultIndex); } \
  NS_IMETHOD SetCompleteDefaultIndex(PRBool aCompleteDefaultIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCompleteDefaultIndex(aCompleteDefaultIndex); } \
  NS_IMETHOD GetCompleteSelectedIndex(PRBool *aCompleteSelectedIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCompleteSelectedIndex(aCompleteSelectedIndex); } \
  NS_IMETHOD SetCompleteSelectedIndex(PRBool aCompleteSelectedIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCompleteSelectedIndex(aCompleteSelectedIndex); } \
  NS_IMETHOD GetForceComplete(PRBool *aForceComplete) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetForceComplete(aForceComplete); } \
  NS_IMETHOD SetForceComplete(PRBool aForceComplete) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetForceComplete(aForceComplete); } \
  NS_IMETHOD GetMinResultsForPopup(PRUint32 *aMinResultsForPopup) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMinResultsForPopup(aMinResultsForPopup); } \
  NS_IMETHOD SetMinResultsForPopup(PRUint32 aMinResultsForPopup) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetMinResultsForPopup(aMinResultsForPopup); } \
  NS_IMETHOD GetMaxRows(PRUint32 *aMaxRows) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMaxRows(aMaxRows); } \
  NS_IMETHOD SetMaxRows(PRUint32 aMaxRows) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetMaxRows(aMaxRows); } \
  NS_IMETHOD GetShowCommentColumn(PRUint32 *aShowCommentColumn) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetShowCommentColumn(aShowCommentColumn); } \
  NS_IMETHOD SetShowCommentColumn(PRUint32 aShowCommentColumn) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetShowCommentColumn(aShowCommentColumn); } \
  NS_IMETHOD GetTimeout(PRUint32 *aTimeout) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTimeout(aTimeout); } \
  NS_IMETHOD SetTimeout(PRUint32 aTimeout) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetTimeout(aTimeout); } \
  NS_IMETHOD GetSearchParam(nsAString & aSearchParam) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSearchParam(aSearchParam); } \
  NS_IMETHOD SetSearchParam(const nsAString & aSearchParam) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSearchParam(aSearchParam); } \
  NS_IMETHOD GetSearchCount(PRUint32 *aSearchCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSearchCount(aSearchCount); } \
  NS_IMETHOD GetSearchAt(PRUint32 index, nsACString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSearchAt(index, _retval); } \
  NS_IMETHOD GetTextValue(nsAString & aTextValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTextValue(aTextValue); } \
  NS_IMETHOD SetTextValue(const nsAString & aTextValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetTextValue(aTextValue); } \
  NS_IMETHOD GetSelectionStart(PRInt32 *aSelectionStart) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSelectionStart(aSelectionStart); } \
  NS_IMETHOD GetSelectionEnd(PRInt32 *aSelectionEnd) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSelectionEnd(aSelectionEnd); } \
  NS_IMETHOD SelectTextRange(PRInt32 startIndex, PRInt32 endIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->SelectTextRange(startIndex, endIndex); } \
  NS_IMETHOD OnSearchComplete(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnSearchComplete(); } \
  NS_IMETHOD OnTextEntered(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnTextEntered(_retval); } \
  NS_IMETHOD OnTextReverted(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnTextReverted(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAutoCompleteInput : public nsIAutoCompleteInput
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAUTOCOMPLETEINPUT

  nsAutoCompleteInput();

private:
  ~nsAutoCompleteInput();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAutoCompleteInput, nsIAutoCompleteInput)

nsAutoCompleteInput::nsAutoCompleteInput()
{
  /* member initializers and constructor code */
}

nsAutoCompleteInput::~nsAutoCompleteInput()
{
  /* destructor code */
}

/* readonly attribute nsIAutoCompletePopup popup; */
NS_IMETHODIMP nsAutoCompleteInput::GetPopup(nsIAutoCompletePopup * *aPopup)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIAutoCompleteController controller; */
NS_IMETHODIMP nsAutoCompleteInput::GetController(nsIAutoCompleteController * *aController)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean popupOpen; */
NS_IMETHODIMP nsAutoCompleteInput::GetPopupOpen(PRBool *aPopupOpen)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetPopupOpen(PRBool aPopupOpen)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean disableAutoComplete; */
NS_IMETHODIMP nsAutoCompleteInput::GetDisableAutoComplete(PRBool *aDisableAutoComplete)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetDisableAutoComplete(PRBool aDisableAutoComplete)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean completeDefaultIndex; */
NS_IMETHODIMP nsAutoCompleteInput::GetCompleteDefaultIndex(PRBool *aCompleteDefaultIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetCompleteDefaultIndex(PRBool aCompleteDefaultIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean completeSelectedIndex; */
NS_IMETHODIMP nsAutoCompleteInput::GetCompleteSelectedIndex(PRBool *aCompleteSelectedIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetCompleteSelectedIndex(PRBool aCompleteSelectedIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean forceComplete; */
NS_IMETHODIMP nsAutoCompleteInput::GetForceComplete(PRBool *aForceComplete)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetForceComplete(PRBool aForceComplete)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute unsigned long minResultsForPopup; */
NS_IMETHODIMP nsAutoCompleteInput::GetMinResultsForPopup(PRUint32 *aMinResultsForPopup)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetMinResultsForPopup(PRUint32 aMinResultsForPopup)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute unsigned long maxRows; */
NS_IMETHODIMP nsAutoCompleteInput::GetMaxRows(PRUint32 *aMaxRows)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetMaxRows(PRUint32 aMaxRows)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute unsigned long showCommentColumn; */
NS_IMETHODIMP nsAutoCompleteInput::GetShowCommentColumn(PRUint32 *aShowCommentColumn)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetShowCommentColumn(PRUint32 aShowCommentColumn)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute unsigned long timeout; */
NS_IMETHODIMP nsAutoCompleteInput::GetTimeout(PRUint32 *aTimeout)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetTimeout(PRUint32 aTimeout)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute AString searchParam; */
NS_IMETHODIMP nsAutoCompleteInput::GetSearchParam(nsAString & aSearchParam)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetSearchParam(const nsAString & aSearchParam)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long searchCount; */
NS_IMETHODIMP nsAutoCompleteInput::GetSearchCount(PRUint32 *aSearchCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* ACString getSearchAt (in unsigned long index); */
NS_IMETHODIMP nsAutoCompleteInput::GetSearchAt(PRUint32 index, nsACString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute AString textValue; */
NS_IMETHODIMP nsAutoCompleteInput::GetTextValue(nsAString & aTextValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteInput::SetTextValue(const nsAString & aTextValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute long selectionStart; */
NS_IMETHODIMP nsAutoCompleteInput::GetSelectionStart(PRInt32 *aSelectionStart)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute long selectionEnd; */
NS_IMETHODIMP nsAutoCompleteInput::GetSelectionEnd(PRInt32 *aSelectionEnd)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void selectTextRange (in long startIndex, in long endIndex); */
NS_IMETHODIMP nsAutoCompleteInput::SelectTextRange(PRInt32 startIndex, PRInt32 endIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onSearchComplete (); */
NS_IMETHODIMP nsAutoCompleteInput::OnSearchComplete()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean onTextEntered (); */
NS_IMETHODIMP nsAutoCompleteInput::OnTextEntered(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean onTextReverted (); */
NS_IMETHODIMP nsAutoCompleteInput::OnTextReverted(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIAutoCompleteInput_h__ */
