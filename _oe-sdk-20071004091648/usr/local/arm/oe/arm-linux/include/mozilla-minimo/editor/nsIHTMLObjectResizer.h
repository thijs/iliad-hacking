/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/editor/idl/nsIHTMLObjectResizer.idl
 */

#ifndef __gen_nsIHTMLObjectResizer_h__
#define __gen_nsIHTMLObjectResizer_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_domstubs_h__
#include "domstubs.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIHTMLObjectResizeListener; /* forward declaration */


/* starting interface:    nsIHTMLObjectResizer */
#define NS_IHTMLOBJECTRESIZER_IID_STR "b0338f6c-ded3-4c39-a953-56e8bae494f5"

#define NS_IHTMLOBJECTRESIZER_IID \
  {0xb0338f6c, 0xded3, 0x4c39, \
    { 0xa9, 0x53, 0x56, 0xe8, 0xba, 0xe4, 0x94, 0xf5 }}

class nsIHTMLObjectResizer : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IHTMLOBJECTRESIZER_IID)

  typedef short EResizerLocation;
  enum { eTopLeft = 0 };

  enum { eTop = 1 };

  enum { eTopRight = 2 };

  enum { eLeft = 3 };

  enum { eRight = 4 };

  enum { eBottomLeft = 5 };

  enum { eBottom = 6 };

  enum { eBottomRight = 7 };

  /**
   * the element currently displaying resizers
   */
  /* readonly attribute nsIDOMElement resizedObject; */
  NS_IMETHOD GetResizedObject(nsIDOMElement * *aResizedObject) = 0;

  /**
   * a boolean indicating if object resizing is enabled in the editor
   */
  /* attribute boolean objectResizingEnabled; */
  NS_IMETHOD GetObjectResizingEnabled(PRBool *aObjectResizingEnabled) = 0;
  NS_IMETHOD SetObjectResizingEnabled(PRBool aObjectResizingEnabled) = 0;

  /**
   * Shows active resizers around an element's frame
   * @param aResizedElement [IN] a DOM Element
   */
  /* void showResizers (in nsIDOMElement aResizedElement); */
  NS_IMETHOD ShowResizers(nsIDOMElement *aResizedElement) = 0;

  /**
   * Hide resizers if they are visible
   */
  /* void hideResizers (); */
  NS_IMETHOD HideResizers(void) = 0;

  /**
   * Refresh visible resizers
   */
  /* void refreshResizers (); */
  NS_IMETHOD RefreshResizers(void) = 0;

  /**
   * event callback when a mouse button is pressed
   * @param aX      [IN] horizontal position of the pointer
   * @param aY      [IN] vertical position of the pointer
   * @param aTarget [IN] the element triggering the event
   */
  /* void mouseDown (in long aX, in long aY, in nsIDOMElement aTarget); */
  NS_IMETHOD MouseDown(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget) = 0;

  /**
   * event callback when a mouse button is released
   * @param aX      [IN] horizontal position of the pointer
   * @param aY      [IN] vertical position of the pointer
   * @param aTarget [IN] the element triggering the event
   */
  /* void mouseUp (in long aX, in long aY, in nsIDOMElement aTarget); */
  NS_IMETHOD MouseUp(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget) = 0;

  /**
   * event callback when the mouse pointer is moved
   * @param aMouseEvent [IN] the event
   */
  /* void mouseMove (in nsIDOMEvent aMouseEvent); */
  NS_IMETHOD MouseMove(nsIDOMEvent *aMouseEvent) = 0;

  /**
   * Creates a resize listener that can be used to get notifications
   * that the user started to resize an object or finalized such an operation
   * @param aListener [IN] an instance of nsIHTMLObjectResizeListener
   */
  /* void addObjectResizeEventListener (in nsIHTMLObjectResizeListener aListener); */
  NS_IMETHOD AddObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener) = 0;

  /**
   * Deletes a resize listener
   * @param aListener [IN] an instance of nsIHTMLObjectResizeListener
   */
  /* void removeObjectResizeEventListener (in nsIHTMLObjectResizeListener aListener); */
  NS_IMETHOD RemoveObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIHTMLOBJECTRESIZER \
  NS_IMETHOD GetResizedObject(nsIDOMElement * *aResizedObject); \
  NS_IMETHOD GetObjectResizingEnabled(PRBool *aObjectResizingEnabled); \
  NS_IMETHOD SetObjectResizingEnabled(PRBool aObjectResizingEnabled); \
  NS_IMETHOD ShowResizers(nsIDOMElement *aResizedElement); \
  NS_IMETHOD HideResizers(void); \
  NS_IMETHOD RefreshResizers(void); \
  NS_IMETHOD MouseDown(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget); \
  NS_IMETHOD MouseUp(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget); \
  NS_IMETHOD MouseMove(nsIDOMEvent *aMouseEvent); \
  NS_IMETHOD AddObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener); \
  NS_IMETHOD RemoveObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIHTMLOBJECTRESIZER(_to) \
  NS_IMETHOD GetResizedObject(nsIDOMElement * *aResizedObject) { return _to GetResizedObject(aResizedObject); } \
  NS_IMETHOD GetObjectResizingEnabled(PRBool *aObjectResizingEnabled) { return _to GetObjectResizingEnabled(aObjectResizingEnabled); } \
  NS_IMETHOD SetObjectResizingEnabled(PRBool aObjectResizingEnabled) { return _to SetObjectResizingEnabled(aObjectResizingEnabled); } \
  NS_IMETHOD ShowResizers(nsIDOMElement *aResizedElement) { return _to ShowResizers(aResizedElement); } \
  NS_IMETHOD HideResizers(void) { return _to HideResizers(); } \
  NS_IMETHOD RefreshResizers(void) { return _to RefreshResizers(); } \
  NS_IMETHOD MouseDown(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget) { return _to MouseDown(aX, aY, aTarget); } \
  NS_IMETHOD MouseUp(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget) { return _to MouseUp(aX, aY, aTarget); } \
  NS_IMETHOD MouseMove(nsIDOMEvent *aMouseEvent) { return _to MouseMove(aMouseEvent); } \
  NS_IMETHOD AddObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener) { return _to AddObjectResizeEventListener(aListener); } \
  NS_IMETHOD RemoveObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener) { return _to RemoveObjectResizeEventListener(aListener); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIHTMLOBJECTRESIZER(_to) \
  NS_IMETHOD GetResizedObject(nsIDOMElement * *aResizedObject) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetResizedObject(aResizedObject); } \
  NS_IMETHOD GetObjectResizingEnabled(PRBool *aObjectResizingEnabled) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetObjectResizingEnabled(aObjectResizingEnabled); } \
  NS_IMETHOD SetObjectResizingEnabled(PRBool aObjectResizingEnabled) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetObjectResizingEnabled(aObjectResizingEnabled); } \
  NS_IMETHOD ShowResizers(nsIDOMElement *aResizedElement) { return !_to ? NS_ERROR_NULL_POINTER : _to->ShowResizers(aResizedElement); } \
  NS_IMETHOD HideResizers(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->HideResizers(); } \
  NS_IMETHOD RefreshResizers(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->RefreshResizers(); } \
  NS_IMETHOD MouseDown(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget) { return !_to ? NS_ERROR_NULL_POINTER : _to->MouseDown(aX, aY, aTarget); } \
  NS_IMETHOD MouseUp(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget) { return !_to ? NS_ERROR_NULL_POINTER : _to->MouseUp(aX, aY, aTarget); } \
  NS_IMETHOD MouseMove(nsIDOMEvent *aMouseEvent) { return !_to ? NS_ERROR_NULL_POINTER : _to->MouseMove(aMouseEvent); } \
  NS_IMETHOD AddObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddObjectResizeEventListener(aListener); } \
  NS_IMETHOD RemoveObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener) { return !_to ? NS_ERROR_NULL_POINTER : _to->RemoveObjectResizeEventListener(aListener); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsHTMLObjectResizer : public nsIHTMLObjectResizer
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIHTMLOBJECTRESIZER

  nsHTMLObjectResizer();

private:
  ~nsHTMLObjectResizer();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsHTMLObjectResizer, nsIHTMLObjectResizer)

