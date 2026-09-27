// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Player_00456de0 {
    int field_0;                        // +0x0
    int field_4;                        // +0x4
    char unknown_8[0x20 - 8];
    unsigned char field_20;             // +0x20
    char unknown_21[0x73 - 0x21];
    char flag_73;                       // +0x73
    char unknown_74[0x14b - 0x74];
};

struct Game_00456de0 {
    char unknown_0[0x1b63];
    Player_00456de0 players[10];        // +0x1b63
    char unknown_2851[0x38d6f - 0x2851];
    unsigned char progress[6];          // +0x38d6f
};

struct Packet_00456de0 {
    unsigned char type;                 // +0x0
    unsigned char progress;             // +0x1
};
#pragma pack(pop)

extern Game_00456de0* g_game;

int __stdcall FUN_00451df0(int player, void* data, int size);

static inline int PlayerId(unsigned char i)
{
    if (i != 10 && g_game->players[i].flag_73)
        return g_game->players[i].field_4;
    return -1;
}

// FUNCTION: 0x456de0
void FUN_00456de0()
{
    Packet_00456de0 packet;
    packet.type = 0x2a;
    packet.progress = (unsigned char)((g_game->progress[0] + g_game->progress[1]
        + g_game->progress[2] + g_game->progress[3] + g_game->progress[4]
        + g_game->progress[5]) / 6);
    for (unsigned char i = 0; i < 10; i++) {
        Player_00456de0* p = &g_game->players[i];
        if (p->field_0 == 0)
            continue;
        if (p->flag_73 != 1 && p->flag_73 != 2)
            continue;
        p->field_20 = packet.progress;
        FUN_00451df0(PlayerId(i), &packet, 2);
    }
}
