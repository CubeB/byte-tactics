// Decompiled by space-bunny-free. Names are provisional.
// A menu control handler: unless the local player is a connected human
// player (a non-empty player slot whose info has bit 6 of the byte at +0x9b
// set), it reads the "GAME" slider, clamps the slider value to at least 1 and
// pushes it into the game setting at g_game+0x38a4b, then applies the control.
// The clamped value comes from the inlined slider reader (FUN_0045ba20), which
// is expanded a second time for the "not clamped" branch, so it is written
// twice in the source too.

#pragma pack(push, 1)
struct PlayerInfo_45c070 {
    char unknown_0[0x9b];
    unsigned short unknown_9b_0 : 6;  // +0x9b, bits 0 to 5
    unsigned short flag_9b_6 : 1;     // +0x9b, bit 6 (mask 0x40)
    unsigned short unknown_9b_7 : 9;
};

struct Player_45c070 {
    int field_0;                      // +0x00
    char unknown_4[0x27 - 0x4];
    PlayerInfo_45c070* info;          // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Entry_45c070 {
    char unknown_0[0x136];
    short steps;                      // +0x136
    char unknown_138[0x13c - 0x138];
    int max;                          // +0x13c
    short pos;                        // +0x140
};

struct Game {
    char unknown_0[0x1b63];
    Player_45c070 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;        // +0x2a42
    char unknown_2a43[0x38a4b - 0x2a43];
    unsigned short field_38a4b;         // +0x38a4b
};
#pragma pack(pop)

struct Holder_45c070 {
    int unknown_0;
    Entry_45c070* entries;            // +0x4
};

struct Object_45c070 {
    char unknown_0[0x18];
    Holder_45c070* holder;            // +0x18
};

extern Game* g_game;

Entry_45c070* __stdcall FUN_004a0200(Entry_45c070* entries, char* name);
void __stdcall SetGameSpeed(unsigned int param1, int param2);
void __stdcall FUN_0049fa90(Object_45c070* obj);

static inline int SliderValue(Entry_45c070* e)
{
    if (e->steps <= 1)
        return 0;
    return (int)((float)e->pos / (e->steps - 1) * e->max);
}

// FUNCTION: 0x45c070
void __stdcall FUN_0045c070(Object_45c070* obj, int unused)
{
    Player_45c070* player = &g_game->players[g_game->localPlayer];
    if (player->field_0 == 0 || !player->info->flag_9b_6) {
        Entry_45c070* e = FUN_004a0200(obj->holder->entries, "GAME");
        if (e != 0) {
            int value = SliderValue(e);
            g_game->field_38a4b = (unsigned short)(value < 1 ? 1 : SliderValue(e));
            SetGameSpeed(g_game->field_38a4b, 1);
            FUN_0049fa90(obj);
        }
    }
}
