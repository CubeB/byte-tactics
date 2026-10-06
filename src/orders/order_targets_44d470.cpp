// Decompiled by Opus and DeepSeek V4.1 Flash. Names are provisional.

#include "../util/hapi_bank.h"

struct Data_0044d470 {
    int a;
    int b;
    int c;
    int d;
    int e;
};

struct Header_0044d470 {
    int magic;                         // +0x0
    Data_0044d470 data;                // +0x4
};

// The vtables are stored by hand, as in the other Class_0044ce20 subclasses.
extern void* DAT_004fd2f8[];
extern void* DAT_004fd358[];

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

class Class_0044d470 : public Class_0044ce20 {
public:
    Data_0044d470 data;                // +0x8

    int FUN_0044d500(int unused, HapiBank* file, char* name);
    Class_0044d470(int owner, HapiBank* file, char* name);
};

// Constructor of a Class_0044ce20 subclass that reads a 24-byte header from
// a named entry of an open file and keeps its last five dwords.
// FUNCTION: 0x44d470
Class_0044d470::Class_0044d470(int owner, HapiBank* file, char* name)
    : Class_0044ce20(owner)
{
    vtable = DAT_004fd358;
    int bad = 0;
    if (owner == 0 || file == 0)
        bad = 1;
    if (name == 0)
        bad = 1;
    if (!bad) {
        file->OpenNamedBox(name);
        ((HapiBank*)file)->SeekBox(0);
        Header_0044d470 hdr;
        if (((HapiBank*)file)->ReadBox(&hdr, 24) == 24) {
            data.a = hdr.data.a;
            data.b = hdr.data.b;
            data.c = hdr.data.c;
            data.d = hdr.data.d;
            data.e = hdr.data.e;
        }
    }
}

// Saving counterpart of Class_0044d470's constructor: writes a 24-byte header
// whose last five dwords are the stored values. The first dword is left
// uninitialised, exactly as in 0x44d090 and 0x44d9a0.
// FUNCTION: 0x44d500
int Class_0044d470::FUN_0044d500(int unused, HapiBank* file, char* name)
{
    Header_0044d470 hdr;
    hdr.data.a = data.a;
    hdr.data.b = data.b;
    hdr.data.c = data.c;
    hdr.data.d = data.d;
    hdr.data.e = data.e;
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->WriteBox(&hdr, 24);
    return 1;
}
