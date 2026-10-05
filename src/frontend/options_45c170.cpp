// Decompiled by space-bunny-free. Names are provisional.
// Reads the "SCREEN" slider of the menu object and stores its value in
// g_game->field_1434d, writing 1 instead of any value of 1 or less, then
// marks the object changed (FUN_0049fa90 sets obj->field_cca = 1).

#pragma pack(push, 1)
struct Entry_45c170 {
    char unknown_0[0x136];
    short steps;                       // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                           // +0x13c
    short pos;                         // +0x140
};

struct Holder_45c170 {
    int unknown_0;
    Entry_45c170* entries;             // +0x4
};

struct Object_45c170 {
    char unknown_0[0x18];
    Holder_45c170* holder;             // +0x18
};

struct Game {
    char unknown_0[0x1434d];
    char field_1434d;                  // +0x1434d
};
#pragma pack(pop)

extern Game* g_game;

Entry_45c170* __stdcall FUN_004a0200(Entry_45c170* entries, char* name);
void __stdcall FUN_0049fa90(Object_45c170* obj);

static inline int SliderValue(Entry_45c170* e)
{
    if (e->steps <= 1)
        return 0;
    return (int)((float)e->pos / (e->steps - 1) * e->max);
}

// FUNCTION: 0x45c170
void __stdcall FUN_0045c170(Object_45c170* obj, int unused)
{
    Entry_45c170* e = FUN_004a0200(obj->holder->entries, "SCREEN");
    if (e != 0) {
        g_game->field_1434d = SliderValue(e) > 1 ? SliderValue(e) : 1;
        FUN_0049fa90(obj);
    }
}
