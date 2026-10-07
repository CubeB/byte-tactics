// Decompiled by Opus. Names are provisional.
// Reads the "filename" key of a TDF entry (sibling of 0x4ae300).
#include <string.h>

#include "../util/tdf.h"

struct Source_004ae4b0 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004ae4b0 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
};
#pragma pack(pop)

extern char DAT_005119b8[];

// FUNCTION: 0x4ae4b0
void __stdcall FUN_004ae4b0(Obj_004ae4b0* obj, Source_004ae4b0* src)
{
    src->tdf->GetFieldString(obj->text, "filename", 0x20, DAT_005119b8);
}
