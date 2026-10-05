// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads the saved audio settings (globals around 0x512f42) into the game and
// applies them to the sound object at g_game+0x10.
//
// DAT_00512f46 is the saved "sound enabled" flag byte. It is declared int
// because the flag-sync xor below is computed at int width: `and ecx, 1` then
// `xor ecx, eax` (a byte-typed operand would give `and cl, 1; movsx dx, cl`).

class Class_004cdb40 {
public:
    void FUN_004cdb40();
};

class Class_004ce3e0 {
public:
    void FUN_004ce3e0(const void* src);
};

class Class_004ce580 {
public:
    void FUN_004ce580(int value);
};

class Class_004ce7a0 {
public:
    int FUN_004ce7a0(int value);
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
    void* field_10;                    // +0x10
    char unknown_14[0x37f08 - 0x14];
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
    unsigned short flags;              // +0x37f14
    unsigned char field_37f16;         // +0x37f16
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_00512f42;
extern int DAT_00512f46;
extern char DAT_00512f48;
extern char DAT_00512f75;
extern int DAT_00512fd9;

void __stdcall FUN_004ba590(float value);

// FUNCTION: 0x45c950
void FUN_0045c950()
{
    g_game->volume2 = DAT_00512f42;
    ((Class_004ce3e0*)g_game->field_10)->FUN_004ce3e0(&DAT_00512f75);
    g_game->field_37f16 = DAT_00512f48;
    ((Class_004ce7a0*)g_game->field_10)->FUN_004ce7a0(g_game->field_37f16);
    if (((unsigned char)g_game->flags ^ (unsigned char)DAT_00512f46) & 1) {
        ((Class_004cdb40*)g_game->field_10)->FUN_004cdb40();
    }
    unsigned short f = g_game->flags;
    f = f ^ ((f ^ DAT_00512f46) & 1);
    g_game->flags = f;
    ((Class_004ce580*)g_game->field_10)->FUN_004ce580(DAT_00512fd9);
    FUN_004ba590(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->field_10)->FUN_004d0070(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->field_10)->FUN_004d00d0(g_game->volume2 << 10, 0);
}
