/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/docshell/shistory/public/nsISHEntry.idl
 */

#ifndef __gen_nsISHEntry_h__
#define __gen_nsISHEntry_h__


#ifndef __gen_nsIHistoryEntry_h__
#include "nsIHistoryEntry.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsILayoutHistoryState; /* forward declaration */

class nsIContentViewer; /* forward declaration */

class nsIURI; /* forward declaration */

class nsIInputStream; /* forward declaration */

class nsIDocShellTreeItem; /* forward declaration */

class nsISupportsArray; /* forward declaration */

struct nsRect;

/* starting interface:    nsISHEntry */
#define NS_ISHENTRY_IID_STR "542a98b9-2889-4922-aaf4-02b6056f4136"

#define NS_ISHENTRY_IID \
  {0x542a98b9, 0x2889, 0x4922, \
    { 0xaa, 0xf4, 0x02, 0xb6, 0x05, 0x6f, 0x41, 0x36 }}

class NS_NO_VTABLE nsISHEntry : public nsIHistoryEntry {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISHENTRY_IID)

  /** URI for the document */
  /* void setURI (in nsIURI aURI); */
  NS_IMETHOD SetURI(nsIURI *aURI) = 0;

  /** Referrer URI */
  /* attribute nsIURI referrerURI; */
  NS_IMETHOD GetReferrerURI(nsIURI * *aReferrerURI) = 0;
  NS_IMETHOD SetReferrerURI(nsIURI * aReferrerURI) = 0;

  /** Content viewer, for fast restoration of presentation */
  /* attribute nsIContentViewer contentViewer; */
  NS_IMETHOD GetContentViewer(nsIContentViewer * *aContentViewer) = 0;
  NS_IMETHOD SetContentViewer(nsIContentViewer * aContentViewer) = 0;

  /** Whether the content viewer is marked "sticky" */
  /* attribute boolean sticky; */
  NS_IMETHOD GetSticky(PRBool *aSticky) = 0;
  NS_IMETHOD SetSticky(PRBool aSticky) = 0;

  /** Saved state of the global window object */
  /* attribute nsISupports windowState; */
  NS_IMETHOD GetWindowState(nsISupports * *aWindowState) = 0;
  NS_IMETHOD SetWindowState(nsISupports * aWindowState) = 0;

  /**
     * Saved position and dimensions of the content viewer; we must adjust the
     * root view's widget accordingly if this has changed when the presentation
     * is restored.
     */
  /* [noscript] void getViewerBounds (in nsRect bounds); */
  NS_IMETHOD GetViewerBounds(nsRect & bounds) = 0;

  /* [noscript] void setViewerBounds ([const] in nsRect bounds); */
  NS_IMETHOD SetViewerBounds(const nsRect & bounds) = 0;

  /**
     * Saved child docshells corresponding to contentViewer.  There are weak
     * references since it's assumed that the content viewer's document has
     * an owning reference to the subdocument for each shell.  The child shells
     * are restored as children of the parent docshell, in this order, when the
     * parent docshell restores a saved presentation.
     */
