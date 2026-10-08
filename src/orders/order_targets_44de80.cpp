// Decompiled by Haiku, Sonnet, Opus, Space Bunny Free, deepseek-v4.1-flash, DeepSeek V4.1 Flash, Claude Opus 5.5 and GPT-6. Names are provisional.
// The order-target area classes, part 2: the Class_0044ce20 family's area
// loaders, constructors and hit tests, the Class_0044e740, Class_0044ea60 and
// Class_0044eb40 path-target classes, the Class_0044ef20 family with its
// Class_0044f010 path, the Pathfinder singleton, and the out-of-line
// std::vector<Point_0044eec0> members.
// 0x44ec30 (vector::insert) stays in order_targets_44ec30.cpp: it only
// matches compiled with /Gi, and /Gi moves 0x44e3c0's registers.
#include "../util/hapi_bank.h"
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <vector>

struct Vec3_0044de80 {
    int x;
    int y;
    int z;
};

struct Vec3_0044e3c0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x21c];
    short field_21c;                   // +0x21c
    char unknown_21e[0x241 - 0x21e];
    union {
        unsigned int flags;            // +0x241
        struct {
            unsigned int unused : 22;
            // Must be a 1-bit bitfield, not folded into a mask test.
            unsigned int flag22 : 1;   // bit 22 of the flags
        };
    };
};
#pragma pack(pop)

struct Sub_0044e190 {
    char unknown_0[0xdc];
    int field_dc;                      // +0xdc
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x10];
    Sub_0044e190* field_10;            // +0x10
    char unknown_14[0x66 - 0x14];
    short heading;                     // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3_0044e3c0 pos;                 // +0x6a
    char unknown_76[0x82 - 0x76];
    int field_82;                      // +0x82
    char unknown_86[0x92 - 0x86];
    UnitDef* def;                      // +0x92
    char unknown_96[0xa8 - 0x96];
    short id;                          // +0xa8
    char unknown_aa[0x118 - 0xaa];
};
#pragma pack(pop)

class Class_004895c0;

#pragma pack(push, 2)
struct Owner_004895c0 {
    char unknown_0[0x92];
    UnitDef* def;                      // +0x92
    char unknown_96[0xa2 - 0x96];
    Class_004895c0* head;              // +0xa2
    short flag;                        // +0xa6
};
#pragma pack(pop)

class Class_004895c0 {
public:
    Owner_004895c0* owner;             // +0x4
    Class_004895c0* next;              // +0x8
    int value;                         // +0xc

    Class_004895c0(Owner_004895c0* o = 0, int v = 0);
    virtual ~Class_004895c0();
    void SetUnit(Owner_004895c0* o);
};

class UnitRef {
public:
    void FUN_00489650();
};

extern void __cdecl operator delete(void*);
extern void* DAT_004fd2f8[];
extern void* DAT_004fd3b8[];
extern void* DAT_004fd3f8[];

#pragma pack(push, 2)
struct Rec_0044de80 {
    int unknown_0;                     // +0x0
    int unknown_4;                     // +0x4
    short id1;                         // +0x8
    void* ref_vt;                      // +0xa
    void* ref_owner;                   // +0xe
    void* ref_next;                    // +0x12
    int ref_value;                     // +0x16
    short id2;                         // +0x1a
    short f1;                          // +0x1c
    short f2;                          // +0x1e
    short f3;                          // +0x20
    short f4;                          // +0x22
    short f5;                          // +0x24
    Vec3_0044de80 pos;                 // +0x26
    int i4;                            // +0x32
};

class Class_0044ce20 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4

    Class_0044ce20() {}
    Class_0044ce20(int param_1)
    {
        vtable = DAT_004fd2f8;
        field_4 = param_1;
    }
};

class Class_0044de80 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    Unit* field_12;                    // +0x12
    Class_004895c0 ref;                // +0x16
    Vec3_0044de80 pos;                 // +0x26
    int field_32;                      // +0x32

    Class_0044de80(int owner, HapiBank* file, char* name);
};
#pragma pack(pop)

class Class_0044df80 {
public:
    void** vtable;
    char unknown_4[0x12];

    void* FUN_0044df80(unsigned char flag);
};

// Reads bit fields from an array of dwords, lowest bits first.
// Bit reader, see src/network/net_stats.cpp.
class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int ReadBits(int bits);

    int ReadBit()
    {
        int r = (data[index] & (1 << bit)) != 0;
        if (++bit == 32) {
            bit = 0;
            index++;
        }
        return r;
    }
};

struct Owner_0044e080;
class Pathfinder;

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14207];
    Pathfinder* field_14207;           // +0x14207
    char unknown_1420b[0x1427f - 0x1420b];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280[0x142b7 - 0x14280];
    int field_142b7;                   // +0x142b7
    char unknown_142bb[0x14357 - 0x142bb];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x38a47 - 0x1435b];
    unsigned int field_38a47;          // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class Pathfinder {
public:
    char unknown_0[0xc9];

    Pathfinder();
    ~Pathfinder();
    void AbortIfGoalMatch(void* param);
};

#pragma pack(push, 2)
class Class_0044e080 : public Class_0044ce20 {
public:
    unsigned short flags;              // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    Owner_0044e080* owner;             // +0x12
    Class_004895c0 ref;                // +0x16
    int pos_x;                         // +0x26
    int pos_y;                         // +0x2a
    int pos_z;                         // +0x2e

    Class_0044e080(Owner_0044e080* owner_, BitReader* reader);
};

struct Order {
    char unknown_0[0xe];
    Unit* unit;                        // +0xe
};

class Class_0044e190 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    Unit* field_12;                    // +0x12
    Class_004895c0 ref;                // +0x16
    Vec3_0044e3c0 pos;                 // +0x26
    int field_32;                      // +0x32

    Class_0044e190(Order* order, Unit* unit);
};

struct Source_0044e250 {
    char unknown_0[0xe];
    Unit* unit;                        // +0xe
};

class Class_0044e250 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    Unit* field_12;                    // +0x12
    Class_004895c0 ref;                // +0x16
    Vec3_0044e3c0 pos;                 // +0x26

    Class_0044e250(Source_0044e250* source, int unit, short value);
};

struct Source_0044e2d0 {
    char unknown_0[0xe];
    int field_e;                       // +0xe
};

struct Vec3_0044e2d0 {
    int x;
    int y;
    int z;
};

class Class_0044e2d0 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    int field_12;                      // +0x12
    Class_004895c0 ref;                // +0x16
    Vec3_0044e2d0 pos;                 // +0x26

    Class_0044e2d0(Source_0044e2d0* source, const Vec3_0044e2d0& p);
};

struct Source_0044e330 {
    char unknown_0[0xe];
    int field_e;                       // +0xe
};

