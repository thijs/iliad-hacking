/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/minimo/components/device/nsIDeviceSupport.idl
 */

#ifndef __gen_nsIDeviceSupport_h__
#define __gen_nsIDeviceSupport_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIDeviceSupport */
#define NS_IDEVICESUPPORT_IID_STR "11ac1ce1-f68f-4c2a-ab38-7dae14c4beaf"

#define NS_IDEVICESUPPORT_IID \
  {0x11ac1ce1, 0xf68f, 0x4c2a, \
    { 0xab, 0x38, 0x7d, 0xae, 0x14, 0xc4, 0xbe, 0xaf }}

class NS_NO_VTABLE nsIDeviceSupport : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IDEVICESUPPORT_IID)

  /* void rotateScreen (in boolean aLandscapeMode); */
  NS_IMETHOD RotateScreen(PRBool aLandscapeMode) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIDEVICESUPPORT \
  NS_IMETHOD RotateScreen(PRBool aLandscapeMode); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIDEVICESUPPORT(_to) \
  NS_IMETHOD RotateScreen(PRBool aLandscapeMode) { return _to RotateScreen(aLandscapeMode); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIDEVICESUPPORT(_to) \
  NS_IMETHOD RotateScreen(PRBool aLandscapeMode) { return !_to ? NS_ERROR_NULL_POINTER : _to->RotateScreen(aLandscapeMode); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsDeviceSupport : public nsIDeviceSupport
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDEVICESUPPORT

  nsDeviceSupport();

private:
  ~nsDeviceSupport();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsDeviceSupport, nsIDeviceSupport)

nsDeviceSupport::nsDeviceSupport()
{
  /* member initializers and constructor code */
}

nsDeviceSupport::~nsDeviceSupport()
{
  /* destructor code */
}

/* void rotateScreen (in boolean aLandscapeMode); */
NS_IMETHODIMP nsDeviceSupport::RotateScreen(PRBool aLandscapeMode)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIDeviceSupport_h__ */
