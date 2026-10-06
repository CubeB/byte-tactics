// Decompiled by Opus. Names are provisional.

#include "../util/hapi_bank.h"

struct Vec3_0044d010 {
    int x;
    int y;
    int z;
};

struct Header_0044d010 {
    int magic;                         // +0x0
    Vec3_0044d010 v;                   // +0x4
};

// The vtables are stored by hand, as in the other Class_0044ce20 subclasses.
extern void* DAT_004fd2f8[];
extern void* DAT_004fd328[];

class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4

    Class_0044ce20(int param_1)
    {
        vtable = DAT_004fd2f8;
        field_4 = param_1;
    }
};

class Class_0044d010 : public Class_0044ce20 {
public:
    Vec3_0044d010 v;                   // +0x8

    int FUN_0044d090(int unused, HapiBank* file, char* name);
    Class_0044d010(int owner, HapiBank* file, char* name);
};

// Constructor of a Class_0044ce20 subclass that reads a 16-byte header from
// a named entry of an open file and keeps its last three dwords.
// FUNCTION: 0x44d010
Class_0044d010::Class_0044d010(int owner, HapiBank* file, char* name)
    : Class_0044ce20(owner)
{
    vtable = DAT_004fd328;
    int bad = 0;
    if (owner == 0 || file == 0)
        bad = 1;
    if (name == 0)
        bad = 1;
    if (!bad) {
        file->OpenNamedBox(name);
        ((HapiBank*)file)->SeekBox(0);
        Header_0044d010 hdr;
        if (((HapiBank*)file)->ReadBox(&hdr, 16) == 16) {
            v.x = hdr.v.x;
            v.y = hdr.v.y;
            v.z = hdr.v.z;
        }
    }
}

// Saving counterpart of Class_0044d010's constructor: writes a 16-byte
// header whose last three dwords are the stored values (the first dword is
// left uninitialised, as in the original).
// FUNCTION: 0x44d090
int Class_0044d010::FUN_0044d090(int unused, HapiBank* file, char* name)
{
    Header_0044d010 hdr;
    hdr.v.x = v.x;
    hdr.v.y = v.y;
    hdr.v.z = v.z;
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->WriteBox(&hdr, 16);
    return 1;
}
