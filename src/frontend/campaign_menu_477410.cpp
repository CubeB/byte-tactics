// Decompiled by space-bunny-free. Names are provisional.
// Runs the "Difficulty" part of the restart dialog: writes the current
// difficulty into the gadget's value byte and pushes the matching button name
// into the GUI menu object at g_game + 0x519.

#pragma pack(push, 1)
// GUI gadget record, 0x15b bytes, as stored in a dialog's gadget array.
struct Gadget_00477410 {
    char unknown_0[0x137];
    unsigned char value;              // +0x137
    char unknown_138[0x15b - 0x138];
};

// A dialog loaded from a .GUI file.
struct Dialog_00477410 {
    int unknown_0;
    Gadget_00477410* gadgets;          // +0x4
};

// The GUI system object, at g_game + 0x519.
struct Menu_00477410 {
    char unknown_0[0x18];
};

struct Game {
    char unknown_0[0x519];
    Menu_00477410 menu;                // +0x519
    Dialog_00477410* dialog;           // +0x531
    char unknown_535[0x37eee - 0x535];
    int difficulty;                    // +0x37eee
};
#pragma pack(pop)

extern Game* g_game;

Gadget_00477410* __stdcall FUN_0049ff10(Gadget_00477410* gadgets, const char* name);
void __stdcall FUN_004a1110(Menu_00477410* menu, const char* name, int value);
void __stdcall FUN_0049fa90(Menu_00477410* menu);

// FUNCTION: 0x477410
void FUN_00477410()
{
    Gadget_00477410* gadget = FUN_0049ff10(g_game->dialog->gadgets, "Difficulty");
    if (g_game->difficulty == 0) {
        gadget->value = 0;
        FUN_004a1110(&g_game->menu, "Easy", 1);
    }
    if (g_game->difficulty == 1) {
        gadget->value = 1;
        FUN_004a1110(&g_game->menu, "Medium", 1);
    }
    if (g_game->difficulty == 2) {
        gadget->value = 2;
        FUN_004a1110(&g_game->menu, "Hard", 1);
    }
    FUN_0049fa90(&g_game->menu);
}
