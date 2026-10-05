// Decompiled by Sonnet. Names are provisional.
// A free __stdcall function (param comes off the stack, ecx unused): waits
// on a handle then clears a 1-bit flag inside a dword bitfield (the load of
// the whole dword but an `and al, 0xfe` on just the low byte is the
// dword-bitfield clear idiom from the guide).

extern void __stdcall FUN_004c2b20(int handle);

struct Flags_004ab4c0
{
    unsigned int active : 1;
    unsigned int rest : 31;
};

struct Obj_004ab4c0
{
    char unknown_0[0x1c];
    int field_1c;
    char unknown_20[0x3c];
    Flags_004ab4c0 flags_5c;
};

// FUNCTION: 0x4ab4c0
void __stdcall FUN_004ab4c0(Obj_004ab4c0* p)
{
    FUN_004c2b20(p->field_1c);
    p->flags_5c.active = 0;
}
