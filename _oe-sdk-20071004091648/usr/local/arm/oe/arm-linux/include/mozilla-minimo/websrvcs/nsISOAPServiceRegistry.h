/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsISOAPServiceRegistry.idl
 */

#ifndef __gen_nsISOAPServiceRegistry_h__
#define __gen_nsISOAPServiceRegistry_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIDOMElement; /* forward declaration */

class nsISOAPService; /* forward declaration */

class nsISOAPEncodingRegistry; /* forward declaration */


/* starting interface:    nsISOAPServiceRegistry */
#define NS_ISOAPSERVICEREGISTRY_IID_STR "9790d6bc-1dd1-11b2-afe0-bcb310c078bf"

#define NS_ISOAPSERVICEREGISTRY_IID \
  {0x9790d6bc, 0x1dd1, 0x11b2, \
    { 0xaf, 0xe0, 0xbc, 0xb3, 0x10, 0xc0, 0x78, 0xbf }}

/**
 * This interface represents a registry of SOAP services.
 * This registry recieves transports to listen for messages
 *   and services to hand the messages to.  Service registries
 *   may be created as required.  Destroying a service registry
 *   stops the registry's action.  To temporarily register
 *   services, create a new registry.  For proper order of
 *   listening precedence, registries should be destroyed
 *   in reverse order.  Otherwise, a listening priority
 *   would be required.
 */
class NS_NO_VTABLE nsISOAPServiceRegistry {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ISOAPSERVICEREGISTRY_IID)

  /**
   * Process a configuration and add the resulting sources
   *   and services.  This will fail if errors occur during
   *   processing of the configuration.
   *
   * @param aConfiguration Root element of configuration XML.
   */
  /* boolean addConfiguration (in nsIDOMElement aConfiguration); */
  NS_IMETHOD AddConfiguration(nsIDOMElement *aConfiguration, PRBool *_retval) = 0;

  /**
   * Add a transport to be serviced by the registered services.
   *   This will fail if the specified source was already added
   *   with the same setting of the capture flag.
   *
   * @param aTransport string specifying the transport to supply
   *   messages for the service.
   *
   * @param aCapture True if capturing before later declarations
   */
  /* void addSource (in AString aTransport, in boolean aCapture); */
  NS_IMETHOD AddSource(const nsAString & aTransport, PRBool aCapture) = 0;

  /**
   * Add a service to service the registered transports.  This
   *   will fail if the specified service was already added.
   *
   * @param aService Service to be serviced.
   */
  /* void addService (in nsISOAPService aService); */
  NS_IMETHOD AddService(nsISOAPService *aService) = 0;

  /**
   * Registry identifying how to encode and decode
   *   messages containing specific types, automatically
   *   added to messages sent to services in this
   *   registry.
   */
  /* attribute nsISOAPEncodingRegistry encodings; */
  NS_IMETHOD GetEncodings(nsISOAPEncodingRegistry * *aEncodings) = 0;
  NS_IMETHOD SetEncodings(nsISOAPEncodingRegistry * aEncodings) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSISOAPSERVICEREGISTRY \
  NS_IMETHOD AddConfiguration(nsIDOMElement *aConfiguration, PRBool *_retval); \
  NS_IMETHOD AddSource(const nsAString & aTransport, PRBool aCapture); \
  NS_IMETHOD AddService(nsISOAPService *aService); \
  NS_IMETHOD GetEncodings(nsISOAPEncodingRegistry * *aEncodings); \
  NS_IMETHOD SetEncodings(nsISOAPEncodingRegistry * aEncodings); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSISOAPSERVICEREGISTRY(_to) \
  NS_IMETHOD AddConfiguration(nsIDOMElement *aConfiguration, PRBool *_retval) { return _to AddConfiguration(aConfiguration, _retval); } \
  NS_IMETHOD AddSource(const nsAString & aTransport, PRBool aCapture) { return _to AddSource(aTransport, aCapture); } \
  NS_IMETHOD AddService(nsISOAPService *aService) { return _to AddService(aService); } \
  NS_IMETHOD GetEncodings(nsISOAPEncodingRegistry * *aEncodings) { return _to GetEncodings(aEncodings); } \
  NS_IMETHOD SetEncodings(nsISOAPEncodingRegistry * aEncodings) { return _to SetEncodings(aEncodings); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSISOAPSERVICEREGISTRY(_to) \
  NS_IMETHOD AddConfiguration(nsIDOMElement *aConfiguration, PRBool *_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddConfiguration(aConfiguration, _retval); } \
  NS_IMETHOD AddSource(const nsAString & aTransport, PRBool aCapture) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddSource(aTransport, aCapture); } \
  NS_IMETHOD AddService(nsISOAPService *aService) { return !_to ? NS_ERROR_NULL_POINTER : _to->AddService(aService); } \
  NS_IMETHOD GetEncodings(nsISOAPEncodingRegistry * *aEncodings) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEncodings(aEncodings); } \
  NS_IMETHOD SetEncodings(nsISOAPEncodingRegistry * aEncodings) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetEncodings(aEncodings); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsSOAPServiceRegistry : public nsISOAPServiceRegistry
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSISOAPSERVICEREGISTRY

  nsSOAPServiceRegistry();

private:
  ~nsSOAPServiceRegistry();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsSOAPServiceRegistry, nsISOAPServiceRegistry)

nsSOAPServiceRegistry::nsSOAPServiceRegistry()
{
  /* member initializers and constructor code */
}

nsSOAPServiceRegistry::~nsSOAPServiceRegistry()
{
  /* destructor code */
}

/* boolean addConfiguration (in nsIDOMElement aConfiguration); */
NS_IMETHODIMP nsSOAPServiceRegistry::AddConfiguration(nsIDOMElement *aConfiguration, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addSource (in AString aTransport, in boolean aCapture); */
NS_IMETHODIMP nsSOAPServiceRegistry::AddSource(const nsAString & aTransport, PRBool aCapture)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void addService (in nsISOAPService aService); */
NS_IMETHODIMP nsSOAPServiceRegistry::AddService(nsISOAPService *aService)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsISOAPEncodingRegistry encodings; */
NS_IMETHODIMP nsSOAPServiceRegistry::GetEncodings(nsISOAPEncodingRegistry * *aEncodings)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP nsSOAPServiceRegistry::SetEncodings(nsISOAPEncodingRegistry * aEncodings)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_SOAPSERVICEREGISTRY_CID                            \
{ /* 3869184e-1dd2-11b2-aa36-d8333498043a */        \
  0x3869184e, 0x1dd2, 0x11b2,                       \
 {0xaa, 0x36, 0xd8, 0x33, 0x34, 0x98, 0x04, 0x3a} }
#define NS_SOAPSERVICEREGISTRY_CONTRACTID \
"@mozilla.org/xmlextras/soap/serviceregistry;1"
#define NS_SOAPDEFAULTSERVICEREGISTRY_CID                            \
{ /* 9120a01e-1dd2-11b2-a61f-906766927a4f */        \
  0x9120a01e, 0x1dd2, 0x11b2,                       \
 {0xa6, 0x1f, 0x90, 0x67, 0x66, 0x92, 0x7a, 0x4f} }
#define NS_SOAPDEFAULTSERVICEREGISTRY_CONTRACTID \
"@mozilla.org/xmlextras/soap/defaultserviceregistry;1"

#endif /* __gen_nsISOAPServiceRegistry_h__ */
