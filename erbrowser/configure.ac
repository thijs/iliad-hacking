dnl ----- Process this file with autoconf to produce a configure script -----

AC_INIT(erbrowser, 0.212)
AC_CONFIG_SRCDIR(src/main.cpp)
AC_PREREQ(2.57)
AM_INIT_AUTOMAKE

dnl ----- Checks for programs ---------------------------------------------
AC_PROG_CC
AC_PROG_CXX
AC_STDC_HEADERS
AC_PROG_INSTALL

dnl ------ eReader Logging ------------------------------------------------

AC_ARG_ENABLE(logging,
  [  --enable-logging        Enable eReader logging library usage [default=no] ],
     enable_logging=$enableval, enable_logging=no )

AM_CONDITIONAL(ENABLE_LOGGING, test x$enable_logging = x"yes")

dnl -----------------------------------------------------------------------

AC_ARG_WITH(mozilla-includes, [--with-mozilla-includes=DIR], mozilla_includes=$withval, mozilla_includes=/usr/include/mozilla)

MOZILLA_INCDIR="$mozilla_includes"
AC_SUBST(MOZILLA_INCDIR)

AC_ARG_WITH(mozilla-libs, [--with-mozilla-libs=DIR], mozilla_libs=$withval, mozilla_libs=/usr/lib/mozilla)

MOZILLA_LIBDIR="$mozilla_libs"
AC_SUBST(MOZILLA_LIBDIR)

PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2 gthread-2.0)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

PKG_CHECK_MODULES(LIBERIPC, liberipc >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader IPC Library (liberipc) 0.1 not installed ***]))
AC_SUBST(LIBERIPC_CFLAGS)
AC_SUBST(LIBERIPC_LIBS)

PKG_CHECK_MODULES(LIBERDM, liberdm >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader display manager Library (liberdm) 0.1 not installed ***]))
AC_SUBST(LIBERDM_CFLAGS)
AC_SUBST(LIBERDM_LIBS)

PKG_CHECK_MODULES(LIBERLOG, liberlog >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader logging Library (liberlog) 0.1 not installed ***]))
AC_SUBST(LIBERLOG_CFLAGS)
AC_SUBST(LIBERLOG_LIBS)

PKG_CHECK_MODULES(LIBXML, libxml-2.0 >= 2.6)
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LDADD)

PKG_CHECK_MODULES(LIBERMANIFEST, libermanifest >= 0.1,,
                            AC_MSG_ERROR([*** Required EReader XML manifest Library (libermanifest) 0.1 not installed ***]))
AC_SUBST(LIBERMANIFEST_CFLAGS)
AC_SUBST(LIBERMANIFEST_LIBS)

AC_OUTPUT([
Makefile
src/Makefile 
])
