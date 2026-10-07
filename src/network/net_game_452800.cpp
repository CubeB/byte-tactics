// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The flags field at +0x38d75 is really a 16-bit word: 0x496861, 0x4975c0 and
// 0x498323 update it with word loads and stores (bit 2 cleared and set, bit 0
// set), and 0x498342 reads bit 1 as a bitfield.

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
    // volatile: keeps the two bit tests as separate memory loads.
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
