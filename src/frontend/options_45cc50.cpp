// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads the saved game settings (globals around 0x512f42) into the game and
// applies them to the sound object at g_game+0x10, then copies the remaining
// saved options (as 0x45ca50 does) and runs the post-load fixups (0x45cae0).

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
struct Game_0045cc50 {
    char unknown_0[0x10];
    void* field_10;                    // +0x10
    char unknown_14[0x1434d - 0x14];
    unsigned char field_1434d;         // +0x1434d
    char unknown_1434e[0x37efa - 0x1434e];
    int field_37efa;                   // +0x37efa
    char unknown_37efe[0x37f08 - 0x37efe];
    int brightness;                    // +0x37f08
    int volume1;                       // +0x37f0c
    int volume2;                       // +0x37f10
    unsigned short flags;              // +0x37f14
    unsigned char field_37f16;         // +0x37f16
    unsigned char field_37f17;         // +0x37f17
    unsigned char field_37f18;         // +0x37f18
    char unknown_37f19[0x37f23 - 0x37f19];
    int field_37f23;                   // +0x37f23
    int field_37f27;                   // +0x37f27
    char unknown_37f2b[0x38a4b - 0x37f2b];
    short field_38a4b;                 // +0x38a4b
    short field_38a4d;                 // +0x38a4d
};
#pragma pack(pop)

extern Game_0045cc50* g_game;
extern int DAT_00512f2c;
extern int DAT_00512f42;
extern unsigned short DAT_00512f46;
extern char DAT_00512f48;
extern char DAT_00512f49;
extern char DAT_00512f4a;
extern int DAT_00512f55;
extern int DAT_00512f59;
extern int DAT_00512f6d;
extern char DAT_00512f71;
extern char DAT_00512f75;
extern int DAT_00512fd9;

void FUN_0045c820();
void FUN_0045cae0();
void __stdcall FUN_004ba590(float value);

// FUNCTION: 0x45cc50
void FUN_0045cc50()
{
    FUN_0045c820();
    g_game->volume2 = DAT_00512f42;
    ((Class_004ce3e0*)g_game->field_10)->FUN_004ce3e0(&DAT_00512f75);
    g_game->field_37f16 = DAT_00512f48;
    ((Class_004ce7a0*)g_game->field_10)->FUN_004ce7a0(g_game->field_37f16);
    if (((unsigned char)g_game->flags ^ (unsigned char)DAT_00512f46) & 1) {
        ((Class_004cdb40*)g_game->field_10)->FUN_004cdb40();
    }
    unsigned short f = g_game->flags;
    g_game->flags = f ^ ((f ^ DAT_00512f46) & 1);
    ((Class_004ce580*)g_game->field_10)->FUN_004ce580(DAT_00512fd9);
    FUN_004ba590(0.5 - g_game->brightness * -0.041666668f);
    ((Class_004d0070*)g_game->field_10)->FUN_004d0070(g_game->volume1 << 10);
    ((Class_004d00d0*)g_game->field_10)->FUN_004d00d0(g_game->volume2 << 10, 0);
    g_game->field_37f23 = DAT_00512f55;
    g_game->field_38a4b = DAT_00512f6d;
    g_game->field_38a4d = DAT_00512f6d;
    g_game->field_1434d = DAT_00512f71;
    g_game->field_37efa = DAT_00512f2c;
    g_game->field_37f17 = DAT_00512f49;
    g_game->field_37f18 = DAT_00512f4a;
    g_game->field_37f27 = DAT_00512f59;
    FUN_0045cae0();
}
