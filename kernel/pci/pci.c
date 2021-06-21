#include "pci.h"
#include "../x86/asm.h"
#include "../module.h"
#include "../drv/device.h"
#include "../string.h"
#include "../stdlib.h"
#include "../mem.h"

///
/// Manages discovering PCI devices, and putting them in the device system </drv/device.c>.
///

MODULE("PCI");
MODULE_CREATOR("kernelvega");
MODULE_CONTACT("watergatchi@protonmail.com");
MODULE_LICENSE("AGPL");

pci_table_u *pci_devices[32];
int devs = 0;

void add_pci_device(pci_table_u *pdev)
{
	pci_devices[devs] = pdev;
	devs ++;
	return;
}

uint16_t pci_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset)
{
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    uint16_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    tmp = (uint16_t)((inl (0xCFC) >> ((offset & 2) * 8)) & 0xffff);
    return tmp;
}

uint32_t pci_read_long(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset)
{
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    uint32_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(0xCF8, address);
    tmp = (uint32_t)inl (0xCFC);
    return tmp;
}

uint16_t getVendorID(uint16_t bus, uint16_t device, uint16_t function)
{
    uint32_t r0 = pci_read_word(bus,device,function,0);
    return r0;
}

uint16_t getDeviceID(uint16_t bus, uint16_t device, uint16_t function)
{
    uint32_t r0 = pci_read_word(bus,device,function,2);
    return r0;
}

uint16_t getClassId(uint16_t bus, uint16_t device, uint16_t function)
{
    uint32_t r0 = pci_read_word(bus,device,function,0xA);
    return (r0 & ~0x00FF) >> 8;
}

uint16_t getSubClassId(uint16_t bus, uint16_t device, uint16_t function)
{
    uint32_t r0 = pci_read_word(bus,device,function,0xA);
    return (r0 & ~0xFF00);
}

void pci_probe()
{
	for(uint32_t bus = 0; bus < 256; bus++)
    {
        for(uint32_t slot = 0; slot < 32; slot++)
        {
            for(uint32_t function = 0; function < 8; function++)
            {
                uint16_t vendor = getVendorID(bus, slot, function);
                if(vendor == 0xffff) {
                    continue;
                }
                uint16_t device = getDeviceID(bus, slot, function);
                pci_table_u *pdev = (pci_table_u *)malloc(sizeof(pci_table_u));
                pdev->base.vendor = vendor;
                pdev->base.device = device;
                pdev->base.function = function;
                pdev->base.bus = bus;
                pdev->base.slot = slot;
                pdev->base.class = getClassId(bus, slot, function);
                pdev->base.subclass = getSubClassId(bus, slot, function);
                pdev->base.header_type = ((stupidint32)pci_read_long(bus, slot, function, 0xE)).b.h;
                switch(pdev->base.header_type) {
                    case 0x0:
                        pdev->header0.bar0 = pci_read_long(bus,slot,function,0x10);
                        pdev->header0.bar1 = pci_read_long(bus,slot,function,0x14);
                        pdev->header0.bar2 = pci_read_long(bus,slot,function,0x18);
                        pdev->header0.bar3 = pci_read_long(bus,slot,function,0x1C);
                        pdev->header0.bar4 = pci_read_long(bus,slot,function,0x20);
                        pdev->header0.bar5 = pci_read_long(bus,slot,function,0x24);
                        pdev->header0.cardbus_cis = pci_read_long(bus,slot,function,0x28);
                        pdev->header0.subsystem_vendor = pci_read_word(bus,slot,function,0x2C);
                        pdev->header0.subsystem_id = pci_read_word(bus,slot,function,0x2E);
                        pdev->header0.expansion_rom = pci_read_long(bus,slot,function,0x30);
                        pdev->header0.capabilities = (uint8_t)pci_read_long(bus,slot,function,0x3f);
                        break;
                    case 0x80:

                        break;
                    default:
                        DVERBOSE(printf("PCI type %i, oopsing",pdev->base.header_type));
                        DVERBOSE(OOPS("Unknown pci type\n"));
                        break;
                }
                device_t* jdev = alloc_device();
                jdev->name = "PCI-0000:0000";
                jdev->dev_type = DEVICE_PCI;
                jdev->priv = pdev;
                char nd[5];
                itoa(pdev->header0.vendor,16,nd);
                memcpy(jdev->name+4,nd,4);
                itoa(pdev->header0.device,16,nd);
                memcpy(jdev->name+9,nd,4);
                switch(pdev->base.class) {
                    case 0x04:
                        switch(pdev->base.subclass) {
                            case 0x01:
                                add_alias(get_device_byname(jdev->name),"ac97");
                                break;
                            case 0x03:
                                add_alias(get_device_byname(jdev->name),"ihda");
                                break;
                            case 0x80:
                                add_alias(get_device_byname(jdev->name),"audio");
                                break;
                        }
                        break;
                }
                char *vendorname = (char*)malloc(255);
                switch(pdev->base.vendor) {
                    case 0x8086:
                        vendorname = "Intel";
                        break;
                    case 0x1022:
                        vendorname = "AMD";
                        break;
                    case 0x1002:
                        vendorname = "AMD/ATI";
                        break;
                    case 0x1234:
                        if(pdev->base.class == 0x6789)
                            vendorname = "Apple";
                        else
                            vendorname = "Bochs/QEMU";
                        break;
                    default:
                        vendorname = "Unknown";
                        break;
                }
                DVERBOSE(mprintf("vendor: %s (0x%x) device: 0x%x name: %s header: %i\n    class: 0x%x subclass: 0x%x\n    bus: 0x%x slot: 0x%x function 0x%x\n", vendorname, vendor, device, jdev->name, pdev->base.header_type, pdev->base.class, pdev->base.subclass, bus, slot, function));
                add_pci_device(pdev);
                free(vendorname);
            }
        }
    }
}

void setpciok(int slot, int bus, int function) {

}

void pci_install()
{
	devs = 0;
	pci_probe();
}