# Top-level Makefile

# Define the path to the library and include directories
LIBDIR = /usr/local/lib
PATH_INCLUDEDIR = /usr/local/include/cs
LOCAL_INCLUDEDIR = ../include

export PATH_INCLUDEDIR
export LOCAL_INCLUDEDIR
export LIBDIR

# Path to each submodule
SUBDIRS = universal vector

all: install

# Install all libraries
install:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir install; \
	done

# Clean all submodules
clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: install clean