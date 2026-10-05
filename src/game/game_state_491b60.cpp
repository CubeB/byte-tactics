// Decompiled by deepseek-v4.1-flash. Names are provisional.

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_004ce690 {
public:
    void FUN_004ce690(int param_1);
};

class Class_004ced40 {
public:
    void FUN_004ced40();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x10];
    Class_004ce690* field_10;             // +0x10
    char unknown_14[0x2a44 - 0x14];
    unsigned short flags_2a44;            // +0x2a44
    char unknown_2a46[0x14383 - 0x2a46];
    void* field_14383;                    // +0x14383
    void* field_14387;                    // +0x14387
    void* field_1438b;                    // +0x1438b
    char unknown_1438f[0x391e9 - 0x1438f];
    Class_00435100* field_391e9;          // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_0041dc20();
void FUN_00437d30();
void FreeUnitMemory();
void FUN_00471de0();
void FUN_00420960();
void FUN_0044f6e0();
void FUN_00464a00();
void FUN_00466aa0();
void FUN_00483dd0();
void __cdecl FUN_004d85a0(void* param_1);
void FreeDownloadMenus();
void FreeUnitTypes();
void FUN_0042f3a0();
void FUN_0042a570();
void FUN_00499a80();
void FreeMovementClasses();
void FreeUnitCategories();
void CloseNetSession();

// FUNCTION: 0x491b60
void FUN_00491b60()
{
    g_game->flags_2a44 &= 0xfffb;
    ((Class_004ced40*)g_game->field_10)->FUN_004ced40();
    g_game->field_10->FUN_004ce690(4);
    FUN_0041dc20();
    FUN_00437d30();
    FreeUnitMemory();
    FUN_00471de0();
    FUN_00420960();
    FUN_0044f6e0();
    FUN_00464a00();
    FUN_00466aa0();
    FUN_00483dd0();
    FUN_004d85a0(g_game->field_1438b);
    FUN_004d85a0(g_game->field_14387);
    FUN_004d85a0(g_game->field_14383);
    g_game->field_1438b = 0;
    g_game->field_14387 = 0;
    g_game->field_14383 = 0;
    FreeDownloadMenus();
    FreeUnitTypes();
    FUN_0042f3a0();
    FUN_0042a570();
    FUN_00499a80();
    FreeMovementClasses();
    FreeUnitCategories();
    if (g_game->field_391e9->FUN_00435100() == 3) {
        CloseNetSession();
    }
}
