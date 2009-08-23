			
source_c =  scbdoc.c         \
            scbpages.c       \
            scbpage.c        \
            scbstrokes.c     \
            scbstroke.c      \
            scbpoints.c      \
            scbpoint.c       \
            scbxml.c         \
            driver.c         \
            scbcolor.c       \
            scbpath.c        \
            scb.c            \
            scbutil.c       
            		
DATADIR=$(datadir)

AM_CFLAGS = @GCC_WARNINGS@ -DDATADIR=\"$(datadir)\" -I$(top_srcdir)/include

INCLUDES = @LIBXML_CFLAGS@ $(DEPS_CFLAGS) $(LIBERMANIFEST_CFLAGS) $(LIBERDM_CFLAGS)

lib_LTLIBRARIES          = liberscribble.la
liberscribble_la_SOURCES = $(source_c)
liberscribble_la_LIBADD  = @LIBXML_LDADD@ $(DEPS_LIBS) $(LIBERMANIFEST_LIBS) $(LIBERDM_LIBS)

# version number = API versions : revision number : backwards compatible info
liberscribble_la_LDFLAGS = -version-info ${liberscribble_la_version_info}

AM_CPPFLAGS = $(DEPS_CFLAGS)
AM_LDFLAGS = $(DEPS_LIBS)

clean-local:
	/bin/rm *.bb *.bbg *.da *.gcov .libs/*.da .libs/*.bbg || true
