/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/startup/public/nsICloseAllWindows.idl
 */

#ifndef __gen_nsICloseAllWindows_h__
#define __gen_nsICloseAllWindows_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsICloseAllWindows */
#define NS_ICLOSEALLWINDOWS_IID_STR "2f977d49-5485-11d4-87e2-0010a4e75ef2"

#define NS_ICLOSEALLWINDOWS_IID \
  {0x2f977d49, 0x5485, 0x11d4, \
    { 0x87, 0xe2, 0x00, 0x10, 0xa4, 0xe7, 0x5e, 0xf2 }}

class NS_NO_VTABLE nsICloseAllWindows : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICLOSEALLWINDOWS_IID)

  /**
     * Closes all open windows, optionally asking to confirm changes if any.
     * @return TRUE if the user does not cancel from any confirmation dialog.
     */
  /* boolean closeAll (in boolean askSave); */
  NS_IMETHOD CloseAll(PRBool askSave, PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICLOSEALLWINDOWS \
  NS_IMETHOD CloseAll(PRBool askSave, PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICLOSEALLWINDOWS(_to) \
  NS_IMETHOD CloseAll(PRBool askSave, PRBool *_retval) { return _to CloseAll(askSave, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICLOSEALLWINDOWS(_to) \
  NS_IMETHOD CloseAll(PRBool askSave, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CloseAll(askSave, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsCloseAllWindows : public nsICloseAllWindows
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICLOSEALLWINDOWS

  nsCloseAllWindows();

private:
  ~nsCloseAllWindows();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsCloseAllWindows, nsICloseAllWindows)

nsCloseAllWindows::nsCloseAllWindows()
{
  /* member initializers and constructor code */
}

nsCloseAllWindows::~nsCloseAllWindows()
{
  /* destructor code */
}

/* boolean closeAll (in boolean askSave); */
NS_IMETHODIMP nsCloseAllWindows::CloseAll(PRBool askSave, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsICloseAllWindows_h__ */
