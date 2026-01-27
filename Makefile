# Top-level Makefile

# ---------------- OS detection (keep yours if you want) ----------------
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

# ---------------- Tooling ----------------
CC      := gcc
RM      := rm -f
MKDIR_P := mkdir -p

LIBOUTDIR := lib

LIBDIR           := /usr/local/lib
PATH_INCLUDEDIR  := /usr/local/include/cs
LOCAL_INCLUDEDIR := include

# Don't put -lm in CFLAGS; keep link libs in LDLIBS
CFLAGS  := -Wall -Wextra -fPIC -Wno-unknown-pragmas
LDLIBS  := -lm

UNITTEST_LOG := unittest_log.ansi
SEED ?= 42

ARGS = --seed $(SEED) --debug-file $(UNITTEST_LOG)

ifeq ($(debug),true)
  CFLAGS += -DDEBUG
endif

# ---------------- Modules ----------------
# All modules that exist in the repo
SUBDIRS := cargs pair vector deque list forward_list set map unordered_set unordered_map stack multiset multimap unordered_multiset unordered_multimap queue priority_queue flat_set

# Only these are built/installed/tested as shared libs
INSTALL_LIBS := cargs pair vector deque list forward_list set map unordered_set unordered_map stack multiset multimap unordered_multiset unordered_multimap queue priority_queue flat_set
# ---------------- Core/Dependency objects ----------------
# Any objects you want to be able to link into other libs (but not necessarily installed as libs)
CORE_OBJS := rbt/rbt.o pair/pair.o hash_table/hash_table.o

# Per-module extra object deps (link-time deps)
DEPS_rbt		   		:= vector/vector.o
DEPS_map           		:= rbt/rbt.o pair/pair.o
DEPS_set           		:= rbt/rbt.o
DEPS_stack		   		:= vector/vector.o deque/deque.o list/list.o
DEPS_queue		   		:= deque/deque.o list/list.o
DEPS_priority_queue		:= vector/vector.o deque/deque.o
DEPS_unordered_set 		:= hash_table/hash_table.o vector/vector.o
DEPS_unordered_map 		:= hash_table/hash_table.o vector/vector.o pair/pair.o
DEPS_multiset      		:= rbt/rbt.o pair/pair.o
DEPS_multimap      		:= rbt/rbt.o pair/pair.o vector/vector.o
DEPS_unordered_multiset := hash_table/hash_table.o pair/pair.o vector/vector.o
DEPS_unordered_multimap := hash_table/hash_table.o pair/pair.o vector/vector.o
DEPS_flat_set       	:= vector/vector.o deque/deque.o
# ---------------- Derived paths ----------------
# Main object for each module is module/module.o
MOD_OBJ = $1/$1.o

# Objects for all modules (compiled up-front)
ALL_MOD_OBJS := $(foreach m,$(SUBDIRS),$(call MOD_OBJ,$(m)))

# Shared libs we actually build/install
LIBS := $(foreach m,$(INSTALL_LIBS),$(LIBOUTDIR)/lib$(m).so)

# ---------------- Default target ----------------
all: clean uninstall install unittest

# ---------------- Output dir ----------------
$(LIBOUTDIR):
	$(MKDIR_P) $(LIBOUTDIR)

# ---------------- Build ALL .o (once) ----------------
# Generic rule: build module/module.o from module/module.c
%/%.o: %/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Build core objects (rbt/pair/hash_table) via the same pattern above
# (No special rules needed as long as paths are <dir>/<dir>.c)

# Convenience target if you want just objects
objects: $(ALL_MOD_OBJS) $(CORE_OBJS)

# ---------------- Link only requested libs ----------------
# Template to link lib<name>.so from <name>.o plus any DEPS_<name>
define LINK_SO_RULE
$(LIBOUTDIR)/lib$1.so: $(call MOD_OBJ,$1) $$(DEPS_$1) | $(LIBOUTDIR)
	$$(CC) -shared -o $$@ $$^ $$(CFLAGS) $$(LDLIBS)
endef
$(foreach m,$(INSTALL_LIBS),$(eval $(call LINK_SO_RULE,$(m))))

