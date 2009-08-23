/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/content/xul/templates/public/nsIXULSortService.idl
 */

#ifndef __gen_nsIXULSortService_h__
#define __gen_nsIXULSortService_h__


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
class nsRDFSortState;
class nsIContent; /* forward declaration */

class nsIDOMNode; /* forward declaration */


/* starting interface:    nsIXULSortService */
#define NS_IXULSORTSERVICE_IID_STR "bfd05261-834c-11d2-8eac-00805f29f371"

#define NS_IXULSORTSERVICE_IID \
  {0xbfd05261, 0x834c, 0x11d2, \
    { 0x8e, 0xac, 0x00, 0x80, 0x5f, 0x29, 0xf3, 0x71 }}

/**
 * A service used to sort the contents of a XUL widget.
 */
class NS_NO_VTABLE nsIXULSortService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IXULSORTSERVICE_IID)

  /**
     *
     * Sort the contents of the widget containing <code>aNode</code>
     * using <code>aSortResource</code> as the comparison key, and
     * <code>aSortDirection</code> as the direction.
     *
     * @param aNode A node in the XUL widget whose children are to be
     * sorted. <code>sort</code> will traverse upwards to find the
     * root node at which to begin the actualy sorting. For optimal
     * results, pass in the root of the widget.
     *
     * @param aSortResource The RDF resource to be used as
     * the comparison key.
     *
     * @param aSortDirection May be either <b>natural</b> to return
     * the contents to their natural (unsorted) order,
     * <b>ascending</b> to sort the contents in ascending order, or
     * <b>descending</b> to sort the contents in descending order.
     */
  /* void sort (in nsIDOMNode aNode, in AString aSortResource, in AString aSortDirection); */
  NS_IMETHOD Sort(nsIDOMNode *aNode, const nsAString & aSortResource, const nsAString & aSortDirection) = 0;

  /**
     * Used internally for insertion sorting.
     */
  /* [noscript] void insertContainerNode (in nsIRDFCompositeDataSource db, in nsRDFSortState sortStatePtr, in nsIContent root, in nsIContent trueParent, in nsIContent container, in nsIContent node, in boolean aNotify); */
  NS_IMETHOD InsertContainerNode(nsIRDFCompositeDataSource *db, nsRDFSortState * sortStatePtr, nsIContent *root, nsIContent *trueParent, nsIContent *container, nsIContent *node, PRBool aNotify) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIXULSORTSERVICE \
  NS_IMETHOD Sort(nsIDOMNode *aNode, const nsAString & aSortResource, const nsAString & aSortDirection); \
  NS_IMETHOD InsertContainerNode(nsIRDFCompositeDataSource *db, nsRDFSortState * sortStatePtr, nsIContent *root, nsIContent *trueParent, nsIContent *container, nsIContent *node, PRBool aNotify); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIXULSORTSERVICE(_to) \
  NS_IMETHOD Sort(nsIDOMNode *aNode, const nsAString & aSortResource, const nsAString & aSortDirection) { return _to Sort(aNode, aSortResource, aSortDirection); } \
  NS_IMETHOD InsertContainerNode(nsIRDFCompositeDataSource *db, nsRDFSortState * sortStatePtr, nsIContent *root, nsIContent *trueParent, nsIContent *container, nsIContent *node, PRBool aNotify) { return _to InsertContainerNode(db, sortStatePtr, root, trueParent, container, node, aNotify); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIXULSORTSERVICE(_to) \
  NS_IMETHOD Sort(nsIDOMNode *aNode, const nsAString & aSortResource, const nsAString & aSortDirection) { return !_to ? NS_ERROR_NULL_POINTER : _to->Sort(aNode, aSortResource, aSortDirection); } \
  NS_IMETHOD InsertContainerNode(nsIRDFCompositeDataSource *db, nsRDFSortState * sortStatePtr, nsIContent *root, nsIContent *trueParent, nsIContent *container, nsIContent *node, PRBool aNotify) { return !_to ? NS_ERROR_NULL_POINTER : _to->InsertContainerNode(db, sortStatePtr, root, trueParent, container, node, aNotify); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsXULSortService : public nsIXULSortService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXULSORTSERVICE

  nsXULSortService();

private:
  ~nsXULSortService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsXULSortService, nsIXULSortService)

nsXULSortService::nsXULSortService()
{
  /* member initializers and constructor code */
}

nsXULSortService::~nsXULSortService()
{
  /* destructor code */
}

/* void sort (in nsIDOMNode aNode, in AString aSortResource, in AString aSortDirection); */
NS_IMETHODIMP nsXULSortService::Sort(nsIDOMNode *aNode, const nsAString & aSortResource, const nsAString & aSortDirection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void insertContainerNode (in nsIRDFCompositeDataSource db, in nsRDFSortState sortStatePtr, in nsIContent root, in nsIContent trueParent, in nsIContent container, in nsIContent node, in boolean aNotify); */
NS_IMETHODIMP nsXULSortService::InsertContainerNode(nsIRDFCompositeDataSource *db, nsRDFSortState * sortStatePtr, nsIContent *root, nsIContent *trueParent, nsIContent *container, nsIContent *node, PRBool aNotify)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

nsresult
NS_NewXULSortService(nsIXULSortService **result);

#endif /* __gen_nsIXULSortService_h__ */
