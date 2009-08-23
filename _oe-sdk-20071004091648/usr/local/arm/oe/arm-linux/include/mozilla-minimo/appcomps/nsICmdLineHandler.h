/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/xpfe/components/startup/public/nsICmdLineHandler.idl
 */

#ifndef __gen_nsICmdLineHandler_h__
#define __gen_nsICmdLineHandler_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
#define COMMAND_LINE_ARGUMENT_HANDLERS "command-line-argument-handlers"
#define PREF_STARTUP_PREFIX "general.startup."
#include "nsCOMPtr.h"
#include "nsIComponentManager.h"
#include "nsICategoryManager.h"
#include "nsIFile.h"
#include "nsIServiceManager.h"
struct nsModuleComponentInfo;   // forward declaration
#define CMDLINEHANDLER_REGISTERPROC_DECLS \
static NS_METHOD RegisterProc(nsIComponentManager *aCompMgr, nsIFile *aPath, const char *registryLocation, const char *componentType, const nsModuleComponentInfo *info);  \
static NS_METHOD UnregisterProc(nsIComponentManager *aCompMgr, nsIFile *aPath, const char *registryLocation, const nsModuleComponentInfo *info);
#define CMDLINEHANDLER_GETDEFAULTARGS_IMPL(macro_class,macro_default_args)\
NS_IMETHODIMP macro_class::GetDefaultArgs(PRUnichar **aDefaultArgs) \
{ \
    if (!aDefaultArgs) return NS_ERROR_FAILURE; \
    *aDefaultArgs = ToNewUnicode(nsDependentCString(macro_default_args)); \
    return NS_OK; \
}
#define CMDLINEHANDLER_GETCHROMEURL_IMPL(macro_class,macro_chrome_url)\
NS_IMETHODIMP macro_class::GetChromeUrlForTask(char **aChromeUrlForTask) \
{ \
    if (!aChromeUrlForTask) return NS_ERROR_FAILURE; \
    *aChromeUrlForTask = PL_strdup(macro_chrome_url); \
    return NS_OK; \
}
#define CMDLINEHANDLER_OTHERS_IMPL(macro_class,macro_cmd_line_arg,macro_pref_name,macro_help_text, macro_handles_args, macro_open_window_with_args)\
NS_IMETHODIMP macro_class::GetCommandLineArgument(char **aCommandLineArgument) \
{ \
    if (!aCommandLineArgument) return NS_ERROR_FAILURE; \
    *aCommandLineArgument = PL_strdup(macro_cmd_line_arg); \
    return NS_OK; \
} \
NS_IMETHODIMP macro_class::GetPrefNameForStartup(char **aPrefNameForStartup) \
{ \
    if (!aPrefNameForStartup) return NS_ERROR_FAILURE; \
    *aPrefNameForStartup = PL_strdup(macro_pref_name); \
    return NS_OK; \
} \
NS_IMETHODIMP macro_class::GetHelpText(char **aHelpText) \
{ \
    if (!aHelpText) return NS_ERROR_FAILURE; \
    *aHelpText = PL_strdup(macro_help_text); \
    return NS_OK; \
} \
NS_IMETHODIMP macro_class::GetHandlesArgs(PRBool *aHandlesArgs) \
{ \
    if (!aHandlesArgs) return NS_ERROR_FAILURE; \
    *aHandlesArgs = macro_handles_args; \
    return NS_OK; \
} \
NS_IMETHODIMP macro_class::GetOpenWindowWithArgs(PRBool *aOpenWindowWithArgs) \
{ \
    if (!aOpenWindowWithArgs) return NS_ERROR_FAILURE; \
    *aOpenWindowWithArgs = macro_open_window_with_args; \
    return NS_OK; \
}
#define CMDLINEHANDLER_REGISTERPROC_IMPL(macro_class, \
                                         macro_pretty_name, \
                                         macro_contractid) \
NS_METHOD macro_class::RegisterProc(nsIComponentManager *aCompMgr, nsIFile *aPath, const char *registryLocation, const char *componentType, const nsModuleComponentInfo *info) \
{ \
    nsresult rv; \
    nsCOMPtr<nsICategoryManager> catman = do_GetService("@mozilla.org/categorymanager;1", &rv); \
    if (NS_FAILED(rv)) return rv; \
    nsXPIDLCString prevEntry; \
    rv = catman->AddCategoryEntry(COMMAND_LINE_ARGUMENT_HANDLERS, macro_pretty_name, macro_contractid, PR_TRUE, PR_TRUE, getter_Copies(prevEntry)); \
	return NS_OK; \
} \
NS_METHOD macro_class::UnregisterProc(nsIComponentManager *aCompMgr, nsIFile *aPath, const char *registryLocation, const nsModuleComponentInfo *info) \
{ \
    nsresult rv; \
    nsCOMPtr<nsICategoryManager> catman = do_GetService("@mozilla.org/categorymanager;1", &rv); \
    if (NS_FAILED(rv)) return rv; \
    rv = catman->DeleteCategoryEntry(COMMAND_LINE_ARGUMENT_HANDLERS, macro_contractid, PR_TRUE); \
    return NS_OK; \
}
// basic definition
#define CMDLINEHANDLER1_IMPL(macro_class, \
                             macro_cmd_line_arg, \
                             macro_pref_name, \
                             macro_help_text, \
                             macro_handles_args, \
                             macro_default_args, \
                             macro_open_window_with_args)\