# ---------------- Install / Uninstall ----------------
install: install_headers install_libs
uninstall: uninstall_headers uninstall_libs

install_headers:
	$(MKDIR_P) $(PATH_INCLUDEDIR)
	cp $(LOCAL_INCLUDEDIR)/universal.h $(PATH_INCLUDEDIR)
	@for h in $(INSTALL_LIBS); do \
		cp $(LOCAL_INCLUDEDIR)/$$h.h $(PATH_INCLUDEDIR); \
	done
	cp $(LOCAL_INCLUDEDIR)/rbt.h $(PATH_INCLUDEDIR)
	cp $(LOCAL_INCLUDEDIR)/hash_table.h $(PATH_INCLUDEDIR)

install_libs: $(LIBS)
	$(MKDIR_P) $(LIBDIR)
	@for m in $(INSTALL_LIBS); do \
		cp $(LIBOUTDIR)/lib$$m.so $(LIBDIR); \
	done
	ldconfig

uninstall_headers:
	@for h in $(INSTALL_LIBS); do \
		$(RM) $(PATH_INCLUDEDIR)/$$h.h; \
	done
	$(RM) $(PATH_INCLUDEDIR)/universal.h
	rmdir --ignore-fail-on-non-empty $(PATH_INCLUDEDIR) || true

uninstall_libs:
	@for m in $(INSTALL_LIBS); do \
		$(RM) $(LIBDIR)/lib$$m.so; \
	done
	ldconfig

# ---------------- Unit tests (only for installed libs) ----------------
UNITTEST_SRC = $1/$1_unittest.c

# Modules that actually have unit tests present
TEST_MODULES := $(foreach m,$(INSTALL_LIBS),$(if $(wildcard $(call UNITTEST_SRC,$(m))),$(m),))

# Modules without a unittest source (will be skipped)
SKIP_MODULES := $(filter-out $(TEST_MODULES),$(INSTALL_LIBS))

unittest-log-init:
	@$(RM) $(UNITTEST_LOG)
	@touch $(UNITTEST_LOG)

define MODULE_TEST_RULES
unittest-$1: install $(call UNITTEST_SRC,$1)
	@echo "Building unit tests for $1..."
	$(CC) -o $1/unittest $(call UNITTEST_SRC,$1) $(CFLAGS) -l$1 $(LDLIBS) -lcargs
	@MODARGS="$(ARGS) --module $1"; \
	if [ "$(memcheck)" = "true" ]; then \
	  echo "[RUN][$1] valgrind --leak-check=full $1/unittest $$$$MODARGS"; \
	  /usr/bin/time -f "[TIME][$1] %E real, %U user, %S sys" \
	    valgrind --leak-check=full \
		$1/unittest $$$$MODARGS 2>&1 | tee -a $(UNITTEST_LOG) ; \
	else \
	  echo "[RUN][$1] $1/unittest $$$$MODARGS"; \
	  /usr/bin/time -f "[TIME][$1] %E real, %U user, %S sys" \
	    $1/unittest $$$$MODARGS 2>&1 | tee -a $(UNITTEST_LOG) ; \
	fi
	@$(RM) $1/unittest
endef

define MODULE_TEST_SKIP_RULES
unittest-$1:
	@echo "Skipping unit tests for $1 (no $(call UNITTEST_SRC,$1) found)."
endef

# Create real test rules only for modules that have the source file
$(foreach m,$(TEST_MODULES),$(eval $(call MODULE_TEST_RULES,$(m))))

# Create "skip" rules for modules without tests
$(foreach m,$(SKIP_MODULES),$(eval $(call MODULE_TEST_SKIP_RULES,$(m))))

# unittest target depends on all modules (some will run, some will skip)
unittest: unittest-log-init $(INSTALL_LIBS:%=unittest-%)

# ---------------- Clean ----------------
clean:
	$(RM) $(UNITTEST_LOG)
	$(RM) $(LIBOUTDIR)/lib*.so
	@for m in $(SUBDIRS); do \
		$(RM) $$m/$$m.o $$m/unittest; \
	done
	@for o in $(CORE_OBJS); do \
		$(RM) $$o; \
	done

.PHONY: all objects install uninstall unittest clean
