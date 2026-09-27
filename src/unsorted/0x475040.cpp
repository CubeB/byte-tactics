// Decompiled by Opus. Names are provisional.

void* __stdcall FUN_004b7f30(void* a, int b);
void __stdcall FUN_004b8500(void* dest, void* src, int x, int y);

class Class_00475040 {
public:
    void* data;                        // +0x00
    char unknown_4[0x6 - 0x4];
    short x;                           // +0x06
    char unknown_8[0xa - 0x8];
    short height;                      // +0x0a
    char unknown_c[0xe - 0xc];
    short y;                           // +0x0e
    char unknown_10[0x14 - 0x10];
    int field_14;                      // +0x14
    void FUN_00475040(void* dest, short px, short py);
};

// FUNCTION: 0x475040
void Class_00475040::FUN_00475040(void* dest, short px, short py)
{
    short sy = y - (height >> 1) - py + 0x20;
    short sx = x - px + 0x80;
    FUN_004b8500(dest, FUN_004b7f30(data, field_14), sx, sy);
}
