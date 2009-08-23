/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPCallCompletion.idl
 */

#ifndef __gen_nsISOAPCallCompletion_h__
#define __gen_nsISOAPCallCompletion_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISOAPCall; /* forward declaration */

class nsISOAPResponse; /* forward declaration */

class nsISOAPResponseListener; /* forward declaration */


/* starting interface:    nsISOAPCallCompletion */
#define NS_ISOAPCALLCOMPLETION_IID_STR "86114dd8-1dd2-11b2-ab2b-91d0c995e03a"

#define NS_ISOAPCALLCOMPLETION_IID \
  {0x86114dd8, 0x1dd2, 0x11b2, \
    { 0xab, 0x2b, 0x91, 0xd0, 0xc9, 0x95, 0xe0, 0x3a }}

/**
 * This permits the invoker of an asynchronous call to
 *   abort the call before it completes or to test to
 *   see whether it is complete.
 */
class NS_NO_VTABLE nsISOAPCallCompletion : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPCALLCOMPLETION_IID)

  /**
   * The call which was invoked (may have changed since
   *   the call was made).
   */
  /* readonly attribute nsISOAPCall call; */
  NS_IMETHOD GetCall(nsISOAPCall * *aCall) = 0;

  /**
   * The response, if any, to the call.
   */
  /* readonly attribute nsISOAPResponse response; */
  NS_IMETHOD GetResponse(nsISOAPResponse * *aResponse) = 0;

  /**
   * The listener to the call.
   */
  /* readonly attribute nsISOAPResponseListener listener; */
  NS_IMETHOD GetListener(nsISOAPResponseListener * *aListener) = 0;

  /**
   * Whether the call is complete.
   */
  /* readonly attribute boolean isComplete; */
  NS_IMETHOD GetIsComplete(PRBool *aIsComplete) = 0;

  /**
   * Cause the invoked method to abort, if it is not
   * already complete.
   * @return true if the state of isComplete became
   *   true as a result.  False if it was already
   *   true or reamined false.
   */
  /* boolean abort (); */
  NS_IMETHOD Abort(PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPCALLCOMPLETION \
  NS_IMETHOD GetCall(nsISOAPCall * *aCall); \
  NS_IMETHOD GetResponse(nsISOAPResponse * *aResponse); \
  NS_IMETHOD GetListener(nsISOAPResponseListener * *aListener); \
  NS_IMETHOD GetIsComplete(PRBool *aIsComplete); \
  NS_IMETHOD Abort(PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPCALLCOMPLETION(_to) \
  NS_IMETHOD GetCall(nsISOAPCall * *aCall) { return _to GetCall(aCall); } \
  NS_IMETHOD GetResponse(nsISOAPResponse * *aResponse) { return _to GetResponse(aResponse); } \
  NS_IMETHOD GetListener(nsISOAPResponseListener * *aListener) { return _to GetListener(aListener); } \
  NS_IMETHOD GetIsComplete(PRBool *aIsComplete) { return _to GetIsComplete(aIsComplete); } \
  NS_IMETHOD Abort(PRBool *_retval) { return _to Abort(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPCALLCOMPLETION(_to) \
  NS_IMETHOD GetCall(nsISOAPCall * *aCall) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCall(aCall); } \
  NS_IMETHOD GetResponse(nsISOAPResponse * *aResponse) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetResponse(aResponse); } \
  NS_IMETHOD GetListener(nsISOAPResponseListener * *aListener) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetListener(aListener); } \
  NS_IMETHOD GetIsComplete(PRBool *aIsComplete) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsComplete(aIsComplete); } \
  NS_IMETHOD Abort(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Abort(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPCallCompletion : public nsISOAPCallCompletion
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPCALLCOMPLETION

  nsSOAPCallCompletion();

private:
  ~nsSOAPCallCompletion();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPCallCompletion, nsISOAPCallCompletion)

nsSOAPCallCompletion::nsSOAPCallCompletion()
{
  /* member initializers and constructor code */
}

nsSOAPCallCompletion::~nsSOAPCallCompletion()
{
  /* destructor code */
}

/* readonly attribute nsISOAPCall call; */
NS_IMETHODIMP nsSOAPCallCompletion::GetCall(nsISOAPCall * *aCall)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISOAPResponse response; */
NS_IMETHODIMP nsSOAPCallCompletion::GetResponse(nsISOAPResponse * *aResponse)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISOAPResponseListener listener; */
NS_IMETHODIMP nsSOAPCallCompletion::GetListener(nsISOAPResponseListener * *aListener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isComplete; */
NS_IMETHODIMP nsSOAPCallCompletion::GetIsComplete(PRBool *aIsComplete)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean abort (); */
NS_IMETHODIMP nsSOAPCallCompletion::Abort(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISOAPCallCompletion_h__ */
