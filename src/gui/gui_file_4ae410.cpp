// Decompiled by Opus. Names are provisional.
// Reads the "hotornot" flag from a section into a 1-bit bitfield. The call
// result goes through an int local; assigning it directly keeps the old field
// value in a separate register (esi).

#include "../util/tdf.h"

struct Source_004ae410 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

struct Obj_004ae410 {
    char unknown_0[0xc8];
    unsigned int hotornot : 1;         // +0xc8 bit 0
};

// FUNCTION: 0x4ae410
void __stdcall ReadHotOrNotField(Obj_004ae410* obj, Source_004ae410* src)
{
    int value = src->tdf->GetFieldInt("hotornot", 0);
    obj->hotornot = value;
}
