JPROG_SOURCES := $(shell find elf2jprog -name *.c -or -name *.s -or -name *.asm)
JPROG_DISASM  := $(shell find elf2jprog -name *.c)
JPROG_OBJECTS := $(JPROG_SOURCES:%=obj/%.o)
JPROG_DISASMS := $(JPROG_DISASM:%=dsm/%.s)
TARGET_OBJECT  := elf2jprog
TARGET_DIR     := bin
JPROGBUILD_ID := $(shell tr -dc A-Za-z </dev/urandom | head -c 13 ; echo '')

.PHONY: all
all: $(TARGET_DIR)/$(TARGET_OBJECT)

$(TARGET_DIR)/$(TARGET_OBJECT): $(JPROG_OBJECTS) 
	@$(MKDIR_P) $(dir $@)
	@echo "Building JPROG $(JPROGBUILD_ID)"
	@gcc -o $(TARGET_DIR)/$(TARGET_OBJECT) -O2 $(JPROG_OBJECTS) -lgcc
	@echo "$(JPROG_OBJECTS) -> $(TARGET_DIR)/$(TARGET_OBJECT)"

obj/%.c.o: %.c c_options.txt
	@$(MKDIR_P) $(dir $@)	
	@gcc $(shell cat c_options.txt) $(CFLAGS) -c $< -o $@ -std=gnu99
	@echo "cc $<"

.PHONY: clean
clean: # clear only what we're responsible for
	rm -f bin/elf2jprog
	rm -rf obj/elf2jprog
	rm -rf dsm/elf2jprog

MKDIR_P := mkdir -p
