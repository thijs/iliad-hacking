/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/autocomplete/public/nsIAutoCompleteResult.idl
 */

#ifndef __gen_nsIAutoCompleteResult_h__
#define __gen_nsIAutoCompleteResult_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIAutoCompleteResult */
#define NS_IAUTOCOMPLETERESULT_IID_STR "eb43e1dc-2060-4d8e-aebf-3efec4e21cf8"

#define NS_IAUTOCOMPLETERESULT_IID \
  {0xeb43e1dc, 0x2060, 0x4d8e, \
    { 0xae, 0xbf, 0x3e, 0xfe, 0xc4, 0xe2, 0x1c, 0xf8 }}

class NS_NO_VTABLE nsIAutoCompleteResult : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IAUTOCOMPLETERESULT_IID)

  /**
   * Possible values for the searchResult attribute
   */
  enum { RESULT_IGNORED = 1U };

  enum { RESULT_FAILURE = 2U };

  enum { RESULT_NOMATCH = 3U };

  enum { RESULT_SUCCESS = 4U };

  /**
   * The original search string
   */
  /* readonly attribute AString searchString; */
  NS_IMETHOD GetSearchString(nsAString & aSearchString) = 0;

  /**
   * The result of the search
   */
  /* readonly attribute unsigned short searchResult; */
  NS_IMETHOD GetSearchResult(PRUint16 *aSearchResult) = 0;

  /**
   * Index of the default item that should be entered if none is selected
   */
  /* readonly attribute long defaultIndex; */
  NS_IMETHOD GetDefaultIndex(PRInt32 *aDefaultIndex) = 0;

  /**
   * A string describing the cause of a search failure
   */
  /* readonly attribute AString errorDescription; */
  NS_IMETHOD GetErrorDescription(nsAString & aErrorDescription) = 0;

  /**
   * The number of matches
   */
  /* readonly attribute unsigned long matchCount; */
  NS_IMETHOD GetMatchCount(PRUint32 *aMatchCount) = 0;

  /**
   * Get the value of the result at the given index
   */
  /* AString getValueAt (in long index); */
  NS_IMETHOD GetValueAt(PRInt32 index, nsAString & _retval) = 0;

  /**
   * Get the comment of the result at the given index
   */
  /* AString getCommentAt (in long index); */
  NS_IMETHOD GetCommentAt(PRInt32 index, nsAString & _retval) = 0;

  /**
   * Get the style hint for the result at the given index
   */
  /* AString getStyleAt (in long index); */
  NS_IMETHOD GetStyleAt(PRInt32 index, nsAString & _retval) = 0;

  /**
   * Remove the value at the given index from the autocomplete results.
   * If removeFromDb is set to true, the value should be removed from
   * persistent storage as well.
   */
  /* void removeValueAt (in long rowIndex, in boolean removeFromDb); */
  NS_IMETHOD RemoveValueAt(PRInt32 rowIndex, PRBool removeFromDb) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIAUTOCOMPLETERESULT \
  NS_IMETHOD GetSearchString(nsAString & aSearchString); \
  NS_IMETHOD GetSearchResult(PRUint16 *aSearchResult); \
  NS_IMETHOD GetDefaultIndex(PRInt32 *aDefaultIndex); \
  NS_IMETHOD GetErrorDescription(nsAString & aErrorDescription); \
  NS_IMETHOD GetMatchCount(PRUint32 *aMatchCount); \
  NS_IMETHOD GetValueAt(PRInt32 index, nsAString & _retval); \
  NS_IMETHOD GetCommentAt(PRInt32 index, nsAString & _retval); \
  NS_IMETHOD GetStyleAt(PRInt32 index, nsAString & _retval); \
  NS_IMETHOD RemoveValueAt(PRInt32 rowIndex, PRBool removeFromDb); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIAUTOCOMPLETERESULT(_to) \
  NS_IMETHOD GetSearchString(nsAString & aSearchString) { return _to GetSearchString(aSearchString); } \
  NS_IMETHOD GetSearchResult(PRUint16 *aSearchResult) { return _to GetSearchResult(aSearchResult); } \
  NS_IMETHOD GetDefaultIndex(PRInt32 *aDefaultIndex) { return _to GetDefaultIndex(aDefaultIndex); } \
  NS_IMETHOD GetErrorDescription(nsAString & aErrorDescription) { return _to GetErrorDescription(aErrorDescription); } \
  NS_IMETHOD GetMatchCount(PRUint32 *aMatchCount) { return _to GetMatchCount(aMatchCount); } \
  NS_IMETHOD GetValueAt(PRInt32 index, nsAString & _retval) { return _to GetValueAt(index, _retval); } \
  NS_IMETHOD GetCommentAt(PRInt32 index, nsAString & _retval) { return _to GetCommentAt(index, _retval); } \
  NS_IMETHOD GetStyleAt(PRInt32 index, nsAString & _retval) { return _to GetStyleAt(index, _retval); } \
  NS_IMETHOD RemoveValueAt(PRInt32 rowIndex, PRBool removeFromDb) { return _to RemoveValueAt(rowIndex, removeFromDb); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIAUTOCOMPLETERESULT(_to) \
  NS_IMETHOD GetSearchString(nsAString & aSearchString) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSearchString(aSearchString); } \
  NS_IMETHOD GetSearchResult(PRUint16 *aSearchResult) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSearchResult(aSearchResult); } \
  NS_IMETHOD GetDefaultIndex(PRInt32 *aDefaultIndex) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultIndex(aDefaultIndex); } \
  NS_IMETHOD GetErrorDescription(nsAString & aErrorDescription) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetErrorDescription(aErrorDescription); } \
  NS_IMETHOD GetMatchCount(PRUint32 *aMatchCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMatchCount(aMatchCount); } \
  NS_IMETHOD GetValueAt(PRInt32 index, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetValueAt(index, _retval); } \
  NS_IMETHOD GetCommentAt(PRInt32 index, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCommentAt(index, _retval); } \
  NS_IMETHOD GetStyleAt(PRInt32 index, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStyleAt(index, _retval); } \
  NS_IMETHOD RemoveValueAt(PRInt32 rowIndex, PRBool removeFromDb) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveValueAt(rowIndex, removeFromDb); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsAutoCompleteResult : public nsIAutoCompleteResult
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIAUTOCOMPLETERESULT

  nsAutoCompleteResult();

