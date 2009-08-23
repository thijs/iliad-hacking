INCLUDES = $(LIBERDM_CFLAGS) $(DEPS_CFLAGS) $(LIBERMANIFEST_CFLAGS) $(xml2_CFLAGS)

source_h =  $(top_srcdir)/inc/erbusy.h \
	    $(top_srcdir)/inc/ScribbleLog.h \
	    $(top_srcdir)/inc/ScribbleUtils.h \
	    $(top_srcdir)/inc/displayUpdate.h \
	    $(top_srcdir)/inc/ink.h \
	    $(top_srcdir)/inc/ink_draw.h \
	    $(top_srcdir)/inc/ink_intersect.h \
	    $(top_srcdir)/inc/ink_rw.h \
	    $(top_srcdir)/inc/FileManager.h \
	    $(top_srcdir)/inc/FileMgr_page.h \
	    $(top_srcdir)/inc/FileMgr_xml.h \
	    $(top_srcdir)/inc/ScribbleDefine.h \
	    $(top_srcdir)/inc/ScribbleSaveThread.h \
	    $(top_srcdir)/inc/ScribbleCore.h \
	    $(top_srcdir)/inc/ScribbleUI.h \
	    $(top_srcdir)/inc/ScribbleIPC.h \
	    $(top_srcdir)/inc/ScribbleSignalDeal.h

source_c =  erbusy.c \
	    ScribbleUtils.c \
	    displayUpdate.c \
	    ink.c \
	    ink_draw.c \
	    ink_intersect.c \
	    ink_rw_file.c \
	    ink_rw_xml.c \
	    FileManager.c \
	    FileMgr_page.c \
	    FileMgr_xml.c \
	    FormatConverter.c \
	    ScribbleSaveThread.c \
	    ScribbleCore.c \
	    ScribbleUI.c \
	    IPCServer.c \
	    pagebar.c \
	    toolbar.c \
	    ScribbleSignalDeal.c

bin_PROGRAMS = scribble
scribble_SOURCES = $(source_c) $(source_h)
scribble_LDADD = $(LIBERDM_LIBS) $(LIBERMANIFEST_LIBS)  $(DEPS_LIBS) $(xml2_LIBS)  -leripc -lgthread-2.0 -lpthread

AM_CPPFLAGS = -Wall -Werror -DDATA_DIR=\"$(pkgdatadir)\" -I$(top_srcdir)/inc 
