#the name of the binary program to be created, 
bin_PROGRAMS = powerMgr clNotify
noinst_PROGRAMS = powerMgrtest

powerMgr_SOURCES = powerMgr.c powerMgr.h toolbar.c toolbar.h

powerMgrtest_SOURCES = powerMgrtest.c

clNotify_SOURCES = notify.c

# The DEPS_FLAGS comes from a variable that is set up in configure.ac file

AM_CPPFLAGS = -Wall -Werror $(DEPS_CFLAGS) $(LIBERIPC_CFLAGS)  

# the DEPS_LIBS variable, is set in configure.ac.
# contentLister_LDADD = $(DEPS_LIBS) 
# 	Note: don't use $(LIBMB_LIBS) it's incompatible with Ann's PC.
# 	AM_LDFLAGS = $(LIBMB_LIBS) $(DEPS_LIBS) -lermds -lgtkPagebar -lgthread-2.0
AM_LDFLAGS = $(DEPS_LIBS) $(LIBERIPC_LIBS) $(LIBERDM_LIBS) 
