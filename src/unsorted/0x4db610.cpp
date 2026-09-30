// Decompiled by deepseek-v4.1. Names are provisional.
// PARTIAL, 90.6% (deepseek-v4.1, 15 check.py runs). 334 bytes of 332.
// The allocator singleton constructor: builds the free-block map (the same
// Class_004db000 red-black tree that 0x4db000/0x4dce60 maintain) and reserves
// address space for it. DAT_00528a40 is the singleton pointer, DAT_00528a54
// the tree's shared _Nil node, DAT_00528a58 a live-tree counter. It returns
// the singleton, which is why the value flows through eax on both exits.
//
// What still differs from the original, in order of appearance:
//   * frame: original `sub esp,0xc`, ours `sub esp,8`. The original has three
//     slots: the uninitialised byte local at frame+3, the outer loop counter
//     at frame+4 and the std::_Lockit at frame+8. MSVC merges our counter into
//     the lock's slot (their live ranges look disjoint: the lock dies at
//     0x4db6b2 and the counter's only store is at 0x4db6c0), so our lock sits
//     at [esp+0x14] instead of [esp+0x18] and every lea for it differs.
//     Dropping the braces around the lock block, or moving the closing brace to
//     after the reservation loop, DOES give frame 0xc with the lock at +8 (and
//     every other offset right), but then MSVC calls the destructor after the
//     loop instead of at 0x4db6b2, and the score drops to 85.8%. Declaring the
//     counter before the lock block (or before the byte local, or at function
//     scope) changes nothing; neither does wrapping the lock in an aggregate
//     struct, nor a for loop instead of do/while.
//   * the two byte loads: original `mov cl,[esp+0x13]` then `mov al,[esp+0x13]`,
//     ours the other way round. The local must be `volatile unsigned char`
//     (uninitialised) for MSVC to reload it twice at all: without volatile we
//     get a single load and the score is 81.5%. With volatile the stores come
//     out right (`mov [ebp+1],cl` then `mov [ebp],al`) only when the source
//     order is cmp_a first, and only the two loads stay swapped. This looks
//     like the original really did declare the byte volatile (every repeated
//     read re-loads it); flag it for review if you disagree.
//   * both exit blocks store before loading the return register: original
//     `mov eax,ebp` / `mov [DAT_00528a40],eax` and `xor eax,eax` /
//     `mov [DAT_00528a40],eax`, ours `mov [DAT_00528a40],ebp` / `mov eax,ebp`
//     and `mov [DAT_00528a40],ebx` / `xor eax,eax`. Splitting the assignment
//     from the return (`DAT_00528a40 = p; return p;`) does not change it.
// Tried and inert (all 90.6%): counter declared first in the block, after the
// byte stores, at function scope; lock wrapped in a one-member struct; for
// (n = 4; n != 0; --n); DAT_00528a40 = p; return p;.
#include <windows.h>
#include <yvals.h>

struct Node_004db610 {
    Node_004db610* left;
    Node_004db610* parent;
    Node_004db610* right;
    unsigned int key;
    int length;
    int color;
};

struct Pair_004db610 {
    unsigned int offset;
    int length;
};

class Class_004ddd70 {
public:
    void* FUN_004ddd70(unsigned int n);
};

class Class_004db000 {
public:
    unsigned char cmp_a;
    unsigned char cmp_b;
    char unknown_2[2];
    Node_004db610* head;
    unsigned char rebuild;
    char unknown_9[3];
    int size;
    int total;

    void FUN_004db000(Pair_004db610 p);
};

extern Class_004db000* DAT_00528a40;
extern Node_004db610* DAT_00528a54;
extern int DAT_00528a58;

// FUNCTION: 0x4db610
Class_004db000* FUN_004db610()
{
    if (DAT_00528a40 != 0) {
        return DAT_00528a40;
    }
    Class_004db000* p = (Class_004db000*)GlobalAlloc(0, 0x14);
    if (p != 0) {
        volatile unsigned char c;
        p->cmp_a = c;
        p->cmp_b = c;
        p->rebuild = 0;
        {
            std::_Lockit lock;
            if (DAT_00528a54 == 0) {
                Node_004db610* n = (Node_004db610*)((Class_004ddd70*)p)->FUN_004ddd70(0x18);
                n->parent = 0;
                n->color = 1;
                DAT_00528a54 = n;
                n->left = 0;
                DAT_00528a54->right = 0;
            }
            Node_004db610* nil = DAT_00528a54;
            DAT_00528a58 = DAT_00528a58 + 1;
            Node_004db610* h = (Node_004db610*)((Class_004ddd70*)p)->FUN_004ddd70(0x18);
            h->parent = nil;
            h->color = 0;
            p->head = h;
            p->size = 0;
            p->head->left = p->head;
            p->head->right = p->head;
        }
        p->total = 0;
        int n = 4;
        do {
            unsigned int len = 0x10000000;
            void* m = VirtualAlloc(0, len + 0x2000, MEM_RESERVE, PAGE_READWRITE);
            for (;;) {
                if (m != 0 && (unsigned int)m + len <= 0x80000000u)
                    break;
                if (m != 0)
                    VirtualFree(m, len + 0x2000, MEM_RELEASE);
                len = (len >> 1) & 0x7fffe000;
                if (len < 0x10000)
                    goto next;
                m = VirtualAlloc(0, len + 0x2000, MEM_RESERVE, PAGE_READWRITE);
            }
            p->total = p->total + len;
            Pair_004db610 pair;
            pair.offset = (unsigned int)m;
            pair.length = len;
            p->FUN_004db000(pair);
        next:
            ;
        } while (--n);
        return DAT_00528a40 = p;
    }
    return DAT_00528a40 = 0;
}
