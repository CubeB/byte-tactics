// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Opens the options menu (ARMOPT.GUI): wires FUN_004609b0 as its handler,
// enables the SAVEGAME/LOADGAME gadgets unless the game is playing a
// campaign mission, fills in the mission settings gadget, and marks the
// options as changed.

class Class_00435100 {
public:
    int FUN_00435100();
};

class Class_004ce910 {
public:
    void FUN_004ce910(int value);
};

struct Entry_00460cc0 {
    char unknown_0[2];
    char name[0x10];                   // +0x2
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6
    char unknown_b8[0x15b - 0xb8];
};

#pragma pack(push, 1)
struct Gui_00460cc0 {
    char unknown_0[0x18];
    void* holder;                      // +0x18
    char unknown_1c[0xa2 - 0x1c];
    int field_a2;                      // +0xa2
    char unknown_a6[0xcca - 0xa6];
    int field_cca;                     // +0xcca
};

struct Game {
    char unknown_0[0x10];
    Class_004ce910* field_10;          // +0x10
    char unknown_14[0x519 - 0x14];
    Gui_00460cc0 sub;                  // +0x519
    char unknown_11e7[0x38a51 - 0x11e7];
    unsigned char flags_38a51;         // +0x38a51
    char unknown_38a52[0x391e9 - 0x38a52];
    Class_00435100* mode;              // +0x391e9
};
#pragma pack(pop)

struct Gadget_00460cc0 {
    char unknown_0[4];
    Entry_00460cc0* info;              // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

extern Game* g_game;

Gadget_00460cc0* __stdcall LoadGuiLayer(Gui_00460cc0* sub, const char* name, int flags);
int __stdcall FindGadgetIndex(Entry_00460cc0* entries, const char* name, int type);
void __stdcall FUN_004a1200(Gui_00460cc0* sub, int index, int value);
void __stdcall FUN_004a0bf0(Gui_00460cc0* sub, const char* name, const char* value, int flags);
void __stdcall FUN_0049fa50(Gui_00460cc0* sub);
void __stdcall FUN_0049fb10(Gui_00460cc0* sub, int value);
void __stdcall RenderLayer(Gui_00460cc0* sub, int value);
const char* __stdcall FUN_004c5740(const char* text);
void __stdcall FUN_004609b0(void* gadget);

// FUNCTION: 0x460cc0
void FUN_00460cc0()
{
    Gadget_00460cc0* gadget = LoadGuiLayer(&g_game->sub, "ARMOPT.GUI", 0x800);
    gadget->handler = FUN_004609b0;
    FUN_004a1200(&g_game->sub, FindGadgetIndex(gadget->info, "SAVEGAME", 1),
                 g_game->mode->FUN_00435100() == 3);
    FUN_004a1200(&g_game->sub, FindGadgetIndex(gadget->info, "LOADGAME", 1),
                 g_game->mode->FUN_00435100() == 3);
    if (g_game->mode->FUN_00435100() == 3 || g_game->mode->FUN_00435100() == 2) {
        FUN_004a0bf0(&g_game->sub, "MISSION", FUN_004c5740("Settings"), 0x80);
    }
    FUN_0049fa50(&g_game->sub);
    FUN_0049fb10(&g_game->sub, 1);
    RenderLayer(&g_game->sub, 0x40);
    if (g_game->mode->FUN_00435100() != 3) {
        g_game->flags_38a51 |= 1;
    }
    g_game->field_10->FUN_004ce910(1);
}
