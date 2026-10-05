// Decompiled by Claude Opus 5.5. Names are provisional.
// Enables the end-of-mission buttons: all of them when the campaign goes on
// to another mission (the inlined FUN_0041f040), otherwise only MainMenu,
// whose entry field +0x15 is set to 0x1a0. Then focuses Missions or
// MainMenu and refreshes the menu.

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_00435980 {
public:
    int FUN_00435980(int index);
};

#pragma pack(push, 1)
struct Entry_0041f400 {                  // 0x15b bytes
    char unknown_0[0x15];
    short field_15;                      // +0x15
    char unknown_17[0x15b - 0x17];
};

struct Layer_0041f400 {
    int unknown_0;
    Entry_0041f400* entries;             // +0x04
};

struct Menu_0041f400 {
    char unknown_0[0x18];
    Layer_0041f400* layer;               // +0x18
};

struct Game {
    char unknown_0[0x519];
    Menu_0041f400 menu;                  // +0x519
    char unknown_535[0x391ab - 0x535];
    int mission;                         // +0x391ab
    int field_391af;                     // +0x391af
    char unknown_391b3[0x391e9 - 0x391b3];
    Class_00435100* campaign;            // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_004a0570(Menu_0041f400* menu, char* name, int value);
int __stdcall FUN_0049fdf0(Entry_0041f400* entries, char* name, int type);
void __stdcall FUN_004a76b0(Menu_0041f400* menu, char* name);
void __stdcall FUN_0049fad0(Menu_0041f400* menu);
void __stdcall FUN_0049fa90(Menu_0041f400* menu);

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

// FUNCTION: 0x41f400
void FUN_0041f400()
{
    char next = HasNextMission();
    if (next) {
        FUN_004a0570(&g_game->menu, "Start", 1);
        FUN_004a0570(&g_game->menu, "LoadGame", 1);
        FUN_004a0570(&g_game->menu, "SaveGame", 1);
        FUN_004a0570(&g_game->menu, "KNOB", 1);
        FUN_004a0570(&g_game->menu, "Missions", 1);
        FUN_004a0570(&g_game->menu, "Difficulty", 1);
        FUN_004a0570(&g_game->menu, "AdjustDiff", 1);
        FUN_004a0570(&g_game->menu, "MainMenu", 1);
        FUN_004a76b0(&g_game->menu, "Missions");
    } else {
        FUN_004a0570(&g_game->menu, "MainMenu", 1);
        Entry_0041f400* entries = g_game->menu.layer->entries;
        entries[FUN_0049fdf0(entries, "MainMenu", 1)].field_15 = 0x1a0;
        FUN_004a76b0(&g_game->menu, "MainMenu");
    }
    FUN_0049fad0(&g_game->menu);
    FUN_0049fa90(&g_game->menu);
}
