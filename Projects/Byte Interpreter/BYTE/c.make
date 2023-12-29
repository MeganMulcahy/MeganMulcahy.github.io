# Makefile for building a single configuration of the C interpreter. It expects
# variables to be passed in for:
#
# MODE         "debug" or "release".
# NAME         Name of the output executable (and object file directory).
# SOURCE_DIR   Directory where root files and headers are found.

CFLAGS := -std=c99 -Wall -Wextra -Werror -Wno-unused-parameter

# If we're building at a point in the middle of a chapter, don't fail if there
# are functions that aren't used yet.
ifeq ($(SNIPPET),true)
	CFLAGS += -Wno-unused-function
endif

# Mode configuration.
ifeq ($(MODE),debug)
	CFLAGS += -arch arm64
	BUILD_DIR := build/debug
else
	CFLAGS += -arch arm64
	BUILD_DIR := build/release
endif

# Files.
HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS := $(addprefix $(BUILD_DIR)/$(NAME)/, $(notdir $(SOURCES:.c=.o)))
TESTER := tester.c  # Change this to the actual name of your tester file

# Targets ---------------------------------------------------------------------

# Link the interpreter.
build/$(NAME): $(OBJECTS)
	@ printf "%8s %-40s %s\n" $(CC) $@ "$(CFLAGS)"
	@ mkdir -p build
	@ $(CC) $(CFLAGS) $^ -o $@

# Compile object files.
$(BUILD_DIR)/$(NAME)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	@ printf "%8s %-40s %s\n" $(CC) $< "$(CFLAGS)"
	@ mkdir -p $(BUILD_DIR)/$(NAME)
	@ $(CC) -c $(CFLAGS) -o $@ $<

# Build and run with the tester file.
test: build/$(NAME)
	@ printf "%8s %-40s %s\n" "./build/$(NAME)" "Running tester..."
	@ ./build/$(NAME) $(TESTER)

.PHONY: default test
