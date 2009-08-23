#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.53)
AC_INIT([liberscribble], 0.212)
AC_SUBST(liberscribble_la_version_info, 0:1:0)
AC_CONFIG_SRCDIR([src/scbdoc.c])

AC_CONFIG_AUX_DIR(.)
AM_INIT_AUTOMAKE()
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_GNU_SOURCE
AC_PROG_CC
AC_PROG_LIBTOOL


dnl ----- Args -------------------------------------------------------------

AC_ARG_ENABLE(doxygen-docs,     
  [  --enable-doxygen-docs   build DOXYGEN API documentation (requires Doxygen)], 
     enable_doxygen_docs=$enableval,enable_doxygen_docs=no)

AC_ARG_ENABLE(debug,
  [  --enable-debug          enable debug ( verbose ) build],
     enable_debug=$enableval, enable_debug=no )

AC_ARG_ENABLE(gcov,
  [  --enable-gcov           enable gcov coverage ( testing only ) build],
     enable_gcov=$enableval, enable_gcov=no )

AC_ARG_ENABLE(gprof,
  [  --enable-gprof          enable gprof profiler ( testing only ) build],
     enable_gprof=$enableval, enable_gprof=no )

AC_ARG_ENABLE(unit-tests,
  [  --enable-unit-tests          enable unit tests ( Requires check )],
     enable_unit_tests=$enableval, enable_unit_tests=no )


dnl ------ Debug -----------------------------------------------------------

if test x$enable_debug != xno; then
  XLIBS_CFLAGS="-DDEBUG $XLIBS_CFLAGS"	
fi


dnl ------ Doxygen docs ----------------------------------------------------

if test x$enable_doxygen_docs = xyes ; then

  AC_PATH_PROG(DOXYGEN, doxygen, no)

  AC_MSG_CHECKING([whether to build Doxygen documentation])

  if test x$DOXYGEN = xno ; then
    have_doxygen=no
  else
    have_doxygen=yes
  fi

  if test x$have_doxygen = xno; then
    AC_MSG_ERROR([Building Doxygen docs explicitly required, but Doxygen not found])
  fi


  AC_MSG_RESULT(yes)

fi

AM_CONDITIONAL(HAVE_DOXYGEN, test x$enable_doxygen_docs = xyes)	

dnl ------ GCC flags --------------------------------------------------------

if test "x$GCC" = "xyes"; then

   if test x$enable_gprof = xyes; then
         GCC_WARNINGS="$GCC_WARNINGS -pg"
	 AC_MSG_RESULT([Enabling gprof Support.])	
   fi	 

   if test x$enable_gcov = xyes; then
         GCC_WARNINGS="$GCC_WARNINGS -fprofile-arcs -ftest-coverage"
	 AC_MSG_RESULT([Enabling gcov Support.])	
   fi	 

   GCC_WARNINGS="-g -Wall -Werror -fno-strict-aliasing $GCC_WARNINGS"

else
   if test x$enable_gprof = xyes; then
      AC_MSG_WARN([*** Need gcc compiler for gprof support])
   fi

   if test x$enable_gcov = xyes; then
      AC_MSG_WARN([*** Need gcc compiler for gcov support])
   fi
fi

AM_CONDITIONAL(HAVE_CHECK, test x$have_check = xyes)

dnl ------ Substitute in found libs, clags to Makefiles etc -----------------

AC_SUBST(GCC_WARNINGS)

PKG_CHECK_MODULES(LIBXML, libxml-2.0 >= 2.6)
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LDADD)

PKG_CHECK_MODULES(DEPS, glib-2.0 >= 2.2 )
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

PKG_CHECK_MODULES(LIBERMANIFEST, libermanifest >= 0.1,,
                            AC_MSG_ERROR([*** Required EReader Manifest Library (libermanifest) 0.1 not installed ***]))
AC_SUBST(LIBERMANIFEST_CFLAGS)
AC_SUBST(LIBERMANIFEST_LIBS)

PKG_CHECK_MODULES(LIBERDM, liberdm >= 0.1,,
                            AC_MSG_ERROR([*** Required EReader DM Library (liberdm) 0.1 not installed ***]))
AC_SUBST(LIBERDM)
AC_SUBST(LIBERDM)

dnl ------ 'Built in' checks ------------------------------------------------ 

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([langinfo.h locale.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_STAT
AC_FUNC_STRCOLL
AC_CHECK_FUNCS([memset select setlocale strcasecmp strchr strdup strncasecmp strstr])

AC_C_BIGENDIAN

AC_OUTPUT([
    Makefile
    liberscribble.pc
    include/Makefile
    src/Makefile
    doc/Makefile
    doc/Doxyfile
])

dnl ==========================================================================
echo "
                    Lib eReader Scribble  $VERSION
                  ===================================

        prefix:                 ${prefix}
        source code location:   ${srcdir}

        Building with Debug:                ${enable_debug}
        Building with gcov:                 ${enable_gcov}
        Building with gprof:                ${enable_gprof}
        Building unit tests:                ${have_check}

        Building with API Documentation:    ${enable_doxygen_docs}

"