struct Vec3_0044e330 {
    int x;
    int y;
    int z;
};

class Class_0044e330 : public Class_0044ce20 {
public:
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    short field_e;                     // +0xe
    short field_10;                    // +0x10
    int field_12;                      // +0x12
    Class_004895c0 ref;                // +0x16
    Vec3_0044e330 pos;                 // +0x26

    Class_0044e330(Source_0044e330* source, int unit, const Vec3_0044e330& p);
};
#pragma pack(pop)

#pragma pack(push, 1)
class Class_0044e3a0 {
public:
    char unknown_0[8];
    unsigned char field_8;
    char unknown_9[17];
    int field_1a;

    int FUN_0044e3a0();
};

class Class_0044e3c0 {
public:
    char unknown_0[8];
    unsigned short flags;                  // +0x8
    short field_a;                         // +0xa
    short field_c;                         // +0xc
    short field_e;                         // +0xe
    short field_10;                        // +0x10
    Unit* unit;                            // +0x12
    char unknown_16[4];                    // +0x16
    Unit* target;                          // +0x1a
    char unknown_1e[8];                    // +0x1e
    Vec3_0044e3c0 pos;                     // +0x26
    int field_32;                          // +0x32

    int FUN_0044e3c0(Vec3_0044e3c0* out);
};

struct Vec3_0044e530 {
    int x, y, z;
};

struct Object_0044e530 {
    char unknown_0[0x66];
    unsigned short heading;            // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3_0044e530 pos;                 // +0x6a
};

class Class_0044e530 {
public:
    char unknown_0[8];
    unsigned short flags;              // +0x08
    char unknown_a[0xe - 0xa];
    unsigned short heading;            // +0x0e
    char unknown_10[0x12 - 0x10];
    Object_0044e530* self;             // +0x12
    char unknown_16[0x1a - 0x16];
    Object_0044e530* target;           // +0x1a
    int FUN_0044e530(unsigned short* out);
};

struct Vec3_0044e5b0 {
    int x, y, z;
};

struct Object_0044e5b0 {
    char unknown_0[0x66];
    unsigned short heading;            // +0x66
    char unknown_68[2];
    Vec3_0044e5b0 pos;                 // +0x6a
};

class Class_0044e5b0 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8(Vec3_0044e5b0* out);   // vtable +0x20
    char unknown_4[4];                 // +0x4
    unsigned short flags;              // +0x8
    short field_a;                     // +0xa
    char unknown_c[0x1a - 0xc];        // +0xc
    Object_0044e5b0* target;           // +0x1a

    int FUN_0044e5b0(Object_0044e5b0* arg);
};
#pragma pack(pop)

struct Pos_0044e6c0 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

#pragma pack(push, 2)
class Class_0044e6c0 {
public:
    char unknown_0[8];
    unsigned short field_8;            // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    char unknown_e[0x26 - 0xe];
    Pos_0044e6c0 pos;                  // +0x26

    void FUN_0044e6c0(int param_1);
};
#pragma pack(pop)

struct Class_0044e720 {
    char unknown_0[8];
    unsigned short unknown_bits : 6;   // +0x8
    unsigned short flag : 1;           // +0x8 bit 6
    unsigned short unknown_rest : 9;
    char unknown_a[4];
    short value;                       // +0xe

    void FUN_0044e720(short v);
};

struct Class_0044e730 {
    char unknown_0[8];
    unsigned short unknown_bits : 4;   // +0x8
    unsigned short flag : 1;           // +0x8 bit 4
    unsigned short unknown_rest : 11;
    short value;                       // +0xa

    void FUN_0044e730(short v);
};

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

class BitWriter {
public:
    int bit;                           // +0x0 current word index
    int index;                         // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10
    void GrowBuffer();
    void WriteBits(int value, int bits);
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

class Class_0044e7b0 {
public:
    void* vtable;

    void* FUN_0044e7b0(int param_1);
};

struct Owner_0044e9c0;

#pragma pack(push, 2)
class Class_0044e9c0 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4
    unsigned short flags;              // +0x8
    int field_a;                       // +0xa
    int field_e;                       // +0xe
    int field_12;                      // +0x12
    int field_16;                      // +0x16
    int field_1a;                      // +0x1a
    int field_1e;                      // +0x1e
    short field_22;                    // +0x22
    unsigned short field_24;           // +0x24
    short pad_26;                      // +0x26
    Owner_0044e9c0* owner;             // +0x28

    Class_0044e9c0(Owner_0044e9c0* owner, BitReader* reader);
};

struct Vec3_0044ea60 {
    int x;
    int y;
    int z;

    Vec3_0044ea60() {}
    Vec3_0044ea60(int ax, int ay, int az)
    {
        x = ax;
        y = ay;
        z = az;
    }
};

#pragma pack(push, 1)
struct UnitType_0044ea60 {
    char unknown_0[0x1ba];
    unsigned short max_turn;           // +0x1ba
};

struct Object_0044ea60 {
    char unknown_0[0x92];
    UnitType_0044ea60* type;           // +0x92
};
#pragma pack(pop)

#pragma pack(push, 2)
class Class_0044ea60 {
public:
    char unknown_0[8];
    short field_8;                     // +0x8
    Vec3_0044ea60 target;              // +0xa
    Vec3_0044ea60 other;               // +0x16
    short field_22;                    // +0x22
    unsigned short heading;            // +0x24
    char unknown_26[2];
    Object_0044ea60* self;             // +0x28

    int FUN_0044ea60(Vec3_0044ea60* out);
};
#pragma pack(pop)

struct Vec3_0044eb40 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Object_0044eb40 {
    char unknown_0[0x6a];
    Vec3_0044eb40 pos;                 // +0x6a
};

class Class_0044eb40 {
public:
    char unknown_0[0xa];
    Vec3_0044eb40 target;              // +0x0a
    char unknown_16[0x28 - 0x16];
    Object_0044eb40* self;             // +0x28
    int FUN_0044eb40(unsigned short* out);
};
#pragma pack(pop)

struct Class_0044ec20 {
    char unknown_0[8];
    unsigned short flag : 1;           // +0x8 bit 0
    unsigned short unknown_rest : 15;
    char unknown_a[0x24 - 0xa];
    short value;                       // +0x24

    void FUN_0044ec20(short v);
};

// The element as 0x44da00.cpp declares it, so the insert has the name that
// caller already uses.
struct Point_0044eec0 {
    short x;
    short y;
};

// The same vector as 0x44ee90 (_Ucopy) and 0x44eec0 (_Ufill): 0x44d0e0,
// 0x44d560 and 0x44da00 call 0x44ee90 and then this with ecx set to it.
typedef std::vector<Point_0044eec0> Vec_0044ee60;
typedef void (Vec_0044ee60::*DestroyFn_0044ee60)(Vec_0044ee60::iterator, Vec_0044ee60::iterator);

