// Decompiled by space-bunny-free. Names are provisional.
// Sibling of 0x474df0 and 0x474880 (same reserve block, same one-trip
// count-down loop, same out-of-line vector insert), but with 0x3c-byte
// records: it reserves room for the frames up to field_4, appends one record
// holding the three positions at +0x20, +0x2c and +0x38, a pointer out of the
// game structure and field_4 itself, then pushes the clock forward.
//
// STILL DIFFERS (best 49.5%, check.py):
// 1. The record's stack slot. The original builds it at L0x04, right on top of
//    the two reserve spills (the count at L0x00, the new buffer at L0x04 which
//    lands in the record's never-written first dword), so the frame is 0x44.
//    Every shape written here puts MSVC's record at L0x08 with the new buffer
//    spill in L0x04, i.e. 4 bytes too high, even though the frame size matches
//    when the record is 0x3c. Tried: the record declared before/after `extra`
//    and inside/outside the loop, in its own block, in an inline method, as a
//    struct holding the count, as 0x40 bytes with the last field in the same
//    object (that one makes the frame 0x48). The 0x3c size is forced by the
//    signed /0x3c in reserve and by the 0xf-dword copy in 0x475bd0.
// 2. Because of (1), the store of field_4 has nowhere to go: at +0x3c it would
//    be past a 0x3c record, so it is written to a separate local here and
//    MSVC drops the store as dead (472 vs 473 bytes, 9 bytes short).
// 3. Register allocation. The original keeps g_game in ecx from the top
//    (one load, reused for the frame read and the two g_game reads later),
//    where this file loads g_game into eax and the frame into edx.
//
// One further attempt from the session supervisor, recorded so it is not
// repeated: hoisting `Record rec;` to function scope before the reserve, and
// writing the field_4 slot as an explicit one-past-the-end store
// (`*(int*)((char*)&rec + 0x3c) = field_4;`) to stop MSVC dropping it as dead.
// That is 41.7 percent and the frame grows to 0x48, because a record live across
// the reserve call stops sharing the frame the way the loop-local one does. The
// explicit cast store also does not rescue the store, so the dropped store is
// not simply a consequence of the offset.
//    (`mov ecx, [g_game]` before the pushes) and reloads it at the loop's back
//    edge, uses edi for the count and esi/ecx for the divide; this file hoists
//    g_game->frame into edx before `sub esp` and ends up with the count in esi
//    and the new buffer in edi. The loop body then loads the three positions
//    through ecx/eax instead of eax/edx.
#include <stdlib.h>
#include <vector>

struct Vec3_004743a0 {
    int x;
    int y;
    int z;
};

// What the vector holds: 0x3c bytes, the size the division in reserve and the
// copy loop in 0x475bd0 both use.
struct Record_004743a0 {
    void* image;                                  // +0x00, never set
    unsigned short* bitmask;                      // +0x04
    Vec3_004743a0 pos0;                           // +0x08
    Vec3_004743a0 pos1;                           // +0x14
    Vec3_004743a0 pos2;                           // +0x20, .z at +0x28
    int field_2c;                                 // +0x2c
    int field_30;                                 // +0x30, never set
    int field_34;                                 // +0x34
    int field_38;                                 // +0x38
};

#pragma pack(push, 1)
struct Game_004743a0 {
    char unknown_0[0x147f3];
    unsigned short* unknown_147f3;                 // +0x147f3
    char unknown_147f7[0x38a47 - 0x147f7];
    int frame;                                    // +0x38a47
};
#pragma pack(pop)

extern Game_004743a0* g_game;

int __stdcall FUN_004b7f60(unsigned short* p);

typedef std::vector<Record_004743a0> Vec_004743a0;

// 0x475bd0 is this vector's insert left out of line by the original; declaring
// it as a method of its own keeps the call out of line here too.
class Class_00475bd0 {
public:
    void* FUN_00475bd0(Record_004743a0* at, unsigned int n, const Record_004743a0& x);
};

class Class_004743a0 {
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
void Class_004743a0::FUN_004743a0()
{
    int extra = field_4 - g_game->frame + 1;
    if (extra > 0) {
        records.reserve(extra + records.size());
    }
    Vec3_004743a0* p = &pos0;
    Vec_004743a0* v = &records;
    for (int i = 1; i != 0; i--) {
        Record_004743a0 rec;
        int tail;                                  // the original's store of
        rec.field_34 = 0;                          //   field_4 at +0x3c, one
        rec.field_38 = unknown_1c;                 //   dword past this record
        tail = field_4;
        rec.pos0 = *p;
        rec.pos1 = pos1;
        rec.pos2 = pos2;
        rec.bitmask = g_game->unknown_147f3;
        ((Class_00475bd0*)v)->FUN_00475bd0(v->end(), 1, rec);
        rec.pos2.z = (int)FUN_004b7f60(g_game->unknown_147f3) - 1;
        rec.field_2c = 0;
    }
    time = g_game->frame + 1;
}
