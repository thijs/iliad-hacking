/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/autocomplete/public/nsIAutoCompleteResults.idl
 */

#ifndef __gen_nsIAutoCompleteResults_h__
#define __gen_nsIAutoCompleteResults_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsISupportsArray_h__
#include "nsISupportsArray.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIAutoCompleteItem */
#define NS_IAUTOCOMPLETEITEM_IID_STR "88dcfa80-062f-11d4-a449-b36a1a94c0fc"

#define NS_IAUTOCOMPLETEITEM_IID \
  {0x88dcfa80, 0x062f, 0x11d4, \
    { 0xa4, 0x49, 0xb3, 0x6a, 0x1a, 0x94, 0xc0, 0xfc }}

class NS_NO_VTABLE nsIAutoCompleteItem : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAUTOCOMPLETEITEM_IID)

  /**
     * the result value
     * using AString to avoid excess allocations
     */
  /* attribute AString value; */
  NS_IMETHOD GetValue(nsAString & aValue) = 0;
  NS_IMETHOD SetValue(const nsAString & aValue) = 0;

  /**
     * an extra comment that will be
     * displayed next to the value but that
     * will not be part of the value
     * itself
     */
  /* attribute wstring comment; */
  NS_IMETHOD GetComment(PRUnichar * *aComment) = 0;
  NS_IMETHOD SetComment(const PRUnichar * aComment) = 0;

  /**
     * class name used to define some style through
     * css like the colors, an icon url, etc...
     */
  /* attribute string className; */
  NS_IMETHOD GetClassName(char * *aClassName) = 0;
  NS_IMETHOD SetClassName(const char * aClassName) = 0;

  /**
     * parameter use by the search engine
     */
  /* attribute nsISupports param; */
  NS_IMETHOD GetParam(nsISupports * *aParam) = 0;
  NS_IMETHOD SetParam(nsISupports * aParam) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUTOCOMPLETEITEM \
  NS_IMETHOD GetValue(nsAString & aValue); \
  NS_IMETHOD SetValue(const nsAString & aValue); \
  NS_IMETHOD GetComment(PRUnichar * *aComment); \
  NS_IMETHOD SetComment(const PRUnichar * aComment); \
  NS_IMETHOD GetClassName(char * *aClassName); \
  NS_IMETHOD SetClassName(const char * aClassName); \
  NS_IMETHOD GetParam(nsISupports * *aParam); \
  NS_IMETHOD SetParam(nsISupports * aParam); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUTOCOMPLETEITEM(_to) \
  NS_IMETHOD GetValue(nsAString & aValue) { return _to GetValue(aValue); } \
  NS_IMETHOD SetValue(const nsAString & aValue) { return _to SetValue(aValue); } \
  NS_IMETHOD GetComment(PRUnichar * *aComment) { return _to GetComment(aComment); } \
  NS_IMETHOD SetComment(const PRUnichar * aComment) { return _to SetComment(aComment); } \
  NS_IMETHOD GetClassName(char * *aClassName) { return _to GetClassName(aClassName); } \
  NS_IMETHOD SetClassName(const char * aClassName) { return _to SetClassName(aClassName); } \
  NS_IMETHOD GetParam(nsISupports * *aParam) { return _to GetParam(aParam); } \
  NS_IMETHOD SetParam(nsISupports * aParam) { return _to SetParam(aParam); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUTOCOMPLETEITEM(_to) \
  NS_IMETHOD GetValue(nsAString & aValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetValue(aValue); } \
  NS_IMETHOD SetValue(const nsAString & aValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetValue(aValue); } \
  NS_IMETHOD GetComment(PRUnichar * *aComment) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetComment(aComment); } \
  NS_IMETHOD SetComment(const PRUnichar * aComment) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetComment(aComment); } \
  NS_IMETHOD GetClassName(char * *aClassName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetClassName(aClassName); } \
  NS_IMETHOD SetClassName(const char * aClassName) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetClassName(aClassName); } \
  NS_IMETHOD GetParam(nsISupports * *aParam) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParam(aParam); } \
  NS_IMETHOD SetParam(nsISupports * aParam) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetParam(aParam); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAutoCompleteItem : public nsIAutoCompleteItem
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAUTOCOMPLETEITEM

  nsAutoCompleteItem();

