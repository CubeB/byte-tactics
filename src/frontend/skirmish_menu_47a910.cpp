// Decompiled by space-bunny-free. Names are provisional.
// Menu gadget callback. With no gadget selected it frees the MAPPIC bitmap and
// the map list object; for MAPNAMES or LOAD it clicks, copies the picked line
// out of the MAPNAMES list into the current player and refreshes "MapName".
// PREVMENU just clicks, anything else clears the selection.

#include <string.h>

#pragma pack(push, 1)
struct Entry_0047a910 {                // GUI entry
    char unknown_0[2];
    char name[0x10];                   // +0x02
    char unknown_12[0xba - 0x12];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
    char unknown_c6[0x15b - 0xc6];
};
#pragma pack(pop)

struct List_0047a910 {
    char unknown_0[0x14];
    void* field_14;                    // +0x14
};

struct Holder_0047a910 {
    int unknown_0;
    Entry_0047a910* entries;           // +0x04
    char unknown_8[0xc - 0x8];
    List_0047a910* list;               // +0x0c
};

struct Menu_0047a910 {
    char unknown_0[0x18];
    Holder_0047a910* holder;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};

struct Player_0047a910 {
    char unknown_0[0x11c];
    char name[0x100];                  // +0x11c
};

class Class_00435a20 {
public:
    int FUN_00435a20(char* name);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_0047a910 sub;                 // +0x519
    char unknown_57d[0x29a0 - 0x57d];
    Player_0047a910* player;           // +0x29a0
    char unknown_29a4[0x391e9 - 0x29a4];
    Class_00435a20* field_391e9;       // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

void __cdecl FUN_004d85a0(void* ptr);
int __stdcall FUN_0047f1a0(char* name, int param_2);
int __stdcall IsCurrentGadgetNamed(Menu_0047a910* menu, char* name);
Entry_0047a910* __stdcall FindGadgetChecked(Entry_0047a910* entries, char* name);
Entry_0047a910* __stdcall FUN_004a0280(Entry_0047a910* entries, char* name);
void __stdcall FUN_004a0e00(Menu_0047a910* menu, char* name, char* value);
void __stdcall FUN_004ab0a0(Menu_0047a910* menu);
char* __stdcall FUN_004b6af0(char* text, int line);

// FUNCTION: 0x47a910
void __stdcall FUN_0047a910(Menu_0047a910* menu)
{
    char buffer[0x100];
    Holder_0047a910* holder = menu->holder;
    List_0047a910* list = holder->list;
    Entry_0047a910* entries = holder->entries;

    if (menu->current == -1) {
        Entry_0047a910* pic = FUN_004a0280(g_game->sub.holder->entries, "MAPPIC");
        if (pic->text != 0) {
            FUN_004d85a0(pic->text);
            pic->text = 0;
        }
        FUN_004d85a0(list->field_14);
        FUN_004d85a0(list);
        return;
    }
    if (!IsCurrentGadgetNamed(menu, "MAPNAMES") && !IsCurrentGadgetNamed(menu, "LOAD")) {
        if (IsCurrentGadgetNamed(menu, "PREVMENU")) {
            FUN_0047f1a0("Previous", 0);
        } else {
            FUN_004ab0a0(menu);
        }
        return;
    }
    if (IsCurrentGadgetNamed(menu, "LOAD")) {
        FUN_0047f1a0("SmallButton", 0);
    }
    Entry_0047a910* g = FindGadgetChecked(entries, "MAPNAMES");
    strncpy(g_game->player->name, FUN_004b6af0(g->text, g->selected), 0x100);
    g_game->field_391e9->FUN_00435a20(g_game->player->name);
    strncpy(buffer, g_game->player->name, 0x100);
    FUN_004a0e00(menu, "MapName", buffer);
}
