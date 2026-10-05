// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Slot of ThrustParticles (same 0x3c-byte record family as 0x474df0 and
// 0x4751c0): reserves room for the frames between g_game->frame and field_4,
// then appends one record holding the three positions at +0x20/+0x2c/+0x38,
// a pointer out of the game structure and field_4, and pushes the clock to
// g_game->frame + 1.
//
// The record has no leading image pointer: bitmask sits at +0x00, so the three
// positions land at +0x04/+0x10/+0x1c and field_4 is the record's last dword
// at +0x38, inside the 0x3c bytes. The earlier attempt's phantom `image` at
// +0x00 shifted every store up by 4 and made the compiler drop field_4 as a
// separate dead local.
#include <stdlib.h>
#include <vector>

struct Vec3_004743a0 {
    int x;
    int y;
    int z;
};

struct Record_004743a0 {
    unsigned short* bitmask;                      // +0x00
    Vec3_004743a0 pos0;                           // +0x04
    Vec3_004743a0 pos1;                           // +0x10
    Vec3_004743a0 pos2;                           // +0x1c
    int field_28;                                 // +0x28
    int field_2c;                                 // +0x2c
    int field_30;                                 // +0x30
    int field_34;                                 // +0x34
    int field_38;                                 // +0x38
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x147f3];
    unsigned short* unknown_147f3;                 // +0x147f3
    char unknown_147f7[0x38a47 - 0x147f7];
    int frame;                                    // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall GetGafFrameCount(unsigned short* p);

typedef std::vector<Record_004743a0> Vec_004743a0;

// 0x475bd0 is this vector's insert left out of line by the original; declaring
// it as a method of its own keeps the call out of line here too.
class Class_00475bd0 {
public:
    void* FUN_00475bd0(Record_004743a0* at, unsigned int n, const Record_004743a0& x);
};

class ThrustParticles {
public:
    virtual void FUN_004743a0();                  // slot 0
    int field_4;                                  // +0x04
    int time;                                     // +0x08
    Vec_004743a0 records;                         // +0x0c (_First +0x10)
    int unknown_1c;                               // +0x1c
    Vec3_004743a0 pos0;                           // +0x20
    Vec3_004743a0 pos1;                           // +0x2c
    Vec3_004743a0 pos2;                           // +0x38
};

// The two pointer locals are only there to get the address of the first
// position and the address of the vector into ebp and esi, in that order,
// before the loop.
// FUNCTION: 0x4743a0
void ThrustParticles::FUN_004743a0()
{
    int extra = field_4 - g_game->frame + 1;
    if (extra > 0) {
        records.reserve(extra + records.size());
    }
    Vec3_004743a0* p = &pos0;
    Vec_004743a0* v = &records;
    for (int i = 1; i != 0; i--) {
        Record_004743a0 rec;
        rec.field_34 = unknown_1c;
        rec.field_38 = field_4;
        rec.field_30 = 0;
        rec.pos0 = *p;
        rec.pos1 = pos1;
        rec.pos2 = pos2;
        rec.bitmask = g_game->unknown_147f3;
        rec.field_28 = (int)GetGafFrameCount(g_game->unknown_147f3) - 1;
        rec.field_2c = 0;
        ((Class_00475bd0*)v)->FUN_00475bd0(v->end(), 1, rec);
    }
    time = g_game->frame + 1;
}
