// Decompiled by Opus. Names are provisional.
#include <string.h>

#include "../util/tdf.h"

struct Source_004ae300 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
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

char* __stdcall Translate(char* text);

// FUNCTION: 0x4ae300
void __stdcall ReadTextLinkFields(Obj_004ae300* obj, Source_004ae300* src)
{
    obj->field_147 = 0;
    obj->link[0] = 0;
    memset(obj->text, 0, sizeof(obj->text));
    src->tdf->GetFieldString(obj->text, "text", 0x80, DAT_005119b8);
    strncpy(obj->text, Translate(obj->text), 0x7f);
    src->tdf->GetFieldString(obj->link, "link", 0x10, DAT_005119b8);
}
