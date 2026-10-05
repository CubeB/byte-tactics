// Decompiled by Opus. Names are provisional.
// Saving counterpart of Class_0044d010's constructor: writes a 16-byte
// header whose last three dwords are the stored values (the first dword is
// left uninitialised, as in the original).

class HapiBank {
public:
    int OpenNamedBox(char* name);
    void SeekBox(int pos);
    int WriteBox(void* src, int len);
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

    int FUN_0044d090(int unused, HapiBank* file, char* name);
};

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
