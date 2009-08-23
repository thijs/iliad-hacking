/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/editor/idl/nsIPlaintextEditor.idl
 */

#ifndef __gen_nsIPlaintextEditor_h__
#define __gen_nsIPlaintextEditor_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include "nsIDOMKeyEvent.h"

/* starting interface:    nsIPlaintextEditor */
#define NS_IPLAINTEXTEDITOR_IID_STR "b5f39ed4-1dd1-11b2-9d00-fd54d6f54962"

#define NS_IPLAINTEXTEDITOR_IID \
  {0xb5f39ed4, 0x1dd1, 0x11b2, \
    { 0x9d, 0x00, 0xfd, 0x54, 0xd6, 0xf5, 0x49, 0x62 }}

class NS_NO_VTABLE nsIPlaintextEditor : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IPLAINTEXTEDITOR_IID)

  enum { eEditorPlaintextBit = 0 };

  enum { eEditorSingleLineBit = 1 };

  enum { eEditorPasswordBit = 2 };

  enum { eEditorReadonlyBit = 3 };

  enum { eEditorDisabledBit = 4 };

  enum { eEditorFilterInputBit = 5 };

  enum { eEditorMailBit = 6 };

  enum { eEditorUseAsyncUpdatesBit = 7 };

  enum { eEditorEnableWrapHackBit = 8 };

  enum { eEditorWidgetBit = 9 };

  enum { eEditorNoCSSBit = 10 };

  enum { eEditorPlaintextMask = 1 };

  enum { eEditorSingleLineMask = 2 };

  enum { eEditorPasswordMask = 4 };

  enum { eEditorReadonlyMask = 8 };

  enum { eEditorDisabledMask = 16 };

  enum { eEditorFilterInputMask = 32 };

  enum { eEditorMailMask = 64 };

  enum { eEditorUseAsyncUpdatesMask = 128 };

  enum { eEditorEnableWrapHackMask = 256 };

  enum { eEditorWidgetMask = 512 };

  enum { eEditorNoCSSMask = 1024 };

  /**
    * The length of the contents in characters.
    */
  /* readonly attribute long textLength; */
  NS_IMETHOD GetTextLength(PRInt32 *aTextLength) = 0;

  /**
    * The maximum number of characters allowed.
    */
  /* attribute long maxTextLength; */
  NS_IMETHOD GetMaxTextLength(PRInt32 *aMaxTextLength) = 0;
  NS_IMETHOD SetMaxTextLength(PRInt32 aMaxTextLength) = 0;

  /** Get and set the body wrap width.
    * 
    * Special values:
    *    0 = wrap to window width
    *   -1 = no wrap at all
    */
  /* attribute long wrapWidth; */
  NS_IMETHOD GetWrapWidth(PRInt32 *aWrapWidth) = 0;
  NS_IMETHOD SetWrapWidth(PRInt32 aWrapWidth) = 0;

  /** 
   * EditorKeyPress consumes a keyevent.
   * @param aKeyEvent    key event to consume
   */
  /* [noscript] void handleKeyPress (in nsIDOMKeyEvent aKeyEvent); */
  NS_IMETHOD HandleKeyPress(nsIDOMKeyEvent * aKeyEvent) = 0;

  /**
   * Inserts a string at the current location,
   * given by the selection.
   * If the selection is not collapsed, the selection is deleted
   * and the insertion takes place at the resulting collapsed selection.
   *
   * @param aString   the string to be inserted
   */
  /* void insertText (in DOMString aStringToInsert); */
  NS_IMETHOD InsertText(const nsAString & aStringToInsert) = 0;

  /**
   * Insert a line break into the content model.
   * The interpretation of a break is up to the implementation:
   * it may enter a character, split a node in the tree, etc.
   * This may be more efficient than calling InsertText with a newline.
   */
  /* void insertLineBreak (); */
  NS_IMETHOD InsertLineBreak(void) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPLAINTEXTEDITOR \
  NS_IMETHOD GetTextLength(PRInt32 *aTextLength); \
  NS_IMETHOD GetMaxTextLength(PRInt32 *aMaxTextLength); \
  NS_IMETHOD SetMaxTextLength(PRInt32 aMaxTextLength); \
  NS_IMETHOD GetWrapWidth(PRInt32 *aWrapWidth); \
  NS_IMETHOD SetWrapWidth(PRInt32 aWrapWidth); \
  NS_IMETHOD HandleKeyPress(nsIDOMKeyEvent * aKeyEvent); \
  NS_IMETHOD InsertText(const nsAString & aStringToInsert); \
  NS_IMETHOD InsertLineBreak(void); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPLAINTEXTEDITOR(_to) \
  NS_IMETHOD GetTextLength(PRInt32 *aTextLength) { return _to GetTextLength(aTextLength); } \
  NS_IMETHOD GetMaxTextLength(PRInt32 *aMaxTextLength) { return _to GetMaxTextLength(aMaxTextLength); } \
  NS_IMETHOD SetMaxTextLength(PRInt32 aMaxTextLength) { return _to SetMaxTextLength(aMaxTextLength); } \
  NS_IMETHOD GetWrapWidth(PRInt32 *aWrapWidth) { return _to GetWrapWidth(aWrapWidth); } \
  NS_IMETHOD SetWrapWidth(PRInt32 aWrapWidth) { return _to SetWrapWidth(aWrapWidth); } \
  NS_IMETHOD HandleKeyPress(nsIDOMKeyEvent * aKeyEvent) { return _to HandleKeyPress(aKeyEvent); } \
  NS_IMETHOD InsertText(const nsAString & aStringToInsert) { return _to InsertText(aStringToInsert); } \
  NS_IMETHOD InsertLineBreak(void) { return _to InsertLineBreak(); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPLAINTEXTEDITOR(_to) \
  NS_IMETHOD GetTextLength(PRInt32 *aTextLength) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTextLength(aTextLength); } \
  NS_IMETHOD GetMaxTextLength(PRInt32 *aMaxTextLength) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMaxTextLength(aMaxTextLength); } \
  NS_IMETHOD SetMaxTextLength(PRInt32 aMaxTextLength) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetMaxTextLength(aMaxTextLength); } \
  NS_IMETHOD GetWrapWidth(PRInt32 *aWrapWidth) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetWrapWidth(aWrapWidth); } \
  NS_IMETHOD SetWrapWidth(PRInt32 aWrapWidth) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetWrapWidth(aWrapWidth); } \
  NS_IMETHOD HandleKeyPress(nsIDOMKeyEvent * aKeyEvent) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleKeyPress(aKeyEvent); } \
  NS_IMETHOD InsertText(const nsAString & aStringToInsert) { return !_to ? NS_ERROR_NULL_POINTER : _to->InsertText(aStringToInsert); } \
  NS_IMETHOD InsertLineBreak(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->InsertLineBreak(); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsPlaintextEditor : public nsIPlaintextEditor
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPLAINTEXTEDITOR

  nsPlaintextEditor();

private:
  ~nsPlaintextEditor();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsPlaintextEditor, nsIPlaintextEditor)

