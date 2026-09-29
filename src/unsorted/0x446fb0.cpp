// Decompiled by deepseek-v4.1-flash, finished by DeepSeek V4.1 Flash, finished by LongCat 2.5 Preview Free, finished by space-bunny-free. Names are provisional.
// Rebuilds the ally GUI list: for the local player it scans all 10 player
// slots and, for each live ally other than the local player, adds a
// LIVEALLY%d entry whose value is two bytes from the local player's arrays at
// +0x108 and +0x113: (field_113 << 1) | field_108.
//
// MATCH (414 of 414 bytes). The whole shape came from two findings, both
// recorded below because neither is guessable from the disassembly alone.
//
//  * Every helper takes the player through a POINTER, Player_00446fb0* p, and
//    the loop passes &g_game->players[i] with no `lp` intermediate. That is
//    what puts g_game in eax (matching the original's `mov eax,[g_game]` at
//    +0x4a) and lets MSVC 5 fold the array base 0x1b63 into each member
//    offset, so the loads become [eax+esi+0x1b63], [eax+esi+0x1b8a],
//    [eax+esi+0x1bd6], [eax+esi+0x1ca3] with no separate base register. The
//    index-form helpers of the earlier attempts (int i, g_game->players[i])
//    give an extra base register and the wrong `mov ecx,[g_game]` (51.6%).
//  * IsAlly's FIRST active test reads a local, `int act = p->active;`, and its
//    SECOND active test reads `p->active` directly. MSVC 5 jump-threads a
//    straight `if (!p->active) return 0; if (flag) return 0; if (!p->active)
//    return 0;` down to a single test: the second one is provably dead. With
//    the local, the two reads are different expressions, so the second test
//    survives and the value stays live in ecx across the flag test. That live
//    value is also what forces the info pointer into edx (orig +0x63) instead
//    of ecx, and it is why the original loads info twice into edx and never
//    into ecx. Every other spelling tried (helper functions, __inline
//    decomposition, && chains, switch, comma, do/while(0), two pointer
//    variables, index-vs-pointer) threaded the test away or forced a reload.
//
// The loop must keep the two pointer induction variables: `for (i = 0; i < 10;
// ++i, ++a, ++b)` with `(*b << 1) | *a` in the body, not a[i] / b[i]. That
//    gives the two `inc ebp` / `inc ebx` at the back edge and the `mov cl,[ebx]
//    / mov dl,[ebp]` pair, and `i < 10` gives the original's signed `cmp esi,
//    0xcee / jl` back edge (i != 10 gives jne). The two name pointers are
//    written as &g_game->players[g_game->localPlayer].field_108[0] and
//    .field_113[0] with no lp variable, which is what makes MSVC 5 emit
//    `lea eax,[esi+eax*2]` with no displacement followed by one lea per array,
//    exactly as the original does.
//
// The buffer is 52 bytes, not 44: the original's frame is 0x34 with the buffer
// at the frame bottom, so `lea eax,[esp+0x14]` is the frame base.
//
// IsLive is a separate helper (not a shared IsPlaying/IsCounted): the original
// runs ONE type chain inside it, where a shared IsCounted would run two.
//
// Not the calling convention: FUN_00446fb0 is __cdecl with no arguments and
// the four callees' ret N values are already consistent with their
// declarations.
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

__inline int IsLiveType_00446fb0(Player_00446fb0* p)
{
    if (p->type != 1 && p->type != 2 && p->type != 3)
        return 0;
    return 1;
}

__inline int IsAlly_00446fb0(Player_00446fb0* p)
{
    int act = p->active;
    if (!act)
        return 0;
    if (p->info->flag_9b & 0x40)
        return 0;
    if (!p->active)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->field_146 == 10)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    return 1;
}

__inline int IsLive_00446fb0(Player_00446fb0* p)
{
    if (!p->active)
        return 0;
    if (!IsLiveType_00446fb0(p))
        return 0;
    if (p->field_146 == 10)
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    if (p->info->field_96 == 0xff)
        return 0;
    return 1;
}

// FUNCTION: 0x446fb0
void FUN_00446fb0()
{
    char text[52];
    unsigned char* a = &g_game->players[g_game->localPlayer].field_108[0];
    unsigned char* b = &g_game->players[g_game->localPlayer].field_113[0];

    if (FUN_004ab060(&g_game->sub, "ALLIES.GUI") != 0) {
        int i;
        for (i = 0; i < 10; ++i, ++a, ++b) {
            if (IsAlly_00446fb0(&g_game->players[i]) && i != g_game->localPlayer
                && IsLive_00446fb0(&g_game->players[i])) {
                sprintf(text, "LIVEALLY%d", i);
                FUN_004a1080(&g_game->sub, text, (*b << 1) | *a);
            }
        }
        FUN_0049fa90(&g_game->sub);
    }
}
