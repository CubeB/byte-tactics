// Decompiled by Opus. Names are provisional.
// Makes sure the text file has a "UTYPENAME<id>" key: if it is missing and
// `id` is a valid unit type, writes the type's name under that key.
#include <stdio.h>

#pragma pack(push, 1)
struct UnitType_0043a2d0 {             // 0x249 bytes
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
    char unknown_40[0x249 - 0x40];
};

struct Game {
    char unknown_0[0x1438f];
    int unitTypeCount;                 // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_0043a2d0* unitTypes;      // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

class Class_004b48f0 {
public:
    int FUN_004b48f0(const char* param_1);
};

class Class_004b4750 {
public:
    void FUN_004b4750(char* key, char* value);
};

// FUNCTION: 0x43a2d0
void __stdcall FUN_0043a2d0(Class_004b48f0* file, unsigned short id)
{
    char key[0x80];
    sprintf(key, "UTYPENAME%4d", id);
    if (!file->FUN_004b48f0(key) && id >= 1 && id < g_game->unitTypeCount)
        ((Class_004b4750*)file)->FUN_004b4750(key, g_game->unitTypes[id].name);
}
