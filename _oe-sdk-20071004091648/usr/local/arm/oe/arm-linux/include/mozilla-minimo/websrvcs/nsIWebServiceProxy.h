/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/extensions/webservices/public/nsIWebServiceProxy.idl
 */

#ifndef __gen_nsIWebServiceProxy_h__
#define __gen_nsIWebServiceProxy_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIWSDL_h__
#include "nsIWSDL.h"
#endif

#ifndef __gen_nsISimpleEnumerator_h__
#include "nsISimpleEnumerator.h"
#endif

#ifndef __gen_nsIException_h__
#include "nsIException.h"
#endif

#ifndef __gen_nsISOAPResponse_h__
#include "nsISOAPResponse.h"
#endif

#ifndef __gen_nsISOAPBlock_h__
#include "nsISOAPBlock.h"
#endif

#ifndef __gen_nsIScriptableInterfaces_h__
#include "nsIScriptableInterfaces.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsIWebServiceProxy; /* forward declaration */

class nsIWebServiceProxyCreationListener; /* forward declaration */

class nsIInterfaceInfo; /* forward declaration */

class nsIPropertyBag; /* forward declaration */


/* starting interface:    nsIWebServiceProxyFactory */
#define NS_IWEBSERVICEPROXYFACTORY_IID_STR "693611be-bb38-40e0-a98e-b46ff8a5bcca"

#define NS_IWEBSERVICEPROXYFACTORY_IID \
  {0x693611be, 0xbb38, 0x40e0, \
    { 0xa9, 0x8e, 0xb4, 0x6f, 0xf8, 0xa5, 0xbc, 0xca }}

