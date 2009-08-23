/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIScriptableInterfaceInfo.idl
 */

#ifndef __gen_nsIScriptableInterfaceInfo_h__
#define __gen_nsIScriptableInterfaceInfo_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIInterfaceInfo_h__
#include "nsIInterfaceInfo.h"
#endif

#ifndef __gen_nsIVariant_h__
#include "nsIVariant.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIScriptableDataType */
#define NS_ISCRIPTABLEDATATYPE_IID_STR "312e3b94-dc98-4ccc-b2fb-e3406f905cc6"

#define NS_ISCRIPTABLEDATATYPE_IID \
  {0x312e3b94, 0xdc98, 0x4ccc, \
    { 0xb2, 0xfb, 0xe3, 0x40, 0x6f, 0x90, 0x5c, 0xc6 }}

class NS_NO_VTABLE nsIScriptableDataType : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCRIPTABLEDATATYPE_IID)

  /* readonly attribute PRBool isPointer; */
  NS_IMETHOD GetIsPointer(PRBool *aIsPointer) = 0;

  /* readonly attribute PRBool isUniquePointer; */
  NS_IMETHOD GetIsUniquePointer(PRBool *aIsUniquePointer) = 0;

  /* readonly attribute PRBool isReference; */
  NS_IMETHOD GetIsReference(PRBool *aIsReference) = 0;

  /* readonly attribute PRBool isArithmetic; */
  NS_IMETHOD GetIsArithmetic(PRBool *aIsArithmetic) = 0;

  /* readonly attribute PRBool isInterfacePointer; */
  NS_IMETHOD GetIsInterfacePointer(PRBool *aIsInterfacePointer) = 0;

  /* readonly attribute PRBool isArray; */
  NS_IMETHOD GetIsArray(PRBool *aIsArray) = 0;

  /* readonly attribute PRBool isDependent; */
  NS_IMETHOD GetIsDependent(PRBool *aIsDependent) = 0;

  /**
    * This is one of the constants declared in nsIDataType
    */
  /* readonly attribute PRUint16 dataType; */
  NS_IMETHOD GetDataType(PRUint16 *aDataType) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCRIPTABLEDATATYPE \
  NS_IMETHOD GetIsPointer(PRBool *aIsPointer); \
  NS_IMETHOD GetIsUniquePointer(PRBool *aIsUniquePointer); \
  NS_IMETHOD GetIsReference(PRBool *aIsReference); \
  NS_IMETHOD GetIsArithmetic(PRBool *aIsArithmetic); \
  NS_IMETHOD GetIsInterfacePointer(PRBool *aIsInterfacePointer); \
  NS_IMETHOD GetIsArray(PRBool *aIsArray); \
  NS_IMETHOD GetIsDependent(PRBool *aIsDependent); \
  NS_IMETHOD GetDataType(PRUint16 *aDataType); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCRIPTABLEDATATYPE(_to) \
  NS_IMETHOD GetIsPointer(PRBool *aIsPointer) { return _to GetIsPointer(aIsPointer); } \
  NS_IMETHOD GetIsUniquePointer(PRBool *aIsUniquePointer) { return _to GetIsUniquePointer(aIsUniquePointer); } \
  NS_IMETHOD GetIsReference(PRBool *aIsReference) { return _to GetIsReference(aIsReference); } \
  NS_IMETHOD GetIsArithmetic(PRBool *aIsArithmetic) { return _to GetIsArithmetic(aIsArithmetic); } \
  NS_IMETHOD GetIsInterfacePointer(PRBool *aIsInterfacePointer) { return _to GetIsInterfacePointer(aIsInterfacePointer); } \
  NS_IMETHOD GetIsArray(PRBool *aIsArray) { return _to GetIsArray(aIsArray); } \
  NS_IMETHOD GetIsDependent(PRBool *aIsDependent) { return _to GetIsDependent(aIsDependent); } \
  NS_IMETHOD GetDataType(PRUint16 *aDataType) { return _to GetDataType(aDataType); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCRIPTABLEDATATYPE(_to) \
  NS_IMETHOD GetIsPointer(PRBool *aIsPointer) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsPointer(aIsPointer); } \
  NS_IMETHOD GetIsUniquePointer(PRBool *aIsUniquePointer) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsUniquePointer(aIsUniquePointer); } \
  NS_IMETHOD GetIsReference(PRBool *aIsReference) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsReference(aIsReference); } \
  NS_IMETHOD GetIsArithmetic(PRBool *aIsArithmetic) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsArithmetic(aIsArithmetic); } \
  NS_IMETHOD GetIsInterfacePointer(PRBool *aIsInterfacePointer) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsInterfacePointer(aIsInterfacePointer); } \
  NS_IMETHOD GetIsArray(PRBool *aIsArray) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsArray(aIsArray); } \
  NS_IMETHOD GetIsDependent(PRBool *aIsDependent) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsDependent(aIsDependent); } \
  NS_IMETHOD GetDataType(PRUint16 *aDataType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDataType(aDataType); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsScriptableDataType : public nsIScriptableDataType
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCRIPTABLEDATATYPE

  nsScriptableDataType();

private:
  ~nsScriptableDataType();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsScriptableDataType, nsIScriptableDataType)

