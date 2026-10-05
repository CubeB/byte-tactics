// Decompiled by space-bunny-free. Names are provisional.
// Applies the unit-type restrictions listed in the embedded list entry that
// Class_004356c0 returns for index 6: clears bit 23 (0x800000) on every unit
// type, then sets it on each type named by the entry.

#include <string.h>

class Class_004c2ea0 {
public:
    int field_0;
    void* current;                     // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int LoadFile(char* file);
};

class Class_004c3e10 {
public:
    char unknown_0[4];
    void* field_0x4;
    void ResetCurrentRecord();
};

class Class_004c3490 {
public:
    int SelectRecordAt(int index);
};

class Class_004c4420 {
public:
    const char* field_0;
    void CopyRecordName(char* dest, size_t count);
};

class Class_004356c0 {
public:
    char* FUN_004356c0(int index);
};

#pragma pack(push, 1)
struct UnitDef_00431740 {
    char unknown_0[0x20];
    char name[0x241 - 0x20];           // +0x20
    unsigned int flags;                // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Game {
    char unknown_0[0x1438f];
    int count;                         // +0x1438f
    char unknown_14393[8];
    UnitDef_00431740* defs;            // +0x1439b
    char unknown_1439f[0x391e9 - 0x1439f];
    void* field_391e9;                 // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl ProtectBlockReadWrite(void* param_1);
void __cdecl ProtectBlockReadOnly(void* param_1);

// FUNCTION: 0x431740
void FUN_00431740()
{
    Class_004c2ea0 parser;
    char name[256];
    char* file = ((Class_004356c0*)g_game->field_391e9)->FUN_004356c0(6);
    if (file == 0)
        return;
    if (!((Class_004c2f60*)&parser)->LoadFile(file))
        return;
    {
        ProtectBlockReadWrite(g_game->defs);
        for (int i = 1; i < g_game->count; i++)
            g_game->defs[i].flags &= 0xff7fffff;
        ((Class_004c3e10*)&parser)->ResetCurrentRecord();
        for (int j = 0; ((Class_004c3490*)&parser)->SelectRecordAt(j); j++, ((Class_004c3e10*)&parser)->ResetCurrentRecord()) {
            ((Class_004c4420*)parser.current)->CopyRecordName(name, 0x100);
            for (int k = 0; k < g_game->count; k++) {
                if (_strcmpi(g_game->defs[k].name, name) == 0) {
                    g_game->defs[k].flags |= 0x800000;
                    break;
                }
            }
        }
        ProtectBlockReadOnly(g_game->defs);
    }
}
