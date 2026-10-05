// Decompiled by Opus. Names are provisional.
// Reads the "filename" key of a TDF entry (byte-identical to 0x4ae4b0).
#include <string.h>

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, size_t size, char* def);
};

struct Source_004ae550 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004ae550 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
};
#pragma pack(pop)

extern char DAT_005119b8[];

// FUNCTION: 0x4ae550
void __stdcall FUN_004ae550(Obj_004ae550* obj, Source_004ae550* src)
{
    src->tdf->GetFieldString(obj->text, "filename", 0x20, DAT_005119b8);
}