class NS_NO_VTABLE nsIWebServiceProxyFactory : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWEBSERVICEPROXYFACTORY_IID)

  /**
   * Create a service proxy. Loading of the WSDL URL will occur
   * in a synchronous manner. Calls to web service can be made as
   * soon as this method completes.
   *
   * @param wsdlURL The URL of the WSDL service description. This
   *                description will be loaded and used as the basis
   *                for the service proxy.
   * @param portname The name of the port of the service that this 
   *                 service proxy represents. Currently the port
   *                 must represent a SOAP binding.
   * @param qualifier The user-specified qualifier is incorporated into
   *                  the names of XPCOM interfaces created for the 
   *                  service proxy. For C++ callers, this qualifier 
   *                  should be the  same one used in creating the IDL 
   *                  used at compile time. Script callers need not 
   *                  specify a qualifier.
   * @param isAsync If PR_TRUE, the method signatures of the service
   *                proxy represent an asynchronous calling convention.
   *                A callback instance must be registered with the proxy.
   *                A method call to a web service is only completed when
   *                the corresponding callback method is invoked.
   *                If PR_FALSE, the method signatures of the service
   *                proxy represent a synchronous callling convention.
   *                A method call to a web service is completed when the
   *                method call to the proxy returns.
   */
  /* nsIWebServiceProxy createProxy (in AString wsdlURL, in AString portname, in AString qualifier, in boolean isAsync); */
  NS_IMETHOD CreateProxy(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxy **_retval) = 0;

  /**
   * Create a service proxy. Loading of the WSDL URL will occur
   * in an asynchronous manner. Methods on the listener instance will
   * be invoked when the proxy has been created. Any method invocations
   * on the service proxy prior to asynchronous completion of the 
   * intialization step will fail. The caller is guaranteed that this
   * method will return before the listener is invoked.
   *
   * @param wsdlURL The URL of the WSDL service description. This
   *                description will be loaded and used as the basis
   *                for the service proxy.
   * @param portname The name of the port of the service that this 
   *                 service proxy represents. Currently the port
   *                 must represent a SOAP binding.
   * @param qualifier The user-specified qualifier is incorporated into
   *                  the names of XPCOM interfaces created for the 
   *                  service proxy. For C++ callers, this qualifier 
   *                  should be the same one used in creating the IDL 
   *                  used at compile time. Script callers need not 
   *                  specify a qualifier.
   * @param isAsync If PR_TRUE, the method signatures of the service
   *                proxy represent an asynchronous calling convention.
   *                A callback instance must be registered with the proxy.
   *                A method call to a web service is only completed when
   *                the corresponding callback method is invoked.
   *                If PR_FALSE, the method signatures of the service
   *                proxy represent a synchronous callling convention.
   *                A method call to a web service is completed when the
   *                method call to the proxy returns.
   * @param listener The callback instance which will be invoked when
   *                 the proxy is completely initialized.
   */
  /* void createProxyAsync (in AString wsdlURL, in AString portname, in AString qualifier, in boolean isAsync, in nsIWebServiceProxyCreationListener listener); */
  NS_IMETHOD CreateProxyAsync(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxyCreationListener *listener) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEBSERVICEPROXYFACTORY \
  NS_IMETHOD CreateProxy(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxy **_retval); \
  NS_IMETHOD CreateProxyAsync(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxyCreationListener *listener); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEBSERVICEPROXYFACTORY(_to) \
  NS_IMETHOD CreateProxy(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxy **_retval) { return _to CreateProxy(wsdlURL, portname, qualifier, isAsync, _retval); } \
  NS_IMETHOD CreateProxyAsync(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxyCreationListener *listener) { return _to CreateProxyAsync(wsdlURL, portname, qualifier, isAsync, listener); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEBSERVICEPROXYFACTORY(_to) \
  NS_IMETHOD CreateProxy(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxy **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateProxy(wsdlURL, portname, qualifier, isAsync, _retval); } \
  NS_IMETHOD CreateProxyAsync(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxyCreationListener *listener) { return !_to ? NS_ERROR_NULL_POINTER : _to->CreateProxyAsync(wsdlURL, portname, qualifier, isAsync, listener); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWebServiceProxyFactory : public nsIWebServiceProxyFactory
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWEBSERVICEPROXYFACTORY

  nsWebServiceProxyFactory();

private:
  ~nsWebServiceProxyFactory();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWebServiceProxyFactory, nsIWebServiceProxyFactory)

nsWebServiceProxyFactory::nsWebServiceProxyFactory()
{
  /* member initializers and constructor code */
}

nsWebServiceProxyFactory::~nsWebServiceProxyFactory()
{
  /* destructor code */
}

/* nsIWebServiceProxy createProxy (in AString wsdlURL, in AString portname, in AString qualifier, in boolean isAsync); */
NS_IMETHODIMP nsWebServiceProxyFactory::CreateProxy(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxy **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void createProxyAsync (in AString wsdlURL, in AString portname, in AString qualifier, in boolean isAsync, in nsIWebServiceProxyCreationListener listener); */
NS_IMETHODIMP nsWebServiceProxyFactory::CreateProxyAsync(const nsAString & wsdlURL, const nsAString & portname, const nsAString & qualifier, PRBool isAsync, nsIWebServiceProxyCreationListener *listener)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWebServiceProxyCreationListener */
#define NS_IWEBSERVICEPROXYCREATIONLISTENER_IID_STR "a711250b-47da-4f16-a1fd-593de16375a1"

#define NS_IWEBSERVICEPROXYCREATIONLISTENER_IID \
  {0xa711250b, 0x47da, 0x4f16, \
    { 0xa1, 0xfd, 0x59, 0x3d, 0xe1, 0x63, 0x75, 0xa1 }}

class NS_NO_VTABLE nsIWebServiceProxyCreationListener : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWEBSERVICEPROXYCREATIONLISTENER_IID)

  /**
 * This interface should be implemented by a user who creates
 * a web service proxy in an asynchronous manner. An instance
 * of this interface is passed into the <code>initAsync</code>
 * method of the proxy.
 */
/**
   * Invoked when the proxy is completely initialized. Method
   * calls on the proxy can be made once this method is called.
   * 
   * @param proxy The initialized web service proxy
   */
  /* void onLoad (in nsIWebServiceProxy proxy); */
  NS_IMETHOD OnLoad(nsIWebServiceProxy *proxy) = 0;

  /**
   * Invoked if an error occurs during web service proxy
   * initialization. This error code be a result of attempting
   * to load the specified WSDL URL or a result of processing
   * the WSDL and creating the interface information for the 
   * proxy.
   *
   * @param error The exception generated as a result of the
   *                  error. This object can be introspected
   *                  for further information.
   */
  /* void onError (in nsIException error); */
  NS_IMETHOD OnError(nsIException *error) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEBSERVICEPROXYCREATIONLISTENER \
  NS_IMETHOD OnLoad(nsIWebServiceProxy *proxy); \
  NS_IMETHOD OnError(nsIException *error); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEBSERVICEPROXYCREATIONLISTENER(_to) \
  NS_IMETHOD OnLoad(nsIWebServiceProxy *proxy) { return _to OnLoad(proxy); } \
  NS_IMETHOD OnError(nsIException *error) { return _to OnError(error); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEBSERVICEPROXYCREATIONLISTENER(_to) \
  NS_IMETHOD OnLoad(nsIWebServiceProxy *proxy) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnLoad(proxy); } \
  NS_IMETHOD OnError(nsIException *error) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnError(error); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWebServiceProxyCreationListener : public nsIWebServiceProxyCreationListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWEBSERVICEPROXYCREATIONLISTENER

  nsWebServiceProxyCreationListener();

private:
  ~nsWebServiceProxyCreationListener();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWebServiceProxyCreationListener, nsIWebServiceProxyCreationListener)

