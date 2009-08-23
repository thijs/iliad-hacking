nl ----- Process this file with autoconf to produce a configure script -----

AC_INIT(ersetup, 0.212)
AC_CONFIG_SRCDIR(src/main.c)
AC_PREREQ(2.57)
AM_INIT_AUTOMAKE([foreign])

dnl ----- Args -------------------------------------------------------------

AC_ARG_ENABLE(pc-environment,
  [  --enable-pc-environment enable pc environment [default=no] ],
     enable_pc_environment=$enableval, enable_pc_environment=no )

AC_ARG_ENABLE(glib-assert,
  [  --enable-glib-assert enable glib assert [default=no] ],
     enable_glib_assert=$enableval, enable_glib_assert=no )
	 

dnl ------ PC environment ----------------------------------------------------
AM_CONDITIONAL(WANT_PC_ENVIRONMENT, test x$enable_pc_environment = x"yes")	 

dnl ------ assert enabled ----------------------------------------------------
AM_CONDITIONAL(WANT_ASSERT_ENABLED, test x$enable_glib_assert = x"yes")	 

	 
dnl ----- Checks for programs ---------------------------------------------
AC_PROG_CC
AC_PROG_CXX
AC_STDC_HEADERS
AC_PROG_INSTALL

dnl ---- Language support -------------------------------------------------
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS(config.h)
AM_GNU_GETTEXT([external]) 
AM_GNU_GETTEXT_VERSION([0.14.1])
localedir='${datadir}/locale'
AC_SUBST(localedir)

PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2 gthread-2.0)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

PKG_CHECK_MODULES(LIBERDM, liberdm >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader display manager Library (liberdm) 0.1 not installed ***]))
AC_SUBST(LIBERDM_CFLAGS)
AC_SUBST(LIBERDM_LIBS)

PKG_CHECK_MODULES(LIBERREGXML, liberregxml >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader registry Library (liberregxml) 0.1 not installed ***]))
AC_SUBST(LIBERREGXML_CFLAGS)
AC_SUBST(LIBERREGXML_LIBS)

PKG_CHECK_MODULES(LIBERIPC, liberipc >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader IPC Library (liberipc) 0.1 not installed ***]))
AC_SUBST(LIBERIPC_CFLAGS)
AC_SUBST(LIBERIPC_LIBS)

PKG_CHECK_MODULES(LIBERGTK, libergtk >= 0.1,,
                            AC_MSG_ERROR([*** Required EReader GTK Library (libergtk) 0.1 not installed ***]))
AC_SUBST(LIBERGTK_CFLAGS)
AC_SUBST(LIBERGTK_LIBS)


AC_OUTPUT([
Makefile
src/Makefile 
data/Makefile
po/Makefile.in
])

dnl ==========================================================================
echo "
                    settings  $VERSION
                  =============================

        prefix:                 ${prefix}
        source code location:   ${srcdir}

	Building for PC environment:	${enable_pc_environment}
	Building with glib assert enabled:	${enable_glib_assert}
"
