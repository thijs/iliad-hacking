/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/profile/public/nsIProfileStartupListener.idl
 */

#ifndef __gen_nsIProfileStartupListener_h__
#define __gen_nsIProfileStartupListener_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#define NS_PROFILE_STARTUP_LISTENER_CID                \
  { /* {6962ca90-0b8b-11d4-9875-00c04fa0d28b} */       \
    0x6962ca90,                                        \
    0x0b8b,                                            \
    0x11d4,                                            \
    { 0x98, 0x75, 0x00, 0xc0, 0x4f, 0xa0, 0xd2, 0x8b } \
  }
#define NS_PROFILE_STARTUP_LISTENER_CONTRACTID	\
	"@mozilla.org/profile/startuplistener;1"

/* starting interface:    nsIProfileStartupListener */
#define NS_IPROFILESTARTUPLISTENER_IID_STR "6962ca8f-0b8b-11d4-9875-00c04fa0d28b"

#define NS_IPROFILESTARTUPLISTENER_IID \
  {0x6962ca8f, 0x0b8b, 0x11d4, \
    { 0x98, 0x75, 0x00, 0xc0, 0x4f, 0xa0, 0xd2, 0x8b }}

class NS_NO_VTABLE nsIProfileStartupListener : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IPROFILESTARTUPLISTENER_IID)

  /* void onProfileStartup (in wstring profileName); */
  NS_IMETHOD OnProfileStartup(const PRUnichar *profileName) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPROFILESTARTUPLISTENER \
  NS_IMETHOD OnProfileStartup(const PRUnichar *profileName); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPROFILESTARTUPLISTENER(_to) \
  NS_IMETHOD OnProfileStartup(const PRUnichar *profileName) { return _to OnProfileStartup(profileName); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPROFILESTARTUPLISTENER(_to) \
  NS_IMETHOD OnProfileStartup(const PRUnichar *profileName) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnProfileStartup(profileName); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsProfileStartupListener : public nsIProfileStartupListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPROFILESTARTUPLISTENER

  nsProfileStartupListener();

private:
  ~nsProfileStartupListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsProfileStartupListener, nsIProfileStartupListener)

nsProfileStartupListener::nsProfileStartupListener()
{
  /* member initializers and constructor code */
}

nsProfileStartupListener::~nsProfileStartupListener()
{
  /* destructor code */
}

/* void onProfileStartup (in wstring profileName); */
NS_IMETHODIMP nsProfileStartupListener::OnProfileStartup(const PRUnichar *profileName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIProfileStartupListener_h__ */
