// Decompiled by space-bunny-free. Names are provisional.
// Slot 4 of Class_004750b0 (vtable 0x4fd638, the family is listed in
// 0x471cc0.cpp): appends one 32-byte record to the vector at +0xc, holding
// the effect named by g_game->unknown_147cf at this->pos, a random lifetime
// of 2 to unknown_24 - 1 periods, and a countdown of unknown_20 periods.
// The records are stepped and aged by slot 1 (0x475600) and drawn by slot 2
// (0x475700), which read the fields at the offsets named in Record_004750b0.
// The body runs once per call inside a countdown loop: MSVC 5 folds a one trip
// loop away when the body is small, and keeps it as a counter when it is not.
#include <stddef.h>
#include <stdlib.h>
#include <vector>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x147cf];
    void* unknown_147cf;               // +0x147cf
    char unknown_147d3[0x38a47 - 0x147d3];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class ParticleSystem {
public:
    int field_4;                                        // +0x4

    ParticleSystem();
    virtual ~ParticleSystem();                          // slot 0
    virtual void Update() = 0;                          // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
};

struct Vec3_00475150 {
    int x;
    int y;
    int z;
};

struct Record_004750b0 {
    void* data;                        // +0x00
    Vec3_00475150 pos;                 // +0x04
    int limit;                         // +0x10
    int count;                         // +0x14
    int period;                        // +0x18
    int timer;                         // +0x1c
};

// 0x476490: std::vector<Record_004750b0>::insert, out of line.
class Class_00476490 {
public:
    void FUN_00476490(Record_004750b0* pos, int count, const Record_004750b0* src);
};

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class Class_004750b0 : public ParticleSystem {
public:
    int time;                                           // +0x8
    std::vector<Record_004750b0> records;               // +0xc (_First +0x10)
    int unknown_1c;                                     // +0x1c
    int unknown_20;                                     // +0x20
    int unknown_24;                                     // +0x24
    Vec3_00475150 pos;                                  // +0x28

    Class_004750b0();
    virtual void Update();                              // slot 1, 0x475600
    virtual void FUN_00472e30(int);                     // slot 2, 0x475700
    virtual int FUN_00472e70();                         // slot 3, 0x475330
    virtual void Emit();                                // slot 4, 0x4751c0
    virtual int FUN_004750f0();                         // slot 5, 0x4750f0
    virtual void FUN_00475150(Vec3_00475150* pos, int a, int b, int c); // slot 6, 0x475150
};

// FUNCTION: 0x4751c0
void Class_004750b0::Emit()
{
    int missed = (field_4 - g_game->ticks + unknown_1c) / unknown_1c;
    if (missed > 0)
        records.reserve(records.size() + missed);
    // The two pointers have to be locals: the original hoists both addresses
    // into callee saved registers before the loop, and reads the record's
    // position and the vector's _Last through them.
    Vec3_00475150* p = &pos;
    std::vector<Record_004750b0>* v = &records;
    int i = 1;
    do {
        Record_004750b0 rec;
        rec.pos = *p;
        rec.period = unknown_20;
        rec.timer = unknown_20;
        rec.data = g_game->unknown_147cf;
        rec.limit = (int)((__int64)rand() * (unknown_24 - 2) / 0x8000) + 2;
        rec.count = 0;
        ((Class_00476490*)v)->FUN_00476490(v->end(), 1, &rec);
    } while (--i);
    time = g_game->ticks + unknown_1c;
}
