// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_00463ef0 {                // 0x48 bytes
    unsigned int time;                 // +0x0
    char unknown_4[0x44];
};

struct Game_00463ef0 {
    char unknown_0[0x132f];
    Entry_00463ef0 entries[30];        // +0x132f
    char unknown_1b9f[0x2a3e - 0x1b9f];
    unsigned short tail;               // +0x2a3e
    unsigned short head;               // +0x2a40
    char unknown_2a42[0x37f23 - 0x2a42];
    int field_37f23;                   // +0x37f23
    char unknown_37f27[0x38a47 - 0x37f27];
    unsigned int now;                  // +0x38a47
};
#pragma pack(pop)

extern Game_00463ef0* g_game;

// FUNCTION: 0x463ef0
int FUN_00463ef0()
{
    int result = 0;
    unsigned short i = g_game->head;
    if (g_game->tail != i
        && g_game->entries[i].time + (g_game->field_37f23 + 1) * 30 < g_game->now) {
        result = 1;
        g_game->head = i + 1;
        if (g_game->head == 30)
            g_game->head = 0;
    }
    return result;
}
