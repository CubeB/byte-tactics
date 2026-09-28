// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (best 28.8%). Rebuilds the ally GUI list: for the local player it
// scans all 10 player slots and adds a "LIVEALLY%d" entry (value = two bytes
// from the local player's arrays at +0x108/+0x113) for each live ally.
//
// What still differs from the original (414 bytes, ours 373):
//  * The original keeps three redundant tests that MSVC 5 SP3 folds away in
//    every source spelling I tried (plain repeats, one big &&, continue
//    chains, static inline / __inline helpers, macro-style duplication):
//    the second `players[i].active == 0` after the flag test (0x447027), the
//    third `Players[i].active == 0` after `i != localPlayer` (0x447094), and
//    the second `field_146 != 10` (0x4470b9, which the original also
//    reloads). The original's `test ecx,ecx` at 0x447027 and 0x447094 are on
//    a register that is provably nonzero, so its optimizer clearly did not
//    propagate across the intervening blocks; I could not reproduce that.
//  * Probably as a consequence, the register allocation differs: the
//    original keeps the two local-player byte arrays in ebp/ebx and the
//    index in edi, and holds g_game in eax; ours puts g_game in ecx, i in
//    ebp and one pointer on the stack.
//  * Stack frame is 0x34 in the original (its sprintf buffer sits at
//    esp+0x10); ours is 0x34 with text[44] but the buffer is placed
//    differently.
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
    unsigned char* a = lp->field_108;
    unsigned char* b = lp->field_113;

    if (FUN_004ab060(&g_game->sub, "ALLIES.GUI") != 0) {
        for (int i = 0; i < 10; i++, a++, b++) {
            if (IsAlly_00446fb0(i) && i != g_game->localPlayer
                && IsLive_00446fb0(i)) {
                sprintf(text, "LIVEALLY%d", i);
                FUN_004a1080(&g_game->sub, text, (*b << 1) | *a);
            }
        }
        FUN_0049fa90(&g_game->sub);
    }
}
