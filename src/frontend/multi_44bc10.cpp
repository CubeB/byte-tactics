// Decompiled by mimo-v2.6-pro. Names are provisional.
// Opens the saved-list dialog (LOADLIST.GUI): packs the extension-stripped
// list names into the DAT_005129b0 buffer, fills the "GAMES" gadget with them,
// and copies the selected name into the "GAMENAME" gadget.
#include <string.h>

#pragma pack(push, 1)
struct Inner_0044bc10 {
    int unknown_0;                     // +0x0
    void* gadgets;                     // +0x4
};

struct Menu_0044bc10 {
    char unknown_0[0x18];
    Inner_0044bc10* inner;             // +0x18
};

struct Entry_0044bc10 {
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0xce - 0xbc];
    void* field_ce;                    // +0xce
};

struct Gadget_0044bc10 {
    char unknown_0[4];
    void* gadgets;                     // +0x4
    void (__stdcall* handler)(Menu_0044bc10*); // +0x8
    void* context;                     // +0xc
};

struct Game {
    char unknown_0[0x519];
    Menu_0044bc10 menu;                // +0x519
    char unknown_535[0x38a51 - 0x535];
    unsigned short flag_38a51 : 1;     // +0x38a51, bit 0
    unsigned short bits_38a51 : 15;
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];
extern char* DAT_005129ac;
extern char* DAT_005129b0;

Gadget_0044bc10* __stdcall LoadGuiLayer(Menu_0044bc10* menu, const char* name, int flags);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void* __stdcall ListSaveGameFiles(int* out);
void __stdcall CloseTopScreen(Menu_0044bc10* menu);
char* __stdcall Translate(const char* text);
int __stdcall OpenMessageBox(Menu_0044bc10* menu, const char* text, int a, int b, int c);
char* __stdcall SkipTextLines(char* text, int n);
void __stdcall FUN_004a32a0(Menu_0044bc10* menu, char* name, void* text, int count, int flag);
void __stdcall FUN_004a0570(Menu_0044bc10* menu, const char* name, int value);
Entry_0044bc10* __stdcall FindGadgetChecked(void* gadgets, char* name);
int __stdcall FindGadgetIndex(void* gadgets, const char* name, int flag);
void __stdcall SetGadgetText(Menu_0044bc10* menu, int index, char* text);
void __stdcall FUN_0049fa90(Menu_0044bc10* menu);
void __stdcall FUN_0049fb10(Menu_0044bc10* menu, int value);
void FUN_00428b60();
void __stdcall FUN_0049fa50(Menu_0044bc10* menu);
void __stdcall RenderLayer(Menu_0044bc10* menu, int value);
void __stdcall HandleLoadListClick(Menu_0044bc10* menu);
void __stdcall FUN_0044b600(int unused1, int unused2);

// FUNCTION: 0x44bc10
void OpenLoadListDialog()
{
    Gadget_0044bc10* gadget = LoadGuiLayer(&g_game->menu, "LOADLIST.GUI", 0x981);
    gadget->handler = HandleLoadListClick;
    gadget->context = g_game;
    LoadPictureCached("DLoadList", 0, 0, 0);
    int count;
    if (ListSaveGameFiles(&count) == 0) {
        CloseTopScreen(&g_game->menu);
        OpenMessageBox(&g_game->menu,
                     Translate("There are no saved lists to choose from"),
                     0x140, 1, 1);
        return;
    }
    char* p = DAT_005129b0;
    for (int i = 0; i < count; i++) {
        strcpy(p, SkipTextLines(DAT_005129ac, i));
        p += strlen(SkipTextLines(DAT_005129ac, i));
        char c = *p;
        while (c != '.') {
            c = *--p;
        }
        *p = 0;
        p++;
    }
    FUN_004a32a0(&g_game->menu, "GAMES", DAT_005129b0, count, 0);
    FUN_004a0570(&g_game->menu, "DELETE", 0);
    FUN_004a0570(&g_game->menu, "GAMENAME", 0);
    Entry_0044bc10* entry = FindGadgetChecked(gadget->gadgets, "GAMES");
    if (entry != 0) {
        entry->field_ce = (void*)FUN_0044b600;
    }
    Menu_0044bc10* menu = &g_game->menu;
    void* gadgets = g_game->menu.inner->gadgets;
    Entry_0044bc10* games = FindGadgetChecked(gadgets, "GAMES");
    int index = FindGadgetIndex(gadgets, "GAMENAME", 3);
    char* name;
    if (games->selected > -1 && (name = SkipTextLines(DAT_005129b0, games->selected)) != 0 && strlen(name) != 0)
        SetGadgetText(menu, index, name);
    else
        SetGadgetText(menu, index, DAT_005119b8);
    FUN_0049fa90(&g_game->menu);
    FUN_0049fb10(&g_game->menu, 1);
    FUN_00428b60();
    FUN_004a0570(&g_game->menu, "SaveGame", 0);
    FUN_0049fa50(&g_game->menu);
    RenderLayer(&g_game->menu, 0x40);
    g_game->flag_38a51 |= 1;
}
