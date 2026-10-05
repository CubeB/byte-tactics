// Decompiled by Sonnet. Names are provisional.

struct Obj_444ba0 {
    char unknown_0[0x60];
    int field_60;              // +0x60
};

extern char DAT_00502ae8[];    // "OK"
extern char DAT_00505974[];    // "Multi"

extern "C" int __stdcall IsCurrentGadgetNamed(Obj_444ba0* obj, char* str);
extern "C" void __stdcall FUN_0047f1a0(char* str, int flag);
extern "C" void __stdcall FUN_004ab0a0(Obj_444ba0* obj);

// FUNCTION: 0x444ba0
void __stdcall FUN_00444ba0(Obj_444ba0* param_1)
{
    if (param_1->field_60 != -1) {
        if (IsCurrentGadgetNamed(param_1, DAT_00502ae8)) {
            FUN_0047f1a0(DAT_00505974, 0);
        } else {
            FUN_004ab0a0(param_1);
        }
    }
}
