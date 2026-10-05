// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Copies six flag bits out of the saved settings value into the flags word at
// +0x37f06, restores the brightness and resolution defaults, then applies the
// brightness and both volume levels to the object at g_game+0x10 (same tail as
// 0x45bcc0).

class Class_004d0070 {
public:
    void SetWaveVolume(int level);
};

class Class_004d00d0 {
public:
    void SetAuxVolume(int level, int flag);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0x2a44 - 0x14];
    unsigned char field_2a44;          // +0x2a44
    char unknown_2a45[0x37f06 - 0x2a45];
    unsigned short flags;              // +0x37f06
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
    char unknown_37f14[0x37f1b - 0x37f14];
    int width;                         // +0x37f1b
    int height;                        // +0x37f1f
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_00512f38;
extern int DAT_00512f3a;
extern int DAT_00512f4d;
extern int DAT_00512f51;

void __stdcall SetBrightness(float value);

// FUNCTION: 0x45cae0
void FUN_0045cae0()
{
    unsigned short v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 2);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 4);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 8);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 0x10);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 0x20);
    v = g_game->flags;
    g_game->flags = v ^ ((v ^ DAT_00512f38) & 0x40);

    g_game->brightness = DAT_00512f3a;
    if (!(g_game->field_2a44 & 4)) {
        g_game->width = DAT_00512f4d;
        g_game->height = DAT_00512f51;
    }

    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->SetWaveVolume(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->SetAuxVolume(g_game->volume2 << 10, 0);
}
