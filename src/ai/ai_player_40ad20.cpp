// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    unsigned int ticks;                // +0x38a47
};

class Class_0040ad20 {
public:
    char unknown_0[0xed];
    unsigned int lastTick;             // +0xed
    void UpdateEveryThirtyTicks();
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall RandomInt(int range);

class Class_0040aa40 {
public:
    void RefreshUnitLists();
};

class Class_00409730 {
public:
    void ComputeBaseWeights();
};

// FUNCTION: 0x40ad20
void Class_0040ad20::UpdateEveryThirtyTicks()
{
    if (g_game->ticks >= lastTick + 0x1e) {
        ((Class_0040aa40*)this)->RefreshUnitLists();
        lastTick = g_game->ticks;
        if (RandomInt(0x1e) == 0) {
            ((Class_00409730*)this)->ComputeBaseWeights();
        }
    }
}
