#include "../../userspace/kernel.h"
#include "../../userspace/udev.h"

udev_d* udev_main() {
	printf("PCI UDEV DRIVER LOADING\n");
	udev_d udev;
	udev.name = "PCITest";
	udev.contact = "No Contact";
	udev.license = "Public Domain";
	printf("PCI UDEV DRIVER LOADED\n");
	return &udev;
}