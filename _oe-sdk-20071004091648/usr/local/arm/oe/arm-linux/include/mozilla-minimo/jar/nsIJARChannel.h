/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/modules/libjar/nsIJARChannel.idl
 */

#ifndef __gen_nsIJARChannel_h__
#define __gen_nsIJARChannel_h__


#ifndef __gen_nsIChannel_h__
#include "nsIChannel.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    nsIJARChannel */
#define NS_IJARCHANNEL_IID_STR "c7e410d1-85f2-11d3-9f63-006008a6efe9"

#define NS_IJARCHANNEL_IID \
  {0xc7e410d1, 0x85f2, 0x11d3, \
    { 0x9f, 0x63, 0x00, 0x60, 0x08, 0xa6, 0xef, 0xe9 }}

class NS_NO_VTABLE nsIJARChannel : public nsIChannel {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IJARCHANNEL_IID)

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIJARCHANNEL \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIJARCHANNEL(_to) \
  /* no methods! */

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIJARCHANNEL(_to) \
  /* no methods! */

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsJARChannel : public nsIJARChannel
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIJARCHANNEL

  nsJARChannel();

private:
  ~nsJARChannel();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsJARChannel, nsIJARChannel)

nsJARChannel::nsJARChannel()
{
  /* member initializers and constructor code */
}

nsJARChannel::~nsJARChannel()
{
  /* destructor code */
}

/* End of implementation class template. */
#endif


#endif /* __gen_nsIJARChannel_h__ */
