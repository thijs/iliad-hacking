/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/uriloader/exthandler/nsIHelperAppLauncherDialog.idl
 */

#ifndef __gen_nsIHelperAppLauncherDialog_h__
#define __gen_nsIHelperAppLauncherDialog_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIHelperAppLauncher; /* forward declaration */

class nsILocalFile; /* forward declaration */


/* starting interface:    nsIHelperAppLauncherDialog */
#define NS_IHELPERAPPLAUNCHERDIALOG_IID_STR "64355793-988d-40a5-ba8e-fcde78cac631"

#define NS_IHELPERAPPLAUNCHERDIALOG_IID \
  {0x64355793, 0x988d, 0x40a5, \
    { 0xba, 0x8e, 0xfc, 0xde, 0x78, 0xca, 0xc6, 0x31 }}

/**
 * This interface is used to display a confirmatino dialog before
 * launching a "helper app" to handle content not handled by
 * Mozilla.
 *
 * Usage:  Clients (of which there is one: the nsIExternalHelperAppService
 * implementation in mozilla/uriloader/exthandler) create an instance of
 * this interface (using the contract ID) and then call the show() method.
 *
 * The dialog is shown non-modally.  The implementation of the dialog
 * will access methods of the nsIHelperAppLauncher passed in to show()
 * in order to cause a "save to disk" or "open using" action.
 */
class NS_NO_VTABLE nsIHelperAppLauncherDialog : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IHELPERAPPLAUNCHERDIALOG_IID)

  /**
   * This request is passed to the helper app dialog because Gecko can not
   * handle content of this type.
   */
  enum { REASON_CANTHANDLE = 0U };

  /**
   * The server requested external handling.
   */
  enum { REASON_SERVERREQUEST = 1U };

  /**
   * Gecko detected that the type sent by the server (e.g. text/plain) does
   * not match the actual type.
   */
  enum { REASON_TYPESNIFFED = 2U };

  /* void show (in nsIHelperAppLauncher aLauncher, in nsISupports aContext, in unsigned long aReason); */
  NS_IMETHOD Show(nsIHelperAppLauncher *aLauncher, nsISupports *aContext, PRUint32 aReason) = 0;

  /* nsILocalFile promptForSaveToFile (in nsIHelperAppLauncher aLauncher, in nsISupports aWindowContext, in wstring aDefaultFile, in wstring aSuggestedFileExtension); */
  NS_IMETHOD PromptForSaveToFile(nsIHelperAppLauncher *aLauncher, nsISupports *aWindowContext, const PRUnichar *aDefaultFile, const PRUnichar *aSuggestedFileExtension, nsILocalFile **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHELPERAPPLAUNCHERDIALOG \
  NS_IMETHOD Show(nsIHelperAppLauncher *aLauncher, nsISupports *aContext, PRUint32 aReason); \
  NS_IMETHOD PromptForSaveToFile(nsIHelperAppLauncher *aLauncher, nsISupports *aWindowContext, const PRUnichar *aDefaultFile, const PRUnichar *aSuggestedFileExtension, nsILocalFile **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHELPERAPPLAUNCHERDIALOG(_to) \
  NS_IMETHOD Show(nsIHelperAppLauncher *aLauncher, nsISupports *aContext, PRUint32 aReason) { return _to Show(aLauncher, aContext, aReason); } \
  NS_IMETHOD PromptForSaveToFile(nsIHelperAppLauncher *aLauncher, nsISupports *aWindowContext, const PRUnichar *aDefaultFile, const PRUnichar *aSuggestedFileExtension, nsILocalFile **_retval) { return _to PromptForSaveToFile(aLauncher, aWindowContext, aDefaultFile, aSuggestedFileExtension, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHELPERAPPLAUNCHERDIALOG(_to) \
  NS_IMETHOD Show(nsIHelperAppLauncher *aLauncher, nsISupports *aContext, PRUint32 aReason) { return !_to ? NS_ERROR_NULL_POINTER : _to->Show(aLauncher, aContext, aReason); } \
  NS_IMETHOD PromptForSaveToFile(nsIHelperAppLauncher *aLauncher, nsISupports *aWindowContext, const PRUnichar *aDefaultFile, const PRUnichar *aSuggestedFileExtension, nsILocalFile **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->PromptForSaveToFile(aLauncher, aWindowContext, aDefaultFile, aSuggestedFileExtension, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHelperAppLauncherDialog : public nsIHelperAppLauncherDialog
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHELPERAPPLAUNCHERDIALOG

  nsHelperAppLauncherDialog();

private:
  ~nsHelperAppLauncherDialog();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHelperAppLauncherDialog, nsIHelperAppLauncherDialog)

nsHelperAppLauncherDialog::nsHelperAppLauncherDialog()
{
  /* member initializers and constructor code */
}

nsHelperAppLauncherDialog::~nsHelperAppLauncherDialog()
{
  /* destructor code */
}

/* void show (in nsIHelperAppLauncher aLauncher, in nsISupports aContext, in unsigned long aReason); */
NS_IMETHODIMP nsHelperAppLauncherDialog::Show(nsIHelperAppLauncher *aLauncher, nsISupports *aContext, PRUint32 aReason)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsILocalFile promptForSaveToFile (in nsIHelperAppLauncher aLauncher, in nsISupports aWindowContext, in wstring aDefaultFile, in wstring aSuggestedFileExtension); */
NS_IMETHODIMP nsHelperAppLauncherDialog::PromptForSaveToFile(nsIHelperAppLauncher *aLauncher, nsISupports *aWindowContext, const PRUnichar *aDefaultFile, const PRUnichar *aSuggestedFileExtension, nsILocalFile **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_IHELPERAPPLAUNCHERDLG_CONTRACTID    "@mozilla.org/helperapplauncherdialog;1"
#define NS_IHELPERAPPLAUNCHERDLG_CLASSNAME "Mozilla Helper App Launcher Confirmation Dialog"

#endif /* __gen_nsIHelperAppLauncherDialog_h__ */
