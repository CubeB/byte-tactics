// Decompiled by mimo-v2.6-pro. Names are provisional.
// Opens the "SAVELIST.GUI" save dialog: copies each save name into the
// description buffer with its extension stripped, fills the "GAMES" list,
// wires up the list callbacks and shows the current game's name.
#include <string.h>

#pragma pack(push, 1)
struct Entry_0044b990 {                // 0x15b bytes
    char unknown_0[0x1b];
    unsigned int flags;                // +0x1b
    char unknown_1f[0xba - 0x1f];
    short selected;                    // +0xba
    char unknown_bc[0xce - 0xbc];
    void (__stdcall* handler)(int, int); // +0xce
    char unknown_d2[0x15b - 0xd2];
};
#pragma pack(pop)

struct Layer_0044b990 {
    int unknown_0;
    Entry_0044b990* entries;           // +0x04
    void (__stdcall* handler)(int, int); // +0x08
    void* data;                        // +0x0c
};

struct Menu_0044b990 {
    char unknown_0[0x18];
    Layer_0044b990* layer;             // +0x18
};

#pragma pack(push, 1)
struct Game_0044b990 {
    char unknown_0[0x519];
    Menu_0044b990 menu;                // +0x519
};
#pragma pack(pop)

extern Game_0044b990* g_game;
extern char* DAT_005091c8;             // savegame directory
extern char* DAT_005129ac;             // savegame names
extern char* DAT_005129b0;             // savegame descriptions
extern char DAT_005119b8[];

Layer_0044b990* __stdcall FUN_004aa8f0(Menu_0044b990* menu, const char* name, int flags);
void __stdcall FUN_0044b690(int a, int b);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall FUN_004bcf00(char* path);
void* __stdcall FUN_0044b4e0(int* out);
void __stdcall FUN_004a0bf0(Menu_0044b990* menu, char* name, char* text, int param_4);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall FUN_004a32a0(Menu_0044b990* menu, char* name, void* text, int value, int flag);
void __stdcall FUN_004a0570(Menu_0044b990* menu, char* name, int param_3);
Entry_0044b990* __stdcall FUN_0049ff90(Entry_0044b990* entries, char* name);
int __stdcall FUN_0049fdf0(Entry_0044b990* entries, const char* name, int flag);
void __stdcall FUN_0044b600(int unused1, int unused2);
void __stdcall FUN_004a0880(Menu_0044b990* menu, int index, char* text);
void __stdcall FUN_0049fa90(Menu_0044b990* menu);
void __stdcall FUN_004a7190(Menu_0044b990* menu, int index);
void __stdcall FUN_0049fb10(Menu_0044b990* menu, int value);
void FUN_00428b60();
void __stdcall FUN_0049fa50(Menu_0044b990* menu);
void __stdcall FUN_004a81e0(Menu_0044b990* menu, int value);

// FUNCTION: 0x44b990
void FUN_0044b990()
{
    int count;
    Layer_0044b990* layer = FUN_004aa8f0(&g_game->menu, "SAVELIST.GUI", 0x880);
    layer->handler = FUN_0044b690;
    layer->data = g_game;
    FUN_004288d0("DSaveList", 0, 0, 0);
    FUN_004bcf00(DAT_005091c8);
    FUN_0044b4e0(&count);
    FUN_004a0bf0(&g_game->menu, "TITLE", "Save Game", 0);
    char* ptr = DAT_005129b0;
    for (int i = 0; i < count; i++) {
        strcpy(ptr, FUN_004b6af0(DAT_005129ac, i));
        ptr += strlen(FUN_004b6af0(DAT_005129ac, i));
        while (*ptr != '.')
            ptr--;
        *ptr = 0;
        ptr++;
    }
    FUN_004a32a0(&g_game->menu, "GAMES", DAT_005129b0, count, 0);
    if (count == 0)
        FUN_004a0570(&g_game->menu, "DELETE", 0);
    Entry_0044b990* games = FUN_0049ff90(layer->entries, "GAMES");
    if (games != 0)
        games->handler = FUN_0044b600;
    int index = FUN_0049fdf0(layer->entries, "GAMENAME", 3);
    layer->entries[index].flags |= 2;

    Menu_0044b990* menu = &g_game->menu;
    Entry_0044b990* entries = g_game->menu.layer->entries;
    Entry_0044b990* games2 = FUN_0049ff90(entries, "GAMES");
    int index2 = FUN_0049fdf0(entries, "GAMENAME", 3);
    char* name;
    if (games2->selected > -1 && (name = FUN_004b6af0(DAT_005129b0, games2->selected)) != 0 && strlen(name) != 0)
        FUN_004a0880(menu, index2, name);
    else
        FUN_004a0880(menu, index2, DAT_005119b8);
    FUN_0049fa90(&g_game->menu);

    FUN_004a7190(&g_game->menu, index);
    FUN_0049fb10(&g_game->menu, 1);
    FUN_00428b60();
    FUN_004a0570(&g_game->menu, "LoadGame", 0);
    FUN_0049fa50(&g_game->menu);
    FUN_004a81e0(&g_game->menu, 0x40);
}
