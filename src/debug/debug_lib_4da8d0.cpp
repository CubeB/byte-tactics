// Decompiled by deepseek-v4.1, edited by deepseek-v4.1, finished by Sonnet 5.5. Names are provisional.
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
    void* operator new(unsigned int n) { return GlobalAlloc(0, n); }
    Tree_004da8d0(const char& a, const char& b);
    void Init();
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

void Tree_004da8d0::Init()
{
    std::_Lockit lock;
    if (DAT_00528a50 == 0) {
        Node_004da8d0* nil =
            (Node_004da8d0*)((Class_004dddf0*)this)->FUN_004dddf0(0x40);
        nil->parent = 0;
        nil->color = 1;
        DAT_00528a50 = nil;
        nil->left = 0;
        DAT_00528a50->right = 0;
    }
    Node_004da8d0* nil = (Node_004da8d0*)DAT_00528a50;
    DAT_00528a4c++;
    Node_004da8d0* node = AllocNode();
    node->color = 0;
    node->parent = nil;
    head = node;
    size = 0;
    node->left = node;
    head->right = head;
}

inline Tree_004da8d0::Tree_004da8d0(const char& a, const char& b)
{
    field_0 = a;
    field_1 = b;
    multi = 0;
    Init();
}

// FUNCTION: 0x4da8d0
void* GetBlockMap()
{
    if (DAT_00528a44 == 0) {
        char s0, s1;
        DAT_00528a44 = new Tree_004da8d0(s0, s1);
    }
    return DAT_00528a44;
}