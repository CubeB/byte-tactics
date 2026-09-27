// Decompiled by Opus. Names are provisional.
// Relocates a tree of nodes loaded from a file: adds `delta` to each
// (non-null) stored pointer of the node, its element array and, recursively,
// its sibling and child nodes, then hands the node to FUN_004cb370.
// Any one of the common headers is needed for the `lea` on ptr_20 (with none,
// MSVC emits `add`; tools/headers.py).
#include <windows.h>

struct Elem_004cb4c0 {                 // 0x20 bytes
    char unknown_0[8];
    int ptr_8;                         // +0x8
    int ptr_c;                         // +0xc
    int ptr_10;                        // +0x10
    char unknown_14[0xc];
};

struct Node_004cb4c0 {
    char unknown_0[8];
    int count;                         // +0x8
    char unknown_c[0x1c - 0xc];
    int ptr_1c;                        // +0x1c
    int ptr_20;                        // +0x20
    int ptr_24;                        // +0x24
    Elem_004cb4c0* elems;              // +0x28
    Node_004cb4c0* next;               // +0x2c
    Node_004cb4c0* child;              // +0x30
};

void __stdcall FUN_004cb370(Node_004cb4c0* node);
void __stdcall FUN_004cb4c0(int delta, Node_004cb4c0* node);

// The loop only matches with a pointer walking the element array.
// FUNCTION: 0x4cb4c0
void __stdcall FUN_004cb4c0(int delta, Node_004cb4c0* node)
{
    if (node->ptr_1c)
        node->ptr_1c += delta;
    if (node->ptr_20)
        node->ptr_20 += delta;
    node->ptr_24 += delta;
    node->elems = (Elem_004cb4c0*)((char*)node->elems + delta);
    if (node->next) {
        node->next = (Node_004cb4c0*)((char*)node->next + delta);
        FUN_004cb4c0(delta, node->next);
    }
    if (node->child) {
        node->child = (Node_004cb4c0*)((char*)node->child + delta);
        FUN_004cb4c0(delta, node->child);
    }
    Elem_004cb4c0* e = node->elems;
    for (int i = 0; i < node->count; i++, e++) {
        if (e->ptr_8)
            e->ptr_8 += delta;
        e->ptr_c += delta;
        if (e->ptr_10)
            e->ptr_10 += delta;
    }
    FUN_004cb370(node);
}