CMDLINEHANDLER_OTHERS_IMPL(macro_class, macro_cmd_line_arg,macro_pref_name,\
                           macro_help_text, macro_handles_args, \
                           macro_open_window_with_args)
// provides everything above, plus GetChromeUrl and regproc stuff
#define CMDLINEHANDLER2_IMPL(macro_class, \
                             macro_cmd_line_arg, \
                             macro_pref_name, \
                             macro_chrome_url, \
                             macro_help_text, \
                             macro_contractid, \
                             macro_pretty_name, \
                             macro_handles_args, \
                             macro_open_window_with_args) \
CMDLINEHANDLER_OTHERS_IMPL(macro_class,macro_cmd_line_arg,macro_pref_name,\
                           macro_help_text, macro_handles_args, \
                           macro_open_window_with_args) \
CMDLINEHANDLER_REGISTERPROC_IMPL(macro_class, macro_pretty_name, \
                                 macro_contractid) \
CMDLINEHANDLER_GETCHROMEURL_IMPL(macro_class,macro_chrome_url)
// deprecated: includes everything above, plus getdefaultargs
#define CMDLINEHANDLER_IMPL(macro_class, \
                            macro_cmd_line_arg,\
                            macro_pref_name, \
                            macro_chrome_url,\
                            macro_help_text, \
                            macro_contractid, \
                            macro_pretty_name, \
                            macro_handles_args, \
                            macro_default_args, \
                            macro_open_window_with_args) \
CMDLINEHANDLER_GETDEFAULTARGS_IMPL(macro_class,macro_default_args) \
CMDLINEHANDLER2_IMPL(macro_class,macro_cmd_line_arg,macro_pref_name, \
                     macro_chrome_url,macro_help_text, macro_contractid, \
                     macro_pretty_name, macro_handles_args, \
                     macro_open_window_with_args)
//Creates a version without the GetChromeUrlForTaskFunction
#define CMDLINEHANDLER3_IMPL(macro_class, \
                             macro_cmd_line_arg, \
                             macro_pref_name, \
                             macro_help_text, \
                             macro_contractid, \
                             macro_pretty_name, \
                             macro_handles_args, \
                             macro_default_args, \
                             macro_open_window_with_args)\
CMDLINEHANDLER_GETDEFAULTARGS_IMPL(macro_class,macro_default_args)\
CMDLINEHANDLER_OTHERS_IMPL(macro_class,macro_cmd_line_arg,macro_pref_name,\
                           macro_help_text, macro_handles_args, \
                           macro_open_window_with_args) \
CMDLINEHANDLER_REGISTERPROC_IMPL(macro_class, macro_pretty_name, \
                                    macro_contractid)

/* starting interface:    nsICmdLineHandler */
#define NS_ICMDLINEHANDLER_IID_STR "ae12670a-1dd1-11b2-80cd-82ec93559deb"

#define NS_ICMDLINEHANDLER_IID \
  {0xae12670a, 0x1dd1, 0x11b2, \
    { 0x80, 0xcd, 0x82, 0xec, 0x93, 0x55, 0x9d, 0xeb }}

