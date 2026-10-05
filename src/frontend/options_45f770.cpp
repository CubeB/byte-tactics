// Decompiled by Opus. Names are provisional.

struct Gadget_0045f770 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

struct Game {
    char unknown_0[0x519];
    char field_519[1];                 // +0x519
};

extern Game* g_game;

int __stdcall IsCurrentGadgetNamed(Gadget_0045f770* gadget, char* name);
void __stdcall PlaySoundByName(char* str, int flag);
void __stdcall FUN_0049fa90(Gadget_0045f770* gadget);
void __stdcall FUN_004ab0a0(Gadget_0045f770* gadget);
void __stdcall FUN_004afcf0(int param_1);
void FUN_00476ef0();

// FUNCTION: 0x45f770
void __stdcall FUN_0045f770(Gadget_0045f770* gadget)
{
    if (gadget->field_60 == -1) {
        FUN_004afcf0((int)g_game->field_519);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "OK")) {
        PlaySoundByName("Options", 0);
        return;
    }
    if (IsCurrentGadgetNamed(gadget, "TextRegion") || IsCurrentGadgetNamed(gadget, "MOREBAR")) {
        PlaySoundByName("Options", 0);
        FUN_00476ef0();
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
    }
    if (gadget->field_60 != -1)
        FUN_004ab0a0(gadget);
}
