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

class Mission {
public:
    int LoadMissionByName(char* name);
    char* FUN_00435c30();
    bool FUN_00435c40();
    void RefreshMapList(int param_1);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_00444ea0 menu;                // +0x519
    char unknown_535[0x391e9 - 0x535];
    void* field_391e9;                 // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_00512990;

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
int __stdcall LoadMapList(char** out, int param_2, int param_3);
char* __stdcall Translate(char* text);
void __stdcall OpenMessageBox(Menu_00444ea0* menu, char* text, int width, int a, int b);
Layer_00444ea0* __stdcall LoadGuiLayer(Menu_00444ea0* menu, const char* name, int flags);
void __stdcall HandleMapSelectClick(void* gadget);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall SortFileList(char* items, int b, int c, int count);
void __stdcall FUN_004a32a0(Menu_00444ea0* menu, char* name, char* items, int count, int flag);
Entry_00444ea0* __stdcall FindGadgetChecked(Entry_00444ea0* entries, char* name);
void __stdcall UpdateMapSelection(void* menu, int unused);
char* __stdcall SkipTextLines(char* text, int line);
void __stdcall FUN_004a2e40(Menu_00444ea0* menu, char* name, int index);
void ShowSelectedMapInfo();
void __stdcall FUN_0049fb10(Menu_00444ea0* menu, int value);
void __stdcall RenderLayer(Menu_00444ea0* menu, int value);
void __stdcall FUN_004a0570(Menu_00444ea0* menu, char* name, int value);

// The call to Mission::RefreshMapList(0) is compiled without its
// argument push, although the callee ends in "ret 4" (see 0x435d30) and every
// other call site of it does push (0x430b98, 0x4446d7, 0x44a49e). That leaves
// the stack 4 bytes short, so this call is kept exactly as the original has
// it. It is never reached: the only caller (0x4488ea) calls this function
// precisely when Mission::FUN_00435c40() is false, and the test at the
// top of this function then returns early.

// FUNCTION: 0x444ea0
void OpenMultiMapSelector()
{
    DAT_00512990 = (char*)FUN_004d83b0("OLDMAPNAME", 0xc8);

    if (!((Mission*)g_game->field_391e9)->FUN_00435c40()) {
        OpenMessageBox(&g_game->menu,
                     Translate("There are no multiplayer maps to choose from"),
                     0x140, 1, 1);
        return;
    }

    strcpy(DAT_00512990,
           ((Mission*)g_game->field_391e9)->FUN_00435c30());
    ((Mission*)g_game->field_391e9)->RefreshMapList(0);

    int n = LoadMapList(0, 0, 0);
    if (n == 0) {
        OpenMessageBox(&g_game->menu,
                     Translate("There are no multiplayer maps to choose from"),
                     0x140, 1, 1);
        return;
    }

    Layer_00444ea0* layer = LoadGuiLayer(&g_game->menu, "SELMAP.GUI", 0x980);
    layer->handler = HandleMapSelectClick;
    Data_00444ea0* data = (Data_00444ea0*)FUN_004d83b0("SELECT MAP DATA", 0x20);
    layer->data = data;
    LoadPictureCached("DSELECTMAP2", 0, 0, 0);
    LoadMapList(&data->items, 0, 0);
    SortFileList(data->items, 0, 0, n);
    FUN_004a32a0(&g_game->menu, "MAPNAMES", data->items, n, 0);
    FindGadgetChecked(layer->entries, "MAPNAMES")->onSelect = UpdateMapSelection;

    for (int i = 0; i < n; i++) {
        if (strcmp(DAT_00512990, SkipTextLines(data->items, i)) == 0) {
            FUN_004a2e40(&g_game->menu, "MAPNAMES", i);
            break;
        }
    }

    Menu_00444ea0* menu = &g_game->menu;
    Entry_00444ea0* g = FindGadgetChecked(menu->holder->entries, "MAPNAMES");
    if (((Mission*)g_game->field_391e9)->LoadMissionByName(
            SkipTextLines(g->text, g->selected)) == 0) {
        FUN_004a0570(menu, "MAPPIC", 0);
    } else {
        FUN_004a0570(menu, "MAPPIC", 1);
        ShowSelectedMapInfo();
    }
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}
