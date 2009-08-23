/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/profile/public/nsISessionRoaming.idl
 */

#ifndef __gen_nsISessionRoaming_h__
#define __gen_nsISessionRoaming_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIFile_h__
#include "nsIFile.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsISessionRoaming */
#define NS_ISESSIONROAMING_IID_STR "ab62465c-494c-446e-b671-930bb98a7bc4"

#define NS_ISESSIONROAMING_IID \
  {0xab62465c, 0x494c, 0x446e, \
    { 0xb6, 0x71, 0x93, 0x0b, 0xb9, 0x8a, 0x7b, 0xc4 }}

/**
 * nsISessionRoaming
 *
 * Implementation should be a service.
 *
 * see extensions/sraoming/README.txt
 * 
 * @status EXPERIMENTAL
 * @version 1.0
 */
class NS_NO_VTABLE nsISessionRoaming : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISESSIONROAMING_IID)

  /* void BeginSession (); */
  NS_IMETHOD BeginSession(void) = 0;

  /* void EndSession (); */
  NS_IMETHOD EndSession(void) = 0;

  /* boolean isRoaming (); */
  NS_IMETHOD IsRoaming(PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISESSIONROAMING \
  NS_IMETHOD BeginSession(void); \
  NS_IMETHOD EndSession(void); \
  NS_IMETHOD IsRoaming(PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISESSIONROAMING(_to) \
  NS_IMETHOD BeginSession(void) { return _to BeginSession(); } \
  NS_IMETHOD EndSession(void) { return _to EndSession(); } \
  NS_IMETHOD IsRoaming(PRBool *_retval) { return _to IsRoaming(_retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISESSIONROAMING(_to) \
  NS_IMETHOD BeginSession(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->BeginSession(); } \
  NS_IMETHOD EndSession(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->EndSession(); } \
  NS_IMETHOD IsRoaming(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsRoaming(_retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSessionRoaming : public nsISessionRoaming
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISESSIONROAMING

  nsSessionRoaming();

private:
  ~nsSessionRoaming();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSessionRoaming, nsISessionRoaming)

nsSessionRoaming::nsSessionRoaming()
{
  /* member initializers and constructor code */
}

nsSessionRoaming::~nsSessionRoaming()
{
  /* destructor code */
}

/* void BeginSession (); */
NS_IMETHODIMP nsSessionRoaming::BeginSession()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void EndSession (); */
NS_IMETHODIMP nsSessionRoaming::EndSession()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isRoaming (); */
NS_IMETHODIMP nsSessionRoaming::IsRoaming(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISessionRoaming_h__ */
