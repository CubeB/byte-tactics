// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Opens the EXITMENU.GUI dialog (handler 0x460800). When the current mode is
// restart (1 or 2) it selects RESTART and renames the button; otherwise, when
// flag bit 4 of g_game+0x2bee is set, it shows MAINMENU.

class Mission {
public:
    int GetGameType();
};

struct Sub_004608b0 {
    char unknown_0[0x1c];
};

struct Flags_004608b0 {
    unsigned short unknown_bit0 : 4;
    unsigned short flag4 : 1;          // bit 4
    unsigned short unknown_rest : 11;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Sub_004608b0 sub;                  // +0x519
    char unknown_535[0x2bee - 0x535];
    Flags_004608b0 flags;              // +0x2bee
    char unknown_2bf0[0x391e9 - 0x2bf0];
    Mission* field_391e9;              // +0x391e9
};
#pragma pack(pop)

struct Dialog_004608b0 {
    int unknown_0;                     // +0x0
    void* gadgets;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

extern Game* g_game;

Dialog_004608b0* __stdcall LoadGuiLayer(Sub_004608b0* sub, const char* name, int flags);
int __stdcall FindGadgetIndex(void* gadgets, const char* name, int type);
void __stdcall FUN_004a0570(Sub_004608b0* sub, const char* name, int value);
char* __stdcall Translate(char* text);
void __stdcall FUN_004a0bf0(Sub_004608b0* sub, const char* name, int value, int param_4);
void __stdcall FUN_0049fb10(Sub_004608b0* sub, int value);
void __stdcall RenderLayer(Sub_004608b0* sub, int value);
void __stdcall HandleExitMenuClick(void* gadget);

// FUNCTION: 0x4608b0
void OpenExitMenu()
{
    Dialog_004608b0* dialog = LoadGuiLayer(&g_game->sub, "EXITMENU.GUI", 0x1800);
    dialog->handler = HandleExitMenuClick;
    FindGadgetIndex(dialog->gadgets, "RESTART", 1);
    if (g_game->field_391e9->GetGameType() == 1) {
        FUN_004a0570(&g_game->sub, "RESTART", 1);
        FUN_004a0bf0(&g_game->sub, "RESTART", (int)Translate("Restart"), 0x80);
        goto tail;
    }
    // Restart body written twice on purpose: the compiler merges them and lays
    // restart out before main-menu.
    if (g_game->field_391e9->GetGameType() == 2) {
        FUN_004a0570(&g_game->sub, "RESTART", 1);
        FUN_004a0bf0(&g_game->sub, "RESTART", (int)Translate("Restart"), 0x80);
        goto tail;
    }
    if (g_game->flags.flag4) {
        FUN_004a0570(&g_game->sub, "MAINMENU", 0);
    }
tail:
    FUN_0049fb10(&g_game->sub, 1);
    RenderLayer(&g_game->sub, 0x40);
}