// Decompiled by Claude Opus 5.5. Names are provisional.
// Opens the end-of-mission screen (ENDMSN.GUI) with FUN_0041ec50 as its
// handler. When the campaign goes on to another mission (the inlined
// FUN_0041f040) it plays "outcome1", makes Start the default button and
// fills the Missions list; otherwise it plays "outcome0" and focuses
// MainMenu. Then it draws the outcome image and shows the menu.
//
// The function is __stdcall although it takes no arguments (plain ret): as
// __cdecl, MSVC 5 hoists the load of `entries` above the "KNOB" push. The
// include must come first: after the declarations, the mission index and
// the (field_391af != 0) term of the last FUN_004a2e40 call are added the
// other way round.
#include <string.h>

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00435980 {
public:
    int FUN_00435980(int index);
};

class Class_00435c00 {
public:
    void FUN_00435c00(int param_1);
};

class Class_00435760 {
public:
    int FUN_00435760(int* param_1);
};

#pragma pack(push, 1)
struct Entry_0041f0a0 {                  // 0x15b bytes
    char unknown_0[0x19];
    short field_19;                      // +0x19
    char unknown_1b[0x136 - 0x1b];
    short field_136;                     // +0x136
    char unknown_138[0x142 - 0x138];
    short field_142;                     // +0x142
    char unknown_144[0x15b - 0x144];
};

struct Data_0041f0a0 {
    char unknown_0[0x14];
    int items;                           // +0x14
    char unknown_18[0x20 - 0x18];
};

struct Layer_0041f0a0 {
    int unknown_0;
    char* entries;                       // +0x04
    void (__stdcall* handler)(void*);    // +0x08
    Data_0041f0a0* data;                 // +0x0c
    char unknown_10[0x24 - 0x10];
    void* surface;                       // +0x24
};

struct Menu_0041f0a0 {
    char unknown_0[0x18];
    Layer_0041f0a0* layer;               // +0x18
};

struct Owner_0041f0a0 {
    char unknown_0[0x9b];
    unsigned char flags;                 // +0x9b
};

struct Player_0041f0a0 {                 // 0x14b bytes
    int active;                          // +0x00
    char unknown_4[0x27 - 0x4];
    Owner_0041f0a0* owner;               // +0x27
    char unknown_2b[0x14b - 0x2b];
};

struct Game {
    char unknown_0[0x519];
    Menu_0041f0a0 menu;                  // +0x519
    char unknown_535[0x1b63 - 0x535];
    Player_0041f0a0 players[10];         // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;           // +0x2a42
    char unknown_2a43[0x2bee - 0x2a43];
    unsigned short bits0 : 4;            // +0x2bee
    unsigned short flag4 : 1;            // +0x2bee, bit 4
    unsigned short bits5 : 11;
    char unknown_2bf0[0x14813 - 0x2bf0];
    unsigned short* image_14813;         // +0x14813
    unsigned short* image_14817;         // +0x14817
    char unknown_1481b[0x37e1b - 0x1481b];
    int surface;                         // +0x37e1b
    int width;                           // +0x37e1f
    char unknown_37e23[0x391ab - 0x37e23];
    int mission;                         // +0x391ab
    int field_391af;                     // +0x391af
    char unknown_391b3[0x391cf - 0x391b3];
    char missionFlags[0x19];             // +0x391cf
    char unknown_391e8[0x391e9 - 0x391e8];
    Class_00435100* campaign;            // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

void FUN_004257a0();
void __stdcall FillSurface(int param_1, int param_2);
void FlipScreen();
Layer_0041f0a0* __stdcall LoadGuiLayer(Menu_0041f0a0* menu, const char* name, int flags);
void __stdcall FUN_0041ec50(void* gadget);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void __stdcall SelectGadgetByName(Menu_0041f0a0* menu, const char* name);
int __stdcall FUN_0041eaa0(int names, char* flags, int count);
void __stdcall FUN_004a32a0(Menu_0041f0a0* menu, char* name, int items, int count, int flag);
Entry_0041f0a0* __stdcall FUN_004a0200(char* entries, char* name);
void __stdcall FUN_004a2e40(Menu_0041f0a0* menu, char* name, int index);
void FUN_00477410();
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);
void __stdcall DrawFrame(void* param_1, int param_2, int x, int y);
void __stdcall FUN_004a0bf0(Menu_0041f0a0* menu, char* name, char* text, int param_4);
void __stdcall FUN_0049fb10(Menu_0041f0a0* menu, int value);
void __stdcall RenderLayer(Menu_0041f0a0* menu, int value);
void __stdcall FUN_00491c80(int param_1);


// Inlined copy of FUN_0041f040.
static inline int HasNextMission()
{
    if (g_game->campaign->FUN_00435100() == 1 &&
        ((g_game->field_391af == 0 &&
          ((Class_00435980*)g_game->campaign)->FUN_00435980(g_game->mission + 1) == 0) ||
         ((Class_00435980*)g_game->campaign)->FUN_00435980(g_game->mission + 1) != 0)) {
        return 1;
    }
    return 0;
}

// FUNCTION: 0x41f0a0
void __stdcall FUN_0041f0a0()
{
    FUN_004257a0();
    FillSurface(g_game->surface, 0);
    FlipScreen();
    Layer_0041f0a0* layer = LoadGuiLayer(&g_game->menu, "ENDMSN.GUI", 0x80);
    layer->handler = FUN_0041ec50;
    Data_0041f0a0* data = (Data_0041f0a0*)FUN_004d83b0("EndMsnGUI", 0x20);
    data->items = 0;
    layer->data = data;
    char* entries = layer->entries;
    char next = HasNextMission();
    if (next) {
        ((Class_00435c00*)g_game->campaign)->FUN_00435c00(g_game->mission);
        FUN_004288d0("outcome1", 1, 1, 0);
        strcpy(layer->entries + 0xcc, "Start");
    } else {
        FUN_004288d0("outcome0", 1, 1, 0);
        SelectGadgetByName(&g_game->menu, "MainMenu");
    }
    next = HasNextMission();
    if (next) {
        int count = ((Class_00435760*)g_game->campaign)->FUN_00435760(&data->items);
        data->items = FUN_0041eaa0(data->items, g_game->missionFlags, count);
        // Suspected original bug: this finds the first 'U' mission flag but
        // the index is never used (perhaps a lost "select the first
        // unplayed mission" step).
        int i;
        for (i = 0; i < 0x19; i++) {
            if (g_game->missionFlags[i] == 'U')
                break;
        }
        FUN_004a32a0(&g_game->menu, "Missions", data->items, count, 0);
        Entry_0041f0a0* knob = FUN_004a0200(entries, "KNOB");
        knob->field_136 = knob->field_19 - knob->field_142 - 3;
        FUN_004a2e40(&g_game->menu, "Missions", g_game->mission);
        FUN_004a2e40(&g_game->menu, "Missions", g_game->mission + (g_game->field_391af != 0));
        FUN_00477410();
    }
    Player_0041f0a0* player = &g_game->players[g_game->localPlayer];
    int x = g_game->width / 2;
    if (g_game->field_391af != 0 && (player->active == 0 || !(player->owner->flags & 0x40))) {
        DrawFrame(layer->surface, GetGafFrame(g_game->image_14813, 0), x, 0x1c);
    } else {
        DrawFrame(layer->surface, GetGafFrame(g_game->image_14817, 0), x, 0x1c);
    }
    if (g_game->flag4)
        FUN_004a0bf0(&g_game->menu, "MainMenu", "OK", 0);
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    FUN_00491c80(0x13);
}
