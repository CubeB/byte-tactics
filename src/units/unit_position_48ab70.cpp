// Decompiled by space-bunny-free, finished by mimo-v2.6-flash, finished by space-bunny-free, finished by MiMo-V2.6-Pro. Names are provisional.
// The order record handler at 0x48aac0 builds a seven byte record on its stack
// (type 0x0a, two unit ids, two bytes) and passes it here after it has been
// through FUN_004fdb0 and FUN_00451df0, so this runs the order on the sending
// player's own units. The two ids index the unit array at g_game+0x14357
// (0x118 byte entries) and both index 0 means "no unit".
//
// The first unit (esi) must be live (flag 0x10000000 at +0x110), not dying
// (0x20000000 clear) and not already a container (its first child at +0x8a is
// null). The second unit (edi), if there is one, must be live, must not be the
// same unit and must have no attached unit at +0x86. Then the first unit is
// unlinked from the tree it is in (walking the sibling chain through +0x8a /
// +0x8e of its owner at +0x86, or asking the owner's list object at +0x82 to
// unlink it when it is a root), the order byte at +5 is stored at +0xf9, and
// the unit is either re-attached under the second unit (which then also gets
// bit 17 of +0x110 when the order byte is 0xff) or detached (bit 17 cleared,
// the list object's push is run). The byte at +6 is then blended into the two
// low bits of the type's byte at +0x2e (the XOR blend is MSVC 5's read
// modify write of a two bit field), and a "BECARRIED" child is created when the
// owner is a human or computer player, its unit list is not empty, a second
// unit was given, and that unit's type does not have bit 9 of the word at
// +0x241 set. FUN_0048c9b0 then refreshes the order, and it is only reached on
// the paths that got that far: every test that fails jumps past it.
//
// MATCH. The block that stores bit 17 is a named bool plus a bitfield store,
// with the comparison done on a `char` against -1 and its result masked with
// `& 1` before the assignment:
//
//     bool v = ((order->param == -1) & 1);
//     u->f110.bits.b17 = v;
//
// That combination gives every part of the original's ten instruction block:
// the `& 1` keeps the field width mask `and eax, 1` (which a plain bitfield
// store of the comparison drops), the named bool keeps the compare's byte
// operand in its own register (cl instead of folding it into the result
// register), and the `char` against -1 is a byte compare (`cmp cl, 0xff`) whose
// operand load is scheduled between the storage word's load and its clear.
// Earlier passes tried each half alone: a bitfield store of `param == 0xff`
// with an `unsigned char` sinks the clear below the comparison and puts the
// operand in dl, and `& 1` inside a whole word read modify write keeps the
// operand in cl but loses the width mask and gets the value computed first.

#pragma pack(push, 1)

struct Def_0048ab70 {
    char unknown_0[0x2e];
    unsigned char bits_2e : 2;        // +0x2e, a two bit field
    char unknown_2f[4];
};

struct Type_0048ab70 {
    char unknown_0[0x241];
    unsigned int f241 : 12;           // +0x241, bit 9
};

struct Player_0048ab70 {
    int f0;                            // +0x00
    char unknown_4[0x73 - 4];
    unsigned char f73;                 // +0x73
};

union Flags_0048ab70 {
    struct {
        unsigned int low : 17;
        unsigned int b17 : 1;          // bit 17
        unsigned int high : 14;
    } bits;
    unsigned int all;
};

class Class_0047cb00 {
public:
    char unknown_0[6];
    void* head;                        // +6
    void FUN_0047cb00(void* node);
};

class Class_0047cb40 {
public:
    char unknown_0[6];
    int head;                          // +6
    void FUN_0047cb40(int node);
};

struct Unit {
    Def_0048ab70* def;                 // +0x00
    char unknown_4[0x82 - 4];
    Class_0047cb00* list;              // +0x82
    Unit* owner;                       // +0x86
    Unit* first;                       // +0x8a
    Unit* next;                        // +0x8e
    Type_0048ab70* type;               // +0x92
    Player_0048ab70* player;           // +0x96
    char unknown_9a[0xf9 - 0x9a];
    unsigned char f9;                  // +0xf9
    char unknown_fa[0x110 - 0xfa];
    Flags_0048ab70 f110;               // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game_0048ab70 {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
};

// The seven byte order record the caller at 0x48aac0 builds on its stack.
struct Order_0048ab70 {
    unsigned char type;                // +0x00
    unsigned short id1;                // +0x01
    unsigned short id2;                // +0x03
    char param;                        // +0x05
    unsigned char param2;              // +0x06
};

struct Beacon_0048ab70;                // what FUN_004384a0 wants
#pragma pack(pop)

extern Game_0048ab70* g_game;

void __stdcall FUN_004384a0(Beacon_0048ab70* beacon);
void __stdcall FUN_0048c9b0(Unit* u);

// FUNCTION: 0x48ab70
void __stdcall FUN_0048ab70(Order_0048ab70* order)
{
    Unit* u = !order->id1 ? 0 : &g_game->units[order->id1];
    Unit* t = !order->id2 ? 0 : &g_game->units[order->id2];
    if (u) {
        unsigned int f = u->f110.all;
        if (f & 0x10000000) {
            if (!(f & 0x20000000)) {
                if (u->first == 0) {
                    if (t == 0
                        || ((t->f110.all & 0x10000000) && t != u && t->owner == 0)) {
                        if (u->owner != 0) {
                            Unit* n = u->owner;
                            Unit** link = &n->first;
                            n = n->first;
                            while (n != u) {
                                link = &n->next;
                                n = n->next;
                            }
                            *link = u->next;
                        } else {
                            u->list->FUN_0047cb00(u);
                        }
                        u->f9 = order->param;
                        if (t) {
                            u->owner = t;
                            u->next = t->first;
                            t->first = u;
                            bool v = ((order->param == -1) & 1);
                            u->f110.bits.b17 = v;
                        } else {
                            u->next = 0;
                            u->f110.bits.b17 = 0;
                            u->owner = 0;
                            ((Class_0047cb40*)u->list)->FUN_0047cb40((int)u);
                        }
                        u->def->bits_2e = order->param2;
                        if (u->player->f0) {
                            unsigned char k = u->player->f73;
                            if (k == 1 || k == 2) {
                                if (t && !(t->type->f241 & 0x200))
                                    FUN_004384a0((Beacon_0048ab70*)u);
                            }
                        }
                        FUN_0048c9b0(u);
                    }
                }
            }
        }
    }
}