/** Append a child shell to the end of our list. */
  /* void addChildShell (in nsIDocShellTreeItem shell); */
  NS_IMETHOD AddChildShell(nsIDocShellTreeItem *shell) = 0;

  /**
     * Get the child shell at |index|; returns null if |index| is out of bounds.
     */
  /* nsIDocShellTreeItem childShellAt (in long index); */
  NS_IMETHOD ChildShellAt(PRInt32 index, nsIDocShellTreeItem **_retval) = 0;

  /**
     * Clear the child shell list.
     */
  /* void clearChildShells (); */
  NS_IMETHOD ClearChildShells(void) = 0;

  /** Saved refresh URI list for the content viewer */
  /* attribute nsISupportsArray refreshURIList; */
  NS_IMETHOD GetRefreshURIList(nsISupportsArray * *aRefreshURIList) = 0;
  NS_IMETHOD SetRefreshURIList(nsISupportsArray * aRefreshURIList) = 0;

  /**
     * Ensure that the cached presentation members are self-consistent.
     * If either |contentViewer| or |windowState| are null, then all of the
     * following members are cleared/reset:
     *  contentViewer, sticky, windowState, viewerBounds, childShells,
     *  refreshURIList.
     */
  /* void syncPresentationState (); */
  NS_IMETHOD SyncPresentationState(void) = 0;

  /** Title for the document */
  /* void setTitle (in AString aTitle); */
  NS_IMETHOD SetTitle(const nsAString & aTitle) = 0;

  /** Post Data for the document */
  /* attribute nsIInputStream postData; */
  NS_IMETHOD GetPostData(nsIInputStream * *aPostData) = 0;
  NS_IMETHOD SetPostData(nsIInputStream * aPostData) = 0;

  /** LayoutHistoryState for scroll position and form values */
  /* attribute nsILayoutHistoryState layoutHistoryState; */
  NS_IMETHOD GetLayoutHistoryState(nsILayoutHistoryState * *aLayoutHistoryState) = 0;
  NS_IMETHOD SetLayoutHistoryState(nsILayoutHistoryState * aLayoutHistoryState) = 0;

  /** parent of this entry */
  /* attribute nsISHEntry parent; */
  NS_IMETHOD GetParent(nsISHEntry * *aParent) = 0;
  NS_IMETHOD SetParent(nsISHEntry * aParent) = 0;

  /**
     * The loadType for this entry. This is typically loadHistory except
     * when reload is pressed, it has the appropriate reload flag
     */
  /* attribute unsigned long loadType; */
  NS_IMETHOD GetLoadType(PRUint32 *aLoadType) = 0;
  NS_IMETHOD SetLoadType(PRUint32 aLoadType) = 0;

  /**
     * An ID to help identify this entry from others during
     * subframe navigation
     */
  /* attribute unsigned long ID; */
  NS_IMETHOD GetID(PRUint32 *aID) = 0;
  NS_IMETHOD SetID(PRUint32 aID) = 0;

  /**
     * pageIdentifier is an integer that should be the same for two entries
     * attached to the same docshell only if the two entries are entries for
     * the same page in the sense that one could go from the state represented
     * by one to the state represented by the other simply by scrolling (so the
     * entries are separated by an anchor traversal or a subframe navigation in
     * some other frame).
     */
  /* attribute unsigned long pageIdentifier; */
  NS_IMETHOD GetPageIdentifier(PRUint32 *aPageIdentifier) = 0;
  NS_IMETHOD SetPageIdentifier(PRUint32 aPageIdentifier) = 0;

  /** attribute to set and get the cache key for the entry */
  /* attribute nsISupports cacheKey; */
  NS_IMETHOD GetCacheKey(nsISupports * *aCacheKey) = 0;
  NS_IMETHOD SetCacheKey(nsISupports * aCacheKey) = 0;

  /** attribute to indicate whether layoutHistoryState should be saved */
  /* attribute boolean saveLayoutStateFlag; */
  NS_IMETHOD GetSaveLayoutStateFlag(PRBool *aSaveLayoutStateFlag) = 0;
  NS_IMETHOD SetSaveLayoutStateFlag(PRBool aSaveLayoutStateFlag) = 0;

  /** attribute to indicate whether the page is already expired in cache */
  /* attribute boolean expirationStatus; */
  NS_IMETHOD GetExpirationStatus(PRBool *aExpirationStatus) = 0;
  NS_IMETHOD SetExpirationStatus(PRBool aExpirationStatus) = 0;

  /**
     * attribute to indicate the content-type of the document that this
     * is a session history entry for
     */
  /* attribute ACString contentType; */
  NS_IMETHOD GetContentType(nsACString & aContentType) = 0;
  NS_IMETHOD SetContentType(const nsACString & aContentType) = 0;

  /** Set/Get scrollers' positon in anchored pages */
  /* void setScrollPosition (in long x, in long y); */
  NS_IMETHOD SetScrollPosition(PRInt32 x, PRInt32 y) = 0;

  /* void getScrollPosition (out long x, out long y); */
  NS_IMETHOD GetScrollPosition(PRInt32 *x, PRInt32 *y) = 0;

  /** Additional ways to create an entry */
  /* void create (in nsIURI URI, in AString title, in nsIInputStream inputStream, in nsILayoutHistoryState layoutHistoryState, in nsISupports cacheKey, in ACString contentType); */
  NS_IMETHOD Create(nsIURI *URI, const nsAString & title, nsIInputStream *inputStream, nsILayoutHistoryState *layoutHistoryState, nsISupports *cacheKey, const nsACString & contentType) = 0;

  /* nsISHEntry clone (); */
  NS_IMETHOD Clone(nsISHEntry **_retval) = 0;

  /** Attribute that indicates if this entry is for a subframe navigation */
  /* void setIsSubFrame (in boolean aFlag); */
  NS_IMETHOD SetIsSubFrame(PRBool aFlag) = 0;

  /** Return any content viewer present in or below this node in the
        nsSHEntry tree.  This will differ from contentViewer in the case
        where a child nsSHEntry has the content viewer for this tree. */
  /* nsIContentViewer getAnyContentViewer (out nsISHEntry ownerEntry); */
  NS_IMETHOD GetAnyContentViewer(nsISHEntry **ownerEntry, nsIContentViewer **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISHENTRY \
  NS_IMETHOD SetURI(nsIURI *aURI); \
  NS_IMETHOD GetReferrerURI(nsIURI * *aReferrerURI); \
  NS_IMETHOD SetReferrerURI(nsIURI * aReferrerURI); \
  NS_IMETHOD GetContentViewer(nsIContentViewer * *aContentViewer); \
  NS_IMETHOD SetContentViewer(nsIContentViewer * aContentViewer); \
  NS_IMETHOD GetSticky(PRBool *aSticky); \
  NS_IMETHOD SetSticky(PRBool aSticky); \
  NS_IMETHOD GetWindowState(nsISupports * *aWindowState); \
  NS_IMETHOD SetWindowState(nsISupports * aWindowState); \
  NS_IMETHOD GetViewerBounds(nsRect & bounds); \
  NS_IMETHOD SetViewerBounds(const nsRect & bounds); \
  NS_IMETHOD AddChildShell(nsIDocShellTreeItem *shell); \
  NS_IMETHOD ChildShellAt(PRInt32 index, nsIDocShellTreeItem **_retval); \
  NS_IMETHOD ClearChildShells(void); \
  NS_IMETHOD GetRefreshURIList(nsISupportsArray * *aRefreshURIList); \
  NS_IMETHOD SetRefreshURIList(nsISupportsArray * aRefreshURIList); \
  NS_IMETHOD SyncPresentationState(void); \
  NS_IMETHOD SetTitle(const nsAString & aTitle); \
  NS_IMETHOD GetPostData(nsIInputStream * *aPostData); \
  NS_IMETHOD SetPostData(nsIInputStream * aPostData); \
  NS_IMETHOD GetLayoutHistoryState(nsILayoutHistoryState * *aLayoutHistoryState); \
  NS_IMETHOD SetLayoutHistoryState(nsILayoutHistoryState * aLayoutHistoryState); \
  NS_IMETHOD GetParent(nsISHEntry * *aParent); \
  NS_IMETHOD SetParent(nsISHEntry * aParent); \
  NS_IMETHOD GetLoadType(PRUint32 *aLoadType); \
  NS_IMETHOD SetLoadType(PRUint32 aLoadType); \
  NS_IMETHOD GetID(PRUint32 *aID); \
  NS_IMETHOD SetID(PRUint32 aID); \
  NS_IMETHOD GetPageIdentifier(PRUint32 *aPageIdentifier); \
  NS_IMETHOD SetPageIdentifier(PRUint32 aPageIdentifier); \
  NS_IMETHOD GetCacheKey(nsISupports * *aCacheKey); \
  NS_IMETHOD SetCacheKey(nsISupports * aCacheKey); \
  NS_IMETHOD GetSaveLayoutStateFlag(PRBool *aSaveLayoutStateFlag); \
  NS_IMETHOD SetSaveLayoutStateFlag(PRBool aSaveLayoutStateFlag); \
  NS_IMETHOD GetExpirationStatus(PRBool *aExpirationStatus); \
  NS_IMETHOD SetExpirationStatus(PRBool aExpirationStatus); \
  NS_IMETHOD GetContentType(nsACString & aContentType); \
  NS_IMETHOD SetContentType(const nsACString & aContentType); \
  NS_IMETHOD SetScrollPosition(PRInt32 x, PRInt32 y); \
  NS_IMETHOD GetScrollPosition(PRInt32 *x, PRInt32 *y); \
  NS_IMETHOD Create(nsIURI *URI, const nsAString & title, nsIInputStream *inputStream, nsILayoutHistoryState *layoutHistoryState, nsISupports *cacheKey, const nsACString & contentType); \
  NS_IMETHOD Clone(nsISHEntry **_retval); \
  NS_IMETHOD SetIsSubFrame(PRBool aFlag); \
  NS_IMETHOD GetAnyContentViewer(nsISHEntry **ownerEntry, nsIContentViewer **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISHENTRY(_to) \
  NS_IMETHOD SetURI(nsIURI *aURI) { return _to SetURI(aURI); } \
  NS_IMETHOD GetReferrerURI(nsIURI * *aReferrerURI) { return _to GetReferrerURI(aReferrerURI); } \
  NS_IMETHOD SetReferrerURI(nsIURI * aReferrerURI) { return _to SetReferrerURI(aReferrerURI); } \
  NS_IMETHOD GetContentViewer(nsIContentViewer * *aContentViewer) { return _to GetContentViewer(aContentViewer); } \
  NS_IMETHOD SetContentViewer(nsIContentViewer * aContentViewer) { return _to SetContentViewer(aContentViewer); } \
  NS_IMETHOD GetSticky(PRBool *aSticky) { return _to GetSticky(aSticky); } \
  NS_IMETHOD SetSticky(PRBool aSticky) { return _to SetSticky(aSticky); } \
  NS_IMETHOD GetWindowState(nsISupports * *aWindowState) { return _to GetWindowState(aWindowState); } \
  NS_IMETHOD SetWindowState(nsISupports * aWindowState) { return _to SetWindowState(aWindowState); } \
  NS_IMETHOD GetViewerBounds(nsRect & bounds) { return _to GetViewerBounds(bounds); } \
  NS_IMETHOD SetViewerBounds(const nsRect & bounds) { return _to SetViewerBounds(bounds); } \
  NS_IMETHOD AddChildShell(nsIDocShellTreeItem *shell) { return _to AddChildShell(shell); } \
  NS_IMETHOD ChildShellAt(PRInt32 index, nsIDocShellTreeItem **_retval) { return _to ChildShellAt(index, _retval); } \
  NS_IMETHOD ClearChildShells(void) { return _to ClearChildShells(); } \
  NS_IMETHOD GetRefreshURIList(nsISupportsArray * *aRefreshURIList) { return _to GetRefreshURIList(aRefreshURIList); } \
  NS_IMETHOD SetRefreshURIList(nsISupportsArray * aRefreshURIList) { return _to SetRefreshURIList(aRefreshURIList); } \
  NS_IMETHOD SyncPresentationState(void) { return _to SyncPresentationState(); } \
  NS_IMETHOD SetTitle(const nsAString & aTitle) { return _to SetTitle(aTitle); } \
  NS_IMETHOD GetPostData(nsIInputStream * *aPostData) { return _to GetPostData(aPostData); } \
  NS_IMETHOD SetPostData(nsIInputStream * aPostData) { return _to SetPostData(aPostData); } \
  NS_IMETHOD GetLayoutHistoryState(nsILayoutHistoryState * *aLayoutHistoryState) { return _to GetLayoutHistoryState(aLayoutHistoryState); } \
  NS_IMETHOD SetLayoutHistoryState(nsILayoutHistoryState * aLayoutHistoryState) { return _to SetLayoutHistoryState(aLayoutHistoryState); } \
  NS_IMETHOD GetParent(nsISHEntry * *aParent) { return _to GetParent(aParent); } \
  NS_IMETHOD SetParent(nsISHEntry * aParent) { return _to SetParent(aParent); } \
  NS_IMETHOD GetLoadType(PRUint32 *aLoadType) { return _to GetLoadType(aLoadType); } \
  NS_IMETHOD SetLoadType(PRUint32 aLoadType) { return _to SetLoadType(aLoadType); } \
  NS_IMETHOD GetID(PRUint32 *aID) { return _to GetID(aID); } \
  NS_IMETHOD SetID(PRUint32 aID) { return _to SetID(aID); } \
  NS_IMETHOD GetPageIdentifier(PRUint32 *aPageIdentifier) { return _to GetPageIdentifier(aPageIdentifier); } \
  NS_IMETHOD SetPageIdentifier(PRUint32 aPageIdentifier) { return _to SetPageIdentifier(aPageIdentifier); } \
  NS_IMETHOD GetCacheKey(nsISupports * *aCacheKey) { return _to GetCacheKey(aCacheKey); } \
  NS_IMETHOD SetCacheKey(nsISupports * aCacheKey) { return _to SetCacheKey(aCacheKey); } \
  NS_IMETHOD GetSaveLayoutStateFlag(PRBool *aSaveLayoutStateFlag) { return _to GetSaveLayoutStateFlag(aSaveLayoutStateFlag); } \
  NS_IMETHOD SetSaveLayoutStateFlag(PRBool aSaveLayoutStateFlag) { return _to SetSaveLayoutStateFlag(aSaveLayoutStateFlag); } \
  NS_IMETHOD GetExpirationStatus(PRBool *aExpirationStatus) { return _to GetExpirationStatus(aExpirationStatus); } \
  NS_IMETHOD SetExpirationStatus(PRBool aExpirationStatus) { return _to SetExpirationStatus(aExpirationStatus); } \
  NS_IMETHOD GetContentType(nsACString & aContentType) { return _to GetContentType(aContentType); } \
  NS_IMETHOD SetContentType(const nsACString & aContentType) { return _to SetContentType(aContentType); } \
  NS_IMETHOD SetScrollPosition(PRInt32 x, PRInt32 y) { return _to SetScrollPosition(x, y); } \
  NS_IMETHOD GetScrollPosition(PRInt32 *x, PRInt32 *y) { return _to GetScrollPosition(x, y); } \
  NS_IMETHOD Create(nsIURI *URI, const nsAString & title, nsIInputStream *inputStream, nsILayoutHistoryState *layoutHistoryState, nsISupports *cacheKey, const nsACString & contentType) { return _to Create(URI, title, inputStream, layoutHistoryState, cacheKey, contentType); } \
  NS_IMETHOD Clone(nsISHEntry **_retval) { return _to Clone(_retval); } \
  NS_IMETHOD SetIsSubFrame(PRBool aFlag) { return _to SetIsSubFrame(aFlag); } \
  NS_IMETHOD GetAnyContentViewer(nsISHEntry **ownerEntry, nsIContentViewer **_retval) { return _to GetAnyContentViewer(ownerEntry, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISHENTRY(_to) \
  NS_IMETHOD SetURI(nsIURI *aURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetURI(aURI); } \
  NS_IMETHOD GetReferrerURI(nsIURI * *aReferrerURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetReferrerURI(aReferrerURI); } \
  NS_IMETHOD SetReferrerURI(nsIURI * aReferrerURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetReferrerURI(aReferrerURI); } \
  NS_IMETHOD GetContentViewer(nsIContentViewer * *aContentViewer) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetContentViewer(aContentViewer); } \
  NS_IMETHOD SetContentViewer(nsIContentViewer * aContentViewer) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetContentViewer(aContentViewer); } \
  NS_IMETHOD GetSticky(PRBool *aSticky) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSticky(aSticky); } \
  NS_IMETHOD SetSticky(PRBool aSticky) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSticky(aSticky); } \
  NS_IMETHOD GetWindowState(nsISupports * *aWindowState) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetWindowState(aWindowState); } \
  NS_IMETHOD SetWindowState(nsISupports * aWindowState) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetWindowState(aWindowState); } \
  NS_IMETHOD GetViewerBounds(nsRect & bounds) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetViewerBounds(bounds); } \
  NS_IMETHOD SetViewerBounds(const nsRect & bounds) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetViewerBounds(bounds); } \
  NS_IMETHOD AddChildShell(nsIDocShellTreeItem *shell) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddChildShell(shell); } \
  NS_IMETHOD ChildShellAt(PRInt32 index, nsIDocShellTreeItem **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->ChildShellAt(index, _retval); } \
  NS_IMETHOD ClearChildShells(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->ClearChildShells(); } \
  NS_IMETHOD GetRefreshURIList(nsISupportsArray * *aRefreshURIList) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRefreshURIList(aRefreshURIList); } \
  NS_IMETHOD SetRefreshURIList(nsISupportsArray * aRefreshURIList) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetRefreshURIList(aRefreshURIList); } \
  NS_IMETHOD SyncPresentationState(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->SyncPresentationState(); } \
  NS_IMETHOD SetTitle(const nsAString & aTitle) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetTitle(aTitle); } \
  NS_IMETHOD GetPostData(nsIInputStream * *aPostData) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPostData(aPostData); } \
  NS_IMETHOD SetPostData(nsIInputStream * aPostData) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPostData(aPostData); } \
  NS_IMETHOD GetLayoutHistoryState(nsILayoutHistoryState * *aLayoutHistoryState) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLayoutHistoryState(aLayoutHistoryState); } \
  NS_IMETHOD SetLayoutHistoryState(nsILayoutHistoryState * aLayoutHistoryState) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetLayoutHistoryState(aLayoutHistoryState); } \
  NS_IMETHOD GetParent(nsISHEntry * *aParent) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParent(aParent); } \
  NS_IMETHOD SetParent(nsISHEntry * aParent) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetParent(aParent); } \
  NS_IMETHOD GetLoadType(PRUint32 *aLoadType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLoadType(aLoadType); } \
  NS_IMETHOD SetLoadType(PRUint32 aLoadType) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetLoadType(aLoadType); } \
  NS_IMETHOD GetID(PRUint32 *aID) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetID(aID); } \
  NS_IMETHOD SetID(PRUint32 aID) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetID(aID); } \
  NS_IMETHOD GetPageIdentifier(PRUint32 *aPageIdentifier) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPageIdentifier(aPageIdentifier); } \
  NS_IMETHOD SetPageIdentifier(PRUint32 aPageIdentifier) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetPageIdentifier(aPageIdentifier); } \
  NS_IMETHOD GetCacheKey(nsISupports * *aCacheKey) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCacheKey(aCacheKey); } \
  NS_IMETHOD SetCacheKey(nsISupports * aCacheKey) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetCacheKey(aCacheKey); } \
  NS_IMETHOD GetSaveLayoutStateFlag(PRBool *aSaveLayoutStateFlag) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSaveLayoutStateFlag(aSaveLayoutStateFlag); } \
  NS_IMETHOD SetSaveLayoutStateFlag(PRBool aSaveLayoutStateFlag) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetSaveLayoutStateFlag(aSaveLayoutStateFlag); } \
  NS_IMETHOD GetExpirationStatus(PRBool *aExpirationStatus) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetExpirationStatus(aExpirationStatus); } \
  NS_IMETHOD SetExpirationStatus(PRBool aExpirationStatus) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetExpirationStatus(aExpirationStatus); } \
  NS_IMETHOD GetContentType(nsACString & aContentType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetContentType(aContentType); } \
  NS_IMETHOD SetContentType(const nsACString & aContentType) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetContentType(aContentType); } \
  NS_IMETHOD SetScrollPosition(PRInt32 x, PRInt32 y) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetScrollPosition(x, y); } \
  NS_IMETHOD GetScrollPosition(PRInt32 *x, PRInt32 *y) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetScrollPosition(x, y); } \
  NS_IMETHOD Create(nsIURI *URI, const nsAString & title, nsIInputStream *inputStream, nsILayoutHistoryState *layoutHistoryState, nsISupports *cacheKey, const nsACString & contentType) { return !_to ? NS_ERROR_NULL_POINTER : _to->Create(URI, title, inputStream, layoutHistoryState, cacheKey, contentType); } \
  NS_IMETHOD Clone(nsISHEntry **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->Clone(_retval); } \
  NS_IMETHOD SetIsSubFrame(PRBool aFlag) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetIsSubFrame(aFlag); } \
  NS_IMETHOD GetAnyContentViewer(nsISHEntry **ownerEntry, nsIContentViewer **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAnyContentViewer(ownerEntry, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSHEntry : public nsISHEntry
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISHENTRY

  nsSHEntry();

private:
  ~nsSHEntry();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSHEntry, nsISHEntry)

nsSHEntry::nsSHEntry()
{
  /* member initializers and constructor code */
}

nsSHEntry::~nsSHEntry()
{
  /* destructor code */
}

/* void setURI (in nsIURI aURI); */
NS_IMETHODIMP nsSHEntry::SetURI(nsIURI *aURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIURI referrerURI; */
NS_IMETHODIMP nsSHEntry::GetReferrerURI(nsIURI * *aReferrerURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetReferrerURI(nsIURI * aReferrerURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIContentViewer contentViewer; */
NS_IMETHODIMP nsSHEntry::GetContentViewer(nsIContentViewer * *aContentViewer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetContentViewer(nsIContentViewer * aContentViewer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean sticky; */
NS_IMETHODIMP nsSHEntry::GetSticky(PRBool *aSticky)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetSticky(PRBool aSticky)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISupports windowState; */
NS_IMETHODIMP nsSHEntry::GetWindowState(nsISupports * *aWindowState)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetWindowState(nsISupports * aWindowState)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void getViewerBounds (in nsRect bounds); */
NS_IMETHODIMP nsSHEntry::GetViewerBounds(nsRect & bounds)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void setViewerBounds ([const] in nsRect bounds); */
NS_IMETHODIMP nsSHEntry::SetViewerBounds(const nsRect & bounds)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addChildShell (in nsIDocShellTreeItem shell); */
NS_IMETHODIMP nsSHEntry::AddChildShell(nsIDocShellTreeItem *shell)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDocShellTreeItem childShellAt (in long index); */
NS_IMETHODIMP nsSHEntry::ChildShellAt(PRInt32 index, nsIDocShellTreeItem **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void clearChildShells (); */
NS_IMETHODIMP nsSHEntry::ClearChildShells()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISupportsArray refreshURIList; */
NS_IMETHODIMP nsSHEntry::GetRefreshURIList(nsISupportsArray * *aRefreshURIList)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetRefreshURIList(nsISupportsArray * aRefreshURIList)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void syncPresentationState (); */
NS_IMETHODIMP nsSHEntry::SyncPresentationState()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setTitle (in AString aTitle); */
NS_IMETHODIMP nsSHEntry::SetTitle(const nsAString & aTitle)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIInputStream postData; */
NS_IMETHODIMP nsSHEntry::GetPostData(nsIInputStream * *aPostData)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetPostData(nsIInputStream * aPostData)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsILayoutHistoryState layoutHistoryState; */
NS_IMETHODIMP nsSHEntry::GetLayoutHistoryState(nsILayoutHistoryState * *aLayoutHistoryState)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetLayoutHistoryState(nsILayoutHistoryState * aLayoutHistoryState)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISHEntry parent; */
NS_IMETHODIMP nsSHEntry::GetParent(nsISHEntry * *aParent)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetParent(nsISHEntry * aParent)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute unsigned long loadType; */
NS_IMETHODIMP nsSHEntry::GetLoadType(PRUint32 *aLoadType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetLoadType(PRUint32 aLoadType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute unsigned long ID; */
NS_IMETHODIMP nsSHEntry::GetID(PRUint32 *aID)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetID(PRUint32 aID)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute unsigned long pageIdentifier; */
NS_IMETHODIMP nsSHEntry::GetPageIdentifier(PRUint32 *aPageIdentifier)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetPageIdentifier(PRUint32 aPageIdentifier)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISupports cacheKey; */
NS_IMETHODIMP nsSHEntry::GetCacheKey(nsISupports * *aCacheKey)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetCacheKey(nsISupports * aCacheKey)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean saveLayoutStateFlag; */
NS_IMETHODIMP nsSHEntry::GetSaveLayoutStateFlag(PRBool *aSaveLayoutStateFlag)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetSaveLayoutStateFlag(PRBool aSaveLayoutStateFlag)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean expirationStatus; */
NS_IMETHODIMP nsSHEntry::GetExpirationStatus(PRBool *aExpirationStatus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetExpirationStatus(PRBool aExpirationStatus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute ACString contentType; */
NS_IMETHODIMP nsSHEntry::GetContentType(nsACString & aContentType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHEntry::SetContentType(const nsACString & aContentType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setScrollPosition (in long x, in long y); */
NS_IMETHODIMP nsSHEntry::SetScrollPosition(PRInt32 x, PRInt32 y)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getScrollPosition (out long x, out long y); */
NS_IMETHODIMP nsSHEntry::GetScrollPosition(PRInt32 *x, PRInt32 *y)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void create (in nsIURI URI, in AString title, in nsIInputStream inputStream, in nsILayoutHistoryState layoutHistoryState, in nsISupports cacheKey, in ACString contentType); */
NS_IMETHODIMP nsSHEntry::Create(nsIURI *URI, const nsAString & title, nsIInputStream *inputStream, nsILayoutHistoryState *layoutHistoryState, nsISupports *cacheKey, const nsACString & contentType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISHEntry clone (); */
NS_IMETHODIMP nsSHEntry::Clone(nsISHEntry **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setIsSubFrame (in boolean aFlag); */
NS_IMETHODIMP nsSHEntry::SetIsSubFrame(PRBool aFlag)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIContentViewer getAnyContentViewer (out nsISHEntry ownerEntry); */
NS_IMETHODIMP nsSHEntry::GetAnyContentViewer(nsISHEntry **ownerEntry, nsIContentViewer **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

// {BFD1A791-AD9F-11d3-BDC7-0050040A9B44}
#define NS_SHENTRY_CID \
{0xbfd1a791, 0xad9f, 0x11d3, {0xbd, 0xc7, 0x0, 0x50, 0x4, 0xa, 0x9b, 0x44}}
#define NS_SHENTRY_CONTRACTID \
    "@mozilla.org/browser/session-history-entry;1"

#endif /* __gen_nsISHEntry_h__ */
