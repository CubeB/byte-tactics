// Decompiled by Space Bunny Free. Names are provisional.
// Load constructor of Class_0044e740 (vtable 0x4fd3f8): reads a 0x2a-byte
// header from a named entry of an open file, then looks up the unit it names.

class Class_004b4ba0 {
public:
    int OpenNamedBox(char* name);
};

class Class_004b4c10 {
public:
    void SeekBox(int pos);
};

class Class_004b4c80 {
public:
    int ReadBox(void* buf, int size);
};

struct Unit_0044e7d0;

extern void* DAT_004fd3f8[];
extern Unit_0044e7d0* __stdcall LoadUnit(int unit, Class_004b4ba0* file);

#pragma pack(push, 2)
struct Vec3_0044e7d0 {
    int x;
    int y;
    int z;
};

// The id is read as a dword but only its low word is used (0x487080 masks with
// 0xffff); the high word is the flag the saving slot 0x44e880 writes at +0xa.
struct UnitFlag_0044e7d0 {
    short unit;                        // +0x0
    short flag;                        // +0x2
};

struct Header_0044e7d0 {              // 0x2a bytes
    char unknown_0[8];                 // not read from the file by name
    union {
        int id;                        // +0x08
        UnitFlag_0044e7d0 id_flag;     // +0x08
    };
    Vec3_0044e7d0 target;             // +0x0c
    Vec3_0044e7d0 other;              // +0x18
    short value_24;                   // +0x24
    unsigned short heading;            // +0x26
    unsigned short value_28;           // +0x28
};

class Class_0044e740 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4
    unsigned short flag;               // +0x8
    Vec3_0044e7d0 target;              // +0xa
    Vec3_0044e7d0 other;               // +0x16
    short value_22;                    // +0x22
    unsigned short heading;            // +0x24
    unsigned short value_26;           // +0x26
    Unit_0044e7d0* self;               // +0x28

    Class_0044e740(int owner, Class_004b4ba0* file, char* name);
};
#pragma pack(pop)

// FUNCTION: 0x44e7d0
Class_0044e740::Class_0044e740(int owner, Class_004b4ba0* file, char* name)
{
    field_4 = owner;
    vtable = DAT_004fd3f8;
    file->OpenNamedBox(name);
    ((Class_004b4c10*)file)->SeekBox(0);
    Header_0044e7d0 hdr;
    if (((Class_004b4c80*)file)->ReadBox(&hdr, 0x2a) == 0x2a) {
        self = LoadUnit(hdr.id, file);
        flag = hdr.id_flag.flag;
        target = hdr.target;
        other = hdr.other;
        value_22 = hdr.value_24;
        heading = hdr.heading;
        value_26 = hdr.value_28;
    }
}
