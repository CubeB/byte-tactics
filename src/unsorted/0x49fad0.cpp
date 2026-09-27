// Decompiled by Haiku. Names are provisional.

struct Class_0049fad0_inner {
    char unknown_0[0x14];
    int field_14;
};

struct Class_0049fad0 {
    char unknown_0[0x18];
    Class_0049fad0_inner* field_18;
};

// FUNCTION: 0x49fad0
void __stdcall FUN_0049fad0(Class_0049fad0* obj)
{
    if (obj->field_18 != 0) {
        obj->field_18->field_14 = 1;
    }
}
