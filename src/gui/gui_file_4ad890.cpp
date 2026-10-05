// Decompiled by deepseek-v4.1-flash. Names are provisional.

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, unsigned size, char* def);
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

struct Source_004ad890 {
    char unknown_0[4];
    Class_004c48c0* tdf;               // +0x4
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
void __stdcall FUN_004ad890(Obj_004ad890* obj, Source_004ad890* src)
{
    obj->field_b6 = (short)((Class_004c46c0*)src->tdf)->FUN_004c46c0("totalgadgets", 0);
    src->tdf->FUN_004c48c0(obj->panel, "panel", 0x10, DAT_005119b8);
    src->tdf->FUN_004c48c0(obj->crdefault, "crdefault", 0x10, DAT_005119b8);
    src->tdf->FUN_004c48c0(obj->escdefault, "escdefault", 0x10, DAT_005119b8);
    src->tdf->FUN_004c48c0(obj->defaultfocus, "defaultfocus", 0x10, DAT_005119b8);
    if (((Class_004c3410*)src)->FUN_004c3410("VERSION") == 1) {
        obj->major = (char)((Class_004c46c0*)src->tdf)->FUN_004c46c0("major", 0);
        obj->minor = (char)((Class_004c46c0*)src->tdf)->FUN_004c46c0("minor", 0);
        obj->revision = (char)((Class_004c46c0*)src->tdf)->FUN_004c46c0("revision", 0);
    }
}
