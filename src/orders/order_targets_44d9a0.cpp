// Decompiled by Opus. Names are provisional.
// Saving counterpart of Class_0044d930's constructor: writes a 20-byte
// header whose last four dwords are the stored values (the first dword is
// left uninitialised, as in the original), like 0x44d090.

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4cf0 {
public:
    int FUN_004b4cf0(void* src, int len);
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

    int FUN_0044d9a0(int unused, Class_004b4ba0* file, char* name);
};

// FUNCTION: 0x44d9a0
int Class_0044d930::FUN_0044d9a0(int unused, Class_004b4ba0* file, char* name)
{
    Header_0044d930 hdr;
    hdr.r.a = r.a;
    hdr.r.b = r.b;
    hdr.r.c = r.c;
    hdr.r.d = r.d;
    file->FUN_004b4ba0(name);
    ((Class_004b4c10*)file)->FUN_004b4c10(0);
    ((Class_004b4cf0*)file)->FUN_004b4cf0(&hdr, 20);
    return 1;
}
