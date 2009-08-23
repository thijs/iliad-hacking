dnl ----- Process this file with autoconf to produce a configure script -----

AC_INIT(scribble, 0.212)
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR(src/ScribbleUI.c)
AC_PREREQ(2.57)

AC_PROG_CC
AC_PROG_CXX
AC_STDC_HEADERS
AC_PROG_INSTALL

PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

dnl CFLAGS="-g -O0"
dnl CXXFLAGS="-g -O0"

PKG_CHECK_MODULES(LIBERDM, liberdm >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader display manager Library (liberdm) 0.1 not installed ***]))
AC_SUBST(LIBERDM_CFLAGS)
AC_SUBST(LIBERDM_LIBS)

PKG_CHECK_MODULES(xml2, libxml-2.0,,
			AC_MSG_ERROR([*** Required libxml2 not installed ***]))
AC_SUBST(xml2_CFLAGS)
AC_SUBST(xml2_LIBS)

PKG_CHECK_MODULES(LIBERMANIFEST, libermanifest >= 0.1,,
			AC_MSG_ERROR([*** Required EReader Manifest Library ***]))
AC_SUBST(LIBERMANIFEST_CFLAGS)
AC_SUBST(LIBERMANIFEST_LIBS)


AC_OUTPUT([
Makefile
src/Makefile
])
