/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/modules/plugin/base/public/nsIPluginHost.idl
 */

#ifndef __gen_nsIPluginHost_h__
#define __gen_nsIPluginHost_h__


#ifndef __gen_nspluginroot_h__
#include "nspluginroot.h"
#endif

#ifndef __gen_nsIFactory_h__
#include "nsIFactory.h"
#endif

#ifndef __gen_nsIPluginInstanceOwner_h__
#include "nsIPluginInstanceOwner.h"
#endif

#ifndef __gen_nsIStreamListener_h__
#include "nsIStreamListener.h"
#endif

#ifndef __gen_nsIStringStream_h__
#include "nsIStringStream.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#include "nsplugindefs.h"
#include "nsString.h"
#include "nsNetUtil.h"
#include "prlink.h"  // for PRLibrary
class nsIPlugin; /* forward declaration */

class nsIURI; /* forward declaration */

class nsIDOMPlugin; /* forward declaration */

class nsIChannel; /* forward declaration */


/* starting interface:    nsIPluginHost */
#define NS_IPLUGINHOST_IID_STR "abb36800-ea77-4172-ad14-603a02db2b53"

#define NS_IPLUGINHOST_IID \
  {0xabb36800, 0xea77, 0x4172, \
    { 0xad, 0x14, 0x60, 0x3a, 0x02, 0xdb, 0x2b, 0x53 }}

