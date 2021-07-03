CFLAGS	:= -march=i686 --target=i686-pc-none-elf -DVERSION=\"$(KERNEL_VERSION)\"
LFLAGS	:= -march=i686 --target=i686-pc-none-elf

all: pcitest.dev

#pcitest

pcitest.dev: pcitest/udev.c udev_start.o
	clang $(CFLAGS) -c pcitest/udev.c -o pcitest/udev.o -ffreestanding -O2 -Wall -Wextra
	i686-elf-ld --oformat binary -Ttext 0x10000 pcitest/udev.o udev_start.o -o pcitest.dev -O2 -nostdlib

udev_start.o: udev_start.s 
	clang $(LFLAGS) -c udev_start.s -o udev_start.o -ffreestanding -O2 -Wall -Wextra

clean:
	rm -f *.o
	rm -f *.dev
	rm -f */*.o 