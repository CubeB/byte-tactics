// Decompiled by Opus. Names are provisional.
// Saving counterpart of Class_0044d010's constructor: writes a 16-byte
// header whose last three dwords are the stored values (the first dword is
// left uninitialised, as in the original).

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

struct Vec3_0044d010 {
    int x;
    int y;
    int z;
};

struct Header_0044d010 {
    int magic;                         // +0x0
    Vec3_0044d010 v;                   // +0x4
};

class Class_0044d010 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4
    Vec3_0044d010 v;                   // +0x8

    int FUN_0044d090(int unused, Class_004b4ba0* file, char* name);
};

// FUNCTION: 0x44d090
int Class_0044d010::FUN_0044d090(int unused, Class_004b4ba0* file, char* name)
{
    Header_0044d010 hdr;
    hdr.v.x = v.x;
    hdr.v.y = v.y;
    hdr.v.z = v.z;
    file->FUN_004b4ba0(name);
    ((Class_004b4c10*)file)->FUN_004b4c10(0);
    ((Class_004b4cf0*)file)->FUN_004b4cf0(&hdr, 16);
    return 1;
}
