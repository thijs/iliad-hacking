
bin_PROGRAMS = pcshareMgr

pcshareMgr_SOURCES = main.c \
			control.c \
			displayUpdate.c  \
			systemcalls.c  \
			sharethread.c \
			view.c \
			settings.c \
			languages.c

AM_CPPFLAGS = $(GTHREAD_CFLAGS) $(GTK_CFLAGS) $(LIBERIPC_CFLAGS) $(LIBERDM_CFLAGS) $(LIBERREGXML_CFLAGS) \
		-DLOCALE_DIR=\"$(localedir)\" \
		-DDATA_DIR=\"$(pkgdatadir)\"

AM_LDFLAGS = $(GTHREAD_LIBS) $(GTK_LIBS) $(LIBERIPC_LIBS) $(LIBERDM_LIBS) $(LIBERREGXML_LIBS) 

INCLUDES=-I$(top_srcdir)/inc

AM_CFLAGS = -Wall -Werror

