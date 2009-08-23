source_c = erregtypes.c        \
		   erregxml.c          \
		   erregxsd.c          \
		   erregbasis.c        \
		   erregtoolbar.c      \
		   erregnwprofiles.c   \
		   erreg.c             \
		   erregcfg.c          \
		   erregrwlock.c       \
		   system.c        

DATADIR=$(localstatedir)
AM_CFLAGS = @GCC_WARNINGS@ -DDATADIR=\"$(localstatedir)\"
if WANT_PC_ENVIRONMENT
AM_CFLAGS += -DPC_ENVIRONMENT
endif

INCLUDES = @glib_CFLAGS@ @LIBXML_CFLAGS@ $(DEPS_CFLAGS) $(LIBERMANIFEST_CFLAGS) -I$(top_srcdir)/inc 

lib_LTLIBRARIES      = liberregxml.la
liberregxml_la_SOURCES     = $(source_c)
#liberregxml_la_LIBADD      = @LIBXML_LDADD@ $(DEPS_LIBS) $(LIBERMANIFEST_LIBS)
liberregxml_la_LIBADD      = @glib_LIBS@ @LIBXML_LDADD@ $(LIBERMANIFEST_LIBS)

# http://sources.redhat.com/autobook/autobook/autobook_91.html#SEC91
# current : revision : age

# version number = API versions : revision number : backwards compatible info
# so compared to the previous value 0:1:0 of this lib :
#    - new API versions (the API was extended)
#    - new revision number for this API version (best start with zero)
#    - one API verion backward compatible
#libregxml_la_LDFLAGS = -version-info 0:1:0
#liberregxml_la_LDFLAGS = -version-info ${liberregxml_la_version_info}

clean-local:
	/bin/rm *.bb *.bbg *.da *.gcov .libs/*.da .libs/*.bbg || true
