// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>

struct Gadget_44b3c0 {
    char unknown_0[0xba];
    short selected;                    // +0xba
};

struct Inner_44b3c0 {
    Inner_44b3c0* unknown_0;           // +0
    void* gadgets;                     // +0x4
};

struct Menu_44b3c0 {
    char unknown_0[0x18];
    Inner_44b3c0* inner;               // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60
};

extern char* g_game;                   // 0x511de8
extern char* DAT_005129ac;
extern char* DAT_005129b0;
extern char* DAT_005091c8;             // savegame directory

int __stdcall IsCurrentGadgetNamed(void* menu, char* name);
void __stdcall PlaySoundByName(char* name, int flag);
Gadget_44b3c0* __stdcall FindGadgetChecked(void* gadgets, char* name);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall FUN_0044b140(char* path);
void __stdcall FUN_0044bfd0(void* menu, int flag);
void __stdcall FUN_004ab0a0(void* menu);
void __cdecl FUN_004d85a0(void* param_1);

// FUNCTION: 0x44b3c0
void __stdcall FUN_0044b3c0(Menu_44b3c0* menu)
{
    void* gadgets = menu->inner->gadgets;
    if (menu->current == -1)
        return;
    if (IsCurrentGadgetNamed(menu, "CANCEL")) {
        PlaySoundByName("Previous", 0);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "LOAD") || IsCurrentGadgetNamed(menu, "GAMES")) {
        PlaySoundByName("Options", 0);
        Gadget_44b3c0* games = FindGadgetChecked(gadgets, "GAMES");
        sprintf(g_game + 0x38c6b, "%s\\%s", DAT_005091c8,
                FUN_004b6af0(DAT_005129ac, games->selected));
        FUN_0044b140(g_game + 0x38c6b);
        Inner_44b3c0* inner = menu->inner;
        menu->inner = inner->unknown_0;
        FUN_0044bfd0(menu, 0);
        menu->inner = inner;
        if (DAT_005129ac)
            FUN_004d85a0(DAT_005129ac);
        if (DAT_005129b0)
            FUN_004d85a0(DAT_005129b0);
        DAT_005129b0 = 0;
        DAT_005129ac = 0;
    } else if (menu->current != -1) {
        FUN_004ab0a0(menu);
    }
}
