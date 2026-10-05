// Decompiled by space-bunny-free. Names are provisional.
// Same class as 0x489a70 (its CountCargo is the owner count inlined below).
// A yes/no test between two units of that class: it returns 1 only when every
// check below passes, and 0 from eight separate early returns, which is why
// the epilogue is duplicated so often.
//
// Bit 19 of the def flags at +0x245 is a one-bit bitfield (MSVC shifts it down
// to test it), while bit 8 of the same word is tested as a plain mask, so the
// word is a union of a bitfield and an int.
#pragma pack(push, 1)
class Unit;

union Flags_00489a90 {
    struct {
        unsigned int unknown_0 : 19;
        unsigned int flag19 : 1;   // bit 19, tested with shr eax, 0x13
        unsigned int unknown_1 : 12;
    } bits;
    int all;
};

struct Def_00489a90 {
    char unknown_0[0x14a];
    short f14a;                   // +0x14a, compared signed
    char unknown_14c[0x16e - 0x14c];
    int f16e;                     // +0x16e
    char unknown_172[0x1c0 - 0x172];
    short f1c0;                   // +0x1c0, tested for < 0
    char unknown_1c2[0x22a - 0x1c2];
    unsigned char f22a;           // +0x22a
    unsigned char f22b;           // +0x22b
    char unknown_22c[0x241 - 0x22c];
    int f241;                     // +0x241, bit 11
    Flags_00489a90 f245;          // +0x245, bits 8 and 19
};

struct Node_00489a90 {
    char unknown_0[0x86];
    Unit* owner;                  // +0x86
    char unknown_8a[4];
    Node_00489a90* next;          // +0x8e
};

struct Game {
    char unknown_0[0x1427f];
    unsigned char f1427f;         // +0x1427f
};

class Unit {
public:
    int f0;                       // +0x0
    char unknown_4[0x6e - 4];
    int f6e;                      // +0x6e
    char unknown_72[0x8a - 0x72];
    Node_00489a90* field_8a;      // +0x8a, list head of the owner count below
    char unknown_8e[4];
    Def_00489a90* def;            // +0x92
    char unknown_96[0x104 - 0x96];
    float f104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    int f110;                     // +0x110

    int CanLoad(Unit* other);
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x489a90
int Unit::CanLoad(Unit* other)
{
    Def_00489a90* theirDef = other->def;
    if (theirDef->f245.bits.flag19)
        return 0;
    Def_00489a90* ourDef = def;
    if (!(ourDef->f245.all & 0x100))
        return 0;
    int count = 0;                    // 0x489a70, inlined
    Node_00489a90* node = field_8a;
    while (node) {
        if (node->owner == this)
            count++;
        node = node->next;
    }
    if (count >= ourDef->f22b)
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
