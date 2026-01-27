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
CFLAGS  := -Wall -Wextra -fPIC -Wno-unknown-pragmas -I$(LOCAL_INCLUDEDIR)
LDLIBS  := -lm

# Unittest Configuration
UNITTEST_LOG := unittest_log.ansi
SEED ?= 42
ARGS = --seed $(SEED) --debug-file $(UNITTEST_LOG)

ifeq ($(debug),true)
  CFLAGS += -DDEBUG
endif

# ---------------- Modules ----------------
SUBDIRS := cargs pair vector deque list forward_list set map unordered_set \
           unordered_map stack multiset multimap unordered_multiset \
           unordered_multimap queue priority_queue flat_set

INSTALL_LIBS := $(SUBDIRS)

# ---------------- Core/Dependency objects ----------------
CORE_OBJS := rbt/rbt.o pair/pair.o hash_table/hash_table.o

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

# Convenience target for objects
objects: $(ALL_MOD_OBJS) $(CORE_OBJS)

# ---------------- Linker Logic ----------------

define LINK_SO_RULE
$(LIBOUTDIR)/lib$1.so: $(call MOD_OBJ,$1) $$(DEPS_$1) | $(LIBOUTDIR)
	$$(CC) -shared -o $$@ $$^ $$(CFLAGS) $$(LDLIBS)
endef
$(foreach m,$(INSTALL_LIBS),$(eval $(call LINK_SO_RULE,$(m))))

# ---------------- Installation (Requires Sudo) ----------------

install: libs install_headers install_libs

install_headers:
	@echo "Installing headers to $(PATH_INCLUDEDIR)..."
	$(MKDIR_P) $(PATH_INCLUDEDIR)
	cp $(LOCAL_INCLUDEDIR)/universal.h $(PATH_INCLUDEDIR)
	@for h in $(INSTALL_LIBS); do \
	    if [ -f $(LOCAL_INCLUDEDIR)/$$h.h ]; then \
	        cp $(LOCAL_INCLUDEDIR)/$$h.h $(PATH_INCLUDEDIR); \
	    fi \
	done
	cp $(LOCAL_INCLUDEDIR)/rbt.h $(PATH_INCLUDEDIR)
	cp $(LOCAL_INCLUDEDIR)/hash_table.h $(PATH_INCLUDEDIR)

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
	@for m in $(INSTALL_LIBS); do $(RM) $(LIBDIR)/lib$$m.so; done
	rmdir --ignore-fail-on-non-empty $(PATH_INCLUDEDIR) || true
	ldconfig

# ---------------- Unit Testing ----------------

UNITTEST_SRC = $1/$1_unittest.c
TEST_MODULES := $(foreach m,$(INSTALL_LIBS),$(if $(wildcard $(call UNITTEST_SRC,$(m))),$(m),))
SKIP_MODULES := $(filter-out $(TEST_MODULES),$(INSTALL_LIBS))

unittest-log-init:
	@$(RM) $(UNITTEST_LOG)
	@touch $(UNITTEST_LOG)

define MODULE_TEST_RULES
unittest-$1: libs $(call UNITTEST_SRC,$1)
	@echo "Building unit test: $1..."
	$(CC) -o $1/unittest $(call UNITTEST_SRC,$1) $(CFLAGS) \
	    -L$(LIBOUTDIR) -l$1 -lcargs $(LDLIBS)
	
	@MODARGS="$(ARGS) --module $1"; \
	export LD_LIBRARY_PATH=$$(shell pwd)/$(LIBOUTDIR):$$LD_LIBRARY_PATH; \
	if [ "$(memcheck)" = "true" ]; then \
	  echo "[RUNNING VALGRIND] $1/unittest"; \
	  valgrind --leak-check=full $1/unittest $$$$MODARGS 2>&1 | tee -a $(UNITTEST_LOG) ; \
	else \
	  echo "[RUNNING] $1/unittest"; \
	  $1/unittest $$$$MODARGS 2>&1 | tee -a $(UNITTEST_LOG) ; \
	fi
	@$(RM) $1/unittest
endef

define MODULE_TEST_SKIP_RULES
unittest-$1:
	@echo "Skipping $1 (no test file found)."
endef

$(foreach m,$(TEST_MODULES),$(eval $(call MODULE_TEST_RULES,$(m))))
$(foreach m,$(SKIP_MODULES),$(eval $(call MODULE_TEST_SKIP_RULES,$(m))))

unittest: unittest-log-init libs $(INSTALL_LIBS:%=unittest-%)

# ---------------- Cleanup ----------------

clean:
	@echo "Cleaning build files..."
	$(RM) $(UNITTEST_LOG)
	$(RM) $(LIBOUTDIR)/*.so
	@for m in $(SUBDIRS); do \
	    $(RM) $$m/$$m.o $$m/unittest; \
	done
	@for o in $(CORE_OBJS); do \
	    $(RM) $$o; \
	done

.PHONY: all libs objects install uninstall unittest clean