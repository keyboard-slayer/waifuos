# --- Variables ---------------------------------------------------------------
LIMINE_VER = 4.3

# --- Config ------------------------------------------------------------------
ifeq ($(CONFIG_LOADER_EFI), y)
	LOADER_FILE = $(BOOT_DIR)/EFI/BOOT/BOOTX64.EFI
else
	$(error "Unsupported loader")
endif

# --- Files -------------------------------------------------------------------
KERNEL_SRC += $(wildcard $(LIBS_DIR)/kawaii-abstract/limine/*.cpp)

# --- Rules -------------------------------------------------------------------
$(BOOT)/EFI/BOOT/BOOTX64.EFI: $(BOOT)/boot/limine.cfg
	wget https://github.com/limine-bootloader/limine/raw/v$(LIMINE_VER)-binary/BOOTX64.EFI -O $(BOOT_DIR)/EFI/BOOT/BOOTX64.EFI

./limine-deploy.c:
	wget https://github.com/limine-bootloader/limine/raw/v$(LIMINE_VER)-binary/limine-deploy.c -O limine-deploy.c

./limine-hhd.h:
	wget https://github.com/limine-bootloader/limine/raw/v$(LIMINE_VER)-binary/limine-hdd.h -O limine-hdd.h

./limine-deploy: ./limine-deploy.c ./limine-hhd.h
	@cc -g -o2 -pipe -Wall -Wextra -std=c99 -D__USE_MINGW_ANSI_STDIO limine-deploy.c -o ./limine-deploy
	@rm limine-deploy.c limine-hdd.h

$(BOOT_DIR)/boot/limine-cd.bin:
	wget https://github.com/limine-bootloader/limine/raw/v$(LIMINE_VER)-binary/limine-cd.bin -O $(BOOT_DIR)/boot/limine-cd.bin

$(BOOT_DIR)/boot/limine-cd-efi.bin:
	wget https://github.com/limine-bootloader/limine/raw/v$(LIMINE_VER)-binary/limine-cd-efi.bin -O $(BOOT_DIR)/boot/limine-cd-efi.bin

$(BOOT_DIR)/boot/limine.sys:
	wget https://github.com/limine-bootloader/limine/raw/v$(LIMINE_VER)-binary/limine.sys -O $(BOOT_DIR)/boot/limine.sys

$(BOOT_DIR)/EFI/BOOT/BOOTX64.EFI: $(BOOT_DIR)/boot/limine.cfg
	wget https://github.com/limine-bootloader/limine/raw/v$(LIMINE_VER)-binary/BOOTX64.EFI -O $(BOOT_DIR)/EFI/BOOT/BOOTX64.EFI

$(BOOT_DIR)/boot/limine.cfg:
	@cp $(LIBS_DIR)/kawaii-abstract/limine/limine.cfg $(BOOT_DIR)/boot/limine.cfg

kernel.iso: $(BOOT_DIR)/boot/limine.cfg ./limine-deploy $(BOOT_DIR)/boot/limine.sys $(BOOT_DIR)/boot/limine-cd.bin $(BOOT_DIR)/boot/limine-cd-efi.bin
	xorriso -as mkisofs -b /boot/limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot /boot/limine-cd-efi.bin -efi-boot-part --efi-boot-image --protective-msdos-label \
		$(BOOT_DIR) -o kernel.iso
	
	./limine-deploy kernel.iso
	@rm limine-deploy