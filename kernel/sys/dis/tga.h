#ifndef TGA_H
#define TGA_H

// https://wiki.osdev.org/Loading_Icons

typedef struct {
    unsigned char magic1;             // must be zero
    unsigned char colormap;           // must be zero
    unsigned char encoding;           // must be 2
    unsigned short cmaporig, cmaplen; // must be zero
    unsigned char cmapent;            // must be zero
    unsigned short x;                 // must be zero
    unsigned short y;                 // image's height
    unsigned short h;                 // image's height
    unsigned short w;                 // image's width
    unsigned char bpp;                // must be 32
    unsigned char pixeltype;          // must be 40
} __attribute__((packed)) tga_header_t;

/**
 * @brief Parses a tga file
 * 
 * @param ptr Pointer to data
 * @param size Size of data
 * @return unsigned int ret[0] = width, ret[1] = height, ret[2..] = ARGB pixels (blue in LSB, alpha in MSB)
 */
unsigned int tga_parse(unsigned char *ptr, int size);

#endif