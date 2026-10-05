// Decompiled by Opus. Names are provisional.
// Finds the GUI entry named after list entry `index` of the game's 0x249-byte
// table at +0x1439b and sets its text at +0xb6 to "+<n>", or clears it
// when n is 0.
#include <windows.h>
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_49ff10 {                  // 0x15b bytes
    char unknown_0[0xb6];
    char text[0x15b - 0xb6];           // +0xb6
};

struct Item_00419940 {                 // 0x249 bytes
    char unknown_0[0x20];
    char name[0x249 - 0x20];           // +0x20
};

struct Game {
    char unknown_0[0x1439b];
    Item_00419940* items;              // +0x1439b
};
#pragma pack(pop)

struct Data_00419940 {
    int unknown_0;
    Entry_49ff10* entries;             // +0x4
};

struct Object_00419940 {
    char unknown_0[0x18];
    Data_00419940* data;               // +0x18
};

extern Game* g_game;

Entry_49ff10* __stdcall FindGadgetOrNull(Entry_49ff10* entries, char* name);

// FUNCTION: 0x419940
void __stdcall SetBuildCountText(Object_00419940* obj, unsigned short index, int n)
{
    Entry_49ff10* e = FindGadgetOrNull(obj->data->entries, g_game->items[index].name);
    if (e) {
        if (n)
            sprintf(e->text, "+%d", n);
        else
            e->text[0] = 0;
    }
}
