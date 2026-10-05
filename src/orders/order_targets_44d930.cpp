// Decompiled by Opus. Names are provisional.
// Constructor of a Class_0044ce20 subclass that reads a 20-byte header from
// a named entry of an open file and keeps its last four dwords.

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int ReadBox(void* buf, int size);
};

struct Rect_0044d930 {
    int a;
    int b;
    int c;
    int d;
};

struct Header_0044d930 {
    int magic;                         // +0x0
    Rect_0044d930 r;                   // +0x4
};

// The vtables are stored by hand, as in the other Class_0044ce20 subclasses.
extern void* DAT_004fd2f8[];
extern void* DAT_004fd388[];

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

class Class_0044d930 : public Class_0044ce20 {
public:
    Rect_0044d930 r;                   // +0x8

    Class_0044d930(int owner, HapiBank* file, char* name);
};

// FUNCTION: 0x44d930
Class_0044d930::Class_0044d930(int owner, HapiBank* file, char* name)
    : Class_0044ce20(owner)
{
    vtable = DAT_004fd388;
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    Header_0044d930 hdr;
    if (((HapiBank*)file)->ReadBox(&hdr, 20) == 20) {
        r.a = hdr.r.a;
        r.b = hdr.r.b;
        r.c = hdr.r.c;
        r.d = hdr.r.d;
    }
}
