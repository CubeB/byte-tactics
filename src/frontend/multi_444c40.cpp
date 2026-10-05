// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Gadget_00444c40 {
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
};
#pragma pack(pop)

struct Inner_00444c40 {
    int unknown_0;
    void* gadgets;                     // +0x04
};

struct Menu_00444c40 {
    char unknown_0[0x18];
    Inner_00444c40* inner;             // +0x18
};

class Class_00435a20 {
public:
    int FUN_00435a20(char* name);
};

#pragma pack(push, 1)
struct Game_00444c40 {
    char unknown_0[0x391e9];
    Class_00435a20* field_391e9;       // +0x391e9
};
#pragma pack(pop)

extern Game_00444c40* g_game;

Gadget_00444c40* __stdcall FUN_0049ff90(void* gadgets, char* name);
char* __stdcall FUN_004b6af0(char* text, int n);
void __stdcall FUN_004a0570(Menu_00444c40* menu, char* name, int value);
void FUN_00444a20();

// FUNCTION: 0x444c40
void __stdcall FUN_00444c40(Menu_00444c40* menu, int unused)
{
    Gadget_00444c40* g = FUN_0049ff90(menu->inner->gadgets, "MAPNAMES");
    if (g_game->field_391e9->FUN_00435a20(FUN_004b6af0(g->text, g->selected)) == 0) {
        FUN_004a0570(menu, "MAPPIC", 0);
    } else {
        FUN_004a0570(menu, "MAPPIC", 1);
        FUN_00444a20();
    }
}
