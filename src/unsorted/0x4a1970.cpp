// Decompiled by Opus. Names are provisional.

struct Struct_004a1970 {
    char unknown_0[0xc];
    int field_c;                       // +0xc
};

// FUNCTION: 0x4a1970
int __stdcall FUN_004a1970(Struct_004a1970* obj, int value)
{
    return value > obj->field_c;
}
