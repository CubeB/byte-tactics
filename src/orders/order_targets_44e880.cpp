// Decompiled by Space Bunny Free. Names are provisional.
// Slot 1 of Class_0044e740 (vtable 0x4fd3f8), the saving counterpart of the
// loading constructor 0x44e7d0: packs this object's fields into a 0x2a-byte
// header (whose first eight bytes are left as they are, exactly as 0x44d500
// leaves its magic dword alone) and appends it to the file. The reader takes
// the id as a dword but only its low word (0x487080 masks with 0xffff), so
// writing it as a word is harmless. The unit id needs the "if (!self) ... else
// ..." form, not a ternary: that puts the store of 0 on the fallthrough path.

#pragma pack(push, 2)

struct Vec3_0044e880 {
    int x;
    int y;
    int z;
};

struct Object_0044e880 {
    char unknown_0[0xa8];
    unsigned short id;                 // +0xa8
};

struct Header_0044e880 {
    char unknown_0[8];                 // not written here
    unsigned short unit_id;            // +0x8
    unsigned short flag;               // +0xa
    Vec3_0044e880 target;              // +0xc
    Vec3_0044e880 other;               // +0x18
    short value_24;                    // +0x24
    unsigned short heading;            // +0x26
    unsigned short value_28;           // +0x28
};

#pragma pack(pop)

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

#pragma pack(push, 2)
class Class_0044e740 {
public:
    char unknown_0[8];
    unsigned short flag;               // +0x8
    Vec3_0044e880 target;              // +0xa
    Vec3_0044e880 other;               // +0x16
    short value_22;                    // +0x22
    unsigned short heading;            // +0x24
    unsigned short value_26;           // +0x26
    Object_0044e880* self;             // +0x28

    int FUN_0044e880(int unused, Class_004b4ba0* file, char* name);
};
#pragma pack(pop)

// FUNCTION: 0x44e880
int Class_0044e740::FUN_0044e880(int unused, Class_004b4ba0* file, char* name)
{
    Header_0044e880 hdr;
    if (!self) {
        hdr.unit_id = 0;
    } else {
        hdr.unit_id = self->id;
    }
    hdr.flag = flag;
    hdr.target = target;
    hdr.other = other;
    hdr.value_24 = value_22;
    hdr.heading = heading;
    hdr.value_28 = value_26;
    file->FUN_004b4ba0(name);
    ((Class_004b4c10*)file)->FUN_004b4c10(0);
    ((Class_004b4cf0*)file)->FUN_004b4cf0(&hdr, 0x2a);
    return 1;
}
