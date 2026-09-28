// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Obj_004badf0 {
    char unknown_0[0xc4];
    unsigned char* buffer; // +0xc4
    char unknown_c8[0x28];
    unsigned short bit0_5 : 6;
    unsigned short flag6 : 1;
    unsigned short bit7_15 : 9;
};
#pragma pack(pop)

extern int FUN_004b6220(void);
void __stdcall FUN_004ba920(unsigned char* data, int* sums, unsigned char* idx);
unsigned char __stdcall FUN_004ba9d0(unsigned char* palette, int* sums, unsigned char* idx, unsigned int color);

extern double DAT_004fdbe8;

// Builds a 32-row shading table of 256 palette indices each into obj->buffer.
// The original addresses the destination as `buffer[row + i]` with a plain
// 32-bit index; MSVC then emits `lea ecx,[esi+ebp]` and keeps the base in edx.
// Writing it that way here makes MSVC fold one addend into the base instead
// (`add edx,esi` with `[edx+ebp]`), so this version routes the index through an
// `unsigned short` to force the same `lea`; what still differs is the extra
// `and ecx,0xffff` MSVC then emits (and the scheduling of `add edi,4`/`inc esi`).
// FUNCTION: 0x4badf0
unsigned char* __stdcall FUN_004badf0(unsigned char* param_1)
{
    Obj_004badf0* obj = (Obj_004badf0*)FUN_004b6220();
    if (obj->flag6) {
        int sums[256];
        unsigned char idx[256];
        FUN_004ba920(param_1, sums, idx);
        double f = 0.0;
        int row = 0;
        do {
            for (int i = 0; i < 256; i++) {
                unsigned char color[4];
                unsigned short v;
                v = (unsigned short)(param_1[i * 4 + 0] * f);
                color[0] = (unsigned char)v;
                if (v > 0xff) color[0] = 0xff;
                v = (unsigned short)(param_1[i * 4 + 1] * f);
                color[1] = (unsigned char)v;
                if (v > 0xff) color[1] = 0xff;
                v = (unsigned short)(param_1[i * 4 + 2] * f);
                color[2] = (unsigned char)v;
                if (v > 0xff) color[2] = 0xff;
                unsigned short at = (unsigned short)(row + i);
                obj->buffer[at] = FUN_004ba9d0(param_1, sums, idx, *(unsigned int*)color);
            }
            f -= DAT_004fdbe8;
            row += 0x100;
        } while (row < 0x2000);
        return obj->buffer;
    }
    return 0;
}
