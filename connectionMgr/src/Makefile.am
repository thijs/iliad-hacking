
# The name of the binary program to be created 
bin_PROGRAMS = connectionMgr

#noinst_PROGRAMS = systemcallsTest

connectionMgr_SOURCES = main.c \
			background.c ../inc/background.h \
			gtkSettingItem.c ../inc/gtkSettingItem.h \
			gtkSettingItemTextEntry.c ../inc/gtkSettingItemTextEntry.h \
			gtkInfoItem.c ../inc/gtkInfoItem.h \
			gtkProfileEntry.c ../inc/gtkProfileEntry.h \
			gtkProfileGroup.c ../inc/gtkProfileGroup.h \
			widgetUtils.c ../inc/widgetUtils.h \
			connectionMgr.c ../inc/connectionMgr.h \
			connectionMgrData.c ../inc/connectionMgrData.h \
			commonData.c ../inc/commonData.h \
			connectScreenData.c ../inc/connectScreenData.h \
			editScreenData.c ../inc/editScreenData.h \
			connectScan.c ../inc/connectScan.h \
			connectPing.c ../inc/connectPing.h \
			connectBackground.c ../inc/connectBackground.h \
			displayStatus.c ../inc/displayStatus.h \
			erbusy.c ../inc/erbusy.h \
			pagebar.c ../inc/pagebar.h \
			connectScreen.c ../inc/connectScreen.h \
			connectScreenOptions.c ../inc/connectScreenOptions.h \
			connectScreenProfiles.c ../inc/connectScreenProfiles.h \
			editScreen.c ../inc/editScreen.h \
			editScreenProfiles.c ../inc/editScreenProfiles.h \
			editScreenOverview.c ../inc/editScreenOverview.h \
			editScreenSearch.c ../inc/editScreenSearch.h \
			editScreenWireless.c ../inc/editScreenWireless.h \
			editScreenWirelessExt.c ../inc/editScreenWirelessExt.h \
			editScreenDialup.c ../inc/editScreenDialup.h \
			editScreenStatic.c ../inc/editScreenStatic.h \
			editScreenProxy.c ../inc/editScreenProxy.h \
			pingThread.c ../inc/pingThread.h \
			scanThread.c ../inc/scanThread.h \
			system.c ../inc/system.h \
			toolbar.c ../inc/toolbar.h \
			modemUtils.c ../inc/modemUtils.h \
			languages.c ../inc/languages.h

#setup_LDADD = $(LIBINTL)
#connectionMgr_LDADD = $(top_builddir)/src/ecdc/libecdc.a $(top_builddir)/src/common/libcommon.a


AM_CPPFLAGS = $(GTHREAD_CFLAGS) $(GTK_CFLAGS) $(LIBERIPC_CFLAGS) $(LIBERDM_CFLAGS) $(LIBERREGXML_CFLAGS) $(LIBERGTK_CFLAGS) $(xml2_CFLAGS) \
		-DLOCALE_DIR=\"$(localedir)\" \
		-DDATA_DIR=\"$(pkgdatadir)\"

AM_LDFLAGS = $(GTHREAD_LIBS) $(GTK_LIBS) $(LIBERIPC_LIBS) $(LIBERDM_LIBS) $(LIBERREGXML_LIBS) $(LIBERGTK_LIBS) $(xml2_LDADD)

INCLUDES=-I$(top_srcdir)/inc

AM_CFLAGS = -Wall -Werror