// _Destroy is protected: a derived class takes its address so it is emitted out of line.
struct Access_0044ee60 : Vec_0044ee60 {
    static DestroyFn_0044ee60 fn;
};

typedef std::vector<Point_0044eec0> Vec_0044ee70;
typedef Vec_0044ee70::size_type (Vec_0044ee70::*SizeFn_0044ee70)() const;

typedef std::vector<Point_0044eec0> Vec_0044ee90;
typedef Vec_0044ee90::iterator (Vec_0044ee90::*UcopyFn_0044ee90)(
    Vec_0044ee90::const_iterator, Vec_0044ee90::const_iterator, Vec_0044ee90::iterator);

// _Ucopy is protected: a derived class takes its address so it is emitted out of line.
struct Access_0044ee90 : Vec_0044ee90 {
    static UcopyFn_0044ee90 fn;
};

typedef std::vector<Point_0044eec0> Vec_0044eec0;
typedef void (Vec_0044eec0::*UfillFn_0044eec0)(
    Vec_0044eec0::iterator, Vec_0044eec0::size_type, const Point_0044eec0&);

// _Ufill is protected: a derived class takes its address so it is emitted out of line.
struct Access_0044eec0 : Vec_0044eec0 {
    static UfillFn_0044eec0 fn;
};

struct Point_0044eef0 {
    short x;
    short y;
};

struct Target_0044f1a0 {
    char unknown_0[0x2e];
    unsigned char field_2e;            // +0x2e
};

struct Target_0044f2a0 {
    char unknown_0[0x42];
    unsigned int field_42;             // +0x42
};

// A 16.16 fixed-point coordinate: the low word is the fraction, the high
// word the signed integer part the path points store.
union Coord_0044f2a0 {
    int fixed;
    short half[2];
};

struct Vec3_004907e0 {
    Coord_0044f2a0 x;
    Coord_0044f2a0 y;
    Coord_0044f2a0 z;
};

#pragma pack(push, 2)
struct Struct_004907e0 {               // the owner
    Target_0044f1a0* target;           // +0x0
    char unknown_4[0x5c - 0x4];
    Target_0044f2a0* field_5c;         // +0x5c
    char unknown_60[0x6a - 0x60];
    Vec3_004907e0 pos;                 // +0x6a
};
#pragma pack(pop)

// The object at +0x4 (see victory_490940.cpp); vtable 0x4fd2f8. Slot 8 is the
// "give me your position as a Vec3" call (an implementation is 0x44dc60); slot
// 8 of the base holds _purecall, slot 11 the stub FUN_0044cef0, which returns 0.
class Base_00490a10 {
public:
    virtual ~Base_00490a10();                           // slot 0
    virtual void FUN_0044ce80();                        // slot 1
    virtual void FUN_0044ce40();                        // slot 2
    virtual void FUN_0044cf30();                        // slot 3
    virtual int FUN_0044cf00(Struct_004907e0* owner);   // slot 4
    virtual int FUN_0044cf20(int x, int y);             // slot 5
    virtual void FUN_0044ce90();                        // slot 6
    virtual void FUN_0044cec0();                        // slot 7
    virtual int FUN_004e6110(Vec3_004907e0* out);       // slot 8
    virtual int FUN_0044cf40();                         // slot 9
    virtual void FUN_0044cf50();                        // slot 10
    virtual int FUN_0044cef0();                         // slot 11
};

class Class_0044ced0 {
public:
    void FUN_0044ced0(int);
};

class Class_0044f010;

// Vtable 0x4fd428, constructor 0x44ef20, ??_G 0x44ef60.
class Class_0044ef20 {
public:
    Base_00490a10* field_4;            // +0x4
    Struct_004907e0* owner;            // +0x8

    Class_0044ef20(Struct_004907e0* p);
    virtual ~Class_0044ef20() {}                    // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1
    virtual void FUN_0044efb0();                    // slot 2
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3
    virtual void FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*);  // slot 4
    virtual int FUN_0044ef80();                     // slot 5
    virtual Class_0044f010* FUN_0044eff0();         // slot 6
    virtual int FUN_0044efe0();                     // slot 7
    virtual void FUN_0044efc0(BitWriter*);          // slot 8
    virtual void FUN_0044efd0(BitReader*);          // slot 9
    virtual void FUN_0044ef50(void*);               // slot 10
};

struct Point_0044f080 {
    short x;
    short y;
};

// Vtable 0x4fd458, constructor 0x44f010, destructor 0x44f450, ??_G 0x44f040.
// Slots 4 and 9 are inherited.
class Class_0044f010 : public Class_0044ef20 {
public:
    Point_0044f080 points[20];         // +0xc
    int count;                         // +0x5c
    unsigned int field_60;             // +0x60
    union {
        struct {
            unsigned char active : 1;  // +0x64 bit 0
            unsigned char flag_1 : 1;  // bit 1
            // flag_2 and flag_3 stay 1-bit bitfields: an int mode store gives and/or.
            unsigned char flag_2 : 1;  // bit 2
            unsigned char flag_3 : 1;  // bit 3, the path changed
        };
        unsigned char field_64;
    };

    Class_0044f010(Struct_004907e0* p);
    virtual ~Class_0044f010();                      // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1, 0x44f2a0
    virtual void FUN_0044efb0();                    // slot 2, 0x44f1a0
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3, 0x44f150
    virtual int FUN_0044ef80();                     // slot 5, 0x44f290
    virtual Class_0044f010* FUN_0044eff0();         // slot 6, 0x44f260
    virtual int FUN_0044efe0();                     // slot 7, 0x44f480
    virtual void FUN_0044efc0(BitWriter*);          // slot 8, 0x44f4a0
    // In console_commands_417e00.cpp: it was compiled with the console code,
    // far from the rest of the class.
    virtual void FUN_0044ef50(void*);               // slot 10, 0x417e00
    void FUN_0044f080(Point_0044f080* src, int n);
    void FUN_0044f100(int n);
};

// Vtable 0x4fd488, constructor 0x44f570, ??_G 0x44f590.
class Class_0044f570 : public Class_0044ef20 {
public:
    char unknown_c[0x18 - 0xc];
    int field_18;                      // +0x18

    Class_0044f570(Struct_004907e0* p);
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3, 0x44f650
    virtual int FUN_0044ef80();                     // slot 5, 0x44f5b0
    virtual void FUN_0044efd0(BitReader*);          // slot 9, 0x44f5c0
};

class Class_0044f5b0 {
public:
    char unknown_0[0x18];
    int field_18;

    int FUN_0044f5b0();
};

