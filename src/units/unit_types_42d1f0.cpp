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

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bbc40(char* path);
void __stdcall FUN_0042bf40(char* path, UnitType_0042d1f0* type);
void __stdcall FUN_004b2540(CobFile_0042d1f0* cob);
CobFile_0042d1f0* __stdcall FUN_004b2450(char* path);
void __cdecl FUN_004d8780(void* param_1);
void __cdecl FUN_004d8710(void* param_1);

// FUNCTION: 0x42d1f0
void __stdcall FUN_0042d1f0(unsigned short index)
{
    if (index == 0)
        return;
    UnitType_0042d1f0* type = &g_game->unitTypes[index];
    if ((type->flags & 0x800000) == 0)
        return;
    FUN_004d8780(g_game->unitTypes);
    char path[256];
    FUN_004290f0(path, "units", type->name, "FBI");
    if (FUN_004bbc40(path)) {
        FUN_0042bf40(path, type);
        FUN_004b2540(*(CobFile_0042d1f0**)((char*)type + 0x18e));
        FUN_004290f0(path, "scripts", type->name, "COB");
        CobFile_0042d1f0* cob = FUN_004b2450(path);
        *(CobFile_0042d1f0**)((char*)type + 0x18e) = cob;
        FUN_004d8710(g_game->unitTypes);
    } else {
        FUN_004d8710(g_game->unitTypes);
    }
}
