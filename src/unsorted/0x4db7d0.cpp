// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Third pass by space-bunny-free: still 73.1 percent, 622 of 624 bytes, same
// first hunk (`lea ecx,[esp+0x10]` wanted, this file lays `it1` down at 0x14).
// Two new things were tried, both worse, and both are the natural next moves:
//  * The reference-alias trick the MATCHED sibling 0x4db450 uses to land a
//    local on a dead parameter's home (`Class_004dd2a0& it =
//    *(Class_004dd2a0*)&size;`) applied to the erase out-param, i.e.
//    `Class_004dbe10& node = *(Class_004dbe10*)&p;` with
//    `FUN_004dc910((Class_004dbe10*)&p, it1.ptr)`. The CALL is then exactly
//    the original's `lea edx,[esp+0x60]`, but only if `p` is dead first, and
//    `p` is not: its last use is the `base` mask. Hoisting the `base` copy
//    above the erase to kill it scores 36.8 percent and rewrites the whole
//    prologue (MSVC loads `p` from [esp+4] before the `sub esp`, the frame
//    shrinks to 0x48, and `EnterCriticalSection` loses the `push esi` shape).
//  * The same alias, but with the copy and the mask split the way the original
//    schedules them (`unsigned int base = (unsigned int)p;` before the erase,
//    `base &= 0xfffff000;` after the `DAT_005289f0` subtraction, so the load
//    lands where 0x4db8c0 has it and the `and` where 0x4db8fd has it): 54.5
//    percent, 620 bytes. So the split is not what breaks it; making `p` die
//    before the erase is. `p` has to stay live in its home until the copy, and
//    a local cannot take a home that is still live, which is the wall.
// Second pass by deepseek-v4.1-flash: best is 73.1 percent, 622 of 624 bytes.
// The remaining difference is stack layout plus two scheduling choices:
//  - our `node` (the erase out-param) occupies frame slot 0x10, so it1 lands at
//    0x14 and n at 0x1c. In the original `node` is at the parameter home
//    [esp+0x60], which leaves it1 at 0x10, n at 0x14, it3 at 0x18 and a 4-byte
//    hole at 0x1c. Fixing that one slot should shift every remaining hunk.
//  - declaring node as the by-value return buffer of the erase call (whose real
//    signature is `Iter erase(Iter)`) did NOT move it to the argument area;
//    MSVC kept the return buffer in the frame at 0x10 (72.9 percent, 626 bytes).
//  - declaring `LiveEntry* ve = &it1.ptr->entry;` reproduces the original's
//    single `lea esi,[edx+0xc]`, but the named local grows the frame by 8 and
//    drops the score to 53.1 percent. The original keeps ve in esi with no slot.
//  - the original schedules `and edi,0xfffff000` between the DAT_005289f0
//    subtraction and the `if (blk == 0)` test. Moving the `base` declaration
//    before that test makes MSVC copy p into esi in the prologue and drops to
//    15.9 percent.
//
// The game's free() for its own heap: under the allocator lock it looks the
// block up in the live-block map, records the freed header in the debug arena,
// drops it from the live map, releases the pages it had reserved for the block
// and finally merges the released range into the free-block map with its two
// neighbours. Same std::map idiom as 0x4db450 and 0x4db000.
//
// NOT MATCHING: 69.2 percent, and the byte count is 13 too high (637 against
// 624). The whole remaining difference is the frame. The original's locals
// occupy a 0x4c frame, this file allocates 0x50, and every single frame
// reference in the function is therefore off by exactly 4, which is what all
// seven diff hunks are. Nothing in the body is structurally missing; fix the
// frame and most of the rest should follow.
//
// What the original's frame is made of, from ctx.py: 19 dwords of locals, 2
// dwords of stack arguments, frame pointer present, so `sub esp, 0x4c` and the
// four pushes, and the parameter `p` is read back at [esp+0x60] and the second
// argument at [esp+0x64]. The slots that are actually referenced are 0x10, 0x14,
// 0x18, 0x1c, 0x20, 0x24, 0x28, 0x2c, 0x30, 0x40, plus 0x60/0x64/0x68, and two
// facts about the ends of that range are load-bearing:
//  - 0x60 is `p`'s own stack home, and the original REUSES it for the `node`
//    local once `p` is dead. `lea edx, [esp + 0x60]` at 0x4db8d4 is the
//    out-parameter for the FUN_004dc910 erase call, and the same slot is
//    compared and rewritten at 0x4db967, 0x4db97a and 0x4db993. So the original
//    does not have a separate slot for `node`; it recycles the dead parameter.
//  - 0x24 holds `cs` early on (`mov [esp+0x24], esi` right before
//    EnterCriticalSection) and is the low half of the free-block `Pair` later
//    (`lea ecx, [esp + 0x24]` as the by-reference key argument, and
//    `add edi, edx` against [esp+0x28] as the length). Two different variables,
//    one slot, because `cs` is dead by then.
//
// The problem is that this file does not reproduce either recycling, and there
// is no plain C++ spelling found here that does. What was proved by compiling
// micro-tests: an address-taken local is always reloaded from its slot after a
// call, so it cannot be folded into a register the way the original's `node`
// appears to be, and a dead parameter's slot is only recycled when the local is
// declared inside the guarded block. The original needs the pointer to be a
// live register (edi) while a DIFFERENT, address-taken variable shares the
// parameter's stack slot. That combination is what is missing.
//
// One thing NOT to repeat: I tried shrinking Class_004d8820 from 0x30 bytes to
// 0xc, on the reading that it sits at the top of the 0x4c frame. It drops the
// frame to 0x2c and leaves the score and the byte count unchanged (69.2% and
// 637), which is the tell that the reading was wrong: a 0x30-byte object still
// fits if it starts lower in the frame. Any offset arithmetic on this function
// has to model the per-callee stack cleanup, because the constructor at
// 0x4d8820 is called with five pushed arguments that are live across the
// following instructions, and a script that tracks only push/pop gets the
// base-relative offsets wrong by the size of those arguments. The harness in
// build/scratch/0x4db7d0/ (cmp.py, score.py) does model the conventions and
// resolves [esp+X] to frame offsets; use that one, not a hand-rolled tracker.
//
// What did work, and is kept: the third VirtualFree argument is MEM_DECOMMIT
// (0x4000), not MEM_RELEASE; discarding the operator-- result (writing
// `it.FUN_004dbe10(0);` and not `it3 = it.FUN_004dbe10(0)`) drops the MSVC
// temporary so the dead result lands in the dead `it3` slot, as the original
// does; giving the iterator an EMPTY default constructor rather than
// `: ptr(0)` drops three zeroing stores and 13 bytes; hoisting
// `LiveEntry* ve = &it1.ptr->entry;` reproduces the single `lea esi,[edx+0xc]`
// before the arena branch instead of re-deriving it in each arm; and a single
// `unsigned int base` feeding both VirtualFree and the pair's offset gives the
// in-place `and edi, 0xfffff000` and removes an extra copy.
#include <windows.h>
#include <memory>

