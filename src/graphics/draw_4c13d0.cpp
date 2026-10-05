// Decompiled by Opus. Names are provisional.
// Stores a value at +0x210 of the object GetDisplay returns; the setter
// for the getter GetTextKeyColor.

struct Obj_004c13d0 {
    char unknown_0[0x210];
    int field_210;                     // +0x210
};

extern void* GetDisplay();

// FUNCTION: 0x4c13d0
void __stdcall SetTextKeyColor(int value)
{
    Obj_004c13d0* obj = (Obj_004c13d0*)GetDisplay();
    obj->field_210 = value;
}
