// Decompiled by Sonnet and Opus. Names are provisional.
// The out-of-line destructor of Class_0044f010 (vtable 0x4fd458), derived
// from Class_0044ef20 (see 0x44ef60.cpp for the family). It stores its own
// vtable, unregisters the object, then the empty inline base destructor
// stores 0x4fd428. Its scalar deleting destructor 0x44f040 inlines it.

class Pathfinder {
public:
    void FUN_0040e9c0(void* param);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14207];
    Pathfinder* field_14207;            // +0x14207
};
#pragma pack(pop)

extern Game* g_game;

class Base_00490a10 {                  // the object at +0x4 (see 0x490a10.cpp)
public:
    virtual ~Base_00490a10();
};

struct Struct_004907e0;                // the owner (see 0x4907e0.cpp)

struct Vec3_004907e0;                  // a position (see 0x4907e0.cpp)
class Class_0044f010;                  // slot 6's result (below)
class BitWriter;                       // the bit writer slot 8 takes
class BitReader;                       // the bit reader slot 9 takes

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

// Vtable 0x4fd458, constructor 0x44f010, destructor 0x44f450, ??_G 0x44f040.
// Slots 4 and 9 are inherited.
class Class_0044f010 : public Class_0044ef20 {
public:
    Class_0044f010(Struct_004907e0* p);
    virtual ~Class_0044f010();                      // slot 0
    virtual void FUN_0044ef90(void* param);         // slot 1, 0x44f2a0
    virtual void FUN_0044efb0();                    // slot 2, 0x44f1a0
    virtual void FUN_0044ef40(Vec3_004907e0*, int, int);  // slot 3, 0x44f150
    virtual int FUN_0044ef80();                     // slot 5, 0x44f290
    virtual Class_0044f010* FUN_0044eff0();         // slot 6, 0x44f260
    virtual int FUN_0044efe0();                     // slot 7, 0x44f480
    virtual void FUN_0044efc0(BitWriter*);          // slot 8, 0x44f4a0
    virtual void FUN_0044ef50(void*);               // slot 10, 0x417e00
};

// FUNCTION: 0x44f040 ??_GClass_0044f010@@UAEPAXI@Z
// FUNCTION: 0x44f450
Class_0044f010::~Class_0044f010()
{
    g_game->field_14207->FUN_0040e9c0(this);
}
