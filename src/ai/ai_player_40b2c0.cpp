// Decompiled by Opus. Names are provisional.
// Runs the periodic update of player `player`'s object (the body of
// 0x40ad20, inlined): at most once every 30 ticks.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    unsigned int ticks;                // +0x38a47
};

class PlayerAI {
public:
    char unknown_0[0xed];
    unsigned int lastTick;             // +0xed
    void RefreshUnitLists();
    void ComputeBaseWeights();
};
#pragma pack(pop)

extern Game* g_game;
extern PlayerAI* g_playerAI[];

int __stdcall RandomInt(int range);

// FUNCTION: 0x40b2c0
void __stdcall UpdatePlayerAI(int player)
{
    PlayerAI* p = g_playerAI[player];
    if (p && g_game->ticks >= p->lastTick + 0x1e) {
        p->RefreshUnitLists();
        p->lastTick = g_game->ticks;
        if (RandomInt(0x1e) == 0) {
            p->ComputeBaseWeights();
        }
    }
}
