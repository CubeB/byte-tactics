// Decompiled by Opus. Names are provisional.
// A global vector-shaped object (an empty allocator byte, then three zeroed
// pointers): the compiler generates its initialiser (0x4814c0) and the
// destructor it registers with atexit (0x4814f0, see 0x4814f0.cpp), which
// calls the out-of-line destructor body FUN_004330b0.
#include <memory>

class LosTables {
public:
    explicit LosTables(const std::allocator<int>& al = std::allocator<int>())
        : allocator(al), first(0), last(0), end(0) {}
    ~LosTables() { FUN_004330b0(); }

    void FUN_004330b0();

    std::allocator<int> allocator;     // +0x0
    int* first;                        // +0x4
    int* last;                         // +0x8
    int* end;                          // +0xc
};

// FUNCTION: 0x4814c0 _$E4
LosTables g_losTables;
