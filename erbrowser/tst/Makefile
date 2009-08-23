




ifeq ($(eReaderPlatform),pc)
	GTKLOC=/usr/include
	ERIPCLOC=/usr/local/include
	GTKLOCWEIRD=/usr/lib
	MOZILLA_BUILD_RESULT=/home/ann/Browsers/mozilla/build_result/dist
  ifeq ($(browser_user),DeTijd)
	  DEFINES =  -D MOZILLA_INTERNAL_API -D PC_ENVIRONMENT -D LOCAL_DISPLAY -DENABLE_NLS -DDETIJD
  else 	
    ifeq ($(used_display),local)	
	  DEFINES =  -D MOZILLA_INTERNAL_API -D PC_ENVIRONMENT -D LOCAL_DISPLAY -D ENABLE_NLS
    else
	  DEFINES = -D MOZILLA_INTERNAL_API -D PC_ENVIRONMENT -D ENABLE_NLS 
    endif
  endif	
else
  ifeq ($(eReaderPlatform),arm)
		GTKLOC=${QPEDIR}/include
   		ERIPCLOC=${QPEDIR}/include
		GTKLOCWEIRD=${QPEDIR}/lib
		MOZILLA_BUILD_RESULT=${WORKDIR}/minimo-0.0cvs20050727-r8/build-arm-linux/dist
		DEFINES = -D MOZILLA_INTERNAL_API -D ENABLE_NLS
  else
	  GTKLOC=/usr/local/include
          GTKPAGEBAR=/usr/local/include
	  ERIPCLOC=/usr/local/include
	  GTKLOCWEIRD=/usr/local/lib
	  MOZILLA_BUILD_RESULT=/home/mma/packages/mozilla/build_result/dist
	  DEFINES = -D MOZILLA_INTERNAL_API -D ENABLE_NLS
  endif
endif

ifeq ($(eReaderPlatform),pc)
  #CFLAGS = -Wall -O2
  CFLAGS = -Wall -g -DDEBUG 
  LDFLAGS =  --verbose -L$(MOZILLA_BUILD_RESULT)/Embed -lgtkembedmoz -lplds4 -lplc4 -lnspr4 -lxpcom -lxpcom_core -lglib-2.0 -lgtk-x11-2.0 -lpango-1.0 -latk-1.0 -lexpat -leripc -lgthread-2.0 -lgtkPagebar -lpangoxft-1.0 -lpangox-1.0 -lerdm
else
  CFLAGS = -Wall -O2 -DDEBUG
  #CFLAGS = -Wall -g -DDEBUG
  #LDFLAGS =  -lgklayout
  EXTRA_LDFLAGS =  --verbose -L$(MOZILLA_BUILD_RESULT)/lib -L$(GTKLOCWEIRD) -lgtkembedmoz -lplds4 -lplc4 -lnspr4 -lxpcom -lxpcom_core -lglib-2.0 -lgtk-x11-2.0 -lpango-1.0 -latk-1.0 -lexpat -leripc -lgthread-2.0 -lgtkPagebar -lerdm
endif


INCLUDES += -I../inc -I../tst -I../../common/inc -I/usr/include -I$(GTKPAGEBAR)/ -I$(GTKLOC)/gtk-2.0 -I$(GTKLOC)/glib-2.0 -I$(GTKLOC)/include \
 -I$(GTKLOC)/gtk-2.0/include -I$(GTKLOCWEIRD)/glib-2.0/include -I$(GTKLOCWEIRD)/gtk-2.0/include \
 -I$(GTKLOC)/pango-1.0 -I$(GTKLOC)/atk-1.0 \
 -I$(MOZILLA_BUILD_RESULT)/include/content -I$(MOZILLA_BUILD_RESULT)/include/widget \
 -I$(MOZILLA_BUILD_RESULT)/include/gfx  -I$(MOZILLA_BUILD_RESULT)/include/webbrowserpersist \
 -I$(MOZILLA_BUILD_RESULT)/include/dom -I$(MOZILLA_BUILD_RESULT)/include/docshell \
 -I$(MOZILLA_BUILD_RESULT)/include/layout -I$(MOZILLA_BUILD_RESULT)/include/pref -I$(MOZILLA_BUILD_RESULT)/include/string \
 -I$(MOZILLA_BUILD_RESULT)/include/necko -I$(MOZILLA_BUILD_RESULT)/include/find -I$(MOZILLA_BUILD_RESULT)/include/webbrwsr \
 -I$(MOZILLA_BUILD_RESULT)/include/gtkembedmoz -I$(MOZILLA_BUILD_RESULT)/include/dom -I$(MOZILLA_BUILD_RESULT)/include/nspr \
 -I$(MOZILLA_BUILD_RESULT)/include/xpcom -I$(MOZILLA_BUILD_RESULT)/include/view \
 -I$(ERIPCLOC)/liberipc/

 
CSOURCE = languages.c gtkToolbarEventbox.c timingInfo.c 
CXXSOURCE = main.cpp browser.cpp mozEmbedCallbacks.cpp mozilla_api.cpp indexFileHandler.cpp selectionFileHandler.cpp \
selection.cpp browserSettings.cpp mozillaPreferences.cpp pagebar.cpp pageLocation.cpp displayUpdate.cpp \
toolbar.cpp find.cpp

HEADERS = languages.h gtkToolbarEventbox.h main.h browserTypes.h browser.h mozEmbedCallbacks.h mozilla_api.h indexFileHandler.h \
timingInfo.h selection.h browserSettings.h mozillaPreferences.h pagebar.h pageLocation.h displayUpdate.h \
toolbar.h find.h 

OBJECTS = $(CXXSOURCE:.cpp=.o) $(CSOURCE:.c=.o)

%.o: ../../common/src/%.c ../../common/inc/%.h 
	$(CC) $(DEFINES) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.cpp %.h 
	$(CXX) $(DEFINES) $(CFLAGS) $(INCLUDES) -c $<


%.o: ../src/%.cpp ../inc/%.h 
	$(CXX) $(DEFINES) $(CFLAGS) $(INCLUDES) -c $<

# $@ refers to the name of the current target	
browser : $(OBJECTS)  
	$(CXX) $(LDFLAGS) $(EXTRA_LDFLAGS) $(OBJECTS) -o $@    

all : browser

clean:
	rm -f *.so *.o *.a *~ browser
