SHELL_SOURCES := $(shell find shell -name *.c -or -name *.s -or -name *.asm)
SHELL_OBJECTS := $(SHELL_SOURCES:%=obj/%.o)
TARGET_OBJECT  := jsh.elf
TARGET_DIR     := bin
SHELLBUILD_ID := $(shell tr -dc A-Za-z </dev/urandom | head -c 13 ; echo '')
BUILDDATE      := $(shell date)
DEFINES        := -DDATE="\"$(BUILDDATE)\""

.PHONY: all
all: $(TARGET_DIR)/$(TARGET_OBJECT)

$(TARGET_DIR)/$(TARGET_OBJECT): $(SHELL_OBJECTS)
	@$(MKDIR_P) $(dir $@)
	@echo "Building SHELL $(SHELLBUILD_ID)"
	@i686-elf-gcc -fno-omit-frame-pointer -o bin/shell.elf -ffreestanding -O2 -nostdlib $(SHELL_OBJECTS) -lgcc
	@echo "$(SHELL_OBJECTS) -> $(TARGET_DIR)/$(TARGET_OBJECT)"
	
obj/%.asm.o: %.asm n_options.txt
	@$(MKDIR_P) $(dir $@)
	@nasm $(shell cat n_options.txt) $(NFLAGS) -felf32 -o $@ $<
	@echo "as (intel) $<"

obj/%.s.o: %.s a_options.txt 
	@$(MKDIR_P) $(dir $@)
	@i686-elf-as $(shell cat a_options.txt) -fpic $(AFLAGS) $(DEFINES) -c $< -o $@
	@echo "as (at&t ) $<"

obj/%.c.o: %.c c_options.txt 
	@$(MKDIR_P) $(dir $@)
	@i686-elf-gcc $(shell cat c_options.txt) -fpic $(CFLAGS) $(DEFINES) -Q -fstack-usage -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	@echo "cc $<"

.PHONY: clean
clean: # clear only what we're responsible for
	rm -rf obj/shell
	rm -f  bin/shell.elf

MKDIR_P := mkdir -p
