AM_CFLAGS = -g -Wall

INCLUDES = @LIBXML_CFLAGS@ -I$(top_srcdir)/include 

noinst_PROGRAMS = ermanifestTest
ermanifestTest_SOURCES = test.c
ermanifestTest_LDADD = @LIBXML_LIBS@ ../src/libermanifest.la

