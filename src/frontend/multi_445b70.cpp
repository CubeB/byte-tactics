// Decompiled by Space Bunny Free. Names are provisional.
// GUI callback (see the entry a slider widget stores at +0x144): shows the
// unit limit of the player being watched, as text, and remembers it on the
// local player. FUN_00456850 picks the watched player, or 10 for "nobody",
// in which case the value comes from the widget's own slider instead.
#include <stdlib.h>

#pragma pack(push, 1)
struct PlayerData_445b70 {
    char unknown_0[0x97];
    unsigned char flags;               // +0x97
    char unknown_98[0xa5 - 0x98];
    unsigned short maxunits;           // +0xa5
};

struct Player_445b70 {
    char unknown_0[0x27];
    PlayerData_445b70* data;           // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x1b63];
    Player_445b70 players[10];          // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
};
#pragma pack(pop)

struct Table_445b70 {
    int unknown_0;
    void* entries;                     // +0x4
};

struct Gui_445b70 {
    char unknown_0[0x18];
    Table_445b70* table;               // +0x18
};

// A GUI layout entry, the kind "MAXUNITS" and "MAXUNITSTEXT" name.
struct Entry_445b70 {
    char unknown_0[0x15b];
};

extern Game* g_game;

void* __stdcall FUN_004a0200(void* entries, char* name);
void __stdcall FUN_004a0bf0(Gui_445b70* gui, char* name, char* data, int param_4);
unsigned char FUN_00456850();
int __stdcall FUN_0045ba20(Entry_445b70* entry);
void FUN_00450f90();

// FUNCTION: 0x445b70
void __stdcall FUN_00445b70(Gui_445b70* gui, int index)
{
    char text[0x14];
    int count;
    Entry_445b70* maxunits = (Entry_445b70*)FUN_004a0200(gui->table->entries, "MAXUNITS");
    if (maxunits != 0) {
        int player = FUN_00456850();
        if (player == g_game->localPlayer || player == 10) {
            count = FUN_0045ba20(maxunits) + 0x14;
        } else {
            count = g_game->players[player].data->maxunits;
        }
        _itoa(count, text, 10);
        FUN_004a0bf0(gui, "MAXUNITSTEXT", text, 0);
        g_game->players[g_game->localPlayer].data->maxunits = count;
        PlayerData_445b70* data = g_game->players[g_game->localPlayer].data;
        unsigned char f = data->flags;
        data->maxunits = count;
        if (f & 1) {
            FUN_00450f90();
        }
    }
}
