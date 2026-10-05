// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Gadget_0047aaa0 {
    char unknown_0[0xba];
    short selected;                    // +0xba
    char unknown_bc[0xc2 - 0xbc];
    char* text;                        // +0xc2
};
#pragma pack(pop)

struct Inner_0047aaa0 {
    int unknown_0;
    void* gadgets;                     // +0x04
};

struct Menu_0047aaa0 {
    char unknown_0[0x18];
    Inner_0047aaa0* inner;             // +0x18
};

class Class_00435a20 {
public:
    int LoadMissionByName(char* name);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Class_00435a20* field_391e9;       // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

Gadget_0047aaa0* __stdcall FindGadgetChecked(void* gadgets, char* name);
char* __stdcall FUN_004b6af0(char* text, int n);
void ShowSelectedMapInfo();

// Like 0x444c40, without the MAPPIC update.
// FUNCTION: 0x47aaa0
void __stdcall FUN_0047aaa0(Menu_0047aaa0* menu, int unused)
{
    Gadget_0047aaa0* g = FindGadgetChecked(menu->inner->gadgets, "MAPNAMES");
    if (g_game->field_391e9->LoadMissionByName(FUN_004b6af0(g->text, g->selected)) != 0) {
        ShowSelectedMapInfo();
    }
}
