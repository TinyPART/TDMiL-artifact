APPLICATION = line_test
# name of the application

# If no BOARD is found in the environment, use this default:
# BOARD ?= arduino-nano-33-ble
BOARD ?= native

# This has to be the absolute path to the TinyPART repo base directory:
TINYPARTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1
# Features required
FEATURES_REQUIRED += cpp # basic C++ support
FEATURES_REQUIRED += libstdcpp # libstdc++ support (for #include <cstdio>)
USEMODULE += cpp_new_delete
USEMODULE += printf_float
USEMODULE += synthetic_dataset
USEMODULE += line_fit_model
USEMODULE += linear_algebra_util
CFLAGS += -DTHREAD_STACKSIZE_MAIN=\(6*THREAD_STACKSIZE_DEFAULT\)

include $(TINYPARTBASE)/Makefile.include

