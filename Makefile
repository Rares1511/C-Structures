# ==============================================================================
# Top-level Makefile for C Data Structures Library
# ==============================================================================

# ---------------- OS Detection ----------------
ifeq ($(OS),Windows_NT)
    DETECTED_OS := windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS := linux
    else ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := macos
    else
        DETECTED_OS := unknown
    endif
endif
$(info [OS] Detected: $(DETECTED_OS))

# ---------------- Tooling & Paths ----------------
CC      := gcc
RM      := rm -rf
MKDIR_P := mkdir -p

# Local Build Directories
LIBOUTDIR        := lib
LOCAL_INCLUDEDIR := include

# System Installation Paths
LIBDIR           := /usr/local/lib
PATH_INCLUDEDIR  := /usr/local/include/cs

# Flags
# -I$(LOCAL_INCLUDEDIR) is key: it allows compilation without prior installation
CFLAGS  := -Wall -Wextra -fPIC -Wno-unknown-pragmas -g -I$(LOCAL_INCLUDEDIR)
LDLIBS  := -lm

# Unittest Configuration
UNITTEST_LOG := unittest_log.ansi
SEED ?= 42
ARGS = --seed $(SEED)

ifeq ($(debug),true)
  ARGS += --debug-file $(UNITTEST_LOG)
endif

# ---------------- Modules ----------------
SUBDIRS := cargs pair vector deque list forward_list set map unordered_set \
           unordered_map stack multiset multimap unordered_multiset \
           unordered_multimap queue priority_queue flat_set large_number clogger cstring

INSTALL_LIBS := $(SUBDIRS)

# ---------------- Core/Dependency objects ----------------
CORE_OBJS := rbt/rbt.o pair/pair.o hash_table/hash_table.o cstring/nfa.o

# Link-time dependencies for shared libraries
DEPS_rbt                := vector/vector.o
DEPS_map                := rbt/rbt.o pair/pair.o
DEPS_set                := rbt/rbt.o
DEPS_stack              := vector/vector.o deque/deque.o list/list.o
DEPS_queue              := deque/deque.o list/list.o
DEPS_priority_queue     := vector/vector.o deque/deque.o
DEPS_unordered_set      := hash_table/hash_table.o vector/vector.o
DEPS_unordered_map      := hash_table/hash_table.o vector/vector.o pair/pair.o
DEPS_multiset           := rbt/rbt.o pair/pair.o
DEPS_multimap           := rbt/rbt.o pair/pair.o vector/vector.o
DEPS_unordered_multiset := hash_table/hash_table.o pair/pair.o vector/vector.o
DEPS_unordered_multimap := hash_table/hash_table.o pair/pair.o vector/vector.o
DEPS_flat_set           := vector/vector.o deque/deque.o
DEPS_cstring            := cstring/nfa.o vector/vector.o

# Additional linker flags for specific modules
LDLIBS_clogger          := -lpthread -rdynamic

# ---------------- Derived paths ----------------
MOD_OBJ = $1/$1.o
ALL_MOD_OBJS := $(foreach m,$(SUBDIRS),$(call MOD_OBJ,$(m)))
LIBS := $(foreach m,$(INSTALL_LIBS),$(LIBOUTDIR)/lib$(m).so)

# ---------------- Default Targets ----------------

# 'all' builds everything locally and runs tests. No sudo required.
all: libs unittest

# 'libs' compiles all shared objects into the local /lib folder
libs: $(LIBOUTDIR) $(LIBS)
	@echo "--- Local build complete ---"

$(LIBOUTDIR):
	$(MKDIR_P) $(LIBOUTDIR)

# Generic rule: build module/module.o from module/module.c
# Uses -I$(LOCAL_INCLUDEDIR) defined in CFLAGS
%/%.o: %/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Explicit rule for non-standard object files (name != directory)
cstring/nfa.o: cstring/nfa.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Convenience target for objects
objects: $(ALL_MOD_OBJS) $(CORE_OBJS)

# ---------------- Linker Logic ----------------

