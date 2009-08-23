noinst_PROGRAMS = regxmlTest testLock

regxmlTest_SOURCES = main.c
testLock_SOURCES = testlock.c

DATADIR=$(localstatedir)

INCLUDES = @glib_CFLAGS@ @LIBXML_CFLAGS@ $(LIBERMANIFEST_CFLAGS) -I$(top_srcdir)/inc
AM_CFLAGS = -g -Wall -Werror -DDATADIR=\"$(localstatedir)\" 
if WANT_PC_ENVIRONMENT
AM_CFLAGS += -DPC_ENVIRONMENT
endif
AM_LDFLAGS = @glib_LIBS@ $(LIBERREGXML_LIBS)	../src/liberregxml.la
