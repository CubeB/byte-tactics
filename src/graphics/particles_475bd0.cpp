// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by GPT-6.1-sol, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5, verified by GPT-6. Names are provisional.
// FLAGS: /Gi
// std::vector<ThrustParticle>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector> on a 0x3c-byte record, emitted out of line through a member
// pointer.
#include <vector>

struct ThrustParticle {
    int data;
    int pos0X;
    int pos0Y;
    int pos0Z;
    int pos1X;
    int pos1Y;
    int pos1Z;
    int pos2X;
    int pos2Y;
    int pos2Z;
    int frameCount;
    int frame;
    int tick;
    int period;
    int endTime;
};

typedef std::vector<ThrustParticle> Vec_00475bd0;
typedef void (Vec_00475bd0::*InsertFn_00475bd0)(
    Vec_00475bd0::iterator, Vec_00475bd0::size_type, const ThrustParticle&);

// The caller's growth step; the insert's bytes need the TU to instantiate
// reserve.
void __stdcall Grow_00475bd0(Vec_00475bd0* records, int extra)
{
    records->reserve(extra + records->size());
}

// FUNCTION: 0x475bd0 ?insert@?$vector@UThrustParticle@@V?$allocator@UThrustParticle@@@std@@@std@@QAEXPAUThrustParticle@@IABU3@@Z
InsertFn_00475bd0 g_insert_00475bd0 = &Vec_00475bd0::insert;
