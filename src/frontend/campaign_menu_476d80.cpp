// Decompiled by space-bunny-free. Names are provisional.
// Fills the SOLARSYSTEM and TextRegion gadgets of the current dialog: both get
// g_game + 0x37ef2 incremented, and the TextRegion's text is replaced with the
// string built from the net object's data. Gadget records are 0x15b bytes.

class Mission {
public:
    char* FUN_004353a0();
    char* FUN_004356c0(int param_1);
};

#pragma pack(push, 1)
struct Gadget_00476d80 {
    char unknown_0[0x17];
    short field_17;                   // +0x17
    char unknown_19[0x28 - 0x19];
    unsigned char field_28;           // +0x28
    char unknown_29[0x15b - 0x29];
};
#pragma pack(pop)

struct Dialog_00476d80 {
    int unknown_0;
    Gadget_00476d80* gadgets;         // +0x4
    void (__stdcall* handler)(void*); // +0x8
};

struct Menu_00476d80 {
    char unknown_0[1];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_00476d80 menu;               // +0x519
    char unknown_51a[0x531 - 0x51a];
    Dialog_00476d80* dialog;          // +0x531
    char unknown_535[0x37ef2 - 0x535];
    unsigned char field_37ef2;
    char unknown_37ef3[0x391e9 - 0x37ef3];
    Mission* net;                     // +0x391e9
    char unknown_391ed[0x391f1 - 0x391ed];
    int field_391f1;                  // +0x391f1
};
#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e650;
extern char* DAT_0051e63c;

int __stdcall FindGadgetIndex(Gadget_00476d80* gadgets, const char* name, int type);
void __stdcall SelectFontForEntry(Gadget_00476d80* gadgets, int index);
char* __stdcall WordWrapText(Menu_00476d80* menu, char* text, int value, int index);
char* __stdcall FUN_00476cd0(char* text);
void DrawHelpPage();
void __stdcall StreamSoundDelayed(char* text, int a, int b);

// FUNCTION: 0x476d80
void FUN_00476d80()
{
    char* text = g_game->net->FUN_004353a0();
    if (text) {
        Gadget_00476d80* gadgets = g_game->dialog->gadgets;
        int i = FindGadgetIndex(gadgets, "SOLARSYSTEM", 0xe);
        if (i != -1) {
            gadgets[i].field_28 = g_game->field_37ef2 + 1;
        }
        int j = FindGadgetIndex(gadgets, "TextRegion", 0xe);
        gadgets[j].field_28 = g_game->field_37ef2 + 1;
        SelectFontForEntry(gadgets, j);
        DAT_0051e650 = 1;
        DAT_0051e63c = WordWrapText(&g_game->menu, text, gadgets[j].field_17, j);
        DAT_0051e63c = FUN_00476cd0(DAT_0051e63c);
        DrawHelpPage();
    }
    if (g_game->field_391f1 != 6) {
        char* name = ((Mission*)g_game->net)->FUN_004356c0(3);
        if (name) {
            StreamSoundDelayed(name, 0, 0x3c);
        }
    }
}
