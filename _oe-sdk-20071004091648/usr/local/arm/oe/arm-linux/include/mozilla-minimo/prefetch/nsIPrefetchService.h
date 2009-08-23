/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/uriloader/prefetch/nsIPrefetchService.idl
 */

#ifndef __gen_nsIPrefetchService_h__
#define __gen_nsIPrefetchService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIURI; /* forward declaration */


/* starting interface:    nsIPrefetchService */
#define NS_IPREFETCHSERVICE_IID_STR "933cb52a-2864-4a40-8678-a2d0851b0ef4"

#define NS_IPREFETCHSERVICE_IID \
  {0x933cb52a, 0x2864, 0x4a40, \
    { 0x86, 0x78, 0xa2, 0xd0, 0x85, 0x1b, 0x0e, 0xf4 }}

class NS_NO_VTABLE nsIPrefetchService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IPREFETCHSERVICE_IID)

  /**
     * Enqueue a request to prefetch the specified URI.
     *
     * @param aURI the URI of the document to prefetch
     * @param aReferrerURI the URI of the referring page
     * @param aExplicit the link element has an explicit prefetch link type
     */
  /* void prefetchURI (in nsIURI aURI, in nsIURI aReferrerURI, in boolean aExplicit); */
  NS_IMETHOD PrefetchURI(nsIURI *aURI, nsIURI *aReferrerURI, PRBool aExplicit) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPREFETCHSERVICE \
  NS_IMETHOD PrefetchURI(nsIURI *aURI, nsIURI *aReferrerURI, PRBool aExplicit); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPREFETCHSERVICE(_to) \
  NS_IMETHOD PrefetchURI(nsIURI *aURI, nsIURI *aReferrerURI, PRBool aExplicit) { return _to PrefetchURI(aURI, aReferrerURI, aExplicit); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPREFETCHSERVICE(_to) \
  NS_IMETHOD PrefetchURI(nsIURI *aURI, nsIURI *aReferrerURI, PRBool aExplicit) { return !_to ? NS_ERROR_NULL_POINTER : _to->PrefetchURI(aURI, aReferrerURI, aExplicit); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsPrefetchService : public nsIPrefetchService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPREFETCHSERVICE

  nsPrefetchService();

private:
  ~nsPrefetchService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsPrefetchService, nsIPrefetchService)

nsPrefetchService::nsPrefetchService()
{
  /* member initializers and constructor code */
}

nsPrefetchService::~nsPrefetchService()
{
  /* destructor code */
}

/* void prefetchURI (in nsIURI aURI, in nsIURI aReferrerURI, in boolean aExplicit); */
NS_IMETHODIMP nsPrefetchService::PrefetchURI(nsIURI *aURI, nsIURI *aReferrerURI, PRBool aExplicit)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIPrefetchService_h__ */
