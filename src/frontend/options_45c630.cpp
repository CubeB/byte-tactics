// Decompiled by Opus. Names are provisional.
// Resets two settings (0x20 at +0x37f10, 4 at +0x37f16), enables the object at
// g_game+0x10 once (bit 0 of +0x37f14), then applies the brightness value and
// the two volume levels (scaled by 1024) to it.

class Class_004cdb40 {
public:
    void FUN_004cdb40();
};

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
    Class_004cdb40* sound;             // +0x10
    char unknown_14[0x37f08 - 0x14];
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
    unsigned short flags;              // +0x37f14
    unsigned char field_37f16;         // +0x37f16
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004ba590(float value);

// FUNCTION: 0x45c630
void FUN_0045c630()
{
    g_game->volume2 = 0x20;
    g_game->field_37f16 = 4;
    if (!(g_game->flags & 1)) {
        g_game->flags |= 1;
        g_game->sound->FUN_004cdb40();
    }
    FUN_004ba590(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->FUN_004d0070(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->FUN_004d00d0(g_game->volume2 << 10, 0);
}
