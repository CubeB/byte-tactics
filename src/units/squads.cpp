// Decompiled by Opus and deepseek-v4.1-flash. Names are provisional.

#include <vector>
#include <algorithm>

class Squad;

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

#pragma pack(push, 1)
struct Unit;

struct Owner {
    char unknown_0[0x67];
    Unit* first;                       // +0x67
    Unit* last;                        // +0x6b
    char unknown_6f[0x78 - 0x6f];
    Squad* squads;                     // +0x78
};

struct Unit {
    char unknown_0[0x96];
    Owner* owner;                      // +0x96
    char unknown_9a[0xa6 - 0x9a];
    short active;                      // +0xa6
    char unknown_a8[0xac - 0xa8];
    int group;                         // +0xac, the squad index; OrderSquad's `key`
    char unknown_b0[0x118 - 0xb0];
};
#pragma pack(pop)

class Squad {
public:
    int field_0;                       // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    std::vector<Unit*> items;          // +0x10

    Squad(int a, int b)
        : field_0(a), field_4(b), field_8(0), field_c(0)
    {
    }
};

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(int* param_1);
Class_00438760 __stdcall GetOrderType(unsigned char mode, Unit* unit,
                                       Unit* target, int flags);
void __stdcall AddOrder(Class_00438760 kind, int remove, Unit* owner, Unit* id, int flags, int param_6, int param_7);

// Allocates the owner's ten squads and constructs each in place with the
// owner and its index (Squad's constructor, inlined here).
// FUNCTION: 0x480190
void __stdcall CreateSquads(Owner* owner)
{
    owner->squads = (Squad*)FUN_004d83b0("SQUADS", 10 * sizeof(Squad));
    for (int i = 0; i < 10; i++)
        new (&owner->squads[i]) Squad((int)owner, i);
}

// Destroys the owner's ten squads allocated by 0x480190 and frees them.
// FUNCTION: 0x4801f0
void __stdcall FreeSquads(Owner* owner)
{
    if (owner->squads) {
        for (int i = 0; i < 10; i++)
            owner->squads[i].items.~vector();
        FUN_004d85a0((int*)owner->squads);
        owner->squads = 0;
    }
}

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
// FUNCTION: 0x480250
void __stdcall SetUnitSquad(Unit* unit, int index)
{
    if (unit->group != -1)
        RemoveFast(unit->owner->squads[unit->group].items, unit);
    if (index != -1)
        unit->owner->squads[index].items.push_back(unit);
    unit->group = index;
}

// For every active unit in the array owned by `owner` whose field at +0xac
// equals `key`, asks GetOrderType for an order kind and hands it, with the
// remaining arguments, to AddOrder.
// FUNCTION: 0x480460
void __stdcall OrderSquad(Owner* owner, int key, unsigned char mode, int remove,
                            Unit* target, int flags, int param_7, int param_8)
{
    for (Unit* u = owner->first; u <= owner->last; u++) {
        if (u->active != 0 && u->group == key) {
            Class_00438760 kind = GetOrderType(mode, u, target, flags);
            AddOrder(kind, remove, u, target, flags, param_7, param_8);
        }
    }
}
