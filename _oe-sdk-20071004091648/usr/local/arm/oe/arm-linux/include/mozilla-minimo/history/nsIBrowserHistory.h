/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/history/public/nsIBrowserHistory.idl
 */

#ifndef __gen_nsIBrowserHistory_h__
#define __gen_nsIBrowserHistory_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIGlobalHistory2_h__
#include "nsIGlobalHistory2.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIBrowserHistory */
#define NS_IBROWSERHISTORY_IID_STR "c43079c3-3d8d-4b7c-af14-0e30ab46865f"

#define NS_IBROWSERHISTORY_IID \
  {0xc43079c3, 0x3d8d, 0x4b7c, \
    { 0xaf, 0x14, 0x0e, 0x30, 0xab, 0x46, 0x86, 0x5f }}

class NS_NO_VTABLE nsIBrowserHistory : public nsIGlobalHistory2 {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IBROWSERHISTORY_IID)

  /** 
     * addPageWithDetails
     * Adds a page to history with specific time stamp information. This is used in
     * the History migrator. 
     */
  /* void addPageWithDetails (in nsIURI aURI, in wstring aTitle, in long long aLastVisited); */
  NS_IMETHOD AddPageWithDetails(nsIURI *aURI, const PRUnichar *aTitle, PRInt64 aLastVisited) = 0;

  /**
     * lastPageVisited
     * The last page that was visited in a top-level window.
     */
  /* readonly attribute AUTF8String lastPageVisited; */
  NS_IMETHOD GetLastPageVisited(nsACString & aLastPageVisited) = 0;

  /**
     * count
     * The number of entries in global history
     */
  /* readonly attribute PRUint32 count; */
  NS_IMETHOD GetCount(PRUint32 *aCount) = 0;

  /**
     * remove a page from history
     */
  /* void removePage (in nsIURI aURI); */
  NS_IMETHOD RemovePage(nsIURI *aURI) = 0;

  /**
     * removePagesFromHost
     * Remove all pages from the given host.
     * If aEntireDomain is true, will assume aHost is a domain,
     * and remove all pages from the entire domain.
     */
  /* void removePagesFromHost (in AUTF8String aHost, in boolean aEntireDomain); */
  NS_IMETHOD RemovePagesFromHost(const nsACString & aHost, PRBool aEntireDomain) = 0;

  /**
     * removeAllPages
     * Remove all pages from global history
     */
  /* void removeAllPages (); */
  NS_IMETHOD RemoveAllPages(void) = 0;

  /**
     * hidePage
     * Hide the specified URL from being enumerated (and thus
     * displayed in the UI)
     *
     * if the page hasn't been visited yet, then it will be added
     * as if it was visited, and then marked as hidden
     */
  /* void hidePage (in nsIURI aURI); */
  NS_IMETHOD HidePage(nsIURI *aURI) = 0;

  /**
     * markPageAsTyped
     * Designate the url as having been explicitly typed in by
     * the user, so it's okay to be an autocomplete result.
     */
  /* void markPageAsTyped (in nsIURI aURI); */
  NS_IMETHOD MarkPageAsTyped(nsIURI *aURI) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIBROWSERHISTORY \
  NS_IMETHOD AddPageWithDetails(nsIURI *aURI, const PRUnichar *aTitle, PRInt64 aLastVisited); \
  NS_IMETHOD GetLastPageVisited(nsACString & aLastPageVisited); \
  NS_IMETHOD GetCount(PRUint32 *aCount); \
  NS_IMETHOD RemovePage(nsIURI *aURI); \
  NS_IMETHOD RemovePagesFromHost(const nsACString & aHost, PRBool aEntireDomain); \
  NS_IMETHOD RemoveAllPages(void); \
  NS_IMETHOD HidePage(nsIURI *aURI); \
  NS_IMETHOD MarkPageAsTyped(nsIURI *aURI); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIBROWSERHISTORY(_to) \
  NS_IMETHOD AddPageWithDetails(nsIURI *aURI, const PRUnichar *aTitle, PRInt64 aLastVisited) { return _to AddPageWithDetails(aURI, aTitle, aLastVisited); } \
  NS_IMETHOD GetLastPageVisited(nsACString & aLastPageVisited) { return _to GetLastPageVisited(aLastPageVisited); } \
  NS_IMETHOD GetCount(PRUint32 *aCount) { return _to GetCount(aCount); } \
  NS_IMETHOD RemovePage(nsIURI *aURI) { return _to RemovePage(aURI); } \
  NS_IMETHOD RemovePagesFromHost(const nsACString & aHost, PRBool aEntireDomain) { return _to RemovePagesFromHost(aHost, aEntireDomain); } \
  NS_IMETHOD RemoveAllPages(void) { return _to RemoveAllPages(); } \
  NS_IMETHOD HidePage(nsIURI *aURI) { return _to HidePage(aURI); } \
  NS_IMETHOD MarkPageAsTyped(nsIURI *aURI) { return _to MarkPageAsTyped(aURI); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIBROWSERHISTORY(_to) \
  NS_IMETHOD AddPageWithDetails(nsIURI *aURI, const PRUnichar *aTitle, PRInt64 aLastVisited) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddPageWithDetails(aURI, aTitle, aLastVisited); } \
  NS_IMETHOD GetLastPageVisited(nsACString & aLastPageVisited) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLastPageVisited(aLastPageVisited); } \
  NS_IMETHOD GetCount(PRUint32 *aCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCount(aCount); } \
  NS_IMETHOD RemovePage(nsIURI *aURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemovePage(aURI); } \
  NS_IMETHOD RemovePagesFromHost(const nsACString & aHost, PRBool aEntireDomain) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemovePagesFromHost(aHost, aEntireDomain); } \
  NS_IMETHOD RemoveAllPages(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveAllPages(); } \
  NS_IMETHOD HidePage(nsIURI *aURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->HidePage(aURI); } \
  NS_IMETHOD MarkPageAsTyped(nsIURI *aURI) { return !_to ? NS_ERROR_NULL_POINTER : _to->MarkPageAsTyped(aURI); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsBrowserHistory : public nsIBrowserHistory
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIBROWSERHISTORY

  nsBrowserHistory();

private:
  ~nsBrowserHistory();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsBrowserHistory, nsIBrowserHistory)

