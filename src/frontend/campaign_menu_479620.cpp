// Decompiled by Opus. Names are provisional.
// Counts the active 24-byte entries whose field at +0x8 equals the argument.

struct Entry_00479620 {
    int active;                        // +0x0
    int unknown_4;                     // +0x4
    int owner;                         // +0x8
    char unknown_c[0x18 - 0xc];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x29a0];
    Entry_00479620* entries;           // +0x29a0
    char unknown_29a4[0x38d81 - 0x29a4];
    int count;                         // +0x38d81
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x479620
int __stdcall CountPlayersInAllyGroup(int owner)
{
    int n = 0;
    for (int i = 0; i < g_game->count; i++) {
        if (g_game->entries[i].owner == owner && g_game->entries[i].active != 0) {
            n++;
        }
    }
    return n;
}
