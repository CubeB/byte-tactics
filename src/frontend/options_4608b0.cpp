// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Opens the EXITMENU.GUI dialog (handler 0x460800). When the current mode is
// restart (1 or 2) it selects RESTART and renames the button; otherwise, when
// flag bit 4 of g_game+0x2bee is set, it shows MAINMENU. The restart body is
// written twice on purpose: MSVC 5 merges the two identical blocks into one,
// and that is what lays the restart block out before the main-menu block and
// allocates the registers the original uses. A plain if/else-if puts the
// main-menu block first and comes out 4 bytes different.

class Class_00435100 {
public:
    int FUN_00435100();
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
    Class_00435100* field_391e9;       // +0x391e9
};
#pragma pack(pop)

struct Dialog_004608b0 {
    int unknown_0;                     // +0x0
    void* gadgets;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

extern Game* g_game;

Dialog_004608b0* __stdcall FUN_004aa8f0(Sub_004608b0* sub, const char* name, int flags);
int __stdcall FUN_0049fdf0(void* gadgets, const char* name, int type);
void __stdcall FUN_004a0570(Sub_004608b0* sub, const char* name, int value);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004a0bf0(Sub_004608b0* sub, const char* name, int value, int param_4);
void __stdcall FUN_0049fb10(Sub_004608b0* sub, int value);
void __stdcall FUN_004a81e0(Sub_004608b0* sub, int value);
void __stdcall FUN_00460800(void* gadget);

// FUNCTION: 0x4608b0
void FUN_004608b0()
{
    Dialog_004608b0* dialog = FUN_004aa8f0(&g_game->sub, "EXITMENU.GUI", 0x1800);
    dialog->handler = FUN_00460800;
    FUN_0049fdf0(dialog->gadgets, "RESTART", 1);
    if (g_game->field_391e9->FUN_00435100() == 1) {
        FUN_004a0570(&g_game->sub, "RESTART", 1);
        FUN_004a0bf0(&g_game->sub, "RESTART", (int)FUN_004c5740("Restart"), 0x80);
        goto tail;
    }
    if (g_game->field_391e9->FUN_00435100() == 2) {
        FUN_004a0570(&g_game->sub, "RESTART", 1);
        FUN_004a0bf0(&g_game->sub, "RESTART", (int)FUN_004c5740("Restart"), 0x80);
        goto tail;
    }
    if (g_game->flags.flag4) {
        FUN_004a0570(&g_game->sub, "MAINMENU", 0);
    }
tail:
    FUN_0049fb10(&g_game->sub, 1);
    FUN_004a81e0(&g_game->sub, 0x40);
}