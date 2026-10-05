// Decompiled by Opus. Names are provisional.
// Applies the brightness value and the two volume levels (scaled by 1024) to
// the object at g_game+0x10 (same tail as 0x45c630).

class Class_004d0070 {
public:
    void FUN_004d0070(int level);
};

class Class_004d00d0 {
public:
    void FUN_004d00d0(int level, int flag);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0x37f08 - 0x14];
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004ba590(float value);

// FUNCTION: 0x45bcc0
void FUN_0045bcc0()
{
    FUN_004ba590(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->FUN_004d0070(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->FUN_004d00d0(g_game->volume2 << 10, 0);
}
