// Decompiled by Sonnet. Names are provisional.

void __stdcall IsGadgetNamed(int param_1, int param_2, const char* param_3);

struct Sub18_4ac300 {
    int unknown_0;
    int field4;
};

struct Obj_4ac300 {
    char unknown_0[0x18];
    Sub18_4ac300* field18;          // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field60;                     // +0x60
};

// FUNCTION: 0x4ac300
void __stdcall InputDialogHandler(Obj_4ac300* param_1)
{
    int esi = param_1->field18->field4;
    int edi = param_1->field60;
    IsGadgetNamed(esi, edi, "CHC1");
    IsGadgetNamed(esi, edi, "CHC2");
    IsGadgetNamed(esi, edi, "INPT");
}