nsWebServiceProxyCreationListener::nsWebServiceProxyCreationListener()
{
  /* member initializers and constructor code */
}

nsWebServiceProxyCreationListener::~nsWebServiceProxyCreationListener()
{
  /* destructor code */
}

/* void onLoad (in nsIWebServiceProxy proxy); */
NS_IMETHODIMP nsWebServiceProxyCreationListener::OnLoad(nsIWebServiceProxy *proxy)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onError (in nsIException error); */
NS_IMETHODIMP nsWebServiceProxyCreationListener::OnError(nsIException *error)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWebServiceProxy */
#define NS_IWEBSERVICEPROXY_IID_STR "2122421c-1326-41db-87f8-25519d8a12cb"

#define NS_IWEBSERVICEPROXY_IID \
  {0x2122421c, 0x1326, 0x41db, \
    { 0x87, 0xf8, 0x25, 0x51, 0x9d, 0x8a, 0x12, 0xcb }}

class NS_NO_VTABLE nsIWebServiceProxy : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWEBSERVICEPROXY_IID)

  /* [noscript] void Init (in nsIWSDLPort aPort, in nsIInterfaceInfo aPrimaryInterface, in nsIInterfaceInfoManager aInterfaceInfoManager, in AString aQualifier, in boolean aIsAsync); */
  NS_IMETHOD Init(nsIWSDLPort *aPort, nsIInterfaceInfo *aPrimaryInterface, nsIInterfaceInfoManager *aInterfaceInfoManager, const nsAString & aQualifier, PRBool aIsAsync) = 0;

  /**
   * The WSDL port that this service proxy represents.
   *
   * @see nsIWSDLPort
   */
  /* readonly attribute nsIWSDLPort port; */
  NS_IMETHOD GetPort(nsIWSDLPort * *aPort) = 0;

  /**
   * PR_TRUE if the service proxy methods represent an asynchronous
   * calling convention. PR_FALSE if the methods are synchronous.
   */
  /* readonly attribute boolean isAsync; */
  NS_IMETHOD GetIsAsync(PRBool *aIsAsync) = 0;

  /**
   * The qualifier used for interface names related to
   * this service proxy.
   */
  /* readonly attribute AString qualifier; */
  NS_IMETHOD GetQualifier(nsAString & aQualifier) = 0;

  /**
   * An enumerator that returns the set of pending calls for the
   * service proxy. Each call is an instance of the 
   * <code>nsIWebServiceCallContext</code> interface.
   *
   * @see nsIWebServiceCallContext
   */
  /* readonly attribute nsISimpleEnumerator pendingCalls; */
  NS_IMETHOD GetPendingCalls(nsISimpleEnumerator * *aPendingCalls) = 0;

  /**
   * The name of the primary interface for this proxy. This may or may not be
   * the async version depending on whether or not this is an async proxy.
   */
  /* readonly attribute string primaryInterfaceName; */
  NS_IMETHOD GetPrimaryInterfaceName(char * *aPrimaryInterfaceName) = 0;

  /**
   * The name of the primary async listener interface for this proxy. 
   * This will be null if this is not an async proxy.
   */
  /* readonly attribute string primaryAsyncListenerInterfaceName; */
  NS_IMETHOD GetPrimaryAsyncListenerInterfaceName(char * *aPrimaryAsyncListenerInterfaceName) = 0;

  /**
   * The collection of interfaces related to this service proxy. This
   * will include the primary interface implemented by the service
   * proxy as well as any listener or complex type interfaces required
   * for method parameters and return values.
   * NOTE: only interesting from JavaScript.
   */
  /* readonly attribute nsIScriptableInterfaces interfaces; */
  NS_IMETHOD GetInterfaces(nsIScriptableInterfaces * *aInterfaces) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEBSERVICEPROXY \
  NS_IMETHOD Init(nsIWSDLPort *aPort, nsIInterfaceInfo *aPrimaryInterface, nsIInterfaceInfoManager *aInterfaceInfoManager, const nsAString & aQualifier, PRBool aIsAsync); \
  NS_IMETHOD GetPort(nsIWSDLPort * *aPort); \
  NS_IMETHOD GetIsAsync(PRBool *aIsAsync); \
  NS_IMETHOD GetQualifier(nsAString & aQualifier); \
  NS_IMETHOD GetPendingCalls(nsISimpleEnumerator * *aPendingCalls); \
  NS_IMETHOD GetPrimaryInterfaceName(char * *aPrimaryInterfaceName); \
  NS_IMETHOD GetPrimaryAsyncListenerInterfaceName(char * *aPrimaryAsyncListenerInterfaceName); \
  NS_IMETHOD GetInterfaces(nsIScriptableInterfaces * *aInterfaces); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEBSERVICEPROXY(_to) \
  NS_IMETHOD Init(nsIWSDLPort *aPort, nsIInterfaceInfo *aPrimaryInterface, nsIInterfaceInfoManager *aInterfaceInfoManager, const nsAString & aQualifier, PRBool aIsAsync) { return _to Init(aPort, aPrimaryInterface, aInterfaceInfoManager, aQualifier, aIsAsync); } \
  NS_IMETHOD GetPort(nsIWSDLPort * *aPort) { return _to GetPort(aPort); } \
  NS_IMETHOD GetIsAsync(PRBool *aIsAsync) { return _to GetIsAsync(aIsAsync); } \
  NS_IMETHOD GetQualifier(nsAString & aQualifier) { return _to GetQualifier(aQualifier); } \
  NS_IMETHOD GetPendingCalls(nsISimpleEnumerator * *aPendingCalls) { return _to GetPendingCalls(aPendingCalls); } \
  NS_IMETHOD GetPrimaryInterfaceName(char * *aPrimaryInterfaceName) { return _to GetPrimaryInterfaceName(aPrimaryInterfaceName); } \
  NS_IMETHOD GetPrimaryAsyncListenerInterfaceName(char * *aPrimaryAsyncListenerInterfaceName) { return _to GetPrimaryAsyncListenerInterfaceName(aPrimaryAsyncListenerInterfaceName); } \
  NS_IMETHOD GetInterfaces(nsIScriptableInterfaces * *aInterfaces) { return _to GetInterfaces(aInterfaces); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEBSERVICEPROXY(_to) \
  NS_IMETHOD Init(nsIWSDLPort *aPort, nsIInterfaceInfo *aPrimaryInterface, nsIInterfaceInfoManager *aInterfaceInfoManager, const nsAString & aQualifier, PRBool aIsAsync) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aPort, aPrimaryInterface, aInterfaceInfoManager, aQualifier, aIsAsync); } \
  NS_IMETHOD GetPort(nsIWSDLPort * *aPort) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPort(aPort); } \
  NS_IMETHOD GetIsAsync(PRBool *aIsAsync) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsAsync(aIsAsync); } \
  NS_IMETHOD GetQualifier(nsAString & aQualifier) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetQualifier(aQualifier); } \
  NS_IMETHOD GetPendingCalls(nsISimpleEnumerator * *aPendingCalls) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPendingCalls(aPendingCalls); } \
  NS_IMETHOD GetPrimaryInterfaceName(char * *aPrimaryInterfaceName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPrimaryInterfaceName(aPrimaryInterfaceName); } \
  NS_IMETHOD GetPrimaryAsyncListenerInterfaceName(char * *aPrimaryAsyncListenerInterfaceName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPrimaryAsyncListenerInterfaceName(aPrimaryAsyncListenerInterfaceName); } \
  NS_IMETHOD GetInterfaces(nsIScriptableInterfaces * *aInterfaces) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInterfaces(aInterfaces); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWebServiceProxy : public nsIWebServiceProxy
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWEBSERVICEPROXY

  nsWebServiceProxy();

