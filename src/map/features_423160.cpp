// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Places the mission's features: for every placement record with a name,
// find the feature definition by name (loading it with LoadFeatureType when it
// is not in the table yet), then put it on the map cell at the placement's
// position, centred on its footprint unless flags bit 0 is set.
// Needed for the register allocation.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Point16 {
    short x;
    short z;
};

struct Placement {
    char name[0x80];
    int x;                          // +0x80
    int y;                          // +0x84
};

struct Feature {
    char name[0x94];
    Point16 footprint;              // +0x94
    char unknown_98[0xfe - 0x98];
    unsigned short flags;           // +0xfe
};

struct Mission {
    char unknown_0[0xdbc];
    Placement* placements;          // +0xdbc
    int placementCount;             // +0xdc0
};

struct Game {
    char unknown_0[0x14253];
    int featureCount;               // +0x14253
    char unknown_14257[0x1426f - 0x14257];
    Feature* features;              // +0x1426f
    char unknown_14273[0x391e9 - 0x14273];
    Mission* mission;               // +0x391e9
};
#pragma pack(pop)

struct Cell;

extern Game* g_game;

unsigned short __stdcall LoadFeatureType(char* name);
Cell* __stdcall GetMapCell(int x, int y);
void* __stdcall PlaceFeature(Cell* cell, unsigned short feature, void* pos, void* rot, unsigned char owner);

static inline int FindFeature(char* name)
{
    for (int i = 0; i < g_game->featureCount; i++) {
        if (_strcmpi(name, g_game->features[i].name) == 0)
            return i;
    }
    return 0xffff;
}

// FUNCTION: 0x423160
void PlaceMissionFeatures(void)
{
    for (int i = 0; i < g_game->mission->placementCount; i++) {
        Placement* p = &g_game->mission->placements[i];
        if (p->name[0] == 0)
            continue;
        unsigned short id = (unsigned short)FindFeature(p->name);
        if (id == 0xffff) {
            id = LoadFeatureType(p->name);
            if (id == 0xffff)
                continue;
        }
        Feature* f = &g_game->features[id];
        // A cell local with one GetMapCell call per branch, not x/y locals and one call.
        Cell* cell;
        if (f->flags & 1)
            cell = GetMapCell(p->x, p->y);
        else
            cell = GetMapCell(p->x - f->footprint.x / 2, p->y - f->footprint.z / 2);
        PlaceFeature(cell, id, 0, 0, 10);
    }
}
