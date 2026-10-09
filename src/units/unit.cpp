// Decompiled by space-bunny-free, Haiku, deepseek-v4.1-flash, Claude Sonnet 5.5, GPT-6.1-sol, deepseek-v4.1 and mimo-v2.6-pro. Names are provisional.

// <windows.h> is needed for CanRepair's operand order (see there).
#include <windows.h>
// Only for their symbol ids: SetStateBits matches at one symbol count.
#include <malloc.h>
#include <ddraw.h>

#include "cob_script.h"

// Unused here: these headers and forward declarations take the symbol ids that keep
// CanLoad (0x489a90) and SetStateBits (0x48b090) matching (docs/c2-regalloc.md).
#include "../util/hapi_bank.h"
#include "../map/mission.h"
#include "../sound/sound.h"


class Unit;

#pragma pack(push, 1)
struct Point_004898b0 {
    short a;                           // +0x0
    short b;                           // +0x2
};

union Flags_004898b0 {
    struct {
        unsigned char bit0 : 1;
        unsigned char bit1 : 1;        // tested with test al, 2
        unsigned char bit2 : 1;
        unsigned char bit3 : 1;
        unsigned char bit4 : 1;        // tested with test al, 0x10
        unsigned char rest : 3;
    } bits;
    unsigned char all;
};

struct Entry_004898b0 {                // 0x1c bytes
    Point_004898b0 point;              // +0x0
    char unknown_4[0x1b - 4];
    Flags_004898b0 flags;              // +0x1b
};

// Bit 19 is a one-bit bitfield (MSVC shifts it down to test it), while bit 8
// of the same word is tested as a plain mask.
union Flags_00489a90 {
    struct {
        unsigned int unknown_0 : 19;
        unsigned int flag19 : 1;       // bit 19, tested with shr eax, 0x13
        unsigned int unknown_1 : 12;
    } bits;
    int all;
};

// The unit's type.
struct UnitDef {
    char unknown_0[0x14a];
    short f14a;                        // +0x14a, compared signed
    char unknown_14c[0x16e - 0x14c];
    union {
        int f16e;                      // +0x16e, 16.16
        struct {
            unsigned short f16e_fraction;
            short f170;                // +0x170, compared signed
        };
    };
    char unknown_172[0x1be - 0x172];
    short f1be;                        // +0x1be, compared signed
    short f1c0;                        // +0x1c0, tested for < 0
    char unknown_1c2[0x1fa - 0x1c2];
    int f1fa;                          // +0x1fa, compared against a sign extended short
    char unknown_1fe[0x22a - 0x1fe];
    unsigned char f22a;                // +0x22a
    unsigned char f22b;                // +0x22b
    char unknown_22c[0x241 - 0x22c];
    int f241;                          // +0x241, bits 11 and 21
    Flags_00489a90 f245;               // +0x245, bits 8, 9, 10, 12 and 19
};

struct Node_00489a90 {
    char unknown_0[0x86];
    Unit* owner;                       // +0x86
    char unknown_8a[4];
    Node_00489a90* next;               // +0x8e
};

struct Game {
    char unknown_0[0x1427f];
    unsigned char f1427f;              // +0x1427f
};

struct Player_0048b090 {
    int active;                        // +0x0
    int id;                            // +0x4
    char unknown_8[0x73 - 0x8];
    char kind;                         // +0x73, 1 or 2 for a real player
};

struct ActivateFlagsPacket {
    unsigned char type;                // +0x0
    short unitId;                      // +0x1, the unit id
    unsigned char activateFlags;       // +0x3, the new state
};
#pragma pack(pop)

extern Game* g_game;

// One virtual slot, called on the object a link belongs to.
class Class_0043a1e0 {
public:
    virtual void OrStatusFlags(unsigned int value);
};

// The links of the owner's list; the head of a unit's list is at +0xa2.
class PathOrderAttach {
public:
    void* vptr;                        // +0x0
    void* owner;                       // +0x4
    PathOrderAttach* next;             // +0x8
    Class_0043a1e0* value;             // +0xc
};

#pragma pack(push, 1)
class Unit {
public:
    int f0;                            // +0x0
    Entry_004898b0 entries[3];         // +0x4
    char unknown_58[0x6e - 0x58];
    union {
        int f6e;                       // +0x6e, 16.16
        struct {
            unsigned short f6e_fraction;
            short f70;                 // +0x70
        };
    };
    char unknown_72[0x8a - 0x72];
    Node_00489a90* cargo;           // +0x8a, list head of the cargo count
    char unknown_8e[4];
    UnitDef* def;                      // +0x92
    Player_0048b090* player;           // +0x96
    CobScript* script;                 // +0x9a
    void* block;                       // +0x9e
    PathOrderAttach* head;             // +0xa2
    char unknown_a6[0xa8 - 0xa6];
    unsigned short id;                 // +0xa8
    char unknown_aa[0x104 - 0xaa];
    float f104;                        // +0x104
    short f108;                        // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char state;               // +0x10e
    char unknown_10f;
    int f110;                          // +0x110

