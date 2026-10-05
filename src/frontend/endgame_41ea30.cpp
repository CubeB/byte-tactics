// Decompiled by Opus. Names are provisional.

struct Amount_41ea30 {
    int current;                     // +0x00
    int required;                    // +0x04
};

#pragma pack(push, 1)
struct Entry_41ea30 {
    unsigned char type;              // +0x00
    char unknown_1[0x28];
    unsigned char flag;              // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                     // +0xb6
    char unknown_b8[2];
    Amount_41ea30 amount;            // +0xba
    char unknown_c2[0x15b - 0xc2];
};

struct Holder_41ea30 {
    char unknown_0[4];
    Entry_41ea30* entries;           // +0x04
};

struct Game_41ea30 {
    char unknown_0[0x531];
    Holder_41ea30* holder;           // +0x531
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game_41ea30* g_game;

// FUNCTION: 0x41ea30
int FUN_0041ea30(void)
{
    Entry_41ea30* entries = g_game->holder->entries;
    int count = entries->count;
    for (int i = 0; i < count; i++) {
        if (entries[i].type == 0xd) {
            Amount_41ea30* amount = &entries[i].amount;
            if (entries[i].flag == 0 || amount->current < amount->required) {
                return 0;
            }
        }
    }
    return 1;
}
