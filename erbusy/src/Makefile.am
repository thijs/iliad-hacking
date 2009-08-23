bin_PROGRAMS = erbusyd erbusy

erbusyd_SOURCES = erbusyd.c
erbusy_SOURCES = erbusy.c

AM_CPPFLAGS = $(LIBERIPC_CFLAGS) -DDATA_DIR=\"$(pkgdatadir)\"
AM_LDFLAGS = $(LIBERIPC_LIBS) -lpthread

AM_CFLAGS = -Wall -Werror

