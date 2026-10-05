// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)

struct Entry_004942e0 {                 // 0x15b bytes
    unsigned char type;                 // +0x00
    unsigned char group;                // +0x01
    char name[0x10];                    // +0x02
    char unknown_12[0x1b - 0x12];
    int attr;                           // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    union {
        short count;                    // +0xb6 (entry 0 only)
        void* callback;                 // +0xb6 (the HOTR entry)
    } u;
    void* image;                        // +0xba
    char unknown_be[0x15b - 0xbe];
};

struct Holder_004942e0 {
    char unknown_0[4];
    Entry_004942e0* entries;            // +0x4
    void* handler;                      // +0x8
    void* data;                         // +0xc
};

struct Menu_004942e0 {
    char unknown_0[0x18];
    Holder_004942e0* holder;            // +0x18
};

union Flags_004942e0 {
    unsigned short raw;
    struct {
        unsigned short low : 11;
        unsigned short b11 : 1;
        unsigned short high : 4;
    } bits;
};

struct Game {
    char unknown_0[0x519];
    Menu_004942e0 menu;                 // +0x519
    char unknown_535[0x581 - 0x535];
    int selected;                       // +0x581
    char unknown_585[0x2a43 - 0x585];
    unsigned char player;               // +0x2a43
    char unknown_2a44[0x2cba - 0x2a44];
    unsigned short field_2cba;          // +0x2cba
    char unknown_2cbc[0x14357 - 0x2cbc];
    char* units;                        // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    char* unitDefs;                     // +0x1439b
    char unknown_1439f[0x37ebe - 0x1439f];
    Flags_004942e0 flags;               // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall HandleUnitInfoDialogEvent(void* gadget);
void __stdcall FUN_00494290(void* gadget, void* entry);

Holder_004942e0* __stdcall LoadGuiLayer(Menu_004942e0* menu, const char* name, int flags);
Entry_004942e0* __stdcall FUN_004a0280(Entry_004942e0* entries, char* name);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall LoadPcx(char* path, int param);
char* __stdcall MakePropList(void* obj);
char* __stdcall Translate(char* text);
void __stdcall AddTextGadget(Holder_004942e0* obj, char* name, char* text, int x,
                            short y, int w, int flags);
void __cdecl FUN_004d85a0(void* data);
void __stdcall FUN_004a0bf0(Menu_004942e0* menu, char* name, char* text, int param);
void __stdcall FUN_0049fa90(Menu_004942e0* menu);
int __stdcall FUN_00465ac0(void* player, void* unit);
unsigned short __stdcall FindUnitTypeId(char* name);

// FUNCTION: 0x4942e0
void __stdcall OpenUnitInfoDialog(void)
{
    if (g_game->flags.bits.b11)
        return;

    int y;
    char* stats;
    char name[0x20];
    char buf[0x100];
    unsigned short type = 0;

    if (g_game->selected != -1) {
        strncpy(name, g_game->menu.holder->entries[g_game->selected].name, 0x10);
        name[0x10] = 0;
        type = FindUnitTypeId(name);
    } else {
        unsigned short t = g_game->field_2cba;
        if (t != 0) {
            char* unit = g_game->units + 0x118 * t;
            char* owner = (char*)g_game + 0x1b63 + 0x14b * g_game->player;
            if (FUN_00465ac0(owner, unit) == 0)
                type = 0;
            else
                type = *(unsigned short*)(unit + 0xa6);
        }
    }

    if (type == 0)
        return;

    Holder_004942e0* layer = LoadGuiLayer(&g_game->menu, "UNITINFOx.GUI", 0x1000);
    Entry_004942e0* entries = layer->entries;
    layer->handler = (void*)HandleUnitInfoDialogEvent;
    layer->data = g_game;
    Entry_004942e0* hotr = FUN_004a0280(entries, "HOTR");
    hotr->u.callback = (void*)FUN_00494290;
    char* def = g_game->unitDefs + 0x249 * (unsigned)type;
    BuildDataPath(buf, "unitpics", def + 0x20, "PCX");
    hotr->image = LoadPcx(buf, 0);
    stats = MakePropList(def);
    int n = entries->u.count;

    AddTextGadget(g_game->menu.holder, "TEXT", Translate("Cost"), 0x82, 0x20, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.holder, "TEXT", Translate("Energy"), 0x8c, 0x2f, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.holder, "TEXT", Translate("Metal"), 0x8c, 0x3e, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.holder, "TEXT", Translate("Build Time"), 0x8c, 0x4d, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.holder, "TEXT", Translate("Statistics"), 0x82, 0x5c, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.holder, "TEXT", Translate("Max Velocity"), 0x8c, 0x6b, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.holder, "TEXT", Translate("Acceleration"), 0x8c, 0x7a, -1, 2);
    n++;
    entries[n].attr = 0x411;
    AddTextGadget(g_game->menu.holder, "TEXT", Translate("Turn Rate"), 0x8c, 0x89, -1, 2);
    n++;
    entries[n].attr = 0x411;

    char* s = stats;
    if (*s != 0) {
        y = 0x20;
        do {
            AddTextGadget(g_game->menu.holder, "TEXT", s, 0xf0, y, -1, 2);
            n++;
            entries[n].attr = 0x411;
            s += strlen(s) + 1;
            y += 0xf;
        } while (*s != 0);
    }

    FUN_004d85a0(stats);
    FUN_004a0bf0(&g_game->menu, "NAME", def, 0x80);
    FUN_0049fa90(&g_game->menu);
}
