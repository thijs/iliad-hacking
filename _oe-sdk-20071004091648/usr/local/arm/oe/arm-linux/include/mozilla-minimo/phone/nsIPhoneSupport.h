/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/minimo/components/phone/nsIPhoneSupport.idl
 */

#ifndef __gen_nsIPhoneSupport_h__
#define __gen_nsIPhoneSupport_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIPhoneSupport */
#define NS_IPHONESUPPORT_IID_STR "c75982d6-811b-4321-980d-46490e1a6703"

#define NS_IPHONESUPPORT_IID \
  {0xc75982d6, 0x811b, 0x4321, \
    { 0x98, 0x0d, 0x46, 0x49, 0x0e, 0x1a, 0x67, 0x03 }}

class NS_NO_VTABLE nsIPhoneSupport : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IPHONESUPPORT_IID)

  /**
   * This method attempts to make a telephone call.
   *
   * @param telephoneNumber this is the telephone to dial.
   * @param telephoneDescription this is an optional
   * description of the telephone number.
   */
  /* void makeCall (in string telephoneNumber, in string telephoneDescription); */
  NS_IMETHOD MakeCall(const char *telephoneNumber, const char *telephoneDescription) = 0;

  /**
   * This method attempts to send a SMS message.
   *
   * @param smsDest this is the telephone or IP to send the sms message.
   * @param smsMessage this is the message to send.
   */
  /* void sendSMS (in string smsDest, in string smsMessage); */
  NS_IMETHOD SendSMS(const char *smsDest, const char *smsMessage) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPHONESUPPORT \
  NS_IMETHOD MakeCall(const char *telephoneNumber, const char *telephoneDescription); \
  NS_IMETHOD SendSMS(const char *smsDest, const char *smsMessage); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPHONESUPPORT(_to) \
  NS_IMETHOD MakeCall(const char *telephoneNumber, const char *telephoneDescription) { return _to MakeCall(telephoneNumber, telephoneDescription); } \
  NS_IMETHOD SendSMS(const char *smsDest, const char *smsMessage) { return _to SendSMS(smsDest, smsMessage); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPHONESUPPORT(_to) \
  NS_IMETHOD MakeCall(const char *telephoneNumber, const char *telephoneDescription) { return !_to ? NS_ERROR_NULL_POINTER : _to->MakeCall(telephoneNumber, telephoneDescription); } \
  NS_IMETHOD SendSMS(const char *smsDest, const char *smsMessage) { return !_to ? NS_ERROR_NULL_POINTER : _to->SendSMS(smsDest, smsMessage); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsPhoneSupport : public nsIPhoneSupport
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPHONESUPPORT

  nsPhoneSupport();

private:
  ~nsPhoneSupport();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsPhoneSupport, nsIPhoneSupport)

nsPhoneSupport::nsPhoneSupport()
{
  /* member initializers and constructor code */
}

nsPhoneSupport::~nsPhoneSupport()
{
  /* destructor code */
}

/* void makeCall (in string telephoneNumber, in string telephoneDescription); */
NS_IMETHODIMP nsPhoneSupport::MakeCall(const char *telephoneNumber, const char *telephoneDescription)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void sendSMS (in string smsDest, in string smsMessage); */
NS_IMETHODIMP nsPhoneSupport::SendSMS(const char *smsDest, const char *smsMessage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIPhoneSupport_h__ */
