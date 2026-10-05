// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_428b60 {
    unsigned char type;              // +0x00
    char unknown_1[0x1a];
    unsigned int flags;              // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                     // +0xb6
    char unknown_b8[0x15b - 0xb8];
};

struct Holder_428b60 {
    char unknown_0[4];
    Entry_428b60* entries;           // +0x04
};

struct Game {
    char unknown_0[0x531];
    Holder_428b60* holder;           // +0x531
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

// FUNCTION: 0x428b60
void FUN_00428b60(void)
{
    Entry_428b60* entries = g_game->holder->entries;
    for (int i = 1; i <= entries->count; i++) {
        if (entries[i].type == 5) {
            entries[i].flags |= 8;
        }
    }
}
