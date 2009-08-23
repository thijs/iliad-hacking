/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/editor/composer/public/nsIEditingSession.idl
 */

#ifndef __gen_nsIEditingSession_h__
#define __gen_nsIEditingSession_h__


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
class nsIEditor; /* forward declaration */

class nsIEditingShell; /* forward declaration */


/* starting interface:    nsIEditingSession */
#define NS_IEDITINGSESSION_IID_STR "d39fd2b4-3978-45d2-a4be-ba448171b61b"

#define NS_IEDITINGSESSION_IID \
  {0xd39fd2b4, 0x3978, 0x45d2, \
    { 0xa4, 0xbe, 0xba, 0x44, 0x81, 0x71, 0xb6, 0x1b }}

class NS_NO_VTABLE nsIEditingSession : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IEDITINGSESSION_IID)

  /**
   *  Error codes when we fail to create an editor
   *  is placed in attribute editorStatus
   */
  enum { eEditorOK = 0 };

  enum { eEditorCreationInProgress = 1 };

  enum { eEditorErrorCantEditMimeType = 2 };

  enum { eEditorErrorFileNotFound = 3 };

  enum { eEditorErrorCantEditFramesets = 8 };

  enum { eEditorErrorUnknown = 9 };

  /**
   *  Status after editor creation and document loading
   *  Value is one of the above error codes
   */
  /* readonly attribute unsigned long editorStatus; */
  NS_IMETHOD GetEditorStatus(PRUint32 *aEditorStatus) = 0;

  /**
   *  Make this window editable
   *  @param aWindow nsIDOMWindow, the window the embedder needs to make editable
   *  @param aEditorType string, "html" "htmlsimple" "text" "textsimple"
   */
  /* void makeWindowEditable (in nsIDOMWindow window, in string aEditorType, in boolean doAfterUriLoad); */
  NS_IMETHOD MakeWindowEditable(nsIDOMWindow *window, const char *aEditorType, PRBool doAfterUriLoad) = 0;

  /**
   *  Test whether a specific window has had its editable flag set; it may have an editor
   *  now, or will get one after the uri load.
   *  
   *  Use this, passing the content root window, to test if we've set up editing
   *  for this content.
   */
  /* boolean windowIsEditable (in nsIDOMWindow window); */
  NS_IMETHOD WindowIsEditable(nsIDOMWindow *window, PRBool *_retval) = 0;

  /**
   *  Get the editor for this window. May return null
   */
  /* nsIEditor getEditorForWindow (in nsIDOMWindow window); */
  NS_IMETHOD GetEditorForWindow(nsIDOMWindow *window, nsIEditor **_retval) = 0;

  /** 
   *  Setup editor and related support objects
   */
  /* void setupEditorOnWindow (in nsIDOMWindow window); */
  NS_IMETHOD SetupEditorOnWindow(nsIDOMWindow *window) = 0;

  /** 
   *   Destroy editor and related support objects
   */
  /* void tearDownEditorOnWindow (in nsIDOMWindow window); */
  NS_IMETHOD TearDownEditorOnWindow(nsIDOMWindow *window) = 0;

  /* void setEditorOnControllers (in nsIDOMWindow aWindow, in nsIEditor aEditor); */
  NS_IMETHOD SetEditorOnControllers(nsIDOMWindow *aWindow, nsIEditor *aEditor) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIEDITINGSESSION \
  NS_IMETHOD GetEditorStatus(PRUint32 *aEditorStatus); \
  NS_IMETHOD MakeWindowEditable(nsIDOMWindow *window, const char *aEditorType, PRBool doAfterUriLoad); \
  NS_IMETHOD WindowIsEditable(nsIDOMWindow *window, PRBool *_retval); \
  NS_IMETHOD GetEditorForWindow(nsIDOMWindow *window, nsIEditor **_retval); \
  NS_IMETHOD SetupEditorOnWindow(nsIDOMWindow *window); \
  NS_IMETHOD TearDownEditorOnWindow(nsIDOMWindow *window); \
  NS_IMETHOD SetEditorOnControllers(nsIDOMWindow *aWindow, nsIEditor *aEditor); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIEDITINGSESSION(_to) \
  NS_IMETHOD GetEditorStatus(PRUint32 *aEditorStatus) { return _to GetEditorStatus(aEditorStatus); } \
  NS_IMETHOD MakeWindowEditable(nsIDOMWindow *window, const char *aEditorType, PRBool doAfterUriLoad) { return _to MakeWindowEditable(window, aEditorType, doAfterUriLoad); } \
  NS_IMETHOD WindowIsEditable(nsIDOMWindow *window, PRBool *_retval) { return _to WindowIsEditable(window, _retval); } \
  NS_IMETHOD GetEditorForWindow(nsIDOMWindow *window, nsIEditor **_retval) { return _to GetEditorForWindow(window, _retval); } \
  NS_IMETHOD SetupEditorOnWindow(nsIDOMWindow *window) { return _to SetupEditorOnWindow(window); } \
  NS_IMETHOD TearDownEditorOnWindow(nsIDOMWindow *window) { return _to TearDownEditorOnWindow(window); } \
  NS_IMETHOD SetEditorOnControllers(nsIDOMWindow *aWindow, nsIEditor *aEditor) { return _to SetEditorOnControllers(aWindow, aEditor); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIEDITINGSESSION(_to) \
  NS_IMETHOD GetEditorStatus(PRUint32 *aEditorStatus) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEditorStatus(aEditorStatus); } \
  NS_IMETHOD MakeWindowEditable(nsIDOMWindow *window, const char *aEditorType, PRBool doAfterUriLoad) { return !_to ? NS_ERROR_NULL_POINTER : _to->MakeWindowEditable(window, aEditorType, doAfterUriLoad); } \
  NS_IMETHOD WindowIsEditable(nsIDOMWindow *window, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->WindowIsEditable(window, _retval); } \
  NS_IMETHOD GetEditorForWindow(nsIDOMWindow *window, nsIEditor **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEditorForWindow(window, _retval); } \
  NS_IMETHOD SetupEditorOnWindow(nsIDOMWindow *window) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetupEditorOnWindow(window); } \
  NS_IMETHOD TearDownEditorOnWindow(nsIDOMWindow *window) { return !_to ? NS_ERROR_NULL_POINTER : _to->TearDownEditorOnWindow(window); } \
  NS_IMETHOD SetEditorOnControllers(nsIDOMWindow *aWindow, nsIEditor *aEditor) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetEditorOnControllers(aWindow, aEditor); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsEditingSession : public nsIEditingSession
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIEDITINGSESSION

  nsEditingSession();

