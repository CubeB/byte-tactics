// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Opens the YESORNO.GUI dialog and fills its CHOICE1 / CHOICE2 / TITLE
// fields; FUN_004605c0 is installed as the handler.
#include <string.h>

struct Sub_00460680 {
    char unknown_0[0x10];
};

struct Flags_00460680 {
    unsigned short unknown_bit0 : 4;
    unsigned short flag4 : 1;          // bit 4
    unsigned short unknown_rest : 11;
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Sub_00460680 sub;                  // +0x519
    char unknown_529[0x2bee - 0x529];
    Flags_00460680 flags;              // +0x2bee
};
#pragma pack(pop)

struct Gadget_00460680 {
    char unknown_0[0x4];
    char* entries;                     // +0x4
    void (__stdcall* handler)(void*);  // +0x8
};

extern Game* g_game;
extern int DAT_00512ff8;

Gadget_00460680* __stdcall FUN_004aa8f0(Sub_00460680* sub, const char* name, int flags);
void __stdcall FUN_0049fb10(Sub_00460680* sub, int value);
int __stdcall FUN_0049fdf0(void* entries, const char* name, int type);
void __stdcall FUN_004a0bf0(Sub_00460680* sub, const char* name, const char* text, int param_4);
void __stdcall FUN_004a76b0(Sub_00460680* sub, const char* name);
void __stdcall FUN_004a81e0(Sub_00460680* sub, int value);
void __stdcall FUN_004605c0(void* gadget);

// FUNCTION: 0x460680
void FUN_00460680()
{
    Gadget_00460680* gadget = FUN_004aa8f0(&g_game->sub, "YESORNO.GUI", 0x1000);
    if (gadget == 0) {
        return;
    }
    FUN_0049fb10(&g_game->sub, 1);
    char* entries = gadget->entries;
    FUN_0049fdf0(entries, "CHOICE1", 1);
    FUN_0049fdf0(entries, "CHOICE2", 1);
    // Suspected original bug: the sibling dialog setup at 0x464e70 copies
    // "CHOICE1" to entries+0xcc and "CHOICE2" to entries+0xdc, but this
    // function copies "CHOICE2" (0x503120) into both fields.
    strcpy(entries + 0xcc, "CHOICE2");
    strcpy(entries + 0xdc, "CHOICE2");
    FUN_004a0bf0(&g_game->sub, "CHOICE1", "Yes", 0);
    FUN_004a0bf0(&g_game->sub, "CHOICE2", "No", 0);
    if (DAT_00512ff8 == 0) {
        FUN_004a0bf0(&g_game->sub, "TITLE", "Surrender this battle and return to main menu?", 0);
    } else if (DAT_00512ff8 == 2) {
        const char* title = g_game->flags.flag4 ? "Exit the Battle"
                                                : "Surrender this battle and exit to Windows?";
        FUN_004a0bf0(&g_game->sub, "TITLE", title, 0);
    }
    FUN_004a76b0(&g_game->sub, "CHOICE2");
    gadget->handler = FUN_004605c0;
    FUN_004a81e0(&g_game->sub, 0x40);
}
