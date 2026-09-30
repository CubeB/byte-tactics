// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, GPT-6.1-sol and space-bunny-free, edited by deepseek-v4.1. Names are provisional.
//
// deepseek-v4.1 pass (#2409): tried the shape that 0x46d2e0's notes claim for this
// same map insert-then-fill idiom: a constructed temporary with a literal 0
// argument, `head->value = Val_0046d6c0(f6, 0, t.shorts, fc)` (ctors with member
// init lists on Val and Tail). MSVC 5 does NOT construct it in place here: it
// builds the 0x14-byte temporary on the stack (`mov [esp+0x48], 0` plus the other
// three members) and copies it with the 5-dword loop
// `lea edi,[edx+0x10] / mov ecx,5 / lea esi,[esp+0x44] / rep movsd`, so the frame
// grows 0x38 -> 0x48 and `this` moves into ebp: 55.6%, 440 bytes. The 0x46d2e0
// case must have had the value type at 0x10 bytes (4 dwords, no uninitialised
// member), while here the value is 0x14 bytes and the original writes only 4 of
// its 5 dwords (node+0x20 is never stored), so a whole-value construction can
// never be in place. An inline member `void Set(int,int,Shorts,int)` called as
// `head->value.Set(f6, 0, t.shorts, fc)` is no better: the 0 argument inlines to
// `mov dword [ecx+0x14], 0` and the base is still folded with a lea (plus the
// dropped field_2 load shrinks the frame to 0x34, `this` to ebp): 58.9%, 416 bytes.
// What is still needed: a 0x10-byte constructed write to
// node+0x10 that starts at displacement 0 (so `add ecx,0x10` is materialised)
// with the constant 0 in a live callee-saved register.
//
// space-bunny-free pass (#1875): the best is still this file at 83.6% / 420 bytes.
// Three new scratch variants, none better:
//  1. The zero read as a BY-REFERENCE local, `int z = *(int*)&((Handle*)&loc)->flag;`
//     before the insert and `pv->tail.field_0 = z;` after it (the read hits the
//     uninitialised 4-byte local at 0x14, the one the original also copies
//     uninitialised, so the shape is the original's). MSVC 5 has turned that
//     read into a MOVE, not a load: `mov ebp,eax / mov [eax+4],ebp`, and put the
//     packet values in eax/ebx instead of edi/ebp. 77.9%, 416 bytes. The same
//     read left next to the store (to get `mov ebx,ebx`) gives 58.0%. So no
//     non-foldable local, by-reference or not, yields `xor ebx,ebx`; the zero
//     in the original is not a value this source can produce.
//  2. Moving the `v.key = f6` store AFTER the insert call, which is where the
//     original's `mov [esp+0x3c],edi` sits (after both pushes), so the IR order
//     is copy tail, key, call: 82.3%, still 420 bytes and still the folded
//     `mov [ecx+0x10],edi / mov [ecx+0x14],ebx / lea eax,[ecx+0x10]` with the
//     `this` reload in the middle. The single-base `add ecx,0x10` store block
//     is therefore not an IR-order effect of the v.key statement, and most
//     likely follows from the constant 0 being a live callee-saved register
//     (as Claude Sonnet 5.5's frame argument says) rather than from the source
//     shape at all.
// GPT-6.1-sol follow-up (#1543): rechecked the 83.6% best. `f2 = direct` and `f2 = !direct`
// both fell to 58.9% because constant propagation shrinks the frame and changes registers.
// Remaining mismatch: the else path is 4 bytes longer; it loads packet+2 into ebx instead
// of zeroing ebx, and splits node-value stores rather than keeping one base in ecx.
//
// deepseek-v4.1-flash pass (#1137): confirmed the remaining 4 bytes are the
// zero source, and that no literal-zero spelling helps. Scratch variants
// (check.py, all 416 bytes / 58.9%): `int f2 = 0`, `void* f2 = 0` cast to int,
// `int f2 = 0` hoisted to function scope, the same with `packet->field_6 + f2`
// at the call, a `void* field_0` with `= 0`, `short f2 = 0`, `char f2 = 0`, and
// an `int*` base with `p[1] = f2`. Every literal zero drops the extra live
// value, so `this` stays in ebp (frame 0x34) instead of the original's spill to
// [esp+0x10] (frame 0x38). Only a non-constant value that happens to be 0 keeps
// the frame; reading `packet->field_2` does that but adds a `mov ebx,[esi+2]`
// the original does not have. The zero must come from an opaque expression that
// the optimizer cannot fold and that does not touch [esi+2].
//
// check.py: 83.6%, ours 420 bytes against the original's 416. The prologue,
// the entry scan, the switch, case 1, case 2 (whole block) and case 4 are all
// byte exact. The only remaining difference is the last two bytes of the else
// branch, which is 142 bytes against the original's 140:
//
//  1. The original never reads packet->field_2 (there is no [esi+2] load
//     anywhere in 0x46d6c0) and puts a plain zero in ebx with `xor ebx,ebx`,
//     stored post-call as `mov [ecx+4], ebx`. So pv->tail.field_0 is set to 0.
//     Every spelling of a constant zero I could find folds to an immediate
//     (`mov dword ptr [reg+4], 0`) in MSVC 5, so this file still reads
//     field_2 into ebx: `mov ebx,[esi+2]`. Micro-tested and all folded:
//     int z = 0 (named and unnamed), 0L, 0u, (int)0, sizeof(char)-1, a ternary
//     on zero, a value of 0.0f, a zeroed const struct member (that one emits a
//     real load), a bss global (real load), x - x on a memory read, x ^ x,
//     -(-x)+(-x), x/1 - x/1, (p - p), equal bitfields, and x & ~x (that last
//     one does keep a callee-saved register but cannot fold to 0).
//  2. The original materialises the node's value pointer once, `add ecx,0x10`,
//     then stores at +0, +4, +8, +0xc off it. MSVC 5 folds the +0x10 into the
//     first two displacements, then has to build a second base with a lea for
//     the last two, because ecx is reused for `this` in between. Tried and all
//     unchanged at 142 bytes: dropping the pv local entirely (that makes the
//     block 139 and the base eax, with all four folded), taking pv straight from
//     the handle, a const pv, an __inline accessor for &node->value, a char*
//     plus 0x10 cast, pv declared before the call, the four stores in a nested
//     block, key stored last, the shorts store through a Shorts* lvalue, a
//     0xc-byte tail written as one struct copy, mixing node->value and pv, and
//     all 24 orderings of the four pv-> stores (397 or 398 bytes, never 396).
//
// Suspected bugs in Cavedog's original, both verified against the disassembly:
//
//  1. The "already tracked" guard in case 2 is asymmetric and incomplete. The
//     scan at 0x46d748-0x46d762 compares vec1's elements against [arg1+6]
//     ONLY (`mov edx,[ebx+6]` / `cmp [eax],edx`), and a hit jumps to 0x46d760,
//     where `cmp eax,ecx / jne 0x46d842` returns and skips both inserts. But
//     the inserts at 0x46d771-0x46d775 and 0x46d783-0x46d787 push [arg1+6]
//     into vec1 and [arg1+0xa] into vec2. So a new [arg1+0xa] is silently
//     dropped whenever [arg1+6] is already tracked, and a [arg1+0xa] already
//     present in vec2 is inserted a second time because vec2 is never scanned.
//     Intent: the two vectors are index parallel (vec1 holds field_6, vec2 holds
//     field_a), so the guard was plainly meant to test the pair, which needs
//     both vectors scanned, not just the first.
//  2. The counter at +0x5c is bumped before every filter. `inc edx` at 0x46d6dc
//     and the store at 0x46d6df sit before the empty-player-list return at
//     0x46d6e2, before the switch dispatch (`ja 0x46d842` for arg above 4 at
//     0x46d72b) and before the arg != 3 returns at 0x46d7c9/0x46d7cd, so packets
//     that change nothing still consume budget. Only the `disabled` test at
//     0x46d6c9 precedes it. It is a single inc, not an accumulate. The counter
//     is read as a LIMIT by `mov eax,[edi+0x5c] / cmp eax,ebx / jle 0x46dec8`
//     at 0x46dd52, which is inside 0x46dad0, a different method of the same
//     class (0x46dad0 is not a caller of 0x46d6c0; the callers are 0x46cef0 and
//     0x46d500), so the drift silently lowers the effective cap in that method.
//     Intent: the bump belongs after the filters, next to the state change.
// Claude Sonnet 5.5 pass (#601): compiler state is ruled out. N unused `extern int
// dummyK;` lines after the include, K = 8 to 400 step 8 (50 builds, check.py
// --sym, not committed): 83.6 percent and 420 bytes for every K; headers.py, all
// 128 sets: best 83.6, the empty set. What the frame says about the zero in
// point 1: the original spills `this` to [esp+0x10] and has a 0x38 frame, and
// that is caused by the live `xor ebx, ebx` (a real callee-saved register held
// across the insert call). Dropping the `field_2` read entirely and storing a
// literal 0 gives 416 bytes, the original's size, but only 58.9 percent because
// `this` then stays in ebp and the frame is 0x34; so the read of field_2 into ebx
// below is what keeps the register pressure right, and the original had some
// other value in ebx that is 0 at run time. Scored with that change, all 416
// bytes and 58.9: `pv->tail.field_0 = 0`, the same after `pv->key = 0; pv->key =
// f6` (a dead first store, to make the constant appear twice), the store written
// twice, and one before and one after `pv->key = f6`. The value probably comes
// from a variable that the compiler cannot prove is 0 (the packet field at +2
// is not it, the original never reads +2).
#include <vector>