    unsigned char GetState() { return state; }

    void ReleaseWeapons(unsigned char index);
    void ClaimWeapons(unsigned char index);
    int CanReclaim(void *param_1);
    int CanRepair(Unit* other);
    int CountCargo();
    int CanLoad(Unit* other);
    void SetStateBits(int mask, int set);
};
#pragma pack(pop)

void __stdcall QueueUnitSpeech(Unit* unit, int kind, char* text);
void __stdcall MarkSelectionOrdersDirty(Unit* unit);
int __stdcall BroadcastPacket(int player, void* data, int size);

// The twin of ClaimWeapons (0x4898b0) with bit 4 the other way round: it
// requires bit 4 clear and then sets it.
// FUNCTION: 0x489800
void Unit::ReleaseWeapons(unsigned char index)
{
    if (index == 3) {
        this->ReleaseWeapons(0);
        this->ReleaseWeapons(1);
        index = 2;
    }
    Entry_004898b0* e = &entries[index];
    if (e->flags.bits.bit1 != 0 && e->flags.bits.bit4 == 0) {
        e->flags.bits.bit4 = 1;
        int i = index;
        Point_004898b0* p = &entries[i].point;
        if (p->a != 0 || p->b != (short)0x8000) {
            p->a = 0;
            p->b = (short)0x8000;
            script->FindScript("StartBuilding");
            script->StartScriptWithArgs("TargetCleared", 0, 0, 1, i, 0, 0, 0);
        }
    }
}

// Clears the unit's target entry `index` (the same reset as ClearWeaponTarget) and
// tells the unit's script "StartBuilding" and "TargetCleared", but only when
// the entry's flag byte at +0x1b has bit 1 and bit 4 both set, and the entry
// is not already clear. Note that bit 4 is *cleared* again on entry, so a set
// bit 4 makes the test pass and then gets turned off: see the note at the end.
// Index 3 is not an entry of its own: it recurses into 0 and 1 and then works
// on entry 2, so the byte parameter is a plain unsigned char.
// FUNCTION: 0x4898b0
void Unit::ClaimWeapons(unsigned char index)
{
    if (index == 3) {
        this->ClaimWeapons(0);
        this->ClaimWeapons(1);
        index = 2;
    }
    Entry_004898b0* e = &entries[index];
    if (e->flags.bits.bit1 != 0 && e->flags.bits.bit4 != 0) {
        // Bitfield view, not a byte mask: a mask folds the element offset into the address.
        e->flags.bits.bit4 = 0;
        int i = index;
        Point_004898b0* p = &entries[i].point;
        if (p->a != 0 || p->b != (short)0x8000) {
            p->a = 0;
            p->b = (short)0x8000;
            script->FindScript("StartBuilding");
            script->StartScriptWithArgs("TargetCleared", 0, 0, 1, i, 0, 0, 0);
        }
    }
    // The guard only fires when bit 4 is *set* and then clears it, so this
    // entry clears its target exactly once per set of bit 4 and never sets
    // it. The twin at 0x489800 is the other way round (it requires bit 4 to be
    // clear and then sets it), so this looks like the two halves of one
    // "toggle the target flag" that was written twice instead of once.
}

// FUNCTION: 0x489960
int Unit::CanReclaim(void *param_1)
{
    void *ptr = *(void **)((char *)this + 0x92);
    unsigned int val1 = *(unsigned int *)((char *)ptr + 0x245);

    if ((val1 & 0x400) != 0) {
        unsigned int val2 = *(unsigned int *)((char *)param_1 + 0x110);

        if ((val2 & 3) != 2) {
            void *ptr2 = *(void **)((char *)param_1 + 0x92);
            unsigned int val3 = *(unsigned int *)((char *)ptr2 + 0x245);

            if ((val3 & 0x1000) == 0) {
                return 1;
            }
        }
    }

    return 0;
}

