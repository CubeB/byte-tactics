// Decompiled by space-bunny-free. Names are provisional.
// 77.6 percent, 204 of 208 bytes. The whole gap is one extra dead store and
// the register it forces. The original does:
//     mov eax, [ebx + 4]      ; _First
//     push eax
//     mov [esp + 0x1c], eax    ; dead store into n's now-dead parameter slot
//     call <deallocate>
//     mov eax, [ebx + 4]      ; _First again, for the null test
//     lea edx, [edi + edi*2] ; end = buf + n * 48, computed in edx
// while this file pushes _First from edx, computes end in eax and only then
// reloads _First into eax for the test.
// The dead store is the guide's "locals in parameter slots" pattern: a pointer
// local declared after n is dead, which MSVC homes in n's slot. Two attempts
// here, both worse, recorded so they are not repeated:
//  - `Elem* old = first;` at the top of the function, live across both
//    allocator calls: 43.9 percent, and the size becomes 208, but MSVC spills
//    this to the stack (`sub esp, 8` plus `mov [esp + 0x10], esi`) and rebuilds
//    the frame from there.
//  - the same local declared immediately before the delete, which is the
//    literal reading of the dead store: 68.7 percent, 199 bytes, the store
//    still does not appear.
// The size matching at 208 in the first case is a coincidence, not a sign that
// the construct is right. The most promising next step is to decompile
// 0x4dd8c0, the sibling vector function with the same clobber at 0x4dda29,
// since its source shape should reveal what makes MSVC 5 allocate that frame
// temp.
#include <stddef.h>

struct Elem_00475770 {
    int dwords[12];                    // 0x30 bytes
};

void* __cdecl operator new(size_t size);
void __cdecl operator delete(void* p);

class Class_00475770 {
public:
    int unknown_0;                     // +0x0
    Elem_00475770* first;              // +0x4
    Elem_00475770* last;               // +0x8
    Elem_00475770* end;                // +0xc

    void FUN_00475770(int n);
};

// FUNCTION: 0x475770
void Class_00475770::FUN_00475770(int n)
{
    if ((unsigned)(first == 0 ? 0 : end - first) < (unsigned)n) {
        int num = n;
        if (num < 0) num = 0;
        Elem_00475770* buf = (Elem_00475770*)::operator new(num * 48);
        Elem_00475770* dst = buf;
        Elem_00475770* s = first;
        Elem_00475770* e = last;
        for (; s != e; ++s, ++dst) {
            if (dst) {
                *dst = *s;
            }
        }
        ::operator delete(first);
        end = buf + n;
        int count = first == 0 ? 0 : last - first;
        first = buf;
        last = buf + count;
    }
}
