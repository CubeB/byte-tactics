// Decompiled by Opus. Names are provisional.
// Frees one of the three buffers at +0x8 and clears it and field_14.

void __cdecl FUN_004d85a0(int* param_1);

struct Obj_004aeda0 {
    char unknown_0[8];
    int* buffers[3];                   // +0x8
    int field_14;                      // +0x14
};

// FUNCTION: 0x4aeda0
void __stdcall FUN_004aeda0(Obj_004aeda0* obj, int i)
{
    if (obj->buffers[i] != 0) {
        FUN_004d85a0(obj->buffers[i]);
        obj->buffers[i] = 0;
        obj->field_14 = 0;
    }
}
