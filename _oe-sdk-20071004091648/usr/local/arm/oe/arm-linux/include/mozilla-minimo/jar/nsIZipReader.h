/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM /data/workareas/matthijs/svn/openembedded/build/tmp/work/minimo-0.0cvs20051025-r9/mozilla/modules/libjar/nsIZipReader.idl
 */

#ifndef __gen_nsIZipReader_h__
#define __gen_nsIZipReader_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class nsISimpleEnumerator; /* forward declaration */

class nsIInputStream; /* forward declaration */

class nsIFile; /* forward declaration */


/* starting interface:    nsIZipEntry */
#define NS_IZIPENTRY_IID_STR "6ca5e43e-9632-11d3-8cd9-0060b0fc14a3"

#define NS_IZIPENTRY_IID \
  {0x6ca5e43e, 0x9632, 0x11d3, \
    { 0x8c, 0xd9, 0x00, 0x60, 0xb0, 0xfc, 0x14, 0xa3 }}

class NS_NO_VTABLE nsIZipEntry : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IZIPENTRY_IID)

  /* readonly attribute string name; */
  NS_IMETHOD GetName(char * *aName) = 0;

  /* readonly attribute unsigned short compression; */
  NS_IMETHOD GetCompression(PRUint16 *aCompression) = 0;

  /* readonly attribute unsigned long size; */
  NS_IMETHOD GetSize(PRUint32 *aSize) = 0;

  /* readonly attribute unsigned long realSize; */
  NS_IMETHOD GetRealSize(PRUint32 *aRealSize) = 0;

  /* readonly attribute unsigned long CRC32; */
  NS_IMETHOD GetCRC32(PRUint32 *aCRC32) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIZIPENTRY \
  NS_IMETHOD GetName(char * *aName); \
  NS_IMETHOD GetCompression(PRUint16 *aCompression); \
  NS_IMETHOD GetSize(PRUint32 *aSize); \
  NS_IMETHOD GetRealSize(PRUint32 *aRealSize); \
  NS_IMETHOD GetCRC32(PRUint32 *aCRC32); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIZIPENTRY(_to) \
  NS_IMETHOD GetName(char * *aName) { return _to GetName(aName); } \
  NS_IMETHOD GetCompression(PRUint16 *aCompression) { return _to GetCompression(aCompression); } \
  NS_IMETHOD GetSize(PRUint32 *aSize) { return _to GetSize(aSize); } \
  NS_IMETHOD GetRealSize(PRUint32 *aRealSize) { return _to GetRealSize(aRealSize); } \
  NS_IMETHOD GetCRC32(PRUint32 *aCRC32) { return _to GetCRC32(aCRC32); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIZIPENTRY(_to) \
  NS_IMETHOD GetName(char * *aName) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetName(aName); } \
  NS_IMETHOD GetCompression(PRUint16 *aCompression) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCompression(aCompression); } \
  NS_IMETHOD GetSize(PRUint32 *aSize) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSize(aSize); } \
  NS_IMETHOD GetRealSize(PRUint32 *aRealSize) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetRealSize(aRealSize); } \
  NS_IMETHOD GetCRC32(PRUint32 *aCRC32) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCRC32(aCRC32); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsZipEntry : public nsIZipEntry
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIZIPENTRY

  nsZipEntry();

private:
  ~nsZipEntry();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsZipEntry, nsIZipEntry)

nsZipEntry::nsZipEntry()
{
  /* member initializers and constructor code */
}

nsZipEntry::~nsZipEntry()
{
  /* destructor code */
}

