/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/extensions/public/nsIExtensionManager.idl
 */

#ifndef __gen_nsIExtensionManager_h__
#define __gen_nsIExtensionManager_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIFile; /* forward declaration */


/* starting interface:    nsIExtensionManager */
#define NS_IEXTENSIONMANAGER_IID_STR "c3515b0f-99f4-453b-805e-1fdf5724d6d9"

#define NS_IEXTENSIONMANAGER_IID \
  {0xc3515b0f, 0x99f4, 0x453b, \
    { 0x80, 0x5e, 0x1f, 0xdf, 0x57, 0x24, 0xd6, 0xd9 }}

class NS_NO_VTABLE nsIExtensionManager : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IEXTENSIONMANAGER_IID)

  enum { FLAG_INSTALL_PROFILE = 1U };

  /* void installExtension (in nsIFile aXPIFile, in unsigned long aFlags); */
  NS_IMETHOD InstallExtension(nsIFile *aXPIFile, PRUint32 aFlags) = 0;

  /* void installTheme (in nsIFile aJARFile, in unsigned long aFlags); */
  NS_IMETHOD InstallTheme(nsIFile *aJARFile, PRUint32 aFlags) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIEXTENSIONMANAGER \
  NS_IMETHOD InstallExtension(nsIFile *aXPIFile, PRUint32 aFlags); \
  NS_IMETHOD InstallTheme(nsIFile *aJARFile, PRUint32 aFlags); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIEXTENSIONMANAGER(_to) \
  NS_IMETHOD InstallExtension(nsIFile *aXPIFile, PRUint32 aFlags) { return _to InstallExtension(aXPIFile, aFlags); } \
  NS_IMETHOD InstallTheme(nsIFile *aJARFile, PRUint32 aFlags) { return _to InstallTheme(aJARFile, aFlags); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIEXTENSIONMANAGER(_to) \
  NS_IMETHOD InstallExtension(nsIFile *aXPIFile, PRUint32 aFlags) { return !_to ? NS_ERROR_NULL_POINTER : _to->InstallExtension(aXPIFile, aFlags); } \
  NS_IMETHOD InstallTheme(nsIFile *aJARFile, PRUint32 aFlags) { return !_to ? NS_ERROR_NULL_POINTER : _to->InstallTheme(aJARFile, aFlags); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsExtensionManager : public nsIExtensionManager
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIEXTENSIONMANAGER

  nsExtensionManager();

private:
  ~nsExtensionManager();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsExtensionManager, nsIExtensionManager)

nsExtensionManager::nsExtensionManager()
{
  /* member initializers and constructor code */
}

nsExtensionManager::~nsExtensionManager()
{
  /* destructor code */
}

/* void installExtension (in nsIFile aXPIFile, in unsigned long aFlags); */
NS_IMETHODIMP nsExtensionManager::InstallExtension(nsIFile *aXPIFile, PRUint32 aFlags)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void installTheme (in nsIFile aJARFile, in unsigned long aFlags); */
NS_IMETHODIMP nsExtensionManager::InstallTheme(nsIFile *aJARFile, PRUint32 aFlags)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIExtensionManager_h__ */
