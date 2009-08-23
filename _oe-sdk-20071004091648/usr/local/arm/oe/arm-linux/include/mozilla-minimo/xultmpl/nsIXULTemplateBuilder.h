/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/content/xul/templates/public/nsIXULTemplateBuilder.idl
 */

#ifndef __gen_nsIXULTemplateBuilder_h__
#define __gen_nsIXULTemplateBuilder_h__


#ifndef __gen_domstubs_h__
#include "domstubs.h"
#endif

#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIRDFCompositeDataSource_h__
#include "nsIRDFCompositeDataSource.h"
#endif

#ifndef __gen_nsIRDFResource_h__
#include "nsIRDFResource.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIXULBuilderListener; /* forward declaration */


/* starting interface:    nsIXULTemplateBuilder */
#define NS_IXULTEMPLATEBUILDER_IID_STR "9da147a7-5854-49e3-a397-22ecdd93e96d"

#define NS_IXULTEMPLATEBUILDER_IID \
  {0x9da147a7, 0x5854, 0x49e3, \
    { 0xa3, 0x97, 0x22, 0xec, 0xdd, 0x93, 0xe9, 0x6d }}

class NS_NO_VTABLE nsIXULTemplateBuilder : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IXULTEMPLATEBUILDER_IID)

  /**
     * The ``root'' node in the DOM to which this builder is attached
     */
  /* readonly attribute nsIDOMElement root; */
  NS_IMETHOD GetRoot(nsIDOMElement * *aRoot) = 0;

  /**
     * The composite datasource that the template builder observes
     * and uses to create content
     */
  /* readonly attribute nsIRDFCompositeDataSource database; */
  NS_IMETHOD GetDatabase(nsIRDFCompositeDataSource * *aDatabase) = 0;

  /**
     * Force the template builder to rebuild its content.
     */
  /* void rebuild (); */
  NS_IMETHOD Rebuild(void) = 0;

  /**
     * Reload any of our RDF datasources that support nsIRDFRemoteDatasource. 
     *
     * @note This is a temporary hack so that remote-XUL authors can
     *       reload remote datasources. When RDF becomes remote-scriptable,
     *       this will no longer be necessary.
     */
  /* void refresh (); */
  NS_IMETHOD Refresh(void) = 0;

  /**
     * Called to initialize a XUL content builder on a particular root
     * element. This element presumably has a ``datasources''
     * attribute, which the builder will parse to set up the template
     * builder's datasources.
     */
  /* [noscript] void init (in nsIContent_ptr aElement); */
  NS_IMETHOD Init(nsIContent * aElement) = 0;

  /**
     * Invoked lazily by a XUL element that needs its child content
     * built.
     */
  /* [noscript] void createContents (in nsIContent_ptr aElement); */
  NS_IMETHOD CreateContents(nsIContent * aElement) = 0;

  /**
     * Add a listener to this template builder. The template builder
     * holds a strong reference to the listener.
     */
  /* void addListener (in nsIXULBuilderListener aListener); */
  NS_IMETHOD AddListener(nsIXULBuilderListener *aListener) = 0;

  /**
     * Remove a listener from this template builder.
     */
  /* void removeListener (in nsIXULBuilderListener aListener); */
  NS_IMETHOD RemoveListener(nsIXULBuilderListener *aListener) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXULTEMPLATEBUILDER \
  NS_IMETHOD GetRoot(nsIDOMElement * *aRoot); \
  NS_IMETHOD GetDatabase(nsIRDFCompositeDataSource * *aDatabase); \
  NS_IMETHOD Rebuild(void); \
  NS_IMETHOD Refresh(void); \
  NS_IMETHOD Init(nsIContent * aElement); \
  NS_IMETHOD CreateContents(nsIContent * aElement); \
  NS_IMETHOD AddListener(nsIXULBuilderListener *aListener); \
  NS_IMETHOD RemoveListener(nsIXULBuilderListener *aListener); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXULTEMPLATEBUILDER(_to) \
  NS_IMETHOD GetRoot(nsIDOMElement * *aRoot) { return _to GetRoot(aRoot); } \
  NS_IMETHOD GetDatabase(nsIRDFCompositeDataSource * *aDatabase) { return _to GetDatabase(aDatabase); } \
  NS_IMETHOD Rebuild(void) { return _to Rebuild(); } \
  NS_IMETHOD Refresh(void) { return _to Refresh(); } \
  NS_IMETHOD Init(nsIContent * aElement) { return _to Init(aElement); } \
  NS_IMETHOD CreateContents(nsIContent * aElement) { return _to CreateContents(aElement); } \
  NS_IMETHOD AddListener(nsIXULBuilderListener *aListener) { return _to AddListener(aListener); } \
  NS_IMETHOD RemoveListener(nsIXULBuilderListener *aListener) { return _to RemoveListener(aListener); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXULTEMPLATEBUILDER(_to) \
  NS_IMETHOD GetRoot(nsIDOMElement * *aRoot) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRoot(aRoot); } \
  NS_IMETHOD GetDatabase(nsIRDFCompositeDataSource * *aDatabase) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDatabase(aDatabase); } \
  NS_IMETHOD Rebuild(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Rebuild(); } \
  NS_IMETHOD Refresh(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Refresh(); } \
  NS_IMETHOD Init(nsIContent * aElement) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aElement); } \
  NS_IMETHOD CreateContents(nsIContent * aElement) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateContents(aElement); } \
  NS_IMETHOD AddListener(nsIXULBuilderListener *aListener) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddListener(aListener); } \
  NS_IMETHOD RemoveListener(nsIXULBuilderListener *aListener) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveListener(aListener); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXULTemplateBuilder : public nsIXULTemplateBuilder
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXULTEMPLATEBUILDER

  nsXULTemplateBuilder();

