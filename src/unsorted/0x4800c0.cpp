// Decompiled by Opus. Names are provisional.
#include <vector>

struct Unit_004800c0 {
    int unknown_0;
};

class Class_004800c0 {
public:
    typedef std::vector<Unit_004800c0*> UnitVector;
    UnitVector units;       // allocator +0x0, _First +0x4, _Last +0x8, _End +0xc

    UnitVector::iterator FUN_004800c0(UnitVector::iterator where);
};

// Unordered erase: overwrites *where with the last element, drops the last
// element (erase(end() - 1), whose inlined copy loop and _Destroy dead store
// remain) and returns where, which now holds the moved element. The only
// caller (0x40b8b2) passes a slot of this same vector.
// FUNCTION: 0x4800c0
Class_004800c0::UnitVector::iterator Class_004800c0::FUN_004800c0(UnitVector::iterator where)
{
    UnitVector::iterator last = units.end() - 1;
    *where = *last;
    units.erase(last);
    return where;
}
