// Decompiled by deepseek-v4.1-flash. Names are provisional.

class Mission {
public:
    int GetGameType();
};

class Sound {
public:
    void SetTrackCategory(int param_1);
};

class Class_004ced40 {
public:
    void StopCdAudio();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    Sound* field_10;                      // +0x10
    char unknown_14[0x2a44 - 0x14];
    unsigned short flags_2a44;            // +0x2a44
    char unknown_2a46[0x14383 - 0x2a46];
    void* field_14383;                    // +0x14383
    void* field_14387;                    // +0x14387
    void* field_1438b;                    // +0x1438b
    char unknown_1438f[0x391e9 - 0x1438f];
    Mission* field_391e9;                 // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_0041dc20();
void EmptyShutdownPreCleanup();
void FreeUnitMemory();
void DestroyParticleLists();
void FreeExplosions();
void FUN_0044f6e0();
void FreePlayers();
void FreeRadar();
void FreeMapResources();
void __cdecl FUN_004d85a0(void* param_1);
void FreeDownloadMenus();
void FreeUnitTypes();
void FreeWeaponTypes();
void FUN_0042a570();
void FreeWeaponArray();
void FreeMovementClasses();
void FreeUnitCategories();
void CloseNetSession();

// FUNCTION: 0x491b60
void FUN_00491b60()
{
    g_game->flags_2a44 &= 0xfffb;
    ((Class_004ced40*)g_game->field_10)->StopCdAudio();
    g_game->field_10->SetTrackCategory(4);
    FUN_0041dc20();
    EmptyShutdownPreCleanup();
    FreeUnitMemory();
    DestroyParticleLists();
    FreeExplosions();
    FUN_0044f6e0();
    FreePlayers();
    FreeRadar();
    FreeMapResources();
    FUN_004d85a0(g_game->field_1438b);
    FUN_004d85a0(g_game->field_14387);
    FUN_004d85a0(g_game->field_14383);
    g_game->field_1438b = 0;
    g_game->field_14387 = 0;
    g_game->field_14383 = 0;
    FreeDownloadMenus();
    FreeUnitTypes();
    FreeWeaponTypes();
    FUN_0042a570();
    FreeWeaponArray();
    FreeMovementClasses();
    FreeUnitCategories();
    if (g_game->field_391e9->GetGameType() == 3) {
        CloseNetSession();
    }
}