private:
  ~nsXULTemplateBuilder();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXULTemplateBuilder, nsIXULTemplateBuilder)

nsXULTemplateBuilder::nsXULTemplateBuilder()
{
  /* member initializers and constructor code */
}

nsXULTemplateBuilder::~nsXULTemplateBuilder()
{
  /* destructor code */
}

/* readonly attribute nsIDOMElement root; */
NS_IMETHODIMP nsXULTemplateBuilder::GetRoot(nsIDOMElement * *aRoot)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIRDFCompositeDataSource database; */
NS_IMETHODIMP nsXULTemplateBuilder::GetDatabase(nsIRDFCompositeDataSource * *aDatabase)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void rebuild (); */
NS_IMETHODIMP nsXULTemplateBuilder::Rebuild()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void refresh (); */
NS_IMETHODIMP nsXULTemplateBuilder::Refresh()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void init (in nsIContent_ptr aElement); */
NS_IMETHODIMP nsXULTemplateBuilder::Init(nsIContent * aElement)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void createContents (in nsIContent_ptr aElement); */
NS_IMETHODIMP nsXULTemplateBuilder::CreateContents(nsIContent * aElement)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addListener (in nsIXULBuilderListener aListener); */
NS_IMETHODIMP nsXULTemplateBuilder::AddListener(nsIXULBuilderListener *aListener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeListener (in nsIXULBuilderListener aListener); */
NS_IMETHODIMP nsXULTemplateBuilder::RemoveListener(nsIXULBuilderListener *aListener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXULTreeBuilderObserver */
#define NS_IXULTREEBUILDEROBSERVER_IID_STR "a5480e0d-ac7c-42e5-aca5-d7f0bbffa207"

#define NS_IXULTREEBUILDEROBSERVER_IID \
  {0xa5480e0d, 0xac7c, 0x42e5, \
    { 0xac, 0xa5, 0xd7, 0xf0, 0xbb, 0xff, 0xa2, 0x07 }}

/**
 * nsIXULTreeBuilderObserver
 *  This interface allows clients of the XULTreeBuilder to define domain 
 *  specific handling of specific nsITreeView methods that 
 *  XULTreeBuilder does not implement.
 */
class NS_NO_VTABLE nsIXULTreeBuilderObserver : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IXULTREEBUILDEROBSERVER_IID)

  enum { DROP_BEFORE = -1 };

  enum { DROP_ON = 0 };

  enum { DROP_AFTER = 1 };

  /**
     * Methods used by the drag feedback code to determine if a drag is allowable at
     * the current location. To get the behavior where drops are only allowed on
     * items, such as the mailNews folder pane, always return false whe
     * the orientation is not DROP_ON.
     */
  /* boolean canDrop (in long index, in long orientation); */
  NS_IMETHOD CanDrop(PRInt32 index, PRInt32 orientation, PRBool *_retval) = 0;

  /**
     * Called when the user drops something on this view. The |orientation| param
     * specifies before/on/after the given |row|.
     */
  /* void onDrop (in long row, in long orientation); */
  NS_IMETHOD OnDrop(PRInt32 row, PRInt32 orientation) = 0;

  /** 
     * Called when an item is opened or closed. 
     */
  /* void onToggleOpenState (in long index); */
  NS_IMETHOD OnToggleOpenState(PRInt32 index) = 0;

  /** 
	 * Called when a header is clicked.
     */
  /* void onCycleHeader (in wstring colID, in nsIDOMElement elt); */
  NS_IMETHOD OnCycleHeader(const PRUnichar *colID, nsIDOMElement *elt) = 0;

  /**
     * Called when a cell in a non-selectable cycling column (e.g. 
     * unread/flag/etc.) is clicked.
     */
  /* void onCycleCell (in long row, in wstring colID); */
  NS_IMETHOD OnCycleCell(PRInt32 row, const PRUnichar *colID) = 0;

  /** 
     * Called when selection in the tree changes
     */
  /* void onSelectionChanged (); */
  NS_IMETHOD OnSelectionChanged(void) = 0;

  /**
     * A command API that can be used to invoke commands on the selection.  
     * The tree will automatically invoke this method when certain keys 
     * are pressed.  For example, when the DEL key is pressed, performAction 
     * will be called with the "delete" string. 
     */
  /* void onPerformAction (in wstring action); */
  NS_IMETHOD OnPerformAction(const PRUnichar *action) = 0;

  /**
     * A command API that can be used to invoke commands on a specific row.
     */
  /* void onPerformActionOnRow (in wstring action, in long row); */
  NS_IMETHOD OnPerformActionOnRow(const PRUnichar *action, PRInt32 row) = 0;

  /**
     * A command API that can be used to invoke commands on a specific cell.
     */
  /* void onPerformActionOnCell (in wstring action, in long row, in wstring colID); */
  NS_IMETHOD OnPerformActionOnCell(const PRUnichar *action, PRInt32 row, const PRUnichar *colID) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXULTREEBUILDEROBSERVER \
  NS_IMETHOD CanDrop(PRInt32 index, PRInt32 orientation, PRBool *_retval); \
  NS_IMETHOD OnDrop(PRInt32 row, PRInt32 orientation); \
  NS_IMETHOD OnToggleOpenState(PRInt32 index); \
  NS_IMETHOD OnCycleHeader(const PRUnichar *colID, nsIDOMElement *elt); \
  NS_IMETHOD OnCycleCell(PRInt32 row, const PRUnichar *colID); \
  NS_IMETHOD OnSelectionChanged(void); \
  NS_IMETHOD OnPerformAction(const PRUnichar *action); \
  NS_IMETHOD OnPerformActionOnRow(const PRUnichar *action, PRInt32 row); \
  NS_IMETHOD OnPerformActionOnCell(const PRUnichar *action, PRInt32 row, const PRUnichar *colID); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXULTREEBUILDEROBSERVER(_to) \
  NS_IMETHOD CanDrop(PRInt32 index, PRInt32 orientation, PRBool *_retval) { return _to CanDrop(index, orientation, _retval); } \
  NS_IMETHOD OnDrop(PRInt32 row, PRInt32 orientation) { return _to OnDrop(row, orientation); } \
  NS_IMETHOD OnToggleOpenState(PRInt32 index) { return _to OnToggleOpenState(index); } \
  NS_IMETHOD OnCycleHeader(const PRUnichar *colID, nsIDOMElement *elt) { return _to OnCycleHeader(colID, elt); } \
  NS_IMETHOD OnCycleCell(PRInt32 row, const PRUnichar *colID) { return _to OnCycleCell(row, colID); } \
  NS_IMETHOD OnSelectionChanged(void) { return _to OnSelectionChanged(); } \
  NS_IMETHOD OnPerformAction(const PRUnichar *action) { return _to OnPerformAction(action); } \
  NS_IMETHOD OnPerformActionOnRow(const PRUnichar *action, PRInt32 row) { return _to OnPerformActionOnRow(action, row); } \
  NS_IMETHOD OnPerformActionOnCell(const PRUnichar *action, PRInt32 row, const PRUnichar *colID) { return _to OnPerformActionOnCell(action, row, colID); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXULTREEBUILDEROBSERVER(_to) \
  NS_IMETHOD CanDrop(PRInt32 index, PRInt32 orientation, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CanDrop(index, orientation, _retval); } \
  NS_IMETHOD OnDrop(PRInt32 row, PRInt32 orientation) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnDrop(row, orientation); } \
  NS_IMETHOD OnToggleOpenState(PRInt32 index) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnToggleOpenState(index); } \
  NS_IMETHOD OnCycleHeader(const PRUnichar *colID, nsIDOMElement *elt) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnCycleHeader(colID, elt); } \
  NS_IMETHOD OnCycleCell(PRInt32 row, const PRUnichar *colID) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnCycleCell(row, colID); } \
  NS_IMETHOD OnSelectionChanged(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnSelectionChanged(); } \
  NS_IMETHOD OnPerformAction(const PRUnichar *action) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnPerformAction(action); } \
  NS_IMETHOD OnPerformActionOnRow(const PRUnichar *action, PRInt32 row) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnPerformActionOnRow(action, row); } \
  NS_IMETHOD OnPerformActionOnCell(const PRUnichar *action, PRInt32 row, const PRUnichar *colID) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnPerformActionOnCell(action, row, colID); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXULTreeBuilderObserver : public nsIXULTreeBuilderObserver
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXULTREEBUILDEROBSERVER

  nsXULTreeBuilderObserver();

