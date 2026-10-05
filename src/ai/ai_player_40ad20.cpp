// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game_0040ad20 {
    char unknown_0[0x38a47];
    unsigned int ticks;                // +0x38a47
};

class Class_0040ad20 {
public:
    char unknown_0[0xed];
    unsigned int lastTick;             // +0xed
    void FUN_0040ad20();
};
#pragma pack(pop)

extern Game_0040ad20* g_game;

int __stdcall FUN_004b6c30(int range);

class Class_0040aa40 {
public:
    void FUN_0040aa40();
};

class Class_00409730 {
public:
    void FUN_00409730();
};

// FUNCTION: 0x40ad20
void Class_0040ad20::FUN_0040ad20()
{
    if (g_game->ticks >= lastTick + 0x1e) {
        ((Class_0040aa40*)this)->FUN_0040aa40();
        lastTick = g_game->ticks;
        if (FUN_004b6c30(0x1e) == 0) {
            ((Class_00409730*)this)->FUN_00409730();
        }
    }
}