private:
  ~nsWebServiceProxy();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWebServiceProxy, nsIWebServiceProxy)

nsWebServiceProxy::nsWebServiceProxy()
{
  /* member initializers and constructor code */
}

nsWebServiceProxy::~nsWebServiceProxy()
{
  /* destructor code */
}

/* [noscript] void Init (in nsIWSDLPort aPort, in nsIInterfaceInfo aPrimaryInterface, in nsIInterfaceInfoManager aInterfaceInfoManager, in AString aQualifier, in boolean aIsAsync); */
NS_IMETHODIMP nsWebServiceProxy::Init(nsIWSDLPort *aPort, nsIInterfaceInfo *aPrimaryInterface, nsIInterfaceInfoManager *aInterfaceInfoManager, const nsAString & aQualifier, PRBool aIsAsync)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIWSDLPort port; */
NS_IMETHODIMP nsWebServiceProxy::GetPort(nsIWSDLPort * *aPort)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isAsync; */
NS_IMETHODIMP nsWebServiceProxy::GetIsAsync(PRBool *aIsAsync)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString qualifier; */
NS_IMETHODIMP nsWebServiceProxy::GetQualifier(nsAString & aQualifier)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsISimpleEnumerator pendingCalls; */
NS_IMETHODIMP nsWebServiceProxy::GetPendingCalls(nsISimpleEnumerator * *aPendingCalls)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string primaryInterfaceName; */
NS_IMETHODIMP nsWebServiceProxy::GetPrimaryInterfaceName(char * *aPrimaryInterfaceName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string primaryAsyncListenerInterfaceName; */
NS_IMETHODIMP nsWebServiceProxy::GetPrimaryAsyncListenerInterfaceName(char * *aPrimaryAsyncListenerInterfaceName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIScriptableInterfaces interfaces; */
NS_IMETHODIMP nsWebServiceProxy::GetInterfaces(nsIScriptableInterfaces * *aInterfaces)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWebServiceCallContext */
#define NS_IWEBSERVICECALLCONTEXT_IID_STR "87d87900-f102-4a15-b345-7b77a49d2df2"

#define NS_IWEBSERVICECALLCONTEXT_IID \
  {0x87d87900, 0xf102, 0x4a15, \
    { 0xb3, 0x45, 0x7b, 0x77, 0xa4, 0x9d, 0x2d, 0xf2 }}

/**
 * A representation of a method invocation on a web service.
 * An instance of this interface is returned as a result of making
 * an asynchronous call and can be queried for status of the
 * call.
 */
class NS_NO_VTABLE nsIWebServiceCallContext : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWEBSERVICECALLCONTEXT_IID)

  /** 
   * Possible values of the <code>status</code> attribute. A pending
   * call has a status of PENDING. A completed call has a status of
   * SUCCEEDED or FAILED depending on the result of the call.
   */
  enum { PENDING = 0U };

  enum { SUCCEEDED = 1U };

  enum { FAILED = 2U };

  enum { ABORTED = 3U };

  /**
   * The proxy object on which the call was made.
   * 
   * @see nsIWebServiceProxy
   */
  /* readonly attribute nsIWebServiceProxy proxy; */
  NS_IMETHOD GetProxy(nsIWebServiceProxy * *aProxy) = 0;

  /**
   * The name of the method that was invoked.
   */
  /* readonly attribute AString methodName; */
  NS_IMETHOD GetMethodName(nsAString & aMethodName) = 0;

  /**
   * The status of the call, whether pending, completed successfully
   * or completed with a fault.
   */
  /* readonly attribute PRUint32 status; */
  NS_IMETHOD GetStatus(PRUint32 *aStatus) = 0;

  /**
   * The exception generated by the call if the status is FAILURE.
   * The exception object can be introspected for more information.
   * The <code>data</code> member can be QIed to a 
   * <code>nsISOAPFault</code> instance for calls that use a
   * SOAP binding.
   */
  /* readonly attribute nsIException pendingException; */
  NS_IMETHOD GetPendingException(nsIException * *aPendingException) = 0;

  /**
   * The WSDL operation that correpsonds to the method being invoked.
   *
   * @see nsIWSDLOperation
   */
  /* readonly attribute nsIWSDLOperation operation; */
  NS_IMETHOD GetOperation(nsIWSDLOperation * *aOperation) = 0;

  /**
   * Called to abort a pending call. If the call is still pending,
   * its callback instance's <code>onError</code> will be invoked,
   * passing in the specified exception.
   *
   * @param error The exception passed to the callback instance's
   *              <code>onError</code> method.
   */
  /* void abort (in nsIException error); */
  NS_IMETHOD Abort(nsIException *error) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEBSERVICECALLCONTEXT \
  NS_IMETHOD GetProxy(nsIWebServiceProxy * *aProxy); \
  NS_IMETHOD GetMethodName(nsAString & aMethodName); \
  NS_IMETHOD GetStatus(PRUint32 *aStatus); \
  NS_IMETHOD GetPendingException(nsIException * *aPendingException); \
  NS_IMETHOD GetOperation(nsIWSDLOperation * *aOperation); \
  NS_IMETHOD Abort(nsIException *error); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEBSERVICECALLCONTEXT(_to) \
  NS_IMETHOD GetProxy(nsIWebServiceProxy * *aProxy) { return _to GetProxy(aProxy); } \
  NS_IMETHOD GetMethodName(nsAString & aMethodName) { return _to GetMethodName(aMethodName); } \
  NS_IMETHOD GetStatus(PRUint32 *aStatus) { return _to GetStatus(aStatus); } \
  NS_IMETHOD GetPendingException(nsIException * *aPendingException) { return _to GetPendingException(aPendingException); } \
  NS_IMETHOD GetOperation(nsIWSDLOperation * *aOperation) { return _to GetOperation(aOperation); } \
  NS_IMETHOD Abort(nsIException *error) { return _to Abort(error); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEBSERVICECALLCONTEXT(_to) \
  NS_IMETHOD GetProxy(nsIWebServiceProxy * *aProxy) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetProxy(aProxy); } \
  NS_IMETHOD GetMethodName(nsAString & aMethodName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMethodName(aMethodName); } \
  NS_IMETHOD GetStatus(PRUint32 *aStatus) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStatus(aStatus); } \
  NS_IMETHOD GetPendingException(nsIException * *aPendingException) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPendingException(aPendingException); } \
  NS_IMETHOD GetOperation(nsIWSDLOperation * *aOperation) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOperation(aOperation); } \
  NS_IMETHOD Abort(nsIException *error) { return !_to ? NS_ERROR_NULL_POINTER : _to->Abort(error); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWebServiceCallContext : public nsIWebServiceCallContext
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWEBSERVICECALLCONTEXT

  nsWebServiceCallContext();

private:
  ~nsWebServiceCallContext();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWebServiceCallContext, nsIWebServiceCallContext)

