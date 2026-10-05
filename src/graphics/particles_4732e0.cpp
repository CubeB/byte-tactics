// Decompiled by GPT-6 Astra, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// Codex / GPT-6 retry, following the 2026-10-03 maintainer lead: try the real
// vector header with /Gi and an operator= instantiation before exploring other
// same-vector members.
// FLAGS: /Gi
#include <vector>

class ParticleSystem {
public:
    int field_4;
};

typedef std::vector<ParticleSystem*> Vec_004732e0;
typedef void (Vec_004732e0::*InsertFn_004732e0)(
    Vec_004732e0::iterator, Vec_004732e0::size_type, ParticleSystem* const&);

void __stdcall Assign_004732e0(Vec_004732e0* dest, const Vec_004732e0* src)
{
    *dest = *src;
}

// FUNCTION: 0x4732e0 ?insert@?$vector@PAVParticleSystem@@V?$allocator@PAVParticleSystem@@@std@@@std@@QAEXPAPAVParticleSystem@@IABQAV3@@Z
InsertFn_004732e0 g_insert_004732e0 = &Vec_004732e0::insert;