private:
  ~nsAutoCompleteItem();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAutoCompleteItem, nsIAutoCompleteItem)

nsAutoCompleteItem::nsAutoCompleteItem()
{
  /* member initializers and constructor code */
}

nsAutoCompleteItem::~nsAutoCompleteItem()
{
  /* destructor code */
}

/* attribute AString value; */
NS_IMETHODIMP nsAutoCompleteItem::GetValue(nsAString & aValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteItem::SetValue(const nsAString & aValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute wstring comment; */
NS_IMETHODIMP nsAutoCompleteItem::GetComment(PRUnichar * *aComment)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteItem::SetComment(const PRUnichar * aComment)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute string className; */
NS_IMETHODIMP nsAutoCompleteItem::GetClassName(char * *aClassName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteItem::SetClassName(const char * aClassName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISupports param; */
NS_IMETHODIMP nsAutoCompleteItem::GetParam(nsISupports * *aParam)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteItem::SetParam(nsISupports * aParam)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIAutoCompleteResults */
#define NS_IAUTOCOMPLETERESULTS_IID_STR "88dcfa81-062f-11d4-a449-b36a1a94c0fc"

#define NS_IAUTOCOMPLETERESULTS_IID \
  {0x88dcfa81, 0x062f, 0x11d4, \
    { 0xa4, 0x49, 0xb3, 0x6a, 0x1a, 0x94, 0xc0, 0xfc }}

class NS_NO_VTABLE nsIAutoCompleteResults : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAUTOCOMPLETERESULTS_IID)

  /**
     * the original search string
     */
  /* attribute wstring searchString; */
  NS_IMETHOD GetSearchString(PRUnichar * *aSearchString) = 0;
  NS_IMETHOD SetSearchString(const PRUnichar * aSearchString) = 0;

  /**
     * Array of result items (nsIAutoCompleteItem)
     */
  /* attribute nsISupportsArray items; */
  NS_IMETHOD GetItems(nsISupportsArray * *aItems) = 0;
  NS_IMETHOD SetItems(nsISupportsArray * aItems) = 0;

  /**
     * Index (0 base) of the default item that
     * will be preselected and displayed
     */
  /* attribute long defaultItemIndex; */
  NS_IMETHOD GetDefaultItemIndex(PRInt32 *aDefaultItemIndex) = 0;
  NS_IMETHOD SetDefaultItemIndex(PRInt32 aDefaultItemIndex) = 0;

  /**
     * param use by the the search engine
     */
  /* attribute nsISupports param; */
  NS_IMETHOD GetParam(nsISupports * *aParam) = 0;
  NS_IMETHOD SetParam(nsISupports * aParam) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUTOCOMPLETERESULTS \
  NS_IMETHOD GetSearchString(PRUnichar * *aSearchString); \
  NS_IMETHOD SetSearchString(const PRUnichar * aSearchString); \
  NS_IMETHOD GetItems(nsISupportsArray * *aItems); \
  NS_IMETHOD SetItems(nsISupportsArray * aItems); \
  NS_IMETHOD GetDefaultItemIndex(PRInt32 *aDefaultItemIndex); \
  NS_IMETHOD SetDefaultItemIndex(PRInt32 aDefaultItemIndex); \
  NS_IMETHOD GetParam(nsISupports * *aParam); \
  NS_IMETHOD SetParam(nsISupports * aParam); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUTOCOMPLETERESULTS(_to) \
  NS_IMETHOD GetSearchString(PRUnichar * *aSearchString) { return _to GetSearchString(aSearchString); } \
  NS_IMETHOD SetSearchString(const PRUnichar * aSearchString) { return _to SetSearchString(aSearchString); } \
  NS_IMETHOD GetItems(nsISupportsArray * *aItems) { return _to GetItems(aItems); } \
  NS_IMETHOD SetItems(nsISupportsArray * aItems) { return _to SetItems(aItems); } \
  NS_IMETHOD GetDefaultItemIndex(PRInt32 *aDefaultItemIndex) { return _to GetDefaultItemIndex(aDefaultItemIndex); } \
  NS_IMETHOD SetDefaultItemIndex(PRInt32 aDefaultItemIndex) { return _to SetDefaultItemIndex(aDefaultItemIndex); } \
  NS_IMETHOD GetParam(nsISupports * *aParam) { return _to GetParam(aParam); } \
  NS_IMETHOD SetParam(nsISupports * aParam) { return _to SetParam(aParam); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUTOCOMPLETERESULTS(_to) \
  NS_IMETHOD GetSearchString(PRUnichar * *aSearchString) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSearchString(aSearchString); } \
  NS_IMETHOD SetSearchString(const PRUnichar * aSearchString) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSearchString(aSearchString); } \
  NS_IMETHOD GetItems(nsISupportsArray * *aItems) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetItems(aItems); } \
  NS_IMETHOD SetItems(nsISupportsArray * aItems) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetItems(aItems); } \
  NS_IMETHOD GetDefaultItemIndex(PRInt32 *aDefaultItemIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultItemIndex(aDefaultItemIndex); } \
  NS_IMETHOD SetDefaultItemIndex(PRInt32 aDefaultItemIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetDefaultItemIndex(aDefaultItemIndex); } \
  NS_IMETHOD GetParam(nsISupports * *aParam) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParam(aParam); } \
  NS_IMETHOD SetParam(nsISupports * aParam) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetParam(aParam); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAutoCompleteResults : public nsIAutoCompleteResults
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAUTOCOMPLETERESULTS

  nsAutoCompleteResults();