nsWebServiceCallContext::nsWebServiceCallContext()
{
  /* member initializers and constructor code */
}

nsWebServiceCallContext::~nsWebServiceCallContext()
{
  /* destructor code */
}

/* readonly attribute nsIWebServiceProxy proxy; */
NS_IMETHODIMP nsWebServiceCallContext::GetProxy(nsIWebServiceProxy * *aProxy)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString methodName; */
NS_IMETHODIMP nsWebServiceCallContext::GetMethodName(nsAString & aMethodName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute PRUint32 status; */
NS_IMETHODIMP nsWebServiceCallContext::GetStatus(PRUint32 *aStatus)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIException pendingException; */
NS_IMETHODIMP nsWebServiceCallContext::GetPendingException(nsIException * *aPendingException)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIWSDLOperation operation; */
NS_IMETHODIMP nsWebServiceCallContext::GetOperation(nsIWSDLOperation * *aOperation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void abort (in nsIException error); */
NS_IMETHODIMP nsWebServiceCallContext::Abort(nsIException *error)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWebServiceSOAPCallContext */
#define NS_IWEBSERVICESOAPCALLCONTEXT_IID_STR "1ef83ece-b645-4b55-a501-df42c3333b47"

#define NS_IWEBSERVICESOAPCALLCONTEXT_IID \
  {0x1ef83ece, 0xb645, 0x4b55, \
    { 0xa5, 0x01, 0xdf, 0x42, 0xc3, 0x33, 0x3b, 0x47 }}

class NS_NO_VTABLE nsIWebServiceSOAPCallContext : public nsIWebServiceCallContext {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWEBSERVICESOAPCALLCONTEXT_IID)

  /**
   * For users who want access to the lower-level constructs that
   * are used for the method invocation, this attributes provides
   * the SOAP response once the call has completed.
   *
   * @see nsISOAPResponse
   */
  /* readonly attribute nsISOAPResponse soapResponse; */
  NS_IMETHOD GetSoapResponse(nsISOAPResponse * *aSoapResponse) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEBSERVICESOAPCALLCONTEXT \
  NS_IMETHOD GetSoapResponse(nsISOAPResponse * *aSoapResponse); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEBSERVICESOAPCALLCONTEXT(_to) \
  NS_IMETHOD GetSoapResponse(nsISOAPResponse * *aSoapResponse) { return _to GetSoapResponse(aSoapResponse); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEBSERVICESOAPCALLCONTEXT(_to) \
  NS_IMETHOD GetSoapResponse(nsISOAPResponse * *aSoapResponse) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSoapResponse(aSoapResponse); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWebServiceSOAPCallContext : public nsIWebServiceSOAPCallContext
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWEBSERVICESOAPCALLCONTEXT

  nsWebServiceSOAPCallContext();

private:
  ~nsWebServiceSOAPCallContext();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWebServiceSOAPCallContext, nsIWebServiceSOAPCallContext)

nsWebServiceSOAPCallContext::nsWebServiceSOAPCallContext()
{
  /* member initializers and constructor code */
}

nsWebServiceSOAPCallContext::~nsWebServiceSOAPCallContext()
{
  /* destructor code */
}

/* readonly attribute nsISOAPResponse soapResponse; */
NS_IMETHODIMP nsWebServiceSOAPCallContext::GetSoapResponse(nsISOAPResponse * *aSoapResponse)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWebServiceComplexTypeWrapper */
#define NS_IWEBSERVICECOMPLEXTYPEWRAPPER_IID_STR "b16b15bc-bfad-43cf-b374-2e4651acbc3c"

#define NS_IWEBSERVICECOMPLEXTYPEWRAPPER_IID \
  {0xb16b15bc, 0xbfad, 0x43cf, \
    { 0xb3, 0x74, 0x2e, 0x46, 0x51, 0xac, 0xbc, 0x3c }}

class NS_NO_VTABLE nsIWebServiceComplexTypeWrapper : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWEBSERVICECOMPLEXTYPEWRAPPER_IID)

  /* void Init (in nsISupports aComplexTypeInstance, in nsIInterfaceInfo aInterfaceInfo); */
  NS_IMETHOD Init(nsISupports *aComplexTypeInstance, nsIInterfaceInfo *aInterfaceInfo) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEBSERVICECOMPLEXTYPEWRAPPER \
  NS_IMETHOD Init(nsISupports *aComplexTypeInstance, nsIInterfaceInfo *aInterfaceInfo); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEBSERVICECOMPLEXTYPEWRAPPER(_to) \
  NS_IMETHOD Init(nsISupports *aComplexTypeInstance, nsIInterfaceInfo *aInterfaceInfo) { return _to Init(aComplexTypeInstance, aInterfaceInfo); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEBSERVICECOMPLEXTYPEWRAPPER(_to) \
  NS_IMETHOD Init(nsISupports *aComplexTypeInstance, nsIInterfaceInfo *aInterfaceInfo) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aComplexTypeInstance, aInterfaceInfo); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWebServiceComplexTypeWrapper : public nsIWebServiceComplexTypeWrapper
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWEBSERVICECOMPLEXTYPEWRAPPER

  nsWebServiceComplexTypeWrapper();

private:
  ~nsWebServiceComplexTypeWrapper();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWebServiceComplexTypeWrapper, nsIWebServiceComplexTypeWrapper)

