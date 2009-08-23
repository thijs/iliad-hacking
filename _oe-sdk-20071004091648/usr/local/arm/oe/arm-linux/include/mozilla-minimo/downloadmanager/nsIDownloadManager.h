/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/download-manager/public/nsIDownloadManager.idl
 */

#ifndef __gen_nsIDownloadManager_h__
#define __gen_nsIDownloadManager_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMWindow; /* forward declaration */

class nsIURI; /* forward declaration */

class nsILocalFile; /* forward declaration */

class nsIDownload; /* forward declaration */

class nsICancelable; /* forward declaration */

class nsIMIMEInfo; /* forward declaration */


/* starting interface:    nsIDownloadManager */
#define NS_IDOWNLOADMANAGER_IID_STR "9cdfcea3-fbe4-4ba1-a0fd-fe273097ddfa"

#define NS_IDOWNLOADMANAGER_IID \
  {0x9cdfcea3, 0xfbe4, 0x4ba1, \
    { 0xa0, 0xfd, 0xfe, 0x27, 0x30, 0x97, 0xdd, 0xfa }}

class NS_NO_VTABLE nsIDownloadManager : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IDOWNLOADMANAGER_IID)

  /**
  * Creates an nsIDownload and adds it to be managed by the download manager.
  *
  * @param aSource The source URI of the transfer. Must not be null.
  *
  * @param aTarget The target URI of the transfer. Must not be null.
  *
  * @param aDisplayName The user-readable description of the transfer.
  *                     Can be empty.
  *
  * @param aMIMEInfo The MIME info associated with the target,
  *                  including MIME type and helper app when appropriate.
  *                  This parameter is optional.
  *
  * @param startTime Time when the download started (ie, when the first
  *                  response from the server was received)
  * @param aTempFile The location of a temporary file; i.e. a file in which
  *                  the received data will be stored, but which is not
  *                  equal to the target file. (will be moved to the real
  *                  target by the caller, when the download is finished)
  *                  May be null.
  *
  * @param aCancelable An object that can be used to abort the download.
  *                    Must not be null.

  * @return The newly created download item with the passed-in properties.
  */
  /* nsIDownload addDownload (in nsIURI aSource, in nsIURI aTarget, in AString aDisplayName, in nsIMIMEInfo aMIMEInfo, in PRTime startTime, in nsILocalFile aTempFile, in nsICancelable aCancelable); */
  NS_IMETHOD AddDownload(nsIURI *aSource, nsIURI *aTarget, const nsAString & aDisplayName, nsIMIMEInfo *aMIMEInfo, PRTime startTime, nsILocalFile *aTempFile, nsICancelable *aCancelable, nsIDownload **_retval) = 0;

  /**
  *  Retrieves an in-progress download managed by the download manager.
  *
  *  @param aTargetPath           A UTF8-encoded path to the target file.
  *
  *  @return The download with the specified path.
  */
  /* nsIDownload getDownload (in AUTF8String aTargetPath); */
  NS_IMETHOD GetDownload(const nsACString & aTargetPath, nsIDownload **_retval) = 0;

  /**
  * Cancels the download with the specified target path if it's
  * currently in progress.  If a "persist" was specified for the download,
  * nsIWebBrowserPersist::CancelSave will be called.  If an observer was set
  * on the nsIDownload, it will be notified with the "oncancel" topic.  Clients
  * that don't provide a "persist" must listen for this topic and cancel the
  * download.
  *
  * @param aTargetPath           The target path of the download to
  *                              be cancelled.
  */
  /* void cancelDownload (in AUTF8String aTargetPath); */
  NS_IMETHOD CancelDownload(const nsACString & aTargetPath) = 0;

  /**
   * Pause a download to allow later resumal. This may or may not close
   * the network connection. In either case, new data won't be received.
   *
   * @param aDownload Download to pause, as returned by getDownload or
   *                  addDownload.
   *
   * Trying to pause a download that is already paused will throw
   * NS_ERROR_NOT_AVAILABLE.
   * NS_ERROR_UNEXPECTED may be thrown to indicate an internal error.
   * Any exception defined by nsIRequest::Suspend may be thrown.
   * No other exceptions will be thrown.
   */
  /* void pauseDownload (in nsIDownload aDownload); */
  NS_IMETHOD PauseDownload(nsIDownload *aDownload) = 0;

  /**
   * Resume a previously paused download.
   * @param aTargetPath Target path of the download to be resumed.
   *
   * @throws NS_ERROR_NOT_RESUMABLE If resuming is not supported for this
   *                                entity, or if the entity has changed since
   *                                the download was paused
   * @throws NS_ERROR_NOT_AVAILABLE If no such download exists, or if the
   *                                download was not paused, or if the
   *                                download is finished already.
   * In addition, this may throw NS_ERROR_OUT_OF_MEMORY, or any error defined by
   * nsIResumableChannel::setEntityID, nsIResumableChannel::asyncOpenAt,
   * nsIRequest::Resume, or nsIIOService::newChannel.
   * Also, NS_ERROR_UNEXPECTED may be thrown to indicate an internal error.
   */
  /* void resumeDownload (in AUTF8String aTargetPath); */
  NS_IMETHOD ResumeDownload(const nsACString & aTargetPath) = 0;

  /**
  * Removes the download with the specified target path if it's not
  * currently in progress.  Whereas cancelDownload simply cancels the transfer
  * but retains information about it, removeDownload removes all knowledge of it.
  *
  * @param aTargetPath           The target path of the download to
  *                              be removed.
  */
  /* void removeDownload (in AUTF8String aTargetPath); */
  NS_IMETHOD RemoveDownload(const nsACString & aTargetPath) = 0;

  /**
  * Opens the Download Manager front end.
  *
  * @param aParent The parent, or opener, of the front end (optional).
  * @param aDownload A download to pass to the manager window.  Useful
  *                  if, for example, you want the window to select a
  *                  certain download (optional).
  */
  /* void open (in nsIDOMWindow aParent, in nsIDownload aDownload); */
  NS_IMETHOD Open(nsIDOMWindow *aParent, nsIDownload *aDownload) = 0;

  /**
  * Opens an individual progress dialog displaying progress for the download.
  *
  * @param aDownload The download object to display progress for, as returned
  *                  by getDownload or addDownload.
  *
  * @param aParent The parent, or opener, of the front end (optional).
  *
  * @param aCancelDownloadOnClose       Whether closing the dialog
  *                                     should cancel the download.
  */
  /* void openProgressDialogFor (in nsIDownload aDownload, in nsIDOMWindow aParent, in boolean aCancelDownloadOnClose); */
  NS_IMETHOD OpenProgressDialogFor(nsIDownload *aDownload, nsIDOMWindow *aParent, PRBool aCancelDownloadOnClose) = 0;

  /**
  * Called when the download manager front end is closed.  Useful for
  * third party managers to let us know when they've closed.
  */
  /* void onClose (); */
  NS_IMETHOD OnClose(void) = 0;

  /**
  * Indicate that a batch update (e.g. mass removal) is about to start.
  */
  /* void startBatchUpdate (); */
  NS_IMETHOD StartBatchUpdate(void) = 0;

  /**
  * Indicate that a batch update is ending.
  */
  /* void endBatchUpdate (); */
  NS_IMETHOD EndBatchUpdate(void) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOWNLOADMANAGER \
  NS_IMETHOD AddDownload(nsIURI *aSource, nsIURI *aTarget, const nsAString & aDisplayName, nsIMIMEInfo *aMIMEInfo, PRTime startTime, nsILocalFile *aTempFile, nsICancelable *aCancelable, nsIDownload **_retval); \
  NS_IMETHOD GetDownload(const nsACString & aTargetPath, nsIDownload **_retval); \
  NS_IMETHOD CancelDownload(const nsACString & aTargetPath); \
  NS_IMETHOD PauseDownload(nsIDownload *aDownload); \
  NS_IMETHOD ResumeDownload(const nsACString & aTargetPath); \
  NS_IMETHOD RemoveDownload(const nsACString & aTargetPath); \
  NS_IMETHOD Open(nsIDOMWindow *aParent, nsIDownload *aDownload); \
  NS_IMETHOD OpenProgressDialogFor(nsIDownload *aDownload, nsIDOMWindow *aParent, PRBool aCancelDownloadOnClose); \
  NS_IMETHOD OnClose(void); \
  NS_IMETHOD StartBatchUpdate(void); \
  NS_IMETHOD EndBatchUpdate(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOWNLOADMANAGER(_to) \
  NS_IMETHOD AddDownload(nsIURI *aSource, nsIURI *aTarget, const nsAString & aDisplayName, nsIMIMEInfo *aMIMEInfo, PRTime startTime, nsILocalFile *aTempFile, nsICancelable *aCancelable, nsIDownload **_retval) { return _to AddDownload(aSource, aTarget, aDisplayName, aMIMEInfo, startTime, aTempFile, aCancelable, _retval); } \
  NS_IMETHOD GetDownload(const nsACString & aTargetPath, nsIDownload **_retval) { return _to GetDownload(aTargetPath, _retval); } \
  NS_IMETHOD CancelDownload(const nsACString & aTargetPath) { return _to CancelDownload(aTargetPath); } \
  NS_IMETHOD PauseDownload(nsIDownload *aDownload) { return _to PauseDownload(aDownload); } \
  NS_IMETHOD ResumeDownload(const nsACString & aTargetPath) { return _to ResumeDownload(aTargetPath); } \
  NS_IMETHOD RemoveDownload(const nsACString & aTargetPath) { return _to RemoveDownload(aTargetPath); } \
  NS_IMETHOD Open(nsIDOMWindow *aParent, nsIDownload *aDownload) { return _to Open(aParent, aDownload); } \
  NS_IMETHOD OpenProgressDialogFor(nsIDownload *aDownload, nsIDOMWindow *aParent, PRBool aCancelDownloadOnClose) { return _to OpenProgressDialogFor(aDownload, aParent, aCancelDownloadOnClose); } \
  NS_IMETHOD OnClose(void) { return _to OnClose(); } \
  NS_IMETHOD StartBatchUpdate(void) { return _to StartBatchUpdate(); } \
  NS_IMETHOD EndBatchUpdate(void) { return _to EndBatchUpdate(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOWNLOADMANAGER(_to) \
  NS_IMETHOD AddDownload(nsIURI *aSource, nsIURI *aTarget, const nsAString & aDisplayName, nsIMIMEInfo *aMIMEInfo, PRTime startTime, nsILocalFile *aTempFile, nsICancelable *aCancelable, nsIDownload **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddDownload(aSource, aTarget, aDisplayName, aMIMEInfo, startTime, aTempFile, aCancelable, _retval); } \
  NS_IMETHOD GetDownload(const nsACString & aTargetPath, nsIDownload **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDownload(aTargetPath, _retval); } \
  NS_IMETHOD CancelDownload(const nsACString & aTargetPath) { return !_to ? NS_ERROR_NULL_POINTER : _to->CancelDownload(aTargetPath); } \
  NS_IMETHOD PauseDownload(nsIDownload *aDownload) { return !_to ? NS_ERROR_NULL_POINTER : _to->PauseDownload(aDownload); } \
  NS_IMETHOD ResumeDownload(const nsACString & aTargetPath) { return !_to ? NS_ERROR_NULL_POINTER : _to->ResumeDownload(aTargetPath); } \
  NS_IMETHOD RemoveDownload(const nsACString & aTargetPath) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveDownload(aTargetPath); } \
  NS_IMETHOD Open(nsIDOMWindow *aParent, nsIDownload *aDownload) { return !_to ? NS_ERROR_NULL_POINTER : _to->Open(aParent, aDownload); } \
  NS_IMETHOD OpenProgressDialogFor(nsIDownload *aDownload, nsIDOMWindow *aParent, PRBool aCancelDownloadOnClose) { return !_to ? NS_ERROR_NULL_POINTER : _to->OpenProgressDialogFor(aDownload, aParent, aCancelDownloadOnClose); } \
  NS_IMETHOD OnClose(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnClose(); } \
  NS_IMETHOD StartBatchUpdate(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->StartBatchUpdate(); } \
  NS_IMETHOD EndBatchUpdate(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->EndBatchUpdate(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDownloadManager : public nsIDownloadManager
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOWNLOADMANAGER

  nsDownloadManager();

private:
  ~nsDownloadManager();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDownloadManager, nsIDownloadManager)

nsDownloadManager::nsDownloadManager()
{
  /* member initializers and constructor code */
}

nsDownloadManager::~nsDownloadManager()
{
  /* destructor code */
}

/* nsIDownload addDownload (in nsIURI aSource, in nsIURI aTarget, in AString aDisplayName, in nsIMIMEInfo aMIMEInfo, in PRTime startTime, in nsILocalFile aTempFile, in nsICancelable aCancelable); */
NS_IMETHODIMP nsDownloadManager::AddDownload(nsIURI *aSource, nsIURI *aTarget, const nsAString & aDisplayName, nsIMIMEInfo *aMIMEInfo, PRTime startTime, nsILocalFile *aTempFile, nsICancelable *aCancelable, nsIDownload **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDownload getDownload (in AUTF8String aTargetPath); */
NS_IMETHODIMP nsDownloadManager::GetDownload(const nsACString & aTargetPath, nsIDownload **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void cancelDownload (in AUTF8String aTargetPath); */
NS_IMETHODIMP nsDownloadManager::CancelDownload(const nsACString & aTargetPath)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void pauseDownload (in nsIDownload aDownload); */
NS_IMETHODIMP nsDownloadManager::PauseDownload(nsIDownload *aDownload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void resumeDownload (in AUTF8String aTargetPath); */
NS_IMETHODIMP nsDownloadManager::ResumeDownload(const nsACString & aTargetPath)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeDownload (in AUTF8String aTargetPath); */
NS_IMETHODIMP nsDownloadManager::RemoveDownload(const nsACString & aTargetPath)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void open (in nsIDOMWindow aParent, in nsIDownload aDownload); */
NS_IMETHODIMP nsDownloadManager::Open(nsIDOMWindow *aParent, nsIDownload *aDownload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void openProgressDialogFor (in nsIDownload aDownload, in nsIDOMWindow aParent, in boolean aCancelDownloadOnClose); */
NS_IMETHODIMP nsDownloadManager::OpenProgressDialogFor(nsIDownload *aDownload, nsIDOMWindow *aParent, PRBool aCancelDownloadOnClose)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onClose (); */
NS_IMETHODIMP nsDownloadManager::OnClose()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void startBatchUpdate (); */
NS_IMETHODIMP nsDownloadManager::StartBatchUpdate()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void endBatchUpdate (); */
NS_IMETHODIMP nsDownloadManager::EndBatchUpdate()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_DOWNLOADMANAGER_CONTRACTID "@mozilla.org/download-manager;1"
#define NS_DOWNLOADMANAGER_CLASSNAME "Mozilla Download Manager"
// {EDB0490E-1DD1-11B2-83B8-DBF8D85906A6}
#define NS_DOWNLOADMANAGER_CID \
    { 0xedb0490e, 0x1dd1, 0x11b2, { 0x83, 0xb8, 0xdb, 0xf8, 0xd8, 0x59, 0x06, 0xa6 } }

#endif /* __gen_nsIDownloadManager_h__ */
