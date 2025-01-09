
export

ROOT_DIR := $(shell pwd)
SCRIPTS_DIR := $(ROOT_DIR)/scripts
MK_SCRIPTS_DIR := $(SCRIPTS_DIR)/make

KLIB_SOURCE_DIR := $(ROOT_DIR)/klib
LIBC_SOURCE_DIR := $(ROOT_DIR)/libc
CRT_SOURCE_DIR := $(ROOT_DIR)/crt

INCLUDE_DIR := $(ROOT_DIR)/include
LIBC_INCLUDE_DIR := $(ROOT_DIR)/include/libc
LINK_DIR := $(ROOT_DIR)/link

SETUPS_DIR := $(ROOT_DIR)/setups
OUTPUT_DIR := $(ROOT_DIR)/build
MODULE_OUTPUT_DIR := $(OUTPUT_DIR)/modules

PYTHON := python3


default:
	@

$(OUTPUT_DIR): FORCE
	$(Q)mkdir -p $@

include $(MK_SCRIPTS_DIR)/include.mk
include $(MK_SCRIPTS_DIR)/config.mk

ifeq ($(findstring config,$(MAKECMDGOALS)),config)
# Don't try to do anything if the goal includes the substring "config"
else
ifndef CONFIG_ELK

default: missing_config_message

missing_config_message: FORCE
	@echo Could Not Find .config File! Run "make menuconfig" or "make defconfig"!

else

ifdef CONFIG_X64
	ARCH := x64
endif

ifdef ARCH
-include $(SCRIPTS_DIR)/arch/$(ARCH)/arch.mk
else
	$(error "No Architecture Specified!")
endif

ifdef CONFIG_CLANG
	TOOLCHAIN := clang
endif
ifdef CONFIG_GCC
	TOOLCHAIN := gcc
endif

ifdef TOOLCHAIN
-include $(SCRIPTS_DIR)/toolchain/$(TOOLCHAIN)/toolchain.mk
else
	$(error "No Toolchain Specified!")
endif

ARCH_SOURCE_DIR := $(ARCH_ROOT_DIR)/$(ARCH)

COMMON_FLAGS += \
				-g \
				-D__ELK_LIBC__\
				-I $(INCLUDE_DIR) \
				-I $(LIBC_INCLUDE_DIR) \
				-I $(ROOT_DIR)/$(subst ",,$(CONFIG_KANAWHA_INCLUDE_PATH)) \
				-include $(AUTOCONF) \
				$(subst ",,$(CONFIG_OPT_FLAGS)) \
				-fno-pie \
				-fno-pic \
				-nostdlib \
				-mgeneral-regs-only \
				-ffreestanding

COMMON_DEPS += $(AUTOCONF)
AFLAGS += -D__ASSEMBLER__

ifdef CONFIG_DEBUG_SYMBOLS
COMMON_FLAGS += -g
endif

LDSCRIPT := $(LINK_DIR)/link.$(ARCH).ld

LDFLAGS += -T $(LDSCRIPT)
LDDEPS += $(LDSCRIPT)

LDFLAGS += $(OUTPUT_DIR)/null.o
LDDEPS += $(OUTPUT_DIR)/null.o
$(OUTPUT_DIR)/null.o: $(SCRIPTS_DIR)/null.c $(CDEPS) $(COMMON_DEPS) | $(OUTPUT_DIR)
	$(call qinfo, CC, $(call rel-dir, $@, $(OUTPUT_DIR)))
	$(Q)$(CC) -c $(CFLAGS) $(COMMON_FLAGS) $< -o $@

$(OUTPUT_DIR)/klib/obj.o: $(LDDEPS) FORCE
	$(Q)$(MAKE) -C $(KLIB_SOURCE_DIR) -f $(MK_SCRIPTS_DIR)/build.mk obj

$(OUTPUT_DIR)/libc/obj.o: $(LDDEPS) FORCE
	$(Q)$(MAKE) -C $(LIBC_SOURCE_DIR) -f $(MK_SCRIPTS_DIR)/build.mk obj

$(OUTPUT_DIR)/crt/crt0-obj.o: $(LDDEPS) FORCE
	$(Q)$(MAKE) -C $(CRT_SOURCE_DIR) -f $(MK_SCRIPTS_DIR)/build.mk crt0-obj

$(OUTPUT_DIR)/crt/crti-obj.o: $(LDDEPS) FORCE
	$(Q)$(MAKE) -C $(CRT_SOURCE_DIR) -f $(MK_SCRIPTS_DIR)/build.mk crti-obj

$(OUTPUT_DIR)/crt/crtn-obj.o: $(LDDEPS) FORCE
	$(Q)$(MAKE) -C $(CRT_SOURCE_DIR) -f $(MK_SCRIPTS_DIR)/build.mk crtn-obj


klib: $(OUTPUT_DIR)/klib.o
$(OUTPUT_DIR)/klib.o: $(OUTPUT_DIR)/klib/obj.o
	$(Q)$(LD) -r $(LDFLAGS) $^ -o $@

libc: $(OUTPUT_DIR)/libc.o
$(OUTPUT_DIR)/libc.o: $(OUTPUT_DIR)/libc/obj.o
	$(Q)$(LD) -r $(LDFLAGS) $^ -o $@

crt0: $(OUTPUT_DIR)/crt0.o
$(OUTPUT_DIR)/crt0.o: $(OUTPUT_DIR)/crt/crt0-obj.o
	$(Q)$(LD) -r $(LDFLAGS) $^ -o $@

crti: $(OUTPUT_DIR)/crti.o
$(OUTPUT_DIR)/crti.o: $(OUTPUT_DIR)/crt/crti-obj.o
	$(Q)$(LD) -r $(LDFLAGS) $^ -o $@

crtn: $(OUTPUT_DIR)/crtn.o
$(OUTPUT_DIR)/crtn.o: $(OUTPUT_DIR)/crt/crtn-obj.o
	$(Q)$(LD) -r $(LDFLAGS) $^ -o $@


default: klib libc crt0 crti crtn

-include $(MK_SCRIPTS_DIR)/qemu.mk
-include $(MK_SCRIPTS_DIR)/asm.mk
-include $(MK_SCRIPTS_DIR)/initrd.mk

clean: FORCE
	$(Q)find $(OUTPUT_DIR) -name "*.o" -delete $(QPIPE) $(QIGNORE)
	$(Q)find $(OUTPUT_DIR) -name "*.d" -delete $(QPIPE) $(QIGNORE)
	$(Q)rm $(AUTOCONF) $(QPIPE) $(QIGNORE)
	$(Q)rm -r $(OUTPUT_DIR) $(QPIPE) $(QIGNORE)

endif
endif

