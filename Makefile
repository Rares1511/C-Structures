# Top-level Makefile

LIBDIR           = /usr/local/lib
PATH_INCLUDEDIR  = /usr/local/include/cs
LOCAL_INCLUDEDIR = include
CFLAGS           = -Wall -Werror -fPIC
CC               = gcc

UNITTEST_LOG = unittest_log.out
SEED ?= 42

ifeq ($(debug),true)
  CFLAGS += -DDEBUG
endif

# List your modules (directories) here
SUBDIRS   = map

# Output directory for local libs
LIBOUTDIR = lib

# Per-module files
MODULE_SRCS = $(SUBDIRS:%=%/%).c             # e.g. map/map.c
MODULE_OBJS = $(SUBDIRS:%=%/%).o             # e.g. map/map.o
LIBS        = $(SUBDIRS:%=$(LIBOUTDIR)/lib%.so)  # e.g. lib/libmap.so

# Default target: uninstall, install, test
all: uninstall install unittest

# ---------- Per-module build rules ----------

$(LIBOUTDIR):
	mkdir -p $(LIBOUTDIR)

define MODULE_BUILD_RULES
$1/$1.o: $1/$1.c
	$$(CC) -fPIC -c -o $$@ $$< $$(CFLAGS)

$(LIBOUTDIR)/lib$1.so: $1/$1.o | $(LIBOUTDIR)
	$$(CC) -shared -o $$@ $$^ $$(CFLAGS)
endef

$(foreach m,$(SUBDIRS),$(eval $(call MODULE_BUILD_RULES,$(m))))

# ---------- Install / Uninstall ----------

install: install_headers install_libs

uninstall: uninstall_headers uninstall_libs

install_headers:
	mkdir -p $(PATH_INCLUDEDIR)
	cp $(LOCAL_INCLUDEDIR)/universal.h $(PATH_INCLUDEDIR)
	@for dir in $(SUBDIRS); do \
		cp $(LOCAL_INCLUDEDIR)/$$dir.h $(PATH_INCLUDEDIR); \
	done

install_libs: $(LIBS)
	mkdir -p $(LIBDIR)
	@for dir in $(SUBDIRS); do \
		cp $(LIBOUTDIR)/lib$$dir.so $(LIBDIR); \
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
		rm -f $(LIBDIR)/lib$$dir.so; \
	done
	ldconfig

# ---------- Unit tests ----------

# Per-module unittest target:
#   unittest-map, unittest-foo, ...
define MODULE_TEST_RULES
unittest-$1: install $1/$1_unittest.c
	@echo "Building unit tests for $1..."
	$(CC) -o $1/unittest $1/$1_unittest.c $(CFLAGS) -l$1
	@echo "Running unit tests for $1..."
	@if [ "$(memcheck)" = "true" ]; then \
	  /usr/bin/time -f "[${1}] Time: %E (real), %U (user), %S (sys)" \
	    valgrind --leak-check=full $1/unittest $(UNITTEST_LOG) $(SEED) \
	    >> $(UNITTEST_LOG) 2>&1 ; \
	else \
	  /usr/bin/time -f "[${1}] Time: %E (real), %U (user), %S (sys)" \
	    $1/unittest $(UNITTEST_LOG) $(SEED) \
	    >> $(UNITTEST_LOG) 2>&1 ; \
	fi
	@echo "Unit tests for $1 completed."
	@rm -f $1/unittest
endef

$(foreach m,$(SUBDIRS),$(eval $(call MODULE_TEST_RULES,$(m))))

# Aggregate unittest target
unittest: $(SUBDIRS:%=unittest-%)

# ---------- Clean ----------

clean:
	@for dir in $(SUBDIRS); do \
		rm -f $$dir/$$dir.o $$dir/unittest; \
	done
	rm -f $(LIBS) $(UNITTEST_LOG)

.PHONY: all install uninstall unittest clean
