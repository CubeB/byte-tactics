// Decompiled by Opus. Names are provisional.
struct ChatHudEntry {
    char unknown_0[0x1e];
    unsigned char flags;   // +0x1e
    char unknown_1f[0x29];
};

struct Game {
    char unknown_0[0x1318];
    ChatHudEntry entries[30];
};

extern Game* g_game;
int ScrollToNextMessageUnit(void);

// FUNCTION: 0x464000
void CycleMessageUnits(void)
{
    int i;
    for (i = 0; i < 30; i++)
        g_game->entries[i].flags &= ~0x20;
    if (ScrollToNextMessageUnit() == 0) {
        for (i = 0; i < 30; i++)
            g_game->entries[i].flags &= ~0x10;
        ScrollToNextMessageUnit();
    }
}
