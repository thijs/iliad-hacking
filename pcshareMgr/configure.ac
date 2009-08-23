#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(pcshareMgr, 0.212)
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS(config.h)

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_STDC 
AC_PROG_RANLIB

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# include DMALLOC support
AM_WITH_DMALLOC

#Check GTK 
PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

#g_thread_init is in libgthread
PKG_CHECK_MODULES(GTHREAD, [gthread-2.0])
AC_SUBST(GTHREAD_CFLAGS)
AC_SUBST(GTHREAD_LIBS)

PKG_CHECK_MODULES(LIBERIPC, liberipc >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader IPC Library (liberipc) 0.1 not installed ***]))
AC_SUBST(LIBERIPC_CFLAGS)
AC_SUBST(LIBERIPC_LIBS)

PKG_CHECK_MODULES(LIBERREGXML, liberregxml >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader Registry Library (liberreg) 0.1 not installed ***]))
AC_SUBST(LIBERREGXML_CFLAGS)
AC_SUBST(LIBERREGXML_LIBS)

PKG_CHECK_MODULES(LIBERDM, liberdm >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader display manager Library (liberdm) 0.1 not installed ***]))
AC_SUBST(LIBERDM_CFLAGS)
AC_SUBST(LIBERDM_LIBS)

PKG_CHECK_MODULES(LIBERGTK, libergtk >= 0.1,,
                            AC_MSG_ERROR([*** Required EReader GTK Library (libergtk) 0.1 not installed ***]))
AC_SUBST(LIBERGTK_CFLAGS)
AC_SUBST(LIBERGTK_LIBS)


#language support
##AC_CONFIG_HEADERS(config.h)
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.16.1])
localedir='${datadir}/locale'
AC_SUBST(localedir)

# use pkgconfig for libxml 
PKG_CHECK_MODULES(xml2, libxml-2.0 >= 2.6)
AC_SUBST(xml2_CFLAGS)
AC_SUBST(xml2_LDADD)

AC_CONFIG_FILES([ Makefile \
                  src/Makefile \
				  po/Makefile.in \
                  data/Makefile \
                  inc/Makefile ])
AC_OUTPUT
