// Decompiled by Claude Opus 5.5. Names are provisional.
// Unit visitor (vtable 0x4fcc60, built on the stack by 0x410850 and passed to
// VisitObjectsInRange): collects every unit whose owner is allied with this owner,
// whose def lacks flag 0x800, and that is not the visitor's own unit. The
// same shape as DamagedAllyCollector (0x405d90), with the vector::push_back inlined.
struct Unit;
void __stdcall CopyDwordIfNonNull(Unit**, Unit* const*);
namespace std {
inline void _Construct(Unit** dest, Unit* const& src) { CopyDwordIfNonNull(dest, &src); }
}
#include <vector>
#pragma pack(push, 1)
struct UnitDef { char pad0[0x241]; unsigned int flags; };
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
struct Unit { char pad0[0x92]; UnitDef* def; Owner* owner; };
#pragma pack(pop)
class GroundAllyVisitor {
public:
    virtual void CollectGroundAlly(Unit*);
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
};

// Stays in a file of its own: it matches only in this file's symbol context.
// FUNCTION: 0x410c70
void GroundAllyVisitor::CollectGroundAlly(Unit* unit)
{
    if (unit->owner->allied[owner->index] && !(unit->def->flags & 0x800) && unit != self)
        units->push_back(unit);
}
