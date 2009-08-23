/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/security/manager/ssl/public/nsICertTree.idl
 */

#ifndef __gen_nsICertTree_h__
#define __gen_nsICertTree_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsITreeView_h__
#include "nsITreeView.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsINSSCertCache; /* forward declaration */

class nsIX509Cert; /* forward declaration */


/* starting interface:    nsICertTree */
#define NS_ICERTTREE_IID_STR "4ea60761-31d6-491d-9e34-4b53a26c416c"

#define NS_ICERTTREE_IID \
  {0x4ea60761, 0x31d6, 0x491d, \
    { 0x9e, 0x34, 0x4b, 0x53, 0xa2, 0x6c, 0x41, 0x6c }}

class NS_NO_VTABLE nsICertTree : public nsITreeView {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICERTTREE_IID)

  /* void loadCerts (in unsigned long type); */
  NS_IMETHOD LoadCerts(PRUint32 type) = 0;

  /* void loadCertsFromCache (in nsINSSCertCache cache, in unsigned long type); */
  NS_IMETHOD LoadCertsFromCache(nsINSSCertCache *cache, PRUint32 type) = 0;

  /* nsIX509Cert getCert (in unsigned long index); */
  NS_IMETHOD GetCert(PRUint32 index, nsIX509Cert **_retval) = 0;

  /* void removeCert (in unsigned long index); */
  NS_IMETHOD RemoveCert(PRUint32 index) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICERTTREE \
  NS_IMETHOD LoadCerts(PRUint32 type); \
  NS_IMETHOD LoadCertsFromCache(nsINSSCertCache *cache, PRUint32 type); \
  NS_IMETHOD GetCert(PRUint32 index, nsIX509Cert **_retval); \
  NS_IMETHOD RemoveCert(PRUint32 index); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICERTTREE(_to) \
  NS_IMETHOD LoadCerts(PRUint32 type) { return _to LoadCerts(type); } \
  NS_IMETHOD LoadCertsFromCache(nsINSSCertCache *cache, PRUint32 type) { return _to LoadCertsFromCache(cache, type); } \
  NS_IMETHOD GetCert(PRUint32 index, nsIX509Cert **_retval) { return _to GetCert(index, _retval); } \
  NS_IMETHOD RemoveCert(PRUint32 index) { return _to RemoveCert(index); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICERTTREE(_to) \
  NS_IMETHOD LoadCerts(PRUint32 type) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoadCerts(type); } \
  NS_IMETHOD LoadCertsFromCache(nsINSSCertCache *cache, PRUint32 type) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoadCertsFromCache(cache, type); } \
  NS_IMETHOD GetCert(PRUint32 index, nsIX509Cert **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCert(index, _retval); } \
  NS_IMETHOD RemoveCert(PRUint32 index) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveCert(index); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsCertTree : public nsICertTree
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICERTTREE

  nsCertTree();

private:
  ~nsCertTree();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsCertTree, nsICertTree)

nsCertTree::nsCertTree()
{
  /* member initializers and constructor code */
}

nsCertTree::~nsCertTree()
{
  /* destructor code */
}

/* void loadCerts (in unsigned long type); */
NS_IMETHODIMP nsCertTree::LoadCerts(PRUint32 type)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void loadCertsFromCache (in nsINSSCertCache cache, in unsigned long type); */
NS_IMETHODIMP nsCertTree::LoadCertsFromCache(nsINSSCertCache *cache, PRUint32 type)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIX509Cert getCert (in unsigned long index); */
NS_IMETHODIMP nsCertTree::GetCert(PRUint32 index, nsIX509Cert **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeCert (in unsigned long index); */
NS_IMETHODIMP nsCertTree::RemoveCert(PRUint32 index)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_CERTTREE_CID { 0x4ea60761, 0x31d6, 0x491d, \
                         { 0x9e, 0x34, 0x4b, 0x53, 0xa2, 0x6c, 0x41, 0x6c } }
#define NS_CERTTREE_CONTRACTID "@mozilla.org/security/nsCertTree;1"

#endif /* __gen_nsICertTree_h__ */
