// Decompiled by deepseek-v4.1-flash, finished by DeepSeek V4.1 Flash, finished by LongCat 2.5 Preview Free. Names are provisional.
// PARTIAL (best 51.6%, 385 of 414 bytes). Rebuilds the ally GUI list: for the
// local player it scans all 10 player slots and adds a LIVEALLY%d entry
// (value = two bytes from the local player's arrays at +0x108/+0x113) for each
// live ally.
//
// The control flow is now right, the prologue, the loop body, the value
// computation and the loop back edge all match. What is left:
//  * the original holds g_game in eax, the byte-offset induction variable in
//    esi, the index in edi and the two name-array pointers in ebp (field_108)
//    and ebx (field_113). We hold g_game in ecx and get ebp/ebx the other way
//    round, so every `mov al,[type] / cmp al` should be `mov cl` / `cmp cl`,
//    `xor eax,eax / mov al,[localPlayer]` should be ecx, and the three loads
//    into eax at the loop head should be ecx.
//  * the original still tests `players[i].active != 0` a second time inside
//    IsAlly (0x447027) and again at the head of IsLive (0x447094); MSVC 5
//    merges all three of ours into the single test at the loop head.
//
// Source shape that matters (all measured, variants in build/scratch/0x446fb0):
//  * `for (i = 0; i < 10; ++i, ++a, ++b)` with `(*b << 1) | *a` in the body, not
//    `a[i]` / `b[i]`. That is what produces the two register induction
//    variables incremented at the back edge and the `mov cl,[ebx] / mov dl,
//    [ebp]` pair; indexing produces one base plus an 11-byte difference
//    instead (51.6% against 38.9%). `i < 10` also gives the original's signed
//    `cmp esi, 0xcee / jl` back edge, where `i != 10` gives `jne`.
//  * the two name pointers must be written as
//    `&g_game->players[g_game->localPlayer].field_108[0]` (and .field_113),
//    with no `lp` intermediate. That is what makes MSVC 5 fold the array base
//    0x1b63 into the member offset and emit `lea eax,[esi+eax*2]` with no
//    displacement followed by one lea per array, exactly as the original does.
//    With an `lp` variable it emits `lea eax,[esi+eax*2+0x1b63]` plus two
//    3-byte leas: 28.8% instead of 31.5%.
//  * the buffer is 52 bytes, not 44: the original's frame is 0x34 with the
//    buffer at the frame bottom, so `lea eax,[esp+0x14]` is the frame base.
//    44 bytes gives frame 0x2c and `[esp+0x10]`; 48/56/64 pad the frame but
//    leave the buffer at the wrong offset (37.1%).
//  * IsAlly must take the player through a local pointer
//    (`Player_00446fb0* p = &g_game->players[i];`, then `p->...`), and its
//    `active` test must come from a separate __inline helper. With the indexed
//    form the compiler keeps `field_144` in di across the whole IsLive block
//    and emits `mov di,word[..] / test di,di` instead of the original's
//    memory-operand `cmp word ptr [eax+esi+0x1ca7],0` (36.6% against 38.9%).
//
// Tried and rejected, all measured: `<windows.h>` (it changes the sibling
// 0x446c70 but not this function); putting the matched 0x446f50 in the same
// file above this one (byte-identical output); an explicit
// `if (players[i].active == 0) continue;` guard at the top of the loop body
// (removed outright by MSVC 5); a local variable holding `active` inside
// IsAlly, or spelling the two `active` tests differently (helper versus
// direct), which does add the extra test back but at the cost of the memory
// operand forms (49.6% to 50.4%); declaring `b` before `a` (50.7%, the leas
// swap but the registers do not follow); the `IsPlaying`/`IsCounted`
// decomposition used by the sibling 0x446c70 (24.6%, it reorders the two type
// chains); `a` as `(unsigned char*)lp + 0x108` or as first+0xb.
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

__inline int IsActive_00446fb0(int i)
{
    return g_game->players[i].active != 0;
}

__inline int IsAlly_00446fb0(int i)
{
    Player_00446fb0* p = &g_game->players[i];
    if (!IsActive_00446fb0(i))
        return 0;
    if (p->info->flag_9b & 0x40)
        return 0;
    if (!IsActive_00446fb0(i))
        return 0;
    if (p->type != 1 && p->type != 2 && p->type != 3)
        return 0;
    if (p->field_146 == 10)
        return 0;
    if (p->type != 1 && p->type != 2 && p->type != 3)
        return 0;
    if (p->field_144 == 0 && p->field_140 != 0)
        return 0;
    return 1;
}

__inline int IsLive_00446fb0(int i)
{
    if (!IsActive_00446fb0(i))
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
    char text[52];
    unsigned char* a = &g_game->players[g_game->localPlayer].field_108[0];
    unsigned char* b = &g_game->players[g_game->localPlayer].field_113[0];

    if (FUN_004ab060(&g_game->sub, "ALLIES.GUI") != 0) {
        int i;
        for (i = 0; i < 10; ++i, ++a, ++b) {
            if (IsAlly_00446fb0(i) && i != g_game->localPlayer
                && IsLive_00446fb0(i)) {
                sprintf(text, "LIVEALLY%d", i);
                FUN_004a1080(&g_game->sub, text, (*b << 1) | *a);
            }
        }
        FUN_0049fa90(&g_game->sub);
    }
}
