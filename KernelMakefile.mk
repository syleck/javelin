KERNEL_SOURCES := $(shell find kernel -name *.c -or -name *.s -or -name *.asm)
KERNEL_DISASM  := $(shell find kernel -name *.c)
KERNEL_OBJECTS := $(KERNEL_SOURCES:%=obj/%.o)
KERNEL_DISASMS := $(KERNEL_DISASM:%=dsm/%.s)
TARGET_OBJECT  := javelin.bin
TARGET_DIR     := bin
KERNELBUILD_ID := $(shell tr -dc A-Za-z </dev/urandom | head -c 13 ; echo '')

.PHONY: all
all: $(TARGET_DIR)/$(TARGET_OBJECT)

$(TARGET_DIR)/$(TARGET_OBJECT): $(KERNEL_OBJECTS) KernelLink.ld 
	@$(MKDIR_P) $(dir $@)
	@echo "Building kernel $(KERNELBUILD_ID)"
	@i686-elf-gcc -T KernelLink.ld -fno-omit-frame-pointer -o bin/javelin.bin -ffreestanding -O2 -nostdlib $(KERNEL_OBJECTS) -lgcc
	@echo "$(KERNEL_OBJECTS) -> $(TARGET_DIR)/$(TARGET_OBJECT)"
	@echo "Determining if grub multiboot"
	grub-file --is-x86-multiboot2 bin/javelin.bin
	
disassemble: $(KERNEL_DISASMS)

obj/%.asm.o: %.asm n_options.txt KernelLink.ld 
	@$(MKDIR_P) $(dir $@)
	@nasm $(shell cat n_options.txt) $(NFLAGS) -felf32 -o $@ $<
	@echo "as (intel) $<"

obj/%.s.o: %.s a_options.txt KernelLink.ld 
	@$(MKDIR_P) $(dir $@)
	@i686-elf-as $(shell cat a_options.txt) $(AFLAGS) -c $< -o $@
	@echo "as (at&t ) $<"

dsm/%.c.s: %.c
	@$(MKDIR_P) $(dir $@)
	@i686-elf-gcc -S $(shell cat c_options.txt) $(CFLAGS) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	@echo "parsing $<"

obj/%.c.o: %.c c_options.txt KernelLink.ld 
	@$(MKDIR_P) $(dir $@)	
	@i686-elf-gcc $(shell cat c_options.txt) $(CFLAGS) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	@echo "cc $<"

.PHONY: clean
clean: # clear only what we're responsible for
	rm -f bin/javelin.bin
	rm -rf obj/kernel
	rm -rf dsm/kernel
	rm -rf iso
	rm -f javelin.iso

MKDIR_P := mkdir -p
