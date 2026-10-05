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

class Mission {
public:
    int LoadMissionByName(char* name);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Mission* field_391e9;              // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

Gadget_00444c40* __stdcall FindGadgetChecked(void* gadgets, char* name);
char* __stdcall SkipTextLines(char* text, int n);
void __stdcall FUN_004a0570(Menu_00444c40* menu, char* name, int value);
void ShowSelectedMapInfo();

// FUNCTION: 0x444c40
void __stdcall UpdateMapSelection(Menu_00444c40* menu, int unused)
{
    Gadget_00444c40* g = FindGadgetChecked(menu->inner->gadgets, "MAPNAMES");
    if (g_game->field_391e9->LoadMissionByName(SkipTextLines(g->text, g->selected)) == 0) {
        FUN_004a0570(menu, "MAPPIC", 0);
    } else {
        FUN_004a0570(menu, "MAPPIC", 1);
        ShowSelectedMapInfo();
    }
}
