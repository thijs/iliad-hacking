/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/autocomplete/public/nsIAutoCompletePopup.idl
 */

#ifndef __gen_nsIAutoCompletePopup_h__
#define __gen_nsIAutoCompletePopup_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIAutoCompleteInput; /* forward declaration */


/* starting interface:    nsIAutoCompletePopup */
#define NS_IAUTOCOMPLETEPOPUP_IID_STR "65f6cd46-22ec-4329-bb3b-bcd1103f2204"

#define NS_IAUTOCOMPLETEPOPUP_IID \
  {0x65f6cd46, 0x22ec, 0x4329, \
    { 0xbb, 0x3b, 0xbc, 0xd1, 0x10, 0x3f, 0x22, 0x04 }}

class NS_NO_VTABLE nsIAutoCompletePopup : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAUTOCOMPLETEPOPUP_IID)

  /* readonly attribute nsIAutoCompleteInput input; */
  NS_IMETHOD GetInput(nsIAutoCompleteInput * *aInput) = 0;

  /* readonly attribute AString overrideValue; */
  NS_IMETHOD GetOverrideValue(nsAString & aOverrideValue) = 0;

  /* attribute long selectedIndex; */
  NS_IMETHOD GetSelectedIndex(PRInt32 *aSelectedIndex) = 0;
  NS_IMETHOD SetSelectedIndex(PRInt32 aSelectedIndex) = 0;

  /* readonly attribute boolean popupOpen; */
  NS_IMETHOD GetPopupOpen(PRBool *aPopupOpen) = 0;

  /* void openPopup (in nsIAutoCompleteInput input, in long x, in long y, in long width); */
  NS_IMETHOD OpenPopup(nsIAutoCompleteInput *input, PRInt32 x, PRInt32 y, PRInt32 width) = 0;

  /* void closePopup (); */
  NS_IMETHOD ClosePopup(void) = 0;

  /* void invalidate (); */
  NS_IMETHOD Invalidate(void) = 0;

  /* void selectBy (in boolean reverse, in boolean page); */
  NS_IMETHOD SelectBy(PRBool reverse, PRBool page) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUTOCOMPLETEPOPUP \
  NS_IMETHOD GetInput(nsIAutoCompleteInput * *aInput); \
  NS_IMETHOD GetOverrideValue(nsAString & aOverrideValue); \
  NS_IMETHOD GetSelectedIndex(PRInt32 *aSelectedIndex); \
  NS_IMETHOD SetSelectedIndex(PRInt32 aSelectedIndex); \
  NS_IMETHOD GetPopupOpen(PRBool *aPopupOpen); \
  NS_IMETHOD OpenPopup(nsIAutoCompleteInput *input, PRInt32 x, PRInt32 y, PRInt32 width); \
  NS_IMETHOD ClosePopup(void); \
  NS_IMETHOD Invalidate(void); \
  NS_IMETHOD SelectBy(PRBool reverse, PRBool page); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUTOCOMPLETEPOPUP(_to) \
  NS_IMETHOD GetInput(nsIAutoCompleteInput * *aInput) { return _to GetInput(aInput); } \
  NS_IMETHOD GetOverrideValue(nsAString & aOverrideValue) { return _to GetOverrideValue(aOverrideValue); } \
  NS_IMETHOD GetSelectedIndex(PRInt32 *aSelectedIndex) { return _to GetSelectedIndex(aSelectedIndex); } \
  NS_IMETHOD SetSelectedIndex(PRInt32 aSelectedIndex) { return _to SetSelectedIndex(aSelectedIndex); } \
  NS_IMETHOD GetPopupOpen(PRBool *aPopupOpen) { return _to GetPopupOpen(aPopupOpen); } \
  NS_IMETHOD OpenPopup(nsIAutoCompleteInput *input, PRInt32 x, PRInt32 y, PRInt32 width) { return _to OpenPopup(input, x, y, width); } \
  NS_IMETHOD ClosePopup(void) { return _to ClosePopup(); } \
  NS_IMETHOD Invalidate(void) { return _to Invalidate(); } \
  NS_IMETHOD SelectBy(PRBool reverse, PRBool page) { return _to SelectBy(reverse, page); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUTOCOMPLETEPOPUP(_to) \
  NS_IMETHOD GetInput(nsIAutoCompleteInput * *aInput) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInput(aInput); } \
  NS_IMETHOD GetOverrideValue(nsAString & aOverrideValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOverrideValue(aOverrideValue); } \
  NS_IMETHOD GetSelectedIndex(PRInt32 *aSelectedIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSelectedIndex(aSelectedIndex); } \
  NS_IMETHOD SetSelectedIndex(PRInt32 aSelectedIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSelectedIndex(aSelectedIndex); } \
  NS_IMETHOD GetPopupOpen(PRBool *aPopupOpen) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPopupOpen(aPopupOpen); } \
  NS_IMETHOD OpenPopup(nsIAutoCompleteInput *input, PRInt32 x, PRInt32 y, PRInt32 width) { return !_to ? NS_ERROR_NULL_POINTER : _to->OpenPopup(input, x, y, width); } \
  NS_IMETHOD ClosePopup(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ClosePopup(); } \
  NS_IMETHOD Invalidate(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Invalidate(); } \
  NS_IMETHOD SelectBy(PRBool reverse, PRBool page) { return !_to ? NS_ERROR_NULL_POINTER : _to->SelectBy(reverse, page); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAutoCompletePopup : public nsIAutoCompletePopup
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAUTOCOMPLETEPOPUP

  nsAutoCompletePopup();

private:
  ~nsAutoCompletePopup();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAutoCompletePopup, nsIAutoCompletePopup)

nsAutoCompletePopup::nsAutoCompletePopup()
{
  /* member initializers and constructor code */
}

nsAutoCompletePopup::~nsAutoCompletePopup()
{
  /* destructor code */
}

/* readonly attribute nsIAutoCompleteInput input; */
NS_IMETHODIMP nsAutoCompletePopup::GetInput(nsIAutoCompleteInput * *aInput)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString overrideValue; */
NS_IMETHODIMP nsAutoCompletePopup::GetOverrideValue(nsAString & aOverrideValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute long selectedIndex; */
NS_IMETHODIMP nsAutoCompletePopup::GetSelectedIndex(PRInt32 *aSelectedIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompletePopup::SetSelectedIndex(PRInt32 aSelectedIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean popupOpen; */
NS_IMETHODIMP nsAutoCompletePopup::GetPopupOpen(PRBool *aPopupOpen)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void openPopup (in nsIAutoCompleteInput input, in long x, in long y, in long width); */
NS_IMETHODIMP nsAutoCompletePopup::OpenPopup(nsIAutoCompleteInput *input, PRInt32 x, PRInt32 y, PRInt32 width)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void closePopup (); */
NS_IMETHODIMP nsAutoCompletePopup::ClosePopup()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void invalidate (); */
NS_IMETHODIMP nsAutoCompletePopup::Invalidate()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void selectBy (in boolean reverse, in boolean page); */
NS_IMETHODIMP nsAutoCompletePopup::SelectBy(PRBool reverse, PRBool page)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIAutoCompletePopup_h__ */