private:
  ~nsAutoCompleteResult();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsAutoCompleteResult, nsIAutoCompleteResult)

nsAutoCompleteResult::nsAutoCompleteResult()
{
  /* member initializers and constructor code */
}

nsAutoCompleteResult::~nsAutoCompleteResult()
{
  /* destructor code */
}

/* readonly attribute AString searchString; */
NS_IMETHODIMP nsAutoCompleteResult::GetSearchString(nsAString & aSearchString)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short searchResult; */
NS_IMETHODIMP nsAutoCompleteResult::GetSearchResult(PRUint16 *aSearchResult)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute long defaultIndex; */
NS_IMETHODIMP nsAutoCompleteResult::GetDefaultIndex(PRInt32 *aDefaultIndex)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString errorDescription; */
NS_IMETHODIMP nsAutoCompleteResult::GetErrorDescription(nsAString & aErrorDescription)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long matchCount; */
NS_IMETHODIMP nsAutoCompleteResult::GetMatchCount(PRUint32 *aMatchCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getValueAt (in long index); */
NS_IMETHODIMP nsAutoCompleteResult::GetValueAt(PRInt32 index, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getCommentAt (in long index); */
NS_IMETHODIMP nsAutoCompleteResult::GetCommentAt(PRInt32 index, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getStyleAt (in long index); */
NS_IMETHODIMP nsAutoCompleteResult::GetStyleAt(PRInt32 index, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeValueAt (in long rowIndex, in boolean removeFromDb); */
NS_IMETHODIMP nsAutoCompleteResult::RemoveValueAt(PRInt32 rowIndex, PRBool removeFromDb)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIAutoCompleteResult_h__ */
