/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/bookmarks/public/nsIBookmarksService.idl
 */

#ifndef __gen_nsIBookmarksService_h__
#define __gen_nsIBookmarksService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIRDFResource; /* forward declaration */

class nsITransactionManager; /* forward declaration */


/* starting interface:    nsIBookmarksService */
#define NS_IBOOKMARKSSERVICE_IID_STR "4342a6ac-1b43-4121-b606-4bdf62de71ff"

#define NS_IBOOKMARKSSERVICE_IID \
  {0x4342a6ac, 0x1b43, 0x4121, \
    { 0xb6, 0x06, 0x4b, 0xdf, 0x62, 0xde, 0x71, 0xff }}

class NS_NO_VTABLE nsIBookmarksService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IBOOKMARKSSERVICE_IID)

  enum { BOOKMARK_DEFAULT_TYPE = 0U };

  enum { BOOKMARK_SEARCH_TYPE = 1U };

  enum { BOOKMARK_FIND_TYPE = 2U };

  enum { SORT_DESCENDING = -1 };

  enum { SORT_ASCENDING = 1 };

  /* boolean readBookmarks (); */
  NS_IMETHOD ReadBookmarks(PRBool *_retval) = 0;

  /* boolean isBookmarked (in string aURL); */
  NS_IMETHOD IsBookmarked(const char *aURL, PRBool *_retval) = 0;

  /* boolean isBookmarkedResource (in nsIRDFResource aSource); */
  NS_IMETHOD IsBookmarkedResource(nsIRDFResource *aSource, PRBool *_retval) = 0;

  /* void addBookmarkImmediately (in wstring aURI, in wstring aTitle, in long bmType, in wstring docCharset); */
  NS_IMETHOD AddBookmarkImmediately(const PRUnichar *aURI, const PRUnichar *aTitle, PRInt32 bmType, const PRUnichar *docCharset) = 0;

  /* nsIRDFResource createFolder (in wstring aName); */
  NS_IMETHOD CreateFolder(const PRUnichar *aName, nsIRDFResource **_retval) = 0;

  /* nsIRDFResource createFolderInContainer (in wstring aName, in nsIRDFResource aParentFolder, in long aIndex); */
  NS_IMETHOD CreateFolderInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval) = 0;

  /* nsIRDFResource createGroup (in wstring aName); */
  NS_IMETHOD CreateGroup(const PRUnichar *aName, nsIRDFResource **_retval) = 0;

  /* nsIRDFResource createGroupInContainer (in wstring aName, in nsIRDFResource aParentFolder, in long aIndex); */
  NS_IMETHOD CreateGroupInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval) = 0;

  /* void sortFolder (in nsIRDFResource aFolder, in nsIRDFResource aProperty, in long aDirection, in boolean aFoldersFirst, in boolean aRecurse); */
  NS_IMETHOD SortFolder(nsIRDFResource *aFolder, nsIRDFResource *aProperty, PRInt32 aDirection, PRBool aFoldersFirst, PRBool aRecurse) = 0;

  /* nsIRDFResource createBookmark (in wstring aName, in wstring aURL, in wstring aShortcutURL, in wstring aDescription, in wstring aDocCharSet); */
  NS_IMETHOD CreateBookmark(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource **_retval) = 0;

  /* nsIRDFResource createBookmarkInContainer (in wstring aName, in wstring aURL, in wstring aShortcutURL, in wstring aDescription, in wstring aDocCharSet, in nsIRDFResource aFolder, in long aIndex); */
  NS_IMETHOD CreateBookmarkInContainer(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource *aFolder, PRInt32 aIndex, nsIRDFResource **_retval) = 0;

  /* nsIRDFResource createSeparator (); */
  NS_IMETHOD CreateSeparator(nsIRDFResource **_retval) = 0;

  /* nsIRDFResource cloneResource (in nsIRDFResource aSource); */
  NS_IMETHOD CloneResource(nsIRDFResource *aSource, nsIRDFResource **_retval) = 0;

  /* void updateBookmarkIcon (in string aURL, in wstring aIconURL); */
  NS_IMETHOD UpdateBookmarkIcon(const char *aURL, const PRUnichar *aIconURL) = 0;

  /* void removeBookmarkIcon (in string aURL, in wstring aIconURL); */
  NS_IMETHOD RemoveBookmarkIcon(const char *aURL, const PRUnichar *aIconURL) = 0;

  /* void updateLastVisitedDate (in string aURL, in wstring docCharset); */
  NS_IMETHOD UpdateLastVisitedDate(const char *aURL, const PRUnichar *docCharset) = 0;

  /* AString getLastCharset (in AUTF8String aURL); */
  NS_IMETHOD GetLastCharset(const nsACString & aURL, nsAString & _retval) = 0;

  /* string resolveKeyword (in wstring aName); */
  NS_IMETHOD ResolveKeyword(const PRUnichar *aName, char **_retval) = 0;

  /* void importSystemBookmarks (in nsIRDFResource aParentFolder); */
  NS_IMETHOD ImportSystemBookmarks(nsIRDFResource *aParentFolder) = 0;

  /* readonly attribute nsITransactionManager transactionManager; */
  NS_IMETHOD GetTransactionManager(nsITransactionManager * *aTransactionManager) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIBOOKMARKSSERVICE \
  NS_IMETHOD ReadBookmarks(PRBool *_retval); \
  NS_IMETHOD IsBookmarked(const char *aURL, PRBool *_retval); \
  NS_IMETHOD IsBookmarkedResource(nsIRDFResource *aSource, PRBool *_retval); \
  NS_IMETHOD AddBookmarkImmediately(const PRUnichar *aURI, const PRUnichar *aTitle, PRInt32 bmType, const PRUnichar *docCharset); \
  NS_IMETHOD CreateFolder(const PRUnichar *aName, nsIRDFResource **_retval); \
  NS_IMETHOD CreateFolderInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval); \
  NS_IMETHOD CreateGroup(const PRUnichar *aName, nsIRDFResource **_retval); \
  NS_IMETHOD CreateGroupInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval); \
  NS_IMETHOD SortFolder(nsIRDFResource *aFolder, nsIRDFResource *aProperty, PRInt32 aDirection, PRBool aFoldersFirst, PRBool aRecurse); \
  NS_IMETHOD CreateBookmark(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource **_retval); \
  NS_IMETHOD CreateBookmarkInContainer(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource *aFolder, PRInt32 aIndex, nsIRDFResource **_retval); \
  NS_IMETHOD CreateSeparator(nsIRDFResource **_retval); \
  NS_IMETHOD CloneResource(nsIRDFResource *aSource, nsIRDFResource **_retval); \
  NS_IMETHOD UpdateBookmarkIcon(const char *aURL, const PRUnichar *aIconURL); \
  NS_IMETHOD RemoveBookmarkIcon(const char *aURL, const PRUnichar *aIconURL); \
  NS_IMETHOD UpdateLastVisitedDate(const char *aURL, const PRUnichar *docCharset); \
  NS_IMETHOD GetLastCharset(const nsACString & aURL, nsAString & _retval); \
  NS_IMETHOD ResolveKeyword(const PRUnichar *aName, char **_retval); \
  NS_IMETHOD ImportSystemBookmarks(nsIRDFResource *aParentFolder); \
  NS_IMETHOD GetTransactionManager(nsITransactionManager * *aTransactionManager); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIBOOKMARKSSERVICE(_to) \
  NS_IMETHOD ReadBookmarks(PRBool *_retval) { return _to ReadBookmarks(_retval); } \
  NS_IMETHOD IsBookmarked(const char *aURL, PRBool *_retval) { return _to IsBookmarked(aURL, _retval); } \
  NS_IMETHOD IsBookmarkedResource(nsIRDFResource *aSource, PRBool *_retval) { return _to IsBookmarkedResource(aSource, _retval); } \
  NS_IMETHOD AddBookmarkImmediately(const PRUnichar *aURI, const PRUnichar *aTitle, PRInt32 bmType, const PRUnichar *docCharset) { return _to AddBookmarkImmediately(aURI, aTitle, bmType, docCharset); } \
  NS_IMETHOD CreateFolder(const PRUnichar *aName, nsIRDFResource **_retval) { return _to CreateFolder(aName, _retval); } \
  NS_IMETHOD CreateFolderInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval) { return _to CreateFolderInContainer(aName, aParentFolder, aIndex, _retval); } \
  NS_IMETHOD CreateGroup(const PRUnichar *aName, nsIRDFResource **_retval) { return _to CreateGroup(aName, _retval); } \
  NS_IMETHOD CreateGroupInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval) { return _to CreateGroupInContainer(aName, aParentFolder, aIndex, _retval); } \
  NS_IMETHOD SortFolder(nsIRDFResource *aFolder, nsIRDFResource *aProperty, PRInt32 aDirection, PRBool aFoldersFirst, PRBool aRecurse) { return _to SortFolder(aFolder, aProperty, aDirection, aFoldersFirst, aRecurse); } \
  NS_IMETHOD CreateBookmark(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource **_retval) { return _to CreateBookmark(aName, aURL, aShortcutURL, aDescription, aDocCharSet, _retval); } \
  NS_IMETHOD CreateBookmarkInContainer(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource *aFolder, PRInt32 aIndex, nsIRDFResource **_retval) { return _to CreateBookmarkInContainer(aName, aURL, aShortcutURL, aDescription, aDocCharSet, aFolder, aIndex, _retval); } \
  NS_IMETHOD CreateSeparator(nsIRDFResource **_retval) { return _to CreateSeparator(_retval); } \
  NS_IMETHOD CloneResource(nsIRDFResource *aSource, nsIRDFResource **_retval) { return _to CloneResource(aSource, _retval); } \
  NS_IMETHOD UpdateBookmarkIcon(const char *aURL, const PRUnichar *aIconURL) { return _to UpdateBookmarkIcon(aURL, aIconURL); } \
  NS_IMETHOD RemoveBookmarkIcon(const char *aURL, const PRUnichar *aIconURL) { return _to RemoveBookmarkIcon(aURL, aIconURL); } \
  NS_IMETHOD UpdateLastVisitedDate(const char *aURL, const PRUnichar *docCharset) { return _to UpdateLastVisitedDate(aURL, docCharset); } \
  NS_IMETHOD GetLastCharset(const nsACString & aURL, nsAString & _retval) { return _to GetLastCharset(aURL, _retval); } \
  NS_IMETHOD ResolveKeyword(const PRUnichar *aName, char **_retval) { return _to ResolveKeyword(aName, _retval); } \
  NS_IMETHOD ImportSystemBookmarks(nsIRDFResource *aParentFolder) { return _to ImportSystemBookmarks(aParentFolder); } \
  NS_IMETHOD GetTransactionManager(nsITransactionManager * *aTransactionManager) { return _to GetTransactionManager(aTransactionManager); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIBOOKMARKSSERVICE(_to) \
  NS_IMETHOD ReadBookmarks(PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReadBookmarks(_retval); } \
  NS_IMETHOD IsBookmarked(const char *aURL, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsBookmarked(aURL, _retval); } \
  NS_IMETHOD IsBookmarkedResource(nsIRDFResource *aSource, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsBookmarkedResource(aSource, _retval); } \
  NS_IMETHOD AddBookmarkImmediately(const PRUnichar *aURI, const PRUnichar *aTitle, PRInt32 bmType, const PRUnichar *docCharset) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddBookmarkImmediately(aURI, aTitle, bmType, docCharset); } \
  NS_IMETHOD CreateFolder(const PRUnichar *aName, nsIRDFResource **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateFolder(aName, _retval); } \
  NS_IMETHOD CreateFolderInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateFolderInContainer(aName, aParentFolder, aIndex, _retval); } \
  NS_IMETHOD CreateGroup(const PRUnichar *aName, nsIRDFResource **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateGroup(aName, _retval); } \
  NS_IMETHOD CreateGroupInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateGroupInContainer(aName, aParentFolder, aIndex, _retval); } \
  NS_IMETHOD SortFolder(nsIRDFResource *aFolder, nsIRDFResource *aProperty, PRInt32 aDirection, PRBool aFoldersFirst, PRBool aRecurse) { return !_to ? NS_ERROR_NULL_POINTER : _to->SortFolder(aFolder, aProperty, aDirection, aFoldersFirst, aRecurse); } \
  NS_IMETHOD CreateBookmark(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateBookmark(aName, aURL, aShortcutURL, aDescription, aDocCharSet, _retval); } \
  NS_IMETHOD CreateBookmarkInContainer(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource *aFolder, PRInt32 aIndex, nsIRDFResource **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateBookmarkInContainer(aName, aURL, aShortcutURL, aDescription, aDocCharSet, aFolder, aIndex, _retval); } \
  NS_IMETHOD CreateSeparator(nsIRDFResource **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateSeparator(_retval); } \
  NS_IMETHOD CloneResource(nsIRDFResource *aSource, nsIRDFResource **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CloneResource(aSource, _retval); } \
  NS_IMETHOD UpdateBookmarkIcon(const char *aURL, const PRUnichar *aIconURL) { return !_to ? NS_ERROR_NULL_POINTER : _to->UpdateBookmarkIcon(aURL, aIconURL); } \
  NS_IMETHOD RemoveBookmarkIcon(const char *aURL, const PRUnichar *aIconURL) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveBookmarkIcon(aURL, aIconURL); } \
  NS_IMETHOD UpdateLastVisitedDate(const char *aURL, const PRUnichar *docCharset) { return !_to ? NS_ERROR_NULL_POINTER : _to->UpdateLastVisitedDate(aURL, docCharset); } \
  NS_IMETHOD GetLastCharset(const nsACString & aURL, nsAString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLastCharset(aURL, _retval); } \
  NS_IMETHOD ResolveKeyword(const PRUnichar *aName, char **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->ResolveKeyword(aName, _retval); } \
  NS_IMETHOD ImportSystemBookmarks(nsIRDFResource *aParentFolder) { return !_to ? NS_ERROR_NULL_POINTER : _to->ImportSystemBookmarks(aParentFolder); } \
  NS_IMETHOD GetTransactionManager(nsITransactionManager * *aTransactionManager) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTransactionManager(aTransactionManager); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsBookmarksService : public nsIBookmarksService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIBOOKMARKSSERVICE

  nsBookmarksService();

