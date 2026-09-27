// Decompiled by Space Bunny Free. Names are provisional.

#pragma pack(push, 1)
// GUI layout entry, as returned by FUN_0049ff90.
struct Layout_004779e0 {
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
};
#pragma pack(pop)

// Gadget table held by the menu.
struct Gadgets_004779e0 {
    int unknown_0;
    void* gadgets;                     // +0x04
};

struct Menu_004779e0 {
    char unknown_0[0x18];
    Gadgets_004779e0* gadgets;         // +0x18
};

struct Table_004779e0 {
    int unknown_0;
    Layout_004779e0* entries;          // +0x4
};

class Class_00435110 {
public:
    int FUN_00435110(char* name);
};

class Class_00435760 {
public:
    int FUN_00435760(int* list);
};

#pragma pack(push, 1)
struct Game_004779e0 {
    char unknown_0[0x519];
    // The menu at +0x519 is only ever passed on by address, so it stays an
    // unnamed blob here; `g_game + 0x519` is what the code computes.
    char unknown_519[0x531 - 0x519];
    Table_004779e0* table;             // +0x531
    char unknown_535[0x391e9 - 0x535];
    Class_00435110* net;               // +0x391e9
};
#pragma pack(pop)

extern Game_004779e0* g_game;
extern char* DAT_0051e660;

void FUN_004d85a0(int* param_1);
Layout_004779e0* __stdcall FUN_0049ff90(Layout_004779e0* entries, char* name);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall FUN_004a32a0(void* menu, char* name, char* text, int count, int flag);
int __stdcall FUN_0049fdf0(void* gadgets, char* name, int type);
void __stdcall FUN_004a2be0(void* menu, int index);
void __stdcall FUN_0049fa90(void* menu);

// FUNCTION: 0x4779e0
void __stdcall FUN_004779e0(Menu_004779e0* menu, int unused)
{
    Gadgets_004779e0* gadgets = menu->gadgets;
    if (DAT_0051e660 != 0) {
        FUN_004d85a0((int*)DAT_0051e660);
        DAT_0051e660 = 0;
    }
    Layout_004779e0* layout =
        FUN_0049ff90(g_game->table->entries, "Campaign");
    g_game->net->FUN_00435110(FUN_004b6af0(layout->text, layout->selected));
    int count = ((Class_00435760*)g_game->net)->FUN_00435760((int*)&DAT_0051e660);
    FUN_004a32a0(menu, "Missions", DAT_0051e660, count, 0);
    FUN_004a2be0((char*)g_game + 0x519,
                 FUN_0049fdf0(gadgets->gadgets, "Missions", 2));
    FUN_0049fa90((char*)g_game + 0x519);
}
