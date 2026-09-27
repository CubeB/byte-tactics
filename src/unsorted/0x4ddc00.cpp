// Decompiled by Opus. Names are provisional.
#include <windows.h>

// 0x18-byte pooled node, allocated 0x155 at a time (one 0x2000 block).
struct Node_004ddc00 {
    Node_004ddc00* next;               // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x14 - 0x8];
    int field_14;                      // +0x14
};

extern Node_004ddc00* DAT_00528a10;    // free list
extern void (*DAT_005289bc)();         // out-of-memory handler

static inline Node_004ddc00* AllocNode()
{
    if (DAT_00528a10 == 0) {
        Node_004ddc00* block;
        do {
            block = (Node_004ddc00*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        Node_004ddc00* head = DAT_00528a10;
        for (int i = 0; i < 0x155; i++) {
            block->next = head;
            head = block;
            block++;
        }
        DAT_00528a10 = head;
    }
    Node_004ddc00* node = DAT_00528a10;
    DAT_00528a10 = node->next;
    return node;
}

// FUNCTION: 0x4ddc00
Node_004ddc00* __stdcall FUN_004ddc00(int param_1, int param_2)
{
    Node_004ddc00* node = AllocNode();
    node->field_4 = param_1;
    node->field_14 = param_2;
    return node;
}
