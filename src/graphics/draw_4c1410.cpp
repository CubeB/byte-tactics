// Decompiled by Haiku. Names are provisional.

extern void* FUN_004b6220();

struct Obj {
    char unknown_0[0x20c];
    int field_20c;
};

// FUNCTION: 0x4c1410
int FUN_004c1410()
{
    Obj* obj = (Obj*)FUN_004b6220();
    return obj->field_20c;
}
