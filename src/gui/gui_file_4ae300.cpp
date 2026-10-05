// Decompiled by Opus. Names are provisional.
#include <string.h>

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

struct Source_004ae300 {
    char unknown_0[4];
    Class_004c48c0* tdf;               // +0x4
};

#pragma pack(push, 1)
struct Obj_004ae300 {
    char unknown_0[0xb6];
    char text[0x80];                   // +0xb6
    char link[0x11];                   // +0x136
    char field_147;                    // +0x147
};
#pragma pack(pop)

extern char DAT_005119b8[];

char* __stdcall FUN_004c5740(char* text);

// FUNCTION: 0x4ae300
void __stdcall ReadTextLinkFields(Obj_004ae300* obj, Source_004ae300* src)
{
    obj->field_147 = 0;
    obj->link[0] = 0;
    memset(obj->text, 0, sizeof(obj->text));
    src->tdf->FUN_004c48c0(obj->text, "text", 0x80, DAT_005119b8);
    strncpy(obj->text, FUN_004c5740(obj->text), 0x7f);
    src->tdf->FUN_004c48c0(obj->link, "link", 0x10, DAT_005119b8);
}
