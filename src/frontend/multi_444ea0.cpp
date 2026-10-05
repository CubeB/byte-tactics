// Decompiled by Space Bunny Free. Names are provisional.
// Opens the multiplayer map selector (SELMAP.GUI): saves the map the local
// player was last on in a global buffer, counts the multiplayer maps, fills
// the MAPNAMES list with them, selects the saved one, then applies the
// selection the way the MAPNAMES callback does.

#include <string.h>

#pragma pack(push, 1)
struct Entry_00444ea0 {                // GUI entry, 0xd2 bytes
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
    char unknown_c6[0xce - 0xc6];
    void (__stdcall* onSelect)(void*, int);   // +0xce
};
#pragma pack(pop)

struct Holder_00444ea0 {
    int unknown_0;
    Entry_00444ea0* entries;           // +0x04
    char unknown_8[4];
    void* layout;                      // +0x0c
};

struct Menu_00444ea0 {
    char unknown_0[0x18];
    Holder_00444ea0* holder;           // +0x18
};

struct Data_00444ea0 {                 // "SELECT MAP DATA", 0x20 bytes
    char unknown_0[0x14];
    char* items;                       // +0x14
};

struct Layer_00444ea0 {
    int unknown_0;
    Entry_00444ea0* entries;           // +0x04
    void (__stdcall* handler)(void*);  // +0x08
    Data_00444ea0* data;               // +0x0c
};

class Class_00435a20 {
public:
    int FUN_00435a20(char* name);
};

class Class_00435c30 {
public:
    char* FUN_00435c30();
};

class Class_00435c40 {
public:
    bool FUN_00435c40();
};

class Class_00435d30 {
public:
    void FUN_00435d30(int param_1);
};

#pragma pack(push, 1)
struct Game_00444ea0 {
    char unknown_0[0x519];
    Menu_00444ea0 menu;                // +0x519
    char unknown_535[0x391e9 - 0x535];
    void* field_391e9;                 // +0x391e9
};
#pragma pack(pop)

extern Game_00444ea0* g_game;
extern char* DAT_00512990;

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
int __stdcall FUN_00434bf0(char** out, int param_2, int param_3);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004abd90(Menu_00444ea0* menu, char* text, int width, int a, int b);
Layer_00444ea0* __stdcall FUN_004aa8f0(Menu_00444ea0* menu, const char* name, int flags);
void __stdcall FUN_00444cb0(void* gadget);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall FUN_004aefa0(char* items, int b, int c, int count);
void __stdcall FUN_004a32a0(Menu_00444ea0* menu, char* name, char* items, int count, int flag);
Entry_00444ea0* __stdcall FUN_0049ff90(Entry_00444ea0* entries, char* name);
void __stdcall FUN_00444c40(void* menu, int unused);
char* __stdcall FUN_004b6af0(char* text, int line);
void __stdcall FUN_004a2e40(Menu_00444ea0* menu, char* name, int index);
void FUN_00444a20();
void __stdcall FUN_0049fb10(Menu_00444ea0* menu, int value);
void __stdcall FUN_004a81e0(Menu_00444ea0* menu, int value);
void __stdcall FUN_004a0570(Menu_00444ea0* menu, char* name, int value);

// The call to Class_00435d30::FUN_00435d30(0) is compiled without its
// argument push, although the callee ends in "ret 4" (see 0x435d30) and every
// other call site of it does push (0x430b98, 0x4446d7, 0x44a49e). That leaves
// the stack 4 bytes short, so this call is kept exactly as the original has
// it. It is never reached: the only caller (0x4488ea) calls this function
// precisely when Class_00435c40::FUN_00435c40() is false, and the test at the
// top of this function then returns early.

// FUNCTION: 0x444ea0
void FUN_00444ea0()
{
    DAT_00512990 = (char*)FUN_004d83b0("OLDMAPNAME", 0xc8);

    if (!((Class_00435c40*)g_game->field_391e9)->FUN_00435c40()) {
        FUN_004abd90(&g_game->menu,
                     FUN_004c5740("There are no multiplayer maps to choose from"),
                     0x140, 1, 1);
        return;
    }

    strcpy(DAT_00512990,
           ((Class_00435c30*)g_game->field_391e9)->FUN_00435c30());
    ((Class_00435d30*)g_game->field_391e9)->FUN_00435d30(0);

    int n = FUN_00434bf0(0, 0, 0);
    if (n == 0) {
        FUN_004abd90(&g_game->menu,
                     FUN_004c5740("There are no multiplayer maps to choose from"),
                     0x140, 1, 1);
        return;
    }

    Layer_00444ea0* layer = FUN_004aa8f0(&g_game->menu, "SELMAP.GUI", 0x980);
    layer->handler = FUN_00444cb0;
    Data_00444ea0* data = (Data_00444ea0*)FUN_004d83b0("SELECT MAP DATA", 0x20);
    layer->data = data;
    FUN_004288d0("DSELECTMAP2", 0, 0, 0);
    FUN_00434bf0(&data->items, 0, 0);
    FUN_004aefa0(data->items, 0, 0, n);
    FUN_004a32a0(&g_game->menu, "MAPNAMES", data->items, n, 0);
    FUN_0049ff90(layer->entries, "MAPNAMES")->onSelect = FUN_00444c40;

    for (int i = 0; i < n; i++) {
        if (strcmp(DAT_00512990, FUN_004b6af0(data->items, i)) == 0) {
            FUN_004a2e40(&g_game->menu, "MAPNAMES", i);
            break;
        }
    }

    Menu_00444ea0* menu = &g_game->menu;
    Entry_00444ea0* g = FUN_0049ff90(menu->holder->entries, "MAPNAMES");
    if (((Class_00435a20*)g_game->field_391e9)->FUN_00435a20(
            FUN_004b6af0(g->text, g->selected)) == 0) {
        FUN_004a0570(menu, "MAPPIC", 0);
    } else {
        FUN_004a0570(menu, "MAPPIC", 1);
        FUN_00444a20();
    }
    FUN_0049fb10(&g_game->menu, 1);
    FUN_004a81e0(&g_game->menu, 0x40);
}
