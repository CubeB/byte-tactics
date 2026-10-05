// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x38a47];
    unsigned int ticks;                // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class SmokeParticles {
public:
    char unknown_0[4];
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    int FUN_00475440();
};

// FUNCTION: 0x475440
int SmokeParticles::FUN_00475440()
{
    if (field_8 <= field_4 && (unsigned int)field_8 <= g_game->ticks) {
        return 1;
    }
    return 0;
}
