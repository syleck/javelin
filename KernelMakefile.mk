KERNEL_SOURCES := $(shell find kernel -name *.c -or -name *.s -or -name *.asm)
KERNEL_DISASM  := $(shell find kernel -name *.c)
KERNEL_OBJECTS := $(KERNEL_SOURCES:%=obj/%.o)
KERNEL_DEPS	   := $(KERNEL_OBJECTS:.o=.d)
KERNEL_DISASMS := $(KERNEL_DISASM:%=dsm/%.s)
TARGET_OBJECT  := javelin.bin
TARGET_DIR     := bin
KERNELBUILD_ID := $(shell tr -dc A-Za-z </dev/urandom | head -c 13 ; echo '')
BUILDDATE      := $(shell date -R)
DEFINES        := -DDATE="\"$(BUILDDATE)\""
KERNEL_VERSION := "UNKNOWN"
AFLAGS         := -march=i686 --target=i686-pc-none-elf
CFLAGS		   := -march=i686 --target=i686-pc-none-elf -MMD -MP -DVERSION=\"$(KERNEL_VERSION)\"
LFLAGS		   := -MMD -MP

AS             := clang
CC             := clang 
LD             := i686-elf-gcc

.PHONY: all
all: $(TARGET_DIR)/$(TARGET_OBJECT)

$(TARGET_DIR)/$(TARGET_OBJECT): $(KERNEL_OBJECTS) KernelLink.ld 
	@$(MKDIR_P) $(dir $@)
	@echo "$(LD) Building kernel $(KERNELBUILD_ID) on $(BUILDDATE)"
	@$(LD) $(LFLAGS) -T KernelLink.ld -o bin/javelin.bin -O2 -nostdlib $(KERNEL_OBJECTS)
	@echo "$(KERNEL_OBJECTS) -> $(TARGET_DIR)/$(TARGET_OBJECT)"
	
disassemble: $(KERNEL_DISASMS)

obj/%.asm.o: %.asm n_options.txt 
	@$(MKDIR_P) $(dir $@)
	@nasm $(shell cat n_options.txt) $(NFLAGS) -felf32 -o $@ $<
	@echo "nasm $(BUILDDATE) as $(shell cat n_options.txt) $(NFLAGS) (intel) $<"

obj/%.s.o: %.s a_options.txt 
	@$(MKDIR_P) $(dir $@)
	@$(AS) $(shell cat a_options.txt) $(AFLAGS) -c $< -o $@
	@echo "$(AS) $(BUILDDATE) as $(shell cat a_options.txt) $(AFLAGS) (at&t ) $<"

dsm/%.c.s: %.c
	@$(MKDIR_P) $(dir $@)
	@$(CC) -S $(shell cat c_options.txt) $(CFLAGS) $(DEFINES) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	@echo "$(CC) $(BUILDDATE) parsing $(shell cat c_options.txt) $(DEFINES) $(CFLAGS) $<"

obj/%.c.o: %.c c_options.txt 
	@$(MKDIR_P) $(dir $@)	
	@$(CC) $(shell cat c_options.txt) $(CFLAGS) $(DEFINES) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	@echo "$(CC) $(BUILDDATE) cc $(shell cat c_options.txt) $(DEFINES) $(CFLAGS) $<"

.PHONY: clean
clean: # clear only what we're responsible for
	rm -f bin/javelin.bin
	rm -rf obj/kernel
	rm -rf dsm/kernel
	rm -rf iso
	rm -f javelin.iso

MKDIR_P := mkdir -p
