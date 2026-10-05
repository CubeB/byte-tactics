// Decompiled by Sonnet. Names are provisional.

void __stdcall FindGadgetIndex(void* param_1, const char* param_2, int param_3);

struct Obj_004ac130 {
    char unknown_0[0x18];
    void** field_18;                    // +0x18
};

// FUNCTION: 0x4ac130
void __stdcall Choice3DialogHandler(Obj_004ac130* param_1)
{
    void* p = param_1->field_18[1];
    FindGadgetIndex(p, "CHC1", 0xe);
    FindGadgetIndex(p, "CHC2", 0xe);
    FindGadgetIndex(p, "CHC3", 0xe);
}
