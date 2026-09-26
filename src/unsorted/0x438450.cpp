// Decompiled by Opus. Names are provisional.
// A global vector-like container: the compiler generates its initialiser and
// the destructor it registers with atexit (0x438480, see 0x438480.cpp, which
// explains why this is not a std::vector).
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

// FUNCTION: 0x438450 _$E4
Vector_00438480<int> DAT_00512340;
