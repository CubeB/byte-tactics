// Decompiled by space-bunny-free. Names are provisional.
// Reads the "MAXLINES" slider into the max lines field of g_game and shows it
// as "<n>", or "None" when it is 0. The slider value is the same inlined
// SliderValue helper as in 0x45c330; it is evaluated twice, and only the second
// result is kept unless the first one was negative (both are identical, so the
// store is a clamp to 0 either way).
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_4a0200 {
    char unknown_0[0x136];
    short steps;                       // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                           // +0x13c
    short pos;                         // +0x140
};

struct Game {
    char unknown_0[0x37f27];
    int max_lines;                     // +0x37f27
};
#pragma pack(pop)

struct Holder_0045c220 {
    int unknown_0;
    Entry_4a0200* entries;             // +0x4
};

struct Object_0045c220 {
    char unknown_0[0x18];
    Holder_0045c220* holder;           // +0x18
};

extern Game* g_game;

Entry_4a0200* __stdcall FUN_004a0200(Entry_4a0200* entries, char* name);
void __stdcall FUN_004a0bf0(Object_0045c220* obj, char* name, char* text, int param_4);
void __stdcall FUN_0049fa90(Object_0045c220* obj);

static inline int SliderValue(Entry_4a0200* e)
{
    if (e->steps <= 1)
        return 0;
    return (int)((float)e->pos / (e->steps - 1) * e->max);
}

// FUNCTION: 0x45c220
void __stdcall FUN_0045c220(Object_0045c220* obj, int unused)
{
    char text[20];
    Entry_4a0200* e = FUN_004a0200(obj->holder->entries, "MAXLINES");
    if (e != 0) {
        int v = SliderValue(e);
        g_game->max_lines = v < 0 ? 0 : SliderValue(e);
        FUN_0049fa90(obj);
    }
    if (g_game->max_lines != 0)
        sprintf(text, "%d", g_game->max_lines);
    else
        strcpy(text, "None");
    FUN_004a0bf0(obj, "MAXLINESTEXT", text, 0);
}
