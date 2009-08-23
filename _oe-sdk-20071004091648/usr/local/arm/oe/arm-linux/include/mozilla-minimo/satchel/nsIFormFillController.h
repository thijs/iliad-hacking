/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/satchel/public/nsIFormFillController.idl
 */

#ifndef __gen_nsIFormFillController_h__
#define __gen_nsIFormFillController_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDocShell; /* forward declaration */

class nsIAutoCompletePopup; /* forward declaration */


/* starting interface:    nsIFormFillController */
#define NS_IFORMFILLCONTROLLER_IID_STR "872f07f3-ed11-47c6-b7cf-246db53379fb"

#define NS_IFORMFILLCONTROLLER_IID \
  {0x872f07f3, 0xed11, 0x47c6, \
    { 0xb7, 0xcf, 0x24, 0x6d, 0xb5, 0x33, 0x79, 0xfb }}

class NS_NO_VTABLE nsIFormFillController : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IFORMFILLCONTROLLER_IID)

  /* void attachToBrowser (in nsIDocShell docShell, in nsIAutoCompletePopup popup); */
  NS_IMETHOD AttachToBrowser(nsIDocShell *docShell, nsIAutoCompletePopup *popup) = 0;

  /* void detachFromBrowser (in nsIDocShell docShell); */
  NS_IMETHOD DetachFromBrowser(nsIDocShell *docShell) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIFORMFILLCONTROLLER \
  NS_IMETHOD AttachToBrowser(nsIDocShell *docShell, nsIAutoCompletePopup *popup); \
  NS_IMETHOD DetachFromBrowser(nsIDocShell *docShell); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIFORMFILLCONTROLLER(_to) \
  NS_IMETHOD AttachToBrowser(nsIDocShell *docShell, nsIAutoCompletePopup *popup) { return _to AttachToBrowser(docShell, popup); } \
  NS_IMETHOD DetachFromBrowser(nsIDocShell *docShell) { return _to DetachFromBrowser(docShell); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIFORMFILLCONTROLLER(_to) \
  NS_IMETHOD AttachToBrowser(nsIDocShell *docShell, nsIAutoCompletePopup *popup) { return !_to ? NS_ERROR_NULL_POINTER : _to->AttachToBrowser(docShell, popup); } \
  NS_IMETHOD DetachFromBrowser(nsIDocShell *docShell) { return !_to ? NS_ERROR_NULL_POINTER : _to->DetachFromBrowser(docShell); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsFormFillController : public nsIFormFillController
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIFORMFILLCONTROLLER

  nsFormFillController();

private:
  ~nsFormFillController();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsFormFillController, nsIFormFillController)

nsFormFillController::nsFormFillController()
{
  /* member initializers and constructor code */
}

nsFormFillController::~nsFormFillController()
{
  /* destructor code */
}

/* void attachToBrowser (in nsIDocShell docShell, in nsIAutoCompletePopup popup); */
NS_IMETHODIMP nsFormFillController::AttachToBrowser(nsIDocShell *docShell, nsIAutoCompletePopup *popup)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void detachFromBrowser (in nsIDocShell docShell); */
NS_IMETHODIMP nsFormFillController::DetachFromBrowser(nsIDocShell *docShell)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIFormFillController_h__ */
