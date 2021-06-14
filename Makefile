javelin.iso: static/grub.cfg static/isoroot/* bin/javelin.bin bin/shell.elf bin/elf2jprog
	rm -rf iso
	mkdir -p iso/boot/grub
	mkdir -p iso/bin
	cp -r bin/javelin.bin iso/boot/javelin.bin
	cp -r bin/shell.elf iso/bin/shell.elf
#	cp bin/fsdrv.bin iso/fsdrv.bin
	cp -r static/grub.cfg iso/boot/grub/grub.cfg
	cp -r kernel iso/src
	cp -r static/isoroot iso/
	grub-mkrescue -o javelin.iso iso

.PHONY: all
all: javelin.iso

.PHONY: run
run: javelin.iso
	qemu-system-x86_64 -serial stdio -serial null -device adlib javelin.iso 

.PHONY: debug
debug: bin/javelin.bin javelin.iso
	@echo "do gdb -ix GdbCommands to start qemu"
	qemu-system-x86_64 -serial stdio -serial null -device adlib -s -S javelin.iso

.PHONY: bochs
bochs: javelin.iso
	bochs -q

.PHONY: clean
clean:
	@make -f KernelMakefile.mk clean $(ARGS)
	@make -f ShellMakefile.mk clean $(ARGS)
	@make -f Elf2JprogMake.mk clean $(ARGS)
#	make -j4 -f FSDrvMakefile.mk clean $(ARGS)

bin/shell.elf: FORCE
	@make -j4 -f ShellMakefile.mk $(ARGS)

bin/javelin.bin: FORCE
	@make -j4 -f KernelMakefile.mk $(ARGS)

bin/elf2jprog: FORCE
	@make -j4 -f Elf2JprogMake.mk $(ARGS)

.PHONY: FORCE
FORCE:
