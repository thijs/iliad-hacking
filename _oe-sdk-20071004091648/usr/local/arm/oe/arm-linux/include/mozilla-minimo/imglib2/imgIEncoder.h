/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/modules/libpr0n/public/imgIEncoder.idl
 */

#ifndef __gen_imgIEncoder_h__
#define __gen_imgIEncoder_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIInputStream_h__
#include "nsIInputStream.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    imgIEncoder */
#define IMGIENCODER_IID_STR "ccc5b3ad-3e67-4e3d-97e1-b06b2e96fef8"

#define IMGIENCODER_IID \
  {0xccc5b3ad, 0x3e67, 0x4e3d, \
    { 0x97, 0xe1, 0xb0, 0x6b, 0x2e, 0x96, 0xfe, 0xf8 }}

/**
 * imgIEncoder interface
 */
class NS_NO_VTABLE imgIEncoder : public nsIInputStream {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(IMGIENCODER_IID)

  enum { INPUT_FORMAT_RGB = 0U };

  enum { INPUT_FORMAT_RGBA = 1U };

  enum { INPUT_FORMAT_HOSTARGB = 2U };

  /* void initFromData ([array, size_is (length), const] in PRUint8 data, in unsigned long length, in PRUint32 width, in PRUint32 height, in PRUint32 stride, in PRUint32 inputFormat, in AString outputOptions); */
  NS_IMETHOD InitFromData(const PRUint8 *data, PRUint32 length, PRUint32 width, PRUint32 height, PRUint32 stride, PRUint32 inputFormat, const nsAString & outputOptions) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IMGIENCODER \
  NS_IMETHOD InitFromData(const PRUint8 *data, PRUint32 length, PRUint32 width, PRUint32 height, PRUint32 stride, PRUint32 inputFormat, const nsAString & outputOptions); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IMGIENCODER(_to) \
  NS_IMETHOD InitFromData(const PRUint8 *data, PRUint32 length, PRUint32 width, PRUint32 height, PRUint32 stride, PRUint32 inputFormat, const nsAString & outputOptions) { return _to InitFromData(data, length, width, height, stride, inputFormat, outputOptions); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IMGIENCODER(_to) \
  NS_IMETHOD InitFromData(const PRUint8 *data, PRUint32 length, PRUint32 width, PRUint32 height, PRUint32 stride, PRUint32 inputFormat, const nsAString & outputOptions) { return !_to ? NS_ERROR_NULL_POINTER : _to->InitFromData(data, length, width, height, stride, inputFormat, outputOptions); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public imgIEncoder
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IMGIENCODER

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, imgIEncoder)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* void initFromData ([array, size_is (length), const] in PRUint8 data, in unsigned long length, in PRUint32 width, in PRUint32 height, in PRUint32 stride, in PRUint32 inputFormat, in AString outputOptions); */
NS_IMETHODIMP _MYCLASS_::InitFromData(const PRUint8 *data, PRUint32 length, PRUint32 width, PRUint32 height, PRUint32 stride, PRUint32 inputFormat, const nsAString & outputOptions)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_imgIEncoder_h__ */
