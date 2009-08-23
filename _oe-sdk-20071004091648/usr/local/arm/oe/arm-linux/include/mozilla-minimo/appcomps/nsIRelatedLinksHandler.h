/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/related/public/nsIRelatedLinksHandler.idl
 */

#ifndef __gen_nsIRelatedLinksHandler_h__
#define __gen_nsIRelatedLinksHandler_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIRelatedLinksHandler */
#define NS_IRELATEDLINKSHANDLER_IID_STR "8bb4fb60-1cac-11d3-bdf9-000064657374"

#define NS_IRELATEDLINKSHANDLER_IID \
  {0x8bb4fb60, 0x1cac, 0x11d3, \
    { 0xbd, 0xf9, 0x00, 0x00, 0x64, 0x65, 0x73, 0x74 }}

class NS_NO_VTABLE nsIRelatedLinksHandler : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IRELATEDLINKSHANDLER_IID)

  /* attribute string URL; */
  NS_IMETHOD GetURL(char * *aURL) = 0;
  NS_IMETHOD SetURL(const char * aURL) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIRELATEDLINKSHANDLER \
  NS_IMETHOD GetURL(char * *aURL); \
  NS_IMETHOD SetURL(const char * aURL); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIRELATEDLINKSHANDLER(_to) \
  NS_IMETHOD GetURL(char * *aURL) { return _to GetURL(aURL); } \
  NS_IMETHOD SetURL(const char * aURL) { return _to SetURL(aURL); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIRELATEDLINKSHANDLER(_to) \
  NS_IMETHOD GetURL(char * *aURL) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetURL(aURL); } \
  NS_IMETHOD SetURL(const char * aURL) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetURL(aURL); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsRelatedLinksHandler : public nsIRelatedLinksHandler
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIRELATEDLINKSHANDLER

  nsRelatedLinksHandler();

private:
  ~nsRelatedLinksHandler();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsRelatedLinksHandler, nsIRelatedLinksHandler)

nsRelatedLinksHandler::nsRelatedLinksHandler()
{
  /* member initializers and constructor code */
}

nsRelatedLinksHandler::~nsRelatedLinksHandler()
{
  /* destructor code */
}

/* attribute string URL; */
NS_IMETHODIMP nsRelatedLinksHandler::GetURL(char * *aURL)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsRelatedLinksHandler::SetURL(const char * aURL)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_RELATEDLINKSHANDLER_CONTRACTID "@mozilla.org/related-links-handler;1"
// {8BB4FB61-1CAC-11d3-BDF9-000064657374}
#define NS_RELATEDLINKSHANDLER_CID \
{ 0x8bb4fb61, 0x1cac, 0x11d3, { 0xbd, 0xf9, 0x0, 0x0, 0x64, 0x65, 0x73, 0x74 } }

#endif /* __gen_nsIRelatedLinksHandler_h__ */
