# Define sanitizer targets
define sanitizer_target
$(1): CFLAGS_MAIN += -fsanitize=$(2) -O0
$(1): LDFLAGS += -fsanitize=$(2)
$(1): $(TARGET)
	$(Q)./$(TARGET) > $(BINARY)_$(1).log 2>&1 || { echo "$(1) build failed"; exit 1; }
$(1)_test: $(1)
	$(Q)./$(TEST_TARGET) || { echo "Test failed with $(1)"; exit 1; }
endef

# Compiler and flags
CC ?= $(shell command -v gcc || command -v clang)
CFLAGS_MAIN ?= -Wall -Wextra -I$(INCLUDEDIR) -MMD -MP
CFLAGS_TEST ?= $(CFLAGS_MAIN) -g -DTESTING
LDFLAGS ?= -lm

# Build mode
MODE ?= release
ifeq ($(MODE),debug)
    CFLAGS_MAIN += -g
else
    CFLAGS_MAIN += -O2
endif

# Directories
SRCDIR := src
BUILDDIR := build
BUILDDIR_MAIN := $(BUILDDIR)/main
BUILDDIR_TEST := $(BUILDDIR)/test
BINDIR := bin
INCLUDEDIR := include
TESTDIR := tests

# Target
PREFIX ?= /usr/local
BINARY ?= srxe
TARGET := $(BINDIR)/$(BINARY)

# Source files
SRCS := $(shell find $(SRCDIR) -name "*.c")
OBJS := $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR_MAIN)/%.o)
TEST_SRCS := $(shell find $(TESTDIR) -name "*.c")
TEST_OBJS := $(TEST_SRCS:$(TESTDIR)/%.c=$(BUILDDIR_TEST)/%.o)
TEST_TARGET := $(BINDIR)/tests

# Utilities
MKDIR_P := mkdir -p
RM := rm -rf
VERBOSE ?= false

ifeq ($(VERBOSE),true)
    $(info Running in verbose mode)
    Q :=
else
    Q := @
endif

# Phony targets
.PHONY: all debug release install help
.PHONY: clean distclean 
.PHONY: valgrind helgrind drd
.PHONY: asan ubsan tsan msan 
.PHONY: sanitizers check test
.PHONY: $(TARGET) $(TEST_TARGET)

# Main targets
all: $(TARGET)

debug: MODE := debug
debug: all

release: MODE := release
release: all

$(TARGET): $(OBJS) | $(BINDIR)
	$(Q)command -v $(CC) >/dev/null 2>&1 || { echo >&2 "$(CC) not found. Aborting."; exit 1; }
	$(Q)$(CC) $(CFLAGS_MAIN) -o $@ $^ $(LDFLAGS)

$(BINDIR):
	$(Q)$(MKDIR_P) $(BINDIR)

$(BUILDDIR_MAIN):
	$(Q)$(MKDIR_P) $(BUILDDIR_MAIN)

$(BUILDDIR_TEST):
	$(Q)$(MKDIR_P) $(BUILDDIR_TEST)

$(BUILDDIR_MAIN)/%.o: $(SRCDIR)/%.c | $(BUILDDIR_MAIN)
	$(Q)$(CC) $(CFLAGS_MAIN) -c $< -o $@

$(BUILDDIR_TEST)/%.o: $(TESTDIR)/%.c | $(BUILDDIR_TEST)
	$(Q)$(CC) $(CFLAGS_TEST) -c $< -o $@

$(TEST_TARGET): $(OBJS) $(TEST_OBJS) | $(BINDIR)
	$(Q)$(CC) $(CFLAGS_TEST) -o $@ $^ $(LDFLAGS)

$(BINDIR) $(BUILDDIR_MAIN) $(BUILDDIR_TEST):
	$(Q)$(MKDIR_P) $@

# Cleaning targets
clean:
	$(Q)$(RM) $(BUILDDIR)/*.o $(BINDIR)/$(BINARY)

distclean: clean
	$(Q)find $(BUILDDIR) -name "*.d" -o -name "*.log" -type f -delete

# Testing and analysis targets
test: $(TEST_TARGET)
	$(Q)./$(TEST_TARGET) || { echo "Tests failed"; exit 1; }

valgrind: $(TEST_TARGET)
	$(Q)command -v valgrind >/dev/null 2>&1 || { echo >&2 "valgrind not found. Aborting."; exit 1; }
	$(Q)valgrind --leak-check=full ./$(TEST_TARGET)

# Sanitizer targets
$(eval $(call sanitizer_target,asan,address))
$(eval $(call sanitizer_target,ubsan,undefined))
$(eval $(call sanitizer_target,tsan,thread))
$(eval $(call sanitizer_target,msan,memory))

sanitizers: asan_test ubsan_test tsan_test msan_test
	@echo "Sanitizer checks and tests completed."

helgrind: $(TARGET)
	$(Q)command -v valgrind >/dev/null 2>&1 || { echo >&2 "valgrind not found. Aborting."; exit 1; }
	$(Q)valgrind --tool=helgrind ./$(TARGET)

drd: $(TARGET)
	$(Q)command -v valgrind >/dev/null 2>&1 || { echo >&2 "valgrind not found. Aborting."; exit 1; }
	$(Q)valgrind --tool=drd ./$(TARGET)

check: all test sanitizers
	@echo "All checks completed."

# Installation
install: $(TARGET)
	$(Q)install -d $(PREFIX)/bin
	$(Q)install $(TARGET) $(PREFIX)/bin/

# Help
help:
	@echo "Usage: make [-j =1] [target] [VERBOSE=true]"
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove build artifacts"
	@echo "  distclean  - Remove build artifacts and generated files"
	@echo "  debug      - Build with debug symbols"
	@echo "  release    - Build optimized release version"
	@echo "  test       - Build and run tests"
	@echo "  valgrind   - Run tests with Valgrind"
	@echo "  asan       - Build with AddressSanitizer"
	@echo "  ubsan      - Build with UndefinedBehaviorSanitizer"
	@echo "  tsan       - Build with ThreadSanitizer"
	@echo "  msan       - Build with MemorySanitizer"
	@echo "  sanitizers - Run all sanitizers"
	@echo "  install    - Install the program to $(PREFIX)"
	@echo "  check      - Run all tests and sanitizers"
	@echo ""
	@echo "Options:"
	@echo "  VERBOSE=true  - Display commands being executed"

# Include dependency files
-include $(OBJS:.o=.d)
-include $(TEST_OBJS:.o=.d)
