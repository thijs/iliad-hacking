##AM_CPPFLAGS = $(DEPS_CFLAGS) -Wall -Werror \
AM_CPPFLAGS = $(DEPS_CFLAGS) -Wall \
        $(LIBERIPC_CFLAGS) $(LIBERDM_CFLAGS) $(LIBXML_CFLAGS) $(LIBERMANIFEST_CFLAGS) \
	-I$(MOZILLA_INCDIR)/gtkembedmoz \
	-I$(MOZILLA_INCDIR)/nspr \
	-I$(MOZILLA_INCDIR)/dom \
	-I$(MOZILLA_INCDIR)/webbrwsr \
	-I$(MOZILLA_INCDIR)/webbrowserpersist \
	-I$(MOZILLA_INCDIR)/docshell \
	-I$(MOZILLA_INCDIR)/content \
	-I$(MOZILLA_INCDIR)/widget \
	-I$(MOZILLA_INCDIR)/gfx \
	-I$(MOZILLA_INCDIR)/layout \
	-I$(MOZILLA_INCDIR)/string \
	-I$(MOZILLA_INCDIR)/necko \
	-I$(MOZILLA_INCDIR)/find \
	-I$(MOZILLA_INCDIR)/view \
	-I$(MOZILLA_INCDIR)/xpcom \
	-I$(MOZILLA_INCDIR)/pref \
	-DMOZILLA_INTERNAL_API \
	-DENABLE_NLS
AM_LDFLAGS = $(DEPS_LIBS) $(LIBERIPC_LIBS) $(LIBXML_LIBS) $(LIBERDM_LIBS) $(LIBERMANIFEST_LIBS) -L$(MOZILLA_LIBDIR) -lgtkembedmoz -lxpcom_core -lxpcom -lnspr4 -lplds4 -lplc4 

if ENABLE_LOGGING
AM_CPPFLAGS += -DENABLE_LOGGING $(LIBERLOG_CFLAGS)
AM_LDFLAGS  += $(LIBERLOG_LIBS)
endif

bin_PROGRAMS = browser
browser_SOURCES = browser.cpp mozEmbedCallbacks.cpp mozilla_api.cpp \
	indexFileHandler.cpp selectionFileHandler.cpp \
	selection.cpp browserSettings.cpp mozillaPreferences.cpp \
	pagebar.cpp displayUpdate.cpp \
	toolbar.cpp find.cpp main.cpp ipcServer.cpp\
	languages.c timingInfo.c contentLister.cpp\
	erbusy.cpp \
	browser.h indexFileHandler.h \
	browserSettings.h languages.h selectionFileHandler.h \
	browserTypes.h main.h selection.h \
	displayUpdate.h mozEmbedCallbacks.h timingInfo.h \
	find.h mozilla_api.h toolbar.h \
	mozillaPreferences.h contentLister.h\
	pagebar.h ipcServer.h browserLog.h \
	erbusy.h 


AM_CFLAGS = -Wall
