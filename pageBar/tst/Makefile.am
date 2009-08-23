#the name of the binary program to be created, 
noinst_PROGRAMS = pagebarTest 

pagebarTest_SOURCES = pagebarTest.c 

#AM_CPPFLAGS = -I$(top_srcdir)/src 
AM_CPPFLAGS = $(DEPS_CFLAGS) $(LIBERMDS_CFLAGS) $(LIBERREG_CFLAGS) $(LIBERIPC_CFLAGS) $(LIBERDM_CFLAGS) $(LIBMB_CFLAGS) 

AM_CFLAGS = -g -Wall -Werror -DDATADIR=\"$(localstatedir)\"
# AM_LDFLAGS = @glib_LIBS@
AM_LDFLAGS = $(DEPS_LIBS) $(LIBERMDS_LIBS) $(LIBERIPC_LIBS) $(LIBERDM_LIBS) $(LIBERREG_LIBS)
