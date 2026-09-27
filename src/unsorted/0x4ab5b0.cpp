// Decompiled by Opus. Names are provisional.
// Returns 1 when any of the mask bits are set in the object's field at +0x54.

struct Object_004ab5b0 {
    char unknown_0[0x54];
    unsigned int flags;                // +0x54
};

// FUNCTION: 0x4ab5b0
int __stdcall FUN_004ab5b0(Object_004ab5b0* obj, unsigned int mask)
{
    return (obj->flags & mask) != 0;
}
