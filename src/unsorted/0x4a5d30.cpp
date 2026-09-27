// Decompiled by Opus. Names are provisional.

struct Struct_004a5d30 {
    char unknown_0[8];
    int values[3];                     // +0x8
    int current;                       // +0x14
};

// FUNCTION: 0x4a5d30
void __stdcall FUN_004a5d30(Struct_004a5d30* p, int index)
{
    p->current = p->values[index];
}
