/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIGenericInterfaceInfoSet.idl
 */

#ifndef __gen_nsIGenericInterfaceInfoSet_h__
#define __gen_nsIGenericInterfaceInfoSet_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIInterfaceInfo_h__
#include "nsIInterfaceInfo.h"
#endif

#ifndef __gen_nsIInterfaceInfoManager_h__
#include "nsIInterfaceInfoManager.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIGenericInterfaceInfo */
#define NS_IGENERICINTERFACEINFO_IID_STR "8344a200-18ed-4538-8d44-e50b5156b564"

#define NS_IGENERICINTERFACEINFO_IID \
  {0x8344a200, 0x18ed, 0x4538, \
    { 0x8d, 0x44, 0xe5, 0x0b, 0x51, 0x56, 0xb5, 0x64 }}

class NS_NO_VTABLE nsIGenericInterfaceInfo : public nsIInterfaceInfo {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IGENERICINTERFACEINFO_IID)

  /**
    * All members except params* and result* copied
    */
  /* PRUint16 appendMethod (in XPTMethodDescriptorPtr aMethod); */
  NS_IMETHOD AppendMethod(XPTMethodDescriptor * aMethod, PRUint16 *_retval) = 0;

  /**
    * All members copied
    */
  /* PRUint16 appendConst (in XPTConstDescriptorPtr aConst); */
  NS_IMETHOD AppendConst(XPTConstDescriptor * aConst, PRUint16 *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIGENERICINTERFACEINFO \
  NS_IMETHOD AppendMethod(XPTMethodDescriptor * aMethod, PRUint16 *_retval); \
  NS_IMETHOD AppendConst(XPTConstDescriptor * aConst, PRUint16 *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIGENERICINTERFACEINFO(_to) \
  NS_IMETHOD AppendMethod(XPTMethodDescriptor * aMethod, PRUint16 *_retval) { return _to AppendMethod(aMethod, _retval); } \
  NS_IMETHOD AppendConst(XPTConstDescriptor * aConst, PRUint16 *_retval) { return _to AppendConst(aConst, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIGENERICINTERFACEINFO(_to) \
  NS_IMETHOD AppendMethod(XPTMethodDescriptor * aMethod, PRUint16 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->AppendMethod(aMethod, _retval); } \
  NS_IMETHOD AppendConst(XPTConstDescriptor * aConst, PRUint16 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->AppendConst(aConst, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsGenericInterfaceInfo : public nsIGenericInterfaceInfo
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIGENERICINTERFACEINFO

  nsGenericInterfaceInfo();

private:
  ~nsGenericInterfaceInfo();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsGenericInterfaceInfo, nsIGenericInterfaceInfo)

nsGenericInterfaceInfo::nsGenericInterfaceInfo()
{
  /* member initializers and constructor code */
}

nsGenericInterfaceInfo::~nsGenericInterfaceInfo()
{
  /* destructor code */
}

/* PRUint16 appendMethod (in XPTMethodDescriptorPtr aMethod); */
NS_IMETHODIMP nsGenericInterfaceInfo::AppendMethod(XPTMethodDescriptor * aMethod, PRUint16 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint16 appendConst (in XPTConstDescriptorPtr aConst); */
NS_IMETHODIMP nsGenericInterfaceInfo::AppendConst(XPTConstDescriptor * aConst, PRUint16 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIGenericInterfaceInfoSet */
#define NS_IGENERICINTERFACEINFOSET_IID_STR "8cc674ee-52ba-45fa-b897-bb88d35eaa91"

#define NS_IGENERICINTERFACEINFOSET_IID \
  {0x8cc674ee, 0x52ba, 0x45fa, \
    { 0xb8, 0x97, 0xbb, 0x88, 0xd3, 0x5e, 0xaa, 0x91 }}

class NS_NO_VTABLE nsIGenericInterfaceInfoSet : public nsIInterfaceInfoManager {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IGENERICINTERFACEINFOSET_IID)

  /**
    * Methods include pointers to param arrays allocated here.
    */
  /* XPTParamDescriptorPtr allocateParamArray (in PRUint16 aCount); */
  NS_IMETHOD AllocateParamArray(PRUint16 aCount, XPTParamDescriptor * *_retval) = 0;

  /**
    * Used for type that is in an array.
    */
  /* XPTTypeDescriptorPtr allocateAdditionalType (out PRUint16 aIndex); */
  NS_IMETHOD AllocateAdditionalType(PRUint16 *aIndex, XPTTypeDescriptor * *_retval) = 0;

  /* PRUint16 createAndAppendInterface (in string aName, in nsIIDRef aIID, in PRUint16 aParent, in PRUint8 aFlags, out nsIGenericInterfaceInfo aInfo); */
  NS_IMETHOD CreateAndAppendInterface(const char *aName, const nsIID & aIID, PRUint16 aParent, PRUint8 aFlags, nsIGenericInterfaceInfo **aInfo, PRUint16 *_retval) = 0;

  /* PRUint16 appendExternalInterface (in nsIInterfaceInfo aInfo); */
  NS_IMETHOD AppendExternalInterface(nsIInterfaceInfo *aInfo, PRUint16 *_retval) = 0;

  /* PRUint16 indexOf (in nsIIDRef aIID); */
  NS_IMETHOD IndexOf(const nsIID & aIID, PRUint16 *_retval) = 0;

  /* PRUint16 indexOfByName (in string aName); */
  NS_IMETHOD IndexOfByName(const char *aName, PRUint16 *_retval) = 0;

  /* nsIInterfaceInfo interfaceInfoAt (in PRUint16 aIndex); */
  NS_IMETHOD InterfaceInfoAt(PRUint16 aIndex, nsIInterfaceInfo **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIGENERICINTERFACEINFOSET \
  NS_IMETHOD AllocateParamArray(PRUint16 aCount, XPTParamDescriptor * *_retval); \
  NS_IMETHOD AllocateAdditionalType(PRUint16 *aIndex, XPTTypeDescriptor * *_retval); \
  NS_IMETHOD CreateAndAppendInterface(const char *aName, const nsIID & aIID, PRUint16 aParent, PRUint8 aFlags, nsIGenericInterfaceInfo **aInfo, PRUint16 *_retval); \
  NS_IMETHOD AppendExternalInterface(nsIInterfaceInfo *aInfo, PRUint16 *_retval); \
  NS_IMETHOD IndexOf(const nsIID & aIID, PRUint16 *_retval); \
  NS_IMETHOD IndexOfByName(const char *aName, PRUint16 *_retval); \
  NS_IMETHOD InterfaceInfoAt(PRUint16 aIndex, nsIInterfaceInfo **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIGENERICINTERFACEINFOSET(_to) \
  NS_IMETHOD AllocateParamArray(PRUint16 aCount, XPTParamDescriptor * *_retval) { return _to AllocateParamArray(aCount, _retval); } \
  NS_IMETHOD AllocateAdditionalType(PRUint16 *aIndex, XPTTypeDescriptor * *_retval) { return _to AllocateAdditionalType(aIndex, _retval); } \
  NS_IMETHOD CreateAndAppendInterface(const char *aName, const nsIID & aIID, PRUint16 aParent, PRUint8 aFlags, nsIGenericInterfaceInfo **aInfo, PRUint16 *_retval) { return _to CreateAndAppendInterface(aName, aIID, aParent, aFlags, aInfo, _retval); } \
  NS_IMETHOD AppendExternalInterface(nsIInterfaceInfo *aInfo, PRUint16 *_retval) { return _to AppendExternalInterface(aInfo, _retval); } \
  NS_IMETHOD IndexOf(const nsIID & aIID, PRUint16 *_retval) { return _to IndexOf(aIID, _retval); } \
  NS_IMETHOD IndexOfByName(const char *aName, PRUint16 *_retval) { return _to IndexOfByName(aName, _retval); } \
  NS_IMETHOD InterfaceInfoAt(PRUint16 aIndex, nsIInterfaceInfo **_retval) { return _to InterfaceInfoAt(aIndex, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIGENERICINTERFACEINFOSET(_to) \
  NS_IMETHOD AllocateParamArray(PRUint16 aCount, XPTParamDescriptor * *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->AllocateParamArray(aCount, _retval); } \
  NS_IMETHOD AllocateAdditionalType(PRUint16 *aIndex, XPTTypeDescriptor * *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->AllocateAdditionalType(aIndex, _retval); } \
  NS_IMETHOD CreateAndAppendInterface(const char *aName, const nsIID & aIID, PRUint16 aParent, PRUint8 aFlags, nsIGenericInterfaceInfo **aInfo, PRUint16 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateAndAppendInterface(aName, aIID, aParent, aFlags, aInfo, _retval); } \
  NS_IMETHOD AppendExternalInterface(nsIInterfaceInfo *aInfo, PRUint16 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->AppendExternalInterface(aInfo, _retval); } \
  NS_IMETHOD IndexOf(const nsIID & aIID, PRUint16 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->IndexOf(aIID, _retval); } \
  NS_IMETHOD IndexOfByName(const char *aName, PRUint16 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->IndexOfByName(aName, _retval); } \
  NS_IMETHOD InterfaceInfoAt(PRUint16 aIndex, nsIInterfaceInfo **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->InterfaceInfoAt(aIndex, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsGenericInterfaceInfoSet : public nsIGenericInterfaceInfoSet
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIGENERICINTERFACEINFOSET

  nsGenericInterfaceInfoSet();

private:
  ~nsGenericInterfaceInfoSet();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsGenericInterfaceInfoSet, nsIGenericInterfaceInfoSet)

nsGenericInterfaceInfoSet::nsGenericInterfaceInfoSet()
{
  /* member initializers and constructor code */
}

nsGenericInterfaceInfoSet::~nsGenericInterfaceInfoSet()
{
  /* destructor code */
}

/* XPTParamDescriptorPtr allocateParamArray (in PRUint16 aCount); */
NS_IMETHODIMP nsGenericInterfaceInfoSet::AllocateParamArray(PRUint16 aCount, XPTParamDescriptor * *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* XPTTypeDescriptorPtr allocateAdditionalType (out PRUint16 aIndex); */
NS_IMETHODIMP nsGenericInterfaceInfoSet::AllocateAdditionalType(PRUint16 *aIndex, XPTTypeDescriptor * *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint16 createAndAppendInterface (in string aName, in nsIIDRef aIID, in PRUint16 aParent, in PRUint8 aFlags, out nsIGenericInterfaceInfo aInfo); */
NS_IMETHODIMP nsGenericInterfaceInfoSet::CreateAndAppendInterface(const char *aName, const nsIID & aIID, PRUint16 aParent, PRUint8 aFlags, nsIGenericInterfaceInfo **aInfo, PRUint16 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint16 appendExternalInterface (in nsIInterfaceInfo aInfo); */
NS_IMETHODIMP nsGenericInterfaceInfoSet::AppendExternalInterface(nsIInterfaceInfo *aInfo, PRUint16 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint16 indexOf (in nsIIDRef aIID); */
NS_IMETHODIMP nsGenericInterfaceInfoSet::IndexOf(const nsIID & aIID, PRUint16 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint16 indexOfByName (in string aName); */
NS_IMETHODIMP nsGenericInterfaceInfoSet::IndexOfByName(const char *aName, PRUint16 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIInterfaceInfo interfaceInfoAt (in PRUint16 aIndex); */
NS_IMETHODIMP nsGenericInterfaceInfoSet::InterfaceInfoAt(PRUint16 aIndex, nsIInterfaceInfo **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

// The contractID for the implementation built into iiextras
#define NS_GENERIC_INTERFACE_INFO_SET_CONTRACTID "@mozilla.org/genericInterfaceInfoSet;1"

#endif /* __gen_nsIGenericInterfaceInfoSet_h__ */
