// Decompiled by Opus. Names are provisional.
// Handler of the help dialog (HELP.GUI, opened by 0x45fb30): "OK" closes it,
// "Page" shows the selected page.

struct Gadget_0045fac0 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

int __stdcall IsCurrentGadgetNamed(Gadget_0045fac0* gadget, char* name);
void __stdcall PlaySoundByName(char* str, int flag);
int __stdcall GetButtonStageByName(Gadget_0045fac0* gadget, char* name);
void __stdcall FillHelpPage(Gadget_0045fac0* gadget, int a, int b);
void __stdcall FUN_004ab0a0(Gadget_0045fac0* gadget);

// FUNCTION: 0x45fac0
void __stdcall HandleHelpClick(Gadget_0045fac0* gadget)
{
    if (gadget->field_60 != -1) {
        if (IsCurrentGadgetNamed(gadget, "OK")) {
            PlaySoundByName("Options", 0);
            return;
        }
        if (IsCurrentGadgetNamed(gadget, "Page")) {
            PlaySoundByName("Options", 0);
            FillHelpPage(gadget, GetButtonStageByName(gadget, "Page"), 0x11);
        }
        FUN_004ab0a0(gadget);
    }
}
