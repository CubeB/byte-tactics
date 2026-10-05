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
};
#pragma pack(pop)

extern Game* g_game;
extern PlayerAI* g_playerAI[];

int __stdcall FUN_004b6c30(int range);

class Class_0040aa40 {
public:
    void FUN_0040aa40();
};

class Class_00409730 {
public:
    void FUN_00409730();
};

// FUNCTION: 0x40b2c0
void __stdcall FUN_0040b2c0(int player)
{
    PlayerAI* p = g_playerAI[player];
    if (p && g_game->ticks >= p->lastTick + 0x1e) {
        ((Class_0040aa40*)p)->FUN_0040aa40();
        p->lastTick = g_game->ticks;
        if (FUN_004b6c30(0x1e) == 0) {
            ((Class_00409730*)p)->FUN_00409730();
        }
    }
}
