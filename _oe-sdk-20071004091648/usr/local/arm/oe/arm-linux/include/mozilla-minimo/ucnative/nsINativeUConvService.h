/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/intl/uconv/native/nsINativeUConvService.idl
 */

#ifndef __gen_nsINativeUConvService_h__
#define __gen_nsINativeUConvService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsINativeUConvService */
#define NS_INATIVEUCONVSERVICE_IID_STR "c60097ba-c79d-461b-9116-80a66404c0b0"

#define NS_INATIVEUCONVSERVICE_IID \
  {0xc60097ba, 0xc79d, 0x461b, \
    { 0x91, 0x16, 0x80, 0xa6, 0x64, 0x04, 0xc0, 0xb0 }}

class NS_NO_VTABLE nsINativeUConvService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_INATIVEUCONVSERVICE_IID)

  /* nsISupports getNativeConverter (in string from, in string to); */
  NS_IMETHOD GetNativeConverter(const char *from, const char *to, nsISupports **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSINATIVEUCONVSERVICE \
  NS_IMETHOD GetNativeConverter(const char *from, const char *to, nsISupports **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSINATIVEUCONVSERVICE(_to) \
  NS_IMETHOD GetNativeConverter(const char *from, const char *to, nsISupports **_retval) { return _to GetNativeConverter(from, to, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSINATIVEUCONVSERVICE(_to) \
  NS_IMETHOD GetNativeConverter(const char *from, const char *to, nsISupports **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNativeConverter(from, to, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsNativeUConvService : public nsINativeUConvService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSINATIVEUCONVSERVICE

  nsNativeUConvService();

private:
  ~nsNativeUConvService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsNativeUConvService, nsINativeUConvService)

nsNativeUConvService::nsNativeUConvService()
{
  /* member initializers and constructor code */
}

nsNativeUConvService::~nsNativeUConvService()
{
  /* destructor code */
}

/* nsISupports getNativeConverter (in string from, in string to); */
NS_IMETHODIMP nsNativeUConvService::GetNativeConverter(const char *from, const char *to, nsISupports **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsINativeUConvService_h__ */