private:
  ~nsEditingSession();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsEditingSession, nsIEditingSession)

nsEditingSession::nsEditingSession()
{
  /* member initializers and constructor code */
}

nsEditingSession::~nsEditingSession()
{
  /* destructor code */
}

/* readonly attribute unsigned long editorStatus; */
NS_IMETHODIMP nsEditingSession::GetEditorStatus(PRUint32 *aEditorStatus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void makeWindowEditable (in nsIDOMWindow window, in string aEditorType, in boolean doAfterUriLoad); */
NS_IMETHODIMP nsEditingSession::MakeWindowEditable(nsIDOMWindow *window, const char *aEditorType, PRBool doAfterUriLoad)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean windowIsEditable (in nsIDOMWindow window); */
NS_IMETHODIMP nsEditingSession::WindowIsEditable(nsIDOMWindow *window, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIEditor getEditorForWindow (in nsIDOMWindow window); */
NS_IMETHODIMP nsEditingSession::GetEditorForWindow(nsIDOMWindow *window, nsIEditor **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setupEditorOnWindow (in nsIDOMWindow window); */
NS_IMETHODIMP nsEditingSession::SetupEditorOnWindow(nsIDOMWindow *window)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void tearDownEditorOnWindow (in nsIDOMWindow window); */
NS_IMETHODIMP nsEditingSession::TearDownEditorOnWindow(nsIDOMWindow *window)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setEditorOnControllers (in nsIDOMWindow aWindow, in nsIEditor aEditor); */
NS_IMETHODIMP nsEditingSession::SetEditorOnControllers(nsIDOMWindow *aWindow, nsIEditor *aEditor)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIEditingSession_h__ */
