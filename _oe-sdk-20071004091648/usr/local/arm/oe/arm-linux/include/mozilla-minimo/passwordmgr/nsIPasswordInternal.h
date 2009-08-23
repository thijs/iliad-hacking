/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/passwordmgr/base/nsIPasswordInternal.idl
 */

#ifndef __gen_nsIPasswordInternal_h__
#define __gen_nsIPasswordInternal_h__


#ifndef __gen_nsIPassword_h__
#include "nsIPassword.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIPasswordInternal */
#define NS_IPASSWORDINTERNAL_IID_STR "2cc35c67-978f-42a9-a958-16e97ad2f4c8"

#define NS_IPASSWORDINTERNAL_IID \
  {0x2cc35c67, 0x978f, 0x42a9, \
    { 0xa9, 0x58, 0x16, 0xe9, 0x7a, 0xd2, 0xf4, 0xc8 }}

/**
 * This interface is supported by password manager entries to expose the
 * fieldnames passed to nsIPasswordManagerInternal's addUserFull method.
 */
class NS_NO_VTABLE nsIPasswordInternal : public nsIPassword {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IPASSWORDINTERNAL_IID)

  /**
   * The name of the field that contained the username.
   */
  /* readonly attribute AString userFieldName; */
  NS_IMETHOD GetUserFieldName(nsAString & aUserFieldName) = 0;

  /**
   * The name of the field that contained the password.
   */
  /* readonly attribute AString passwordFieldName; */
  NS_IMETHOD GetPasswordFieldName(nsAString & aPasswordFieldName) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPASSWORDINTERNAL \
  NS_IMETHOD GetUserFieldName(nsAString & aUserFieldName); \
  NS_IMETHOD GetPasswordFieldName(nsAString & aPasswordFieldName); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPASSWORDINTERNAL(_to) \
  NS_IMETHOD GetUserFieldName(nsAString & aUserFieldName) { return _to GetUserFieldName(aUserFieldName); } \
  NS_IMETHOD GetPasswordFieldName(nsAString & aPasswordFieldName) { return _to GetPasswordFieldName(aPasswordFieldName); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPASSWORDINTERNAL(_to) \
  NS_IMETHOD GetUserFieldName(nsAString & aUserFieldName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetUserFieldName(aUserFieldName); } \
  NS_IMETHOD GetPasswordFieldName(nsAString & aPasswordFieldName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPasswordFieldName(aPasswordFieldName); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsPasswordInternal : public nsIPasswordInternal
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPASSWORDINTERNAL

  nsPasswordInternal();

private:
  ~nsPasswordInternal();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsPasswordInternal, nsIPasswordInternal)

nsPasswordInternal::nsPasswordInternal()
{
  /* member initializers and constructor code */
}

nsPasswordInternal::~nsPasswordInternal()
{
  /* destructor code */
}

/* readonly attribute AString userFieldName; */
NS_IMETHODIMP nsPasswordInternal::GetUserFieldName(nsAString & aUserFieldName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString passwordFieldName; */
NS_IMETHODIMP nsPasswordInternal::GetPasswordFieldName(nsAString & aPasswordFieldName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIPasswordInternal_h__ */
