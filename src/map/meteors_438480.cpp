// Decompiled by Opus. Names are provisional.
// The global at 0x512340 and the destructor the compiler registers for it with
// atexit (0x438450 is its initialiser, see 0x438450.cpp).
//
// It is laid out like std::vector (an empty allocator, then first/last/end),
// but it is not one: MSVC 5's ~vector runs an element-destroy loop, and for a
// trivial element type the emptied loop still leaves a dead store (and a
// `push ecx` to make room for it), as the game's own inlined ~vector<int> at
// 0x434400 shows. This destructor only frees the storage.
#include <memory>

template <class T, class A = std::allocator<T> >
class Vector_00438480 {
public:
    explicit Vector_00438480(const A& al = A())
        : allocator(al), first(0), last(0), end(0) {}
    ~Vector_00438480()
    {
        allocator.deallocate(first, end - first);
        first = 0, last = 0, end = 0;
    }

    A allocator;
    T* first;
    T* last;
    T* end;
};

// FUNCTION: 0x438480 _$E2
Vector_00438480<int> DAT_00512340;
