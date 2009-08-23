/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/security/manager/ssl/public/nsITokenDialogs.idl
 */

#ifndef __gen_nsITokenDialogs_h__
#define __gen_nsITokenDialogs_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIInterfaceRequestor; /* forward declaration */


/* starting interface:    nsITokenDialogs */
#define NS_ITOKENDIALOGS_IID_STR "bb4bae9c-39c5-11d5-ba26-00108303b117"

#define NS_ITOKENDIALOGS_IID \
  {0xbb4bae9c, 0x39c5, 0x11d5, \
    { 0xba, 0x26, 0x00, 0x10, 0x83, 0x03, 0xb1, 0x17 }}

class NS_NO_VTABLE nsITokenDialogs : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ITOKENDIALOGS_IID)

  /* void ChooseToken (in nsIInterfaceRequestor ctx, [array, size_is (count)] in wstring tokenNameList, in unsigned long count, out wstring tokenName, out boolean canceled); */
  NS_IMETHOD ChooseToken(nsIInterfaceRequestor *ctx, const PRUnichar **tokenNameList, PRUint32 count, PRUnichar **tokenName, PRBool *canceled) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSITOKENDIALOGS \
  NS_IMETHOD ChooseToken(nsIInterfaceRequestor *ctx, const PRUnichar **tokenNameList, PRUint32 count, PRUnichar **tokenName, PRBool *canceled); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSITOKENDIALOGS(_to) \
  NS_IMETHOD ChooseToken(nsIInterfaceRequestor *ctx, const PRUnichar **tokenNameList, PRUint32 count, PRUnichar **tokenName, PRBool *canceled) { return _to ChooseToken(ctx, tokenNameList, count, tokenName, canceled); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSITOKENDIALOGS(_to) \
  NS_IMETHOD ChooseToken(nsIInterfaceRequestor *ctx, const PRUnichar **tokenNameList, PRUint32 count, PRUnichar **tokenName, PRBool *canceled) { return !_to ? NS_ERROR_NULL_POINTER : _to->ChooseToken(ctx, tokenNameList, count, tokenName, canceled); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsTokenDialogs : public nsITokenDialogs
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSITOKENDIALOGS

  nsTokenDialogs();

private:
  ~nsTokenDialogs();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsTokenDialogs, nsITokenDialogs)

nsTokenDialogs::nsTokenDialogs()
{
  /* member initializers and constructor code */
}

nsTokenDialogs::~nsTokenDialogs()
{
  /* destructor code */
}

/* void ChooseToken (in nsIInterfaceRequestor ctx, [array, size_is (count)] in wstring tokenNameList, in unsigned long count, out wstring tokenName, out boolean canceled); */
NS_IMETHODIMP nsTokenDialogs::ChooseToken(nsIInterfaceRequestor *ctx, const PRUnichar **tokenNameList, PRUint32 count, PRUnichar **tokenName, PRBool *canceled)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_TOKENDIALOGS_CONTRACTID "@mozilla.org/nsTokenDialogs;1"

#endif /* __gen_nsITokenDialogs_h__ */
