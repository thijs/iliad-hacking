
source_c =  ermanifest.c 

DATADIR = $(localstatedir)

AM_CFLAGS = @GCC_WARNINGS@ -DDATA_DIR=\"$(pkgdatadir)\" -I$(top_srcdir)/include

INCLUDES = @LIBXML_CFLAGS@

lib_LTLIBRARIES          = libermanifest.la
libermanifest_la_SOURCES = $(source_c)
libermanifest_la_LIBADD  = @LIBXML_LDADD@

# version number = API versions : revision number : backwards compatible info
libermanifest_la_LDFLAGS = -version-info ${libermanifest_la_version_info}

libergtkheadersdir = $(includedir)/libergtk
libergtkheaders_HEADERS = $(source_h)

AM_CPPFLAGS = $(DEPS_CFLAGS)
AM_LDFLAGS = $(DEPS_LIBS)

clean-local:
	/bin/rm *.bb *.bbg *.da *.gcov .libs/*.da .libs/*.bbg || true
