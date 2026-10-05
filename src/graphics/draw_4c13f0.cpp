// Decompiled by Haiku. Names are provisional.

extern void* FUN_004b6220();

struct Obj {
    char unknown_0[0x210];
    int field_210;
};

// FUNCTION: 0x4c13f0
int FUN_004c13f0()
{
    Obj* obj = (Obj*)FUN_004b6220();
    return obj->field_210;
}
