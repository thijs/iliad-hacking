#the name of the binary program to be created, 
#bin_PROGRAMS = contentLister tstProgramManager
bin_PROGRAMS = pageBar

pageBar_SOURCES = main.c gtkPagebar.c gtkPagebar.h displayUpdate.h displayUpdate.c erbusy.c erbusy.h

# The DEPS_FLAGS comes from a variable that is set up in configure.ac file

AM_CPPFLAGS = -Wall -Werror $(DEPS_CFLAGS) $(LIBERMDS_CFLAGS) $(LIBERIPC_CFLAGS) $(LIBERDM_CFLAGS) $(LIBMB_CFLAGS) -DDATA_DIR=\"$(pkgdatadir)\"

# the DEPS_LIBS variable, is set in configure.ac.
AM_LDFLAGS = $(DEPS_LIBS) $(LIBERMDS_LIBS) $(LIBERIPC_LIBS) $(LIBERDM_LIBS)

#INCLUDES=-I$(top_srcdir)/inc 