nsHTMLObjectResizer::nsHTMLObjectResizer()
{
  /* member initializers and constructor code */
}

nsHTMLObjectResizer::~nsHTMLObjectResizer()
{
  /* destructor code */
}

/* readonly attribute nsIDOMElement resizedObject; */
NS_IMETHODIMP nsHTMLObjectResizer::GetResizedObject(nsIDOMElement * *aResizedObject)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute boolean objectResizingEnabled; */
NS_IMETHODIMP nsHTMLObjectResizer::GetObjectResizingEnabled(PRBool *aObjectResizingEnabled)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsHTMLObjectResizer::SetObjectResizingEnabled(PRBool aObjectResizingEnabled)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void showResizers (in nsIDOMElement aResizedElement); */
NS_IMETHODIMP nsHTMLObjectResizer::ShowResizers(nsIDOMElement *aResizedElement)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void hideResizers (); */
NS_IMETHODIMP nsHTMLObjectResizer::HideResizers()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void refreshResizers (); */
NS_IMETHODIMP nsHTMLObjectResizer::RefreshResizers()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void mouseDown (in long aX, in long aY, in nsIDOMElement aTarget); */
NS_IMETHODIMP nsHTMLObjectResizer::MouseDown(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void mouseUp (in long aX, in long aY, in nsIDOMElement aTarget); */
NS_IMETHODIMP nsHTMLObjectResizer::MouseUp(PRInt32 aX, PRInt32 aY, nsIDOMElement *aTarget)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void mouseMove (in nsIDOMEvent aMouseEvent); */
NS_IMETHODIMP nsHTMLObjectResizer::MouseMove(nsIDOMEvent *aMouseEvent)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addObjectResizeEventListener (in nsIHTMLObjectResizeListener aListener); */
NS_IMETHODIMP nsHTMLObjectResizer::AddObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeObjectResizeEventListener (in nsIHTMLObjectResizeListener aListener); */
NS_IMETHODIMP nsHTMLObjectResizer::RemoveObjectResizeEventListener(nsIHTMLObjectResizeListener *aListener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIHTMLObjectResizer_h__ */
