// Decompiled by Haiku. Names are provisional.

struct Dialog {
    char unknown_0[0x18];
    int field_18;
};

// FUNCTION: 0x4ab040
int __stdcall FUN_004ab040(Dialog* obj)
{
    return obj->field_18 != 0;
}
