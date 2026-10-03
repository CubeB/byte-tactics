// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by DeepSeek V4.1 Flash, checked by GPT-6, matched by Claude Opus 5.5. Names are provisional.
// Places a unit's height (pos.y, 16.16 fixed point) on the ground, at the sea
// level or on the water surface, depending on the flags in the unit's type
// (+0x241): bit 12 floats, bit 19 floats on water, bit 20 can leave the water.
// Only runs for a unit that belongs to somebody and whose type can be off the
// ground; the flag at +0x110 bit 16 asks for this to be redone.
//
// The bit 19 branch builds the height in a 16.16 `Fixed` union local and
// copies the whole union into pos.y (as 0x4589c0 does with its `Fixed yv`).
// Every spelling that assigned an int let MSVC 5 fold
// `(draft * 0xffff + sea) << 16` into `(sea - draft) << 16`, or, with the fold
// blocked by a pointer or volatile, rotate the registers (88.8% at best); the
// union copy keeps the original's product, sum and shift in place.

union Fixed { int value; struct { unsigned short fraction; short whole; }; };  // 16.16

#pragma pack(push, 1)
struct UnitType_0048a870 {
    char unknown_0[0x22c];
    unsigned char draft;                // +0x22c
    char unknown_22d[0x241 - 0x22d];
    unsigned int flags_lo : 12;         // +0x241 bits 0..11
    unsigned int floats : 1;            // +0x241 bit 12
    unsigned int unknown_13 : 6;        // +0x241 bits 13..18
    unsigned int on_water : 1;          // +0x241 bit 19
    unsigned int over_water : 1;        // +0x241 bit 20
    unsigned int unknown_21 : 11;       // +0x241 bits 21..31
};

struct Pos_0048a870 {
    int x;                              // +0x0
    Fixed y;                            // +0x4
    int z;                              // +0x8
};

struct Unit_0048a870 {
    int* owner;                         // +0x0
    char unknown_4[0x6a - 4];
    Pos_0048a870 pos;                   // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitType_0048a870* type;            // +0x92
    char unknown_96[0x110 - 0x96];
    unsigned int flags;                 // +0x110
};

struct Game_0048a870 {
    char unknown_0[0x1427f];
    unsigned char seaLevel;             // +0x1427f
};
#pragma pack(pop)

extern Game_0048a870* g_game;

int __stdcall FUN_00485070(Pos_0048a870* pos);
void __stdcall FUN_0048a490(Unit_0048a870* unit);

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x48a870
void __stdcall FUN_0048a870(Unit_0048a870* unit)
{
    if ((unit->flags & 0x10000) || unit->type->floats) {
        unit->flags &= ~0x10000;
        if (unit->owner && (unit->flags & 3) == 1) {
            if (unit->type->over_water) {
                if (unit->type->floats) {
                    unit->pos.y.value = max(FUN_00485070(&unit->pos), g_game->seaLevel - unit->type->draft) << 16;
                } else {
                    unit->pos.y.value = FUN_00485070(&unit->pos) << 16;
                }
            } else if (unit->type->on_water) {
                Fixed h;
                h.value = unit->type->draft * 0xffff + g_game->seaLevel;
                h.value <<= 16;
                unit->pos.y = h;
            } else {
                FUN_0048a490(unit);
            }
        }
    }
}
