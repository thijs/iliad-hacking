#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([liberdm], 0.212)
AC_SUBST(liberdm_la_version_info, 2:1:0)
AC_CONFIG_SRCDIR([src/erdmServer.c])
AM_INIT_AUTOMAKE()
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netinet/in.h stddef.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_TIME

# Checks for library functions.
AC_PROG_GCC_TRADITIONAL
AC_FUNC_SELECT_ARGTYPES
AC_CHECK_FUNCS([bzero gettimeofday select socket strtol])

dnl ----- Args -------------------------------------------------------------

AC_ARG_ENABLE(doxygen-docs,     
  [  --enable-doxygen-docs   build DOXYGEN API documentation (requires Doxygen)], 
     enable_doxygen_docs=$enableval,enable_doxygen_docs=no)

AC_ARG_ENABLE(epaper,
  [  --enable-epaper  enable support for epaper display [default=yes] ],
     enable_epaper=$enableval, enable_epaper=yes )

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


dnl ------ Precooked data --------------------------------------------------

AM_CONDITIONAL(WANT_EPAPER, test x$enable_epaper = x"yes")


AC_CONFIG_FILES([
    Makefile
    include/Makefile
    src/Makefile
    doc/Makefile
    liberdm.pc
    doc/Doxyfile
])
AC_OUTPUT

dnl ==========================================================================
echo "
                    Lib eReader Display Manager  $VERSION
                   =======================================

        prefix:                 ${prefix}
        source code location:   ${srcdir}

        Building with Debug:                ${enable_debug}
        Building with epaper support:		${enable_epaper}

        Building with API Documentation:    ${enable_doxygen_docs}

"
