// Decompiled by Opus. Names are provisional.
// Reads the "hotornot" flag from a section into a 1-bit bitfield. The call
// result goes through an int local; assigning it directly keeps the old field
// value in a separate register (esi).

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

struct Source_004ae410 {
    char unknown_0[4];
    Class_004c46c0* tdf;               // +0x4
};

struct Obj_004ae410 {
    char unknown_0[0xc8];
    unsigned int hotornot : 1;         // +0xc8 bit 0
};

// FUNCTION: 0x4ae410
void __stdcall FUN_004ae410(Obj_004ae410* obj, Source_004ae410* src)
{
    int value = src->tdf->FUN_004c46c0("hotornot", 0);
    obj->hotornot = value;
}
