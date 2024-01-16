APPLICATION = eigen_test
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
USEMODULE += mlmci
USEPKG += nanocbor
CFLAGS += -DTHREAD_STACKSIZE_MAIN=\(6*THREAD_STACKSIZE_DEFAULT\)
BLOBS+= data/train_iid_wine_norm_0.csv data/train_niid_wine_norm_1.csv data/val_wine_norm.csv data/test_wine_norm.csv

CXXINCLUDES += -I/usr/include/eigen3

include $(TINYPARTBASE)/Makefile.include