nsPlaintextEditor::nsPlaintextEditor()
{
  /* member initializers and constructor code */
}

nsPlaintextEditor::~nsPlaintextEditor()
{
  /* destructor code */
}

/* readonly attribute long textLength; */
NS_IMETHODIMP nsPlaintextEditor::GetTextLength(PRInt32 *aTextLength)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute long maxTextLength; */
NS_IMETHODIMP nsPlaintextEditor::GetMaxTextLength(PRInt32 *aMaxTextLength)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsPlaintextEditor::SetMaxTextLength(PRInt32 aMaxTextLength)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute long wrapWidth; */
NS_IMETHODIMP nsPlaintextEditor::GetWrapWidth(PRInt32 *aWrapWidth)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsPlaintextEditor::SetWrapWidth(PRInt32 aWrapWidth)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void handleKeyPress (in nsIDOMKeyEvent aKeyEvent); */
NS_IMETHODIMP nsPlaintextEditor::HandleKeyPress(nsIDOMKeyEvent * aKeyEvent)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void insertText (in DOMString aStringToInsert); */
NS_IMETHODIMP nsPlaintextEditor::InsertText(const nsAString & aStringToInsert)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void insertLineBreak (); */
NS_IMETHODIMP nsPlaintextEditor::InsertLineBreak()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIPlaintextEditor_h__ */
