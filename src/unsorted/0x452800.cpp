// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (90.2%), best found. Everything matches except the flags test at
// +0x38d75: the original emits `test byte ptr [edx+0x38d75],1` then `...,2`
// (two memory-operand bit tests), while this source emits
// `mov al,[edx+0x38d75]; test al,1; test al,2` (one load reused, 4 bytes
// shorter). Modelled the byte as a bitfield struct, a struct with inline
// bit0()/bit1() methods, explicit `== 0`/`!= 0` forms, `goto` per test, two
// different lvalue paths and all 128 header sets; MSVC always CSEs the load.
// This looks like compiler register-allocation state the source alone does not
// reproduce.

#pragma pack(push, 1)
struct Player_00452800 {
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x73 - 8];
    char flag_73;                      // +0x73
    char unknown_74[0x14b - 0x73 - 1];
};

struct Game_00452800 {
    char unknown_0[0x1b63];
    Player_00452800 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    char unknown_2a3c[0x2a42 - 0x2a3c];
    unsigned char field_2a42;          // +0x2a42
    char unknown_2a43[0x38d75 - 0x2a43];
    unsigned char flags_38d75;         // +0x38d75
};
#pragma pack(pop)

extern Game_00452800* g_game;

void __stdcall FUN_00452cc0(int id);
int __stdcall FUN_00451df0(int player, void* data, int size);

static inline int PlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].flag_73)
        return g_game->players[i].field_4;
    return -1;
}

static unsigned char LookupPlayer(int id)
{
    if (id == -1)
        return 10;
    unsigned char i;
    for (i = 0; i < 10; i++) {
        if (PlayerId(i) == id)
            return i;
    }
    return 10;
}

// FUNCTION: 0x452800
int __stdcall FUN_00452800(int id)
{
    if (LookupPlayer(id) == 10)
        return 0;
    unsigned char* msg = g_game->buffer;
    msg[0] = 0x1c;
    *(int*)(msg + 1) = id;
    unsigned char index = LookupPlayer(id);
    Player_00452800* player = &g_game->players[index];
    if ((player->field_0 != 0 && player->flag_73 == 3)
        || !(g_game->flags_38d75 & 1)
        || (g_game->flags_38d75 & 2)) {
        FUN_00452cc0(id);
    }
    return FUN_00451df0(g_game->players[g_game->field_2a42].field_4, msg, 5);
}
