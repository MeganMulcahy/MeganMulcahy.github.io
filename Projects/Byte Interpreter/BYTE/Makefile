BUILD_DIR := build
SOURCE_DIR := .

HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS := $(addprefix $(BUILD_DIR)/$(NAME)/, $(notdir $(SOURCES:.c=.o)))

# Compile the C interpreter.
default:
	@ $(MAKE) -f ./c.make NAME=clox MODE=release SOURCE_DIR=$(SOURCE_DIR)
	@ echo "Building..."
	@ cp build/clox clox
	@ echo "Running..."
	@ ./clox tester.lox
	@ ./clox tester2.lox

# Compile a debug build of clox.
debug:
	@ $(MAKE) -f ./c.make NAME=cloxd MODE=debug SOURCE_DIR=$(SOURCE_DIR)
	@ cp build/cloxd clox
