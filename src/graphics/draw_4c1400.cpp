// Decompiled by Haiku. Names are provisional.

extern void* FUN_004b6220();

struct Obj {
    char unknown_0[0x208];
    int field_208;
};

// FUNCTION: 0x4c1400
int FUN_004c1400()
{
    Obj* obj = (Obj*)FUN_004b6220();
    return obj->field_208;
}
