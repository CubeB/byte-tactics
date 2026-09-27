// Decompiled by space-bunny-free. Names are provisional.
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
// 96.7%: the size, the registers and the jump targets all match, but one ten
// instruction block is in the original's order and not mine. The original
//
//     mov edx, [esi+0x110]   ; the bitfield's storage word
//     mov cl, [ebp+5]        ; the value's operand
//     and edx, 0xfffdffff
//     xor eax, eax / cmp cl, 0xff / sete al / and eax, 1 / shl eax, 0x11
//     or edx, eax / mov [esi+0x110], edx
//
// hoists the storage word load and its clear above the whole comparison, while
// mine emits the comparison first and the load after the `cmp`, with the byte
// operand in dl instead of cl:
//
//     mov dl, [ebp+5] / xor edx, edx / cmp dl, 0xff
//     mov edx, [esi+0x110]
//     sete al / and eax, 1 / and edx, 0xfffdffff / shl eax, 0x11 / or edx, eax
//
// Same instructions, same lengths, same final registers; only the order
// differs, and the register classes follow from it (in the original the word
// load takes edx first, so the byte has to go to cl). The source shape is
// pinned down by the block itself: the `and eax, 1` is the bitfield's field
// width mask, and it only appears when the right hand side is a named `bool`
// (a `bool` local, or an inlined helper returning one). Writing the
// comparison inline gives the mask away, an `int` local gives it away, and
// `? 1 : 0` gives the block a different length. Flat and tried for the order:
// the comparison in fourteen spellings (both operand orders, `!=` then
// negate, masked, `signed char` against -1, `register`), the value from a
// static inline helper taking the byte, the order pointer or the unit, the
// store through a helper taking the unit or a pointer to the word, a pointer
// local for the operand, a bool local declared in the block or at function
// scope or bound to a reference, the word in a union or a plain bitfield
// struct or a struct copy in a local, the bitfield group with one field, two
// fields or a bare field, the statement before or after the three field
// stores it belongs with, the whole branch flattened into one `&&` chain, the
// declaration order of every type in the file, and all 128 header sets from
// tools/headers.py (none matches). Writing the store in the middle of the
// branch does move the byte into cl, which is what the original has, but then
// the word load moves to ecx and the three field stores land after the
// bitfield, so the block still does not match. Treat it as compiler state.

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

struct Unit_0048ab70 {
    Def_0048ab70* def;                 // +0x00
    char unknown_4[0x82 - 4];
    Class_0047cb00* list;              // +0x82
    Unit_0048ab70* owner;              // +0x86
    Unit_0048ab70* first;              // +0x8a
    Unit_0048ab70* next;               // +0x8e
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
    Unit_0048ab70* units;              // +0x14357
};

// The seven byte order record the caller at 0x48aac0 builds on its stack.
struct Order_0048ab70 {
    unsigned char type;                // +0x00
    unsigned short id1;                // +0x01
    unsigned short id2;                // +0x03
    unsigned char param;               // +0x05
    unsigned char param2;              // +0x06
};

struct Beacon_0048ab70;                // what FUN_004384a0 wants
#pragma pack(pop)

extern Game_0048ab70* g_game;

void __stdcall FUN_004384a0(Beacon_0048ab70* beacon);
void __stdcall FUN_0048c9b0(Unit_0048ab70* u);

// FUNCTION: 0x48ab70
void __stdcall FUN_0048ab70(Order_0048ab70* order)
{
    Unit_0048ab70* u = !order->id1 ? 0 : &g_game->units[order->id1];
    Unit_0048ab70* t = !order->id2 ? 0 : &g_game->units[order->id2];
    if (u) {
        unsigned int f = u->f110.all;
        if (f & 0x10000000) {
            if (!(f & 0x20000000)) {
                if (u->first == 0) {
                    if (t == 0
                        || ((t->f110.all & 0x10000000) && t != u && t->owner == 0)) {
                        if (u->owner != 0) {
                            Unit_0048ab70* n = u->owner;
                            Unit_0048ab70** link = &n->first;
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
                            bool v = (order->param == 0xff);
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
