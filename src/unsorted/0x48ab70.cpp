// Decompiled by space-bunny-free, finished by mimo-v2.6-flash. Names are provisional.
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
// pinned down by the block itself: the `and eax, 1` is the field width mask
// MSVC 5 puts in front of a bitfield store from a value it cannot prove is
// 0 or 1, so the right hand side has to be a named temporary. Writing the
// comparison inline gives the mask away, an `int` local gives it away, and
// `? 1 : 0` gives the block a different length.
//
// A second worker then ruled out, with about 60 more scratch variants, every
// other way of getting the load and the clear above the comparison:
// - The clear as its own statement (`b17 = 0;` before the compare, at the top
//   of the `if (t)` block, or as `b17 &= 0;`) is NOT folded away: the
//   function grows to 461 or 465 bytes, so the original really does the whole
//   read-modify-write in one bitfield store.
// - A hand written whole word read-modify-write
//   (`w = u->f110.all; w &= 0xfffdffff; ... u->f110.all = w | (v << 17);`)
//   puts the byte in cl, which is what the original has, but MSVC 5 sinks
//   the word load to the use and folds the `&=` into the `|`, giving
//   [char][word][xor][cmp][sete][shl][and][or][store]: the load lands second
//   instead of first, the clear lands after the value, and a named `bool`
//   shifted by 17 spills through the stack (`mov byte ptr [esp+N], dl`), so
//   the block loses the field mask.
// - A local copy of the flags union, an inline `set17` on it, a `T&` to the
//   union or to the unit, a pointer to the union, `(Flags*)&w`, the clear
//   through a second identical bitfield view, the three field stores plus the
//   store in one inline method: every one of these compiles to exactly the
//   block above. So do a `char`/`const bool`/`register bool` value, a
//   `(char)` or `((int) == 255)` cast, an extra `{}` or `do {} while (0)`
//   scope, `if (t != 0)`, a macro for the compare, `! (p != 0xff)`,
//   `0xff - p == 0`, `& 1`, an `unsigned long` bitfield group, an 18 bit
//   group, and the union with its members swapped.
// - Only the register roles move with compiler state, never the order. Forty
//   unused `extern` prototypes, or any of `<string>`, `<iostream>`,
//   `<vector>`, `<map>`, `<list>` (alone or after `<windows.h>`), put the word
//   in eax and the value in edx (`or eax, edx`), which is 95.0%, worse than
//   the 96.7% here. Below 40 externs the file is exactly this version. Note
//   that tools/headers.py does not include any of those big headers, so they
//   are worth trying when a function is stuck on register roles.
//
// The two register classes and the order are one decision: whichever load MSVC
// 5 emits first takes its register, and the other is forced into the next
// byte register. Treat it as compiler state; 0x485a40 has the same block with
// the same shape (a local word, `&=` then a bool shifted in) and its order is
// a third variant, which is what makes this look like a scheduling accident
// rather than a different source.
//
// A third worker (mimo-v2.6-flash) gave up at 96.7% after about 500 more free
// probe configurations, none of which changed the load order, only the
// register roles:
// - A cross of expression forms (one statement bitfield store, named bool,
//   local word read-modify-write, ternary, comma, anchored dependency) x
//   header sets x 0 to 2000 extern prototypes: every build emits the byte
//   load first. Local word forms sink the word load to its use, so the clear
//   lands after the compare instead of before it.
// - Neighbour in file: defining the predecessor 0x48aac0 in this file changes
//   nothing, and defining the callees 0x47cb00 / 0x47cb40 in this file gets
//   them inlined (the calls vanish) and only moves the byte to cl while the
//   word takes ecx, still byte first.
// - Statement order of independent statements does not survive compilation:
//   `w = u->f110.all; bool v = (order->param == 0xff);` and the reverse order
//   compile to identical bytes.
// - Searching orig/TotalA.exe for siblings: `mov cl, [ebp + 5]` occurs only
//   at 0x48ac8e, and `sete al` followed by `and eax, 1` only at 0x485a93 and
//   0x48ac9c, with no matched source for either site, so there is no in tree
//   shape to copy. Every reachable build puts the byte's load first or sinks
//   the word load to its use; no source shape found emits word first.

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
