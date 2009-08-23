/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/startup/public/nsIAppStartup.idl
 */

#ifndef __gen_nsIAppStartup_h__
#define __gen_nsIAppStartup_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsICmdLineService; /* forward declaration */


/* starting interface:    nsIAppStartup */
#define NS_IAPPSTARTUP_IID_STR "e9b0f89b-0529-4d96-98a8-eb5b2b9a8383"

#define NS_IAPPSTARTUP_IID \
  {0xe9b0f89b, 0x0529, 0x4d96, \
    { 0x98, 0xa8, 0xeb, 0x5b, 0x2b, 0x9a, 0x83, 0x83 }}

class NS_NO_VTABLE nsIAppStartup : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAPPSTARTUP_IID)

  /**
     * Create the hidden window.
     */
  /* void createHiddenWindow (); */
  NS_IMETHOD CreateHiddenWindow(void) = 0;

  /**
     * Runs an application event loop: normally the main event pump which
     * defines the lifetime of the application. If there are no windows open
     * and no outstanding calls to enterLastWindowClosingSurvivalArea this
     * method will exit immediately.
     *
     * @returnCode NS_SUCCESS_RESTART_APP
     *             This return code indicates that the application should be
     *             restarted because quit was called with the eRestart flag.
     */
  /* void run (); */
  NS_IMETHOD Run(void) = 0;

  /**
     * There are situations where all application windows will be
     * closed but we don't want to take this as a signal to quit the
     * app. Bracket the code where the last window could close with
     * these.
     */
  /* void enterLastWindowClosingSurvivalArea (); */
  NS_IMETHOD EnterLastWindowClosingSurvivalArea(void) = 0;

  /* void exitLastWindowClosingSurvivalArea (); */
  NS_IMETHOD ExitLastWindowClosingSurvivalArea(void) = 0;

  /**
     * The following flags may be passed as the aMode parameter to the quit
     * method.  One and only one of the "Quit" flags must be specified.  The
     * eRestart flag may be bit-wise combined with one of the "Quit" flags to
     * cause the application to restart after it quits.
     */
/**
     * Attempt to quit if all windows are closed.
     */
  enum { eConsiderQuit = 1U };

  /**
     * Try to close all windows, then quit if successful.
     */
  enum { eAttemptQuit = 2U };

  /**
     * Quit, damnit!
     */
  enum { eForceQuit = 3U };

  /**
     * Restart the application after quitting.  The application will be
     * restarted with the same profile and an empty command line.
     */
  enum { eRestart = 16U };

  /**
     * Exit the event loop, and shut down the app.
     *
     * @param aMode
     *        This parameter modifies how the app is shutdown, and it is
     *        constructed from the constants defined above.
     */
  /* void quit (in PRUint32 aMode); */
  NS_IMETHOD Quit(PRUint32 aMode) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAPPSTARTUP \
  NS_IMETHOD CreateHiddenWindow(void); \
  NS_IMETHOD Run(void); \
  NS_IMETHOD EnterLastWindowClosingSurvivalArea(void); \
  NS_IMETHOD ExitLastWindowClosingSurvivalArea(void); \
  NS_IMETHOD Quit(PRUint32 aMode); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAPPSTARTUP(_to) \
  NS_IMETHOD CreateHiddenWindow(void) { return _to CreateHiddenWindow(); } \
  NS_IMETHOD Run(void) { return _to Run(); } \
  NS_IMETHOD EnterLastWindowClosingSurvivalArea(void) { return _to EnterLastWindowClosingSurvivalArea(); } \
  NS_IMETHOD ExitLastWindowClosingSurvivalArea(void) { return _to ExitLastWindowClosingSurvivalArea(); } \
  NS_IMETHOD Quit(PRUint32 aMode) { return _to Quit(aMode); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAPPSTARTUP(_to) \
  NS_IMETHOD CreateHiddenWindow(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateHiddenWindow(); } \
  NS_IMETHOD Run(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Run(); } \
  NS_IMETHOD EnterLastWindowClosingSurvivalArea(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->EnterLastWindowClosingSurvivalArea(); } \
  NS_IMETHOD ExitLastWindowClosingSurvivalArea(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ExitLastWindowClosingSurvivalArea(); } \
  NS_IMETHOD Quit(PRUint32 aMode) { return !_to ? NS_ERROR_NULL_POINTER : _to->Quit(aMode); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAppStartup : public nsIAppStartup
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAPPSTARTUP

  nsAppStartup();

private:
  ~nsAppStartup();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAppStartup, nsIAppStartup)

nsAppStartup::nsAppStartup()
{
  /* member initializers and constructor code */
}

nsAppStartup::~nsAppStartup()
{
  /* destructor code */
}

/* void createHiddenWindow (); */
NS_IMETHODIMP nsAppStartup::CreateHiddenWindow()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void run (); */
NS_IMETHODIMP nsAppStartup::Run()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void enterLastWindowClosingSurvivalArea (); */
NS_IMETHODIMP nsAppStartup::EnterLastWindowClosingSurvivalArea()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void exitLastWindowClosingSurvivalArea (); */
NS_IMETHODIMP nsAppStartup::ExitLastWindowClosingSurvivalArea()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void quit (in PRUint32 aMode); */
NS_IMETHODIMP nsAppStartup::Quit(PRUint32 aMode)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

/**
 * This success code may be returned by nsIAppStartup::Run to indicate that the
 * application should be restarted.  This condition corresponds to the case in
 * which nsIAppStartup::Quit was called with the eRestart flag.
 */
#define NS_SUCCESS_RESTART_APP \
    NS_ERROR_GENERATE_SUCCESS(NS_ERROR_MODULE_GENERAL, 1)

#endif /* __gen_nsIAppStartup_h__ */
