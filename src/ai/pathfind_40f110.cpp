// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Reallocates the element array (20-byte elements) and the parallel array of
// element pointers of a container, fixing up each pointer to the new block.
#include <stddef.h>
#include <string.h>

struct Elem_0040f110 {
    int field_0;
    int field_4;
    int field_8;
    int field_c;
    int field_10;
};

class Class_0040f110 {
public:
    Elem_0040f110* elems;              // +0x0
    Elem_0040f110** ptrs;              // +0x4
    int field_8;                       // +0x8
    int count;                         // +0xc
    int capacity;                      // +0x10
    int ptr_count;                     // +0x14

    void GrowNodes(int param_1);
};

// FUNCTION: 0x40f110
void Class_0040f110::GrowNodes(int param_1)
{
    int cap = capacity;
    if (param_1 < cap)
        param_1 = cap + (cap >> 1) + 0x10;
    Elem_0040f110* newe = (Elem_0040f110*)operator new(param_1 * 0x14);
    int i;
    for (i = 0; i < count; i++)
        *(newe + i) = elems[i];
    operator delete(elems);
    Elem_0040f110** newp = (Elem_0040f110**)operator new(param_1 * 4);
    for (i = 0; i < ptr_count; i++)
        newp[i] = newe + (ptrs[i] - elems);
    operator delete(ptrs);
    ptrs = newp;
    elems = newe;
    capacity = param_1;
}
