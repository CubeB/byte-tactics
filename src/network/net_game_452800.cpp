// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Matches once flags_38d75 is declared `volatile`, as the original evidently
// did (see 0x494e70.cpp for the evidence). Earlier notes, kept for reference:
// matches except the flags test at +0x38d75: the original emits
// `test byte ptr [edx+0x38d75],1` then `...,2` (two memory-operand bit
// tests), while this source emits `mov al,[edx+0x38d75]; test al,1;
// test al,2` (one load reused, 4 bytes shorter). Modelled the byte as a
// bitfield struct, a struct with inline bit0()/bit1() methods, explicit
// `== 0`/`!= 0` forms, `goto` per test, two different lvalue paths and all
// 128 header sets; MSVC always CSEs the load.
//
// Notes from Claude Opus 5.5 (#228): the N-declarations test (0 to 400
// unused externs) stays at 94.5% for every N, so this is a source-shape
// problem, not compiler state. The field is really a 16-bit word:
// 0x496861, 0x4975c0 and 0x498323 update it with word loads and stores
// (bit 2 cleared and set, bit 0 set), and 0x498342 reads bit 1 as a
// bitfield (`shr dl,1; test dl,1`). The same pair of memory tests appears
// in 0x4550c2 (this function inlined into another one) and 0x494e8b, and
// nowhere else in the game. Also tried, all still sharing one load: the
// word as an `unsigned short`, `int` or `unsigned char` bitfield, a plain
// `unsigned short`/`unsigned int` with masks, an embedded flags struct,
// unions mixing byte, word and dword views, a 2-bit field compared with 1,
// the word placed at +0x38d74 with masks 0x100/0x200, separate if/else-if
// statements, ternaries, a switch, `!(a && !b)` forms, inline accessors
// (with and without a Game* parameter), one test through a local copy of
// g_game, and pointer casts. In scratch tests only a real call between the
// two tests stops MSVC 5 sharing the load.

#pragma pack(push, 1)
struct Player_00452800 {
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x73 - 8];
    char flag_73;                      // +0x73
    char unknown_74[0x14b - 0x73 - 1];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00452800 players[10];       // +0x1b63
    char unknown_2851[0x2a38 - 0x2851];
    unsigned char* buffer;             // +0x2a38
    char unknown_2a3c[0x2a42 - 0x2a3c];
    unsigned char field_2a42;          // +0x2a42
    char unknown_2a43[0x38d75 - 0x2a43];
    volatile unsigned char flags_38d75; // +0x38d75, volatile in the original (see 0x494e70.cpp)
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall RemovePlayer(int id);
int __stdcall BroadcastPacket(int player, void* data, int size);

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
        RemovePlayer(id);
    }
    return BroadcastPacket(g_game->players[g_game->field_2a42].field_4, msg, 5);
}
