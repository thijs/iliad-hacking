/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/security/manager/ssl/public/nsINSSCertCache.idl
 */

#ifndef __gen_nsINSSCertCache_h__
#define __gen_nsINSSCertCache_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsINSSCertCache */
#define NS_INSSCERTCACHE_IID_STR "6c143dac-bd65-4333-b594-7ed1e748e0f9"

#define NS_INSSCERTCACHE_IID \
  {0x6c143dac, 0xbd65, 0x4333, \
    { 0xb5, 0x94, 0x7e, 0xd1, 0xe7, 0x48, 0xe0, 0xf9 }}

class NS_NO_VTABLE nsINSSCertCache : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_INSSCERTCACHE_IID)

  /* void cacheAllCerts (); */
  NS_IMETHOD CacheAllCerts(void) = 0;

  /* [noscript, notxpcom] voidPtr getCachedCerts (); */
  NS_IMETHOD_(void *) GetCachedCerts(void) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSINSSCERTCACHE \
  NS_IMETHOD CacheAllCerts(void); \
  NS_IMETHOD_(void *) GetCachedCerts(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSINSSCERTCACHE(_to) \
  NS_IMETHOD CacheAllCerts(void) { return _to CacheAllCerts(); } \
  NS_IMETHOD_(void *) GetCachedCerts(void) { return _to GetCachedCerts(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSINSSCERTCACHE(_to) \
  NS_IMETHOD CacheAllCerts(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->CacheAllCerts(); } \
  NS_IMETHOD_(void *) GetCachedCerts(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCachedCerts(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsNSSCertCache : public nsINSSCertCache
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSINSSCERTCACHE

  nsNSSCertCache();

private:
  ~nsNSSCertCache();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsNSSCertCache, nsINSSCertCache)

nsNSSCertCache::nsNSSCertCache()
{
  /* member initializers and constructor code */
}

nsNSSCertCache::~nsNSSCertCache()
{
  /* destructor code */
}

/* void cacheAllCerts (); */
NS_IMETHODIMP nsNSSCertCache::CacheAllCerts()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript, notxpcom] voidPtr getCachedCerts (); */
NS_IMETHODIMP_(void *) nsNSSCertCache::GetCachedCerts()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_NSSCERTCACHE_CID { /* 3f429a14-dffe-417d-8cb8-fdf09bacd09e */ \
    0x3f429a14,                                                          \
    0xdffe,                                                              \
    0x417d,                                                              \
    {0x8c, 0xb8, 0xfd, 0xf0, 0x9b, 0xac, 0xd0, 0x9e}                     \
  }
#define NS_NSSCERTCACHE_CONTRACTID "@mozilla.org/security/nsscertcache;1"

#endif /* __gen_nsINSSCertCache_h__ */