nsScriptableDataType::nsScriptableDataType()
{
  /* member initializers and constructor code */
}

nsScriptableDataType::~nsScriptableDataType()
{
  /* destructor code */
}

/* readonly attribute PRBool isPointer; */
NS_IMETHODIMP nsScriptableDataType::GetIsPointer(PRBool *aIsPointer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isUniquePointer; */
NS_IMETHODIMP nsScriptableDataType::GetIsUniquePointer(PRBool *aIsUniquePointer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isReference; */
NS_IMETHODIMP nsScriptableDataType::GetIsReference(PRBool *aIsReference)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isArithmetic; */
NS_IMETHODIMP nsScriptableDataType::GetIsArithmetic(PRBool *aIsArithmetic)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isInterfacePointer; */
NS_IMETHODIMP nsScriptableDataType::GetIsInterfacePointer(PRBool *aIsInterfacePointer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isArray; */
NS_IMETHODIMP nsScriptableDataType::GetIsArray(PRBool *aIsArray)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isDependent; */
NS_IMETHODIMP nsScriptableDataType::GetIsDependent(PRBool *aIsDependent)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint16 dataType; */
NS_IMETHODIMP nsScriptableDataType::GetDataType(PRUint16 *aDataType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIScriptableParamInfo */
#define NS_ISCRIPTABLEPARAMINFO_IID_STR "2309482b-4631-455f-833f-5e4e9ce38589"

#define NS_ISCRIPTABLEPARAMINFO_IID \
  {0x2309482b, 0x4631, 0x455f, \
    { 0x83, 0x3f, 0x5e, 0x4e, 0x9c, 0xe3, 0x85, 0x89 }}

class NS_NO_VTABLE nsIScriptableParamInfo : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCRIPTABLEPARAMINFO_IID)

  /* readonly attribute PRBool isIn; */
  NS_IMETHOD GetIsIn(PRBool *aIsIn) = 0;

  /* readonly attribute PRBool isOut; */
  NS_IMETHOD GetIsOut(PRBool *aIsOut) = 0;

  /* readonly attribute PRBool isRetval; */
  NS_IMETHOD GetIsRetval(PRBool *aIsRetval) = 0;

  /* readonly attribute PRBool isShared; */
  NS_IMETHOD GetIsShared(PRBool *aIsShared) = 0;

  /* readonly attribute PRBool isDipper; */
  NS_IMETHOD GetIsDipper(PRBool *aIsDipper) = 0;

  /* readonly attribute nsIScriptableDataType type; */
  NS_IMETHOD GetType(nsIScriptableDataType * *aType) = 0;

  /* [noscript] void getParamInfo ([shared, const, retval] out nsXPTParamInfoPtr aInfo); */
  NS_IMETHOD GetParamInfo(const nsXPTParamInfo * *aInfo) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCRIPTABLEPARAMINFO \
  NS_IMETHOD GetIsIn(PRBool *aIsIn); \
  NS_IMETHOD GetIsOut(PRBool *aIsOut); \
  NS_IMETHOD GetIsRetval(PRBool *aIsRetval); \
  NS_IMETHOD GetIsShared(PRBool *aIsShared); \
  NS_IMETHOD GetIsDipper(PRBool *aIsDipper); \
  NS_IMETHOD GetType(nsIScriptableDataType * *aType); \
  NS_IMETHOD GetParamInfo(const nsXPTParamInfo * *aInfo); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCRIPTABLEPARAMINFO(_to) \
  NS_IMETHOD GetIsIn(PRBool *aIsIn) { return _to GetIsIn(aIsIn); } \
  NS_IMETHOD GetIsOut(PRBool *aIsOut) { return _to GetIsOut(aIsOut); } \
  NS_IMETHOD GetIsRetval(PRBool *aIsRetval) { return _to GetIsRetval(aIsRetval); } \
  NS_IMETHOD GetIsShared(PRBool *aIsShared) { return _to GetIsShared(aIsShared); } \
  NS_IMETHOD GetIsDipper(PRBool *aIsDipper) { return _to GetIsDipper(aIsDipper); } \
  NS_IMETHOD GetType(nsIScriptableDataType * *aType) { return _to GetType(aType); } \
  NS_IMETHOD GetParamInfo(const nsXPTParamInfo * *aInfo) { return _to GetParamInfo(aInfo); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCRIPTABLEPARAMINFO(_to) \
  NS_IMETHOD GetIsIn(PRBool *aIsIn) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsIn(aIsIn); } \
  NS_IMETHOD GetIsOut(PRBool *aIsOut) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsOut(aIsOut); } \
  NS_IMETHOD GetIsRetval(PRBool *aIsRetval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsRetval(aIsRetval); } \
  NS_IMETHOD GetIsShared(PRBool *aIsShared) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsShared(aIsShared); } \
  NS_IMETHOD GetIsDipper(PRBool *aIsDipper) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsDipper(aIsDipper); } \
  NS_IMETHOD GetType(nsIScriptableDataType * *aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_IMETHOD GetParamInfo(const nsXPTParamInfo * *aInfo) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParamInfo(aInfo); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsScriptableParamInfo : public nsIScriptableParamInfo
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCRIPTABLEPARAMINFO

  nsScriptableParamInfo();

private:
  ~nsScriptableParamInfo();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsScriptableParamInfo, nsIScriptableParamInfo)

