// Surface, a drawing surface (0x30 bytes): the size, the pitch and the pixel
// pointer, the draw priority and colour key, the hotspot, the clip rectangle
// and the owned flag. The one declaration of the struct for the files that
// draw through it; surface.cpp defines the two methods. An allocated image's
// pixels follow the header at +0x30. Only rect.h is included, for the clip.
#ifndef SURFACE_H
#define SURFACE_H

#include "rect.h"

struct Surface {
    int width;                         // +0x00
    int height;                        // +0x04
    int pitch;                         // +0x08
    unsigned char* pixels;             // +0x0c
    int zPriority;                     // +0x10, 10000 when set up
    int colorKey;                      // +0x14, -1 when set up
    short x;                           // +0x18, the hotspot
    short y;                           // +0x1a
    Rect clip;                         // +0x1c
    unsigned int flag0 : 1;            // +0x2c, set when the surface owns its allocation
    unsigned int flag1 : 1;            // +0x2c bit 1

    Surface() {}                       // keeps the struct non-trivial: it is copied member by member
    Rect* GetClipRect(Rect* out);
    void SetClipRect(Rect r);
};

#endif
