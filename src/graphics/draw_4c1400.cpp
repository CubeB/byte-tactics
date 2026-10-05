// Decompiled by Haiku. Names are provisional.

extern void* GetDisplay();

struct Obj {
    char unknown_0[0x208];
    int field_208;
};

// FUNCTION: 0x4c1400
int GetTextForeColor()
{
    Obj* obj = (Obj*)GetDisplay();
    return obj->field_208;
}
