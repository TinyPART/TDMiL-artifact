#model control interface
# INCLUDES += -I$(CURDIR)/include
PARAMS_PERSISTENT_ON_FLASH ?= 0

ifeq ($(PARAMS_PERSISTENT_ON_FLASH), 1)
  CFLAGS += -DCONFIG_PARAMS_PERSISTENT_ON_FLASH
endif

include $(RIOTBASE)/Makefile.base
