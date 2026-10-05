// Decompiled by Haiku. Names are provisional.

extern void* __cdecl FUN_004d83b0(const char* param_1, int param_2);
extern const char DAT_0050a43c[];

struct Obj_004ba610 {
    char unknown_0[0xc4];
    void* field_c4;
};

// FUNCTION: 0x4ba610
int __stdcall FUN_004ba610(Obj_004ba610* param_1) {
    void* result = FUN_004d83b0(DAT_0050a43c, 0x2000);
    param_1->field_c4 = result;
    return 1;
}
