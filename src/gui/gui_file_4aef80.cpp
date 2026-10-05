// Decompiled by Opus. Names are provisional.

struct Obj_004aef80 {
    char unknown_0[4];
    int* field_4;                      // +0x4
};

void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x4aef80
void __stdcall FUN_004aef80(Obj_004aef80* obj)
{
    if (obj->field_4) {
        FUN_004d85a0(obj->field_4);
        obj->field_4 = 0;
    }
}
