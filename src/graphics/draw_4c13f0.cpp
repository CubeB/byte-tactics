// Decompiled by Haiku. Names are provisional.

extern void* GetDisplay();

struct Obj {
    char unknown_0[0x210];
    int field_210;
};

// FUNCTION: 0x4c13f0
int GetTextKeyColor()
{
    Obj* obj = (Obj*)GetDisplay();
    return obj->field_210;
}
