
source_c = erGtkEntry.c             \
           erGtkSelectionGroup.c    \
           erGtkSelectionList.c     \
           erGtkToggleButton.c      \
           erGtkEntryFilter.c             

DATADIR = $(localstatedir)

AM_CFLAGS = -Wall @GCC_WARNINGS@ -DDATADIR=\"$(localstatedir)\" -I$(top_srcdir)/include

INCLUDES = @glib_CFLAGS@

lib_LTLIBRARIES     = libergtk.la
libergtk_la_SOURCES = $(source_c)
libergtk_la_LIBADD  = @glib_LIBS@

# version number = API versions : revision number : backwards compatible info
libergtk_la_LDFLAGS = -version-info ${libergtk_la_version_info}

libergtkheadersdir = $(includedir)/libergtk
libergtkheaders_HEADERS = $(source_h)

AM_CPPFLAGS = $(DEPS_CFLAGS)
AM_LDFLAGS = $(DEPS_LIBS)

