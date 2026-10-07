// Decompiled by Space Bunny Free. Names are provisional.
// Reads the body of a PCX file (the 0x80-byte header starts with 0x0a 0x05) into
// the caller's struct: the 8-bit RLE decoded pixels, the 256-entry packed RGB
// colour map that sits in the last 0x300 bytes of the file, and the width and
// height from the header's Xmin/Ymin/Xmax/Ymax words. The four output fields
// are cleared first, and a header that does not match leaves them all zero.
// The colour map is read by seeking back 0x300 from the end of the file (that
// is ftell, not a size), so the body is decoded from the 0x80 byte header on.
#include <string.h>

struct PCX_004caa40 {
    unsigned char* data;     // +0x0 the decoded 8-bit pixels
    unsigned char* palette;  // +0x4 the 0x300 byte colour map
    int width;               // +0x8
    int height;              // +0xc
};

int __stdcall HAPI_readfromfile(void* file, void* buf, int size);
void __stdcall HAPI_SeekFile(void* file, int pos);
int __stdcall HAPI_FileLength(void* file);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);

// Static inline: the only shape that puts x, b, run, then p in the first four
// stack slots.
static inline void FUN_row(void* file, unsigned char* p, int x)
{
    unsigned char b;
    unsigned char run;
    while (x > 0) {
        HAPI_readfromfile(file, &b, 1);
        if ((b & 0xc0) == 0xc0) {
            run = b & 0x3f;
            x -= run;
            if (x < 0)
                run += x;             // a run longer than the row is cut short
            HAPI_readfromfile(file, &b, 1);
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
int __stdcall DecodePcx(void* file, PCX_004caa40* pcx)
{
    unsigned char header[0x80];
    memset(pcx, 0, 16);
    if (HAPI_readfromfile(file, header, 0x80) == 0x80 && header[0] == 0x0a && header[1] == 5) {
        pcx->width = *(unsigned short*)(header + 8) - *(unsigned short*)(header + 4) + 1;
        pcx->height = *(unsigned short*)(header + 10) - *(unsigned short*)(header + 6) + 1;
        pcx->data = (unsigned char*)FUN_004d83b0("PCX BODY", pcx->width * pcx->height);
        pcx->palette = (unsigned char*)FUN_004d83b0("COLOR MAP", 0x300);
        HAPI_SeekFile(file, HAPI_FileLength(file) - 0x300);
        HAPI_readfromfile(file, pcx->palette, 0x300);
        HAPI_SeekFile(file, 0x80);
        {
            unsigned char* p = pcx->data;
            int rows;
            int w;
            w = pcx->width;
            // Counts down from height - 1 to 0, not from height to 1.
            for (rows = pcx->height - 1; rows >= 0; rows--) {
                FUN_row(file, p, w);
                p += w;
            }
        }
        return 1;
    }
    return 0;
}