private:
  ~nsAutoCompleteResults();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAutoCompleteResults, nsIAutoCompleteResults)

nsAutoCompleteResults::nsAutoCompleteResults()
{
  /* member initializers and constructor code */
}

nsAutoCompleteResults::~nsAutoCompleteResults()
{
  /* destructor code */
}

/* attribute wstring searchString; */
NS_IMETHODIMP nsAutoCompleteResults::GetSearchString(PRUnichar * *aSearchString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteResults::SetSearchString(const PRUnichar * aSearchString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISupportsArray items; */
NS_IMETHODIMP nsAutoCompleteResults::GetItems(nsISupportsArray * *aItems)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteResults::SetItems(nsISupportsArray * aItems)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute long defaultItemIndex; */
NS_IMETHODIMP nsAutoCompleteResults::GetDefaultItemIndex(PRInt32 *aDefaultItemIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteResults::SetDefaultItemIndex(PRInt32 aDefaultItemIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISupports param; */
NS_IMETHODIMP nsAutoCompleteResults::GetParam(nsISupports * *aParam)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsAutoCompleteResults::SetParam(nsISupports * aParam)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

// {FE4FC780-063D-11d4-A449-D45A120774BC}
#define NS_AUTOCOMPLETERESULTS_CID \
{ 0xfe4fc780, 0x063d, 0x11d4, { 0xa4, 0x49, 0xd4, 0x5a, 0x12, 0x07, 0x74, 0xbc } }
#define NS_AUTOCOMPLETERESULTS_CONTRACTID  "@mozilla.org/autocomplete/results;1"
// {FE4FC781-063D-11d4-A449-D45A120774BC}
#define NS_AUTOCOMPLETEITEM_CID \
{ 0xfe4fc781, 0x063d, 0x11d4, { 0xa4, 0x49, 0xd4, 0x5a, 0x12, 0x07, 0x74, 0xbc } }
#define NS_AUTOCOMPLETEITEM_CONTRACTID  "@mozilla.org/autocomplete/item;1"

#endif /* __gen_nsIAutoCompleteResults_h__ */
