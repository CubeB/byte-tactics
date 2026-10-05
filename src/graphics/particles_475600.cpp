// Decompiled by space-bunny-free. Names are provisional.
// Slot 1 of Class_004750b0 (vtable 0x4fd638, the family is listed in
// 0x471cc0.cpp): steps every 32-byte record of the vector at +0xc with the
// body of 0x474fc0 inlined (drift by the game's per-tick counts, and when the
// countdown runs out count one more round and restart the countdown at half
// the period plus a random part of the other half). A record that has counted
// as many rounds as its limit is erased. Then the record-collection tests
// (slot 5) and redraws (slot 4) if there is anything new to show.
#include <stdlib.h>
#include <stddef.h>
#include <vector>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x14263];
    int rise;                          // +0x14263
    char unknown_14267[0x37ecc - 0x14267];
    int windX;                         // +0x37ecc
    char unknown_37ed0[4];
    int windZ;                         // +0x37ed4
};
#pragma pack(pop)

extern Game* g_game;

// Vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, ??_G 0x471cd0.
class Class_00471cc0 {
public:
    int field_4;                                        // +0x4

    Class_00471cc0();
    virtual ~Class_00471cc0();                          // slot 0
    virtual void FUN_00472d50() = 0;                    // slot 1
    virtual void FUN_00472e30(int) = 0;                 // slot 2
    virtual int FUN_00472e70() = 0;                     // slot 3
    static void* __stdcall operator new(size_t size);   // 0x471d10
    static void __stdcall operator delete(void* p);     // 0x471d50
};

struct Record_004750b0 {
    void* data;                        // +0x00
    int x;                             // +0x04
    int y;                             // +0x08
    int z;                             // +0x0c
    int limit;                         // +0x10
    int count;                         // +0x14
    int period;                        // +0x18
    int timer;                         // +0x1c
};

struct Vec3_00475150;

// Vtable 0x4fd638, constructor 0x4750b0, ??_G 0x475110; 0x34 bytes.
class Class_004750b0 : public Class_00471cc0 {
public:
    int time;                                           // +0x8
    std::vector<Record_004750b0> records;               // +0xc (_First +0x10)
    char unknown_1c[0x34 - 0x1c];

    Class_004750b0();
    virtual void FUN_00472d50();                        // slot 1, 0x475600
    virtual void FUN_00472e30(int);                     // slot 2, 0x475700
    virtual int FUN_00472e70();                         // slot 3, 0x475330
    virtual void FUN_004751c0();                        // slot 4, 0x4751c0
    virtual int FUN_004750f0();                         // slot 5, 0x4750f0
    virtual void FUN_00475150(Vec3_00475150* pos, int a, int b, int c); // slot 6, 0x475150
};

// FUNCTION: 0x475600
void Class_004750b0::FUN_00472d50()
{
    std::vector<Record_004750b0>::iterator it = records.begin();
    while (it != records.end()) {
        it->x += g_game->windX * 8;
        it->y += g_game->rise * 16;
        it->z += g_game->windZ * 8;
        if (--it->timer == 0) {
            it->count++;
            int half = it->period / 2;
            it->timer = (int)((__int64)rand() * half / 0x8000) + half;
        }
        if (it->count >= it->limit) {
            records.erase(it);
        } else {
            ++it;
        }
    }
    if (FUN_004750f0())
        FUN_004751c0();
}
