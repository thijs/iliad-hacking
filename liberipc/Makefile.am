SUBDIRS=include src test doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = liberipc.pc

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)-snap`date +"%Y%m%d"`
