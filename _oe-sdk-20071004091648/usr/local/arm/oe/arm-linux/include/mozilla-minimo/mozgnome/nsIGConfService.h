/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/toolkit/components/gnome/nsIGConfService.idl
 */

#ifndef __gen_nsIGConfService_h__
#define __gen_nsIGConfService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIGConfService */
#define NS_IGCONFSERVICE_IID_STR "01ac7b2e-c07c-465f-b35c-542eaef420a9"

#define NS_IGCONFSERVICE_IID \
  {0x01ac7b2e, 0xc07c, 0x465f, \
    { 0xb3, 0x5c, 0x54, 0x2e, 0xae, 0xf4, 0x20, 0xa9 }}

class NS_NO_VTABLE nsIGConfService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IGCONFSERVICE_IID)

  /* boolean getBool (in AUTF8String key); */
  NS_IMETHOD GetBool(const nsACString & key, PRBool *_retval) = 0;

  /* AUTF8String getString (in AUTF8String key); */
  NS_IMETHOD GetString(const nsACString & key, nsACString & _retval) = 0;

  /* long getInt (in AUTF8String key); */
  NS_IMETHOD GetInt(const nsACString & key, PRInt32 *_retval) = 0;

  /* float getFloat (in AUTF8String key); */
  NS_IMETHOD GetFloat(const nsACString & key, float *_retval) = 0;

  /* void setBool (in AUTF8String key, in boolean value); */
  NS_IMETHOD SetBool(const nsACString & key, PRBool value) = 0;

  /* void setString (in AUTF8String key, in AUTF8String value); */
  NS_IMETHOD SetString(const nsACString & key, const nsACString & value) = 0;

  /* void setInt (in AUTF8String key, in long value); */
  NS_IMETHOD SetInt(const nsACString & key, PRInt32 value) = 0;

  /* void setFloat (in AUTF8String key, in float value); */
  NS_IMETHOD SetFloat(const nsACString & key, float value) = 0;

  /* AUTF8String getAppForProtocol (in AUTF8String scheme, out boolean enabled); */
  NS_IMETHOD GetAppForProtocol(const nsACString & scheme, PRBool *enabled, nsACString & _retval) = 0;

  /* boolean handlerRequiresTerminal (in AUTF8String scheme); */
  NS_IMETHOD HandlerRequiresTerminal(const nsACString & scheme, PRBool *_retval) = 0;

  /* void setAppForProtocol (in AUTF8String scheme, in AUTF8String command); */
  NS_IMETHOD SetAppForProtocol(const nsACString & scheme, const nsACString & command) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIGCONFSERVICE \
  NS_IMETHOD GetBool(const nsACString & key, PRBool *_retval); \
  NS_IMETHOD GetString(const nsACString & key, nsACString & _retval); \
  NS_IMETHOD GetInt(const nsACString & key, PRInt32 *_retval); \
  NS_IMETHOD GetFloat(const nsACString & key, float *_retval); \
  NS_IMETHOD SetBool(const nsACString & key, PRBool value); \
  NS_IMETHOD SetString(const nsACString & key, const nsACString & value); \
  NS_IMETHOD SetInt(const nsACString & key, PRInt32 value); \
  NS_IMETHOD SetFloat(const nsACString & key, float value); \
  NS_IMETHOD GetAppForProtocol(const nsACString & scheme, PRBool *enabled, nsACString & _retval); \
  NS_IMETHOD HandlerRequiresTerminal(const nsACString & scheme, PRBool *_retval); \
  NS_IMETHOD SetAppForProtocol(const nsACString & scheme, const nsACString & command); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIGCONFSERVICE(_to) \
  NS_IMETHOD GetBool(const nsACString & key, PRBool *_retval) { return _to GetBool(key, _retval); } \
  NS_IMETHOD GetString(const nsACString & key, nsACString & _retval) { return _to GetString(key, _retval); } \
  NS_IMETHOD GetInt(const nsACString & key, PRInt32 *_retval) { return _to GetInt(key, _retval); } \
  NS_IMETHOD GetFloat(const nsACString & key, float *_retval) { return _to GetFloat(key, _retval); } \
  NS_IMETHOD SetBool(const nsACString & key, PRBool value) { return _to SetBool(key, value); } \
  NS_IMETHOD SetString(const nsACString & key, const nsACString & value) { return _to SetString(key, value); } \
  NS_IMETHOD SetInt(const nsACString & key, PRInt32 value) { return _to SetInt(key, value); } \
  NS_IMETHOD SetFloat(const nsACString & key, float value) { return _to SetFloat(key, value); } \
  NS_IMETHOD GetAppForProtocol(const nsACString & scheme, PRBool *enabled, nsACString & _retval) { return _to GetAppForProtocol(scheme, enabled, _retval); } \
  NS_IMETHOD HandlerRequiresTerminal(const nsACString & scheme, PRBool *_retval) { return _to HandlerRequiresTerminal(scheme, _retval); } \
  NS_IMETHOD SetAppForProtocol(const nsACString & scheme, const nsACString & command) { return _to SetAppForProtocol(scheme, command); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIGCONFSERVICE(_to) \
  NS_IMETHOD GetBool(const nsACString & key, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetBool(key, _retval); } \
  NS_IMETHOD GetString(const nsACString & key, nsACString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetString(key, _retval); } \
  NS_IMETHOD GetInt(const nsACString & key, PRInt32 *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInt(key, _retval); } \
  NS_IMETHOD GetFloat(const nsACString & key, float *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFloat(key, _retval); } \
  NS_IMETHOD SetBool(const nsACString & key, PRBool value) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetBool(key, value); } \
  NS_IMETHOD SetString(const nsACString & key, const nsACString & value) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetString(key, value); } \
  NS_IMETHOD SetInt(const nsACString & key, PRInt32 value) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetInt(key, value); } \
  NS_IMETHOD SetFloat(const nsACString & key, float value) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetFloat(key, value); } \
  NS_IMETHOD GetAppForProtocol(const nsACString & scheme, PRBool *enabled, nsACString & _retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAppForProtocol(scheme, enabled, _retval); } \
  NS_IMETHOD HandlerRequiresTerminal(const nsACString & scheme, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandlerRequiresTerminal(scheme, _retval); } \
  NS_IMETHOD SetAppForProtocol(const nsACString & scheme, const nsACString & command) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetAppForProtocol(scheme, command); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsGConfService : public nsIGConfService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIGCONFSERVICE

  nsGConfService();