nsWebServiceComplexTypeWrapper::nsWebServiceComplexTypeWrapper()
{
  /* member initializers and constructor code */
}

nsWebServiceComplexTypeWrapper::~nsWebServiceComplexTypeWrapper()
{
  /* destructor code */
}

/* void Init (in nsISupports aComplexTypeInstance, in nsIInterfaceInfo aInterfaceInfo); */
NS_IMETHODIMP nsWebServiceComplexTypeWrapper::Init(nsISupports *aComplexTypeInstance, nsIInterfaceInfo *aInterfaceInfo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIWebServicePropertyBagWrapper */
#define NS_IWEBSERVICEPROPERTYBAGWRAPPER_IID_STR "e0b1765e-c0be-4a28-aca3-b292c5c3788b"

#define NS_IWEBSERVICEPROPERTYBAGWRAPPER_IID \
  {0xe0b1765e, 0xc0be, 0x4a28, \
    { 0xac, 0xa3, 0xb2, 0x92, 0xc5, 0xc3, 0x78, 0x8b }}

class NS_NO_VTABLE nsIWebServicePropertyBagWrapper : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IWEBSERVICEPROPERTYBAGWRAPPER_IID)

  /* void Init (in nsIPropertyBag aPropertyBag, in nsIInterfaceInfo aInterfaceInfo); */
  NS_IMETHOD Init(nsIPropertyBag *aPropertyBag, nsIInterfaceInfo *aInterfaceInfo) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIWEBSERVICEPROPERTYBAGWRAPPER \
  NS_IMETHOD Init(nsIPropertyBag *aPropertyBag, nsIInterfaceInfo *aInterfaceInfo); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIWEBSERVICEPROPERTYBAGWRAPPER(_to) \
  NS_IMETHOD Init(nsIPropertyBag *aPropertyBag, nsIInterfaceInfo *aInterfaceInfo) { return _to Init(aPropertyBag, aInterfaceInfo); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIWEBSERVICEPROPERTYBAGWRAPPER(_to) \
  NS_IMETHOD Init(nsIPropertyBag *aPropertyBag, nsIInterfaceInfo *aInterfaceInfo) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(aPropertyBag, aInterfaceInfo); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsWebServicePropertyBagWrapper : public nsIWebServicePropertyBagWrapper
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWEBSERVICEPROPERTYBAGWRAPPER

  nsWebServicePropertyBagWrapper();

private:
  ~nsWebServicePropertyBagWrapper();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsWebServicePropertyBagWrapper, nsIWebServicePropertyBagWrapper)