class NS_NO_VTABLE nsIPluginHost : public nsIFactory {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IPLUGINHOST_IID)

  /* void init (); */
  NS_IMETHOD Init(void) = 0;

  /* void destroy (); */
  NS_IMETHOD Destroy(void) = 0;

  /* void loadPlugins (); */
  NS_IMETHOD LoadPlugins(void) = 0;

  /* void getPluginFactory (in string aMimeType, out nsIPlugin aPlugin); */
  NS_IMETHOD GetPluginFactory(const char *aMimeType, nsIPlugin **aPlugin) = 0;

  /* void instantiateEmbeddedPlugin (in string aMimeType, in nsIURI aURL, in nsIPluginInstanceOwner aOwner); */
  NS_IMETHOD InstantiateEmbeddedPlugin(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner) = 0;

  /* void instantiateFullPagePlugin (in string aMimeType, in nsIURI aURI, in nsIStreamListenerRef aStreamListener, in nsIPluginInstanceOwner aOwner); */
  NS_IMETHOD InstantiateFullPagePlugin(const char *aMimeType, nsIURI *aURI, nsIStreamListener * & aStreamListener, nsIPluginInstanceOwner *aOwner) = 0;

  /**
   * Instantiate an embedded plugin for an existing channel. The caller is
   * responsible for opening the channel. It may or may not be already opened
   * when this function is called.
   */
  /* nsIStreamListener instantiatePluginForChannel (in nsIChannel aChannel, in nsIPluginInstanceOwner aOwner); */
  NS_IMETHOD InstantiatePluginForChannel(nsIChannel *aChannel, nsIPluginInstanceOwner *aOwner, nsIStreamListener **_retval) = 0;

  /* void setUpPluginInstance (in string aMimeType, in nsIURI aURL, in nsIPluginInstanceOwner aOwner); */
  NS_IMETHOD SetUpPluginInstance(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner) = 0;

  /* void isPluginEnabledForType (in string aMimeType); */
  NS_IMETHOD IsPluginEnabledForType(const char *aMimeType) = 0;

  /* void isPluginEnabledForExtension (in string aExtension, in constCharStarRef aMimeType); */
  NS_IMETHOD IsPluginEnabledForExtension(const char *aExtension, const char * & aMimeType) = 0;

  /* readonly attribute unsigned long pluginCount; */
  NS_IMETHOD GetPluginCount(PRUint32 *aPluginCount) = 0;

  /* [noscript] void getPlugins (in unsigned long aPluginCount, out nsIDOMPlugin aPluginArray); */
  NS_IMETHOD GetPlugins(PRUint32 aPluginCount, nsIDOMPlugin **aPluginArray) = 0;

  /* void stopPluginInstance (in nsIPluginInstance aInstance); */
  NS_IMETHOD StopPluginInstance(nsIPluginInstance *aInstance) = 0;

  /* void handleBadPlugin (in PRLibraryPtr aLibrary, in nsIPluginInstance instance); */
  NS_IMETHOD HandleBadPlugin(PRLibrary * aLibrary, nsIPluginInstance *instance) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIPLUGINHOST \
  NS_IMETHOD Init(void); \
  NS_IMETHOD Destroy(void); \
  NS_IMETHOD LoadPlugins(void); \
  NS_IMETHOD GetPluginFactory(const char *aMimeType, nsIPlugin **aPlugin); \
  NS_IMETHOD InstantiateEmbeddedPlugin(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner); \
  NS_IMETHOD InstantiateFullPagePlugin(const char *aMimeType, nsIURI *aURI, nsIStreamListener * & aStreamListener, nsIPluginInstanceOwner *aOwner); \
  NS_IMETHOD InstantiatePluginForChannel(nsIChannel *aChannel, nsIPluginInstanceOwner *aOwner, nsIStreamListener **_retval); \
  NS_IMETHOD SetUpPluginInstance(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner); \
  NS_IMETHOD IsPluginEnabledForType(const char *aMimeType); \
  NS_IMETHOD IsPluginEnabledForExtension(const char *aExtension, const char * & aMimeType); \
  NS_IMETHOD GetPluginCount(PRUint32 *aPluginCount); \
  NS_IMETHOD GetPlugins(PRUint32 aPluginCount, nsIDOMPlugin **aPluginArray); \
  NS_IMETHOD StopPluginInstance(nsIPluginInstance *aInstance); \
  NS_IMETHOD HandleBadPlugin(PRLibrary * aLibrary, nsIPluginInstance *instance); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIPLUGINHOST(_to) \
  NS_IMETHOD Init(void) { return _to Init(); } \
  NS_IMETHOD Destroy(void) { return _to Destroy(); } \
  NS_IMETHOD LoadPlugins(void) { return _to LoadPlugins(); } \
  NS_IMETHOD GetPluginFactory(const char *aMimeType, nsIPlugin **aPlugin) { return _to GetPluginFactory(aMimeType, aPlugin); } \
  NS_IMETHOD InstantiateEmbeddedPlugin(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner) { return _to InstantiateEmbeddedPlugin(aMimeType, aURL, aOwner); } \
  NS_IMETHOD InstantiateFullPagePlugin(const char *aMimeType, nsIURI *aURI, nsIStreamListener * & aStreamListener, nsIPluginInstanceOwner *aOwner) { return _to InstantiateFullPagePlugin(aMimeType, aURI, aStreamListener, aOwner); } \
  NS_IMETHOD InstantiatePluginForChannel(nsIChannel *aChannel, nsIPluginInstanceOwner *aOwner, nsIStreamListener **_retval) { return _to InstantiatePluginForChannel(aChannel, aOwner, _retval); } \
  NS_IMETHOD SetUpPluginInstance(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner) { return _to SetUpPluginInstance(aMimeType, aURL, aOwner); } \
  NS_IMETHOD IsPluginEnabledForType(const char *aMimeType) { return _to IsPluginEnabledForType(aMimeType); } \
  NS_IMETHOD IsPluginEnabledForExtension(const char *aExtension, const char * & aMimeType) { return _to IsPluginEnabledForExtension(aExtension, aMimeType); } \
  NS_IMETHOD GetPluginCount(PRUint32 *aPluginCount) { return _to GetPluginCount(aPluginCount); } \
  NS_IMETHOD GetPlugins(PRUint32 aPluginCount, nsIDOMPlugin **aPluginArray) { return _to GetPlugins(aPluginCount, aPluginArray); } \
  NS_IMETHOD StopPluginInstance(nsIPluginInstance *aInstance) { return _to StopPluginInstance(aInstance); } \
  NS_IMETHOD HandleBadPlugin(PRLibrary * aLibrary, nsIPluginInstance *instance) { return _to HandleBadPlugin(aLibrary, instance); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIPLUGINHOST(_to) \
  NS_IMETHOD Init(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(); } \
  NS_IMETHOD Destroy(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Destroy(); } \
  NS_IMETHOD LoadPlugins(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->LoadPlugins(); } \
  NS_IMETHOD GetPluginFactory(const char *aMimeType, nsIPlugin **aPlugin) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPluginFactory(aMimeType, aPlugin); } \
  NS_IMETHOD InstantiateEmbeddedPlugin(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner) { return !_to ? NS_ERROR_NULL_POINTER : _to->InstantiateEmbeddedPlugin(aMimeType, aURL, aOwner); } \
  NS_IMETHOD InstantiateFullPagePlugin(const char *aMimeType, nsIURI *aURI, nsIStreamListener * & aStreamListener, nsIPluginInstanceOwner *aOwner) { return !_to ? NS_ERROR_NULL_POINTER : _to->InstantiateFullPagePlugin(aMimeType, aURI, aStreamListener, aOwner); } \
  NS_IMETHOD InstantiatePluginForChannel(nsIChannel *aChannel, nsIPluginInstanceOwner *aOwner, nsIStreamListener **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->InstantiatePluginForChannel(aChannel, aOwner, _retval); } \
  NS_IMETHOD SetUpPluginInstance(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner) { return !_to ? NS_ERROR_NULL_POINTER : _to->SetUpPluginInstance(aMimeType, aURL, aOwner); } \
  NS_IMETHOD IsPluginEnabledForType(const char *aMimeType) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsPluginEnabledForType(aMimeType); } \
  NS_IMETHOD IsPluginEnabledForExtension(const char *aExtension, const char * & aMimeType) { return !_to ? NS_ERROR_NULL_POINTER : _to->IsPluginEnabledForExtension(aExtension, aMimeType); } \
  NS_IMETHOD GetPluginCount(PRUint32 *aPluginCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPluginCount(aPluginCount); } \
  NS_IMETHOD GetPlugins(PRUint32 aPluginCount, nsIDOMPlugin **aPluginArray) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPlugins(aPluginCount, aPluginArray); } \
  NS_IMETHOD StopPluginInstance(nsIPluginInstance *aInstance) { return !_to ? NS_ERROR_NULL_POINTER : _to->StopPluginInstance(aInstance); } \
  NS_IMETHOD HandleBadPlugin(PRLibrary * aLibrary, nsIPluginInstance *instance) { return !_to ? NS_ERROR_NULL_POINTER : _to->HandleBadPlugin(aLibrary, instance); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsPluginHost : public nsIPluginHost
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIPLUGINHOST

  nsPluginHost();

private:
  ~nsPluginHost();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsPluginHost, nsIPluginHost)

