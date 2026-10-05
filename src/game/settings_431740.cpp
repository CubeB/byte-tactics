// Decompiled by space-bunny-free. Names are provisional.
// Applies the unit-type restrictions listed in the embedded list entry that
// Mission returns for index 6: clears bit 23 (0x800000) on every unit
// type, then sets it on each type named by the entry.

#include <string.h>

class TdfFile {
public:
    int field_0;
    void* current;                     // +0x4
    int field_8;
    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
    void ResetCurrentRecord();
    int SelectRecordAt(int index);
};

class TdfRecord {
public:
    const char* field_0;
    void CopyRecordName(char* dest, size_t count);
};

class Mission {
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
    TdfFile parser;
    char name[256];
    char* file = ((Mission*)g_game->field_391e9)->FUN_004356c0(6);
    if (file == 0)
        return;
    if (!((TdfFile*)&parser)->LoadFile(file))
        return;
    {
        ProtectBlockReadWrite(g_game->defs);
        for (int i = 1; i < g_game->count; i++)
            g_game->defs[i].flags &= 0xff7fffff;
        ((TdfFile*)&parser)->ResetCurrentRecord();
        for (int j = 0; ((TdfFile*)&parser)->SelectRecordAt(j); j++, ((TdfFile*)&parser)->ResetCurrentRecord()) {
            ((TdfRecord*)parser.current)->CopyRecordName(name, 0x100);
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
