// Decompiled by space-bunny-free. Names are provisional.
// Applies the "VIDSLDR" slider: picks the current resolution entry from the
// slider's table of 3 dword entries, writes its "%d X %d" text into the
// "VIDVAL" name, and copies the resolution into the game state.

#include <stdio.h>

#pragma pack(push, 1)
struct Res_0045bbf0 {
    int x;                            // +0x0
    int y;                            // +0x4
    int unknown_8;                    // +0x8
};

struct Holder_0045bbf0;

struct Slider_0045bbf0 {
    char unknown_0[0x136];
    short steps;                      // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                          // +0x13c
    short pos;                        // +0x140
    char unknown_142[0x14a - 0x142];
    Holder_0045bbf0* list;            // +0x14a
};

struct Holder_0045bbf0 {
    int unknown_0;
    Res_0045bbf0* entries;            // +0x4
};

struct Object_0045bbf0 {
    char unknown_0[0x18];
    Holder_0045bbf0* holder;           // +0x18
};

struct Game_0045bbf0 {
    char unknown_0[0x519];
    char menu[0x37f1b - 0x519];
    int resolutionX;                  // +0x37f1b
    int resolutionY;                  // +0x37f1f
};
#pragma pack(pop)

extern Game_0045bbf0* g_game;

Slider_0045bbf0* __stdcall FUN_004a0200(Res_0045bbf0* entries, char* name);
char* __stdcall FUN_004a0180(Res_0045bbf0* entries, char* name);
void __stdcall FUN_0049fa90(void* menu);

static inline int SliderValue(Slider_0045bbf0* e)
{
    if (e->steps <= 1)
        return 0;
    return (int)((float)e->pos / (e->steps - 1) * e->max);
}

// FUNCTION: 0x45bbf0
void __stdcall FUN_0045bbf0(Object_0045bbf0* obj, int unused)
{
    // The original reads e->list before testing e for null, so this load must
    // stay above the if. If FUN_004a0200 ever returned 0 the original would
    // have read through a null pointer; that is a real bug in the game code.
    Slider_0045bbf0* e = FUN_004a0200(obj->holder->entries, "VIDSLDR");
    Holder_0045bbf0* list = e->list;
    if (e != 0) {
        Res_0045bbf0* r = &list->entries[SliderValue(e)];
        sprintf(FUN_004a0180(obj->holder->entries, "VIDVAL") + 0xb6, "%d X %d", r->x, r->y);
        g_game->resolutionX = r->x;
        g_game->resolutionY = r->y;
    }
    FUN_0049fa90(&g_game->menu);
}
