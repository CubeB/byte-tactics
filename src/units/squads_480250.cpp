// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Moves a unit to the unit group (squad) `index`: first removes it from the
// group it is currently in (unit+0xac, -1 means none), then adds it to the
// new group, then records the new group index. Each player owns ten
// 0x20-byte Squad squads at player+0x78 (built by 0x480190); the
// squad holds its units in the std::vector<Unit*> at +0x10.
//
// The removal (`RemoveFast`) is a separate inline helper: inlining it is what
// leaves the compiler unable to also inline the four size() occurrences in
// push_back's reallocation branch, so three of them stay out-of-line calls to
// 0x40c560 exactly as in the original. Written as one flat body the compiler
// inlines all of them and the function is 16 bytes too long.
//
// The removal itself overwrites the found slot with the last element and then
// erases the last element (order is not preserved); the original used
// v.erase(v.end() - 1) rather than pop_back(), and that shape (the dead
// "copy(end, end, end-1)" loop before --_Last) is what makes block 1 match.
#include <vector>
#include <algorithm>

class Squad;

#pragma pack(push, 1)
struct Player_00480250 {
    char unknown_0[0x78];
    Squad* squads;                     // +0x78
};

struct Unit {
    char unknown_0[0x96];
    Player_00480250* owner;            // +0x96
    char unknown_9a[0xac - 0x9a];
    int group;                         // +0xac
};
#pragma pack(pop)

class Squad {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    std::vector<Unit*> items;          // +0x10
};

// Removes `u` from `v` by moving the last element into its slot and erasing
// the last element. Kept as an inline helper (see the note above): the
// original translation unit had this as its own small function.
static inline void RemoveFast(std::vector<Unit*>& v, Unit* u)
{
    std::vector<Unit*>::iterator it = std::find(v.begin(), v.end(), u);
    if (it != v.end()) {
        std::vector<Unit*>::iterator last = v.end() - 1;
        *it = *last;
        v.erase(last);
    }
}

// FUNCTION: 0x480250
void __stdcall SetUnitSquad(Unit* unit, int index)
{
    if (unit->group != -1)
        RemoveFast(unit->owner->squads[unit->group].items, unit);
    if (index != -1)
        unit->owner->squads[index].items.push_back(unit);
    unit->group = index;
}
