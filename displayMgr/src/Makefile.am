#the name of the binary program to be created, 

bin_PROGRAMS = displayMgr displayMgrClient blank

displayMgr_SOURCES = displayMgr.c displayEinkTypes.h
displayMgr_LDADD = $(LIBERDM_LIBS) $(LIBERIPC_LIBS)

displayMgrClient_SOURCES = displayMgrClient.c
displayMgrClient_LDADD = $(LIBERDM_LIBS) $(LIBERIPC_LIBS)

blank_SOURCES = blank.c

AM_CPPFLAGS = $(LIBERDM_CFLAGS) $(LIBERIPC_CFLAGS) -DDATA_DIR=\""$(datadir)/data"\"

# INCLUDES= $(LIBERDM_CFLAGS)
AM_CFLAGS = -Wall -Werror
