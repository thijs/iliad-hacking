/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/docshell/shistory/public/nsISHistoryInternal.idl
 */

#ifndef __gen_nsISHistoryInternal_h__
#define __gen_nsISHistoryInternal_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIFactory_h__
#include "nsIFactory.h"
#endif

#ifndef __gen_nsISHEntry_h__
#include "nsISHEntry.h"
#endif

#ifndef __gen_nsISHTransaction_h__
#include "nsISHTransaction.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISHistoryListener; /* forward declaration */

class nsIDocShell; /* forward declaration */

#define NS_SHISTORY_INTERNAL_CID \
{0x5b4cba4c, 0xbf67, 0x499a, {0xae, 0x2c, 0x3f, 0x76, 0x65, 0x6f, 0x4a, 0x4e}}
#define NS_SHISTORY_INTERNAL_CONTRACTID "@mozilla.org/browser/shistory-internal;1"

/* starting interface:    nsISHistoryInternal */
#define NS_ISHISTORYINTERNAL_IID_STR "df8788d6-c0ed-4517-b47e-c719afc94284"

#define NS_ISHISTORYINTERNAL_IID \
  {0xdf8788d6, 0xc0ed, 0x4517, \
    { 0xb4, 0x7e, 0xc7, 0x19, 0xaf, 0xc9, 0x42, 0x84 }}

class NS_NO_VTABLE nsISHistoryInternal : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISHISTORYINTERNAL_IID)

  /**
   * Add a new Entry to the History List
   * @param aEntry - The entry to add
   * @param aPersist - If true this specifies that the entry should persist
   * in the list.  If false, this means that when new entries are added
   * this element will not appear in the session history list.
   */
  /* void addEntry (in nsISHEntry aEntry, in boolean aPersist); */
  NS_IMETHOD AddEntry(nsISHEntry *aEntry, PRBool aPersist) = 0;

  /**
   * Get the root transaction
   */
  /* readonly attribute nsISHTransaction rootTransaction; */
  NS_IMETHOD GetRootTransaction(nsISHTransaction * *aRootTransaction) = 0;

  /**
   * The toplevel docshell object to which this SHistory object belongs to.
   */
  /* attribute nsIDocShell rootDocShell; */
  NS_IMETHOD GetRootDocShell(nsIDocShell * *aRootDocShell) = 0;
  NS_IMETHOD SetRootDocShell(nsIDocShell * aRootDocShell) = 0;

  /** 
   * Update the index maintained by sessionHistory
   */
  /* void updateIndex (); */
  NS_IMETHOD UpdateIndex(void) = 0;

  /**
   * Replace the nsISHEntry at a particular index
   * @param aIndex - The index at which the entry shoud be replaced
   * @param aReplaceEntry - The replacement entry for the index.
   */
  /* void replaceEntry (in long aIndex, in nsISHEntry aReplaceEntry); */
  NS_IMETHOD ReplaceEntry(PRInt32 aIndex, nsISHEntry *aReplaceEntry) = 0;

  /** 
   * Get handle to the history listener
   */
  /* readonly attribute nsISHistoryListener listener; */
  NS_IMETHOD GetListener(nsISHistoryListener * *aListener) = 0;

  /**
   * Evict content viewers until the number of content viewers per tab
   * is no more than gHistoryMaxViewers.  Also, count
   * total number of content viewers globally and evict one if we are over
   * our total max.  This is always called in Show(), after we destroy
   * the previous viewer.
   */
  /* void evictContentViewers (in long previousIndex, in long index); */
  NS_IMETHOD EvictContentViewers(PRInt32 previousIndex, PRInt32 index) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISHISTORYINTERNAL \
  NS_IMETHOD AddEntry(nsISHEntry *aEntry, PRBool aPersist); \
  NS_IMETHOD GetRootTransaction(nsISHTransaction * *aRootTransaction); \
  NS_IMETHOD GetRootDocShell(nsIDocShell * *aRootDocShell); \
  NS_IMETHOD SetRootDocShell(nsIDocShell * aRootDocShell); \
  NS_IMETHOD UpdateIndex(void); \
  NS_IMETHOD ReplaceEntry(PRInt32 aIndex, nsISHEntry *aReplaceEntry); \
  NS_IMETHOD GetListener(nsISHistoryListener * *aListener); \
  NS_IMETHOD EvictContentViewers(PRInt32 previousIndex, PRInt32 index); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISHISTORYINTERNAL(_to) \
  NS_IMETHOD AddEntry(nsISHEntry *aEntry, PRBool aPersist) { return _to AddEntry(aEntry, aPersist); } \
  NS_IMETHOD GetRootTransaction(nsISHTransaction * *aRootTransaction) { return _to GetRootTransaction(aRootTransaction); } \
  NS_IMETHOD GetRootDocShell(nsIDocShell * *aRootDocShell) { return _to GetRootDocShell(aRootDocShell); } \
  NS_IMETHOD SetRootDocShell(nsIDocShell * aRootDocShell) { return _to SetRootDocShell(aRootDocShell); } \
  NS_IMETHOD UpdateIndex(void) { return _to UpdateIndex(); } \
  NS_IMETHOD ReplaceEntry(PRInt32 aIndex, nsISHEntry *aReplaceEntry) { return _to ReplaceEntry(aIndex, aReplaceEntry); } \
  NS_IMETHOD GetListener(nsISHistoryListener * *aListener) { return _to GetListener(aListener); } \
  NS_IMETHOD EvictContentViewers(PRInt32 previousIndex, PRInt32 index) { return _to EvictContentViewers(previousIndex, index); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISHISTORYINTERNAL(_to) \
  NS_IMETHOD AddEntry(nsISHEntry *aEntry, PRBool aPersist) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddEntry(aEntry, aPersist); } \
  NS_IMETHOD GetRootTransaction(nsISHTransaction * *aRootTransaction) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRootTransaction(aRootTransaction); } \
  NS_IMETHOD GetRootDocShell(nsIDocShell * *aRootDocShell) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRootDocShell(aRootDocShell); } \
  NS_IMETHOD SetRootDocShell(nsIDocShell * aRootDocShell) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetRootDocShell(aRootDocShell); } \
  NS_IMETHOD UpdateIndex(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->UpdateIndex(); } \
  NS_IMETHOD ReplaceEntry(PRInt32 aIndex, nsISHEntry *aReplaceEntry) { return !_to ? NS_ERROR_NULL_POINTER : _to->ReplaceEntry(aIndex, aReplaceEntry); } \
  NS_IMETHOD GetListener(nsISHistoryListener * *aListener) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetListener(aListener); } \
  NS_IMETHOD EvictContentViewers(PRInt32 previousIndex, PRInt32 index) { return !_to ? NS_ERROR_NULL_POINTER : _to->EvictContentViewers(previousIndex, index); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSHistoryInternal : public nsISHistoryInternal
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISHISTORYINTERNAL

  nsSHistoryInternal();

private:
  ~nsSHistoryInternal();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSHistoryInternal, nsISHistoryInternal)

