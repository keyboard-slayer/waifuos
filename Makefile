.SUFFIXES:
.DEFAULT_GOAL := all

# --- Variables ---------------------------------------------------------------
export MENUCONFIG_STYLE?=aquatic

# --- Paths -------------------------------------------------------------------
SRC_DIR := ./src
BOOT_DIR := ./.boot
BUILD_DIR := ./build
LIBS_DIR := ./$(SRC_DIR)/libs
TOOLS_DIR := ./tools
RES_DIR := ./res

# --- Functions ---------------------------------------------------------------
MKCWD = @mkdir -p $(@D)
CONF = python $(TOOLS_DIR)/config.py

# --- Default Flags -----------------------------------------------------------
WARNINGS := -Wall -Wextra -Wpedantic -Werror -Wvla -Wshadow -Wno-pointer-arith \
			-Wno-writable-strings
INCLUDES := -I$(SRC_DIR) -I$(LIBS_DIR)
CXXFLAGS := -std=c++2b -MD -O0 -g $(WARNINGS) $(INCLUDES)

# --- Declare source variables ------------------------------------------------
KERNEL_SRC =

# --- Is configured? ----------------------------------------------------------
ifeq (,$(wildcard ./.config))
.PHONY: all
all:
	@echo "Looks like you haven't configured the project yet."
	@echo "Run 'make menuconfig' to configure the project."
	@exit 1
endif

# --- Includes  ---------------------------------------------------------------
-include $(shell find $(BUILD_DIR) -name '*.d')
-include .config.mk

include $(LIBS_DIR)/kawaii-base/.build.mk
include $(SRC_DIR)/kernel/.build.mk

# --- Rules -------------------------------------------------------------------
.PHONY: menuconfig
menuconfig: 
	@$(CONF) --menuconfig
	@$(CONF) --genmake .config.mk

.PHONY: all
all: $(KERNEL)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(BOOT_DIR)

boot_skel:
	@mkdir -p $(BOOT_DIR)/boot $(BOOT_DIR)/EFI/BOOT
	@cp $(KERNEL) $(BOOT_DIR)/boot/kernel.elf
	@cp -r $(RES_DIR)/* $(BOOT_DIR)/

.PHONY: run
run: $(KERNEL) boot_skel $(LOADER_FILE)
	$(EMU) $(EMUFLAGS)
