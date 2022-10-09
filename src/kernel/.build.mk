# --- Variables ---------------------------------------------------------------
KERNEL = $(BUILD_DIR)/kernel.elf
KERNEL_BUILD = $(BUILD_DIR)/kernel

# --- Flags -------------------------------------------------------------------
KERNEL_CXXFLAGS = $(CROSS_CXXFLAGS) -fno-builtin -fno-stack-protector 		   \
				-fno-stack-check -nostdlib -fno-zero-initialized-in-bss 	   \
				-mcmodel=kernel -D__KERNEL__

KERNEL_LDFLAGS = $(CROSS_LDFLAGS) -static --no-undefined -no-pie

# --- Files -------------------------------------------------------------------
KERNEL_SRC += $(wildcard src/kernel/*.cpp)
KERNEL_OBJ = $(patsubst %, $(KERNEL_BUILD)/%.o, $(KERNEL_SRC))

# --- Rules -------------------------------------------------------------------
$(KERNEL_BUILD)/%.cpp.o: %.cpp
	@$(MKCWD)
	$(CROSS_CXX) $(KERNEL_CXXFLAGS) -c $< -o $@

$(KERNEL_BUILD)/%.s.o: %.s
	@$(MKCWD)
	$(CROSS_AS) $(KERNEL_ASFLAGS) -o $@ $<

$(KERNEL): $(KERNEL_OBJ)
		@$(MKCWD)
		$(CROSS_LD) $(KERNEL_LDFLAGS) -o $@ $^