extern unsigned int DAT_005289f0;

// ---- the live-block map (FUN_004da8d0) -------------------------------------

// The map's value_type as the debug arena stores it: the tree node's _Color
// followed by the key and the rest of the value, 0x30 bytes in all.
struct LiveEntry {
    unsigned int color;                // +0x0
    unsigned int key;                  // +0x4
    char unknown_8[0x28];              // +0x8
};

struct LiveNode {
    LiveNode* left;                    // +0x0
    LiveNode* parent;                  // +0x4
    LiveNode* right;                   // +0x8
    LiveEntry entry;                   // +0xc
};

class Iter_004dce00 {
public:
    LiveNode* ptr;

    Iter_004dce00() {}
    Iter_004dce00(LiveNode* q) : ptr(q) {}
    bool operator==(const Iter_004dce00& o) const { return ptr == o.ptr; }
};

class Class_004dce00 {
public:
    char unknown_0[4];
    LiveNode* head;                    // +0x4

    Iter_004dce00 End() { return Iter_004dce00(head); }
    Iter_004dce00 FUN_004dce00(const unsigned int& key);
};

// The per-block header the allocator builds; its first dword is the map's key.
class Class_004d8820 {
public:
    unsigned int key;                  // +0x0
    unsigned int field_4;              // +0x4
    unsigned int field_8;              // +0x8
    char unknown_c[0x20];              // +0xc
    unsigned int field_2c;             // +0x2c

    Class_004d8820(void* a, unsigned int b, unsigned int c, unsigned int d,
                   unsigned int e);
};

// ---- the debug arena that records every freed header (FUN_004da9f0) --------

template <class T, class A = std::allocator<T> >
class Container_004da9f0 {
public:
    A allocator;                       // +0x0
    LiveEntry* field_4;                // +0x4
    unsigned int field_8;              // +0x8
    char unknown_c[4];
    unsigned int field_10;             // +0x10
};

Container_004da9f0<int>* FUN_004da9f0();

class Class_004dd8c0 {
public:
    void FUN_004dd8c0(unsigned int a, int b, LiveEntry* c);
};

// ---- the allocator's free-block map (FUN_004db610) --------------------------

struct Node_004db450 {
    Node_004db450* left;               // +0x0
    Node_004db450* parent;             // +0x4
    Node_004db450* right;              // +0x8
    unsigned int key;                  // +0xc
    int length;                        // +0x10
    int color;                         // +0x14
};

// The map's value_type: the block's base address and its length.
struct Pair_004db450 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

