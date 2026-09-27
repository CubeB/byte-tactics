// Decompiled by space-bunny-free. Names are provisional.
// Resolves a unit definition slot number to a unit type id. The parsed text
// file can name the type of each slot with the key "UTYPENAME<id>"; when that
// key is present the name it holds is looked up in the unit type table by
// FUN_00488b10. Otherwise the id'th unit type whose +0x241 flags do not have
// bit 5 set is taken, and its table index minus one is returned.
#include <stdio.h>

#pragma pack(push, 1)
struct UnitType_0043a360 {            // 0x249 bytes
    char unknown_0[0x241];
    unsigned char flags;              // +0x241
    char unknown_242[0x249 - 0x242];
};

struct Game_0043a360 {
    char unknown_0[0x1438f];
    int unitTypeCount;                // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    UnitType_0043a360* unitTypes;     // +0x1439b
};
#pragma pack(pop)

extern Game_0043a360* g_game;

class Class_004b48f0 {
public:
    int FUN_004b48f0(char* name);
};

class Class_004b48a0 {
public:
    char* FUN_004b48a0(char* name, char* def);
};

short __stdcall FUN_00488b10(char* name);

// FUNCTION: 0x43a360
short __stdcall FUN_0043a360(Class_004b48f0* file, unsigned short id)
{
    char key[0x80];
    sprintf(key, "UTYPENAME%4d", id);
    if (file->FUN_004b48f0(key))
        return FUN_00488b10(((Class_004b48a0*)file)->FUN_004b48a0(key, 0));
    int i, n = 0;                     // n counts every table entry, k only the
    unsigned short k = 0;             // ones without flag bit 5
    for (i = 1; i < g_game->unitTypeCount; i++, n++) {
        if (!(g_game->unitTypes[(unsigned short)i].flags & 0x20)) {
            if (k == id)
                return n;
            k++;
        }
    }
    return 0;
}
