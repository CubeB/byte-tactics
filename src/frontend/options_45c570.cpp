// Decompiled by space-bunny-free. Names are provisional.
// Sets up the sound state: volume1, three sound flag bits, a redraw of the
// sound object, more flag bits, then brightness and the two volume levels
// scaled by 1024 (same tail as 0x45bcc0).

class Class_004cfe90 {
public:
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
// Flag bits at +0x37f19, an unaligned unsigned short in the original, written
// both as individual bits and as a whole word.
struct Bits_0045c570 {
    unsigned short bit0 : 1;
    unsigned short bit1 : 1;
    unsigned short bit2 : 1;
    unsigned short bit3 : 1;
    unsigned short bit4 : 1;
    unsigned short bit5 : 1;
    unsigned short bit6 : 1;
};

union Flags_0045c570 {
    unsigned short word;
    Bits_0045c570 bits;
};

struct Game {
    char unknown_0[0x10];
    void* sound;                       // +0x10
    char unknown_14[0x37f08 - 0x14];
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
    char unknown_37f14[0x37f17 - 0x37f14];
    unsigned char f37f17;              // +0x37f17
    char unknown_37f18;
    Flags_0045c570 soundFlags;         // +0x37f19
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004ba590(float value);

// FUNCTION: 0x45c570
void FUN_0045c570()
{
    g_game->volume1 = 0x1b;
    g_game->soundFlags.bits.bit4 = 1;
    g_game->soundFlags.bits.bit5 = 1;
    g_game->soundFlags.bits.bit6 = 1;
    ((Class_004cfe90*)g_game->sound)->FUN_004cfe90();
    g_game->soundFlags.word = (g_game->soundFlags.word & 0xfff9) | 1;
    g_game->f37f17 = 10;
    FUN_004ba590(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->sound)->FUN_004d0070(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->sound)->FUN_004d00d0(g_game->volume2 << 10, 0);
}