#pragma pack(push, 1)
struct Field_0046d6c0 {               // 4 bytes at +0xa
    union {
        int all;
        struct {
            unsigned char lo;         // +0x0
            unsigned char hi;         // +0x1
            short top;                // +0x2
        } part;
    };
};

struct Packet_0046d6c0 {              // 0xe bytes
    unsigned char type;               // +0x0
    unsigned char arg;                // +0x1
    int field_2;                      // +0x2
    int field_6;                      // +0x6
    Field_0046d6c0 field_a;           // +0xa
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PlayerEntry_0046d6c0 {         // 0x14b bytes
    int id;                           // +0x0, g_game + 0x1b67
    char unknown_4[0x14b - 4];
};

struct Game_0046d6c0 {
    char unknown_0[0x1b67];
    PlayerEntry_0046d6c0 players[10];
};
#pragma pack(pop)

extern Game_0046d6c0* g_game;

struct Shorts_0046d6c0 {
    unsigned short a;                 // +0x0
    unsigned short b;                 // +0x2
};

struct Tail_0046d6c0 {                // 0x10 bytes
    int field_0;                      // +0x0
    Shorts_0046d6c0 field_4;          // +0x4
    int field_8;                      // +0x8
    int field_c;                      // +0xc
};

struct Val_0046d6c0 {                 // 0x14 bytes, the map's value
    int key;                          // +0x0
    Tail_0046d6c0 tail;               // +0x4
};

struct Node_0046d6c0 {
    Node_0046d6c0* left;              // +0x0
    Node_0046d6c0* parent;            // +0x4
    Node_0046d6c0* right;             // +0x8
    unsigned int key;                 // +0xc
    Val_0046d6c0 value;               // +0x10
};


struct H8_0046d6c0 {                 // 8 bytes
    int field_0;                     // +0x0
    Shorts_0046d6c0 shorts;          // +0x4
};

struct H10_0046d6c0 {                // 0x10 bytes
    int field_0;                     // +0x0
    int field_4;                     // +0x4
    Shorts_0046d6c0 shorts;          // +0x8
    short field_c;                   // +0xc
};

struct H18_0046d6c0 {                // 0x18 bytes
    H10_0046d6c0 part;               // +0x00
    H10_0046d6c0 rest;               // +0x10
};

struct Handle_0046d6c0 {              // 5 bytes
    Node_0046d6c0* head;              // +0x0
    unsigned char flag;               // +0x4
};

struct Loc_0046d6c0 {
    Tail_0046d6c0 src;
};

// A std::vector<int>, whose insert() is the out-of-line 0x46e640. Leaving the
// method undefined here is what keeps the call out of line.
class Vec_0046d6c0 {                  // 0x10 bytes
public:
    int pad;                          // +0x0
    int* first;                       // +0x4
    int* last;                        // +0x8
    int* cap;                         // +0xc

