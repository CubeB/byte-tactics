// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct UnitDef_0040bdb0 {
    char unknown_0[0xa0];
    char field_a0[0x152 - 0xa0];       // +0xa0
    int count;                         // +0x152
    unsigned short* ids;               // +0x156
    char unknown_15a[0x249 - 0x15a];
};

struct Unit {
    char unknown_0[0x92];
    UnitDef_0040bdb0* def;             // +0x92
};

struct Game {
    char unknown_0[0x1439b];
    UnitDef_0040bdb0* defs;            // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall GetBuildRating(unsigned int player, unsigned short id);
int __stdcall RandomInt(int range);

// FUNCTION: 0x40bdb0
unsigned short __stdcall ChooseBuildOption(unsigned int player, Unit* unit)
{
    unsigned short chosen = 0;
    int total = 0;
    for (int i = 0; i < unit->def->count; i++) {
        unsigned short id = unit->def->ids[i];
        int r = GetBuildRating(player, id);
        if (r > 0) {
            total += r;
            if (RandomInt(total) < r)
                chosen = id;
        }
    }
    if (chosen != 0) {
        if (strcmp(unit->def->field_a0, g_game->defs[chosen].field_a0) != 0)
            chosen = 0;
    }
    return chosen;
}
