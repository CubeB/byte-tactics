// Decompiled by Space Bunny Free. Names are provisional.
#include <string.h>

// Opens RESTART.GUI, fills the two MISSIONNAME text gadgets with the first and
// second line of the wrapped mission name of the current mission, then shows
// the dialog.

// GUI gadget, 0x15b bytes, as stored in a dialog's gadget array.
#pragma pack(push, 1)
struct Gadget_004604a0 {
    char unknown_0[0x17];
    short field_17;                   // +0x17
    char unknown_19[0x15b - 0x19];
};
#pragma pack(pop)

// A dialog loaded from a .GUI file.
struct Dialog_004604a0 {
    int unknown_0;
    Gadget_004604a0* gadgets;         // +0x4
    void (__stdcall* handler)(void*); // +0x8
};

// The GUI system object, at g_game + 0x519.
struct Menu_004604a0 {
    int unknown_0;
    int unknown_4;
    int field_8;
    int field_c;
    int unknown_10;
    int field_14;                     // +0x14
    char unknown_18[0x1c - 0x18];
};

class Mission {
public:
    char* FUN_00435c30(int player);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_004604a0 menu;               // +0x519
    char unknown_535[0x391e9 - 0x535];
    Mission* texts;                   // +0x391e9
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

Dialog_004604a0* __stdcall LoadGuiLayer(Menu_004604a0* menu, const char* name, int flags);
void __stdcall LoadPictureCached(const char* name, int a, int b, int c);
int __stdcall FindGadgetIndex(Gadget_004604a0* gadgets, const char* name, int flag);
// Called with three arguments here, but the function itself pops four, so the
// declaration the original was compiled against must have been a three
// parameter one: the extra dword is left on the stack. Kept as it is.
char* __stdcall WordWrapText(Menu_004604a0* menu, char* text, int player);
void __stdcall FUN_004a0bf0(Menu_004604a0* menu, const char* name, int value, int count);
void __stdcall FUN_0049fb10(Menu_004604a0* menu, int value);
void __stdcall RenderLayer(Menu_004604a0* menu, int value);
int __stdcall FUN_00491c80(int value);
void FUN_00477410();
void __stdcall HandleRestartDialogClick(void* dialog);

// FUNCTION: 0x4604a0
void OpenRestartDialog()
{
    Menu_004604a0* menu = &g_game->menu;
    Dialog_004604a0* dialog = LoadGuiLayer(menu, "RESTART.GUI", 0x1000);
    Gadget_004604a0* gadgets = dialog->gadgets;
    dialog->handler = HandleRestartDialogClick;
    LoadPictureCached("drestart", 0, 0, 0);
    int index = FindGadgetIndex(gadgets, "MISSIONNAME", 5);
    menu->field_14 = menu->field_c;
    char* text = WordWrapText(&g_game->menu,
                              g_game->texts->FUN_00435c30(gadgets[index].field_17),
                              -1);
    menu->field_14 = menu->field_8;
    char* first = strtok(text, "\n");
    FUN_004a0bf0(&g_game->menu, "MISSIONNAME", (int)first, 0x80);
    char* second = strtok(0, "\n");
    if (second) {
        FUN_004a0bf0(&g_game->menu, "MISSIONNAME1", (int)second, 0x80);
    }
    FUN_00477410();
    FUN_0049fb10(&g_game->menu, 1);
    RenderLayer(&g_game->menu, 0x40);
    FUN_00491c80(0x13);
}
