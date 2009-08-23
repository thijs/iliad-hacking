/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/download-manager/public/nsIDownloadProgressListener.idl
 */

#ifndef __gen_nsIDownloadProgressListener_h__
#define __gen_nsIDownloadProgressListener_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIWebProgress; /* forward declaration */

class nsIRequest; /* forward declaration */

class nsIURI; /* forward declaration */

class nsIDownload; /* forward declaration */

class nsIDOMDocument; /* forward declaration */


/* starting interface:    nsIDownloadProgressListener */
#define NS_IDOWNLOADPROGRESSLISTENER_IID_STR "c7e22278-53a7-49d9-999f-0e7861738b50"

#define NS_IDOWNLOADPROGRESSLISTENER_IID \
  {0xc7e22278, 0x53a7, 0x49d9, \
    { 0x99, 0x9f, 0x0e, 0x78, 0x61, 0x73, 0x8b, 0x50 }}

class NS_NO_VTABLE nsIDownloadProgressListener : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IDOWNLOADPROGRESSLISTENER_IID)

  /**
  * document
  * The document of the download manager frontend.
  */
  /* attribute nsIDOMDocument document; */
  NS_IMETHOD GetDocument(nsIDOMDocument * *aDocument) = 0;
  NS_IMETHOD SetDocument(nsIDOMDocument * aDocument) = 0;

  /* void onStateChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in unsigned long aStateFlags, in nsresult aStatus, in nsIDownload aDownload); */
  NS_IMETHOD OnStateChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aStateFlags, nsresult aStatus, nsIDownload *aDownload) = 0;

  /* void onProgressChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in long long aCurSelfProgress, in long long aMaxSelfProgress, in long long aCurTotalProgress, in long long aMaxTotalProgress, in nsIDownload aDownload); */
  NS_IMETHOD OnProgressChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRInt64 aCurSelfProgress, PRInt64 aMaxSelfProgress, PRInt64 aCurTotalProgress, PRInt64 aMaxTotalProgress, nsIDownload *aDownload) = 0;

  /* void onStatusChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in nsresult aStatus, in wstring aMessage, in nsIDownload aDownload); */
  NS_IMETHOD OnStatusChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsresult aStatus, const PRUnichar *aMessage, nsIDownload *aDownload) = 0;

  /* void onLocationChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in nsIURI aLocation, in nsIDownload aDownload); */
  NS_IMETHOD OnLocationChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsIURI *aLocation, nsIDownload *aDownload) = 0;

  /* void onSecurityChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in unsigned long aState, in nsIDownload aDownload); */
  NS_IMETHOD OnSecurityChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aState, nsIDownload *aDownload) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDOWNLOADPROGRESSLISTENER \
  NS_IMETHOD GetDocument(nsIDOMDocument * *aDocument); \
  NS_IMETHOD SetDocument(nsIDOMDocument * aDocument); \
  NS_IMETHOD OnStateChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aStateFlags, nsresult aStatus, nsIDownload *aDownload); \
  NS_IMETHOD OnProgressChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRInt64 aCurSelfProgress, PRInt64 aMaxSelfProgress, PRInt64 aCurTotalProgress, PRInt64 aMaxTotalProgress, nsIDownload *aDownload); \
  NS_IMETHOD OnStatusChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsresult aStatus, const PRUnichar *aMessage, nsIDownload *aDownload); \
  NS_IMETHOD OnLocationChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsIURI *aLocation, nsIDownload *aDownload); \
  NS_IMETHOD OnSecurityChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aState, nsIDownload *aDownload); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDOWNLOADPROGRESSLISTENER(_to) \
  NS_IMETHOD GetDocument(nsIDOMDocument * *aDocument) { return _to GetDocument(aDocument); } \
  NS_IMETHOD SetDocument(nsIDOMDocument * aDocument) { return _to SetDocument(aDocument); } \
  NS_IMETHOD OnStateChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aStateFlags, nsresult aStatus, nsIDownload *aDownload) { return _to OnStateChange(aWebProgress, aRequest, aStateFlags, aStatus, aDownload); } \
  NS_IMETHOD OnProgressChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRInt64 aCurSelfProgress, PRInt64 aMaxSelfProgress, PRInt64 aCurTotalProgress, PRInt64 aMaxTotalProgress, nsIDownload *aDownload) { return _to OnProgressChange(aWebProgress, aRequest, aCurSelfProgress, aMaxSelfProgress, aCurTotalProgress, aMaxTotalProgress, aDownload); } \
  NS_IMETHOD OnStatusChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsresult aStatus, const PRUnichar *aMessage, nsIDownload *aDownload) { return _to OnStatusChange(aWebProgress, aRequest, aStatus, aMessage, aDownload); } \
  NS_IMETHOD OnLocationChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsIURI *aLocation, nsIDownload *aDownload) { return _to OnLocationChange(aWebProgress, aRequest, aLocation, aDownload); } \
  NS_IMETHOD OnSecurityChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aState, nsIDownload *aDownload) { return _to OnSecurityChange(aWebProgress, aRequest, aState, aDownload); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDOWNLOADPROGRESSLISTENER(_to) \
  NS_IMETHOD GetDocument(nsIDOMDocument * *aDocument) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDocument(aDocument); } \
  NS_IMETHOD SetDocument(nsIDOMDocument * aDocument) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDocument(aDocument); } \
  NS_IMETHOD OnStateChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aStateFlags, nsresult aStatus, nsIDownload *aDownload) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStateChange(aWebProgress, aRequest, aStateFlags, aStatus, aDownload); } \
  NS_IMETHOD OnProgressChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRInt64 aCurSelfProgress, PRInt64 aMaxSelfProgress, PRInt64 aCurTotalProgress, PRInt64 aMaxTotalProgress, nsIDownload *aDownload) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnProgressChange(aWebProgress, aRequest, aCurSelfProgress, aMaxSelfProgress, aCurTotalProgress, aMaxTotalProgress, aDownload); } \
  NS_IMETHOD OnStatusChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsresult aStatus, const PRUnichar *aMessage, nsIDownload *aDownload) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStatusChange(aWebProgress, aRequest, aStatus, aMessage, aDownload); } \
  NS_IMETHOD OnLocationChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsIURI *aLocation, nsIDownload *aDownload) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnLocationChange(aWebProgress, aRequest, aLocation, aDownload); } \
  NS_IMETHOD OnSecurityChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aState, nsIDownload *aDownload) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnSecurityChange(aWebProgress, aRequest, aState, aDownload); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDownloadProgressListener : public nsIDownloadProgressListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOWNLOADPROGRESSLISTENER

  nsDownloadProgressListener();

