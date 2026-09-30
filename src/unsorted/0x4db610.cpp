// Decompiled by deepseek-v4.1. Names are provisional.
// The allocator singleton constructor: builds the free-block map (a red-black
// tree, the same Class_004db000 tree that 0x4db000/0x4dce60 maintain) and
// reserves address space for it. DAT_00528a40 is the singleton pointer,
// DAT_00528a54 the tree's shared _Nil node, DAT_00528a58 a live-tree counter.
// It returns the singleton: the already-created path leaves eax holding
// DAT_00528a40, the failure path returns 0, and the end is
// `mov eax,ebp; mov [DAT_00528a40],eax`, which is why the function is not void.
//
// PARTIAL, 48.8%. Still differs from the original:
//   * frame: original `sub esp,0xc`, ours `sub esp,8`. The original has three
//     slots: the byte local at frame+3, the outer loop counter at frame+4 and
//     the std::_Lockit at frame+8. MSVC merges our counter into the lock's
//     slot (their live ranges look disjoint), so the lock is at [esp+0x14]
//     instead of [esp+0x18]. Declaring the counter before the lock block does
//     not help; the compiler sinks its initialising store past the lock's
//     destructor anyway.
//   * the one-byte local read twice (`mov cl,[esp+0x13]` and
//     `mov al,[esp+0x13]`, then stores to [ebp+1] and [ebp]) is a byte MSVC
//     places in the top byte of a slot. Ours loads it once and reuses al
//     because MSVC proves the object store cannot alias the local.
//   * the zero constant lives in ebx in the original (so `xor ebx,ebx`,
//     `push ebx`, `cmp eax,ebx`, `mov byte ptr [ebp+8],bl`); ours colours it
//     edi and uses `test eax,eax` and immediate stores instead, with the
//     VirtualAlloc import pointer in ebx rather than edi.
//   * the first VirtualAlloc size is the folded immediate 0x10002000 in the
//     original; ours computes it into a register (`lea edi,[esi+0x2000]`).
//   * the p == 0 path shares the original's tail (`xor eax,eax;
//     mov [DAT_00528a40],eax` then the common epilogue); ours emits its own
//     epilogue copy.
// Tried and rejected: a 4-byte local whose top byte is read (37.5%), swapping
// the two byte stores (no change), a `bool` at +0x8 (no change), MEM_COMMIT
// (worse, the original really passes MEM_RESERVE), and rewriting the
// reservation loop as the 0x4db1c0 for(;;) idiom (42.7%, 321 bytes).
// For the frame: `volatile unsigned char c;` does give the two loads (310
// bytes, 48.5%) but al and cl come out the other way round. Still 8 bytes of
// frame after `int n[1] = {4};`, `int& rn = n;`, `volatile int n = 4;` and
// `int n = 4;` as the first statement (the init store sinks past the lock's
// destructor).
#include <windows.h>
#include <yvals.h>

struct Node_004db610 {
    Node_004db610* left;            // +0x0
    Node_004db610* parent;          // +0x4
    Node_004db610* right;           // +0x8
    unsigned int key;               // +0xc
    int length;                     // +0x10
    int color;                      // +0x14
};

struct Pair_004db610 {
    unsigned int offset;            // +0x0
    int length;                     // +0x4
};

class Class_004ddd70 {
public:
    void* FUN_004ddd70(unsigned int n);
};

class Class_004db000 {
public:
    unsigned char cmp_a;            // +0x0
    unsigned char cmp_b;            // +0x1
    char unknown_2[2];
    Node_004db610* head;            // +0x4
    unsigned char rebuild;          // +0x8
    char unknown_9[3];
    int size;                       // +0xc
    int total;                      // +0x10

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
    if (p == 0) {
        return DAT_00528a40 = 0;
    }
    unsigned char c;
    p->cmp_b = c;
    p->cmp_a = c;
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

    p->total = 0;
    int n = 4;
    do {
        unsigned int len = 0x10000000;
        for (;;) {
            void* m = VirtualAlloc(0, len + 0x2000, MEM_RESERVE, PAGE_READWRITE);
            if (m == 0) {
                goto shrink;
            }
            if ((unsigned int)m + len > 0x80000000u) {
                if (m != 0) {
                    VirtualFree(m, len + 0x2000, MEM_RELEASE);
                }
                goto shrink;
            }
            p->total = p->total + len;
            Pair_004db610 pair;
            pair.offset = (unsigned int)m;
            pair.length = len;
            p->FUN_004db000(pair);
            break;
        shrink:
            len = (len >> 1) & 0x7fffe000;
            if (len < 0x10000) {
                break;
            }
        }
    } while (--n);
    return DAT_00528a40 = p;
}
