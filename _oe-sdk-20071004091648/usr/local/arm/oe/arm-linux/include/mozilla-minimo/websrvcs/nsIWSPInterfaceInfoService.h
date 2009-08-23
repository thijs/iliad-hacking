/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIWSPInterfaceInfoService.idl
 */

#ifndef __gen_nsIWSPInterfaceInfoService_h__
#define __gen_nsIWSPInterfaceInfoService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIInterfaceInfo_h__
#include "nsIInterfaceInfo.h"
#endif

#ifndef __gen_nsIInterfaceInfoManager_h__
#include "nsIInterfaceInfoManager.h"
#endif

#ifndef __gen_nsIWSDL_h__
#include "nsIWSDL.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIWSPInterfaceInfoService */
#define NS_IWSPINTERFACEINFOSERVICE_IID_STR "8c7bf4bc-d3fe-4524-b213-ce578bfe936e"

#define NS_IWSPINTERFACEINFOSERVICE_IID \
  {0x8c7bf4bc, 0xd3fe, 0x4524, \
    { 0xb2, 0x13, 0xce, 0x57, 0x8b, 0xfe, 0x93, 0x6e }}

class NS_NO_VTABLE nsIWSPInterfaceInfoService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWSPINTERFACEINFOSERVICE_IID)

  /* nsIInterfaceInfo infoForPort (in nsIWSDLPort aPort, in AString aPortURL, in AString aQualifier, in PRBool aIsAsync, out nsIInterfaceInfoManager aSet); */
  NS_IMETHOD InfoForPort(nsIWSDLPort *aPort, const nsAString & aPortURL, const nsAString & aQualifier, PRBool aIsAsync, nsIInterfaceInfoManager **aSet, nsIInterfaceInfo **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWSPINTERFACEINFOSERVICE \
  NS_IMETHOD InfoForPort(nsIWSDLPort *aPort, const nsAString & aPortURL, const nsAString & aQualifier, PRBool aIsAsync, nsIInterfaceInfoManager **aSet, nsIInterfaceInfo **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWSPINTERFACEINFOSERVICE(_to) \
  NS_IMETHOD InfoForPort(nsIWSDLPort *aPort, const nsAString & aPortURL, const nsAString & aQualifier, PRBool aIsAsync, nsIInterfaceInfoManager **aSet, nsIInterfaceInfo **_retval) { return _to InfoForPort(aPort, aPortURL, aQualifier, aIsAsync, aSet, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWSPINTERFACEINFOSERVICE(_to) \
  NS_IMETHOD InfoForPort(nsIWSDLPort *aPort, const nsAString & aPortURL, const nsAString & aQualifier, PRBool aIsAsync, nsIInterfaceInfoManager **aSet, nsIInterfaceInfo **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->InfoForPort(aPort, aPortURL, aQualifier, aIsAsync, aSet, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWSPInterfaceInfoService : public nsIWSPInterfaceInfoService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWSPINTERFACEINFOSERVICE

  nsWSPInterfaceInfoService();

private:
  ~nsWSPInterfaceInfoService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWSPInterfaceInfoService, nsIWSPInterfaceInfoService)

nsWSPInterfaceInfoService::nsWSPInterfaceInfoService()
{
  /* member initializers and constructor code */
}

nsWSPInterfaceInfoService::~nsWSPInterfaceInfoService()
{
  /* destructor code */
}

/* nsIInterfaceInfo infoForPort (in nsIWSDLPort aPort, in AString aPortURL, in AString aQualifier, in PRBool aIsAsync, out nsIInterfaceInfoManager aSet); */
NS_IMETHODIMP nsWSPInterfaceInfoService::InfoForPort(nsIWSDLPort *aPort, const nsAString & aPortURL, const nsAString & aQualifier, PRBool aIsAsync, nsIInterfaceInfoManager **aSet, nsIInterfaceInfo **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_WSP_INTERFACEINFOSERVICE_CONTRACTID "@mozilla.org/xmlextras/proxy/interfaceinfoservice;1"

#endif /* __gen_nsIWSPInterfaceInfoService_h__ */
