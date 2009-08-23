AM_CPPFLAGS = $(DEPS_CFLAGS) $(LIBERDM_CFLAGS) $(LIBERGTK_CFLAGS) $(LIBERIPC_CFLAGS) $(LIBERREGXML_CFLAGS) -DLOCALE_DIR=\"$(localedir)\" -DDATA_DIR=\"$(pkgdatadir)\"
AM_LDFLAGS  = $(DEPS_LIBS)   $(LIBERDM_LIBS)   $(LIBERGTK_LIBS)   $(LIBERIPC_LIBS)   $(LIBERREGXML_LIBS) 

bin_PROGRAMS = setup
setup_SOURCES = main.c                                               \
                background.c               background.h              \
                setupLog.h                                           \
                settings.c                 settings.h                \
                settingsData.c             settingsData.h            \
                displayStatus.c            displayStatus.h           \
                erbusy.c                   erbusy.h                  \
                pagebar.c                  pagebar.h                 \
                gtkPincodeEntry.c          gtkPincodeEntry.h         \
                gtkSettingItem.c           gtkSettingItem.h          \
                gtkSettingItemTextEntry.c  gtkSettingItemTextEntry.h \
                gtkInfoItem.c              gtkInfoItem.h             \
                languages.c                languages.h               \
                system.c                   system.h                  \
                iLiadConfigScreen.c        iLiadConfigScreen.h       \
                iLiadConfigData.c          iLiadConfigData.h         \
                iLiadPincodeScreen.c       iLiadPincodeScreen.h      \
                iLiadPincodeData.c         iLiadPincodeData.h        \
                iLiadUserScreen.c          iLiadUserScreen.h         \
                iLiadUserData.c            iLiadUserData.h           \
                iLiadDateTimeScreen.c      iLiadDateTimeScreen.h     \
                iLiadBgConnectScreen.c     iLiadBgConnectScreen.h    \
                iLiadBgConnectData.c       iLiadBgConnectData.h      \
                iLiadPCConnectData.c       iLiadPCConnectData.h      \
                iLiadPCConnectScreen.c     iLiadPCConnectScreen.h    \
                iLiadArcLocData.c          iLiadArcLocData.h         \
                iLiadArcLocScreen.c        iLiadArcLocScreen.h       \
                iLiadStartUpData.c         iLiadStartUpData.h        \
                iLiadStartUpScreen.c       iLiadStartUpScreen.h      \
                iLiadLanguageData.c        iLiadLanguageData.h       \
                iLiadLanguageScreen.c      iLiadLanguageScreen.h     \
                iLiadTimediDSScreen.c      iLiadTimediDSScreen.h     \
                iLiadTimediDSData.c        iLiadTimediDSData.h       \
                iLiadDeviceOverview.c      iLiadDeviceOverview.h     \
                gettext.h                                            \
                toolbar.h toolbar.c

setup_LDADD = $(LIBINTL)
AM_CFLAGS = -Wall -Werror
