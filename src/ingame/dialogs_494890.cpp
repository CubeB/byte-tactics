// Decompiled by Opus. Names are provisional.

struct Gadget_00494890 {
    char unknown_0[0x60];
    int field_60;                      // +0x60
};

void __stdcall FUN_004ab0a0(Gadget_00494890* gadget);

// FUNCTION: 0x494890
void __stdcall FUN_00494890(Gadget_00494890* gadget)
{
    if (gadget->field_60 != -1) {
        FUN_004ab0a0(gadget);
    }
}
