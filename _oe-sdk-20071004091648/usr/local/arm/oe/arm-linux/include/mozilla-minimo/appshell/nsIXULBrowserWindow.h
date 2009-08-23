/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/appshell/public/nsIXULBrowserWindow.idl
 */

#ifndef __gen_nsIXULBrowserWindow_h__
#define __gen_nsIXULBrowserWindow_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIRequest; /* forward declaration */


/* starting interface:    nsIXULBrowserWindow */
#define NS_IXULBROWSERWINDOW_IID_STR "46b4015c-0121-11d4-9877-00c04fa0d27a"

#define NS_IXULBROWSERWINDOW_IID \
  {0x46b4015c, 0x0121, 0x11d4, \
    { 0x98, 0x77, 0x00, 0xc0, 0x4f, 0xa0, 0xd2, 0x7a }}

class NS_NO_VTABLE nsIXULBrowserWindow : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IXULBROWSERWINDOW_IID)

  /* void setJSStatus (in wstring status); */
  NS_IMETHOD SetJSStatus(const PRUnichar *status) = 0;

  /* void setJSDefaultStatus (in wstring status); */
  NS_IMETHOD SetJSDefaultStatus(const PRUnichar *status) = 0;

  /* void setOverLink (in wstring link); */
  NS_IMETHOD SetOverLink(const PRUnichar *link) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXULBROWSERWINDOW \
  NS_IMETHOD SetJSStatus(const PRUnichar *status); \
  NS_IMETHOD SetJSDefaultStatus(const PRUnichar *status); \
  NS_IMETHOD SetOverLink(const PRUnichar *link); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXULBROWSERWINDOW(_to) \
  NS_IMETHOD SetJSStatus(const PRUnichar *status) { return _to SetJSStatus(status); } \
  NS_IMETHOD SetJSDefaultStatus(const PRUnichar *status) { return _to SetJSDefaultStatus(status); } \
  NS_IMETHOD SetOverLink(const PRUnichar *link) { return _to SetOverLink(link); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXULBROWSERWINDOW(_to) \
  NS_IMETHOD SetJSStatus(const PRUnichar *status) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetJSStatus(status); } \
  NS_IMETHOD SetJSDefaultStatus(const PRUnichar *status) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetJSDefaultStatus(status); } \
  NS_IMETHOD SetOverLink(const PRUnichar *link) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetOverLink(link); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXULBrowserWindow : public nsIXULBrowserWindow
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXULBROWSERWINDOW

  nsXULBrowserWindow();

private:
  ~nsXULBrowserWindow();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXULBrowserWindow, nsIXULBrowserWindow)

nsXULBrowserWindow::nsXULBrowserWindow()
{
  /* member initializers and constructor code */
}

nsXULBrowserWindow::~nsXULBrowserWindow()
{
  /* destructor code */
}

/* void setJSStatus (in wstring status); */
NS_IMETHODIMP nsXULBrowserWindow::SetJSStatus(const PRUnichar *status)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setJSDefaultStatus (in wstring status); */
NS_IMETHODIMP nsXULBrowserWindow::SetJSDefaultStatus(const PRUnichar *status)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setOverLink (in wstring link); */
NS_IMETHODIMP nsXULBrowserWindow::SetOverLink(const PRUnichar *link)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIXULBrowserWindow_h__ */