class NS_NO_VTABLE nsICmdLineHandler : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_ICMDLINEHANDLER_IID)

  /* readonly attribute string commandLineArgument; */
  NS_IMETHOD GetCommandLineArgument(char * *aCommandLineArgument) = 0;

  /* readonly attribute string prefNameForStartup; */
  NS_IMETHOD GetPrefNameForStartup(char * *aPrefNameForStartup) = 0;

  /* readonly attribute string chromeUrlForTask; */
  NS_IMETHOD GetChromeUrlForTask(char * *aChromeUrlForTask) = 0;

  /* readonly attribute string helpText; */
  NS_IMETHOD GetHelpText(char * *aHelpText) = 0;

  /* readonly attribute boolean handlesArgs; */
  NS_IMETHOD GetHandlesArgs(PRBool *aHandlesArgs) = 0;

  /* readonly attribute wstring defaultArgs; */
  NS_IMETHOD GetDefaultArgs(PRUnichar * *aDefaultArgs) = 0;

  /* readonly attribute boolean openWindowWithArgs; */
  NS_IMETHOD GetOpenWindowWithArgs(PRBool *aOpenWindowWithArgs) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSICMDLINEHANDLER \
  NS_IMETHOD GetCommandLineArgument(char * *aCommandLineArgument); \
  NS_IMETHOD GetPrefNameForStartup(char * *aPrefNameForStartup); \
  NS_IMETHOD GetChromeUrlForTask(char * *aChromeUrlForTask); \
  NS_IMETHOD GetHelpText(char * *aHelpText); \
  NS_IMETHOD GetHandlesArgs(PRBool *aHandlesArgs); \
  NS_IMETHOD GetDefaultArgs(PRUnichar * *aDefaultArgs); \
  NS_IMETHOD GetOpenWindowWithArgs(PRBool *aOpenWindowWithArgs); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSICMDLINEHANDLER(_to) \
  NS_IMETHOD GetCommandLineArgument(char * *aCommandLineArgument) { return _to GetCommandLineArgument(aCommandLineArgument); } \
  NS_IMETHOD GetPrefNameForStartup(char * *aPrefNameForStartup) { return _to GetPrefNameForStartup(aPrefNameForStartup); } \
  NS_IMETHOD GetChromeUrlForTask(char * *aChromeUrlForTask) { return _to GetChromeUrlForTask(aChromeUrlForTask); } \
  NS_IMETHOD GetHelpText(char * *aHelpText) { return _to GetHelpText(aHelpText); } \
  NS_IMETHOD GetHandlesArgs(PRBool *aHandlesArgs) { return _to GetHandlesArgs(aHandlesArgs); } \
  NS_IMETHOD GetDefaultArgs(PRUnichar * *aDefaultArgs) { return _to GetDefaultArgs(aDefaultArgs); } \
  NS_IMETHOD GetOpenWindowWithArgs(PRBool *aOpenWindowWithArgs) { return _to GetOpenWindowWithArgs(aOpenWindowWithArgs); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSICMDLINEHANDLER(_to) \
  NS_IMETHOD GetCommandLineArgument(char * *aCommandLineArgument) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCommandLineArgument(aCommandLineArgument); } \
  NS_IMETHOD GetPrefNameForStartup(char * *aPrefNameForStartup) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetPrefNameForStartup(aPrefNameForStartup); } \
  NS_IMETHOD GetChromeUrlForTask(char * *aChromeUrlForTask) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetChromeUrlForTask(aChromeUrlForTask); } \
  NS_IMETHOD GetHelpText(char * *aHelpText) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHelpText(aHelpText); } \
  NS_IMETHOD GetHandlesArgs(PRBool *aHandlesArgs) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHandlesArgs(aHandlesArgs); } \
  NS_IMETHOD GetDefaultArgs(PRUnichar * *aDefaultArgs) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetDefaultArgs(aDefaultArgs); } \
  NS_IMETHOD GetOpenWindowWithArgs(PRBool *aOpenWindowWithArgs) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetOpenWindowWithArgs(aOpenWindowWithArgs); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsCmdLineHandler : public nsICmdLineHandler
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSICMDLINEHANDLER

  nsCmdLineHandler();

private:
  ~nsCmdLineHandler();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsCmdLineHandler, nsICmdLineHandler)

nsCmdLineHandler::nsCmdLineHandler()
{
  /* member initializers and constructor code */
}

nsCmdLineHandler::~nsCmdLineHandler()
{
  /* destructor code */
}

/* readonly attribute string commandLineArgument; */
NS_IMETHODIMP nsCmdLineHandler::GetCommandLineArgument(char * *aCommandLineArgument)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string prefNameForStartup; */
NS_IMETHODIMP nsCmdLineHandler::GetPrefNameForStartup(char * *aPrefNameForStartup)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string chromeUrlForTask; */
NS_IMETHODIMP nsCmdLineHandler::GetChromeUrlForTask(char * *aChromeUrlForTask)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute string helpText; */
NS_IMETHODIMP nsCmdLineHandler::GetHelpText(char * *aHelpText)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean handlesArgs; */
NS_IMETHODIMP nsCmdLineHandler::GetHandlesArgs(PRBool *aHandlesArgs)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute wstring defaultArgs; */
NS_IMETHODIMP nsCmdLineHandler::GetDefaultArgs(PRUnichar * *aDefaultArgs)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean openWindowWithArgs; */
NS_IMETHODIMP nsCmdLineHandler::GetOpenWindowWithArgs(PRBool *aOpenWindowWithArgs)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsICmdLineHandler_h__ */
