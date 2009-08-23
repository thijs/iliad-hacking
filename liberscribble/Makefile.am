SUBDIRS = include src doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liberscribble.pc

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)-snap`date +"%Y%m%d"`
