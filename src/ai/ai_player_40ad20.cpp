// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    unsigned int ticks;                // +0x38a47
};

class PlayerAI {
public:
    char unknown_0[0xed];
    unsigned int lastTick;             // +0xed
    void UpdateEveryThirtyTicks();
    void RefreshUnitLists();
    void ComputeBaseWeights();
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall RandomInt(int range);

// FUNCTION: 0x40ad20
void PlayerAI::UpdateEveryThirtyTicks()
{
    if (g_game->ticks >= lastTick + 0x1e) {
        ((PlayerAI*)this)->RefreshUnitLists();
        lastTick = g_game->ticks;
        if (RandomInt(0x1e) == 0) {
            ((PlayerAI*)this)->ComputeBaseWeights();
        }
    }
}
