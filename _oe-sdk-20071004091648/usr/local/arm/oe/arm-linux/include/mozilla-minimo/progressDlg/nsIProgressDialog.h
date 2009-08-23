/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/embedding/components/ui/progressDlg/nsIProgressDialog.idl
 */

#ifndef __gen_nsIProgressDialog_h__
#define __gen_nsIProgressDialog_h__


#ifndef __gen_nsIDownload_h__
#include "nsIDownload.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMWindow; /* forward declaration */

class nsIObserver; /* forward declaration */


/* starting interface:    nsIProgressDialog */
#define NS_IPROGRESSDIALOG_IID_STR "20e790a2-76c6-462d-851a-22ab6cbbe48b"

#define NS_IPROGRESSDIALOG_IID \
  {0x20e790a2, 0x76c6, 0x462d, \
    { 0x85, 0x1a, 0x22, 0xab, 0x6c, 0xbb, 0xe4, 0x8b }}

class NS_NO_VTABLE nsIProgressDialog : public nsIDownload {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IPROGRESSDIALOG_IID)

  /**
    * Open the dialog
    *
    * @param aParent  Parent window; optional (if null, then
    *                 a top-level window is created)
    */
  /* void open (in nsIDOMWindow aParent); */
  NS_IMETHOD Open(nsIDOMWindow *aParent) = 0;

  /**
    * Whether the download should be cancelled when the progress
    * dialog is closed using the standard OS close box.  This is
    * useful for showing the progress dialog as an information
    * window, which is what download manager does.
    */
  /* attribute PRBool cancelDownloadOnClose; */
  NS_IMETHOD GetCancelDownloadOnClose(PRBool *aCancelDownloadOnClose) = 0;
  NS_IMETHOD SetCancelDownloadOnClose(PRBool aCancelDownloadOnClose) = 0;

  /**
    * Observer for this dialog. If set, receives the following topics:
    *   oncancel - observer should cancel the transfer
    *   onpause  - observer should suspend the transfer
    *   onresume - observer should resume the suspended transfer
    * For each of those, the subject will be the nsIProgressDialog.
    */
  /* attribute nsIObserver observer; */
  NS_IMETHOD GetObserver(nsIObserver * *aObserver) = 0;
  NS_IMETHOD SetObserver(nsIObserver * aObserver) = 0;

  /**
   * The dialog object itself.  This might be null if the dialog isn't
   * open yet, or has been closed.
   */
  /* attribute nsIDOMWindow dialog; */
  NS_IMETHOD GetDialog(nsIDOMWindow * *aDialog) = 0;
  NS_IMETHOD SetDialog(nsIDOMWindow * aDialog) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPROGRESSDIALOG \
  NS_IMETHOD Open(nsIDOMWindow *aParent); \
  NS_IMETHOD GetCancelDownloadOnClose(PRBool *aCancelDownloadOnClose); \
  NS_IMETHOD SetCancelDownloadOnClose(PRBool aCancelDownloadOnClose); \
  NS_IMETHOD GetObserver(nsIObserver * *aObserver); \
  NS_IMETHOD SetObserver(nsIObserver * aObserver); \
  NS_IMETHOD GetDialog(nsIDOMWindow * *aDialog); \
  NS_IMETHOD SetDialog(nsIDOMWindow * aDialog); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPROGRESSDIALOG(_to) \
  NS_IMETHOD Open(nsIDOMWindow *aParent) { return _to Open(aParent); } \
  NS_IMETHOD GetCancelDownloadOnClose(PRBool *aCancelDownloadOnClose) { return _to GetCancelDownloadOnClose(aCancelDownloadOnClose); } \
  NS_IMETHOD SetCancelDownloadOnClose(PRBool aCancelDownloadOnClose) { return _to SetCancelDownloadOnClose(aCancelDownloadOnClose); } \
  NS_IMETHOD GetObserver(nsIObserver * *aObserver) { return _to GetObserver(aObserver); } \
  NS_IMETHOD SetObserver(nsIObserver * aObserver) { return _to SetObserver(aObserver); } \
  NS_IMETHOD GetDialog(nsIDOMWindow * *aDialog) { return _to GetDialog(aDialog); } \
  NS_IMETHOD SetDialog(nsIDOMWindow * aDialog) { return _to SetDialog(aDialog); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPROGRESSDIALOG(_to) \
  NS_IMETHOD Open(nsIDOMWindow *aParent) { return !_to ? NS_ERROR_NULL_POINTER : _to->Open(aParent); } \
  NS_IMETHOD GetCancelDownloadOnClose(PRBool *aCancelDownloadOnClose) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCancelDownloadOnClose(aCancelDownloadOnClose); } \
  NS_IMETHOD SetCancelDownloadOnClose(PRBool aCancelDownloadOnClose) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCancelDownloadOnClose(aCancelDownloadOnClose); } \
  NS_IMETHOD GetObserver(nsIObserver * *aObserver) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetObserver(aObserver); } \
  NS_IMETHOD SetObserver(nsIObserver * aObserver) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetObserver(aObserver); } \
  NS_IMETHOD GetDialog(nsIDOMWindow * *aDialog) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDialog(aDialog); } \
  NS_IMETHOD SetDialog(nsIDOMWindow * aDialog) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDialog(aDialog); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsProgressDialog : public nsIProgressDialog
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPROGRESSDIALOG

  nsProgressDialog();

private:
  ~nsProgressDialog();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsProgressDialog, nsIProgressDialog)

nsProgressDialog::nsProgressDialog()
{
  /* member initializers and constructor code */
}

nsProgressDialog::~nsProgressDialog()
{
  /* destructor code */
}

/* void open (in nsIDOMWindow aParent); */
NS_IMETHODIMP nsProgressDialog::Open(nsIDOMWindow *aParent)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute PRBool cancelDownloadOnClose; */
NS_IMETHODIMP nsProgressDialog::GetCancelDownloadOnClose(PRBool *aCancelDownloadOnClose)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsProgressDialog::SetCancelDownloadOnClose(PRBool aCancelDownloadOnClose)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIObserver observer; */
NS_IMETHODIMP nsProgressDialog::GetObserver(nsIObserver * *aObserver)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsProgressDialog::SetObserver(nsIObserver * aObserver)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIDOMWindow dialog; */
NS_IMETHODIMP nsProgressDialog::GetDialog(nsIDOMWindow * *aDialog)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsProgressDialog::SetDialog(nsIDOMWindow * aDialog)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIProgressDialog_h__ */
