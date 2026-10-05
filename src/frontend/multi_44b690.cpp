// Decompiled by mimo-v2.6-pro. Names are provisional.
// Click handler of the save game dialog (opened by FUN_00493060). On close
// (field +0x60 == -1) it clears the save game name lists and the dialog flag;
// CANCEL goes back to "Previous", DELETE removes the selected entry's path
// ("dir\name" through FUN_004bbc30, the CRT _rmdir) and rebuilds the "GAMES"
// list from the remaining names, stripping their extensions, GAMES/LOAD/
// GAMENAME write the name field to a .LST file, anything else resets the
// gadget.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_0044b690 {                // 0x15b bytes
    char unknown_0[0xb6];
    char text[0x15b - 0xb6];           // +0xb6
};

struct Gadget_0044b690 {
    char unknown_0[0xba];
    short selected;                    // +0xba
};

struct Layer_0044b690 {
    int unknown_0;
    Entry_0044b690* entries;           // +0x04
};

struct Menu_0044b690 {
    char unknown_0[0x18];
    Layer_0044b690* layer;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};

struct Game_0044b690 {
    char unknown_0[0x519];
    Menu_0044b690 menu;                // +0x519
    char unknown_57d[0x38a51 - 0x57d];
    unsigned short flag_38a51 : 1;     // +0x38a51, bit 0
    unsigned short bits_38a51 : 15;
    char unknown_38a53[0x38c6b - 0x38a53];
    char save_38c6b[0x100];            // +0x38c6b
};
#pragma pack(pop)

extern Game_0044b690* g_game;
extern char* DAT_005091c8;             // savegame directory
extern char DAT_005119b8[];
extern char* DAT_005129ac;
extern char* DAT_005129b0;

int __stdcall FUN_0049fd60(Menu_0044b690* menu, char* name);
void __stdcall FUN_0047f1a0(char* name, int flag);
Gadget_0044b690* __stdcall FUN_0049ff90(Entry_0044b690* entries, char* name);
int __stdcall FUN_0049fdf0(Entry_0044b690* entries, const char* name, int flag);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall FUN_004a0880(Menu_0044b690* menu, int index, char* text);
void __stdcall FUN_0049fa90(Menu_0044b690* menu);
void __stdcall FUN_004ab0a0(Menu_0044b690* menu);
void __stdcall FUN_004ab190(Menu_0044b690* menu, int flag);
void __stdcall FUN_004bbc30(char* path);
void* __stdcall FUN_0044b4e0(int* out);
void __stdcall FUN_004a32a0(Menu_0044b690* menu, char* name, char* text, int value, int flag);
void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_0044b230(char* filename);
void __cdecl FUN_004d85a0(char* p);

// FUNCTION: 0x44b690
void __stdcall FUN_0044b690(Menu_0044b690* menu)
{
    Entry_0044b690* entries = menu->layer->entries;
    if (menu->current == -1) {
        FUN_004ab190(menu, 1);
        if (DAT_005129ac)
            FUN_004d85a0(DAT_005129ac);
        if (DAT_005129b0)
            FUN_004d85a0(DAT_005129b0);
        DAT_005129ac = DAT_005129b0 = 0;
        g_game->flag_38a51 = 0;
        return;
    }
    if (FUN_0049fd60(menu, "CANCEL")) {
        FUN_0047f1a0("Previous", 0);
        return;
    }
    if (FUN_0049fd60(menu, "DELETE")) {
        FUN_0047f1a0("SMLBUTTON", 0);
        Gadget_0044b690* games = FUN_0049ff90(entries, "GAMES");
        char buf[0x100];
        sprintf(buf, "%s\\%s", DAT_005091c8,
                FUN_004b6af0(DAT_005129ac, games->selected));
        FUN_004bbc30(buf);
        int count;
        FUN_0044b4e0(&count);
        char* p = DAT_005129b0;
        for (int i = 0; i < count; i++) {
            strcpy(p, FUN_004b6af0(DAT_005129ac, i));
            p += strlen(FUN_004b6af0(DAT_005129ac, i));
            while (*p != '.')
                p--;
            *p++ = 0;
        }
        FUN_004a32a0(&g_game->menu, "GAMES", DAT_005129b0, count, 0);
        FUN_004ab0a0(menu);
        Menu_0044b690* menu2 = &g_game->menu;
        Entry_0044b690* gadgets = g_game->menu.layer->entries;
        Gadget_0044b690* games2 = FUN_0049ff90(gadgets, "GAMES");
        int index = FUN_0049fdf0(gadgets, "GAMENAME", 3);
        char* name;
        if (games2->selected > -1 &&
            (name = FUN_004b6af0(DAT_005129b0, games2->selected)) != 0 &&
            strlen(name) != 0)
            FUN_004a0880(menu2, index, name);
        else
            FUN_004a0880(menu2, index, DAT_005119b8);
        FUN_0049fa90(&g_game->menu);
        return;
    }
    if (FUN_0049fd60(menu, "GAMES") || FUN_0049fd60(menu, "LOAD") ||
        FUN_0049fd60(menu, "GAMENAME")) {
        FUN_0047f1a0("Options", 0);
        int idx = FUN_0049fdf0(entries, "GAMENAME", 3);
        char* name = entries[idx].text;
        if (strlen(name) != 0) {
            FUN_004290f0(g_game->save_38c6b, DAT_005091c8, name, "LST");
            FUN_0044b230(g_game->save_38c6b);
        }
    } else if (menu->current != -1) {
        FUN_004ab0a0(menu);
    }
}
