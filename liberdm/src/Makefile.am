
source_c = erdm.c		\
	    erdminternal.c	\
	    erdmServer.c

DATADIR=$(localstatedir)

AM_CFLAGS = -Wall -Werror -DDATADIR=\"$(localstatedir)\" -I$(top_srcdir)/include

lib_LTLIBRARIES    = liberdm.la
liberdm_la_SOURCES = $(source_c)

# -version-info : <api-version>:<Revision-of-implementation>:<binary compat upto 'n' versions>
liberdm_la_LDFLAGS = -version-info ${liberdm_la_version_info}

liberdmheadersdir = $(includedir)/liberdm
liberdmheaders_HEADERS = $(source_h)

AM_CPPFLAGS = $(DEPS_CFLAGS)
AM_LDFLAGS = $(DEPS_LIBS)