struct Target_0044f5c0 {
    char unknown_0[0x2e];
    unsigned char flag_0 : 1;          // +0x2e bit 0
    unsigned char flag_1 : 1;          // +0x2e bit 1
    unsigned char flag_2 : 1;          // +0x2e bit 2
    unsigned char flag_rest : 5;
};

struct Owner_0044f5c0 {
    Target_0044f5c0* target;           // +0x0
};

struct Point_0044f5c0 {
    short x;                           // +0x0
    short y;                           // +0x2
};

class Class_0044f5c0 {
public:
    void* vtable;                      // +0x0
    int field_4;                       // +0x4
    Owner_0044f5c0* owner;             // +0x8
    Point_0044f5c0 points[3];          // +0xc
    int count;                         // +0x18

    void FUN_0044f5c0(BitReader* reader);
};

struct Point_0044f650 {
    short x;                           // +0x0
    short y;                           // +0x2
};

struct Vec3_0044f650 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

class Class_0044f650 {
public:
    char unknown_0[0xc];
    Point_0044f650 points[3];          // +0xc
    int count;                         // +0x18

    void FUN_0044f650(Vec3_0044f650* out, int unused, int n);
};

Unit* __stdcall LoadUnit(unsigned short index, HapiBank* file);
unsigned short __stdcall GetHeadingBetween(void* from, void* to);
Vec3_0044e3c0 __stdcall GetPiecePosition(Unit* obj, int param);
int __stdcall GetGroundHeight(Pos_0044e6c0* pos);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
void __cdecl FUN_004b7173(short angle, int* xy);

// Loading counterpart of the virtual save method 0x44dfb0 (vtable
// DAT_004fd3b8, slot 1): reads a 0x36-byte record from a named entry of an
// open file and copies its fields into this object. The record it reads is
// built by 0x44dfb0 and by 0x44e330 / 0x44e250.
// FUNCTION: 0x44de80
Class_0044de80::Class_0044de80(int owner, HapiBank* file, char* name)
    : Class_0044ce20(owner), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    Rec_0044de80 rec;
    ((Class_004895c0*)&rec.ref_vt)->Class_004895c0::Class_004895c0(0, 0);
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    if (((HapiBank*)file)->ReadBox(&rec, 0x36) == 0x36) {
        field_12 = LoadUnit(rec.id1, file);
        ref.SetUnit((Owner_004895c0*)LoadUnit(rec.id2, file));
        field_8 = rec.f1;
        field_a = rec.f2;
        field_c = rec.f3;
        field_e = rec.f4;
        field_10 = rec.f5;
        pos = rec.pos;
        field_32 = rec.i4;
    }
    ((UnitRef*)&rec.ref_vt)->FUN_00489650();
}

// FUNCTION: 0x44df70
int FUN_0044df70(void)
{
    return 2;
}

// Scalar-deleting-destructor shape: unlink the embedded list node at +0x16
// (via UnitRef::FUN_00489650, the same unlink method used elsewhere),
// restore this object's own vtable, conditionally operator delete, and
// return `this`.
// FUNCTION: 0x44df80
void* Class_0044df80::FUN_0044df80(unsigned char flag)
{
    ((UnitRef*)((char*)this + 0x16))->FUN_00489650();
    vtable = DAT_004fd2f8;
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}

struct Unit_0044dfb0 {
    char unknown_0[0xa8];
    short id;                       // +0xa8
};

struct Vec3_0044dfb0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 2)
struct Rec_0044dfb0 {
    int unknown_0;                  // +0x0
    int unknown_4;                  // +0x4
    short id1;                      // +0x8
    void* ref_vt;                   // +0xa (Class_004895c0)
    void* ref_owner;                // +0xe
    void* ref_next;                 // +0x12
    int ref_value;                  // +0x16
    short id2;                      // +0x1a
    short f1;                       // +0x1c
    short f2;                       // +0x1e
    short f3;                       // +0x20
    short f4;                       // +0x22
    short f5;                       // +0x24
    Vec3_0044dfb0 pos;              // +0x26
    int i4;                         // +0x32
};

class Class_0044dfb0 {
public:
    void* vtable;                   // +0x0
    void* field_4;                  // +0x4
    short f8;                       // +0x8
    short fa;                       // +0xa
    short fc;                       // +0xc
    short fe;                       // +0xe
    short f10;                      // +0x10
    Unit_0044dfb0* unit1;           // +0x12
    void* ref_vt;                   // +0x16 (Class_004895c0)
    Unit_0044dfb0* owner;           // +0x1a
    void* ref_next;                 // +0x1e
    int ref_value;                  // +0x22
    Vec3_0044dfb0 pos;              // +0x26
    int i4;                         // +0x32

    int FUN_0044dfb0(int unused, HapiBank* file, char* name);
};
#pragma pack(pop)

// Virtual save method (vtable 0x4fd3b8, slot 1) for the class built by
// 0x44e330 / 0x44e250. Builds a 0x36-byte record on the stack, writing the
// referenced unit's id, the embedded link's owner id, five flag shorts and
// the stored position, then writes it to the file via
// HapiBank::WriteBox. The read counterpart is 0x44de80.
//
// The record's first 8 bytes are never assigned and are still written out:
// 0x36 bytes of stack, 8 of them uninitialised, go to the save file. The read
// counterpart (0x44de80) reads all 0x36 bytes but never looks at 0..7, so the
// bytes are only leaked, never used.
// FUNCTION: 0x44dfb0
int Class_0044dfb0::FUN_0044dfb0(int unused, HapiBank* file, char* name)
{
    // The first 8 bytes of rec stay unassigned, as in the original.
    Rec_0044dfb0 rec;
    Class_004895c0* ref = (Class_004895c0*)&rec.ref_vt;
    ref->Class_004895c0::Class_004895c0(0, 0);
    if (unit1 == 0)
        rec.id1 = 0;
    else
        rec.id1 = unit1->id;
    if (owner == 0)
        rec.id2 = 0;
    else
        rec.id2 = owner->id;
    rec.f1 = f8;
    rec.f2 = fa;
    rec.f3 = fc;
    rec.f4 = fe;
    rec.f5 = f10;
    rec.pos = pos;
    rec.i4 = i4;
    file->OpenNamedBox(name);
    ((HapiBank*)file)->SeekBox(0);
    ((HapiBank*)file)->WriteBox(&rec, 0x36);
    ((UnitRef*)&rec.ref_vt)->FUN_00489650();
    return 1;
}

