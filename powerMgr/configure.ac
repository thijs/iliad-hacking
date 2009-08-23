dnl ----- Process this file with autoconf to produce a configure script -----

AC_INIT(powerMgr, 0.212)
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR(src/powerMgr.c)
AC_PREREQ(2.57)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

dnl ----- Checks for programs ---------------------------------------------
AC_PROG_CC
AC_STDC_HEADERS
AC_PROG_INSTALL

dnl ------------------------------------------------------------------------

PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2 gthread-2.0)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

PKG_CHECK_MODULES(LIBERIPC, liberipc >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader IPC Library (liberipc) 0.1 not installed ***]))
AC_SUBST(LIBERIPC_CFLAGS)
AC_SUBST(LIBERIPC_LIBS)

dnl PKG_CHECK_MODULES(LIBERREG, liberreg >= 0.1,, 
dnl 			    AC_MSG_ERROR([*** Required EReader Registry Library (liberreg) 0.1 not installed ***]))
dnl AC_SUBST(LIBERREG_CFLAGS)
dnl AC_SUBST(LIBERREG_LIBS)

PKG_CHECK_MODULES(LIBERDM, liberdm >= 0.1,,
                            AC_MSG_ERROR([*** Required EReader display manager Library (liberdm) 0.1 not installed ***]))
AC_SUBST(LIBERDM_CFLAGS)
AC_SUBST(LIBERDM_LIBS)


AC_OUTPUT([
Makefile
src/Makefile 
])
