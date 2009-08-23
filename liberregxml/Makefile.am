SUBDIRS=src inc tst doc data

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liberregxml.pc

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)-snap`date +"%Y%m%d"`
