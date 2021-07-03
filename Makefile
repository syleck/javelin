KERNEL_VERSION	:= \"0.1.2-$(shell uuidgen -t)\"
QEMUARGS		:= -m 2G

javelin.iso: static/grub.cfg static/isoroot/* bin/javelin.bin bin/shell.elf bin/elf2jprog udevprojs
	@echo "Determining if grub multiboot"
	grub-file --is-x86-multiboot2 bin/javelin.bin
	rm -rf iso
	mkdir -p iso/boot/grub
	mkdir -p iso/bin
	cp udevs/*.dev iso/
	cp -r bin/javelin.bin iso/boot/javelin.bin
	cp -r bin/shell.elf iso/bin/shell.elf
#	cp bin/fsdrv.bin iso/fsdrv.bin
	cp -r static/grub.cfg iso/boot/grub/grub.cfg
	cp -r kernel iso/src
	cp -r static/isoroot iso/
	grub-mkrescue -o javelin.iso iso
	@echo JAVELIN version $(KERNEL_VERSION) finished compiling!

.PHONY: all
all: javelin.iso

.PHONY: run
run: javelin.iso
	qemu-system-i386 $(QEMUARGS) -serial stdio -serial null -device adlib javelin.iso 

.PHONY: debug
debug: bin/javelin.bin javelin.iso
	@echo "do gdb -ix GdbCommands to start qemu"
	qemu-system-i386 $(QEMUARGS) -serial stdio -serial null -device adlib -s -S javelin.iso

.PHONY: bochs
bochs: javelin.iso
	bochs -q

.PHONY: clean
clean:
	@make -f KernelMakefile.mk clean $(ARGS)
	@make -f ShellMakefile.mk clean $(ARGS)
	@make -f Elf2JprogMake.mk clean $(ARGS)
	@cd udevs && make -f Makefile clean $(ARGS)
	rm -rf iso
#	make -j4 -f FSDrvMakefile.mk clean $(ARGS)

bin/shell.elf: FORCE
	@make KERNEL_VERSION=$(KERNEL_VERSION) -f ShellMakefile.mk $(ARGS)

bin/javelin.bin: FORCE
	@make KERNEL_VERSION=$(KERNEL_VERSION) -f KernelMakefile.mk $(ARGS)

bin/elf2jprog: FORCE
	@make KERNEL_VERSION=$(KERNEL_VERSION) -f Elf2JprogMake.mk $(ARGS)

udevprojs:	FORCE
	@cd udevs && make KERNEL_VERSION=$(KERNEL_VERSION) -f Makefile $(ARGS)

.PHONY: FORCE
FORCE:

docs:
	doxygen