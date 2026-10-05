// Decompiled by Sonnet. Names are provisional.

struct Inner_00444910 {
    char unknown_0[4];
    int field_4;
};

struct Obj_00444910 {
    char unknown_0[0x18];
    Inner_00444910* field_18;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                       // +0x60
};

extern int __stdcall FindGadgetIndex(int value, const char* name, int flag);

// FUNCTION: 0x444910
void __stdcall FUN_00444910(Obj_00444910* param1, int param2)
{
    param1->field_60 = FindGadgetIndex(param1->field_18->field_4, "LOGOS", 2);
}
