// Decompiled by space-bunny-free. Names are provisional.
#include <string.h>

struct Form_00464e70 {
    char unknown_0[0xcc];
    char choice1[0x10];                // +0xcc
    char choice2[0x10];                // +0xdc
};

struct Gadget_00464de0;

struct Screen_00464e70 {
    char unknown_0[4];
    Form_00464e70* form;               // +0x4
    void (__stdcall *callback)(Gadget_00464de0*); // +0x8
};

struct Menu_00464e70 {
    char unknown_0[0x40];
};

#pragma pack(push, 1)
struct Game_00464e70 {
    char unknown_0[0x519];
    Menu_00464e70 menu;                // +0x519
};
#pragma pack(pop)

extern Game_00464e70* g_game;

extern char DAT_00503120[];
extern char DAT_00503128[];
extern char DAT_0050313c[];
extern char DAT_00503160[];
extern char DAT_00503164[];
extern char DAT_00503168[];
extern char DAT_00507318[];

Screen_00464e70* __stdcall FUN_004aa8f0(Menu_00464e70* menu, char* name, int value);
void __stdcall FUN_0049fb10(Menu_00464e70* menu, int flag);
void __stdcall FUN_004a0bf0(Menu_00464e70* menu, char* name, char* text, int value);
void __stdcall FUN_004a81e0(Menu_00464e70* menu, int value);
void __stdcall FUN_00464de0(Gadget_00464de0* gadget);

// FUNCTION: 0x464e70
void FUN_00464e70()
{
    Screen_00464e70* screen = FUN_004aa8f0(&g_game->menu, DAT_00503168, 0x900);
    if (screen) {
        Form_00464e70* form;
        FUN_0049fb10(&g_game->menu, 1);
        form = screen->form;
        FUN_004a0bf0(&g_game->menu, DAT_00503128, DAT_00503164, 0);
        FUN_004a0bf0(&g_game->menu, DAT_00503120, DAT_00503160, 0);
        FUN_004a0bf0(&g_game->menu, DAT_0050313c, DAT_00507318, 0);
        strcpy(form->choice1, DAT_00503128);
        strcpy(form->choice2, DAT_00503120);
        screen->callback = FUN_00464de0;
        FUN_004a81e0(&g_game->menu, 0x40);
    }
}
