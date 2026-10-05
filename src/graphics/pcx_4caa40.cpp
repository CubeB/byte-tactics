// Decompiled by Space Bunny Free. Names are provisional.
// Reads the body of a PCX file (the 0x80-byte header starts with 0x0a 0x05) into
// the caller's struct: the 8-bit RLE decoded pixels, the 256-entry packed RGB
// colour map that sits in the last 0x300 bytes of the file, and the width and
// height from the header's Xmin/Ymin/Xmax/Ymax words. The four output fields
// are cleared first, and a header that does not match leaves them all zero.
// The row decoder is a static inline helper: it is the only shape that puts its
// three locals (the count x, the byte b, the run length) in the first three
// stack slots and the pointer p in the fourth, which is what the frame layout
// and the parameter register choice (esi = the struct, edi = the file) need.
// The row loop counts down from height - 1 to 0, not from height to 1: MSVC
// strength-reduces that into a counter biased by one, which is the dec/js/inc
// guard at the loop entry and the `dec; jne` at its back edge.
// The colour map is read by seeking back 0x300 from the end of the file (that
// is ftell, not a size), so the body is decoded from the 0x80 byte header on.
#include <string.h>

struct PCX_004caa40 {
    unsigned char* data;     // +0x0 the decoded 8-bit pixels
    unsigned char* palette;  // +0x4 the 0x300 byte colour map
    int width;               // +0x8
    int height;              // +0xc
};

int __stdcall FUN_004bb7c0(void* file, void* buf, int size);
void __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bbd00(void* file);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

static inline void FUN_row(void* file, unsigned char* p, int x)
{
    unsigned char b;
    unsigned char run;
    while (x > 0) {
        FUN_004bb7c0(file, &b, 1);
        if ((b & 0xc0) == 0xc0) {
            run = b & 0x3f;
            x -= run;
            if (x < 0)
                run += x;             // a run longer than the row is cut short
            FUN_004bb7c0(file, &b, 1);
            if (run == 1) {
                *p = b;
                p++;
            } else {
                memset(p, b, run);
                p += run;
            }
        } else {
            *p = b;
            p++;
            x--;
        }
    }
}

// FUNCTION: 0x4caa40
int __stdcall FUN_004caa40(void* file, PCX_004caa40* pcx)
{
    unsigned char header[0x80];
    memset(pcx, 0, 16);
    if (FUN_004bb7c0(file, header, 0x80) == 0x80 && header[0] == 0x0a && header[1] == 5) {
        pcx->width = *(unsigned short*)(header + 8) - *(unsigned short*)(header + 4) + 1;
        pcx->height = *(unsigned short*)(header + 10) - *(unsigned short*)(header + 6) + 1;
        pcx->data = (unsigned char*)FUN_004d83b0("PCX BODY", pcx->width * pcx->height);
        pcx->palette = (unsigned char*)FUN_004d83b0("COLOR MAP", 0x300);
        FUN_004bb710(file, FUN_004bbd00(file) - 0x300);
        FUN_004bb7c0(file, pcx->palette, 0x300);
        FUN_004bb710(file, 0x80);
        {
            unsigned char* p = pcx->data;
            int rows;
            int w;
            w = pcx->width;
            for (rows = pcx->height - 1; rows >= 0; rows--) {
                FUN_row(file, p, w);
                p += w;
            }
        }
        return 1;
    }
    return 0;
}