private:
  ~nsBookmarksService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsBookmarksService, nsIBookmarksService)

nsBookmarksService::nsBookmarksService()
{
  /* member initializers and constructor code */
}

nsBookmarksService::~nsBookmarksService()
{
  /* destructor code */
}

/* boolean readBookmarks (); */
NS_IMETHODIMP nsBookmarksService::ReadBookmarks(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isBookmarked (in string aURL); */
NS_IMETHODIMP nsBookmarksService::IsBookmarked(const char *aURL, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isBookmarkedResource (in nsIRDFResource aSource); */
NS_IMETHODIMP nsBookmarksService::IsBookmarkedResource(nsIRDFResource *aSource, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addBookmarkImmediately (in wstring aURI, in wstring aTitle, in long bmType, in wstring docCharset); */
NS_IMETHODIMP nsBookmarksService::AddBookmarkImmediately(const PRUnichar *aURI, const PRUnichar *aTitle, PRInt32 bmType, const PRUnichar *docCharset)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIRDFResource createFolder (in wstring aName); */
NS_IMETHODIMP nsBookmarksService::CreateFolder(const PRUnichar *aName, nsIRDFResource **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIRDFResource createFolderInContainer (in wstring aName, in nsIRDFResource aParentFolder, in long aIndex); */
NS_IMETHODIMP nsBookmarksService::CreateFolderInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIRDFResource createGroup (in wstring aName); */
NS_IMETHODIMP nsBookmarksService::CreateGroup(const PRUnichar *aName, nsIRDFResource **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIRDFResource createGroupInContainer (in wstring aName, in nsIRDFResource aParentFolder, in long aIndex); */
NS_IMETHODIMP nsBookmarksService::CreateGroupInContainer(const PRUnichar *aName, nsIRDFResource *aParentFolder, PRInt32 aIndex, nsIRDFResource **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void sortFolder (in nsIRDFResource aFolder, in nsIRDFResource aProperty, in long aDirection, in boolean aFoldersFirst, in boolean aRecurse); */
NS_IMETHODIMP nsBookmarksService::SortFolder(nsIRDFResource *aFolder, nsIRDFResource *aProperty, PRInt32 aDirection, PRBool aFoldersFirst, PRBool aRecurse)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIRDFResource createBookmark (in wstring aName, in wstring aURL, in wstring aShortcutURL, in wstring aDescription, in wstring aDocCharSet); */
NS_IMETHODIMP nsBookmarksService::CreateBookmark(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIRDFResource createBookmarkInContainer (in wstring aName, in wstring aURL, in wstring aShortcutURL, in wstring aDescription, in wstring aDocCharSet, in nsIRDFResource aFolder, in long aIndex); */
NS_IMETHODIMP nsBookmarksService::CreateBookmarkInContainer(const PRUnichar *aName, const PRUnichar *aURL, const PRUnichar *aShortcutURL, const PRUnichar *aDescription, const PRUnichar *aDocCharSet, nsIRDFResource *aFolder, PRInt32 aIndex, nsIRDFResource **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIRDFResource createSeparator (); */
NS_IMETHODIMP nsBookmarksService::CreateSeparator(nsIRDFResource **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIRDFResource cloneResource (in nsIRDFResource aSource); */
NS_IMETHODIMP nsBookmarksService::CloneResource(nsIRDFResource *aSource, nsIRDFResource **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void updateBookmarkIcon (in string aURL, in wstring aIconURL); */
NS_IMETHODIMP nsBookmarksService::UpdateBookmarkIcon(const char *aURL, const PRUnichar *aIconURL)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeBookmarkIcon (in string aURL, in wstring aIconURL); */
NS_IMETHODIMP nsBookmarksService::RemoveBookmarkIcon(const char *aURL, const PRUnichar *aIconURL)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void updateLastVisitedDate (in string aURL, in wstring docCharset); */
NS_IMETHODIMP nsBookmarksService::UpdateLastVisitedDate(const char *aURL, const PRUnichar *docCharset)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getLastCharset (in AUTF8String aURL); */
NS_IMETHODIMP nsBookmarksService::GetLastCharset(const nsACString & aURL, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string resolveKeyword (in wstring aName); */
NS_IMETHODIMP nsBookmarksService::ResolveKeyword(const PRUnichar *aName, char **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void importSystemBookmarks (in nsIRDFResource aParentFolder); */
NS_IMETHODIMP nsBookmarksService::ImportSystemBookmarks(nsIRDFResource *aParentFolder)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsITransactionManager transactionManager; */
NS_IMETHODIMP nsBookmarksService::GetTransactionManager(nsITransactionManager * *aTransactionManager)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

// {E638D760-8687-11d2-B530-000000000000}
#define NS_BOOKMARKS_SERVICE_CID \
{ 0xe638d760, 0x8687, 0x11d2, { 0xb5, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } }
#define NS_BOOKMARKS_SERVICE_CONTRACTID \
    "@mozilla.org/browser/bookmarks-service;1"
#define NS_BOOKMARKS_DATASOURCE_CONTRACTID \
    "@mozilla.org/rdf/datasource;1?name=bookmarks"

#endif /* __gen_nsIBookmarksService_h__ */
