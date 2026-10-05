// Decompiled by space-bunny-free. Names are provisional.
// Reads the unit definition and script of a unit type from disk, both under
// "units/<TypeName>/", and stores the script at UnitType+0x18e: first the
// definition file ("FBI") when it exists, then the script file ("COB").
// The whole table at g_game+0x1439b (0x249-byte entries) is locked while the
// files are read.
#pragma pack(push, 1)

struct UnitType_0042d1f0 {
    char unknown_0[0x20];
    char name[0x221];                  // +0x20, up to 0x241
    unsigned int flags;                // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Game {
    char unknown_0[0x1439b];
    UnitType_0042d1f0* unitTypes;      // +0x1439b
};
#pragma pack(pop)

struct CobFile_0042d1f0 {
    char unknown_0[0x18e];
    int field_18e;                     // +0x18e
};

extern Game* g_game;

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall HAPI_FileLengthByName(char* path);
void __stdcall LoadUnitFbi(char* path, UnitType_0042d1f0* type);
void __stdcall FreeCobScript(CobFile_0042d1f0* cob);
CobFile_0042d1f0* __stdcall LoadCobScript(char* path);
void __cdecl ProtectBlockReadWrite(void* param_1);
void __cdecl ProtectBlockReadOnly(void* param_1);

// FUNCTION: 0x42d1f0
void __stdcall ReloadUnitType(unsigned short index)
{
    if (index == 0)
        return;
    UnitType_0042d1f0* type = &g_game->unitTypes[index];
    if ((type->flags & 0x800000) == 0)
        return;
    ProtectBlockReadWrite(g_game->unitTypes);
    char path[256];
    BuildDataPath(path, "units", type->name, "FBI");
    if (HAPI_FileLengthByName(path)) {
        LoadUnitFbi(path, type);
        FreeCobScript(*(CobFile_0042d1f0**)((char*)type + 0x18e));
        BuildDataPath(path, "scripts", type->name, "COB");
        CobFile_0042d1f0* cob = LoadCobScript(path);
        *(CobFile_0042d1f0**)((char*)type + 0x18e) = cob;
        ProtectBlockReadOnly(g_game->unitTypes);
    } else {
        ProtectBlockReadOnly(g_game->unitTypes);
    }
}