// Same class as 0x489a90: a `[ecx+0x92]` def pointer, the def flags word at
// +0x245 and the def word at +0x241, and the g_game byte at +0x1427f.
//
// A yes/no test between two units of that class.
// FUNCTION: 0x4899b0
int Unit::CanRepair(Unit* other)
{
    if (other
        && (def->f245.all & 0x200)
        && (other->f108 != other->def->f1fa)
        && ((other->f110 & 3) != 2)) {
        if (def->f241 & 0x800) {
            if (!(def->f241 & 0x200000) && other->f70 + other->def->f170 < g_game->f1427f)
                goto fail;
        }
        // Tests the same condition again, not an else.
        if (!(def->f241 & 0x800)) {
            if (other->f70 + other->def->f170 < g_game->f1427f - def->f1be)
                goto fail;
        }
        return 1;
    }
    // Failures are a goto to this shared return 0, placed after the return 1.
fail:
    return 0;
}

// FUNCTION: 0x489a70
int Unit::CountCargo()
{
    int count = 0;
    Node_00489a90* node = cargo;
    while (node) {
        if (node->owner == this)
            count++;
        node = node->next;
    }
    return count;
}

// A yes/no test between two units of that class: it returns 1 only when every
// check below passes, and 0 from eight separate early returns.
// FUNCTION: 0x489a90
int Unit::CanLoad(Unit* other)
{
    UnitDef* theirDef = other->def;
    if (theirDef->f245.bits.flag19)
        return 0;
    UnitDef* ourDef = def;
    if (!(ourDef->f245.all & 0x100))
        return 0;
    if (CountCargo() >= ourDef->f22b)
        return 0;
    if (!other->f0)
        return 0;
    if (theirDef->f14a > ourDef->f22a)
        return 0;
    if ((other->f110 & 3) == 2)
        return 0;
    if (!(ourDef->f241 & 0x800) && theirDef->f1c0 >= 0)
        return 0;
    if (other->f6e + theirDef->f16e <= (g_game->f1427f << 16))
        return 0;
    if (other->f104 != 0.0f)           // the fcomp tests equality, not a range
        return 0;
    return 1;
}

// Sets or clears bits of the unit's state byte at +0x10e and reacts to the three
// bits that mean active (1), building (8) and working (4). The gained and the
// lost bits are tested separately: each gained bit plays its script event and
// its message, and losing the working bit (4) tells every object linked to this
// unit (the list head at +0xa2) to update, then a network packet (0x11) tells
// the owner when the owner is a real player (1 or 2).
static inline int HasBit(unsigned char bits) { return 1 & bits; }

static inline int LostBits(unsigned char was, int is) { return (unsigned char)was & ~is; }

static inline unsigned char GainedBits(unsigned char was, int is) { return (unsigned char)(~was & (int)is); }

static inline unsigned char AsByte(unsigned char bits) { return (unsigned char)bits; }

// Unused here: a real declaration from the neighbouring COB code, kept
// because its symbol ids are what SetStateBits matches at (docs/c2-regalloc.md).
void FreeCobScript(int);

// FUNCTION: 0x48b090
void Unit::SetStateBits(int mask, int set)
{
    // Old state read through the in-class accessor: it fixes the set arm's load order.
    unsigned char lost, gained, old = GetState();
    int isOne, active;
    PathOrderAttach* link;
    int now;
    if (set)
        now = old | (unsigned char)mask;
    else
        now = old & ~(mask & 0xff);
    state = (unsigned char)now;
    {
        if ((unsigned char)now != old) {
            // LostBits returns int and narrows its first operand, and lost then
            // goes round AsByte: that pair is what makes MSVC 5 compute `lost`
            // into cl, the original's register, instead of into al. isOne and
            // active are unused, and so are HasBit and GainedBits above; all
            // four are dead weight the bytes need.
            lost = LostBits(old, now);
            unsigned char newLost = AsByte(lost);
            gained = ~old & now;
            lost = (unsigned char)newLost;
            if (gained & 1) {
                script->StartScript("Activate", 0, 0);
                QueueUnitSpeech(this, 3, 0);
            }
            if (lost & 1) {
                script->StartScript("Deactivate", 0, 0);
                QueueUnitSpeech(this, 4, 0);
            }
            if (gained & 8)
                script->StartScript("StartBuilding", 0, 0);
            if (lost & 8)
                script->StartScript("StopBuilding", 0, 0);
            if (gained & 4) {
                QueueUnitSpeech(this, 0xe, 0);
                for (link = head; link; link = link->next) {
                    if (link->value)
                        link->value->OrStatusFlags(0x10000);
                }
            }
            if (lost & 4)
                QueueUnitSpeech(this, 0xf, 0);
            MarkSelectionOrdersDirty(this);
            if (player->active != 0) {
                if (player->kind == 1 || player->kind == 2) {
                    ActivateFlagsPacket packet;
                    packet.type = 0x11;
                    packet.unitId = id;
                    packet.activateFlags = state;
                    BroadcastPacket(player->id, &packet, 4);
                }
            }
        }
    }
}
