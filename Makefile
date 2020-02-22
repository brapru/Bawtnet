# Top level Makefile


default: all

.DEFAULT:
		cd src && $(MAKE) $@

install:
		cd src && $(MAKE) $@

.PHONY: install
