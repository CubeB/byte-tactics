// Decompiled by Opus. Names are provisional.
// Builds a zeroed 0x6b-byte parameter block (owner, position, kind 10) and
// hands it to ApplyAreaDamage (an ebp-framed __stdcall routine in a gap).
#include <string.h>

struct Vec3_0049a0c0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Params_0049a0c0 {
    void* owner;                       // +0x0
    Vec3_0049a0c0 pos;                 // +0x4
    char unknown_10[0x42];
    int field_52;                      // +0x52
    char unknown_56[0x10];
    char kind;                         // +0x66
    char unknown_67[4];
};
#pragma pack(pop)

void __stdcall ApplyAreaDamage(Params_0049a0c0* params, Vec3_0049a0c0* pos);

// FUNCTION: 0x49a0c0
void __stdcall ApplyAreaDamageAt(void* owner, Vec3_0049a0c0* pos)
{
    Params_0049a0c0 p;
    memset(&p, 0, sizeof(p));
    p.field_52 = 0;
    p.owner = owner;
    p.kind = 10;
    p.pos = *pos;
    ApplyAreaDamage(&p, pos);
}
