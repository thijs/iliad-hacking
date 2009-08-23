/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/modules/libpr0n/public/imgIDecoderObserver.idl
 */

#ifndef __gen_imgIDecoderObserver_h__
#define __gen_imgIDecoderObserver_h__


#ifndef __gen_imgIContainerObserver_h__
#include "imgIContainerObserver.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class imgIRequest; /* forward declaration */

class imgIContainer; /* forward declaration */

class gfxIImageFrame; /* forward declaration */

#include "nsRect.h"

/* starting interface:    imgIDecoderObserver */
#define IMGIDECODEROBSERVER_IID_STR "cce7152e-4395-4231-a781-c347c5446cc2"

#define IMGIDECODEROBSERVER_IID \
  {0xcce7152e, 0x4395, 0x4231, \
    { 0xa7, 0x81, 0xc3, 0x47, 0xc5, 0x44, 0x6c, 0xc2 }}

/**
 * imgIDecoderObserver interface
 *
 * @author Stuart Parmenter <pavlov@netscape.com>
 * @version 0.1
 * @see imagelib2
 */
class NS_NO_VTABLE imgIDecoderObserver : public imgIContainerObserver {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(IMGIDECODEROBSERVER_IID)

  /**
   * called as soon as the image begins getting decoded
   */
  /* void onStartDecode (in imgIRequest aRequest); */
  NS_IMETHOD OnStartDecode(imgIRequest *aRequest) = 0;

  /**
   * called once the image has been inited and therefore has a width and height
   */
  /* void onStartContainer (in imgIRequest aRequest, in imgIContainer aContainer); */
  NS_IMETHOD OnStartContainer(imgIRequest *aRequest, imgIContainer *aContainer) = 0;

  /**
   * called when each frame is created
   */
  /* void onStartFrame (in imgIRequest aRequest, in gfxIImageFrame aFrame); */
  NS_IMETHOD OnStartFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame) = 0;

  /**
   * called when some part of the frame has new data in it
   */
  /* [noscript] void onDataAvailable (in imgIRequest aRequest, in gfxIImageFrame aFrame, [const] in nsIntRect aRect); */
  NS_IMETHOD OnDataAvailable(imgIRequest *aRequest, gfxIImageFrame *aFrame, const nsIntRect * aRect) = 0;

  /**
   * called when a frame is finished decoding
   */
  /* void onStopFrame (in imgIRequest aRequest, in gfxIImageFrame aFrame); */
  NS_IMETHOD OnStopFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame) = 0;

  /**
   * probably not needed.  called right before onStopDecode
   */
  /* void onStopContainer (in imgIRequest aRequest, in imgIContainer aContainer); */
  NS_IMETHOD OnStopContainer(imgIRequest *aRequest, imgIContainer *aContainer) = 0;

  /**
   * called when the decoder is dying off
   */
  /* void onStopDecode (in imgIRequest aRequest, in nsresult status, in wstring statusArg); */
  NS_IMETHOD OnStopDecode(imgIRequest *aRequest, nsresult status, const PRUnichar *statusArg) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IMGIDECODEROBSERVER \
  NS_IMETHOD OnStartDecode(imgIRequest *aRequest); \
  NS_IMETHOD OnStartContainer(imgIRequest *aRequest, imgIContainer *aContainer); \
  NS_IMETHOD OnStartFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame); \
  NS_IMETHOD OnDataAvailable(imgIRequest *aRequest, gfxIImageFrame *aFrame, const nsIntRect * aRect); \
  NS_IMETHOD OnStopFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame); \
  NS_IMETHOD OnStopContainer(imgIRequest *aRequest, imgIContainer *aContainer); \
  NS_IMETHOD OnStopDecode(imgIRequest *aRequest, nsresult status, const PRUnichar *statusArg); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IMGIDECODEROBSERVER(_to) \
  NS_IMETHOD OnStartDecode(imgIRequest *aRequest) { return _to OnStartDecode(aRequest); } \
  NS_IMETHOD OnStartContainer(imgIRequest *aRequest, imgIContainer *aContainer) { return _to OnStartContainer(aRequest, aContainer); } \
  NS_IMETHOD OnStartFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame) { return _to OnStartFrame(aRequest, aFrame); } \
  NS_IMETHOD OnDataAvailable(imgIRequest *aRequest, gfxIImageFrame *aFrame, const nsIntRect * aRect) { return _to OnDataAvailable(aRequest, aFrame, aRect); } \
  NS_IMETHOD OnStopFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame) { return _to OnStopFrame(aRequest, aFrame); } \
  NS_IMETHOD OnStopContainer(imgIRequest *aRequest, imgIContainer *aContainer) { return _to OnStopContainer(aRequest, aContainer); } \
  NS_IMETHOD OnStopDecode(imgIRequest *aRequest, nsresult status, const PRUnichar *statusArg) { return _to OnStopDecode(aRequest, status, statusArg); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IMGIDECODEROBSERVER(_to) \
  NS_IMETHOD OnStartDecode(imgIRequest *aRequest) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStartDecode(aRequest); } \
  NS_IMETHOD OnStartContainer(imgIRequest *aRequest, imgIContainer *aContainer) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStartContainer(aRequest, aContainer); } \
  NS_IMETHOD OnStartFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStartFrame(aRequest, aFrame); } \
  NS_IMETHOD OnDataAvailable(imgIRequest *aRequest, gfxIImageFrame *aFrame, const nsIntRect * aRect) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnDataAvailable(aRequest, aFrame, aRect); } \
  NS_IMETHOD OnStopFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStopFrame(aRequest, aFrame); } \
  NS_IMETHOD OnStopContainer(imgIRequest *aRequest, imgIContainer *aContainer) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStopContainer(aRequest, aContainer); } \
  NS_IMETHOD OnStopDecode(imgIRequest *aRequest, nsresult status, const PRUnichar *statusArg) { return !_to ? NS_ERROR_NULL_POINTER : _to->OnStopDecode(aRequest, status, statusArg); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public imgIDecoderObserver
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IMGIDECODEROBSERVER

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, imgIDecoderObserver)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* void onStartDecode (in imgIRequest aRequest); */
NS_IMETHODIMP _MYCLASS_::OnStartDecode(imgIRequest *aRequest)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onStartContainer (in imgIRequest aRequest, in imgIContainer aContainer); */
NS_IMETHODIMP _MYCLASS_::OnStartContainer(imgIRequest *aRequest, imgIContainer *aContainer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onStartFrame (in imgIRequest aRequest, in gfxIImageFrame aFrame); */
NS_IMETHODIMP _MYCLASS_::OnStartFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* [noscript] void onDataAvailable (in imgIRequest aRequest, in gfxIImageFrame aFrame, [const] in nsIntRect aRect); */
NS_IMETHODIMP _MYCLASS_::OnDataAvailable(imgIRequest *aRequest, gfxIImageFrame *aFrame, const nsIntRect * aRect)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onStopFrame (in imgIRequest aRequest, in gfxIImageFrame aFrame); */
NS_IMETHODIMP _MYCLASS_::OnStopFrame(imgIRequest *aRequest, gfxIImageFrame *aFrame)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onStopContainer (in imgIRequest aRequest, in imgIContainer aContainer); */
NS_IMETHODIMP _MYCLASS_::OnStopContainer(imgIRequest *aRequest, imgIContainer *aContainer)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void onStopDecode (in imgIRequest aRequest, in nsresult status, in wstring statusArg); */
NS_IMETHODIMP _MYCLASS_::OnStopDecode(imgIRequest *aRequest, nsresult status, const PRUnichar *statusArg)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_imgIDecoderObserver_h__ */
