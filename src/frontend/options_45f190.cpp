// Decompiled by Opus. Names are provisional.

struct Gadget_0045f190 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

int __stdcall IsCurrentGadgetNamed(Gadget_0045f190* gadget, char* name);
void __stdcall PlaySoundByName(char* str, int flag);
void __stdcall FUN_004ab0a0(Gadget_0045f190* gadget);

// FUNCTION: 0x45f190
void __stdcall FUN_0045f190(Gadget_0045f190* gadget)
{
    if (gadget->field_60 != -1) {
        if (IsCurrentGadgetNamed(gadget, "OK")) {
            PlaySoundByName("Options", 0);
            return;
        }
        FUN_004ab0a0(gadget);
    }
}
