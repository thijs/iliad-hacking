/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIInterfaceInfoToIDL.idl
 */

#ifndef __gen_nsIInterfaceInfoToIDL_h__
#define __gen_nsIInterfaceInfoToIDL_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIInterfaceInfoToIDL */
#define NS_IINTERFACEINFOTOIDL_IID_STR "b01eb40c-026b-49c9-af55-25e8c9d034c8"

#define NS_IINTERFACEINFOTOIDL_IID \
  {0xb01eb40c, 0x026b, 0x49c9, \
    { 0xaf, 0x55, 0x25, 0xe8, 0xc9, 0xd0, 0x34, 0xc8 }}

class NS_NO_VTABLE nsIInterfaceInfoToIDL : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IINTERFACEINFOTOIDL_IID)

  /* string generateIDL (in nsIIDRef aIID, in PRBool withIncludes, in PRBool withForwardDeclarations); */
  NS_IMETHOD GenerateIDL(const nsIID & aIID, PRBool withIncludes, PRBool withForwardDeclarations, char **_retval) = 0;

  /* void getReferencedInterfaceNames (in nsIIDRef aIID, out PRUint32 aArrayLength, [array, size_is (aArrayLength), retval] out string aNames); */
  NS_IMETHOD GetReferencedInterfaceNames(const nsIID & aIID, PRUint32 *aArrayLength, char ***aNames) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIINTERFACEINFOTOIDL \
  NS_IMETHOD GenerateIDL(const nsIID & aIID, PRBool withIncludes, PRBool withForwardDeclarations, char **_retval); \
  NS_IMETHOD GetReferencedInterfaceNames(const nsIID & aIID, PRUint32 *aArrayLength, char ***aNames); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIINTERFACEINFOTOIDL(_to) \
  NS_IMETHOD GenerateIDL(const nsIID & aIID, PRBool withIncludes, PRBool withForwardDeclarations, char **_retval) { return _to GenerateIDL(aIID, withIncludes, withForwardDeclarations, _retval); } \
  NS_IMETHOD GetReferencedInterfaceNames(const nsIID & aIID, PRUint32 *aArrayLength, char ***aNames) { return _to GetReferencedInterfaceNames(aIID, aArrayLength, aNames); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIINTERFACEINFOTOIDL(_to) \
  NS_IMETHOD GenerateIDL(const nsIID & aIID, PRBool withIncludes, PRBool withForwardDeclarations, char **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GenerateIDL(aIID, withIncludes, withForwardDeclarations, _retval); } \
  NS_IMETHOD GetReferencedInterfaceNames(const nsIID & aIID, PRUint32 *aArrayLength, char ***aNames) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetReferencedInterfaceNames(aIID, aArrayLength, aNames); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsInterfaceInfoToIDL : public nsIInterfaceInfoToIDL
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIINTERFACEINFOTOIDL

  nsInterfaceInfoToIDL();

private:
  ~nsInterfaceInfoToIDL();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsInterfaceInfoToIDL, nsIInterfaceInfoToIDL)

nsInterfaceInfoToIDL::nsInterfaceInfoToIDL()
{
  /* member initializers and constructor code */
}

nsInterfaceInfoToIDL::~nsInterfaceInfoToIDL()
{
  /* destructor code */
}

/* string generateIDL (in nsIIDRef aIID, in PRBool withIncludes, in PRBool withForwardDeclarations); */
NS_IMETHODIMP nsInterfaceInfoToIDL::GenerateIDL(const nsIID & aIID, PRBool withIncludes, PRBool withForwardDeclarations, char **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getReferencedInterfaceNames (in nsIIDRef aIID, out PRUint32 aArrayLength, [array, size_is (aArrayLength), retval] out string aNames); */
NS_IMETHODIMP nsInterfaceInfoToIDL::GetReferencedInterfaceNames(const nsIID & aIID, PRUint32 *aArrayLength, char ***aNames)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIInterfaceInfoToIDL_h__ */
