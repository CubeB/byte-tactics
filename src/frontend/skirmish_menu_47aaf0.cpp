// Decompiled by mimo-v2.6-pro. Names are provisional.
// Opens the skirmish map selector (SELMAP.GUI): counts the skirmish maps,
// fills the MAPNAMES list with them, selects the one the current player
// already has, then applies the selection the way the MAPNAMES callback does.

#include <string.h>

#pragma pack(push, 1)
struct Entry_0047aaf0 {                // GUI entry, 0x15b bytes
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
    char unknown_c6[0xce - 0xc6];
    void (__stdcall* onSelect)(void*, int);   // +0xce
};
#pragma pack(pop)

struct Holder_0047aaf0 {
    int unknown_0;
    Entry_0047aaf0* entries;           // +0x04
};

struct Menu_0047aaf0 {
    char unknown_0[0x18];
    Holder_0047aaf0* holder;           // +0x18
};

struct Data_0047aaf0 {                 // "SELECT MAP DATA", 0x20 bytes
    char unknown_0[0x14];
    char* items;                       // +0x14
};

struct Layer_0047aaf0 {
    int unknown_0;
    Entry_0047aaf0* entries;           // +0x04
    void (__stdcall* handler)(void*);  // +0x08
    Data_0047aaf0* data;               // +0x0c
};

struct Player_0047aaf0 {
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
    Menu_0047aaf0 menu;                // +0x519
    char unknown_535[0x29a0 - 0x535];
    Player_0047aaf0* player;           // +0x29a0
    char unknown_29a4[0x391e9 - 0x29a4];
    Class_00435a20* field_391e9;       // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_00434bf0(char** out, int param_2, int param_3);
char* __stdcall FUN_004c5740(char* text);
void __stdcall OpenMessageBox(Menu_0047aaf0* menu, char* text, int width, int a, int b);
Layer_0047aaf0* __stdcall LoadGuiLayer(Menu_0047aaf0* menu, const char* name, int flags);
void __stdcall FUN_0047a910(void* gadget);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall SortFileList(char* items, int b, int c, int count);
void __stdcall FUN_004a32a0(Menu_0047aaf0* menu, char* name, char* items, int count, int flag);
Entry_0047aaf0* __stdcall FindGadgetChecked(Entry_0047aaf0* entries, char* name);
void __stdcall FUN_0047aaa0(void* menu, int unused);
char* __stdcall FUN_004b6af0(char* text, int line);
void __stdcall FUN_004a2e40(Menu_0047aaf0* menu, char* name, int index);
void FUN_00444a20();
void __stdcall FUN_0049fb10(Menu_0047aaf0* menu, int value);
void __stdcall RenderLayer(Menu_0047aaf0* menu, int value);
void __stdcall FUN_00491c80(int value);

// FUNCTION: 0x47aaf0
void FUN_0047aaf0()
{
    int n = FUN_00434bf0(0, 0, 0);
    if (n == 0) {
        OpenMessageBox(&g_game->menu,
                     FUN_004c5740("There are no skirmish maps to choose from"),
                     0x140, 1, 1);
        return;
    }
    Layer_0047aaf0* layer = LoadGuiLayer(&g_game->menu, "SELMAP.GUI", 0x880);
    layer->handler = FUN_0047a910;
    Data_0047aaf0* data = (Data_0047aaf0*)FUN_004d83b0("SELECT MAP DATA", 0x20);
    layer->data = data;
    FUN_004288d0("DSELECTMAP2", 0, 0, 0);
    FUN_00434bf0(&data->items, 0, 0);
    SortFileList(data->items, 0, 0, n);
    FUN_004a32a0(&g_game->menu, "MAPNAMES", data->items, n, 0);
    FindGadgetChecked(layer->entries, "MAPNAMES")->onSelect = FUN_0047aaa0;

    for (int i = 0; i < n; i++) {
        if (strcmp(g_game->player->name, FUN_004b6af0(data->items, i)) == 0) {
            FUN_004a2e40(&g_game->menu, "MAPNAMES", i);
            break;
        }
    }

    Entry_0047aaf0* g = FindGadgetChecked(g_game->menu.holder->entries, "MAPNAMES");
    if (g_game->field_391e9->FUN_00435a20(FUN_004b6af0(g->text, g->selected)) != 0) {
        FUN_00444a20();
    }
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    FUN_00491c80(0x13);
}
