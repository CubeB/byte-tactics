// Decompiled by Opus. Names are provisional.
// Claims a free slot (first byte 0xff) in the 300-entry table in g_game,
// marks it used and returns it, or returns 0 when the table is full.

struct Slot_00420920 {
    unsigned char state;               // +0x0 (0xff = free)
    char unknown_1[0x33];
};

#pragma pack(push, 1)
struct Game_00420920 {
    char unknown_0[0x1ab9f];
    Slot_00420920 slots[300];          // +0x1ab9f
};
#pragma pack(pop)

extern Game_00420920* g_game;

// FUNCTION: 0x420920
Slot_00420920* FUN_00420920()
{
    for (int i = 0; i < 300; i++) {
        if (g_game->slots[i].state == 0xff) {
            g_game->slots[i].state = 0;
            return &g_game->slots[i];
        }
    }
    return 0;
}