nsBrowserHistory::nsBrowserHistory()
{
  /* member initializers and constructor code */
}

nsBrowserHistory::~nsBrowserHistory()
{
  /* destructor code */
}

/* void addPageWithDetails (in nsIURI aURI, in wstring aTitle, in long long aLastVisited); */
NS_IMETHODIMP nsBrowserHistory::AddPageWithDetails(nsIURI *aURI, const PRUnichar *aTitle, PRInt64 aLastVisited)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AUTF8String lastPageVisited; */
NS_IMETHODIMP nsBrowserHistory::GetLastPageVisited(nsACString & aLastPageVisited)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 count; */
NS_IMETHODIMP nsBrowserHistory::GetCount(PRUint32 *aCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removePage (in nsIURI aURI); */
NS_IMETHODIMP nsBrowserHistory::RemovePage(nsIURI *aURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removePagesFromHost (in AUTF8String aHost, in boolean aEntireDomain); */
NS_IMETHODIMP nsBrowserHistory::RemovePagesFromHost(const nsACString & aHost, PRBool aEntireDomain)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeAllPages (); */
NS_IMETHODIMP nsBrowserHistory::RemoveAllPages()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void hidePage (in nsIURI aURI); */
NS_IMETHODIMP nsBrowserHistory::HidePage(nsIURI *aURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void markPageAsTyped (in nsIURI aURI); */
NS_IMETHODIMP nsBrowserHistory::MarkPageAsTyped(nsIURI *aURI)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIBrowserHistory_h__ */
