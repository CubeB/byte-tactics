// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Builds a 512-entry bitmap of the type indices (+0xa6) of the local player's
// selected units (bit 4 of the unit flags at +0x110), then sets bit 4 on every
// unit of that player that is tagged (bit 5), whose float at +0x104 is 0.0f,
// whose dword at +0xfb is 0, that has either no attached unit at +0x86 or one
// whose flags carry 0x40000000, and whose type index is in the bitmap. It then
// drops the current selection (+0x37e9c) to none, issues the STOP order
// (FUN_00495860) and sets order flag 0x10 at +0x37ebe. It is the mirror image
// of 0x48c9b0, which clears the same bit under the opposite conditions, and it
// repeats the loop 0x48bd50 does without the two-pass bitmap.
//
// Fixed the last four instructions by declaring +0x37ebe as the 16-bit
// bitfield group the rest of the game uses (see 0x432610.cpp): bit 4 is one
// named member, and writing it compiles to the original's single
// `or byte ptr [eax + 0x37ebe], 0x10` instead of a load/or/store in cl.
// Suspected original bug: MSVC 5 inverts the sense of a float comparison
// against zero here, so the source `== 0.0f` tags units whose +0x104 value is
// NOT 0.0f (the same inversion is visible in 0x48c9b0, which is written `!=
// 0.0f` and matched with `je` to its body). Checked with
// 0x48c9b0.cpp, 0x48bd00.cpp, 0x48bd50.cpp, 0x48d920.cpp and
// 0x495860.cpp.

#include <string.h>

#pragma pack(push, 1)
struct UnitFlags_0048be00 {
    unsigned int bits0 : 4;
    unsigned int selected : 1;         // bit 4
    unsigned int bits5 : 27;
};

struct Unit_0048be00 {                 // 0x118 bytes
    char unknown_0[0x86];
    Unit_0048be00* field_86;            // +0x86
    char unknown_8a[0xa6 - 0x8a];
    short field_a6;                    // +0xa6
    char unknown_a8[0xfb - 0xa8];
    int field_fb;                       // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                    // +0x104
    char unknown_108[0x110 - 0x108];
    UnitFlags_0048be00 flags;           // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_0048be00 {               // 0x14b bytes
    char unknown_0[0x67];
    Unit_0048be00* unitsBegin;          // +0x67
    Unit_0048be00* unitsEnd;            // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048be00 {
    char unknown_0[0x1b63];
    Player_0048be00 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;          // +0x2a42
    char unknown_2a43[0x37e9c - 0x2a43];
    unsigned short field_37e9c;         // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags_0 : 4;         // +0x37ebe
    unsigned short orderFlag : 1;       // +0x37ebe, bit 4
    unsigned short flags_5 : 11;
};
#pragma pack(pop)

extern Game_0048be00* g_game;

void FUN_00495860(void);

// The flags word at +0x110 is read whole in the second loop and through the
// bitfield view in the first, which is what the original does: bit 4 through
// the bitfield (shr 4; test cl, 1), bits 5, 30 and the store as a dword
// (test bl, 0x20; test dword [...], esi; or ebx, 0x10).
static inline unsigned int* FlagsPtr(Unit_0048be00* u)
{
    return (unsigned int*)&u->flags;
}

// FUNCTION: 0x48be00
void FUN_0048be00(void)
{
    Player_0048be00* player = &g_game->players[g_game->localPlayer];
    unsigned int selected[16];
    memset(selected, 0, sizeof(selected));
    {
        for (Unit_0048be00* u = player->unitsBegin; u <= player->unitsEnd; u++) {
            if (u->flags.selected) {
                unsigned int v = u->field_a6 & 0xffff;
                selected[v >> 5] |= 1 << (v & 0x1f);
            }
        }
    }
    for (Unit_0048be00* u = player->unitsBegin; u <= player->unitsEnd; u++) {
        unsigned int f = *FlagsPtr(u);
        if (f & 0x20) {
            if (u->field_104 == 0.0f) {
                if (u->field_fb == 0) {
                    if (u->field_86 == 0 || (*FlagsPtr(u->field_86) & 0x40000000)) {
                        unsigned int v = u->field_a6 & 0xffff;
                        if (selected[v >> 5] & (1 << (v & 0x1f)))
                            *FlagsPtr(u) = f | 0x10;
                    }
                }
            }
        }
    }
    g_game->field_37e9c = 0;
    FUN_00495860();
    g_game->orderFlag = 1;
}
