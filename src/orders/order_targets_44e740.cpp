// Decompiled by Opus, Space Bunny Free and DeepSeek V4.1 Flash. Names are provisional.
// Class_0044e740 (vtable 0x4fd3f8, 0x2c bytes), a Class_0044ce20 subclass
// holding two points; slot 9 (0x44eb40) heads towards the first one. The
// vtables are stored by hand, as in the other Class_0044ce20 subclasses.
#include <math.h>

struct Vec3_0044e740 {
    int x;
    int y;
    int z;
    Vec3_0044e740() {}
    Vec3_0044e740(int ax, int ay, int az)
    {
        x = ax;
        y = ay;
        z = az;
    }
};

#pragma pack(push, 1)
struct Object_0044e740 {
    char unknown_0[0x6a];
    Vec3_0044e740 pos;                 // +0x6a
    char unknown_76[0xa8 - 0x76];
    unsigned short id;                 // +0xa8
};
#pragma pack(pop)

#pragma pack(push, 2)
struct Source_0044e740 {
    char unknown_0[0xe];
    Object_0044e740* unit;             // +0xe
};

// The 0x2a-byte save record. The reader takes the id as a dword but only its
// low word is used (0x487080 masks with 0xffff); the high word is the flag.
struct Header_0044e740 {
    char unknown_0[8];                 // not read or written by name
    union {
        int id;                        // +0x08
        struct {
            unsigned short unit_id;    // +0x08
            unsigned short flag;       // +0x0a
        };
    };
    Vec3_0044e740 target;              // +0x0c
    Vec3_0044e740 other;               // +0x18
    short value_24;                    // +0x24
    unsigned short heading;            // +0x26
    unsigned short value_28;           // +0x28
};
#pragma pack(pop)

#include "../util/hapi_bank.h"

class BitWriter {
public:
    void WriteBits(int value, int bits);
};

extern void* DAT_004fd2f8[];
extern void* DAT_004fd3f8[];

extern Object_0044e740* __stdcall LoadUnit(int unit, HapiBank* file);
unsigned short __stdcall GetHeadingBetween(Vec3_0044e740* from, Vec3_0044e740* to);

#pragma pack(push, 2)
class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    Source_0044e740* field_4;          // +0x4

    Class_0044ce20() {}
    Class_0044ce20(Source_0044e740* source)
    {
        vtable = DAT_004fd2f8;
        field_4 = source;
    }
};

class Class_0044e740 : public Class_0044ce20 {
public:
    unsigned short field_8;            // +0x8, bit 0: heading set
    Vec3_0044e740 target;              // +0xa
    Vec3_0044e740 other;               // +0x16
    short field_22;                    // +0x22
    unsigned short heading;            // +0x24
    unsigned short value_26;           // +0x26
    Object_0044e740* self;             // +0x28

    Class_0044e740(Source_0044e740* source, const Vec3_0044e740& a, const Vec3_0044e740& b);
    Class_0044e740(int owner, HapiBank* file, char* name);
    int FUN_0044e880(int unused, HapiBank* file, char* name);
    void FUN_0044e930(BitWriter* stream);
    int FUN_0044eb60(Object_0044e740* unit);
    void FUN_0044ec10(int);
};
#pragma pack(pop)

// The constructor.
// FUNCTION: 0x44e740 ??0Class_0044e740@@QAE@PAUSource_0044e740@@ABUVec3_0044e740@@1@Z
Class_0044e740::Class_0044e740(Source_0044e740* source, const Vec3_0044e740& a, const Vec3_0044e740& b)
    : Class_0044ce20(source)
{
    vtable = DAT_004fd3f8;
    self = source->unit;
    field_8 = 0;
    field_22 = 0;
    target = a;
    other = b;
}

// The load constructor: reads a 0x2a-byte
// header from a named entry of an open file, then looks up the unit it names.
// FUNCTION: 0x44e7d0 ??0Class_0044e740@@QAE@HPAVHapiBank@@PAD@Z
Class_0044e740::Class_0044e740(int owner, HapiBank* file, char* name)
{
    field_4 = (Source_0044e740*)owner;
    vtable = DAT_004fd3f8;
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    Header_0044e740 hdr;
    if (((HapiBank*)file)->ReadBox(&hdr, 0x2a) == 0x2a) {
        self = LoadUnit(hdr.id, file);
        field_8 = hdr.flag;
        target = hdr.target;
        other = hdr.other;
        field_22 = hdr.value_24;
        heading = hdr.heading;
        value_26 = hdr.value_28;
    }
}

// Slot 1, the saving counterpart of the
// loading constructor 0x44e7d0: packs this object's fields into a 0x2a-byte
// header (whose first eight bytes are left as they are, exactly as 0x44d500
// leaves its magic dword alone) and appends it to the file. The reader takes
// the id as a dword but only its low word (0x487080 masks with 0xffff), so
// writing it as a word is harmless. The unit id needs the "if (!self) ... else
// ..." form, not a ternary: that puts the store of 0 on the fallthrough path.
// FUNCTION: 0x44e880
int Class_0044e740::FUN_0044e880(int unused, HapiBank* file, char* name)
{
    Header_0044e740 hdr;
    if (!self) {
        hdr.unit_id = 0;
    } else {
        hdr.unit_id = self->id;
    }
    hdr.flag = field_8;
    hdr.target = target;
    hdr.other = other;
    hdr.value_24 = field_22;
    hdr.heading = heading;
    hdr.value_28 = value_26;
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->WriteBox(&hdr, 0x2a);
    return 1;
}

// Writes the flag word, the two
// points, and (when flag bit 0 is set) the word at +0x24 to a bit stream.
// FUNCTION: 0x44e930
void Class_0044e740::FUN_0044e930(BitWriter* stream)
{
    stream->WriteBits(field_8, 1);
    stream->WriteBits(target.x, 0x20);
    stream->WriteBits(target.y, 0x20);
    stream->WriteBits(target.z, 0x20);
    stream->WriteBits(other.x, 0x20);
    stream->WriteBits(other.y, 0x20);
    stream->WriteBits(other.z, 0x20);
    if ((field_8 & 1) != 0) {
        stream->WriteBits(heading, 0x10);
    }
}

// Slot 4: done
// when the unit is within 48 world units of the target point, or, when flag
// bit 0 is set, when the heading from the origin to the second point equals
// the stored heading.
// FUNCTION: 0x44eb60
int Class_0044e740::FUN_0044eb60(Object_0044e740* unit)
{
    if ((float)_hypot(unit->pos.x - target.x, unit->pos.z - target.z) / 65536.0f < 48.0f)
        return 1;
    if (field_8 & 1) {
        Vec3_0044e740 origin;
        origin = Vec3_0044e740(0, 0, 0);
        if (GetHeadingBetween(&origin, &other) == heading)
            return 1;
    }
    return 0;
}

// An empty method: its one caller (0x412d40) calls it on
// the object it has just built with that class's constructor (0x44e740).
// FUNCTION: 0x44ec10
void Class_0044e740::FUN_0044ec10(int)
{
}
