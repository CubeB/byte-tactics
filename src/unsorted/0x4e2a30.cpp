// Decompiled by Opus. Names are provisional.
// Shaped like std::_Tree<...>::_Buynode(_Nodeptr, _Redbl) from MSVC 5's
// <xtree> (parent at +4, colour at +0x204) with a pooled allocator inlined:
// DAT_00529e58 is the pool's free list, refilled 0x2000 bytes at a time with
// the generic "carve n-byte pieces" loop (the out-of-line copy is 0x4e2b60).
#include <windows.h>

struct Data1 {
    int field_0;
};

struct Data2 {
    char field_0;
};

class Class_004e2a10 {
public:
    int field_0;
    char field_4;

    Class_004e2a10* FUN_004e2a10(const Data1* param_1, const Data2* param_2);
};

// The function just before this one in the original file.
// With no function compiled before it, MSVC copies the
// node-field stores into the out-of-memory path instead of jumping to them.
// FUNCTION: 0x4e2a10
Class_004e2a10* Class_004e2a10::FUN_004e2a10(const Data1* param_1, const Data2* param_2)
{
    field_0 = param_1->field_0;
    field_4 = param_2->field_0;
    return this;
}

struct Node_004e2a30 {
    Node_004e2a30* next;               // +0x0
    int field_4;                       // +0x4
    char unknown_8[0x204 - 0x8];
    int field_204;                     // +0x204
};

extern void* DAT_00529e58;             // free list
extern void (*DAT_005289bc)();         // out-of-memory handler

static inline void* PoolAlloc(unsigned int n)
{
    if (DAT_00529e58 == 0) {
        char* block;
        do {
            block = (char*)GlobalAlloc(0, 0x2000);
            if (block == 0 && DAT_005289bc != 0) {
                DAT_005289bc();
            }
        } while (block == 0 && DAT_005289bc != 0);
        if (block == 0) {
            return 0;
        }
        for (unsigned int rem = 0x2000; rem >= n; rem -= n) {
            *(void**)block = DAT_00529e58;
            DAT_00529e58 = block;
            block += n;
        }
    }
    void* p = DAT_00529e58;
    DAT_00529e58 = *(void**)p;
    return p;
}

// A method that ignores `this`: its caller (0x4e2250, an inlined tree insert
// after its std::_Lockit) sets ecx to the tree. Shaped like
// std::_Tree<...>::_Buynode(parent, colour) with a pooled allocator.
class Class_004e2a30 {
public:
    Node_004e2a30* FUN_004e2a30(int param_1, int param_2);
};

// FUNCTION: 0x4e2a30
Node_004e2a30* Class_004e2a30::FUN_004e2a30(int param_1, int param_2)
{
    Node_004e2a30* node = (Node_004e2a30*)PoolAlloc(sizeof(Node_004e2a30));
    node->field_4 = param_1;
    node->field_204 = param_2;
    return node;
}
