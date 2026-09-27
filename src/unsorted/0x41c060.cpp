// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Partial (50%): sets the selected unit's active/count bits and raises the
// game flag. The diff is a pure eax/edx register mirror: the original keeps
// the unit pointer in eax and the sub pointer in edx, with param_1 in ecx and
// the count in ebx; this version keeps the unit in edx and the sub in eax.
// A direct inline lookup gives unit=eax/sub=edx but then param_1=edx and
// count=ecx; the inlined GetSelectedUnit helper gives param_1=ecx/count=ebx
// but unit=edx/sub=eax. No source form tried (locals for param_1 or count, a
// Sub* local, reversed comparisons, early return, a helper with different
// arguments) produces both halves at once.

#pragma pack(push, 1)
struct Sub_0041c060 {
    char unknown_0[0x22e];
    unsigned char count;                 // +0x22e
};

struct Unit_0041c060 {
    char unknown_0[0x92];
    Sub_0041c060* sub;                   // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                      // +0xa6
    char unknown_a8[0x110 - 0xa8];
    unsigned int bits_110_0 : 22;
    unsigned int active : 1;             // bit 22
    unsigned int count : 3;              // bits 23-25
    unsigned int bits_110_26 : 6;
    char unknown_114[0x118 - 0x114];
};

struct Game_0041c060 {
    char unknown_0[0x14357];
    Unit_0041c060* units;                // +0x14357
    char unknown_1435b[0x37e9c - 0x1435b];
    unsigned short unitIndex;            // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short b0 : 1;               // +0x37ebe
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;               // 0x10
    unsigned short b5 : 1;
    unsigned short b6 : 1;
    unsigned short b7 : 1;
    unsigned short b8 : 1;
    unsigned short b9 : 1;
    unsigned short b10 : 1;
    unsigned short b11 : 1;
    unsigned short b12 : 1;
    unsigned short b13 : 1;
    unsigned short b14 : 1;
    unsigned short b15 : 1;
};
#pragma pack(pop)

extern Game_0041c060* g_game;
extern char DAT_00502850[];              // "nextbuildmenu"
void __stdcall FUN_0047f1a0(char* name, int param);

// The unit at g_game->unitIndex, or 0 when the index is empty or the slot is
// not live (field_a6 == 0). Inlined at the call site.
static Unit_0041c060* GetSelectedUnit()
{
    unsigned short index = g_game->unitIndex;
    if (index) {
        Unit_0041c060* unit = &g_game->units[index];
        if (unit->field_a6)
            return unit;
    }
    return 0;
}

// FUNCTION: 0x41c060
void __stdcall FUN_0041c060(int param_1)
{
    Unit_0041c060* unit = GetSelectedUnit();
    if (unit != 0) {
        if (param_1 < unit->sub->count) {
            unit->active = param_1 > 0;
            if (param_1 != 0)
                unit->count = param_1;
            g_game->b4 = 1;
            FUN_0047f1a0(DAT_00502850, 0);
        }
    }
}