// FUNCTION: 0x44e080
Class_0044e080::Class_0044e080(Owner_0044e080* owner_, BitReader* reader)
    : Class_0044ce20(0), owner(owner_), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    flags = reader->ReadBits(8);
    if (flags & 1) {
        field_10 = reader->ReadBits(0x10);
        unsigned short index = reader->ReadBits(0x10);
        ref.SetUnit(index == 0 ? 0 : (Owner_004895c0*)&g_game->units[index]);
    }
    if (flags & 0x10)
        field_a = reader->ReadBits(0x10);
    else
        field_a = 0;
    if (flags & 8)
        field_c = reader->ReadBits(0x10);
    else
        field_c = 0;
    if (flags & 0x40)
        field_e = reader->ReadBits(0x10);
    else
        field_e = 0;
    if (flags & 0x20) {
        pos_x = reader->ReadBits(0x20);
        pos_y = reader->ReadBits(0x20);
        pos_z = reader->ReadBits(0x20);
    }
}

// Another constructor of the class built by 0x44e250 / 0x44e2d0 / 0x44e330
// (vtable DAT_004fd3b8): it copies the position from the order's unit and
// picks a type of 7 or 1 from the unit definition's flag bit 11.
// FUNCTION: 0x44e190
Class_0044e190::Class_0044e190(Order* order, Unit* unit)
    : Class_0044ce20((int)order), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    ref.SetUnit((Owner_004895c0*)unit);
    field_a = 0;
    field_c = 0;
    field_12 = order->unit;
    pos = field_12->pos;
    field_10 = -1;
    if ((unsigned char)(ref.owner->def->flags >> 11) & 1) {
        field_8 = 7;
        int t = field_12->field_10->field_dc;
        if (t != 0)
            field_32 = t << 16;
        else
            field_32 = 0x640000;
    } else {
        field_8 = 1;
    }
}

// A second constructor for the class built by 0x44e330 (same vtable
// DAT_004fd3b8): it takes its position from the source's unit instead.
// FUNCTION: 0x44e250
Class_0044e250::Class_0044e250(Source_0044e250* source, int unit, short value)
    : Class_0044ce20((int)source), field_10(value), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    field_a = 0;
    field_c = 0;
    field_8 = 5;
    field_12 = source->unit;
    pos = field_12->pos;
    ref.SetUnit((Owner_004895c0*)unit);
}

// Another constructor of the class built by 0x44e330 and 0x44e250 (vtable
// DAT_004fd3b8), with a type of 0x20 and the position passed in.
// FUNCTION: 0x44e2d0
Class_0044e2d0::Class_0044e2d0(Source_0044e2d0* source, const Vec3_0044e2d0& p)
    : Class_0044ce20((int)source), ref(0, 0), pos(p)
{
    vtable = DAT_004fd3b8;
    field_a = 0;
    field_c = 0;
    field_10 = -1;
    field_8 = 0x20;
    field_12 = source->field_e;
}

// FUNCTION: 0x44e330
Class_0044e330::Class_0044e330(Source_0044e330* source, int unit, const Vec3_0044e330& p)
    : Class_0044ce20((int)source), ref(0, 0)
{
    vtable = DAT_004fd3b8;
    ref.SetUnit((Owner_004895c0*)unit);
    pos = p;
    field_a = 0;
    field_c = 0;
    field_10 = -1;
    field_12 = source->field_e;
    field_8 = 0xa3;
}

// FUNCTION: 0x44e3a0
int Class_0044e3a0::FUN_0044e3a0() {
    if ((field_8 & 1) == 0 || field_1a == 0) {
        return 0;
    }
    return 1;
}

// Virtual method (DAT_004fd3b8 slot) of the Class_0044ce20 family: works out
// where the object should be. When the flags say it is active and not fully
// set, it aims at the reference unit's predicted position, optionally adding
// a direction offset from the fixed-point trig helpers; otherwise it just
// places itself at terrain level using the unit definition's height.
static inline Vec3_0044e3c0 Direction(short angle, int scale)
{
    Vec3_0044e3c0 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}

// FUNCTION: 0x44e3c0
int Class_0044e3c0::FUN_0044e3c0(Vec3_0044e3c0* out)
{
    unsigned short f = flags;
    if ((f & 1) && !(f & 0x80)) {
        if (target == 0 || target->field_82 == g_game->field_142b7)
            return 0;
        // Through a local pointer: keeps the struct-assignment destination in edi.
        Vec3_0044e3c0* p = &pos;
        *p = GetPiecePosition(target, field_10);
        if (flags & 2) {
            short angle = target->heading;
            if (flags & 0x40)
                angle += field_e;
            Vec3_0044e3c0 d = Direction(angle, field_32);
            p->x -= d.x;
            p->y -= d.y;
            p->z -= d.z;
        }
        pos.y += field_c << 16;
    } else {
        if ((f & 8) == 0) {
            UnitDef* def = unit->def;
            if (def->flag22)
                pos.y = (def->field_21c + g_game->seaLevel) << 16;
            else
                pos.y = (def->field_21c + *(unsigned char*)(unit->field_82 + 1)) << 16;
        }
    }
    if (pos.y > 0x1ff0000)
        pos.y = 0x1ff0000;
    *out = pos;
    return 1;
}

// FUNCTION: 0x44e530
int Class_0044e530::FUN_0044e530(unsigned short* out)
{
    unsigned short f = flags;
    if ((f & 0x84) && target != 0) {
        if (f & 2) {
            *out = GetHeadingBetween(&self->pos, &target->pos);
            return 1;
        }
        if (f & 0x40) {
            *out = heading;
            return 1;
        }
        *out = target->heading;
        return 1;
    }
    if (f & 0x40) {
        *out = heading;
        return 1;
    }
    return 0;
}

