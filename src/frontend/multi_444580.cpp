// Decompiled by Claude Opus 5.5. Names are provisional.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_00444580 {                 // a gadget of the menu, 0x15b bytes
    short unknown_0;
    char name[0x13];                    // +0x02
    short field_15;                     // +0x15
    char unknown_17[0x19 - 0x17];
    short field_19;                     // +0x19
    char unknown_1b[0x1f - 0x1b];
    int field_1f;                       // +0x1f
    char unknown_23[0x29 - 0x23];
    char field_29;                      // +0x29
    char unknown_2a[0xb6 - 0x2a];
    short count;                        // +0xb6 (entry 0 only)
    char unknown_b8[0xcc - 0xb8];
    char field_cc[0x15b - 0xcc];        // +0xcc (entry 0 only)
};

struct Holder_00444580 {
    int unknown_0;
    Entry_00444580* entries;            // +0x4
};

struct Menu_00444580 {
    char unknown_0[0x18];
    Holder_00444580* holder;            // +0x18
};

struct Gadget_00444580 {
    int unknown_0;
    Entry_00444580* entries;            // +0x4
    void (__stdcall* handler)(void*);   // +0x8
    int field_c;                        // +0xc
};

struct Conn_00444580 {
    void* data;
    int size;
};

class Mission {
public:
    void RefreshMapList(int param_1);
};

struct Game_00444580 {
    char unknown_0[0x14];
    char field_14[0x4f9 - 0x14];        // +0x14
    int field_4f9;                      // +0x4f9
    char unknown_4fd[0x519 - 0x4fd];
    Menu_00444580 menu;                 // +0x519
    char unknown_535[0x5cb - 0x535];
    char field_5cb[0x2a4b - 0x5cb];     // +0x5cb
    void* descriptions;                 // +0x2a4b
    char unknown_2a4f[0x2a9f - 0x2a4f];
    void* guids;                        // +0x2a9f
    Conn_00444580* conns;               // +0x2aa3
    char unknown_2aa7[0x143a7 - 0x2aa7];
    unsigned char palette[0x391e9 - 0x143a7];   // +0x143a7
    Mission* field_391e9;               // +0x391e9
};
#pragma pack(pop)

struct LinkInfo {
    int id;             // -1: unused
    char name[32];
};

extern Game_00444580* g_game;
extern LinkInfo DAT_005127c8[];
extern char DAT_004fcfb8[];

void __stdcall CloseTopScreen(void* menu);
void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);
void __stdcall FUN_004ac7d0(void* menu, void* palette, void* param_3);
Gadget_00444580* __stdcall LoadGuiLayer(void* menu, char* name, int size);
void __stdcall FUN_004441a0(void* menu);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __stdcall HAPINET_uninitmultiplay(void* param_1);
void __stdcall HAPINET_getconnections(void* param_1, void* guids, void* conns, void* descriptions, void* param_5);
int __stdcall FindGadgetIndex(void* entries, const char* name, int flag);
unsigned int __stdcall OnlineGetLinkInfo(LinkInfo* links);
void __stdcall FUN_004a09c0(void* menu, int index, int param_3, int param_4);
void __stdcall FUN_004a32a0(void* menu, char* name, void* items, int count, int flag);
void FUN_00428b60();
void __stdcall FUN_0049fb10(void* menu, int value);
void __stdcall RenderLayer(void* menu, int value);

// Appends a copy of entry `from` under a new name. This is 0x4444d0, which the
// original defines between the two functions of this file and inlines here;
// this spelling (one local, `index`) also matches 0x4444d0 itself, and it is
// the one that gives this function's frame (two more locals add two slots).
// It is static here so that the object does not define 0x4444d0 a second time.
static int __stdcall FUN_004444d0(Entry_00444580* entries, int from, short y, int param_4, char* name)
{
    int index = ++entries[0].count;
    entries[index] = entries[from];
    FUN_004a09c0(&g_game->menu, index, param_4, 0);
    strcpy(entries[index].name, name);
    entries[index].field_15 = y;
    entries[index].field_29 = 1;
    entries[index].field_1f = 0;
    return index;
}

// Opens the SELPROV.GUI menu (choose a connection provider) and adds a button
// for each online service online.dll reports, copied from the SERVICEX one.
// FUNCTION: 0x444580
void FillProviderList()
{
    if (g_game->menu.holder != 0 && strcmp(g_game->menu.holder->entries->name, "SELPROV.GUI") == 0)
        CloseTopScreen(&g_game->menu);
    SetPaletteColors(g_game->palette, 0, 0x100);
    FUN_004ac7d0(&g_game->menu, g_game->palette, g_game->field_5cb);
    Gadget_00444580* menu = LoadGuiLayer(&g_game->menu, "SELPROV.GUI", 0x80);
    menu->handler = FUN_004441a0;
    menu->field_c = (int)g_game;
    LoadPictureCached("selconnect2", 1, 0, 0);
    g_game->descriptions = FUN_004d83b0("PROVIDER DESCRIPTIONS", 0x500);
    g_game->guids = FUN_004d83b0("PROVIDER GUIDS", 0xa0);
    g_game->conns = (Conn_00444580*)FUN_004d83b0("DPLAY CONNECTIONS", 0x50);
    if (g_game->conns != 0)
        memset(g_game->conns, 0, 0x50);
    g_game->field_391e9->RefreshMapList(1);
    HAPINET_uninitmultiplay(g_game->field_14);
    HAPINET_getconnections(g_game->field_14, g_game->guids, g_game->conns, g_game->descriptions, DAT_004fcfb8);
    Entry_00444580* entries = menu->entries;
    // The names tmpl, y, k and n, at function scope, set the stack slot order.
    int tmpl = FindGadgetIndex(entries, "SERVICEX", 1);
    int y;
    int k;
    // n = 0 is a dead store: makes n the first loop variable seen, fixing the reload order.
    int n = 0;
    int count = 0;
    try {
        count = OnlineGetLinkInfo(DAT_005127c8);
    } catch (...) {
    }
    if (count != 0 && tmpl != -1) {
        y = entries[tmpl].field_15;
        k = 0;
        for (n = 0; n < count; k++) {
            if (DAT_005127c8[k].id != -1) {
                n++;
                char name[0x1c];
                sprintf(name, "SERVICE%d", k);
                FUN_004444d0(entries, tmpl, y, (int)DAT_005127c8[k].name, name);
                y += entries[tmpl].field_19 + 1;
            }
        }
    }
    strcpy(entries->field_cc, "SELECT");
    FUN_004a32a0(&g_game->menu, "DPLAY", g_game->descriptions, g_game->field_4f9, 0);
    FUN_00428b60();
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
}
