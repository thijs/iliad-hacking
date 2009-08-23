/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/modules/libjar/nsIJARURI.idl
 */

#ifndef __gen_nsIJARURI_h__
#define __gen_nsIJARURI_h__


#ifndef __gen_nsIURL_h__
#include "nsIURL.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIJARURI */
#define NS_IJARURI_IID_STR "c7e410d3-85f2-11d3-9f63-006008a6efe9"

#define NS_IJARURI_IID \
  {0xc7e410d3, 0x85f2, 0x11d3, \
    { 0x9f, 0x63, 0x00, 0x60, 0x08, 0xa6, 0xef, 0xe9 }}

/**
 * JAR URLs have the following syntax
 *
 * jar:<jar-file-uri>!/<jar-entry>
 *
 * EXAMPLE: jar:http://www.big.com/blue.jar!/ocean.html
 *
 * The nsIURL methods operate on the <jar-entry> part of the spec.
 */
class NS_NO_VTABLE nsIJARURI : public nsIURL {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IJARURI_IID)

  /**
     * Returns the root URI (the one for the actual JAR file) for this JAR.
     * eg http://www.big.com/blue.jar
     */
  /* attribute nsIURI JARFile; */
  NS_IMETHOD GetJARFile(nsIURI * *aJARFile) = 0;
  NS_IMETHOD SetJARFile(nsIURI * aJARFile) = 0;

  /**
     * Returns the entry specified for this JAR URI.
     * eg ocean.html
     */
  /* attribute AUTF8String JAREntry; */
  NS_IMETHOD GetJAREntry(nsACString & aJAREntry) = 0;
  NS_IMETHOD SetJAREntry(const nsACString & aJAREntry) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIJARURI \
  NS_IMETHOD GetJARFile(nsIURI * *aJARFile); \
  NS_IMETHOD SetJARFile(nsIURI * aJARFile); \
  NS_IMETHOD GetJAREntry(nsACString & aJAREntry); \
  NS_IMETHOD SetJAREntry(const nsACString & aJAREntry); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIJARURI(_to) \
  NS_IMETHOD GetJARFile(nsIURI * *aJARFile) { return _to GetJARFile(aJARFile); } \
  NS_IMETHOD SetJARFile(nsIURI * aJARFile) { return _to SetJARFile(aJARFile); } \
  NS_IMETHOD GetJAREntry(nsACString & aJAREntry) { return _to GetJAREntry(aJAREntry); } \
  NS_IMETHOD SetJAREntry(const nsACString & aJAREntry) { return _to SetJAREntry(aJAREntry); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIJARURI(_to) \
  NS_IMETHOD GetJARFile(nsIURI * *aJARFile) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetJARFile(aJARFile); } \
  NS_IMETHOD SetJARFile(nsIURI * aJARFile) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetJARFile(aJARFile); } \
  NS_IMETHOD GetJAREntry(nsACString & aJAREntry) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetJAREntry(aJAREntry); } \
  NS_IMETHOD SetJAREntry(const nsACString & aJAREntry) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetJAREntry(aJAREntry); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsJARURI : public nsIJARURI
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIJARURI

  nsJARURI();

private:
  ~nsJARURI();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsJARURI, nsIJARURI)

nsJARURI::nsJARURI()
{
  /* member initializers and constructor code */
}

nsJARURI::~nsJARURI()
{
  /* destructor code */
}

/* attribute nsIURI JARFile; */
NS_IMETHODIMP nsJARURI::GetJARFile(nsIURI * *aJARFile)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsJARURI::SetJARFile(nsIURI * aJARFile)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute AUTF8String JAREntry; */
NS_IMETHODIMP nsJARURI::GetJAREntry(nsACString & aJAREntry)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsJARURI::SetJAREntry(const nsACString & aJAREntry)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIJARURI_h__ */
