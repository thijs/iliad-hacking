#ifndef SCBXML_H_
#define SCBXML_H_

// xml file support
// internal use
#include <libermanifest/ermanifest.h>
#include "scbconfig.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _ScbXml
{
    erManifest  handle;
    char        xPath[SCB_MAX_XML_PATH];
    int         index;
}ScbXml;
typedef ScbXml * ScbXmlPtr;

// shallow copy
ScbXmlPtr scb_xml_clone(const ScbXmlPtr src);
void scb_xml_free(ScbXmlPtr ptr);

#ifdef __cplusplus
}
#endif

#endif
