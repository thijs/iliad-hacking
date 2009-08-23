AC_INIT(erbusyd, 0.212)
AC_CONFIG_SRCDIR(src/erbusyd.c)
AC_PREREQ(2.57)
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC

PKG_CHECK_MODULES(LIBERIPC, liberipc >= 0.6,, 
			    AC_MSG_ERROR([*** Required EReader IPC Library (liberipc) 0.6 not installed ***]))
AC_SUBST(LIBERIPC_CFLAGS)
AC_SUBST(LIBERIPC_LIBS)

AC_OUTPUT([
Makefile
src/Makefile 
])
