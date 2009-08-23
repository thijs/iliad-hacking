dnl ----- Process this file with autoconf to produce a configure script -----

AC_INIT(contentLister, 0.212)
AC_CONFIG_SRCDIR(src/main.cpp)
AC_PREREQ(2.57)

dnl ---- Language support -------------------------------------------------
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS(config.h)
AM_GNU_GETTEXT([external]) 
AM_GNU_GETTEXT_VERSION([0.14.1])
localedir='${datadir}/locale'
AC_SUBST(localedir)

dnl ----- Checks for programs ---------------------------------------------
AC_PROG_CC
AC_PROG_CXX
AC_STDC_HEADERS
AC_PROG_INSTALL

dnl ----- Args -------------------------------------------------------------

AC_ARG_ENABLE(debug,
  [  --enable-debug          enable debug ( verbose ) build],
     enable_debug=$enableval, enable_debug=no )

AC_ARG_ENABLE(command-line-input,
  [  --enable-command-line-input enable command line button key presses simulation [default=no] ],
     enable_command_line_input=$enableval, enable_command_line_input=no )
	 
dnl ------ eReader Logging ------------------------------------------------

AC_ARG_ENABLE(logging,
  [  --enable-logging        Enable eReader logging library usage [default=no] ],
     enable_logging=$enableval, enable_logging=no )

AM_CONDITIONAL(ENABLE_LOGGING, test x$enable_logging = x"yes")

dnl -----------------------------------------------------------------------

dnl ------ PC content ----------------------------------------------------

AM_CONDITIONAL(WANT_PC_CONTENT, test x$enable_pc_content = x"yes")

dnl ------ Command line input --------------------------------------------------

AM_CONDITIONAL(WANT_COMMANDLINEINPUT, test x$enable_command_line_input = x"yes")

dnl ------------------------------------------------------------------------

PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2 gthread-2.0)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

dnl --- removed dependency on Matchbox Library
dnl PKG_CHECK_MODULES(LIBMB, libmb >= 1.6,,
dnl 	         AC_MSG_ERROR([*** Required Matchbox Library (libmb) 1.6 not installed ***]))
dnl AC_SUBST(LIBMB_CFLAGS)
dnl AC_SUBST(LIBMB_LIBS)

PKG_CHECK_MODULES(LIBERIPC, liberipc >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader IPC Library (liberipc) 0.1 not installed ***]))
AC_SUBST(LIBERIPC_CFLAGS)
AC_SUBST(LIBERIPC_LIBS)

PKG_CHECK_MODULES(LIBERLOG, liberlog >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader LOG Library (liberlog) 0.1 not installed ***]))
AC_SUBST(LIBERLOG_CFLAGS)
AC_SUBST(LIBERLOG_LIBS)

PKG_CHECK_MODULES(LIBERDM, liberdm >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader display manager Library (liberdm) 0.1 not installed ***]))
AC_SUBST(LIBERDM_CFLAGS)
AC_SUBST(LIBERDM_LIBS)

PKG_CHECK_MODULES(LIBERREGXML, liberregxml >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader Registry Library (liberregxml) 0.1 not installed ***]))
AC_SUBST(LIBERREGXML_CFLAGS)
AC_SUBST(LIBERREGXML_LIBS)

PKG_CHECK_MODULES(LIBERGTK, libergtk >= 0.1,, 
			    AC_MSG_ERROR([*** Required EReader GTK Library (libergtk) 0.1 not installed ***]))
AC_SUBST(LIBERGTK_CFLAGS)
AC_SUBST(LIBERGTK_LIBS)

PKG_CHECK_MODULES(LIBERMANIFEST, libermanifest >= 0.1,,
                            AC_MSG_ERROR([*** Required EReader Manifest Library (libermanifest) 0.1 not installed ***]))
AC_SUBST(LIBERMANIFEST_CFLAGS)
AC_SUBST(LIBERMANIFEST_LIBS)

PKG_CHECK_MODULES(xml2, libxml-2.0,,
			AC_MSG_ERROR([*** Required libxml2 not installed ***]))
AC_SUBST(xml2_CFLAGS)
AC_SUBST(xml2_LIBS)

PKG_CHECK_MODULES(OPENSSL, openssl >= 0.9,,
			AC_MSG_ERROR([*** Required package openssl not installed ***]))
AC_SUBST(OPENSSL_CFLAGS)
AC_SUBST(OPENSSL_LIBS)

AC_OUTPUT([
Makefile
src/Makefile 
data/Makefile 
po/Makefile.in
])

dnl ==========================================================================
echo "
                    contentLister  $VERSION
                  =============================

        prefix:                 ${prefix}
        source code location:   ${srcdir}

        Building with Debug:                ${enable_debug}
        Building with command line input:   ${enable_command_line_input}
        Use ereader logging library:        ${enable_logging}

"