nsSHistoryInternal::nsSHistoryInternal()
{
  /* member initializers and constructor code */
}

nsSHistoryInternal::~nsSHistoryInternal()
{
  /* destructor code */
}

/* void addEntry (in nsISHEntry aEntry, in boolean aPersist); */
NS_IMETHODIMP nsSHistoryInternal::AddEntry(nsISHEntry *aEntry, PRBool aPersist)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISHTransaction rootTransaction; */
NS_IMETHODIMP nsSHistoryInternal::GetRootTransaction(nsISHTransaction * *aRootTransaction)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsIDocShell rootDocShell; */
NS_IMETHODIMP nsSHistoryInternal::GetRootDocShell(nsIDocShell * *aRootDocShell)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSHistoryInternal::SetRootDocShell(nsIDocShell * aRootDocShell)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void updateIndex (); */
NS_IMETHODIMP nsSHistoryInternal::UpdateIndex()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void replaceEntry (in long aIndex, in nsISHEntry aReplaceEntry); */
NS_IMETHODIMP nsSHistoryInternal::ReplaceEntry(PRInt32 aIndex, nsISHEntry *aReplaceEntry)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISHistoryListener listener; */
NS_IMETHODIMP nsSHistoryInternal::GetListener(nsISHistoryListener * *aListener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void evictContentViewers (in long previousIndex, in long index); */
NS_IMETHODIMP nsSHistoryInternal::EvictContentViewers(PRInt32 previousIndex, PRInt32 index)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsISHistoryInternal_h__ */
