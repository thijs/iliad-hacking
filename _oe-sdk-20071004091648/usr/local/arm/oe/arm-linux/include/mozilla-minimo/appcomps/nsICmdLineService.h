/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/startup/public/nsICmdLineService.idl
 */

#ifndef __gen_nsICmdLineService_h__
#define __gen_nsICmdLineService_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

#ifndef __gen_nsIFactory_h__
#include "nsIFactory.h"
#endif

#ifndef __gen_nsICmdLineHandler_h__
#include "nsICmdLineHandler.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsICmdLineService */
#define NS_ICMDLINESERVICE_IID_STR "e34783f4-ac08-11d2-8d19-00805fc2500c"

#define NS_ICMDLINESERVICE_IID \
  {0xe34783f4, 0xac08, 0x11d2, \
    { 0x8d, 0x19, 0x00, 0x80, 0x5f, 0xc2, 0x50, 0x0c }}

class NS_NO_VTABLE nsICmdLineService : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICMDLINESERVICE_IID)

  /**
     * initialize
     * Used to pass the original argv/argc from main()
     * Warning: This will hold a reference to the original argv
     * passed into Initialze();
     */
  /* [noscript] void initialize (in long argc, out string argv); */
  NS_IMETHOD Initialize(PRInt32 argc, char **argv) = 0;

  /**
     * getCmdLineValue
     * returns the 2nd parameter, if any, to the parameter passed in
     * For example, getCmdLineValue "-edit" will return any url that
     * came after the -edit parameter
     */
  /* string getCmdLineValue (in string argv); */
  NS_IMETHOD GetCmdLineValue(const char *argv, char **_retval) = 0;

  /**
     * URLToLoad
     * The URL to load as passed to the command line
     */
  /* readonly attribute string URLToLoad; */
  NS_IMETHOD GetURLToLoad(char * *aURLToLoad) = 0;

  /**
     * programName
     *
     */
  /* readonly attribute string programName; */
  NS_IMETHOD GetProgramName(char * *aProgramName) = 0;

  /**
     * argc
     * The number of parameters passed in on the command line
     */
  /* readonly attribute long argc; */
  NS_IMETHOD GetArgc(PRInt32 *aArgc) = 0;

  /**
     * argv
     * returns a direct reference to the parameter array passed in
     * to initialize() - do NOT dereference this array!
     */
  /* [noscript] readonly attribute charArray argv; */
  NS_IMETHOD GetArgv(char* * *aArgv) = 0;

  /**
     * Get the command line handler for the given parameter
     * @param param - can be any parameter, with or without leading
     *                "-" such as "-mail" or "edit" - Pass in a null
     *                string if you want the "default" handler
     */
  /* nsICmdLineHandler getHandlerForParam (in string param); */
  NS_IMETHOD GetHandlerForParam(const char *param, nsICmdLineHandler **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICMDLINESERVICE \
  NS_IMETHOD Initialize(PRInt32 argc, char **argv); \
  NS_IMETHOD GetCmdLineValue(const char *argv, char **_retval); \
  NS_IMETHOD GetURLToLoad(char * *aURLToLoad); \
  NS_IMETHOD GetProgramName(char * *aProgramName); \
  NS_IMETHOD GetArgc(PRInt32 *aArgc); \
  NS_IMETHOD GetArgv(char* * *aArgv); \
  NS_IMETHOD GetHandlerForParam(const char *param, nsICmdLineHandler **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICMDLINESERVICE(_to) \
  NS_IMETHOD Initialize(PRInt32 argc, char **argv) { return _to Initialize(argc, argv); } \
  NS_IMETHOD GetCmdLineValue(const char *argv, char **_retval) { return _to GetCmdLineValue(argv, _retval); } \
  NS_IMETHOD GetURLToLoad(char * *aURLToLoad) { return _to GetURLToLoad(aURLToLoad); } \
  NS_IMETHOD GetProgramName(char * *aProgramName) { return _to GetProgramName(aProgramName); } \
  NS_IMETHOD GetArgc(PRInt32 *aArgc) { return _to GetArgc(aArgc); } \
  NS_IMETHOD GetArgv(char* * *aArgv) { return _to GetArgv(aArgv); } \
  NS_IMETHOD GetHandlerForParam(const char *param, nsICmdLineHandler **_retval) { return _to GetHandlerForParam(param, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICMDLINESERVICE(_to) \
  NS_IMETHOD Initialize(PRInt32 argc, char **argv) { return !_to ? NS_ERROR_NULL_POINTER : _to->Initialize(argc, argv); } \
  NS_IMETHOD GetCmdLineValue(const char *argv, char **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCmdLineValue(argv, _retval); } \
  NS_IMETHOD GetURLToLoad(char * *aURLToLoad) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetURLToLoad(aURLToLoad); } \
  NS_IMETHOD GetProgramName(char * *aProgramName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetProgramName(aProgramName); } \
  NS_IMETHOD GetArgc(PRInt32 *aArgc) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetArgc(aArgc); } \
  NS_IMETHOD GetArgv(char* * *aArgv) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetArgv(aArgv); } \
  NS_IMETHOD GetHandlerForParam(const char *param, nsICmdLineHandler **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHandlerForParam(param, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsCmdLineService : public nsICmdLineService
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICMDLINESERVICE

  nsCmdLineService();

private:
  ~nsCmdLineService();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsCmdLineService, nsICmdLineService)

nsCmdLineService::nsCmdLineService()
{
  /* member initializers and constructor code */
}

nsCmdLineService::~nsCmdLineService()
{
  /* destructor code */
}

/* [noscript] void initialize (in long argc, out string argv); */
NS_IMETHODIMP nsCmdLineService::Initialize(PRInt32 argc, char **argv)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* string getCmdLineValue (in string argv); */
NS_IMETHODIMP nsCmdLineService::GetCmdLineValue(const char *argv, char **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string URLToLoad; */
NS_IMETHODIMP nsCmdLineService::GetURLToLoad(char * *aURLToLoad)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string programName; */
NS_IMETHODIMP nsCmdLineService::GetProgramName(char * *aProgramName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute long argc; */
NS_IMETHODIMP nsCmdLineService::GetArgc(PRInt32 *aArgc)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] readonly attribute charArray argv; */
NS_IMETHODIMP nsCmdLineService::GetArgv(char* * *aArgv)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsICmdLineHandler getHandlerForParam (in string param); */
NS_IMETHODIMP nsCmdLineService::GetHandlerForParam(const char *param, nsICmdLineHandler **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsICmdLineService_h__ */
