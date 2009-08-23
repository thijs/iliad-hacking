bin_PROGRAMS = newLister

## noinst_PROGRAMS = testContentScan testStack testMetadataStoreManager

newLister_SOURCES = main.cpp \
	Geometry.cpp \
			gtkContentListItem.cpp \
			lister.cpp\
			control.cpp\
			control_share.cpp\
			metadataStoreManager.c \
			displayUpdate.c \
			button.c \
			programManager.c \
			viewer.c \
			erbusy.c \
			erMdsFs.c \
			cursor.cpp \
			gtkErrorScreen.c \
			erMdsXml.c \
			stack.c \
			erMSDisk.c \
			history.c \
			icons.cpp \
			pagebar.c \
			languages.c \
			erConnect.c \
			toolbar.c \
			system.c \
			powerManager.c \
			click.c \
			gtkMetadataWnd.c \
			gtkSearchWnd.c \
			search_files.c \
			pincodeIdleTime.c \
			gtkPincodeScreen.c \
			sign.c \
			gethwid.c \
			gtkSortWnd.c \
			gtkDistListWnd.c \
			timedids.c \
			$(top_srcdir)/inc/button.h \
			$(top_srcdir)/inc/contentListerLog.h \
			$(top_srcdir)/inc/control.h \
			$(top_srcdir)/inc/control_share.h \
			$(top_srcdir)/inc/displayUpdate.h \
			$(top_srcdir)/inc/gtkContentListItem.h \
			$(top_srcdir)/inc/lister.h \
			$(top_srcdir)/inc/metadataStoreManager.h \
			$(top_srcdir)/inc/programManager.h \
			$(top_srcdir)/inc/viewer.h \
			$(top_srcdir)/inc/erbusy.h \
			$(top_srcdir)/inc/erMdsContent.h \
			$(top_srcdir)/inc/cursor.h \
			$(top_srcdir)/inc/gtkErrorScreen.h \
			$(top_srcdir)/inc/stack.h \
			$(top_srcdir)/inc/erMSDisk.h \
			$(top_srcdir)/inc/history.h \
			$(top_srcdir)/inc/languages.h \
			$(top_srcdir)/inc/gettext.h \
			$(top_srcdir)/inc/icons.h \
			$(top_srcdir)/inc/regValue.h \
			$(top_srcdir)/inc/erConnect.h \
			$(top_srcdir)/inc/pagebar.h \
			$(top_srcdir)/inc/system.h \
			$(top_srcdir)/inc/toolbar.h \
			$(top_srcdir)/inc/powerManager.h \
			$(top_srcdir)/inc/click.h \
			$(top_srcdir)/inc/gtkMetadataWnd.h \
			$(top_srcdir)/inc/gtkSearchWnd.h \
			$(top_srcdir)/inc/search_files.h \
			$(top_srcdir)/inc/gtkPincodeScreen.h \
			$(top_srcdir)/inc/pincodeIdleTime.h \
			$(top_srcdir)/inc/sign.h \
			$(top_srcdir)/inc/gethwid.h \
			$(top_srcdir)/inc/tffs.h \
			$(top_srcdir)/inc/gtkSortWnd.h \
			$(top_srcdir)/inc/gtkDistListWnd.h \
			$(top_srcdir)/inc/timedids.h


# Removed tstProgramManager (rev 773)
# tstProgramManager_SOURCES = tstProgramManager.c programManager.c erbusy.c

testContentScan_SOURCES = testContentScan.c erMdsFs.c erMdsXml.c languages.c metadataStoreManager.c stack.c

testStack_SOURCES = stack.c testStack.c erMdsFs.c erMdsXml.c languages.c metadataStoreManager.c 

testMetadataStoreManager_SOURCES = languages.c stack.c metadataStoreManager.c testMetadataStoreManager.c erMdsFs.c erMdsXml.c

# The DEPS_FLAGS comes from a variable that is set up in configure.ac file

AM_CPPFLAGS = $(DEPS_CFLAGS) $(LIBERLOG_CFLAGS) $(LIBERREGXML_CFLAGS) $(LIBERIPC_CFLAGS) $(LIBERDM_CFLAGS) $(LIBERGTK_CFLAGS) $(LIBERMANIFEST_CFLAGS) $(xml2_CFLAGS) $(OPENSSL_CFLAGS) -DDATA_DIR=\"$(pkgdatadir)\" -DLOCALE_DIR=\"$(localedir)\"
AM_LDFLAGS =  $(DEPS_LIBS)   $(LIBERLOG_LIBS)   $(LIBERREGXML_LIBS)   $(LIBERIPC_LIBS)   $(LIBERDM_LIBS)   $(LIBERGTK_LIBS)   $(LIBERMANIFEST_LIBS)   $(xml2_LIBS)   $(OPENSSL_LIBS)

if WANT_COMMANDLINEINPUT
AM_CPPFLAGS += -DCOMMAND_LINE_INPUT
endif

if ENABLE_LOGGING
AM_CPPFLAGS += -DENABLE_LOGGING $(LIBERLOG_CFLAGS)
AM_LDFLAGS  += $(LIBERLOG_LIBS)
endif

AM_CPPFLAGS += -I$(top_srcdir)/inc 

AM_CFLAGS = -Wall -Werror

