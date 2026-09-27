// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Applies saved option values to the game: the volume at +0x37f0c, the sound
// flag word at +0x37f19 (bits 4, 5 and 6 copied from DAT_00512f4b, bits 0-2
// after the sound object is switched by the low three bits), the byte at
// +0x37f17, then the brightness and both volume levels (same tail as
// 0x45c630 and 0x45bcc0).
//
// DAT_00512f4b is a byte in the original but is declared as unsigned int here:
// the compiler then keeps it in bl and emits the xor/and/xor bitfield-merge
// idiom the original used. The low-three-bit test reads it as a byte again.

class Class_004cfe80 {
public:
    char unknown_0[4];
    int field_4;

    void FUN_004cfe80();
};

class Class_004cfe90 {
public:
    char unknown_0[4];
    int field_4;

    void FUN_004cfe90();
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
struct Game_0045c820 {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0x37f08 - 0x14];
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
    char unknown_37f14[0x37f17 - 0x37f14];
    unsigned char field_37f17;         // +0x37f17
    char unknown_37f18[0x37f19 - 0x37f18];
    unsigned short field_37f19;        // +0x37f19
};
#pragma pack(pop)

extern Game_0045c820* g_game;
extern int DAT_00512f3e;
extern unsigned char DAT_00512f49;
extern unsigned int DAT_00512f4b;

void __stdcall FUN_004ba590(float value);

// FUNCTION: 0x45c820
void FUN_0045c820()
{
    g_game->volume1 = DAT_00512f3e;
    g_game->field_37f19 = (g_game->field_37f19 & ~0x10) | (DAT_00512f4b & 0x10);
    g_game->field_37f19 = (g_game->field_37f19 & ~0x20) | (DAT_00512f4b & 0x20);
    g_game->field_37f19 = (g_game->field_37f19 & ~0x40) | ((DAT_00512f4b & 0x20) << 1);
    if ((((unsigned char)DAT_00512f4b) & 7) == 2)
        ((Class_004cfe80*)g_game->sound)->FUN_004cfe80();
    else
        ((Class_004cfe90*)g_game->sound)->FUN_004cfe90();
    g_game->field_37f19 = (g_game->field_37f19 & ~7) | (DAT_00512f4b & 7);
    g_game->field_37f17 = DAT_00512f49;
    FUN_004ba590(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->FUN_004d0070(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->FUN_004d00d0(g_game->volume2 << 10, 0);
}
