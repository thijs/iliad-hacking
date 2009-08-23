/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/startup/public/nsINativeAppSupport.idl
 */

#ifndef __gen_nsINativeAppSupport_h__
#define __gen_nsINativeAppSupport_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIXULWindow; /* forward declaration */

class nsICmdLineService; /* forward declaration */


/* starting interface:    nsINativeAppSupport */
#define NS_INATIVEAPPSUPPORT_IID_STR "5fdf8480-1f98-11d4-8077-00600811a9c3"

#define NS_INATIVEAPPSUPPORT_IID \
  {0x5fdf8480, 0x1f98, 0x11d4, \
    { 0x80, 0x77, 0x00, 0x60, 0x08, 0x11, 0xa9, 0xc3 }}

class NS_NO_VTABLE nsINativeAppSupport : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_INATIVEAPPSUPPORT_IID)

  /* boolean start (); */
  NS_IMETHOD Start(PRBool *_retval) = 0;

  /* boolean stop (); */
  NS_IMETHOD Stop(PRBool *_retval) = 0;

  /* void quit (); */
  NS_IMETHOD Quit(void) = 0;

  /* [noscript] void ensureProfile (in nsICmdLineService aCmdService); */
  NS_IMETHOD EnsureProfile(nsICmdLineService *aCmdService) = 0;

  /* void showSplashScreen (); */
  NS_IMETHOD ShowSplashScreen(void) = 0;

  /* void hideSplashScreen (); */
  NS_IMETHOD HideSplashScreen(void) = 0;

  /* attribute boolean isServerMode; */
  NS_IMETHOD GetIsServerMode(PRBool *aIsServerMode) = 0;
  NS_IMETHOD SetIsServerMode(PRBool aIsServerMode) = 0;

  /* attribute boolean shouldShowUI; */
  NS_IMETHOD GetShouldShowUI(PRBool *aShouldShowUI) = 0;
  NS_IMETHOD SetShouldShowUI(PRBool aShouldShowUI) = 0;

  /* void startServerMode (); */
  NS_IMETHOD StartServerMode(void) = 0;

  /* void onLastWindowClosing (); */
  NS_IMETHOD OnLastWindowClosing(void) = 0;

  /* void ReOpen (); */
  NS_IMETHOD ReOpen(void) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSINATIVEAPPSUPPORT \
  NS_IMETHOD Start(PRBool *_retval); \
  NS_IMETHOD Stop(PRBool *_retval); \
  NS_IMETHOD Quit(void); \
  NS_IMETHOD EnsureProfile(nsICmdLineService *aCmdService); \
  NS_IMETHOD ShowSplashScreen(void); \
  NS_IMETHOD HideSplashScreen(void); \
  NS_IMETHOD GetIsServerMode(PRBool *aIsServerMode); \
  NS_IMETHOD SetIsServerMode(PRBool aIsServerMode); \
  NS_IMETHOD GetShouldShowUI(PRBool *aShouldShowUI); \
  NS_IMETHOD SetShouldShowUI(PRBool aShouldShowUI); \
  NS_IMETHOD StartServerMode(void); \
  NS_IMETHOD OnLastWindowClosing(void); \
  NS_IMETHOD ReOpen(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSINATIVEAPPSUPPORT(_to) \
  NS_IMETHOD Start(PRBool *_retval) { return _to Start(_retval); } \
  NS_IMETHOD Stop(PRBool *_retval) { return _to Stop(_retval); } \
  NS_IMETHOD Quit(void) { return _to Quit(); } \
  NS_IMETHOD EnsureProfile(nsICmdLineService *aCmdService) { return _to EnsureProfile(aCmdService); } \
  NS_IMETHOD ShowSplashScreen(void) { return _to ShowSplashScreen(); } \
  NS_IMETHOD HideSplashScreen(void) { return _to HideSplashScreen(); } \
  NS_IMETHOD GetIsServerMode(PRBool *aIsServerMode) { return _to GetIsServerMode(aIsServerMode); } \
  NS_IMETHOD SetIsServerMode(PRBool aIsServerMode) { return _to SetIsServerMode(aIsServerMode); } \
  NS_IMETHOD GetShouldShowUI(PRBool *aShouldShowUI) { return _to GetShouldShowUI(aShouldShowUI); } \
  NS_IMETHOD SetShouldShowUI(PRBool aShouldShowUI) { return _to SetShouldShowUI(aShouldShowUI); } \
  NS_IMETHOD StartServerMode(void) { return _to StartServerMode(); } \
  NS_IMETHOD OnLastWindowClosing(void) { return _to OnLastWindowClosing(); } \
  NS_IMETHOD ReOpen(void) { return _to ReOpen(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSINATIVEAPPSUPPORT(_to) \
  NS_IMETHOD Start(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Start(_retval); } \
  NS_IMETHOD Stop(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Stop(_retval); } \
  NS_IMETHOD Quit(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Quit(); } \
  NS_IMETHOD EnsureProfile(nsICmdLineService *aCmdService) { return !_to ? NS_ERROR_NULL_POINTER : _to->EnsureProfile(aCmdService); } \
  NS_IMETHOD ShowSplashScreen(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ShowSplashScreen(); } \
  NS_IMETHOD HideSplashScreen(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->HideSplashScreen(); } \
  NS_IMETHOD GetIsServerMode(PRBool *aIsServerMode) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsServerMode(aIsServerMode); } \
  NS_IMETHOD SetIsServerMode(PRBool aIsServerMode) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetIsServerMode(aIsServerMode); } \
  NS_IMETHOD GetShouldShowUI(PRBool *aShouldShowUI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetShouldShowUI(aShouldShowUI); } \
  NS_IMETHOD SetShouldShowUI(PRBool aShouldShowUI) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetShouldShowUI(aShouldShowUI); } \
  NS_IMETHOD StartServerMode(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->StartServerMode(); } \
  NS_IMETHOD OnLastWindowClosing(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnLastWindowClosing(); } \
  NS_IMETHOD ReOpen(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReOpen(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsNativeAppSupport : public nsINativeAppSupport
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSINATIVEAPPSUPPORT

  nsNativeAppSupport();

private:
  ~nsNativeAppSupport();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsNativeAppSupport, nsINativeAppSupport)

nsNativeAppSupport::nsNativeAppSupport()
{
  /* member initializers and constructor code */
}

nsNativeAppSupport::~nsNativeAppSupport()
{
  /* destructor code */
}

/* boolean start (); */
NS_IMETHODIMP nsNativeAppSupport::Start(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean stop (); */
NS_IMETHODIMP nsNativeAppSupport::Stop(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void quit (); */
NS_IMETHODIMP nsNativeAppSupport::Quit()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void ensureProfile (in nsICmdLineService aCmdService); */
NS_IMETHODIMP nsNativeAppSupport::EnsureProfile(nsICmdLineService *aCmdService)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void showSplashScreen (); */
NS_IMETHODIMP nsNativeAppSupport::ShowSplashScreen()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void hideSplashScreen (); */
NS_IMETHODIMP nsNativeAppSupport::HideSplashScreen()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean isServerMode; */
NS_IMETHODIMP nsNativeAppSupport::GetIsServerMode(PRBool *aIsServerMode)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsNativeAppSupport::SetIsServerMode(PRBool aIsServerMode)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean shouldShowUI; */
NS_IMETHODIMP nsNativeAppSupport::GetShouldShowUI(PRBool *aShouldShowUI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsNativeAppSupport::SetShouldShowUI(PRBool aShouldShowUI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void startServerMode (); */
NS_IMETHODIMP nsNativeAppSupport::StartServerMode()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onLastWindowClosing (); */
NS_IMETHODIMP nsNativeAppSupport::OnLastWindowClosing()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void ReOpen (); */
NS_IMETHODIMP nsNativeAppSupport::ReOpen()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsINativeAppSupport_h__ */