private:
  ~nsDownloadProgressListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDownloadProgressListener, nsIDownloadProgressListener)

nsDownloadProgressListener::nsDownloadProgressListener()
{
  /* member initializers and constructor code */
}

nsDownloadProgressListener::~nsDownloadProgressListener()
{
  /* destructor code */
}

/* attribute nsIDOMDocument document; */
NS_IMETHODIMP nsDownloadProgressListener::GetDocument(nsIDOMDocument * *aDocument)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsDownloadProgressListener::SetDocument(nsIDOMDocument * aDocument)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onStateChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in unsigned long aStateFlags, in nsresult aStatus, in nsIDownload aDownload); */
NS_IMETHODIMP nsDownloadProgressListener::OnStateChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aStateFlags, nsresult aStatus, nsIDownload *aDownload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onProgressChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in long long aCurSelfProgress, in long long aMaxSelfProgress, in long long aCurTotalProgress, in long long aMaxTotalProgress, in nsIDownload aDownload); */
NS_IMETHODIMP nsDownloadProgressListener::OnProgressChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRInt64 aCurSelfProgress, PRInt64 aMaxSelfProgress, PRInt64 aCurTotalProgress, PRInt64 aMaxTotalProgress, nsIDownload *aDownload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onStatusChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in nsresult aStatus, in wstring aMessage, in nsIDownload aDownload); */
NS_IMETHODIMP nsDownloadProgressListener::OnStatusChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsresult aStatus, const PRUnichar *aMessage, nsIDownload *aDownload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onLocationChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in nsIURI aLocation, in nsIDownload aDownload); */
NS_IMETHODIMP nsDownloadProgressListener::OnLocationChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsIURI *aLocation, nsIDownload *aDownload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onSecurityChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in unsigned long aState, in nsIDownload aDownload); */
NS_IMETHODIMP nsDownloadProgressListener::OnSecurityChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aState, nsIDownload *aDownload)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDownloadProgressListener_h__ */
