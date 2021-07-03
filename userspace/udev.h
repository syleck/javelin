#ifndef __USERSPACE_UDEV_H__
#define __USERSPACE_UDEV_H__

typedef struct udev_d_t {
	char* name;
	char* license;
	char* contact;
	int schedule;
} udev_d;

enum capabilities {
	STDIO_INTERCEPT,
	FB_INTERCEPT,
	PCI_INTERCEPT,
};

static void add_capability(int capability, void* fp) {

}

#endif