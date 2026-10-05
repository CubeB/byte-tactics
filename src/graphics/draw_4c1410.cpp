// Decompiled by Haiku. Names are provisional.

extern void* GetDisplay();

struct Obj {
    char unknown_0[0x20c];
    int field_20c;
};

// FUNCTION: 0x4c1410
int GetTextBackColor()
{
    Obj* obj = (Obj*)GetDisplay();
    return obj->field_20c;
}
