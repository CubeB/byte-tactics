// Decompiled by Opus. Names are provisional.
#include <windows.h>

// 0x40-byte pooled node, allocated 0x80 at a time.
struct Node_004ddce0 {
    Node_004ddce0* next;               // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x3c - 0x8];
    int field_3c;                      // +0x3c
};

extern Node_004ddce0* DAT_005289e0;    // free list
extern void (*DAT_005289bc)();         // out-of-memory handler

static inline Node_004ddce0* AllocNode()
{
    if (DAT_005289e0 == 0) {
        Node_004ddce0* block;
        do {
            block = (Node_004ddce0*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        Node_004ddce0* head = DAT_005289e0;
        for (int i = 0; i < 0x80; i++) {
            block->next = head;
            head = block;
            block++;
        }
        DAT_005289e0 = head;
    }
    Node_004ddce0* node = DAT_005289e0;
    DAT_005289e0 = node->next;
    return node;
}

// A method that ignores `this`: its caller (0x4dc680, an inlined tree insert
// after its std::_Lockit) sets ecx to the tree. Shaped like
// std::_Tree<...>::_Buynode(parent, colour) with a pooled allocator.
class Class_004ddce0 {
public:
    Node_004ddce0* FUN_004ddce0(int param_1, int param_2);
};

// FUNCTION: 0x4ddce0
Node_004ddce0* Class_004ddce0::FUN_004ddce0(int param_1, int param_2)
{
    Node_004ddce0* node = AllocNode();
    node->field_4 = param_1;
    node->field_3c = param_2;
    return node;
}