define LINK_SO_RULE
$(LIBOUTDIR)/lib$1.so: $(call MOD_OBJ,$1) $$(DEPS_$1) | $(LIBOUTDIR)
	$$(CC) -shared -o $$@ $$^ $$(CFLAGS) $$(LDLIBS) $$(LDLIBS_$1)
endef
$(foreach m,$(INSTALL_LIBS),$(eval $(call LINK_SO_RULE,$(m))))

# ---------------- Installation (Requires Sudo) ----------------

install: libs install_headers install_libs

install_headers:
	@echo "Installing headers to $(PATH_INCLUDEDIR)..."
	$(MKDIR_P) $(PATH_INCLUDEDIR)
	cp $(LOCAL_INCLUDEDIR)/cs/universal.h $(PATH_INCLUDEDIR)
	@for h in $(INSTALL_LIBS); do \
	    if [ -f $(LOCAL_INCLUDEDIR)/cs/$$h.h ]; then \
	        cp $(LOCAL_INCLUDEDIR)/cs/$$h.h $(PATH_INCLUDEDIR); \
	    fi \
	done
	cp $(LOCAL_INCLUDEDIR)/cs/rbt.h $(PATH_INCLUDEDIR)
	cp $(LOCAL_INCLUDEDIR)/cs/hash_table.h $(PATH_INCLUDEDIR)
	cp cstring/nfa.h $(PATH_INCLUDEDIR)

install_libs:
	@echo "Installing libraries to $(LIBDIR)..."
	$(MKDIR_P) $(LIBDIR)
	@for m in $(INSTALL_LIBS); do \
	    cp $(LIBOUTDIR)/lib$$m.so $(LIBDIR); \
	done
	ldconfig

uninstall:
	@echo "Uninstalling..."
	@for h in $(INSTALL_LIBS); do $(RM) $(PATH_INCLUDEDIR)/$$h.h; done
	$(RM) $(PATH_INCLUDEDIR)/universal.h
	$(RM) $(PATH_INCLUDEDIR)/rbt.h
	$(RM) $(PATH_INCLUDEDIR)/hash_table.h
	$(RM) $(PATH_INCLUDEDIR)/nfa.h
	@for m in $(INSTALL_LIBS); do $(RM) $(LIBDIR)/lib$$m.so; done
	rmdir --ignore-fail-on-non-empty $(PATH_INCLUDEDIR) || true
	ldconfig

# ---------------- Unit Testing ----------------

# All libraries needed for linking (all modules that have tests)
UNITTEST_LIBS := $(foreach m,$(INSTALL_LIBS),-l$(m))

unittest-log-init:
ifeq ($(debug),true)
	@$(RM) $(UNITTEST_LOG)
	@touch $(UNITTEST_LOG)
endif

build_unittest: unittest-log-init libs
	@$(CC) -o unittest unittest.c $(CFLAGS) \
	    -L$(LIBOUTDIR) $(UNITTEST_LIBS) $(LDLIBS)
	
run_unittest: build_unittest
	@export LD_LIBRARY_PATH=$(CURDIR)/$(LIBOUTDIR):$$LD_LIBRARY_PATH; \
	for module in $(SUBDIRS); do \
	  if [ "$(memcheck)" = "true" ]; then \
	      valgrind --leak-check=full ./unittest $(ARGS) \
		  --debug-file logs/debug.ansi \
		  --module $$module \
		  --results-file logs/results.ansi \
		  >> logs/memcheck.ansi 2>&1; \
	  else \
	    ./unittest $(ARGS) \
		--debug-file logs/debug.ansi \
		--module $$module \
		--results-file logs/results.ansi; \
	  fi \
	done

unittest: build_unittest run_unittest

# ---------------- Cleanup ----------------

clean:
	@echo "Cleaning build files..."
	$(RM) $(UNITTEST_LOG)
	$(RM) unittest
	$(RM) logs/*
	$(RM) $(LIBOUTDIR)/*.so
	@for m in $(SUBDIRS); do \
	    $(RM) $$m/$$m.o; \
	done
	@for o in $(CORE_OBJS); do \
	    $(RM) $$o; \
	done

.PHONY: all libs objects install uninstall unittest clean