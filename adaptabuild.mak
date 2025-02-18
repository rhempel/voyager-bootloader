# ----------------------------------------------------------------------------
# voyager-bootloader makefile for adaptabuild
#
# This is designed to be included as part of a make system designed
# to be expandable and maintainable using techniques found in:
#
# Managing Projects with GNU Make - Robert Mecklenburg - ISBN 0-596-00610-1
# ----------------------------------------------------------------------------

MODULE := voyager-bootloader

MODULE_PATH := $(call make_current_module_path)
$(call log_debug,MODULE_PATH is $(MODULE_PATH))

$(MODULE)_PATH := $(MODULE_PATH)
$(call log_debug,$(MODULE)_PATH is $($(MODULE)_PATH))

# ----------------------------------------------------------------------------
# Source file lists go here, C dependencies are automatically generated
# by the compiler using the -m option
#
# You can set up a common source path late in the file
#
# Note that each module gets its own, privately scoped variable for building
# ----------------------------------------------------------------------------

# We need both else a previous definition is used :-) Can we make this an include?

SRC_C :=
SRC_ASM :=
SRC_TEST :=

# Here is where we begin to add files to list of sources

SRC_C += src/voyager.c

SRC_TEST += test/test_api.cpp
SRC_TEST += test/test_bootloader.cpp
SRC_TEST += test/test_defaults.cpp
SRC_TEST += test/test_dfu.cpp
SRC_TEST += test/dummy_test.cpp
SRC_TEST += test/main.cpp

SRC_TEST += test/mocks/mock_dfu.c
SRC_TEST += test/mocks/mock_nvm.c

# ----------------------------------------------------------------------------
# Set up the module level include path

$(MODULE)_INCPATH :=
$(MODULE)_INCPATH += $(MODULE_PATH)/inc

ifeq (unittest,$(MAKECMDGOALS))
  $(MODULE)_INCPATH += $(MODULE_PATH)/test/mocks
  $(MODULE)_INCPATH += $(MODULE_PATH)/utils/host
endif

# ----------------------------------------------------------------------------
# NOTE: The default config file must be created somehow - it is normally
#       up to the developer to specify which defines are needed and how they
#       are to be configured.
#
# By convention we place config files in $(PRODUCT)/config/$(MCU) because
# that's an easy pace to leave things like HAL config, linker scripts etc

$(MODULE)_INCPATH += $(PRODUCT)/config/$(MCU)

# ----------------------------------------------------------------------------
# Set any module level compile time defaults here

$(MODULE)_CDEFS :=
$(MODULE)_CDEFS +=

$(MODULE)_CFLAGS :=
$(MODULE)_CFLAGS +=

ifeq (unittest,$(MAKECMDGOALS))
  $(MODULE)_CDEFS += VOYAGER_UNIT_TEST
endif

# ----------------------------------------------------------------------------
# Include the adaptabuild library makefile - must be done for each module!

include $(ADAPTABUILD_PATH)/make/library.mak

# ----------------------------------------------------------------------------
# Include the unit test framework makefile that works for this module
# if the target is unittest

# ifeq (unittest,$(MAKECMDGOALS))
#   TESTABLE_MODULES += $(MODULE)_UNITTEST
#   $(MODULE)_test_main := test/main.o
#  include $(ADAPTABUILD_PATH)/make/test/cpputest.mak
# endif

# ----------------------------------------------------------------------------
