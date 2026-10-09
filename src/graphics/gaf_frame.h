// GafFrame, one image of a GAF file (0x18 bytes): the size, the hotspot offset,
// the format bytes and the two pixel planes. The one declaration for the files
// that share it. Pure data (the exe has no member function of it), and nothing
// is included.
#ifndef GAF_FRAME_H
#define GAF_FRAME_H

struct GafFrame {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
    short xOffset;                     // +0x04
    short yOffset;                     // +0x06
    unsigned char transparency;        // +0x08, the key colour
    unsigned char compressed;          // +0x09
    unsigned char layers;              // +0x0a
    unsigned char blend;               // +0x0b
    int reserved;                      // +0x0c
    unsigned char* pixelsOrLayers;     // +0x10
    unsigned char* scratch;            // +0x14, the second plane
};

#endif