nsPluginHost::nsPluginHost()
{
  /* member initializers and constructor code */
}

nsPluginHost::~nsPluginHost()
{
  /* destructor code */
}

/* void init (); */
NS_IMETHODIMP nsPluginHost::Init()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void destroy (); */
NS_IMETHODIMP nsPluginHost::Destroy()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void loadPlugins (); */
NS_IMETHODIMP nsPluginHost::LoadPlugins()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getPluginFactory (in string aMimeType, out nsIPlugin aPlugin); */
NS_IMETHODIMP nsPluginHost::GetPluginFactory(const char *aMimeType, nsIPlugin **aPlugin)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void instantiateEmbeddedPlugin (in string aMimeType, in nsIURI aURL, in nsIPluginInstanceOwner aOwner); */
NS_IMETHODIMP nsPluginHost::InstantiateEmbeddedPlugin(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void instantiateFullPagePlugin (in string aMimeType, in nsIURI aURI, in nsIStreamListenerRef aStreamListener, in nsIPluginInstanceOwner aOwner); */
NS_IMETHODIMP nsPluginHost::InstantiateFullPagePlugin(const char *aMimeType, nsIURI *aURI, nsIStreamListener * & aStreamListener, nsIPluginInstanceOwner *aOwner)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIStreamListener instantiatePluginForChannel (in nsIChannel aChannel, in nsIPluginInstanceOwner aOwner); */
NS_IMETHODIMP nsPluginHost::InstantiatePluginForChannel(nsIChannel *aChannel, nsIPluginInstanceOwner *aOwner, nsIStreamListener **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setUpPluginInstance (in string aMimeType, in nsIURI aURL, in nsIPluginInstanceOwner aOwner); */
NS_IMETHODIMP nsPluginHost::SetUpPluginInstance(const char *aMimeType, nsIURI *aURL, nsIPluginInstanceOwner *aOwner)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void isPluginEnabledForType (in string aMimeType); */
NS_IMETHODIMP nsPluginHost::IsPluginEnabledForType(const char *aMimeType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void isPluginEnabledForExtension (in string aExtension, in constCharStarRef aMimeType); */
NS_IMETHODIMP nsPluginHost::IsPluginEnabledForExtension(const char *aExtension, const char * & aMimeType)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long pluginCount; */
NS_IMETHODIMP nsPluginHost::GetPluginCount(PRUint32 *aPluginCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void getPlugins (in unsigned long aPluginCount, out nsIDOMPlugin aPluginArray); */
NS_IMETHODIMP nsPluginHost::GetPlugins(PRUint32 aPluginCount, nsIDOMPlugin **aPluginArray)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void stopPluginInstance (in nsIPluginInstance aInstance); */
NS_IMETHODIMP nsPluginHost::StopPluginInstance(nsIPluginInstance *aInstance)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void handleBadPlugin (in PRLibraryPtr aLibrary, in nsIPluginInstance instance); */
NS_IMETHODIMP nsPluginHost::HandleBadPlugin(PRLibrary * aLibrary, nsIPluginInstance *instance)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

/**
 * Used for creating the correct input stream for plugins
 * We can either have raw data (with or without \r\n\r\n) or a path to a file (but it must be native!)
 * When making an nsIInputStream stream for the plugins POST data, be sure to take into 
 * account that it could be binary and full of nulls, see bug 105417. Also, we need 
 * to make a copy of the buffer because the plugin may have allocated it on the stack.
 * For an example of this, see Shockwave registration or bug 108966
 * We malloc only for headers here, buffer for data itself is malloced by ParsePostBufferToFixHeaders()
 */
inline nsresult
NS_NewPluginPostDataStream(nsIInputStream **result,
                           const char *data,
                           PRUint32 contentLength,
                           PRBool isFile = PR_FALSE,
                           PRBool headers = PR_FALSE)
{
  nsresult rv = NS_ERROR_UNEXPECTED;
  if (!data)
    return rv;
  if (!isFile) { // do raw data case first
    if (contentLength < 1)
      return rv;
    
    char *buf = (char*) data;
    if (headers) {
      // in assumption we got correctly formated headers just passed in
      if (!(buf = (char*)nsMemory::Alloc(contentLength)))
        return NS_ERROR_OUT_OF_MEMORY;
      memcpy(buf, data, contentLength);
    }
    nsCOMPtr<nsIStringInputStream> sis = do_CreateInstance("@mozilla.org/io/string-input-stream;1",&rv);
    if (NS_SUCCEEDED(rv)) {
      sis->AdoptData(buf, contentLength);  // let the string stream manage our data
      rv = CallQueryInterface(sis, result);
    }
    else if (headers)
      nsMemory::Free(buf); // Cleanup the memory if the data was copied.
  } else {
    nsCOMPtr<nsILocalFile> file; // tmp file will be deleted on release of stream
    nsCOMPtr<nsIInputStream> fileStream;
    if (NS_SUCCEEDED(rv = NS_NewNativeLocalFile(nsDependentCString(data), PR_FALSE, getter_AddRefs(file))) &&
        NS_SUCCEEDED(rv = NS_NewLocalFileInputStream(getter_AddRefs(fileStream),
                                     file,
                                     PR_RDONLY,
                                     0600,
                                     nsIFileInputStream::DELETE_ON_CLOSE |
                                     nsIFileInputStream::CLOSE_ON_EOF))
                                     ) 
    {
      // wrap the file stream with a buffered input stream
      return NS_NewBufferedInputStream(result, fileStream, 8192);
    }
  }
  return rv;
}

#endif /* __gen_nsIPluginHost_h__ */
