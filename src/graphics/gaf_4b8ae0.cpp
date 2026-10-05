// Decompiled by Opus. Names are provisional.
// Initialises a 0x14-byte sprite/frame reference from a source record. The
// first field is written twice in the original.

struct Src_004b8ae0 {
    unsigned short a;               // +0x0
    char unknown_2[2];
    unsigned short b;               // +0x4
    char unknown_6[2];
    unsigned short c;               // +0x8
    char unknown_a[2];
    int d;                          // +0xc
    char unknown_10[8];
    unsigned short e;               // +0x18
    unsigned short f;               // +0x1a
};

struct Dst_004b8ae0 {
    unsigned short a;               // +0x0
    unsigned short b;               // +0x2
    unsigned short e;               // +0x4
    unsigned short f;               // +0x6
    unsigned char flag8;            // +0x8
    unsigned char flag9;            // +0x9
    unsigned char flaga;            // +0xa
    unsigned char flagb;            // +0xb
    char unknown_c[4];
    int d;                          // +0x10
};

// FUNCTION: 0x4b8ae0
void __stdcall FrameFromSurface(Dst_004b8ae0* dst, Src_004b8ae0* src)
{
    dst->a = src->a;
    dst->b = src->b;
    dst->a = src->c;
    dst->d = src->d;
    dst->e = src->e;
    dst->f = src->f;
    dst->flag9 = 0;
    dst->flag8 = 0xff;
    dst->flaga = 0;
    dst->flagb = 0;
}
