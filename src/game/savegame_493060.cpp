// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_00493060 {                // 0x15b bytes
    char unknown_0[0x1b];
    unsigned int flags;                // +0x1b
    char unknown_1f[0xce - 0x1f];
    void* handler;                     // +0xce
    char unknown_d2[0x15b - 0xd2];
};

struct Layer_00493060 {
    int unknown_0;
    Entry_00493060* entries;           // +0x04
    void (__stdcall* handler)(int, int); // +0x08
    void* data;                        // +0x0c
};

struct Menu_00493060 {
    char unknown_0[0x18];
    Layer_00493060* layer;             // +0x18
};

struct Game {
    char unknown_0[0x519];
    Menu_00493060 menu;                // +0x519
    char unknown_535[0x38a51 - 0x535];
    unsigned short flag_38a51 : 1;     // +0x38a51, bit 0
    unsigned short bits_38a51 : 15;
};
#pragma pack(pop)

extern Game* g_game;
extern char* DAT_005091c8;
extern char* DAT_0051f2e8;

Layer_00493060* __stdcall LoadGuiLayer(Menu_00493060* menu, const char* name, int flags);
void __stdcall SaveGameScreenHandler(int a, int b);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall FUN_004bcf00(char* path);
void __stdcall ListSavedGames(int* out);
void __stdcall FUN_004a0bf0(Menu_00493060* menu, char* name, char* text, int param_4);
void __stdcall FUN_004a0570(Menu_00493060* menu, char* name, int param_3);
Entry_00493060* __stdcall FindGadgetChecked(Entry_00493060* entries, char* name);
int __stdcall FindGadgetIndex(Entry_00493060* entries, char* name, int type);
void __stdcall FUN_00492de0(int a, int b);
char* FUN_00476830();
void ShowSavedGameInfo();
void __stdcall FUN_004a7190(Menu_00493060* menu, int index);
void __stdcall FUN_0049fb10(Menu_00493060* menu, int value);
void FUN_00428b60();
void __stdcall FUN_0049fa50(Menu_00493060* menu);
void __stdcall RenderLayer(Menu_00493060* menu, int value);

// FUNCTION: 0x493060
void ShowSaveGameScreen()
{
    int local;

    g_game->flag_38a51 |= 1;
    Layer_00493060* layer = LoadGuiLayer(&g_game->menu, "LOADGAME.GUI", 0x880);
    layer->handler = SaveGameScreenHandler;
    layer->data = g_game;
    FUN_004288d0("DSAVEGAME2", 0, 0, 0);
    FUN_004bcf00(DAT_005091c8);
    ListSavedGames(&local);
    FUN_004a0bf0(&g_game->menu, "TITLE", "Save Game", 0);
    if (local == 0) {
        FUN_004a0570(&g_game->menu, "DELETE", 0);
    }
    Entry_00493060* games = FindGadgetChecked(layer->entries, "GAMES");
    if (games != 0) {
        games->handler = FUN_00492de0;
    }
    int index = FindGadgetIndex(layer->entries, "GAMENAME", 3);
    layer->entries[index].flags |= 2;
    DAT_0051f2e8 = FUN_00476830();
    ShowSavedGameInfo();
    FUN_004a7190(&g_game->menu, index);
    FUN_0049fb10(&g_game->menu, 1);
    FUN_00428b60();
    FUN_004a0570(&g_game->menu, "LoadGame", 0);
    FUN_0049fa50(&g_game->menu);
    RenderLayer(&g_game->menu, 0x40);
}
