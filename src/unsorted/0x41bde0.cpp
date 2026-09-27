// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (best 64.5%). What still differs:
//   * the sub-object pointer is loaded into scratch edx (`mov edx,[eax+0x92]`)
//     where the original gets a callee-saved register (`mov esi,[eax+0x92]`,
//     hence the extra `push esi`/`pop esi`). This shifts all later registers
//     and branch offsets: the original computes the count comparison in edx
//     with ebx from esi, ours uses edx for the pointer.
//   * the original orders `shr edx,0x17; and edx,7; dec ebx`, ours puts the
//     `dec ebx` before the `and`.
// Everything else (struct sizes, 0x118 unit stride, branch structure, bit
// constants) matches byte for byte.

#pragma pack(push, 1)
struct Sub_0041bde0 {
    char unknown_0[0x22e];
    unsigned char count;                 // +0x22e
};

struct Unit_0041bde0 {
    char unknown_0[0x92];
    Sub_0041bde0* sub;                   // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                      // +0xa6
    char unknown_a8[0x110 - 0xa8];
    unsigned int bits_110_0 : 22;
    unsigned int active : 1;                      // bit 22
    unsigned int count : 3;                       // bits 23-25
    unsigned int bits_110_26 : 6;
    char unknown_114[0x118 - 0x114];
};

struct Game_0041bde0 {
    char unknown_0[0x14357];
    Unit_0041bde0* units;                // +0x14357
    char unknown_1435b[0x37e9c - 0x1435b];
    unsigned short unitIndex;            // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned char flags;                 // +0x37ebe
};
#pragma pack(pop)

extern Game_0041bde0* g_game;
extern char DAT_00502850[];              // "nextbuildmenu"
void __stdcall FUN_0047f1a0(char* name, int param);

// FUNCTION: 0x41bde0
void __stdcall FUN_0041bde0(int param_1)
{
    unsigned short index = g_game->unitIndex;
    if (index != 0) {
        Unit_0041bde0* unit = &g_game->units[index];
        if (unit->field_a6 == 0)
            unit = 0;
        if (unit != 0) {
            if (param_1 != 0) {
                if (!unit->active) {
                    unit->active = 1;
                    unit->count = 1;
                } else if (unit->count + 1 == unit->sub->count) {
                    unit->active = 0;
                } else {
                    unit->count++;
                }
            } else {
                if (unit->count + 1 == unit->sub->count)
                    unit->count = 1;
                else
                    unit->count++;
                unit->active = 1;
            }
            g_game->flags |= 0x10;
        }
    }
    FUN_0047f1a0(DAT_00502850, 0);
}