private:
  ~nsGConfService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsGConfService, nsIGConfService)

nsGConfService::nsGConfService()
{
  /* member initializers and constructor code */
}

nsGConfService::~nsGConfService()
{
  /* destructor code */
}

/* boolean getBool (in AUTF8String key); */
NS_IMETHODIMP nsGConfService::GetBool(const nsACString & key, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AUTF8String getString (in AUTF8String key); */
NS_IMETHODIMP nsGConfService::GetString(const nsACString & key, nsACString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long getInt (in AUTF8String key); */
NS_IMETHODIMP nsGConfService::GetInt(const nsACString & key, PRInt32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* float getFloat (in AUTF8String key); */
NS_IMETHODIMP nsGConfService::GetFloat(const nsACString & key, float *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setBool (in AUTF8String key, in boolean value); */
NS_IMETHODIMP nsGConfService::SetBool(const nsACString & key, PRBool value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setString (in AUTF8String key, in AUTF8String value); */
NS_IMETHODIMP nsGConfService::SetString(const nsACString & key, const nsACString & value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setInt (in AUTF8String key, in long value); */
NS_IMETHODIMP nsGConfService::SetInt(const nsACString & key, PRInt32 value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setFloat (in AUTF8String key, in float value); */
NS_IMETHODIMP nsGConfService::SetFloat(const nsACString & key, float value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AUTF8String getAppForProtocol (in AUTF8String scheme, out boolean enabled); */
NS_IMETHODIMP nsGConfService::GetAppForProtocol(const nsACString & scheme, PRBool *enabled, nsACString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean handlerRequiresTerminal (in AUTF8String scheme); */
NS_IMETHODIMP nsGConfService::HandlerRequiresTerminal(const nsACString & scheme, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setAppForProtocol (in AUTF8String scheme, in AUTF8String command); */
NS_IMETHODIMP nsGConfService::SetAppForProtocol(const nsACString & scheme, const nsACString & command)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_GCONFSERVICE_CONTRACTID "@mozilla.org/gnome-gconf-service;1"

#endif /* __gen_nsIGConfService_h__ */
