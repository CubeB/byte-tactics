// Decompiled by Opus. Names are provisional.
// Constructor of a Class_0044ce20 subclass that reads a 24-byte header from
// a named entry of an open file and keeps its last five dwords.

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int ReadBox(void* buf, int size);
};

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

    Class_0044d470(int owner, HapiBank* file, char* name);
};

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
