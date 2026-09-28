// Decompiled by deepseek-v4.1-flash, finished by DeepSeek V4.1 Flash. Names are provisional.
// PARTIAL (best 28.8%, 373 of 414 bytes). Rebuilds the ally GUI list: for the
// local player it scans all 10 player slots and adds a "LIVEALLY%d" entry
// (value = two bytes from the local player's arrays at +0x108/+0x113) for each
// live ally.
//
// The control flow is now understood exactly. The original is:
//   * the first test of IsAlly (active, flag_9b & 0x40) is pulled out and
//     duplicated at the top of the loop body (0x447004..0x447021),
//   * IsAlly is then inlined IN FULL with all seven tests, and the redundant
//     trailing `field_144 == 0 && field_140 != 0` pair (the one whose result
//     was already computed at 0x44706a) is dead-code eliminated at 0x447094,
//   * then IsLive is inlined with the body offset by its own `active` test,
//     which is DCE'd as well (0x4470b9 starts at `type`).
// My v5 plain form above lowers to that shape and matches everything except:
//  * the two DCE'd tests cannot be suppressed: MSVC 5 keeps them (0x447094's
//    `test ecx,ecx`, 0x4470b9's `cmp byte [..+0x1ca9],0xa` which it also
//    reloads). The register is provably nonzero there; the optimizer never
//    propagated the earlier load across the sprintf/other blocks.
//  * the body's loop entry test is a single fused null check, not the
//    active-in-eax plus memory-operand form the original uses; putting the
//    index into its own statement (v7) does not change it either.
//  * hence the loop is missing 9 instructions (27 bytes) and the register
//    allocation differs: the original holds g_game in eax, the local player
//    aliases in ebp/ebx and the index in edi; mine uses ecx/ebp plus two
//    stack slots.
//  * my stack frame is already 0x34 (text[44], buffer at esp+0x18 vs esp+0x10).
//
// Tried and rejected (all produce byte-identical code to the plain form, so
// the exact source spelling does not matter): separate __inline helpers per
// test, a single big && chain, `continue` chains, a do/while with the body
// separated from the guard, binding the local player aliases as struct fields
// instead of pointers, `!`-negated tests, tests ordinary-if and negated.
#include <stdio.h>

#pragma pack(push, 1)
struct PlayerInfo_00446fb0 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
    char unknown_97[0x9b - 0x97];
    unsigned char flag_9b;             // +0x9b, tested with 0x40
};

struct Player_00446fb0 {               // 0x14b bytes
    int active;                        // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_00446fb0* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    unsigned char type;                // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char field_108[0xb];      // +0x108
    unsigned char field_113[0xb];      // +0x113
    char unknown_11e[0x140 - 0x11e];
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Sub_00446fb0 {
    char unknown_0[0x18];
    void* entry;                       // +0x18
};

struct Game_00446fb0 {
    char unknown_0[0x519];
    Sub_00446fb0 sub;                  // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_00446fb0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

extern Game_00446fb0* g_game;

int __stdcall FUN_004ab060(Sub_00446fb0* obj, const char* name);
int __stdcall FUN_004a1080(Sub_00446fb0* obj, char* name, int value);
void __stdcall FUN_0049fa90(Sub_00446fb0* obj);

__inline int IsAlly_00446fb0(int i)
{
    if (g_game->players[i].active == 0)
        return 0;
    if (g_game->players[i].info->flag_9b & 0x40)
        return 0;
    if (g_game->players[i].active == 0)
        return 0;
    if (g_game->players[i].type != 1 && g_game->players[i].type != 2
        && g_game->players[i].type != 3)
        return 0;
    if (g_game->players[i].field_146 == 10)
        return 0;
    if (g_game->players[i].type != 1 && g_game->players[i].type != 2
        && g_game->players[i].type != 3)
        return 0;
    if (g_game->players[i].field_144 == 0 && g_game->players[i].field_140 != 0)
        return 0;
    return 1;
}

__inline int IsLive_00446fb0(int i)
{
    if (g_game->players[i].active == 0)
        return 0;
    if (g_game->players[i].type != 1 && g_game->players[i].type != 2
        && g_game->players[i].type != 3)
        return 0;
    if (g_game->players[i].field_146 == 10)
        return 0;
    if (g_game->players[i].field_144 == 0 && g_game->players[i].field_140 != 0)
        return 0;
    if (g_game->players[i].info->field_96 == 0xff)
        return 0;
    return 1;
}

// FUNCTION: 0x446fb0
void FUN_00446fb0()
{
    char text[44];
    unsigned char p = g_game->localPlayer;
    Player_00446fb0* lp = &g_game->players[p];
    unsigned char* a = &lp->field_108[0];   // +0x1c6b
    unsigned char* b = &lp->field_113[0];   // +0x1c76

    int i;
    if (FUN_004ab060(&g_game->sub, "ALLIES.GUI") != 0) {
        for (i = 0; i != 10; ++i, ++a, ++b) {
            if (IsAlly_00446fb0(i) && i != g_game->localPlayer
                && IsLive_00446fb0(i)) {
                sprintf(text, "LIVEALLY%d", i);
                FUN_004a1080(&g_game->sub, text, (*b << 1) | *a);
            }
        }
        FUN_0049fa90(&g_game->sub);
    }
}
