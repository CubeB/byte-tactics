// Decompiled by Opus. Names are provisional.

struct Gadget_00494840 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Menu_00494840 {
    char unknown_0[1];
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    Menu_00494840 menu;                // +0x519
    char unknown_51a[0x2bee - 0x51a];
    unsigned short flags;              // +0x2bee
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall FUN_0049fa70(Menu_00494840* menu);
int __stdcall IsCurrentGadgetNamed(Gadget_00494840* gadget, char* name);
void __stdcall FUN_004ab0a0(Gadget_00494840* gadget);

// FUNCTION: 0x494840
void __stdcall FUN_00494840(Gadget_00494840* gadget)
{
    if (gadget->field_60 == -1) {
        g_game->flags &= 0xff1f;
        FUN_0049fa70(&g_game->menu);
        return;
    }
    if (!IsCurrentGadgetNamed(gadget, "CANCEL"))
        FUN_004ab0a0(gadget);
}
