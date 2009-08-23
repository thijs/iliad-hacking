dnl ----- Process this file with autoconf to produce a configure script -----

AC_INIT(displayMgr, 0.212)
AC_CONFIG_SRCDIR(src/displayMgr.c)
AC_PREREQ(2.57)
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS(config.h)

dnl ----- Checks for programs ---------------------------------------------
AC_PROG_CC
AC_STDC_HEADERS
AC_PROG_INSTALL

dnl ----- Args -------------------------------------------------------------

AC_ARG_ENABLE(debug,
  [  --enable-debug          enable debug ( verbose ) build],
     enable_debug=$enableval, enable_debug=no )
	 
dnl ------------------------------------------------------------------------

PKG_CHECK_MODULES(LIBERDM, liberdm >= 0.2,, 
			    AC_MSG_ERROR([*** Required EReader Display Manager Library (liberdm) 0.2 not installed ***]))
AC_SUBST(LIBERDM_CFLAGS)
AC_SUBST(LIBERDM_LIBS)

PKG_CHECK_MODULES(LIBERIPC, liberipc >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader IPC Library (liberipc) 0.1 not installed ***]))
AC_SUBST(LIBERIPC_CFLAGS)
AC_SUBST(LIBERIPC_LIBS)

AC_OUTPUT([
Makefile
src/Makefile 
])

dnl ==========================================================================
echo "
                    displayManager  $VERSION
                  =============================

        prefix:                 ${prefix}
        source code location:   ${srcdir}

        Building with Debug:                ${enable_debug}
"
