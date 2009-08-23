/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/satchel/public/nsIFormHistory.idl
 */

#ifndef __gen_nsIFormHistory_h__
#define __gen_nsIFormHistory_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIFormHistory */
#define NS_IFORMHISTORY_IID_STR "fa5a871d-29f5-489e-aaa4-c266def52b79"

#define NS_IFORMHISTORY_IID \
  {0xfa5a871d, 0x29f5, 0x489e, \
    { 0xaa, 0xa4, 0xc2, 0x66, 0xde, 0xf5, 0x2b, 0x79 }}

class NS_NO_VTABLE nsIFormHistory : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IFORMHISTORY_IID)

  /**
   * The total number of rows in the form history.
   */
  /* readonly attribute unsigned long rowCount; */
  NS_IMETHOD GetRowCount(PRUint32 *aRowCount) = 0;

  /**
   * Gets the name and value at a position in the form history.
   */
  /* void getEntryAt (in unsigned long index, out AString name, out AString value); */
  NS_IMETHOD GetEntryAt(PRUint32 index, nsAString & name, nsAString & value) = 0;

  /**
   * Gets just the name at a position in the form history.
   */
  /* void getNameAt (in unsigned long index, out AString name); */
  NS_IMETHOD GetNameAt(PRUint32 index, nsAString & name) = 0;

  /**
   * Gets just the value at a position in the form history.
   */
  /* void getValueAt (in unsigned long index, out AString value); */
  NS_IMETHOD GetValueAt(PRUint32 index, nsAString & value) = 0;

  /**
   * Appends a name and value pair to the end of the form history.
   */
  /* void addEntry (in AString name, in AString value); */
  NS_IMETHOD AddEntry(const nsAString & name, const nsAString & value) = 0;

  /**
   * Removes the entry at a position.
   */
  /* void removeEntryAt (in unsigned long index); */
  NS_IMETHOD RemoveEntryAt(PRUint32 index) = 0;

  /**
   * Removes all entries that are paired with a name.
   */
  /* void removeEntriesForName (in AString name); */
  NS_IMETHOD RemoveEntriesForName(const nsAString & name) = 0;

  /**
   * Removes all entries in the entire form history.
   */
  /* void removeAllEntries (); */
  NS_IMETHOD RemoveAllEntries(void) = 0;

  /**
   * Returns true if there is no entry that is paired with a name.
   */
  /* boolean nameExists (in AString name); */
  NS_IMETHOD NameExists(const nsAString & name, PRBool *_retval) = 0;

  /**
   * Gets whether a name and value pair exists in the form history.
   */
  /* boolean entryExists (in AString name, in AString value); */
  NS_IMETHOD EntryExists(const nsAString & name, const nsAString & value, PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIFORMHISTORY \
  NS_IMETHOD GetRowCount(PRUint32 *aRowCount); \
  NS_IMETHOD GetEntryAt(PRUint32 index, nsAString & name, nsAString & value); \
  NS_IMETHOD GetNameAt(PRUint32 index, nsAString & name); \
  NS_IMETHOD GetValueAt(PRUint32 index, nsAString & value); \
  NS_IMETHOD AddEntry(const nsAString & name, const nsAString & value); \
  NS_IMETHOD RemoveEntryAt(PRUint32 index); \
  NS_IMETHOD RemoveEntriesForName(const nsAString & name); \
  NS_IMETHOD RemoveAllEntries(void); \
  NS_IMETHOD NameExists(const nsAString & name, PRBool *_retval); \
  NS_IMETHOD EntryExists(const nsAString & name, const nsAString & value, PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIFORMHISTORY(_to) \
  NS_IMETHOD GetRowCount(PRUint32 *aRowCount) { return _to GetRowCount(aRowCount); } \
  NS_IMETHOD GetEntryAt(PRUint32 index, nsAString & name, nsAString & value) { return _to GetEntryAt(index, name, value); } \
  NS_IMETHOD GetNameAt(PRUint32 index, nsAString & name) { return _to GetNameAt(index, name); } \
  NS_IMETHOD GetValueAt(PRUint32 index, nsAString & value) { return _to GetValueAt(index, value); } \
  NS_IMETHOD AddEntry(const nsAString & name, const nsAString & value) { return _to AddEntry(name, value); } \
  NS_IMETHOD RemoveEntryAt(PRUint32 index) { return _to RemoveEntryAt(index); } \
  NS_IMETHOD RemoveEntriesForName(const nsAString & name) { return _to RemoveEntriesForName(name); } \
  NS_IMETHOD RemoveAllEntries(void) { return _to RemoveAllEntries(); } \
  NS_IMETHOD NameExists(const nsAString & name, PRBool *_retval) { return _to NameExists(name, _retval); } \
  NS_IMETHOD EntryExists(const nsAString & name, const nsAString & value, PRBool *_retval) { return _to EntryExists(name, value, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIFORMHISTORY(_to) \
  NS_IMETHOD GetRowCount(PRUint32 *aRowCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRowCount(aRowCount); } \
  NS_IMETHOD GetEntryAt(PRUint32 index, nsAString & name, nsAString & value) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEntryAt(index, name, value); } \
  NS_IMETHOD GetNameAt(PRUint32 index, nsAString & name) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetNameAt(index, name); } \
  NS_IMETHOD GetValueAt(PRUint32 index, nsAString & value) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetValueAt(index, value); } \
  NS_IMETHOD AddEntry(const nsAString & name, const nsAString & value) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddEntry(name, value); } \
  NS_IMETHOD RemoveEntryAt(PRUint32 index) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveEntryAt(index); } \
  NS_IMETHOD RemoveEntriesForName(const nsAString & name) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveEntriesForName(name); } \
  NS_IMETHOD RemoveAllEntries(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveAllEntries(); } \
  NS_IMETHOD NameExists(const nsAString & name, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->NameExists(name, _retval); } \
  NS_IMETHOD EntryExists(const nsAString & name, const nsAString & value, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->EntryExists(name, value, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsFormHistory : public nsIFormHistory
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIFORMHISTORY

  nsFormHistory();

private:
  ~nsFormHistory();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsFormHistory, nsIFormHistory)

nsFormHistory::nsFormHistory()
{
  /* member initializers and constructor code */
}

nsFormHistory::~nsFormHistory()
{
  /* destructor code */
}

/* readonly attribute unsigned long rowCount; */
NS_IMETHODIMP nsFormHistory::GetRowCount(PRUint32 *aRowCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getEntryAt (in unsigned long index, out AString name, out AString value); */
NS_IMETHODIMP nsFormHistory::GetEntryAt(PRUint32 index, nsAString & name, nsAString & value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getNameAt (in unsigned long index, out AString name); */
NS_IMETHODIMP nsFormHistory::GetNameAt(PRUint32 index, nsAString & name)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getValueAt (in unsigned long index, out AString value); */
NS_IMETHODIMP nsFormHistory::GetValueAt(PRUint32 index, nsAString & value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addEntry (in AString name, in AString value); */
NS_IMETHODIMP nsFormHistory::AddEntry(const nsAString & name, const nsAString & value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeEntryAt (in unsigned long index); */
NS_IMETHODIMP nsFormHistory::RemoveEntryAt(PRUint32 index)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeEntriesForName (in AString name); */
NS_IMETHODIMP nsFormHistory::RemoveEntriesForName(const nsAString & name)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeAllEntries (); */
NS_IMETHODIMP nsFormHistory::RemoveAllEntries()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean nameExists (in AString name); */
NS_IMETHODIMP nsFormHistory::NameExists(const nsAString & name, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean entryExists (in AString name, in AString value); */
NS_IMETHODIMP nsFormHistory::EntryExists(const nsAString & name, const nsAString & value, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIFormHistory_h__ */
