// Decompiled by space-bunny-free. Names are provisional.
// Clears the unit's target entry `index` (the same reset as ClearWeaponTarget) and
// tells the unit's script "StartBuilding" and "TargetCleared", but only when
// the entry's flag byte at +0x1b has bit 1 and bit 4 both set, and the entry
// is not already clear. Note that bit 4 is *cleared* again on entry, so a set
// bit 4 makes the test pass and then gets turned off: see the note at the end.
// Index 3 is not an entry of its own: it recurses into 0 and 1 and then works
// on entry 2, so the byte parameter is a plain unsigned char.
// The flags byte needs the one-bit bitfield view: `e->flags.bits.bit4 = 0`
// is what produces the read-modify-write `and al, 0xef` on the container byte
// with the store through the element pointer (`[ecx + 0x1b]`), while the same
// thing written as a mask on a plain byte folds the element offset into the
// address (`lea ecx, [edi + eax*4 + 0x1f]`, `mov [ecx], al`) instead.
class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8);
    int FindScript(char* name);
};

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

class Unit {
public:
    int unknown_0;
    Entry_004898b0 entries[5];          // +0x4
    char unknown_90[0x9a - 0x90];
    CobScript* script;             // +0x9a

    void ClaimWeapons(unsigned char index);
};
#pragma pack(pop)

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
        e->flags.bits.bit4 = 0;
        int i = index;
        Point_004898b0* p = &entries[i].point;
        if (p->a != 0 || p->b != (short)0x8000) {
            p->a = 0;
            p->b = (short)0x8000;
            script->FindScript("StartBuilding");
            ((CobScript*)script)->StartScriptWithArgs("TargetCleared", 0, 0, 1, i, 0, 0, 0);
        }
    }
    // The guard only fires when bit 4 is *set* and then clears it, so this
    // entry clears its target exactly once per set of bit 4 and never sets
    // it. The twin at 0x489800 is the other way round (it requires bit 4 to be
    // clear and then sets it), so this looks like the two halves of one
    // "toggle the target flag" that was written twice instead of once.
}
