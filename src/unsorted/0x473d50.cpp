// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Slot 4 of Class_00471560 (vtable 0x4fd5b8, family in 0x471cc0.cpp): reserves
// room for the five particles a beat, then per particle picks a random point on
// the segment at +0x1c and one on the segment at +0x34, divides the difference
// by the segment length so the particle travels about 4 units a tick, and
// appends the record to the std::vector<Elem_00475880> at +0xc.
//
// Closest shape found: 79.2% (1035 bytes vs the original 983). The record
// layout, the six rand()*value/0x8000 samples, the Len()/sqrt, the 16.16
// quotient union (see 0x4736e0.cpp) and the whole inlined insert body all
// match, including the four _Ucopy and two _Ufill calls and the three insert
// boughs. What still differs:
//   - the reallocate arm of insert inlines size() where the original calls
//     0x475840 three times (three `call FUN_00475840` in the original);
//   - the delta section assigns target.z to edx and pos_x/tgt_y to ecx/edi
//     where the original uses ecx and ebx/ecx, so several instructions are
//     permuted (same instructions, different registers);
//   - the loop counter i and the step union swap their stack slots
//     ([esp+0x14]/[esp+0x10] instead of [esp+0x10]/[esp+0x14]).
// The reserve call must stay out of line (the original calls 0x475770); the
// local member-function pointer below folds to a direct call and stops MSVC
// inlining reserve, which is otherwise inlined (63.8%).
#include <math.h>
#include <stdlib.h>
#include <vector>

extern char* g_game;                   // 0x511de8, frame at +0x38a47

struct Vec3_00473d50 {
    int x;
    int y;
    int z;

    Vec3_00473d50 operator-(const Vec3_00473d50& o) const
    {
        Vec3_00473d50 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    int Length() const
    {
        float fx = x;
        float fy = y;
        float fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
};

// 16.16 fixed point seen as the short above the short below.
union Fix_00473d50 {
    int whole;
    short half[2];
};

struct Elem_00475880 {
    Vec3_00473d50 pos;                 // +0x00
    Vec3_00473d50 tgt;                 // +0x0c
    Vec3_00473d50 vel;                 // +0x18
    int field_24;                      // +0x24
    int flags;                         // +0x28
    int endTime;                       // +0x2c
};

class Class_00471560 {
public:
    char unknown_0[4];                          // +0x00
    int field_4;                                // +0x04
    int time;                                   // +0x08
    std::vector<Elem_00475880> records;         // +0x0c
    Vec3_00473d50 center;                       // +0x1c
    Vec3_00473d50 radius;                       // +0x28
    Vec3_00473d50 target;                       // +0x34
    Vec3_00473d50 spread;                       // +0x40

    void FUN_00473d50();
};

// FUNCTION: 0x473d50
void Class_00471560::FUN_00473d50()
{
    int grow = field_4 - *(int*)(g_game + 0x38a47) + 1;
    if (grow > 0) {
        void (std::vector<Elem_00475880>::*pf)(std::vector<Elem_00475880>::size_type)
            = &std::vector<Elem_00475880>::reserve;
        (records.*pf)(records.size() + grow * 5);
    }

    int i = 0;
    for (int n = 5; n != 0; n--) {
        Elem_00475880 e;
        e.pos.x = (int)(((__int64)rand() * radius.x) / 0x8000) + center.x;
        e.pos.y = (int)(((__int64)rand() * radius.y) / 0x8000) + center.y;
        e.pos.z = (int)(((__int64)rand() * radius.z) / 0x8000) + center.z;
        e.tgt.x = (int)(((__int64)rand() * spread.x) / 0x8000) + target.x;
        e.tgt.y = (int)(((__int64)rand() * spread.y) / 0x8000) + target.y;
        e.tgt.z = (int)(((__int64)rand() * spread.z) / 0x8000) + target.z;

        Vec3_00473d50 d = e.tgt - e.pos;
        e.vel = d;
        int len = d.Length();
        Fix_00473d50 step;
        step.whole = (int)(((__int64)len << 16) / 0x40000);
        short s = step.half[1];
        if (s != 0) {
            e.vel.x = d.x / s;
            e.vel.y = d.y / s;
            e.vel.z = d.z / s;
            e.endTime = *(int*)(g_game + 0x38a47) + s;
            e.field_24 = 0x100;
            e.flags = i % 7 + 0xa1;
            records.insert(records.end(), e);
        }
        i++;
    }
    time = *(int*)(g_game + 0x38a47) + 1;
}
