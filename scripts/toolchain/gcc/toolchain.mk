
CROSS_COMPILE_PREFIX = $(subst ",,$(CONFIG_GCC_CROSS_COMPILE_PREFIX))
CC := $(CROSS_COMPILE_PREFIX)gcc
CPP := $(CROSS_COMPILE_PREFIX)cpp
LD := $(CROSS_COMPILE_PREFIX)ld
AS := $(CROSS_COMPILE_PREFIX)gcc

OBJCOPY := $(CROSS_COMPILE_PREFIX)objcopy
OBJDUMP := $(CROSS_COMPILE_PREFIX)objdump

