// Decompiled by Opus. Names are provisional.
// Saving counterpart of Class_0044d930's constructor: writes a 20-byte
// header whose last four dwords are the stored values (the first dword is
// left uninitialised, as in the original), like 0x44d090.

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int WriteBox(void* src, int len);
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

class Class_0044d930 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4
    Rect_0044d930 r;                   // +0x8

    int FUN_0044d9a0(int unused, HapiBank* file, char* name);
};

// FUNCTION: 0x44d9a0
int Class_0044d930::FUN_0044d9a0(int unused, HapiBank* file, char* name)
{
    Header_0044d930 hdr;
    hdr.r.a = r.a;
    hdr.r.b = r.b;
    hdr.r.c = r.c;
    hdr.r.d = r.d;
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->WriteBox(&hdr, 20);
    return 1;
}