nsWebServicePropertyBagWrapper::nsWebServicePropertyBagWrapper()
{
  /* member initializers and constructor code */
}

nsWebServicePropertyBagWrapper::~nsWebServicePropertyBagWrapper()
{
  /* destructor code */
}

/* void Init (in nsIPropertyBag aPropertyBag, in nsIInterfaceInfo aInterfaceInfo); */
NS_IMETHODIMP nsWebServicePropertyBagWrapper::Init(nsIPropertyBag *aPropertyBag, nsIInterfaceInfo *aInterfaceInfo)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_WEBSERVICEPROXYFACTORY_CLASSID           \
{ /* 4E557E69-CCE0-47da-A7D3-2A7ED666F566 */        \
 0x4e557e69, 0xcce0, 0x47da,                        \
 {0xa7, 0xd3, 0x2a, 0x7e, 0xd6, 0x66, 0xf5, 0x66}}
#define NS_WEBSERVICEPROXYFACTORY_CONTRACTID "@mozilla.org/xmlextras/proxy/webserviceproxyfactory;1"
#define NS_WEBSERVICEPROXY_CLASSID                  \
{ /* 1220efd4-8018-45b0-bfc1-0c0716ee0bfb */        \
 0x1220efd4, 0x8018, 0x45b0,                        \
 {0xbf, 0xc1, 0x0c, 0x07, 0x16, 0xee, 0x0b, 0xfb}}
