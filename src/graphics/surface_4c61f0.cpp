// Decompiled by Haiku. Names are provisional.

struct Class_004c61f0 {
    char unknown_0[0x98];
    int field_98;
};

extern Class_004c61f0* GetDisplay();

// FUNCTION: 0x4c61f0
void __stdcall SetRestoreSurface(int param_1)
{
    Class_004c61f0* obj = GetDisplay();
    obj->field_98 = param_1;
}
