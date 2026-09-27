// Decompiled by GPT-6 Astra. Names are provisional.
struct Unit;
void __stdcall FUN_00406c70(Unit**, Unit* const*);
namespace std {
inline void _Construct(Unit** dest, Unit* const& src) { FUN_00406c70(dest, &src); }
}
#include <vector>
#pragma pack(push, 1)
struct UnitDef { char pad0[0x1fa]; unsigned int maxHealth; };
struct Owner { char pad0[0x108]; unsigned char allied[0x3e]; unsigned char index; };
struct Unit {
    char pad0[0x92]; UnitDef* def; Owner* owner;
    char pad9a[0xf4-0x9a]; unsigned char orderPlayer, orderKind;
    char padf6[0x104-0xf6]; float progress;
    short health; char pad10a[6]; unsigned int flags;
};
#pragma pack(pop)
class Class_00405d90 {
public:
    virtual void FUN_00405d90(Unit*);
    Owner* owner;
    std::vector<Unit*>* units;
    Unit* self;
};
// FUNCTION: 0x405d90
void Class_00405d90::FUN_00405d90(Unit* unit)
{
    if (unit == self) return;
    unsigned int index = 0;
    index = unit->owner->index;
    if (!owner->allied[index]) return;
    unsigned int kind = unit->flags & 3;
    if ((unsigned char)kind != 1) return;
    if ((unsigned int)unit->health >= unit->def->maxHealth && unit->progress == 0.0f) return;
    if (unit->orderPlayer == owner->index && unit->orderKind == 5) return;
    units->push_back(unit);
}
