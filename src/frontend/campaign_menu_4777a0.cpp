// Decompiled by mimo-v2.6-pro. Names are provisional.
// Sets up the single player menu: opens "SINGLE.GUI" with HandleSingleMenuClick as its
// handler and g_game as its data, paints the "singlebg" background, marks which
// of the two side objects shows which side from the flag at +0x37ef2, installs
// ToggleAnyMission on the layer at +0x531 and pushes the menu.
#include <string.h>

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x95];
    unsigned char side;                // +0x95
};

struct PlayerEntry_004777a0 {
    Unit* unit;                        // +0x00
    char unknown_4[0x14b - 4];
};

struct Layer_004777a0 {
    char unknown_0[8];
    void (__stdcall* handler)(void*);  // +0x8
    void* field_c;                     // +0xc
    char unknown_10[0x3b - 0x10];
    void (__stdcall* field_3b)(void*); // +0x3b
};

struct Menu_004777a0 {
    char unknown_0[0x18];
    Layer_004777a0* layer;             // +0x18 (g_game + 0x531)
};

struct Game {
    char unknown_0[0x519];
    Menu_004777a0 menu;                // +0x519
    char unknown_535[0x1b8a - 0x535];
    PlayerEntry_004777a0 players[10];  // +0x1b8a
    // MSVC 5 gives players[10] a total size 8 bytes larger than 10 * 0x14b,
    // so the pad below starts at 0x2878 and every later offset is right.
    char unknown_2878[0x2a42 - 0x2878];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37ef2 - 0x2a43];
    int flag_37ef2;                    // +0x37ef2
    char unknown_37ef6[0x38d7f - 0x37ef6];
    unsigned short flags_38d7f;        // +0x38d7f
};
#pragma pack(pop)

extern Game* g_game;

Layer_004777a0* __stdcall LoadGuiLayer(Menu_004777a0* menu, const char* name, int flags);
void __stdcall HandleSingleMenuClick(void* layer);
void BlankScreen();
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
void __stdcall SetMissionType(int owner);
void __stdcall FUN_004a0570(Menu_004777a0* menu, const char* name, int value);
void __stdcall ToggleAnyMission(void* gadget);
int FUN_0049f580();
void __stdcall FUN_004a1530(Menu_004777a0* menu, const char* name, char value);
void __stdcall FUN_0049fb10(Menu_004777a0* menu, int value);
void __stdcall FUN_00491c80(int value);
void __stdcall RenderLayer(Menu_004777a0* menu, int value);

// FUNCTION: 0x4777a0
void OpenSingleMenu()
{
    Layer_004777a0* gui = LoadGuiLayer(&g_game->menu, "SINGLE.GUI", 0);
    gui->handler = HandleSingleMenuClick;
    gui->field_c = g_game;
    BlankScreen();
    LoadPictureCached("singlebg", 0, 0, 0);
    switch (g_game->flag_37ef2) {
    case 0:
        g_game->players[g_game->localPlayer].unit->side = 0;
        g_game->players[g_game->localPlayer + 1].unit->side = 1;
        break;
    case 1:
        g_game->players[g_game->localPlayer].unit->side = 1;
        g_game->players[g_game->localPlayer + 1].unit->side = 0;
        break;
    }
    SetMissionType(1);
    if (g_game->flags_38d7f & 1) {
        FUN_004a0570(&g_game->menu, "AnyMsn", 1);
        // Original oddity, kept: the test guards an |= of the same bit.
        g_game->flags_38d7f |= 1;
    }
    g_game->menu.layer->field_3b = ToggleAnyMission;
    if (FUN_0049f580() && _strcmpi((char*)FUN_0049f580(), "spanish") == 0) {
        FUN_004a1530(&g_game->menu, "Skirmish", 0x73);
    }
    FUN_0049fb10(&g_game->menu, 1);
    FUN_00491c80(0x13);
    RenderLayer(&g_game->menu, 0x40);
}