#define NS_WEBSERVICEPROXY_CONTRACTID "@mozilla.org/xmlextras/proxy/webserviceproxy;1"
#define NS_WEBSERVICECOMPLEXTYPEWRAPPER_CLASSID     \
{ /* 615272eb-3908-4fcc-b8f6-94d5a146e2bc */        \
 0x615272eb, 0x3908, 0x4fcc,                        \
 {0xb8, 0xf6, 0x94, 0xd5, 0xa1, 0x46, 0xe2, 0xbc}}
#define NS_WEBSERVICECOMPLEXTYPEWRAPPER_CONTRACTID "@mozilla.org/xmlextras/proxy/webservicecomplextypewrapper;1"
#define NS_WEBSERVICEPROPERTYBAGWRAPPER_CLASSID     \
{ /* 1c76aea3-0810-45b7-bce9-03abc209eb8c */        \
 0x1c76aea3, 0x0810, 0x45b7,                        \
 {0xbc, 0xe9, 0x03, 0xab, 0xc2, 0x09, 0xeb, 0x8c}} 
#define NS_WEBSERVICEPROPERTYBAGWRAPPER_CONTRACTID "@mozilla.org/xmlextras/proxy/webservicepropertybagwrapper;1"

#endif /* __gen_nsIWebServiceProxy_h__ */
