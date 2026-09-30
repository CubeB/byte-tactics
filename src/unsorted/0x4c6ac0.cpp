// Decompiled by Opus. Names are provisional.
// Frees an object if its "owned" flag (bit 0 of +0x2c) is set.

struct Struct_004c6ac0 {
    char unknown_0[0x2c];
    unsigned char flags;               // +0x2c
};

void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4c6ac0
void __stdcall FUN_004c6ac0(Struct_004c6ac0* obj)
{
    if (obj != 0 && (obj->flags & 1)) {
        FUN_004d85a0(obj);
    }
}
