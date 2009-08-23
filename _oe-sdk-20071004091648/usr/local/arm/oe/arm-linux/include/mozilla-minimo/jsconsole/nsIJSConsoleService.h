/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/embedding/components/jsconsole/public/nsIJSConsoleService.idl
 */

#ifndef __gen_nsIJSConsoleService_h__
#define __gen_nsIJSConsoleService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMWindow; /* forward declaration */


/* starting interface:    nsIJSConsoleService */
#define NS_IJSCONSOLESERVICE_IID_STR "1b86a0a6-1dd2-11b2-a85c-e3f42b4dcceb"

#define NS_IJSCONSOLESERVICE_IID \
  {0x1b86a0a6, 0x1dd2, 0x11b2, \
    { 0xa8, 0x5c, 0xe3, 0xf4, 0x2b, 0x4d, 0xcc, 0xeb }}

class NS_NO_VTABLE nsIJSConsoleService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IJSCONSOLESERVICE_IID)

  /* void open (in nsIDOMWindow inParent); */
  NS_IMETHOD Open(nsIDOMWindow *inParent) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIJSCONSOLESERVICE \
  NS_IMETHOD Open(nsIDOMWindow *inParent); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIJSCONSOLESERVICE(_to) \
  NS_IMETHOD Open(nsIDOMWindow *inParent) { return _to Open(inParent); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIJSCONSOLESERVICE(_to) \
  NS_IMETHOD Open(nsIDOMWindow *inParent) { return !_to ? NS_ERROR_NULL_POINTER : _to->Open(inParent); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsJSConsoleService : public nsIJSConsoleService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIJSCONSOLESERVICE

  nsJSConsoleService();

private:
  ~nsJSConsoleService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsJSConsoleService, nsIJSConsoleService)

nsJSConsoleService::nsJSConsoleService()
{
  /* member initializers and constructor code */
}

nsJSConsoleService::~nsJSConsoleService()
{
  /* destructor code */
}

/* void open (in nsIDOMWindow inParent); */
NS_IMETHODIMP nsJSConsoleService::Open(nsIDOMWindow *inParent)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIJSConsoleService_h__ */
