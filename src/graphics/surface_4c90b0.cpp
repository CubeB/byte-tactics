// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free (third pass), edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by GPT-6.1-sol, finished by claude-sonnet-5-5. Names are provisional.
// Append of the reference-counted string handle: concatenates other's
// characters onto this handle's, allocating a new block whose first int is
// the reference count, then releasing the old block. The handle points at the
// characters; the count is the int just before them (see 0x4c91a0, 0x4c9290,
// 0x4c93b0, 0x4c93f0). The class is named after this address because it has
// no name in data/symbols.csv.
#include <string.h>
#include <stdlib.h>

class Class_004c90b0 {
public:
    char* ptr;              // refcount lives in the dword before ptr

    bool IsEmpty() const { return *ptr == 0; }

    Class_004c90b0* Append(const Class_004c90b0& other);
};

// FUNCTION: 0x4c90b0
Class_004c90b0* Class_004c90b0::Append(const Class_004c90b0& other)
{
    // Bool helper, not a plain pointer test: gives the original emptiness-test code.
    if (!other.IsEmpty()) {
        // Declared before the strlen locals: sets the second strcpy destination encoding.
        char* chars;
        int n = (int)strlen(ptr);
        int m = (int)strlen(other.ptr);
        int len = n + m + 1;
        int* lp = &len;                   // opaque store: keeps the +1 and +5 apart
        *lp += 5;
        int* block = (int*)malloc(len);
        *block = 1;
        chars = (char*)(block + 1);
        strcpy(chars, ptr);
        strcpy(chars + n, other.ptr);
        ((int*)ptr)[-1]--;
        int* old = (int*)ptr - 1;
        if (((int*)ptr)[-1] == 0) {
            free(old);
        }
        ptr = chars;
    }
    return this;
}