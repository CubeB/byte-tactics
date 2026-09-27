// Decompiled by Opus. Names are provisional.
// Reads the "TXTSCROL" slider into the text scroll time and shows it as
// "<n> secs" (the slider value is the inlined FUN_0045ba20, as in 0x45bea0).
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_4a0200 {
    char unknown_0[0x136];
    short steps;                       // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                           // +0x13c
    short pos;                         // +0x140
};

struct Game_0045c330 {
    char unknown_0[0x37f23];
    int scroll_time;                   // +0x37f23
};
#pragma pack(pop)

struct Holder_0045c330 {
    int unknown_0;
    Entry_4a0200* entries;             // +0x4
};

struct Object_0045c330 {
    char unknown_0[0x18];
    Holder_0045c330* holder;           // +0x18
};

extern Game_0045c330* g_game;

Entry_4a0200* __stdcall FUN_004a0200(Entry_4a0200* entries, char* name);
void __stdcall FUN_004a0bf0(Object_0045c330* obj, char* name, char* text, int param_4);
void __stdcall FUN_0049fa90(Object_0045c330* obj);

static inline int SliderValue(Entry_4a0200* e)
{
    if (e->steps <= 1)
        return 0;
    return (int)((float)e->pos / (e->steps - 1) * e->max);
}

// FUNCTION: 0x45c330
void __stdcall FUN_0045c330(Object_0045c330* obj, int unused)
{
    char text[20];
    Entry_4a0200* e = FUN_004a0200(obj->holder->entries, "TXTSCROL");
    if (e != 0) {
        g_game->scroll_time = SliderValue(e);
        sprintf(text, "%d secs", g_game->scroll_time);
        FUN_004a0bf0(obj, "TEXTSCROLLTEXT", text, 0);
        FUN_0049fa90(obj);
    }
}
