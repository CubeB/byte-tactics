// Decompiled by deepseek-v4.1-flash. Names are provisional.

#include "../util/tdf.h"


struct Source_004ad890 {
    char unknown_0[4];
    TdfRecord* tdf;                    // +0x4
};

#pragma pack(push, 1)
struct Obj_004ad890 {
    char unknown_0[0xb6];
    short field_b6;                    // +0xb6
    char unknown_b8[0x11];             // +0xb8
    char major;                        // +0xc9
    char minor;                        // +0xca
    char revision;                     // +0xcb
    char crdefault[0x10];              // +0xcc
    char escdefault[0x10];             // +0xdc
    char defaultfocus[0x10];           // +0xec
    char panel[0x10];                  // +0xfc
};
#pragma pack(pop)

extern char DAT_005119b8[];

// FUNCTION: 0x4ad890
void __stdcall ReadPanelFields(Obj_004ad890* obj, Source_004ad890* src)
{
    obj->field_b6 = (short)src->tdf->GetFieldInt("totalgadgets", 0);
    src->tdf->GetFieldString(obj->panel, "panel", 0x10, DAT_005119b8);
    src->tdf->GetFieldString(obj->crdefault, "crdefault", 0x10, DAT_005119b8);
    src->tdf->GetFieldString(obj->escdefault, "escdefault", 0x10, DAT_005119b8);
    src->tdf->GetFieldString(obj->defaultfocus, "defaultfocus", 0x10, DAT_005119b8);
    if (((TdfFile*)src)->SelectRecord("VERSION") == 1) {
        obj->major = (char)src->tdf->GetFieldInt("major", 0);
        obj->minor = (char)src->tdf->GetFieldInt("minor", 0);
        obj->revision = (char)src->tdf->GetFieldInt("revision", 0);
    }
}