nsScriptableParamInfo::nsScriptableParamInfo()
{
  /* member initializers and constructor code */
}

nsScriptableParamInfo::~nsScriptableParamInfo()
{
  /* destructor code */
}

/* readonly attribute PRBool isIn; */
NS_IMETHODIMP nsScriptableParamInfo::GetIsIn(PRBool *aIsIn)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isOut; */
NS_IMETHODIMP nsScriptableParamInfo::GetIsOut(PRBool *aIsOut)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isRetval; */
NS_IMETHODIMP nsScriptableParamInfo::GetIsRetval(PRBool *aIsRetval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isShared; */
NS_IMETHODIMP nsScriptableParamInfo::GetIsShared(PRBool *aIsShared)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isDipper; */
NS_IMETHODIMP nsScriptableParamInfo::GetIsDipper(PRBool *aIsDipper)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIScriptableDataType type; */
NS_IMETHODIMP nsScriptableParamInfo::GetType(nsIScriptableDataType * *aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void getParamInfo ([shared, const, retval] out nsXPTParamInfoPtr aInfo); */
NS_IMETHODIMP nsScriptableParamInfo::GetParamInfo(const nsXPTParamInfo * *aInfo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIScriptableConstant */
#define NS_ISCRIPTABLECONSTANT_IID_STR "0f6c5b09-88b0-43ca-b55c-578f24f3d810"

#define NS_ISCRIPTABLECONSTANT_IID \
  {0x0f6c5b09, 0x88b0, 0x43ca, \
    { 0xb5, 0x5c, 0x57, 0x8f, 0x24, 0xf3, 0xd8, 0x10 }}

class NS_NO_VTABLE nsIScriptableConstant : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCRIPTABLECONSTANT_IID)

  /* readonly attribute string name; */
  NS_IMETHOD GetName(char * *aName) = 0;

  /* readonly attribute nsIScriptableDataType type; */
  NS_IMETHOD GetType(nsIScriptableDataType * *aType) = 0;

  /* readonly attribute nsIVariant value; */
  NS_IMETHOD GetValue(nsIVariant * *aValue) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCRIPTABLECONSTANT \
  NS_IMETHOD GetName(char * *aName); \
  NS_IMETHOD GetType(nsIScriptableDataType * *aType); \
  NS_IMETHOD GetValue(nsIVariant * *aValue); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCRIPTABLECONSTANT(_to) \
  NS_IMETHOD GetName(char * *aName) { return _to GetName(aName); } \
  NS_IMETHOD GetType(nsIScriptableDataType * *aType) { return _to GetType(aType); } \
  NS_IMETHOD GetValue(nsIVariant * *aValue) { return _to GetValue(aValue); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCRIPTABLECONSTANT(_to) \
  NS_IMETHOD GetName(char * *aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetType(nsIScriptableDataType * *aType) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetType(aType); } \
  NS_IMETHOD GetValue(nsIVariant * *aValue) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetValue(aValue); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsScriptableConstant : public nsIScriptableConstant
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCRIPTABLECONSTANT

  nsScriptableConstant();

private:
  ~nsScriptableConstant();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsScriptableConstant, nsIScriptableConstant)

nsScriptableConstant::nsScriptableConstant()
{
  /* member initializers and constructor code */
}

nsScriptableConstant::~nsScriptableConstant()
{
  /* destructor code */
}

/* readonly attribute string name; */
NS_IMETHODIMP nsScriptableConstant::GetName(char * *aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIScriptableDataType type; */
NS_IMETHODIMP nsScriptableConstant::GetType(nsIScriptableDataType * *aType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIVariant value; */
NS_IMETHODIMP nsScriptableConstant::GetValue(nsIVariant * *aValue)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIScriptableMethodInfo */
#define NS_ISCRIPTABLEMETHODINFO_IID_STR "9228afa2-187c-4feb-9228-5108e640ca33"

#define NS_ISCRIPTABLEMETHODINFO_IID \
  {0x9228afa2, 0x187c, 0x4feb, \
    { 0x92, 0x28, 0x51, 0x08, 0xe6, 0x40, 0xca, 0x33 }}

class NS_NO_VTABLE nsIScriptableMethodInfo : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCRIPTABLEMETHODINFO_IID)

  /* readonly attribute PRBool isGetter; */
  NS_IMETHOD GetIsGetter(PRBool *aIsGetter) = 0;

  /* readonly attribute PRBool isSetter; */
  NS_IMETHOD GetIsSetter(PRBool *aIsSetter) = 0;

  /* readonly attribute PRBool isNotXPCOM; */
  NS_IMETHOD GetIsNotXPCOM(PRBool *aIsNotXPCOM) = 0;

  /* readonly attribute PRBool isConstructor; */
  NS_IMETHOD GetIsConstructor(PRBool *aIsConstructor) = 0;

  /* readonly attribute PRBool isHidden; */
  NS_IMETHOD GetIsHidden(PRBool *aIsHidden) = 0;

  /* readonly attribute string name; */
  NS_IMETHOD GetName(char * *aName) = 0;

  /* readonly attribute PRUint8 paramCount; */
  NS_IMETHOD GetParamCount(PRUint8 *aParamCount) = 0;

  /* nsIScriptableParamInfo getParam (in PRUint8 idx); */
  NS_IMETHOD GetParam(PRUint8 idx, nsIScriptableParamInfo **_retval) = 0;

  /* readonly attribute nsIScriptableParamInfo result; */
  NS_IMETHOD GetResult(nsIScriptableParamInfo * *aResult) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCRIPTABLEMETHODINFO \
  NS_IMETHOD GetIsGetter(PRBool *aIsGetter); \
  NS_IMETHOD GetIsSetter(PRBool *aIsSetter); \
  NS_IMETHOD GetIsNotXPCOM(PRBool *aIsNotXPCOM); \
  NS_IMETHOD GetIsConstructor(PRBool *aIsConstructor); \
  NS_IMETHOD GetIsHidden(PRBool *aIsHidden); \
  NS_IMETHOD GetName(char * *aName); \
  NS_IMETHOD GetParamCount(PRUint8 *aParamCount); \
  NS_IMETHOD GetParam(PRUint8 idx, nsIScriptableParamInfo **_retval); \
  NS_IMETHOD GetResult(nsIScriptableParamInfo * *aResult); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCRIPTABLEMETHODINFO(_to) \
  NS_IMETHOD GetIsGetter(PRBool *aIsGetter) { return _to GetIsGetter(aIsGetter); } \
  NS_IMETHOD GetIsSetter(PRBool *aIsSetter) { return _to GetIsSetter(aIsSetter); } \
  NS_IMETHOD GetIsNotXPCOM(PRBool *aIsNotXPCOM) { return _to GetIsNotXPCOM(aIsNotXPCOM); } \
  NS_IMETHOD GetIsConstructor(PRBool *aIsConstructor) { return _to GetIsConstructor(aIsConstructor); } \
  NS_IMETHOD GetIsHidden(PRBool *aIsHidden) { return _to GetIsHidden(aIsHidden); } \
  NS_IMETHOD GetName(char * *aName) { return _to GetName(aName); } \
  NS_IMETHOD GetParamCount(PRUint8 *aParamCount) { return _to GetParamCount(aParamCount); } \
  NS_IMETHOD GetParam(PRUint8 idx, nsIScriptableParamInfo **_retval) { return _to GetParam(idx, _retval); } \
  NS_IMETHOD GetResult(nsIScriptableParamInfo * *aResult) { return _to GetResult(aResult); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCRIPTABLEMETHODINFO(_to) \
  NS_IMETHOD GetIsGetter(PRBool *aIsGetter) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsGetter(aIsGetter); } \
  NS_IMETHOD GetIsSetter(PRBool *aIsSetter) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsSetter(aIsSetter); } \
  NS_IMETHOD GetIsNotXPCOM(PRBool *aIsNotXPCOM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsNotXPCOM(aIsNotXPCOM); } \
  NS_IMETHOD GetIsConstructor(PRBool *aIsConstructor) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsConstructor(aIsConstructor); } \
  NS_IMETHOD GetIsHidden(PRBool *aIsHidden) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsHidden(aIsHidden); } \
  NS_IMETHOD GetName(char * *aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetParamCount(PRUint8 *aParamCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParamCount(aParamCount); } \
  NS_IMETHOD GetParam(PRUint8 idx, nsIScriptableParamInfo **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParam(idx, _retval); } \
  NS_IMETHOD GetResult(nsIScriptableParamInfo * *aResult) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetResult(aResult); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsScriptableMethodInfo : public nsIScriptableMethodInfo
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCRIPTABLEMETHODINFO

  nsScriptableMethodInfo();

private:
  ~nsScriptableMethodInfo();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsScriptableMethodInfo, nsIScriptableMethodInfo)