/* readonly attribute string name; */
NS_IMETHODIMP nsZipEntry::GetName(char * *aName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short compression; */
NS_IMETHODIMP nsZipEntry::GetCompression(PRUint16 *aCompression)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long size; */
NS_IMETHODIMP nsZipEntry::GetSize(PRUint32 *aSize)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long realSize; */
NS_IMETHODIMP nsZipEntry::GetRealSize(PRUint32 *aRealSize)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long CRC32; */
NS_IMETHODIMP nsZipEntry::GetCRC32(PRUint32 *aCRC32)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIZipReader */
#define NS_IZIPREADER_IID_STR "6ff6a966-9632-11d3-8cd9-0060b0fc14a3"

#define NS_IZIPREADER_IID \
  {0x6ff6a966, 0x9632, 0x11d3, \
    { 0x8c, 0xd9, 0x00, 0x60, 0xb0, 0xfc, 0x14, 0xa3 }}

class NS_NO_VTABLE nsIZipReader : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IZIPREADER_IID)

  /**
     * Initializes a zip reader after construction.
     */
  /* void init (in nsIFile zipFile); */
  NS_IMETHOD Init(nsIFile *zipFile) = 0;

  /* readonly attribute nsIFile file; */
  NS_IMETHOD GetFile(nsIFile * *aFile) = 0;

  /**
     * Opens a zip reader.
     */
  /* void open (); */
  NS_IMETHOD Open(void) = 0;

  /**
     * Closes a zip reader. Subsequent attempts to extract files or read from
     * its input stream will result in an error.
     */
  /* void close (); */
  NS_IMETHOD Close(void) = 0;

  /**
     * Tests the integrity of the archive by performing a CRC check 
     * on each item expanded into memory.  If an entry is specified
     * the integrity of only that item is tested.  If NULL is passed 
     * in the inetgrity of all items in the archive are tested.  
     */
  /* void test (in string aEntryName); */
  NS_IMETHOD Test(const char *aEntryName) = 0;

  /**
     * Extracts a zip entry into a local file specified by outFile.
     */
  /* void extract (in string zipEntry, in nsIFile outFile); */
  NS_IMETHOD Extract(const char *zipEntry, nsIFile *outFile) = 0;

  /**
     * Returns a nsIZipEntry describing a specified zip entry.
     */
  /* nsIZipEntry getEntry (in string zipEntry); */
  NS_IMETHOD GetEntry(const char *zipEntry, nsIZipEntry **_retval) = 0;

  /**
     * Returns a simple enumerator whose elements are of type nsIZipEntry.
     */
  /* nsISimpleEnumerator findEntries (in string aPattern); */
  NS_IMETHOD FindEntries(const char *aPattern, nsISimpleEnumerator **_retval) = 0;

  /**
     * Returns an input stream containing the contents of the specified zip entry.
     */
  /* nsIInputStream getInputStream (in string zipEntry); */
  NS_IMETHOD GetInputStream(const char *zipEntry, nsIInputStream **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIZIPREADER \
  NS_IMETHOD Init(nsIFile *zipFile); \
  NS_IMETHOD GetFile(nsIFile * *aFile); \
  NS_IMETHOD Open(void); \
  NS_IMETHOD Close(void); \
  NS_IMETHOD Test(const char *aEntryName); \
  NS_IMETHOD Extract(const char *zipEntry, nsIFile *outFile); \
  NS_IMETHOD GetEntry(const char *zipEntry, nsIZipEntry **_retval); \
  NS_IMETHOD FindEntries(const char *aPattern, nsISimpleEnumerator **_retval); \
  NS_IMETHOD GetInputStream(const char *zipEntry, nsIInputStream **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIZIPREADER(_to) \
  NS_IMETHOD Init(nsIFile *zipFile) { return _to Init(zipFile); } \
  NS_IMETHOD GetFile(nsIFile * *aFile) { return _to GetFile(aFile); } \
  NS_IMETHOD Open(void) { return _to Open(); } \
  NS_IMETHOD Close(void) { return _to Close(); } \
  NS_IMETHOD Test(const char *aEntryName) { return _to Test(aEntryName); } \
  NS_IMETHOD Extract(const char *zipEntry, nsIFile *outFile) { return _to Extract(zipEntry, outFile); } \
  NS_IMETHOD GetEntry(const char *zipEntry, nsIZipEntry **_retval) { return _to GetEntry(zipEntry, _retval); } \
  NS_IMETHOD FindEntries(const char *aPattern, nsISimpleEnumerator **_retval) { return _to FindEntries(aPattern, _retval); } \
  NS_IMETHOD GetInputStream(const char *zipEntry, nsIInputStream **_retval) { return _to GetInputStream(zipEntry, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIZIPREADER(_to) \
  NS_IMETHOD Init(nsIFile *zipFile) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(zipFile); } \
  NS_IMETHOD GetFile(nsIFile * *aFile) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetFile(aFile); } \
  NS_IMETHOD Open(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Open(); } \
  NS_IMETHOD Close(void) { return !_to ? NS_ERROR_NULL_POINTER : _to->Close(); } \
  NS_IMETHOD Test(const char *aEntryName) { return !_to ? NS_ERROR_NULL_POINTER : _to->Test(aEntryName); } \
  NS_IMETHOD Extract(const char *zipEntry, nsIFile *outFile) { return !_to ? NS_ERROR_NULL_POINTER : _to->Extract(zipEntry, outFile); } \
  NS_IMETHOD GetEntry(const char *zipEntry, nsIZipEntry **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetEntry(zipEntry, _retval); } \
  NS_IMETHOD FindEntries(const char *aPattern, nsISimpleEnumerator **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->FindEntries(aPattern, _retval); } \
  NS_IMETHOD GetInputStream(const char *zipEntry, nsIInputStream **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetInputStream(zipEntry, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsZipReader : public nsIZipReader
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIZIPREADER

  nsZipReader();

private:
  ~nsZipReader();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsZipReader, nsIZipReader)

nsZipReader::nsZipReader()
{
  /* member initializers and constructor code */
}

nsZipReader::~nsZipReader()
{
  /* destructor code */
}

/* void init (in nsIFile zipFile); */
NS_IMETHODIMP nsZipReader::Init(nsIFile *zipFile)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIFile file; */
NS_IMETHODIMP nsZipReader::GetFile(nsIFile * *aFile)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void open (); */
NS_IMETHODIMP nsZipReader::Open()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void close (); */
NS_IMETHODIMP nsZipReader::Close()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void test (in string aEntryName); */
NS_IMETHODIMP nsZipReader::Test(const char *aEntryName)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void extract (in string zipEntry, in nsIFile outFile); */
NS_IMETHODIMP nsZipReader::Extract(const char *zipEntry, nsIFile *outFile)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIZipEntry getEntry (in string zipEntry); */
NS_IMETHODIMP nsZipReader::GetEntry(const char *zipEntry, nsIZipEntry **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsISimpleEnumerator findEntries (in string aPattern); */
NS_IMETHODIMP nsZipReader::FindEntries(const char *aPattern, nsISimpleEnumerator **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIInputStream getInputStream (in string zipEntry); */
NS_IMETHODIMP nsZipReader::GetInputStream(const char *zipEntry, nsIInputStream **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    nsIZipReaderCache */
#define NS_IZIPREADERCACHE_IID_STR "52c45d86-0cc3-11d4-986e-00c04fa0cf4a"

#define NS_IZIPREADERCACHE_IID \
  {0x52c45d86, 0x0cc3, 0x11d4, \
    { 0x98, 0x6e, 0x00, 0xc0, 0x4f, 0xa0, 0xcf, 0x4a }}

class NS_NO_VTABLE nsIZipReaderCache : public nsISupports {
 public: 

  NS_DEFINE_STATIC_IID_ACCESSOR(NS_IZIPREADERCACHE_IID)

  /**
     * Initializes a new zip reader cache. 
     * @param cacheSize - the number of released entries to maintain before
     *   beginning to throw some out (note that the number of outstanding
     *   entries can be much greater than this number -- this is the count
     *   for those otherwise unused entries)
     */
  /* void init (in unsigned long cacheSize); */
  NS_IMETHOD Init(PRUint32 cacheSize) = 0;

  /**
     * Returns a (possibly shared) nsIZipReader for an nsIFile.
     */
  /* nsIZipReader getZip (in nsIFile zipFile); */
  NS_IMETHOD GetZip(nsIFile *zipFile, nsIZipReader **_retval) = 0;

};

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_NSIZIPREADERCACHE \
  NS_IMETHOD Init(PRUint32 cacheSize); \
  NS_IMETHOD GetZip(nsIFile *zipFile, nsIZipReader **_retval); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_NSIZIPREADERCACHE(_to) \
  NS_IMETHOD Init(PRUint32 cacheSize) { return _to Init(cacheSize); } \
  NS_IMETHOD GetZip(nsIFile *zipFile, nsIZipReader **_retval) { return _to GetZip(zipFile, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_NSIZIPREADERCACHE(_to) \
  NS_IMETHOD Init(PRUint32 cacheSize) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(cacheSize); } \
  NS_IMETHOD GetZip(nsIFile *zipFile, nsIZipReader **_retval) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetZip(zipFile, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class nsZipReaderCache : public nsIZipReaderCache
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIZIPREADERCACHE

  nsZipReaderCache();

private:
  ~nsZipReaderCache();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(nsZipReaderCache, nsIZipReaderCache)

nsZipReaderCache::nsZipReaderCache()
{
  /* member initializers and constructor code */
}

nsZipReaderCache::~nsZipReaderCache()
{
  /* destructor code */
}

/* void init (in unsigned long cacheSize); */
NS_IMETHODIMP nsZipReaderCache::Init(PRUint32 cacheSize)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIZipReader getZip (in nsIFile zipFile); */
NS_IMETHODIMP nsZipReaderCache::GetZip(nsIFile *zipFile, nsIZipReader **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif

#define NS_ZIPREADER_CID                             \
{ /* 7526a738-9632-11d3-8cd9-0060b0fc14a3 */         \
    0x7526a738,                                      \
    0x9632,                                          \
    0x11d3,                                          \
    {0x8c, 0xd9, 0x00, 0x60, 0xb0, 0xfc, 0x14, 0xa3} \
}
#define NS_ZIPREADERCACHE_CID                        \
{ /* 1b117e16-0cad-11d4-986e-00c04fa0cf4a */         \
    0x1b117e16,                                      \
    0x0cad,                                          \
    0x11d4,                                          \
    {0x98, 0x6e, 0x00, 0xc0, 0x4f, 0xa0, 0xcf, 0x4a} \
}

#endif /* __gen_nsIZipReader_h__ */
