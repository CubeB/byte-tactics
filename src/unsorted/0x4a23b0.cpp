// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Entry_004a23b0 {
    char unknown_0[0x13];
    short x;                    // +0x13
    short y;                    // +0x15
    short w;                    // +0x17
    short h;                    // +0x19
    unsigned char flags;        // +0x1b
    char unknown_1c[0x140 - 0x1c];
    short off;                  // +0x140
    short size;                 // +0x142
    char unknown_144[0x15b - 0x144];
};
#pragma pack(pop)

// FUNCTION: 0x4a23b0
void __stdcall FUN_004a23b0(Entry_004a23b0* base, int index, int* r1, int* r2)
{
    Entry_004a23b0* e = base + index;
    r1[0] = e->x;
    r1[1] = e->y;
    r1[2] = r1[0] + e->w;
    r1[3] = r1[1] + e->h;
    if (e->flags & 1) {
        int left = r1[0] + e->off + 1;
        r2[0] = left;
        r2[1] = r1[1] + 1;
        r2[2] = left + e->size;
        r2[3] = r2[1] + e->h - 2;
    } else {
        r2[0] = r1[0] + 1;
        r2[1] = r1[1] + e->off + 2;
        r2[2] = r2[0] + e->w - 2;
        r2[3] = r2[1] + e->size;
    }
}
