APPLICATION = line_test
# name of the application

# If no BOARD is found in the environment, use this default:
BOARD ?= arduino-nano-33-ble

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
USEMODULE += coap_ml
USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules
USEMODULE += gnrc_ipv6_default
USEMODULE += gnrc_ipv6_router_default
# Additional networking modules that can be dropped if not needed
USEMODULE += gnrc_icmpv6_echo
USEMODULE += netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += usbus_cdc_ecm
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += ps

include $(TINYPARTBASE)/Makefile.include

