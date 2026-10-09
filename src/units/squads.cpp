// Decompiled by Opus and deepseek-v4.1-flash. Names are provisional.

#include <vector>
#include <algorithm>

class Squad;

class MissionType {
public:
    unsigned char index;
    MissionType(const char* name);
};

#pragma pack(push, 1)
struct Unit;

struct PlayerView {
    char unknown_0[0x67];
    Unit* first;                       // +0x67
    Unit* last;                        // +0x6b
    char unknown_6f[0x78 - 0x6f];
    Squad* squads;                     // +0x78
};

struct Unit {
    char unknown_0[0x96];
    PlayerView* player;                     // +0x96
    char unknown_9a[0xa6 - 0x9a];
    short unitDefIndex;                // +0xa6
    char unknown_a8[0xac - 0xa8];
    int group;                         // +0xac, the squad index; OrderSquad's `key`
    char unknown_b0[0x118 - 0xb0];
};
#pragma pack(pop)

class Squad {
public:
    int owner;                         // +0x0
    int index;                         // +0x4
    int field_8;                       // +0x8
    int field_c;                       // +0xc
    std::vector<Unit*> items;          // +0x10

    Squad(int a, int b)
        : owner(a), index(b), field_8(0), field_c(0)
    {
    }
};

void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __cdecl GameFreeThunk(int* param_1);
MissionType __stdcall GetOrderType(unsigned char mode, Unit* unit,
                                       Unit* target, int flags);
void __stdcall AddOrder(MissionType kind, int remove, Unit* owner, Unit* id, int flags, int param_6, int param_7);

// Allocates the owner's ten squads and constructs each in place with the
// owner and its index (Squad's constructor, inlined here).
// FUNCTION: 0x480190
void __stdcall CreateSquads(PlayerView* owner)
{
    owner->squads = (Squad*)GameAllocIgnoreTag("SQUADS", 10 * sizeof(Squad));
    for (int i = 0; i < 10; i++)
        new (&owner->squads[i]) Squad((int)owner, i);
}

// Destroys the owner's ten squads allocated by 0x480190 and frees them.
// FUNCTION: 0x4801f0
void __stdcall FreeSquads(PlayerView* owner)
{
    if (owner->squads) {
        for (int i = 0; i < 10; i++)
            owner->squads[i].items.~vector();
        GameFreeThunk((int*)owner->squads);
        owner->squads = 0;
    }
}

// Removes `u` from `v` by moving the last element into its slot and erasing
// the last element. Kept as an inline helper: the
// original translation unit had this as its own small function.
static inline void RemoveFast(std::vector<Unit*>& v, Unit* u)
{
    std::vector<Unit*>::iterator it = std::find(v.begin(), v.end(), u);
    if (it != v.end()) {
        std::vector<Unit*>::iterator last = v.end() - 1;
        *it = *last;
        // erase(end() - 1), not pop_back().
        v.erase(last);
    }
}

// Moves a unit to the unit group (squad) `index`: first removes it from the
// group it is currently in (unit+0xac, -1 means none), then adds it to the
// new group, then records the new group index. Each player owns ten
// 0x20-byte Squad squads at player+0x78 (built by 0x480190); the
// squad holds its units in the std::vector<Unit*> at +0x10.
//
// The removal itself overwrites the found slot with the last element and then
// erases the last element (order is not preserved).
// FUNCTION: 0x480250
void __stdcall SetUnitSquad(Unit* unit, int index)
{
    // RemoveFast stays a separate inline helper: a flat body inlines push_back's size() calls too.
    if (unit->group != -1)
        RemoveFast(unit->player->squads[unit->group].items, unit);
    if (index != -1)
        unit->player->squads[index].items.push_back(unit);
    unit->group = index;
}

// For every active unit in the array owned by `owner` whose field at +0xac
// equals `key`, asks GetOrderType for an order kind and hands it, with the
// remaining arguments, to AddOrder.
// FUNCTION: 0x480460
void __stdcall OrderSquad(PlayerView* owner, int key, unsigned char mode, int remove,
                            Unit* target, int flags, int param_7, int param_8)
{
    for (Unit* u = owner->first; u <= owner->last; u++) {
        if (u->unitDefIndex != 0 && u->group == key) {
            MissionType kind = GetOrderType(mode, u, target, flags);
            AddOrder(kind, remove, u, target, flags, param_7, param_8);
        }
    }
}
