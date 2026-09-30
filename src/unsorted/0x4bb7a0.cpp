// Decompiled by Haiku. Names are provisional.

struct Struct_4bb7a0 {
    void* field_0;
    int field_4;
    char unknown_8[4];
    long field_c;
};

long __cdecl ftell(void*);

// FUNCTION: 0x4bb7a0
long __stdcall FUN_004bb7a0(Struct_4bb7a0* param_1)
{
    if (param_1->field_4 != 0) {
        return param_1->field_c;
    }
    return ftell(param_1->field_0);
}
