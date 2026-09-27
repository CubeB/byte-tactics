// Decompiled by Haiku. Names are provisional.

struct Obj_004a1950 {
    char unknown_0[4];
    int field_4;
};

// FUNCTION: 0x4a1950
int __stdcall FUN_004a1950(Obj_004a1950* param_1, int param_2) {
    return param_2 < param_1->field_4;
}
