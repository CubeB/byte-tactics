// Decompiled by Opus. Names are provisional.
// Handler of the help dialog (HELP.GUI, opened by 0x45fb30): "OK" closes it,
// "Page" shows the selected page.

struct Gadget_0045fac0 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

int __stdcall IsCurrentGadgetNamed(Gadget_0045fac0* gadget, char* name);
void __stdcall FUN_0047f1a0(char* str, int flag);
int __stdcall GetButtonStageByName(Gadget_0045fac0* gadget, char* name);
void __stdcall FUN_0045f8c0(Gadget_0045fac0* gadget, int a, int b);
void __stdcall FUN_004ab0a0(Gadget_0045fac0* gadget);

// FUNCTION: 0x45fac0
void __stdcall FUN_0045fac0(Gadget_0045fac0* gadget)
{
    if (gadget->field_60 != -1) {
        if (IsCurrentGadgetNamed(gadget, "OK")) {
            FUN_0047f1a0("Options", 0);
            return;
        }
        if (IsCurrentGadgetNamed(gadget, "Page")) {
            FUN_0047f1a0("Options", 0);
            FUN_0045f8c0(gadget, GetButtonStageByName(gadget, "Page"), 0x11);
        }
        FUN_004ab0a0(gadget);
    }
}
