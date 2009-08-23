/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/spatialnavigation/public/nsISpatialNavigation.idl
 */

#ifndef __gen_nsISpatialNavigation_h__
#define __gen_nsISpatialNavigation_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMWindow; /* forward declaration */


/* starting interface:    nsISpatialNavigation */
#define NS_ISPATIALNAVIGATION_IID_STR "ef7cb437-9eb3-47c7-9d89-71679bfefcc1"

#define NS_ISPATIALNAVIGATION_IID \
  {0xef7cb437, 0x9eb3, 0x47c7, \
    { 0x9d, 0x89, 0x71, 0x67, 0x9b, 0xfe, 0xfc, 0xc1 }}

class NS_NO_VTABLE nsISpatialNavigation : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISPATIALNAVIGATION_IID)

  /* void init (in nsIDOMWindow aWindow); */
  NS_IMETHOD Init(nsIDOMWindow *aWindow) = 0;

  /* void shutdown (); */
  NS_IMETHOD Shutdown(void) = 0;

  /* void up (); */
  NS_IMETHOD Up(void) = 0;

  /* void down (); */
  NS_IMETHOD Down(void) = 0;

  /* void left (); */
  NS_IMETHOD Left(void) = 0;

  /* void right (); */
  NS_IMETHOD Right(void) = 0;

  /* readonly attribute nsIDOMWindow attachedWindow; */
  NS_IMETHOD GetAttachedWindow(nsIDOMWindow * *aAttachedWindow) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISPATIALNAVIGATION \
  NS_IMETHOD Init(nsIDOMWindow *aWindow); \
  NS_IMETHOD Shutdown(void); \
  NS_IMETHOD Up(void); \
  NS_IMETHOD Down(void); \
  NS_IMETHOD Left(void); \
  NS_IMETHOD Right(void); \
  NS_IMETHOD GetAttachedWindow(nsIDOMWindow * *aAttachedWindow); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISPATIALNAVIGATION(_to) \
  NS_IMETHOD Init(nsIDOMWindow *aWindow) { return _to Init(aWindow); } \
  NS_IMETHOD Shutdown(void) { return _to Shutdown(); } \
  NS_IMETHOD Up(void) { return _to Up(); } \
  NS_IMETHOD Down(void) { return _to Down(); } \
  NS_IMETHOD Left(void) { return _to Left(); } \
  NS_IMETHOD Right(void) { return _to Right(); } \
  NS_IMETHOD GetAttachedWindow(nsIDOMWindow * *aAttachedWindow) { return _to GetAttachedWindow(aAttachedWindow); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISPATIALNAVIGATION(_to) \
  NS_IMETHOD Init(nsIDOMWindow *aWindow) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aWindow); } \
  NS_IMETHOD Shutdown(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Shutdown(); } \
  NS_IMETHOD Up(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Up(); } \
  NS_IMETHOD Down(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Down(); } \
  NS_IMETHOD Left(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Left(); } \
  NS_IMETHOD Right(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Right(); } \
  NS_IMETHOD GetAttachedWindow(nsIDOMWindow * *aAttachedWindow) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAttachedWindow(aAttachedWindow); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSpatialNavigation : public nsISpatialNavigation
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISPATIALNAVIGATION

  nsSpatialNavigation();

private:
  ~nsSpatialNavigation();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSpatialNavigation, nsISpatialNavigation)

nsSpatialNavigation::nsSpatialNavigation()
{
  /* member initializers and constructor code */
}

nsSpatialNavigation::~nsSpatialNavigation()
{
  /* destructor code */
}

/* void init (in nsIDOMWindow aWindow); */
NS_IMETHODIMP nsSpatialNavigation::Init(nsIDOMWindow *aWindow)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void shutdown (); */
NS_IMETHODIMP nsSpatialNavigation::Shutdown()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void up (); */
NS_IMETHODIMP nsSpatialNavigation::Up()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void down (); */
NS_IMETHODIMP nsSpatialNavigation::Down()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void left (); */
NS_IMETHODIMP nsSpatialNavigation::Left()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void right (); */
NS_IMETHODIMP nsSpatialNavigation::Right()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMWindow attachedWindow; */
NS_IMETHODIMP nsSpatialNavigation::GetAttachedWindow(nsIDOMWindow * *aAttachedWindow)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISpatialNavigation_h__ */
