// Decompiled by space-bunny-free, finished by mimo-v2.6-flash, finished by space-bunny-free. Names are provisional.
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
// 96.7%: the size (448 bytes), the registers, the jump targets and every
// instruction of the function match except one ten instruction block, which is
// the same instructions in the original's order. The original
//
//     mov edx, [esi+0x110]   ; the bitfield's storage word
//     mov cl, [ebp+5]        ; the value's operand
//     and edx, 0xfffdffff
//     xor eax, eax / cmp cl, 0xff / sete al / and eax, 1 / shl eax, 0x11
//     or edx, eax / mov [esi+0x110], edx
//
// loads the storage word first and clears it above the whole comparison; mine
// (a named bool plus a bitfield store, the only shape that keeps the field
// width mask `and eax, 1`) emits
//
//     mov dl, [ebp+5] / xor eax, eax / cmp dl, 0xff
//     mov edx, [esi+0x110]
//     sete al / and eax, 1 / and edx, 0xfffdffff / shl eax, 0x11
//     or edx, eax / mov [esi+0x110], edx
//
// which is the same ten instructions, the same lengths and the same final
// registers (eax holds the value, edx the word) with only the order, and with
// the byte's operand in dl instead of cl.
//
// What earlier workers ruled out (the clear as its own statement grows the
// function to 461 or 465 bytes, so the original really does one read modify
// write in a single bitfield store; local word copies sink the word load to its
// use; a hand written whole word read modify write folds the clear into the
// `|`; `T&`, pointers to the union, `char`/`const bool` temps, an 18 bit or
// `unsigned long` bitfield group and every header set either keep this order or
// move the word into eax and the value into edx) all still holds. What is new
// from the third pass:
//
// - The original's order is a plain walk of the tree "destination first, then
//   value": word load, operand load, clear, zero, compare, set, mask, shift,
//   or, store. MSVC 5 only emits that walk when the value's operand needs a
//   conversion node, because the conversion is then scheduled before the
//   destination's clear. Both of this function's own spellings that keep the
//   comparison a dword one produce exactly that order:
//       u->f110.bits.b17 = (order->param == -1);            (unsigned char)
//         mov edx,[esi+0x110] / xor eax,eax / mov al,[ebp+5]
//         and edx,0xfffdffff / xor ecx,ecx / cmp eax,-1 / sete cl
//         and ecx,1 / shl ecx,0x11 / or edx,ecx / mov [esi+0x110],edx
//       u->f110.bits.b17 = (order->param == 0xff);          (signed char)
//         movsx edx,[ebp+5] / mov ecx,[esi+0x110] / xor eax,eax
//         cmp edx,0xff / sete al / and eax,1 / and ecx,0xfffdffff
//         shl eax,0x11 / or eax,ecx / mov [esi+0x110],eax
//   Both need a constant outside the byte's range, so they change the meaning
//   and are not used. Every spelling that keeps the meaning (`unsigned char`
//   against 0xff, or `signed char` against -1) is a byte compare, and MSVC then
//   sinks the clear to after the value and folds the byte's operand into the
//   result register (dl or al). The two effects always go together.
// - The mask `and eax, 1` and a separate register for the compare's byte
//   operand are also mutually exclusive in every spelling: `&& 1` (or a bitfield
//   store, or a named bool) gives the separate register, `& 1` gives the mask.
//   With `&& 1` in a whole word read modify write the rest of the block,
//   including the byte in cl, is right and only the mask and the order are
//   wrong; with `& 1` the mask is right and the byte goes to al.
// - 0x485a40 (still open, issue #787) has the same block with the same register
//   roles (value eax, word edi, byte cl, `or edi,eax`, store through edi) but
//   compares `cmp cl, bl` against a zeroed local, and a register to register
//   compare cannot reuse the result register for the operand. That is why it
//   gets cl and we do not. A `static unsigned char` holding 0xff here does give
//   `cmp cl, dl` and the wanted `or edx, eax` plus store through edx, but the
//   compare then carries a register, not the 0xff immediate.
// - So treat the two register classes and the order as one scheduling decision
//   that no spelling found here reaches, not as a missing piece of syntax.
//
// Method note for the next attempt: compiling every candidate spelling of the
// statement into its own copy of the whole function in a single file, and
// disassembling all of them at once, explores hundreds of shapes per minute
// (build/scratch/48ab70/genprobe.py and blocks.py). It also showed that a
// whole word read modify write `(w & mask) | v` puts the value first whichever
// operand the source writes first (MSVC 5 sorts commutative operands), and that
// adding redundant nodes around either operand (`| 0`, `^ 0`, `& 0xffffffff`,
// `(unsigned)` casts, an extra `& 1`) never changes the order.

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
