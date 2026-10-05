// Decompiled by Opus. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Obj_00423bf0 {
    char unknown_0[0x28];
    short x;                         // +0x28
    short y;                         // +0x2a
    unsigned short entry;            // +0x2c
};

struct Entry_00423bf0 {
    char unknown_0[0xf6];
    unsigned short id;               // +0xf6
    char unknown_f8[0x100 - 0xf8];
};

struct Game {
    char unknown_0[0x1426f];
    Entry_00423bf0* entries;         // +0x1426f
};
#pragma pack(pop)

extern Game* g_game;

void* __stdcall GetMapCell(int x, int y);
void __stdcall RemoveFeature(void* target, int flag);
void* __stdcall PlaceFeature(void* target, unsigned short id, void* pos, void* field_64, unsigned char owner);

// FUNCTION: 0x423bf0
void __stdcall ReplaceFeatureWithBurnt(Obj_00423bf0* obj)
{
    void* target = GetMapCell(obj->x, obj->y);
    if (target != 0) {
        unsigned short id = g_game->entries[obj->entry].id;
        RemoveFeature(target, 0);
        if (id != 0xffff) {
            PlaceFeature(target, id, 0, 0, 10);
        }
    }
}
