// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Finds the first player slot with flag_73 set and info->flags_97 bit 0 set
// (or 10 when there is none), then either handles the local player or
// broadcasts packet 0x17 to that slot.
//
// PARTIAL: 66.0%. The loop body is byte-identical, but three regions differ:
//  - prologue: the original computes the local player pointer with
//    `lea eax,[edx+esi]` and schedules the `i = 0` store between the two index
//    multiplies; ours uses `mov eax,edx; add eax,esi` after them.
//  - loop exit: the original materialises the byte local on both exits
//    (`mov [esp+0x10],0xa` on the normal exit and an out-of-line
//    `mov [esp+0x10],bl; jmp` for the found exit); ours merges the exits and
//    keeps the value in bl.
//  - post-loop: the original reads the byte into eax and the i != 10 branch
//    copies eax to ecx; ours uses ecx from the start.
// The same loop appears inlined in 0x44fed0 and 0x451220 and compiles to the
// materialised form there, so the source is probably an inlined `FindSlot`
// helper whose result byte MSVC coalesced with the caller's variable; every
// helper spelling tried (by value, by pointer, by reference, __inline,
// __forceinline) still returned the value through al here.

#pragma pack(push, 1)
struct PlayerInfo_004526c0 {
    char unknown_0[0x96];
    unsigned char field_96;            // +0x96
    unsigned char flags_97;            // +0x97
};

struct Player_004526c0 {
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x27 - 8];
    PlayerInfo_004526c0* info;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    char flag_73;                      // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_004526c0 {
    char unknown_0[0x1b63];
    Player_004526c0 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    char unknown_2a3c[0x2a42 - 0x2a3c];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

class Class_004618a0 {
public:
    int FUN_004618a0(int param_1);
};

extern Game_004526c0* g_game;
extern int DAT_00506dbc;
extern Class_004618a0 DAT_00513000;

int __stdcall FUN_00451bc0(int from, int to, void* packet, int size);
int __stdcall FUN_00451df0(int player, void* data, int size);
int __stdcall FUN_004523e0(int a, int b, int c);
int __stdcall FUN_00452570(int a, int b);

// FUNCTION: 0x4526c0
int __stdcall FUN_004526c0(int param)
{
    int local = g_game->localPlayer;
    Player_004526c0* p = &g_game->players[local];
    unsigned char i;

    for (i = 0; i < 10; i++) {
        if (g_game->players[i].flag_73 != 0
            && (g_game->players[i].info->flags_97 & 1) != 0)
            break;
    }

    if (i == local) {
        if (FUN_00452570(p->field_4, param) == 0) {
            FUN_004523e0(p->field_4, p->field_4, param);
            return 1;
        }
        p->info->field_96 = param;
        return 1;
    }

    unsigned char* buffer = g_game->buffer;
    buffer[0] = 0x17;
    buffer[1] = (unsigned char)param;

    int result;
    if (i == 10)
        result = FUN_00451df0(p->field_4, buffer, 2);
    else
        result = FUN_00451bc0(p->field_4, g_game->players[i].field_4, buffer, 2);

    if (DAT_00506dbc != 0)
        DAT_00513000.FUN_004618a0(1);
    return result;
}