// The map's iterator; FUN_004dbe10 is its operator--(int).
class Class_004dbe10 {
public:
    Node_004db450* ptr;

    Class_004dbe10() {}
    Class_004dbe10(Node_004db450* q) : ptr(q) {}
    bool operator==(const Class_004dbe10& o) const { return ptr == o.ptr; }

    Class_004dbe10 FUN_004dbe10(int);
};

class Class_004db450 {
public:
    char unknown_0[4];
    Node_004db450* head;               // +0x4
    char unknown_8[8];
    int total;                         // +0x10

    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dbe10 a, Class_004dbe10 b) { return !(a == b); }
};

class Class_004dbd20 {
public:
    void FUN_004dbd20(Class_004dbe10* out, const unsigned int& kv);
};

class Class_004dbeb0 {
public:
    void FUN_004dbeb0(Class_004dbe10* out);
};

class Class_004dbd00 {
public:
    void FUN_004dbd00(Class_004dbe10* out, void* node);
};

class Class_004dbbc0 {
public:
    void FUN_004dbbc0(Class_004dbe10* out, Pair_004db450* p);
};

class Class_004dc910 {
public:
    void FUN_004dc910(Class_004dbe10* out, void* node);
};

LPCRITICAL_SECTION FUN_004da780();
Class_004dce00* FUN_004da8d0();
Class_004db450* FUN_004db610();
char FUN_004db760();
int FUN_004db7c0();
void __cdecl FUN_004d8310(void* p, int pattern, unsigned int size);
void __cdecl FUN_004da840(int param_1);
unsigned int __cdecl FUN_004da8c0(int param_1);
int __cdecl FUN_004da8a0(int param_1);

// FUNCTION: 0x4db7d0
void __cdecl FUN_004db7d0(void* p, int flags)
{
    if (p) {
        LPCRITICAL_SECTION cs = FUN_004da780();
        EnterCriticalSection(cs);
        Class_004dce00* live = (Class_004dce00*)FUN_004da8d0();
        Class_004d8820 hdr(p, 0, 0, 0, 0);
        Iter_004dce00 it1 = live->FUN_004dce00(hdr.key);
        if (it1 == ((Class_004dce00*)FUN_004da8d0())->End()) {
            LeaveCriticalSection(cs);
            return;
        }
        unsigned int blk = it1.ptr->entry.key;
        unsigned int off = (0 - (blk & 0xfff)) & 0xfff;
        if (FUN_004db760())
            FUN_004d8310((char*)p - off, FUN_004db7c0(), off);
        else
            FUN_004d8310((char*)p + blk, FUN_004db7c0(), off);
        Container_004da9f0<int>* arena = FUN_004da9f0();
        if (arena->field_10 < 0x2000) {
            ((Class_004dd8c0*)arena)->FUN_004dd8c0(arena->field_8, 1,
                                                  &it1.ptr->entry);
        } else {
            arena->field_4[arena->field_10 & 0x1fff] = it1.ptr->entry;
        }
        arena->field_10++;
        Class_004dbe10 node;
        ((Class_004dc910*)FUN_004da8d0())->FUN_004dc910(&node, it1.ptr);
        FUN_004da840(blk);
        DAT_005289f0 -= (blk + 0xfff) & 0xfffff000;
        if (blk == 0)
            blk = 1;
        unsigned int base = (unsigned int)p & 0xfffff000;
        VirtualFree((void*)base, FUN_004da8c0(blk), MEM_DECOMMIT);
        Pair_004db450 pair;
        pair.length = FUN_004da8a0(blk);
        Class_004db450* alloc = (Class_004db450*)FUN_004db610();
        pair.offset = base;
        Class_004dbe10 n;
        ((Class_004dbd20*)alloc)->FUN_004dbd20(&n, pair.offset);
        Class_004dbe10 it3;
        ((Class_004dbeb0*)alloc)->FUN_004dbeb0(&it3);
        if (node == it3)
            node.ptr = alloc->head;
        else
            node.FUN_004dbe10(0);
        if (alloc->Neq(n, Class_004dbe10(alloc->head))) {
            if (n.ptr->key == pair.offset + pair.length) {
                pair.length = pair.length + n.ptr->length;
                ((Class_004dbd00*)alloc)->FUN_004dbd00(&it3, n.ptr);
            }
        }
        if (alloc->Neq(node, Class_004dbe10(alloc->head))) {
            if (node.ptr->key + node.ptr->length == pair.offset) {
                pair.length = pair.length + node.ptr->length;
                pair.offset = node.ptr->key;
                ((Class_004dbd00*)alloc)->FUN_004dbd00(&it3, node.ptr);
            }
        }
        ((Class_004dbbc0*)alloc)->FUN_004dbbc0(&it3, &pair);
        LeaveCriticalSection(cs);
    }
}
