
source_c =  eripc.c			\
	    eripcarinc.c		\
	    eripcbusyd.c		\
	    eripcclient.c		\
	    eripcconnect.c		\
	    eripccontentlister.c	\
	    eripceba.c			\
	    eripcpagebar.c		\
	    eripcparser.c		\
	    eripcports.c		\
	    eripcpowermgr.c		\
	    eripcsetup.c		\
	    eripctoolbar.c		\
	    eripcviewer.c

DATADIR=$(localstatedir)

AM_CFLAGS = @GCC_WARNINGS@ -DDATADIR=\"$(localstatedir)\" -I$(top_srcdir)/include

lib_LTLIBRARIES     = liberipc.la
liberipc_la_SOURCES = $(source_c)

# version number = API versions : revision number : backwards compatible info
liberipc_la_LDFLAGS = -version-info ${liberipc_la_version_info}

liberipcheadersdir = $(includedir)/liberipc
liberipcheaders_HEADERS = $(source_h)

AM_CPPFLAGS = $(DEPS_CFLAGS)
AM_LDFLAGS = $(DEPS_LIBS)

clean-local:
	/bin/rm *.bb *.bbg *.da *.gcov .libs/*.da .libs/*.bbg || true
