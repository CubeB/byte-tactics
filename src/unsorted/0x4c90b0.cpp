// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Append of the reference-counted string handle: concatenates other's
// characters onto this handle's, allocating a new block whose first int is
// the reference count, then releasing the old block. The handle points at the
// characters; the count is the int just before them (see 0x4c91a0, 0x4c9290,
// 0x4c93b0, 0x4c93f0). The class is named after this address because it has
// no name in data/symbols.csv.
//
// Still differs from the original (best 91.4%):
//  - the original materialises the emptiness test as a bool first
//    (xor ecx,ecx; cmp byte ptr [esi],0; sete cl; test cl,cl; jne). A bool
//    local or an inline IsEmpty() reproduces those three instructions but
//    makes MSVC choose a different register schedule for the two string
//    copies (86.1%), so the direct test below scores higher.
//  - the original computes the allocation size as
//    lea eax,[ecx+edx+1]; add eax,5 (two additions), i.e. n + m + 1 + 5;
//    every source spelling of that sum folds to one lea with displacement 6
//    in this toolchain. A live `len` variable reproduces the split but spills
//    extra locals. This is probably compiler state from the file's earlier
//    functions (0x4c8760, 0x4c8bb0).
//  - the destination address comes out lea edx,[ebp+eax] where the original
//    has lea edx,[eax+ebp] (same address, swapped base/index encoding).
#include <string.h>
#include <stdlib.h>

class Class_004c90b0 {
public:
    char* ptr;              // refcount lives in the dword before ptr

    Class_004c90b0* FUN_004c90b0(const Class_004c90b0& other);
};

// FUNCTION: 0x4c90b0
Class_004c90b0* Class_004c90b0::FUN_004c90b0(const Class_004c90b0& other)
{
    if (*other.ptr != 0) {
        int n = (int)strlen(ptr);
        int m = (int)strlen(other.ptr);
        int* block = (int*)malloc(n + m + 1 + 5);
        *block = 1;
        char* chars = (char*)(block + 1);
        strcpy(chars, ptr);
        strcpy(chars + n, other.ptr);
        (*(int*)(ptr - 4))--;
        if (*(int*)(ptr - 4) == 0) {
            free(ptr - 4);
        }
        ptr = chars;
    }
    return this;
}