private:
  ~nsXULTreeBuilderObserver();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXULTreeBuilderObserver, nsIXULTreeBuilderObserver)

nsXULTreeBuilderObserver::nsXULTreeBuilderObserver()
{
  /* member initializers and constructor code */
}

nsXULTreeBuilderObserver::~nsXULTreeBuilderObserver()
{
  /* destructor code */
}

/* boolean canDrop (in long index, in long orientation); */
NS_IMETHODIMP nsXULTreeBuilderObserver::CanDrop(PRInt32 index, PRInt32 orientation, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onDrop (in long row, in long orientation); */
NS_IMETHODIMP nsXULTreeBuilderObserver::OnDrop(PRInt32 row, PRInt32 orientation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onToggleOpenState (in long index); */
NS_IMETHODIMP nsXULTreeBuilderObserver::OnToggleOpenState(PRInt32 index)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onCycleHeader (in wstring colID, in nsIDOMElement elt); */
NS_IMETHODIMP nsXULTreeBuilderObserver::OnCycleHeader(const PRUnichar *colID, nsIDOMElement *elt)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onCycleCell (in long row, in wstring colID); */
NS_IMETHODIMP nsXULTreeBuilderObserver::OnCycleCell(PRInt32 row, const PRUnichar *colID)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onSelectionChanged (); */
NS_IMETHODIMP nsXULTreeBuilderObserver::OnSelectionChanged()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onPerformAction (in wstring action); */
NS_IMETHODIMP nsXULTreeBuilderObserver::OnPerformAction(const PRUnichar *action)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onPerformActionOnRow (in wstring action, in long row); */
NS_IMETHODIMP nsXULTreeBuilderObserver::OnPerformActionOnRow(const PRUnichar *action, PRInt32 row)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onPerformActionOnCell (in wstring action, in long row, in wstring colID); */
NS_IMETHODIMP nsXULTreeBuilderObserver::OnPerformActionOnCell(const PRUnichar *action, PRInt32 row, const PRUnichar *colID)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIXULTreeBuilder */
#define NS_IXULTREEBUILDER_IID_STR "06b31b15-ebf5-4e74-a0e2-6bc0a18a3969"

#define NS_IXULTREEBUILDER_IID \
  {0x06b31b15, 0xebf5, 0x4e74, \
    { 0xa0, 0xe2, 0x6b, 0xc0, 0xa1, 0x8a, 0x39, 0x69 }}

class NS_NO_VTABLE nsIXULTreeBuilder : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IXULTREEBUILDER_IID)

  /**
     * Retrieve the RDF resource associated with the specified row.
     */
  /* nsIRDFResource getResourceAtIndex (in long aRowIndex); */
  NS_IMETHOD GetResourceAtIndex(PRInt32 aRowIndex, nsIRDFResource **_retval) = 0;

  /**
     * Retrieve the index associated with specified RDF resource.
     */
  /* long getIndexOfResource (in nsIRDFResource resource); */
  NS_IMETHOD GetIndexOfResource(nsIRDFResource *resource, PRInt32 *_retval) = 0;

  /** 
     * Add a Tree Builder Observer to handle Tree View 
     * methods that the base builder does not implement. 
     */
  /* void addObserver (in nsIXULTreeBuilderObserver aObserver); */
  NS_IMETHOD AddObserver(nsIXULTreeBuilderObserver *aObserver) = 0;

  /** 
     * Remove an Tree Builder Observer.
     */
  /* void removeObserver (in nsIXULTreeBuilderObserver aObserver); */
  NS_IMETHOD RemoveObserver(nsIXULTreeBuilderObserver *aObserver) = 0;

  /** 
     * Sort the contents of the tree using the specified column.
     */
  /* void sort (in nsIDOMElement aColumnElement); */
  NS_IMETHOD Sort(nsIDOMElement *aColumnElement) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXULTREEBUILDER \
  NS_IMETHOD GetResourceAtIndex(PRInt32 aRowIndex, nsIRDFResource **_retval); \
  NS_IMETHOD GetIndexOfResource(nsIRDFResource *resource, PRInt32 *_retval); \
  NS_IMETHOD AddObserver(nsIXULTreeBuilderObserver *aObserver); \
  NS_IMETHOD RemoveObserver(nsIXULTreeBuilderObserver *aObserver); \
  NS_IMETHOD Sort(nsIDOMElement *aColumnElement); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXULTREEBUILDER(_to) \
  NS_IMETHOD GetResourceAtIndex(PRInt32 aRowIndex, nsIRDFResource **_retval) { return _to GetResourceAtIndex(aRowIndex, _retval); } \
  NS_IMETHOD GetIndexOfResource(nsIRDFResource *resource, PRInt32 *_retval) { return _to GetIndexOfResource(resource, _retval); } \
  NS_IMETHOD AddObserver(nsIXULTreeBuilderObserver *aObserver) { return _to AddObserver(aObserver); } \
  NS_IMETHOD RemoveObserver(nsIXULTreeBuilderObserver *aObserver) { return _to RemoveObserver(aObserver); } \
  NS_IMETHOD Sort(nsIDOMElement *aColumnElement) { return _to Sort(aColumnElement); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXULTREEBUILDER(_to) \
  NS_IMETHOD GetResourceAtIndex(PRInt32 aRowIndex, nsIRDFResource **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetResourceAtIndex(aRowIndex, _retval); } \
  NS_IMETHOD GetIndexOfResource(nsIRDFResource *resource, PRInt32 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIndexOfResource(resource, _retval); } \
  NS_IMETHOD AddObserver(nsIXULTreeBuilderObserver *aObserver) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddObserver(aObserver); } \
  NS_IMETHOD RemoveObserver(nsIXULTreeBuilderObserver *aObserver) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveObserver(aObserver); } \
  NS_IMETHOD Sort(nsIDOMElement *aColumnElement) { return !_to ? NS_ERROR_NULL_POINTER : _to->Sort(aColumnElement); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXULTreeBuilder : public nsIXULTreeBuilder
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXULTREEBUILDER

  nsXULTreeBuilder();

private:
  ~nsXULTreeBuilder();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXULTreeBuilder, nsIXULTreeBuilder)

nsXULTreeBuilder::nsXULTreeBuilder()
{
  /* member initializers and constructor code */
}

nsXULTreeBuilder::~nsXULTreeBuilder()
{
  /* destructor code */
}

/* nsIRDFResource getResourceAtIndex (in long aRowIndex); */
NS_IMETHODIMP nsXULTreeBuilder::GetResourceAtIndex(PRInt32 aRowIndex, nsIRDFResource **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long getIndexOfResource (in nsIRDFResource resource); */
NS_IMETHODIMP nsXULTreeBuilder::GetIndexOfResource(nsIRDFResource *resource, PRInt32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addObserver (in nsIXULTreeBuilderObserver aObserver); */
NS_IMETHODIMP nsXULTreeBuilder::AddObserver(nsIXULTreeBuilderObserver *aObserver)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeObserver (in nsIXULTreeBuilderObserver aObserver); */
NS_IMETHODIMP nsXULTreeBuilder::RemoveObserver(nsIXULTreeBuilderObserver *aObserver)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void sort (in nsIDOMElement aColumnElement); */
NS_IMETHODIMP nsXULTreeBuilder::Sort(nsIDOMElement *aColumnElement)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIXULTemplateBuilder_h__ */
