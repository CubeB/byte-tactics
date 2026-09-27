// Decompiled by Opus. Names are provisional.
// Constructor of a Class_0044ce20 subclass that reads a 16-byte header from
// a named entry of an open file and keeps its last three dwords.

class Class_004b4ba0 {
public:
    int FUN_004b4ba0(char* name);
};

class Class_004b4c10 {
public:
    void FUN_004b4c10(int pos);
};

class Class_004b4c80 {
public:
    int FUN_004b4c80(void* buf, int size);
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

    Class_0044d010(int owner, Class_004b4ba0* file, char* name);
};

// FUNCTION: 0x44d010
Class_0044d010::Class_0044d010(int owner, Class_004b4ba0* file, char* name)
    : Class_0044ce20(owner)
{
    vtable = DAT_004fd328;
    int bad = 0;
    if (owner == 0 || file == 0)
        bad = 1;
    if (name == 0)
        bad = 1;
    if (!bad) {
        file->FUN_004b4ba0(name);
        ((Class_004b4c10*)file)->FUN_004b4c10(0);
        Header_0044d010 hdr;
        if (((Class_004b4c80*)file)->FUN_004b4c80(&hdr, 16) == 16) {
            v.x = hdr.v.x;
            v.y = hdr.v.y;
            v.z = hdr.v.z;
        }
    }
}
