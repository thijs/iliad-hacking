/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/alerts/public/nsIAlertsService.idl
 */

#ifndef __gen_nsIAlertsService_h__
#define __gen_nsIAlertsService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIObserver_h__
#include "nsIObserver.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIAlertsService */
#define NS_IALERTSSERVICE_IID_STR "647248fd-f925-4e30-93dd-cde26d7e3a90"

#define NS_IALERTSSERVICE_IID \
  {0x647248fd, 0xf925, 0x4e30, \
    { 0x93, 0xdd, 0xcd, 0xe2, 0x6d, 0x7e, 0x3a, 0x90 }}

class NS_NO_VTABLE nsIAlertsService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IALERTSSERVICE_IID)

  /* void showAlertNotification (in AString aImageUrl, in AString aAlertTitle, in AString aAlertText, in boolean alertTextClickable, in AString aAlertCookie, in nsIObserver aAlertListener); */
  NS_IMETHOD ShowAlertNotification(const nsAString & aImageUrl, const nsAString & aAlertTitle, const nsAString & aAlertText, PRBool alertTextClickable, const nsAString & aAlertCookie, nsIObserver *aAlertListener) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIALERTSSERVICE \
  NS_IMETHOD ShowAlertNotification(const nsAString & aImageUrl, const nsAString & aAlertTitle, const nsAString & aAlertText, PRBool alertTextClickable, const nsAString & aAlertCookie, nsIObserver *aAlertListener); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIALERTSSERVICE(_to) \
  NS_IMETHOD ShowAlertNotification(const nsAString & aImageUrl, const nsAString & aAlertTitle, const nsAString & aAlertText, PRBool alertTextClickable, const nsAString & aAlertCookie, nsIObserver *aAlertListener) { return _to ShowAlertNotification(aImageUrl, aAlertTitle, aAlertText, alertTextClickable, aAlertCookie, aAlertListener); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIALERTSSERVICE(_to) \
  NS_IMETHOD ShowAlertNotification(const nsAString & aImageUrl, const nsAString & aAlertTitle, const nsAString & aAlertText, PRBool alertTextClickable, const nsAString & aAlertCookie, nsIObserver *aAlertListener) { return !_to ? NS_ERROR_NULL_POINTER : _to->ShowAlertNotification(aImageUrl, aAlertTitle, aAlertText, alertTextClickable, aAlertCookie, aAlertListener); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAlertsService : public nsIAlertsService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIALERTSSERVICE

  nsAlertsService();

private:
  ~nsAlertsService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAlertsService, nsIAlertsService)

nsAlertsService::nsAlertsService()
{
  /* member initializers and constructor code */
}

nsAlertsService::~nsAlertsService()
{
  /* destructor code */
}

/* void showAlertNotification (in AString aImageUrl, in AString aAlertTitle, in AString aAlertText, in boolean alertTextClickable, in AString aAlertCookie, in nsIObserver aAlertListener); */
NS_IMETHODIMP nsAlertsService::ShowAlertNotification(const nsAString & aImageUrl, const nsAString & aAlertTitle, const nsAString & aAlertText, PRBool alertTextClickable, const nsAString & aAlertCookie, nsIObserver *aAlertListener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIAlertsService_h__ */