// A virtual method of the big class whose vtable starts at 0x4fd2f8 (it shares
// that vtable with 0x44e530, 0x44e3a0 and friends). It fetches a position from
// itself through vtable slot +0x20, measures the horizontal (x/z) distance to
// `arg`, converts it from 16.16 to pixels, then tests range/flags.
// FUNCTION: 0x44e5b0
int Class_0044e5b0::FUN_0044e5b0(Object_0044e5b0* arg)
{
    Vec3_0044e5b0 pos;
    v8(&pos);
    float dist = (float)_hypot((double)(arg->pos.x - pos.x), (double)(arg->pos.z - pos.z)) * 1.52587890625e-05f;
    unsigned short flags = this->flags;
    if (flags & 0x10) {
        return (double)field_a > dist;
    }
    else {
        if (dist > 0.5f)
            return 0;
        if ((flags & 1) && target == 0)
            return 0;
        if ((flags & 4) && arg->heading != target->heading)
            return 0;
        if ((flags & 8) && abs(arg->pos.y - pos.y) > 0x10000)
            return 0;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x44e6b0
int FUN_0044e6b0(void)
{
    return 0;
}

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x44e6c0
void Class_0044e6c0::FUN_0044e6c0(int param_1)
{
    field_8 |= 8;
    field_c = param_1;
    if ((unsigned char)field_8 & 0x20) {
        pos.y = (max(GetGroundHeight(&pos), g_game->seaLevel) + param_1) << 16;
        if (pos.y > 0x1ff0000)
            pos.y = 0x1ff0000;
    }
}

// Sets a flag bit in the word at +0x8 (a 1-bit unsigned short bitfield, which
// MSVC sets with `or byte ptr` straight to memory) and stores a short.
// FUNCTION: 0x44e720
void Class_0044e720::FUN_0044e720(short v)
{
    flag = 1;
    value = v;
}

// Sets a flag bit in the word at +0x8 (a 1-bit unsigned short bitfield, which
// MSVC sets with `or byte ptr` straight to memory) and stores a short.
// FUNCTION: 0x44e730
void Class_0044e730::FUN_0044e730(short v)
{
    flag = 1;
    value = v;
}

// The constructor.
// FUNCTION: 0x44e740 ??0Class_0044e740@@QAE@PAUSource_0044e740@@ABUVec3_0044e740@@1@Z
Class_0044e740::Class_0044e740(Source_0044e740* source, const Vec3_0044e740& a, const Vec3_0044e740& b)
    : Class_0044ce20((int)source)
{
    vtable = DAT_004fd3f8;
    self = source->unit;
    field_8 = 0;
    field_22 = 0;
    target = a;
    other = b;
}

// FUNCTION: 0x44e7a0
int FUN_0044e7a0(void)
{
    return 3;
}

// FUNCTION: 0x44e7b0
void* Class_0044e7b0::FUN_0044e7b0(int param_1)
{
    vtable = &DAT_004fd2f8;
    if ((param_1 & 1) != 0) {
        operator delete(this);
    }
    return this;
}

// The load constructor: reads a 0x2a-byte
// header from a named entry of an open file, then looks up the unit it names.
// FUNCTION: 0x44e7d0 ??0Class_0044e740@@QAE@HPAVHapiBank@@PAD@Z
Class_0044e740::Class_0044e740(int owner, HapiBank* file, char* name)
{
    field_4 = owner;
    vtable = DAT_004fd3f8;
    file->OpenNamedBox(name);
    file->SeekBox(0);
    Header_0044e740 hdr;
    if (file->ReadBox(&hdr, 0x2a) == 0x2a) {
        self = (Object_0044e740*)LoadUnit(hdr.id, file);
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
// writing it as a word is harmless.
// FUNCTION: 0x44e880
int Class_0044e740::FUN_0044e880(int unused, HapiBank* file, char* name)
{
    Header_0044e740 hdr;
    // if/else, not a ternary: puts the store of 0 on the fallthrough path.
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
    file->SeekBox(0);
    file->WriteBox(&hdr, 0x2a);
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

// FUNCTION: 0x44e9c0
Class_0044e9c0::Class_0044e9c0(Owner_0044e9c0* owner, BitReader* reader)
{
    field_4 = 0;
    this->owner = owner;
    vtable = DAT_004fd3f8;
    flags = reader->ReadBits(1);
    field_a = reader->ReadBits(0x20);
    field_e = reader->ReadBits(0x20);
    field_12 = reader->ReadBits(0x20);
    field_16 = reader->ReadBits(0x20);
    field_1a = reader->ReadBits(0x20);
    field_1e = reader->ReadBits(0x20);
    if (flags & 1) {
        field_24 = reader->ReadBits(0x10);
    }
}

// FUNCTION: 0x44ea50
int FUN_0044ea50(void)
{
    return 0;
}

// Slot 8 of Class_0044e740 (vtable 0x4fd3f8, constructor 0x44e740), the slot
// before 0x44eb40: copies the first point out, advances it by the second point
// and, when flag bit 0 is set, turns the second point (a step per frame) towards
// the stored heading, by at most an eighth of the unit type's turn rate.
// The rotation helper takes a pair of ints, so the (x, z) step is built in the
// local's x and y fields and read back from y into other.z.
// FUNCTION: 0x44ea60
int Class_0044ea60::FUN_0044ea60(Vec3_0044ea60* out)
{
    *out = target;
    target.x += other.x;
    target.z += other.z;
    Vec3_0044ea60 v;
    v = Vec3_0044ea60(0, 0, 0);
    unsigned short h = GetHeadingBetween(&v, &other);
    if ((field_8 & 1) && h != heading) {
        short diff = heading - h;
        v.x = other.x;
        v.y = other.z;
        short step = diff;
        unsigned short max = self->type->max_turn;
        if (step >= (max >> 3))
            step = max >> 3;
        else if (step <= -(max >> 3))
            step = -(max >> 3);
        FUN_004b7173(-step, (int*)&v);
        other.x = v.x;
        other.z = v.y;
        other.y = 0;
    }
    return 1;
}

// Virtual slot 9 (vtable at 0x4fd3f8), the same slot as 0x44e530: writes the
// heading from the owner's position (+0x6a) to the target point at +0xa.
// FUNCTION: 0x44eb40
int Class_0044eb40::FUN_0044eb40(unsigned short* out)
{
    *out = GetHeadingBetween(&self->pos, &target);
    return 1;
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

// FUNCTION: 0x44ec00
int FUN_0044ec00(void)
{
    return 0;
}

// An empty method: its one caller (0x412d40) calls it on
// the object it has just built with that class's constructor (0x44e740).
// FUNCTION: 0x44ec10
void Class_0044e740::FUN_0044ec10(int)
{
}

// Sets a flag bit in the word at +0x8 (a 1-bit unsigned short bitfield, which
// MSVC sets with `or byte ptr` straight to memory) and stores a short.
// FUNCTION: 0x44ec20
void Class_0044ec20::FUN_0044ec20(short v)
{
    flag = 1;
    value = v;
}

// std::vector<Point_0044eec0>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial.
// FUNCTION: 0x44ee60 ?_Destroy@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@IAEXPAUPoint_0044eec0@@0@Z
DestroyFn_0044ee60 Access_0044ee60::fn = &Access_0044ee60::_Destroy;

// std::vector<Point_0044eec0>::size() for the 4-byte point (two shorts); its
// callers are the vector insert paths in 0x44d0e0 and 0x44d560.
// FUNCTION: 0x44ee70 ?size@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@QBEIXZ
SizeFn_0044ee70 g_size_0044ee70 = &Vec_0044ee70::size;

// std::vector<Point_0044eec0>::_Ucopy(first, last, dest) from MSVC 5's
// <vector>: copy-constructs [first, last) into raw storage at dest and
// returns the end of the copies. Its callers (around 0x44d190) set ecx to the
// vector whose _Ufill is 0x44eec0.
// FUNCTION: 0x44ee90 ?_Ucopy@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@IAEPAUPoint_0044eec0@@PBU3@0PAU3@@Z
UcopyFn_0044ee90 Access_0044ee90::fn = &Access_0044ee90::_Ucopy;

// std::vector<Point_0044eec0>::_Ufill(first, n, value) from MSVC 5's <vector>:
// copy-constructs n copies of value into raw storage. Its callers (around
// 0x44d190) set ecx to the vector; its _Ucopy is 0x44ee90 and its copy is
// 0x44eef0 (4-byte elements, two shorts).
// FUNCTION: 0x44eec0 ?_Ufill@?$vector@UPoint_0044eec0@@V?$allocator@UPoint_0044eec0@@@std@@@std@@IAEXPAUPoint_0044eec0@@IABU3@@Z
UfillFn_0044eec0 Access_0044eec0::fn = &Access_0044eec0::_Ufill;

// std::copy for 4-byte elements (two shorts), compiled with __stdcall as the
// default convention. Its one caller (0x44da00) is an inlined
// vector::erase(begin(), end()) whose _Destroy is 0x44ee60. Not a member of
// the Class_0044ef20 family next to it: it takes no `this`.
// FUNCTION: 0x44eef0
Point_0044eef0* __stdcall FUN_0044eef0(Point_0044eef0* first, Point_0044eef0* last, Point_0044eef0* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}

// The base of a class family (vtable 0x4fd428, 11 slots; constructor
// 0x44ef20, ??_G 0x44ef60). Derived classes, all with this class's
// declaration copied verbatim:
//   Class_0044f010  vtable 0x4fd458  ctor 0x44f010  dtor 0x44f450  ??_G 0x44f040
//   Class_0044f570  vtable 0x4fd488  ctor 0x44f570  ??_G 0x44f590
//   Class_00490630  vtable 0x4fd980  ctor 0x4905e0  ??_G 0x490630
//     Class_004907e0  vtable 0x4fd9b0  ctor 0x4907e0  ??_G 0x490840
//     Class_00490880  vtable 0x4fd9e0  ctor 0x490940  dtor 0x4909e0  ??_G 0x4909a0

// The constructor.
// FUNCTION: 0x44ef20
// FUNCTION: 0x44ef60 ??_GClass_0044ef20@@UAEPAXI@Z
Class_0044ef20::Class_0044ef20(Struct_004907e0* p)
{
    owner = p;
    field_4 = 0;
}

// Slot 3: does nothing.
// FUNCTION: 0x44ef40
void Class_0044ef20::FUN_0044ef40(Vec3_004907e0*, int, int)
{
}

// Slot 10: does nothing.
// FUNCTION: 0x44ef50
void Class_0044ef20::FUN_0044ef50(void*)
{
}

// Slot 5: whether there is an object at +0x4.
// FUNCTION: 0x44ef80
int Class_0044ef20::FUN_0044ef80()
{
    return field_4 != 0;
}

// Slot 1: sets the object at +0x4, first telling the one it replaces 0x80.
// FUNCTION: 0x44ef90
void Class_0044ef20::FUN_0044ef90(void* param)
{
    if (field_4 != 0) {
        ((Class_0044ced0*)field_4)->FUN_0044ced0(0x80);
    }
    field_4 = (Base_00490a10*)param;
}

// Slot 2: does nothing.
// FUNCTION: 0x44efb0
void Class_0044ef20::FUN_0044efb0()
{
}

// Slot 8: does nothing.
// FUNCTION: 0x44efc0
void Class_0044ef20::FUN_0044efc0(BitWriter*)
{
}

// Slot 9: does nothing.
// FUNCTION: 0x44efd0
void Class_0044ef20::FUN_0044efd0(BitReader*)
{
}

// Slot 7: 0.
// FUNCTION: 0x44efe0
int Class_0044ef20::FUN_0044efe0()
{
    return 0;
}

// Slot 6: none.
// FUNCTION: 0x44eff0
Class_0044f010* Class_0044ef20::FUN_0044eff0()
{
    return 0;
}

// Slot 4: does nothing.
// FUNCTION: 0x44f000
void Class_0044ef20::FUN_0044f000(Vec3_004907e0*, Vec3_004907e0*, short*)
{
}

// The constructor.
// FUNCTION: 0x44f010
Class_0044f010::Class_0044f010(Struct_004907e0* p)
    : Class_0044ef20(p)
{
    count = 0;
    active = 0;
    flag_1 = 0;
    flag_3 = 1;
    field_60 = 0;
}

// Sets the path points (at most 20) and marks it active, or with no
// points asks the object at +0x4 about the owner and flags it (0x40) when that
// fails.
// FUNCTION: 0x44f080
void Class_0044f010::FUN_0044f080(Point_0044f080* src, int n)
{
    if (n == 0) {
        if (field_4 && field_4->FUN_0044cf00(owner) == 0)
            ((Class_0044ced0*)field_4)->FUN_0044ced0(0x40);
        active = 0;
    } else {
        if (n >= 20)
            n = 20;
        count = n;
        std::copy(src, src + n, points);
        active = 1;
    }
    flag_1 = 0;
    flag_3 = 1;
}

// Drops the first n path points, clears the active flag when fewer
// than two are left and sets flag 3.
// FUNCTION: 0x44f100
void Class_0044f010::FUN_0044f100(int n)
{
    if (n != 0) {
        std::copy(points + n, points + count, points);
        count -= n;
        if (count < 2)
            active = 0;
        flag_3 = 1;
    }
}

// Slot 3: fills n positions from the path points, repeating the last point past the
// end of the path.
// FUNCTION: 0x44f150
void Class_0044f010::FUN_0044ef40(Vec3_004907e0* out, int unused, int n)
{
    for (int i = 0; i < n; i++) {
        int j = i < count ? i : count - 1;
        out[i].x.fixed = points[j].x << 16;
        out[i].y.fixed = 0;
        out[i].z.fixed = points[j].y << 16;
    }
}

// Slot 2: tells the object at +0x4 about the owner, then, when the unit has reached
// its second path point (within 5 units of the owner), drops that point with
// an overlapping std::copy and refreshes the flags.
// FUNCTION: 0x44f1a0
void Class_0044f010::FUN_0044efb0()
{
    if (field_4) {
        if (field_4->FUN_0044cf00(owner)) {
            ((Class_0044ced0*)field_4)->FUN_0044ced0(0x20);
            if (!field_4->FUN_0044cef0())
                FUN_0044ef90(0);
        }
    }
    if (count >= 2) {
        int dx = owner->pos.z.half[1] - points[1].y;
        int dy = owner->pos.x.half[1] - points[1].x;
        if (dy * dy + dx * dx <= 25) {
            // std::copy, not memmove: memmove stays a library call.
            std::copy(points + 1, points + count, points);
            int n = --count;
            if (n < 2)
                active = 0;
            flag_3 = 1;
        }
    }
    if (field_4 && (owner->target->field_2e & 4 || count < 2))
        flag_1 = 1;
}

// Slot 6: returns this object when its flag bit 1 is set and the counter at
// g_game+0x38a47 has reached field_60 + 0x3c (storing the counter in
// field_60), or null. The path search scheduler (0x40eb70) calls it through
// slot 6 to pick the path to search for next.
// FUNCTION: 0x44f260
Class_0044f010* Class_0044f010::FUN_0044eff0()
{
    if (field_64 & 2) {
        unsigned int limit = g_game->field_38a47;
        if (limit >= field_60 + 0x3c) {
            field_60 = limit;
            return this;
        }
    }
    return 0;
}

// Slot 5: the active flag (bit 0 of +0x64).
// FUNCTION: 0x44f290
int Class_0044f010::FUN_0044ef80()
{
    return field_64 & 1;
}

// Slot 1: hands the object at +0x4 over to the
// path logic, then, unless the path is already active, steps the path
// forwards: it asks the object for its position (its slot 8) and marks the
// path active when the next path point is at most half as far from that
// object as the owner is. With no usable point left it resets the path to
// two points taken from the owner and the object's position.
// FUNCTION: 0x44f2a0
void Class_0044f010::FUN_0044ef90(void* param)
{
    g_game->field_14207->AbortIfGoalMatch(this);
    if (field_4)
        ((Class_0044ced0*)field_4)->FUN_0044ced0(0x80);
    active = 0;
    field_4 = (Base_00490a10*)param;
    if (param == 0) {
        flag_1 = 0;
    } else {
        flag_1 = 1;
        if (count >= 3) {
            if (field_4->FUN_0044cf20(points[count - 1].x >> 4, points[count - 1].y >> 4)) {
                active = 1;
                flag_1 = 0;
            }
        }
        if (!active) {
            Vec3_004907e0 p;
            if (field_4->FUN_004e6110(&p)) {
                if (count >= 3) {
                    int sx = points[count - 1].x << 16;
                    int sz = points[count - 1].y << 16;
                    int d1 = (int)_hypot(owner->pos.x.fixed - p.x.fixed,
                                         owner->pos.z.fixed - p.z.fixed);
                    int d2 = (int)_hypot(sx - p.x.fixed,
                                         sz - p.z.fixed);
                    if (d2 * 2 < d1)
                        active = 1;
                }
                if (!active) {
                    Target_0044f2a0* t = owner->field_5c;
                    if (t && !(t->field_42 & 0x800000)) {
                        count = 2;
                        points[0].x = owner->pos.x.half[1];
                        points[0].y = owner->pos.z.half[1];
                        points[1].x = p.x.half[1];
                        points[1].y = p.z.half[1];
                        active = 1;
                    }
                }
            }
        }
    }
    if (field_60 <= g_game->field_38a47 - 10)
        field_60 = 0;
    flag_3 = 1;
}

// The out-of-line destructor: it stores its own vtable, unregisters the object, then the empty inline base destructor
// stores 0x4fd428.
// FUNCTION: 0x44f040 ??_GClass_0044f010@@UAEPAXI@Z
// FUNCTION: 0x44f450
Class_0044f010::~Class_0044f010()
{
    g_game->field_14207->AbortIfGoalMatch(this);
}

// Slot 7: true when there is something to send: flag 3 of +0x64 (the path changed) is
// set, or flag 2 no longer matches bit 2 of the owner's target. Slot 8
// (0x44f4a0) writes the path out and brings both flags up to date.
// FUNCTION: 0x44f480
int Class_0044f010::FUN_0044efe0()
{
    return (field_64 & 8) || ((owner->target->field_2e ^ field_64) & 4);
}

// Slot 8, the write counterpart of the reader 0x44f5c0. It sets the stream's next bit when the
// unit's mode asks for it, writes the point count in 2 bits, then up to three
// points of 16 bits each, and finally copies the unit's mode into flag 2 while
// clearing flag 3 (the "changed" flag the readers rely on).
// FUNCTION: 0x44f4a0
void Class_0044f010::FUN_0044efc0(BitWriter* stream)
{
    int n;
    if (active) {
        n = count < 3 ? count : 3;
    } else {
        n = 0;
    }
    if (owner->target->field_2e & 4) {
        stream->data[stream->bit] |= 1 << stream->index;
    }
    stream->index++;
    if (stream->index == 0x20) {
        stream->index = 0;
        stream->bit++;
        if (stream->bit == stream->capacity) {
            stream->GrowBuffer();
        }
        stream->data[stream->bit] = 0;
    }
    stream->WriteBits(n, 2);
    for (int i = 0; i < n; i++) {
        stream->WriteBits(points[i].x, 0x10);
        stream->WriteBits(points[i].y, 0x10);
    }
    flag_2 = (owner->target->field_2e & 4) != 0;
    flag_3 = 0;
}

// The constructor, with the base constructor inlined. Its vtable reference
// makes the compiler emit the scalar deleting destructor here too: the
// destructor is trivial, so the dead store of this class's vtable disappears
// and only the inlined base destructor's store of 0x4fd428 is left.
// FUNCTION: 0x44f570
// FUNCTION: 0x44f590 ??_GClass_0044f570@@UAEPAXI@Z
Class_0044f570::Class_0044f570(Struct_004907e0* p)
    : Class_0044ef20(p)
{
    field_18 = 0;
}

// FUNCTION: 0x44f5b0
int Class_0044f5b0::FUN_0044f5b0()
{
    return field_18 >= 2;
}

// FUNCTION: 0x44f5c0
void Class_0044f5c0::FUN_0044f5c0(BitReader* reader)
{
    owner->target->flag_2 = reader->ReadBit();
    count = reader->ReadBits(2);
    for (int i = 0; i < count; i++) {
        points[i].x = reader->ReadBits(16);
        points[i].y = reader->ReadBits(16);
    }
}

// Converts up to `count` short 2D points into 16.16 fixed-point 3D vectors
// (x, 0, y), repeating the last point once the list runs out.
// FUNCTION: 0x44f650
void Class_0044f650::FUN_0044f650(Vec3_0044f650* out, int unused, int n)
{
    for (int i = 0; i < n; i++) {
        int j = i < count ? i : count - 1;
        out[i].x = points[j].x << 16;
        out[i].y = 0;
        out[i].z = points[j].y << 16;
    }
}

// Creates the object at g_game+0x14207 (deleted again by 0x44f6e0).
// FUNCTION: 0x44f6a0
void FUN_0044f6a0()
{
    g_game->field_14207 = new Pathfinder;
}

// Deletes the object at g_game+0x14207 and clears the pointer.
// FUNCTION: 0x44f6e0
void FUN_0044f6e0()
{
    delete g_game->field_14207;
    g_game->field_14207 = 0;
}