    int* begin() { return first; }
    int* end() { return last; }
    void insert(int* pos, int n, int const& val);
};

class Map_0046d6c0 {                  // the map at +0x00
public:
    char compare[4];                  // +0x0
    Node_0046d6c0* head;              // +0x4, the tree's _Head node
    int multi;                        // +0x8
    int size;                         // +0xc

    void FUN_0046ef50(Handle_0046d6c0* out, const Val_0046d6c0* val);
};

struct Entry_0046d6c0 {               // 0x5c bytes
    int id;                           // +0x0
    Vec_0046d6c0 ids;                 // +0x4
    Vec_0046d6c0 pairs;               // +0x14
    int field_24;                     // +0x24
    char unknown_28[0x2c - 0x28];
    unsigned int field_2c;            // +0x2c
    char unknown_30[0x5c - 0x30];
};

class Class_0046d6c0 {
public:
    Map_0046d6c0 map;                            // +0x00
    std::vector<Entry_0046d6c0> players;         // +0x10
    char unknown_20[0x58 - 0x20];
    int direct;                                  // +0x58
    int field_5c;                                // +0x5c
    char unknown_60[0x64 - 0x60];
    int disabled;                                // +0x64

    void FUN_0046d6c0(Packet_0046d6c0* packet, unsigned char player);
    void FUN_0046d860(unsigned int key);
    void FUN_0046d970(unsigned int key, int y);
};

// FUNCTION: 0x46d6c0
void Class_0046d6c0::FUN_0046d6c0(Packet_0046d6c0* packet, unsigned char player)
{
    if (disabled != 0) {
        return;
    }
    field_5c++;
    if (direct != 0) {
        std::vector<Entry_0046d6c0>::iterator i = players.begin();
        if (i != players.end()) {
            unsigned int id = g_game->players[player].id;
            for (; i != players.end(); i++) {
                if (i->id == id) {
                    break;
                }
            }
        }

        switch (packet->arg) {
        case 0:
            break;

        case 1:
            i->field_24 = packet->field_a.all;
            break;

        case 2:
            {
                int* j = i->ids.begin();
                while (j != i->ids.end()) {
                    if (*j == packet->field_6) {
                        break;
                    }
                    j++;
                }
                if (j != i->ids.end()) {
                    return;
                }
            }
            {
                Vec_0046d6c0& v = i->ids;
                v.insert(v.end(), 1, packet->field_6);
            }
            {
                Vec_0046d6c0& w = i->pairs;
                w.insert(w.end(), 1, packet->field_a.all);
            }
            FUN_0046d970(packet->field_6, packet->field_a.all);
            break;

        case 3:
            break;

        case 4:
            if (i->field_2c < (unsigned int)packet->field_a.all) {
                i->field_2c = packet->field_a.all;
            }
            break;
        }
    } else {
        Loc_0046d6c0 loc;
        H10_0046d6c0 t;
        Val_0046d6c0 v;
        if (packet->arg != 0 && packet->arg == 3) {
            int f6 = packet->field_6;
            int f2 = packet->field_2;
            int fc = packet->field_a.part.top;
            t.shorts.a = packet->field_a.part.lo;
            t.shorts.b = packet->field_a.part.hi;
            v.tail = loc.src;
            v.key = f6;
            ((Map_0046d6c0*)this)->FUN_0046ef50((Handle_0046d6c0*)&loc, &v);
            Node_0046d6c0* node = ((Handle_0046d6c0*)&loc)->head;
            Val_0046d6c0* pv = &node->value;
            pv->key = f6;
            pv->tail.field_0 = f2;
            pv->tail.field_4 = t.shorts;
            pv->tail.field_8 = fc;
            FUN_0046d860(packet->field_6);
        }
    }
}
