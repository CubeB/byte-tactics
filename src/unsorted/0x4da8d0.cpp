// Decompiled by deepseek-v4.1, edited by deepseek-v4.1. Names are provisional.
// check.py: 95.8%, 285 bytes against the original 283. The only difference left
// is which side of the store the return value is copied on, in both tails:
// the original writes `mov eax,edi / mov [0x528a44],eax` (success) and
// `xor eax,eax / mov [0x528a44],eax` (allocation failed), i.e. the value is
// materialised in eax first and the store uses the 5-byte eax form; ours
// writes `mov [0x528a44],edi / mov eax,edi` and `mov [0x528a44],ebx /
// xor eax,eax`, one byte longer per tail. Every spelling tried compiles to the
// latter: plain `g = p; return p;`, `return g = p;`, chained `g = q = p;`,
// a block-scoped q, a `static __inline` storer that assigns and returns, and
// a typed Tree* global instead of void*.
// Newer attempts: a literal `0` and `(void*)0` on the right of the failure
// tail still store the known-zero ebx (`mov [g],ebx / xor eax,eax`), and an
// inlined identity helper (`return v;`) is folded away leaving no copy to eax,
// so the store stays `mov [g],edi / mov eax,edi`. What the original needs is a
// value that reaches eax before the store (A3 form), which no plain assignment
// spelling has produced; an expression whose value is materialised in the
// result register (an inlined non-trivial return, or an indirect store) is the
// remaining lead.
// Lazily built singleton for the game's file-record map (the std::_Tree whose
// insert is 0x4dc680 and whose erase is 0x4dc910): allocate the 0x10-byte tree
// object, make the _Nil node DAT_00528a50 (black, self-null children) and the
// head node (red, parent _Nil, both children itself), both carved from the
// pooled free list at DAT_005289e0.
#include <windows.h>
#include <yvals.h>

extern void* DAT_005289e0;             // free list of 0x40-byte nodes
extern void (*DAT_005289bc)();         // out-of-memory handler
extern int DAT_00528a4c;               // bumped on every node carved here
extern void* DAT_00528a44;             // the tree singleton

struct Node_004da8d0 {
    Node_004da8d0* left;               // +0x0
    Node_004da8d0* parent;             // +0x4
    Node_004da8d0* right;              // +0x8
    char value[0x30];                  // +0xc
    int color;                         // +0x3c (0 = red)
};

extern Node_004da8d0* DAT_00528a50;    // the tree's _Nil node

// The pool allocator that sits at +0 of the tree; its method ignores `this`.
class Class_004dddf0 {
public:
    void* FUN_004dddf0(unsigned int n);
};

class Tree_004da8d0 {
public:
    char field_0;                      // +0x0
    char field_1;                      // +0x1
    Node_004da8d0* head;               // +0x4
    unsigned char multi;               // +0x8
    int size;                          // +0xc
};


static inline Node_004da8d0* AllocNode()
{
    if (DAT_005289e0 == 0) {
        Node_004da8d0* block;
        do {
            block = (Node_004da8d0*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        Node_004da8d0* head = (Node_004da8d0*)DAT_005289e0;
        for (int i = 0; i < 0x80; i++) {
            block->left = head;
            head = block;
            block++;
        }
        DAT_005289e0 = head;
    }
    Node_004da8d0* node = (Node_004da8d0*)DAT_005289e0;
    DAT_005289e0 = node->left;
    return node;
}

// FUNCTION: 0x4da8d0
void* FUN_004da8d0()
{
    Tree_004da8d0* p = (Tree_004da8d0*)DAT_00528a44;
    if (p != 0) {
        return p;
    }
    p = (Tree_004da8d0*)GlobalAlloc(0, 0x10);
    if (p != 0) {
        {
            char seed0;
            p->field_0 = seed0;
        }
        {
            char seed1;
            p->field_1 = seed1;
        }
        p->multi = 0;
        {
            std::_Lockit lock;
            if (DAT_00528a50 == 0) {
                Node_004da8d0* nil =
                    (Node_004da8d0*)((Class_004dddf0*)p)->FUN_004dddf0(0x40);
                nil->parent = 0;
                nil->color = 1;
                DAT_00528a50 = nil;
                nil->left = 0;
                ((Node_004da8d0*)DAT_00528a50)->right = 0;
            }
            Node_004da8d0* nil = (Node_004da8d0*)DAT_00528a50;
            DAT_00528a4c++;
            Node_004da8d0* node = AllocNode();
            node->color = 0;
            node->parent = nil;
            p->head = node;
            p->size = 0;
            node->left = node;
            p->head->right = p->head;
        }
        DAT_00528a44 = p;
        return p;
    }
    DAT_00528a44 = p;
    return p;
}
