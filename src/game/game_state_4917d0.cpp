// Decompiled by Sonnet 5.5. Names are provisional.
// Resets the game state for a new match: clears the scratch fields and flags
// in g_game, runs the per-subsystem reset functions, allocates the three
// transform point buffers, and zeroes the per-player counters and the input
// history.
#include <string.h>

class Mission {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Player_004917d0 {
    char unknown_0[0x140];
    int field_140;                     // +0x140
    char unknown_144[0x14b - 0x144];
};

struct Zero11_004917d0 {
    int a;
    int b;
    short c;
    char d;

    void Clear()
    {
        a = 0;
        b = 0;
        c = 0;
        d = 0;
    }
};

struct Game {
    char unknown_0[0x1b63 - 0x0];
    Player_004917d0 players[10];   // +0x1b63
    char unknown_2851[0x2a46 - 0x2851];
    unsigned char field_2a46;   // +0x2a46
    char unknown_2a47[0x2bee - 0x2a47];
    unsigned short pad_2bee : 5;
    unsigned short flags_2bee : 3;
    unsigned short rest_2bee : 8;   // +0x2bee
    char unknown_2bf0[0x2bf1 - 0x2bf0];
    Zero11_004917d0 zero_2bf1;   // +0x2bf1
    char unknown_2bfc[0x2cba - 0x2bfc];
    unsigned short field_2cba;   // +0x2cba
    char unknown_2cbc[0x2cc3 - 0x2cbc];
    unsigned char field_2cc3;   // +0x2cc3
    unsigned short field_2cc4;   // +0x2cc4
    unsigned char pad_2cc6 : 5;
    unsigned char bit5_2cc6 : 1;
    unsigned char bit6_2cc6 : 1;
    unsigned char rest_2cc6 : 1;   // +0x2cc6
    char unknown_2cc7[0x14383 - 0x2cc7];
    void* xform;   // +0x14383
    void* projected;   // +0x14387
    void* assem;   // +0x1438b
    char unknown_1438f[0x37ebe - 0x1438f];
    unsigned short bit0_37ebe : 1;
    unsigned short pad_37ebe : 10;
    unsigned short bit11_37ebe : 1;
    unsigned short rest_37ebe : 4;   // +0x37ebe
    char unknown_37ec0[0x37ec4 - 0x37ec0];
    int field_37ec4;   // +0x37ec4
    int field_37ec8;   // +0x37ec8
    char unknown_37ecc[0x37f2f - 0x37ecc];
    unsigned short pad_37f2f : 7;
    unsigned short bit7_37f2f : 1;
    unsigned short bit8_37f2f : 1;
    unsigned short bit9_37f2f : 1;
    unsigned short rest_37f2f : 6;   // +0x37f2f
    char unknown_37f31[0x38a37 - 0x37f31];
    unsigned int field_38a37;   // +0x38a37
    char unknown_38a3b[0x38a43 - 0x38a3b];
    int field_38a43;   // +0x38a43
    int field_38a47;   // +0x38a47
    short field_38a4b;   // +0x38a4b
    short field_38a4d;   // +0x38a4d
    char unknown_38a4f[0x391e9 - 0x38a4f];
    Mission* net;          // +0x391e9
    char unknown_391ed[0x3923b - 0x391ed];
    unsigned short pad0_3923b : 2;
    unsigned short bit2_3923b : 1;
    unsigned short pad1_3923b : 1;
    unsigned short bit4_3923b : 1;
    unsigned short bit5_3923b : 1;
    unsigned short bit6_3923b : 1;
    unsigned short rest_3923b : 9;   // +0x3923b
    char unknown_3923d[0x39249 - 0x3923d];
    int field_39249;   // +0x39249
};
#pragma pack(pop)

extern Game* g_game;
extern unsigned int DAT_0051f2d8;
extern unsigned int DAT_0051f2dc;
extern int DAT_0051e710[30];

void FUN_00463c80();
void LoadLightBar();
void ResetSpeech();
void LoadTextureGafs();
void LoadFeatureFileList();
void InitUnitCategories();
void FUN_00440930();
void CreateParticleLists();
void AllocWeaponArray();
void LoadWeaponTypes();
void LoadTntMap();
void FUN_0041c2b0();
void LoadUnitTypes();
void LoadDownloadMenus();
void AllocateUnitMemory();
void ResolveFeatureLinks();
void FreeFeatureFileList();
void BuildAllPassMaps();
void UpdateWind();
void InitMeteors();
void InitRadar();
void InitPlayers();
void FUN_0044f6a0();
void InitExplosions();
void FUN_00419560();
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
unsigned int GetTicks();

// FUNCTION: 0x4917d0
void FUN_004917d0()
{
    FUN_00463c80();
    memset(&g_game->zero_2bf1, 0, 11);
    g_game->field_2cc3 = 1;
    g_game->field_2cc4 = 0;
    g_game->bit5_2cc6 = 0;
    g_game->bit6_2cc6 = 0;
    g_game->field_2cba = 0;
    g_game->bit0_37ebe = 0;
    g_game->bit11_37ebe = 0;
    g_game->field_39249 = 0;
    g_game->bit9_37f2f = 0;
    g_game->bit7_37f2f = 0;
    g_game->bit8_37f2f = 0;
    LoadLightBar();
    ResetSpeech();
    LoadTextureGafs();
    LoadFeatureFileList();
    InitUnitCategories();
    FUN_00440930();
    CreateParticleLists();
    AllocWeaponArray();
    LoadWeaponTypes();
    LoadTntMap();
    FUN_0041c2b0();
    LoadUnitTypes();
    LoadDownloadMenus();
    AllocateUnitMemory();
    ResolveFeatureLinks();
    FreeFeatureFileList();
    BuildAllPassMaps();
    g_game->field_37ec8 = 5000;
    g_game->field_37ec4 = 0;
    UpdateWind();
    g_game->xform = FUN_004d83b0("TEMP XFORM PTS", 0x960);
    g_game->projected = FUN_004d83b0("TEMP PROJECTED PTS", 0x640);
    g_game->assem = FUN_004d83b0("ASSEM PTS", 0xa0);
    g_game->field_38a37 = GetTicks();
    g_game->field_38a47 = 0;
    if (g_game->net->FUN_00435100() == 3) {
        g_game->field_38a4b = 10;
        g_game->field_38a4d = 10;
    }
    g_game->field_38a43 = 0;
    InitMeteors();
    InitRadar();
    InitPlayers();
    FUN_0044f6a0();
    InitExplosions();
    FUN_00419560();
    for (int i = 0; i < 10; i++)
        g_game->players[i].field_140 = 0;
    g_game->bit2_3923b = 0;
    g_game->bit4_3923b = 0;
    g_game->bit5_3923b = 0;
    g_game->bit6_3923b = 0;
    g_game->flags_2bee = 0;
    g_game->field_2a46 = 0xff;
    DAT_0051f2dc = 0;
    memset(DAT_0051e710, 0, sizeof(DAT_0051e710));
    DAT_0051f2d8 = 0;
}
