# Top-level Makefile

# Define the path to the library and include directories
LIBDIR = /usr/local/lib
PATH_INCLUDEDIR = /usr/local/include/cs
LOCAL_INCLUDEDIR = ../include
CFLAGS = -Wall -Werror -fPIC
CC = gcc
AR = ar

export CFLAGS
export LIBDIR

# Path to each submodule
SUBDIRS = map

all: install

# Install all libraries
install: install_headers install_libs
uninstall: uninstall_headers uninstall_libs

install_headers:
	mkdir -p $(PATH_INCLUDEDIR)
	cp include/universal.h $(PATH_INCLUDEDIR)
	@for dir in $(SUBDIRS); do \
		cp include/$$dir.h $(PATH_INCLUDEDIR); \
	done

install_libs:
	mkdir -p $(LIBDIR)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir install; \
	done
	ldconfig

uninstall_headers:
	@for dir in $(SUBDIRS); do \
		rm -f $(PATH_INCLUDEDIR)/$$dir.h; \
	done
	rm -f $(PATH_INCLUDEDIR)/universal.h
	rmdir --ignore-fail-on-non-empty $(PATH_INCLUDEDIR)

uninstall_libs:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir uninstall; \
	done
	ldconfig

unittest:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir unittest; \
	done

# Clean all submodules
clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: install clean unittest