nsScriptableMethodInfo::nsScriptableMethodInfo()
{
  /* member initializers and constructor code */
}

nsScriptableMethodInfo::~nsScriptableMethodInfo()
{
  /* destructor code */
}

/* readonly attribute PRBool isGetter; */
NS_IMETHODIMP nsScriptableMethodInfo::GetIsGetter(PRBool *aIsGetter)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isSetter; */
NS_IMETHODIMP nsScriptableMethodInfo::GetIsSetter(PRBool *aIsSetter)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isNotXPCOM; */
NS_IMETHODIMP nsScriptableMethodInfo::GetIsNotXPCOM(PRBool *aIsNotXPCOM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isConstructor; */
NS_IMETHODIMP nsScriptableMethodInfo::GetIsConstructor(PRBool *aIsConstructor)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isHidden; */
NS_IMETHODIMP nsScriptableMethodInfo::GetIsHidden(PRBool *aIsHidden)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string name; */
NS_IMETHODIMP nsScriptableMethodInfo::GetName(char * *aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint8 paramCount; */
NS_IMETHODIMP nsScriptableMethodInfo::GetParamCount(PRUint8 *aParamCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIScriptableParamInfo getParam (in PRUint8 idx); */
NS_IMETHODIMP nsScriptableMethodInfo::GetParam(PRUint8 idx, nsIScriptableParamInfo **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIScriptableParamInfo result; */
NS_IMETHODIMP nsScriptableMethodInfo::GetResult(nsIScriptableParamInfo * *aResult)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIScriptableInterfaceInfo */
#define NS_ISCRIPTABLEINTERFACEINFO_IID_STR "f902d5ba-2ef6-444e-8a17-52cb70715c10"

#define NS_ISCRIPTABLEINTERFACEINFO_IID \
  {0xf902d5ba, 0x2ef6, 0x444e, \
    { 0x8a, 0x17, 0x52, 0xcb, 0x70, 0x71, 0x5c, 0x10 }}

class NS_NO_VTABLE nsIScriptableInterfaceInfo : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISCRIPTABLEINTERFACEINFO_IID)

  /* [noscript] attribute nsIInterfaceInfo info; */
  NS_IMETHOD GetInfo(nsIInterfaceInfo * *aInfo) = 0;
  NS_IMETHOD SetInfo(nsIInterfaceInfo * aInfo) = 0;

  /* void init (in nsIIDPtr aIID); */
  NS_IMETHOD Init(const nsIID * aIID) = 0;

  /* void initWithName (in string name); */
  NS_IMETHOD InitWithName(const char *name) = 0;

  /* readonly attribute string name; */
  NS_IMETHOD GetName(char * *aName) = 0;

  /* readonly attribute nsIIDPtr interfaceID; */
  NS_IMETHOD GetInterfaceID(nsIID * *aInterfaceID) = 0;

  /** 
     * True if this object has been sucessfully set to wrap an interface info.
     */
  /* readonly attribute PRBool isValid; */
  NS_IMETHOD GetIsValid(PRBool *aIsValid) = 0;

  /* readonly attribute PRBool isScriptable; */
  NS_IMETHOD GetIsScriptable(PRBool *aIsScriptable) = 0;

  /* readonly attribute nsIScriptableInterfaceInfo parent; */
  NS_IMETHOD GetParent(nsIScriptableInterfaceInfo * *aParent) = 0;

  /**
    * These include counts for parent (and all ancestors).
    */
  /* readonly attribute PRUint16 methodCount; */
  NS_IMETHOD GetMethodCount(PRUint16 *aMethodCount) = 0;

  /* readonly attribute PRUint16 constantCount; */
  NS_IMETHOD GetConstantCount(PRUint16 *aConstantCount) = 0;

  /**
    * These include methods and constants for parent (and all ancestors).
    */
  /* nsIScriptableMethodInfo getMethodInfo (in PRUint16 index); */
  NS_IMETHOD GetMethodInfo(PRUint16 index, nsIScriptableMethodInfo **_retval) = 0;

  /* nsIScriptableMethodInfo getMethodInfoForName (in string methodName, out PRUint16 index); */
  NS_IMETHOD GetMethodInfoForName(const char *methodName, PRUint16 *index, nsIScriptableMethodInfo **_retval) = 0;

  /* nsIScriptableConstant getConstant (in PRUint16 index); */
  NS_IMETHOD GetConstant(PRUint16 index, nsIScriptableConstant **_retval) = 0;

  /**
    * Get the interface information or iid associated with a param of some
    * method in this interface.
    */
  /* nsIScriptableInterfaceInfo getInfoForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param); */
  NS_IMETHOD GetInfoForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIScriptableInterfaceInfo **_retval) = 0;

  /* nsIIDPtr getIIDForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param); */
  NS_IMETHOD GetIIDForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIID * *_retval) = 0;

  /* nsIScriptableDataType getTypeForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param, in PRUint16 dimension); */
  NS_IMETHOD GetTypeForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, nsIScriptableDataType **_retval) = 0;

  /* PRUint8 getSizeIsArgNumberForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param, in PRUint16 dimension); */
  NS_IMETHOD GetSizeIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval) = 0;

  /* PRUint8 getLengthIsArgNumberForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param, in PRUint16 dimension); */
  NS_IMETHOD GetLengthIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval) = 0;

  /* PRUint8 getInterfaceIsArgNumberForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param); */
  NS_IMETHOD GetInterfaceIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint8 *_retval) = 0;

  /* PRBool isIID (in nsIIDPtr IID); */
  NS_IMETHOD IsIID(const nsIID * IID, PRBool *_retval) = 0;

  /* readonly attribute PRBool isFunction; */
  NS_IMETHOD GetIsFunction(PRBool *aIsFunction) = 0;

  /* PRBool hasAncestor (in nsIIDPtr iid); */
  NS_IMETHOD HasAncestor(const nsIID * iid, PRBool *_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISCRIPTABLEINTERFACEINFO \
  NS_IMETHOD GetInfo(nsIInterfaceInfo * *aInfo); \
  NS_IMETHOD SetInfo(nsIInterfaceInfo * aInfo); \
  NS_IMETHOD Init(const nsIID * aIID); \
  NS_IMETHOD InitWithName(const char *name); \
  NS_IMETHOD GetName(char * *aName); \
  NS_IMETHOD GetInterfaceID(nsIID * *aInterfaceID); \
  NS_IMETHOD GetIsValid(PRBool *aIsValid); \
  NS_IMETHOD GetIsScriptable(PRBool *aIsScriptable); \
  NS_IMETHOD GetParent(nsIScriptableInterfaceInfo * *aParent); \
  NS_IMETHOD GetMethodCount(PRUint16 *aMethodCount); \
  NS_IMETHOD GetConstantCount(PRUint16 *aConstantCount); \
  NS_IMETHOD GetMethodInfo(PRUint16 index, nsIScriptableMethodInfo **_retval); \
  NS_IMETHOD GetMethodInfoForName(const char *methodName, PRUint16 *index, nsIScriptableMethodInfo **_retval); \
  NS_IMETHOD GetConstant(PRUint16 index, nsIScriptableConstant **_retval); \
  NS_IMETHOD GetInfoForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIScriptableInterfaceInfo **_retval); \
  NS_IMETHOD GetIIDForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIID * *_retval); \
  NS_IMETHOD GetTypeForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, nsIScriptableDataType **_retval); \
  NS_IMETHOD GetSizeIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval); \
  NS_IMETHOD GetLengthIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval); \
  NS_IMETHOD GetInterfaceIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint8 *_retval); \
  NS_IMETHOD IsIID(const nsIID * IID, PRBool *_retval); \
  NS_IMETHOD GetIsFunction(PRBool *aIsFunction); \
  NS_IMETHOD HasAncestor(const nsIID * iid, PRBool *_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISCRIPTABLEINTERFACEINFO(_to) \
  NS_IMETHOD GetInfo(nsIInterfaceInfo * *aInfo) { return _to GetInfo(aInfo); } \
  NS_IMETHOD SetInfo(nsIInterfaceInfo * aInfo) { return _to SetInfo(aInfo); } \
  NS_IMETHOD Init(const nsIID * aIID) { return _to Init(aIID); } \
  NS_IMETHOD InitWithName(const char *name) { return _to InitWithName(name); } \
  NS_IMETHOD GetName(char * *aName) { return _to GetName(aName); } \
  NS_IMETHOD GetInterfaceID(nsIID * *aInterfaceID) { return _to GetInterfaceID(aInterfaceID); } \
  NS_IMETHOD GetIsValid(PRBool *aIsValid) { return _to GetIsValid(aIsValid); } \
  NS_IMETHOD GetIsScriptable(PRBool *aIsScriptable) { return _to GetIsScriptable(aIsScriptable); } \
  NS_IMETHOD GetParent(nsIScriptableInterfaceInfo * *aParent) { return _to GetParent(aParent); } \
  NS_IMETHOD GetMethodCount(PRUint16 *aMethodCount) { return _to GetMethodCount(aMethodCount); } \
  NS_IMETHOD GetConstantCount(PRUint16 *aConstantCount) { return _to GetConstantCount(aConstantCount); } \
  NS_IMETHOD GetMethodInfo(PRUint16 index, nsIScriptableMethodInfo **_retval) { return _to GetMethodInfo(index, _retval); } \
  NS_IMETHOD GetMethodInfoForName(const char *methodName, PRUint16 *index, nsIScriptableMethodInfo **_retval) { return _to GetMethodInfoForName(methodName, index, _retval); } \
  NS_IMETHOD GetConstant(PRUint16 index, nsIScriptableConstant **_retval) { return _to GetConstant(index, _retval); } \
  NS_IMETHOD GetInfoForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIScriptableInterfaceInfo **_retval) { return _to GetInfoForParam(methodIndex, param, _retval); } \
  NS_IMETHOD GetIIDForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIID * *_retval) { return _to GetIIDForParam(methodIndex, param, _retval); } \
  NS_IMETHOD GetTypeForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, nsIScriptableDataType **_retval) { return _to GetTypeForParam(methodIndex, param, dimension, _retval); } \
  NS_IMETHOD GetSizeIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval) { return _to GetSizeIsArgNumberForParam(methodIndex, param, dimension, _retval); } \
  NS_IMETHOD GetLengthIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval) { return _to GetLengthIsArgNumberForParam(methodIndex, param, dimension, _retval); } \
  NS_IMETHOD GetInterfaceIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint8 *_retval) { return _to GetInterfaceIsArgNumberForParam(methodIndex, param, _retval); } \
  NS_IMETHOD IsIID(const nsIID * IID, PRBool *_retval) { return _to IsIID(IID, _retval); } \
  NS_IMETHOD GetIsFunction(PRBool *aIsFunction) { return _to GetIsFunction(aIsFunction); } \
  NS_IMETHOD HasAncestor(const nsIID * iid, PRBool *_retval) { return _to HasAncestor(iid, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISCRIPTABLEINTERFACEINFO(_to) \
  NS_IMETHOD GetInfo(nsIInterfaceInfo * *aInfo) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInfo(aInfo); } \
  NS_IMETHOD SetInfo(nsIInterfaceInfo * aInfo) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetInfo(aInfo); } \
  NS_IMETHOD Init(const nsIID * aIID) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aIID); } \
  NS_IMETHOD InitWithName(const char *name) { return !_to ? NS_ERROR_NULL_POINTER : _to->InitWithName(name); } \
  NS_IMETHOD GetName(char * *aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetInterfaceID(nsIID * *aInterfaceID) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInterfaceID(aInterfaceID); } \
  NS_IMETHOD GetIsValid(PRBool *aIsValid) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsValid(aIsValid); } \
  NS_IMETHOD GetIsScriptable(PRBool *aIsScriptable) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsScriptable(aIsScriptable); } \
  NS_IMETHOD GetParent(nsIScriptableInterfaceInfo * *aParent) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetParent(aParent); } \
  NS_IMETHOD GetMethodCount(PRUint16 *aMethodCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMethodCount(aMethodCount); } \
  NS_IMETHOD GetConstantCount(PRUint16 *aConstantCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetConstantCount(aConstantCount); } \
  NS_IMETHOD GetMethodInfo(PRUint16 index, nsIScriptableMethodInfo **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMethodInfo(index, _retval); } \
  NS_IMETHOD GetMethodInfoForName(const char *methodName, PRUint16 *index, nsIScriptableMethodInfo **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMethodInfoForName(methodName, index, _retval); } \
  NS_IMETHOD GetConstant(PRUint16 index, nsIScriptableConstant **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetConstant(index, _retval); } \
  NS_IMETHOD GetInfoForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIScriptableInterfaceInfo **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInfoForParam(methodIndex, param, _retval); } \
  NS_IMETHOD GetIIDForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIID * *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIIDForParam(methodIndex, param, _retval); } \
  NS_IMETHOD GetTypeForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, nsIScriptableDataType **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetTypeForParam(methodIndex, param, dimension, _retval); } \
  NS_IMETHOD GetSizeIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSizeIsArgNumberForParam(methodIndex, param, dimension, _retval); } \
  NS_IMETHOD GetLengthIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetLengthIsArgNumberForParam(methodIndex, param, dimension, _retval); } \
  NS_IMETHOD GetInterfaceIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint8 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInterfaceIsArgNumberForParam(methodIndex, param, _retval); } \
  NS_IMETHOD IsIID(const nsIID * IID, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsIID(IID, _retval); } \
  NS_IMETHOD GetIsFunction(PRBool *aIsFunction) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsFunction(aIsFunction); } \
  NS_IMETHOD HasAncestor(const nsIID * iid, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->HasAncestor(iid, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsScriptableInterfaceInfo : public nsIScriptableInterfaceInfo
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISCRIPTABLEINTERFACEINFO

  nsScriptableInterfaceInfo();

private:
  ~nsScriptableInterfaceInfo();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsScriptableInterfaceInfo, nsIScriptableInterfaceInfo)

nsScriptableInterfaceInfo::nsScriptableInterfaceInfo()
{
  /* member initializers and constructor code */
}

nsScriptableInterfaceInfo::~nsScriptableInterfaceInfo()
{
  /* destructor code */
}

/* [noscript] attribute nsIInterfaceInfo info; */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetInfo(nsIInterfaceInfo * *aInfo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsScriptableInterfaceInfo::SetInfo(nsIInterfaceInfo * aInfo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void init (in nsIIDPtr aIID); */
NS_IMETHODIMP nsScriptableInterfaceInfo::Init(const nsIID * aIID)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void initWithName (in string name); */
NS_IMETHODIMP nsScriptableInterfaceInfo::InitWithName(const char *name)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string name; */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetName(char * *aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIIDPtr interfaceID; */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetInterfaceID(nsIID * *aInterfaceID)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isValid; */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetIsValid(PRBool *aIsValid)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isScriptable; */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetIsScriptable(PRBool *aIsScriptable)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIScriptableInterfaceInfo parent; */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetParent(nsIScriptableInterfaceInfo * *aParent)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint16 methodCount; */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetMethodCount(PRUint16 *aMethodCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint16 constantCount; */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetConstantCount(PRUint16 *aConstantCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIScriptableMethodInfo getMethodInfo (in PRUint16 index); */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetMethodInfo(PRUint16 index, nsIScriptableMethodInfo **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIScriptableMethodInfo getMethodInfoForName (in string methodName, out PRUint16 index); */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetMethodInfoForName(const char *methodName, PRUint16 *index, nsIScriptableMethodInfo **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIScriptableConstant getConstant (in PRUint16 index); */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetConstant(PRUint16 index, nsIScriptableConstant **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIScriptableInterfaceInfo getInfoForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param); */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetInfoForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIScriptableInterfaceInfo **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIIDPtr getIIDForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param); */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetIIDForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, nsIID * *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIScriptableDataType getTypeForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param, in PRUint16 dimension); */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetTypeForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, nsIScriptableDataType **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint8 getSizeIsArgNumberForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param, in PRUint16 dimension); */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetSizeIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint8 getLengthIsArgNumberForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param, in PRUint16 dimension); */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetLengthIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint16 dimension, PRUint8 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRUint8 getInterfaceIsArgNumberForParam (in PRUint16 methodIndex, in nsIScriptableParamInfo param); */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetInterfaceIsArgNumberForParam(PRUint16 methodIndex, nsIScriptableParamInfo *param, PRUint8 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRBool isIID (in nsIIDPtr IID); */
NS_IMETHODIMP nsScriptableInterfaceInfo::IsIID(const nsIID * IID, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRBool isFunction; */
NS_IMETHODIMP nsScriptableInterfaceInfo::GetIsFunction(PRBool *aIsFunction)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* PRBool hasAncestor (in nsIIDPtr iid); */
NS_IMETHODIMP nsScriptableInterfaceInfo::HasAncestor(const nsIID * iid, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

// The contractID for the implementation built in to xpconnect.
#define NS_SCRIPTABLE_INTERFACE_INFO_CONTRACTID "@mozilla.org/scriptableInterfaceInfo;1"

#endif /* __gen_nsIScriptableInterfaceInfo_h__ */
