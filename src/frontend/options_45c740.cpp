// Decompiled by space-bunny-free. Names are provisional.
// Sets the five share flag bits in the flags word at +0x37f06, sets the
// brightness at +0x37f08, and (unless bit 2 of +0x2a44 is set) the screen
// width and height at +0x37f1b/+0x37f1f and clears bit 6 of the flags word.
// Then applies the brightness and the two volume levels to the sound object.

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
    void* sound;                      // +0x10
    char unknown_14[0x2a44 - 0x14];
    unsigned short pad_2a44 : 2;      // +0x2a44
    unsigned short flag_2a44 : 1;
    unsigned short rest_2a44 : 13;
    char unknown_2a46[0x37f06 - 0x2a46];
    unsigned short bit0 : 1;          // +0x37f06
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short bit3 : 1;
    unsigned short bit4 : 1;
    unsigned short bit5 : 1;
    unsigned short bit6 : 1;
    unsigned short rest : 9;
    int brightness;                   // +0x37f08
    int volume1;                      // +0x37f0c
    int volume2;                      // +0x37f10
    char unknown_37f14[0x37f1b - 0x37f14];
    int width;                        // +0x37f1b
    int height;                       // +0x37f1f
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall SetBrightness(float value);

// FUNCTION: 0x45c740
void FUN_0045c740()
{
    g_game->bit1 = 1;
    g_game->bit2 = 1;
    g_game->bit3 = 1;
    g_game->bit4 = 1;
    g_game->bit5 = 1;
    g_game->brightness = 12;
    if (!g_game->flag_2a44) {
        g_game->width = 640;
        g_game->height = 480;
        g_game->bit6 = 0;
    }
    SetBrightness(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->FUN_004d0070(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->FUN_004d00d0(g_game->volume2 << 10, 0);
}
