# --- Variables ---------------------------------------------------------------
CONFIG_COM_PORT_1 = 0x3f8
CONFIG_COM_PORT_2 = 0x2f8
CONFIG_COM_PORT_3 = 0x3e8
CONFIG_COM_PORT_4 = 0x2e8

# --- Compilers ---------------------------------------------------------------
CROSS_CXX := clang++ --target=x86_64-pc-none-elf
CROSS_LD := ld.lld
CROSS_AS := nasm

# --- Emulator ----------------------------------------------------------------
EMU := qemu-system-x86_64

# --- Flags -------------------------------------------------------------------
CROSS_CXXFLAGS := $(CXXFLAGS) -fno-rtti -fno-exceptions                        \
				-mno-sse -mno-sse2 -fno-pic -fno-pie -fno-lto                  \
				-mno-red-zone  -m64 -march=x86-64	                           \
				-mabi=sysv -D__x86_64__   					       			   \
				-D__CONFIG_BAUDS__=$(CONFIG_BAUD_RATE)                         \
				-D__CONFIG_SERIAL__==${CONFIG_COM_PORT_${CONFIG_SERIAL_PORT}}

CROSS_ASFLAGS := -f elf64 -g -F dwarf
CROSS_LDFLAGS := -z max-page-size=0x1000 -z common-page-size=0x1000 -z norelro \
				-T $(SRC_DIR)/arch/x86_64/linker.ld -m elf_x86_64
				
EMUFLAGS := -no-reboot -serial mon:stdio -smp $(CONFIG_CPU_CORE) 			   \
			-m $(CONFIG_MEMORY)m -M q35


# --- Config ------------------------------------------------------------------
IS_BAUD_VALID = $(shell echo $(( $(CONFIG_BAUD_RATE) % 115200 )))
ifeq ($(IS_BAUD_VALID), 0)
$(error "$(CONFIG_BAUD_RATE) bauds is not a valid baud rate")
endif

ifeq ($(CONFIG_ENBABLE_KVM), y)
EMUFLAGS += -enable-kvm -cpu host
endif

ifeq ($(CONFIG_DUMP_INTERRUPT), y)
EMUFLAGS += -d int
endif

ifeq ($(CONFIG_LOADER_EFI), y)
EMUFLAGS += -drive file=fat:rw:$(BOOT_DIR),media=disk,format=raw				\
			-bios $(CONFIG_EFI_BIOS_PATH)
else
EMUFLAGS += -drive format=raw,file=kernel.iso
endif

ifeq ($(CONFIG_NO_POWEROFF), y)
EMUFLAGS += -no-shutdown
endif

# --- Sources -----------------------------------------------------------------
KERNEL_SRC += $(wildcard src/arch/x86_64/*.cpp) $(wildcard src/arch/x86_64/*.s)