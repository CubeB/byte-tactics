// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// std::vector<std::vector<Elem_00434020> >::~vector(): each inner vector's
// elements are destroyed through allocator::destroy (empty for a trivial
// element), then its _First is freed and its three pointers zeroed; finally
// the outer _First is freed and zeroed.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

typedef std::vector<Elem_00434020> Inner_00434020;
typedef std::vector<Inner_00434020> Outer_00434020;
typedef void (std::allocator<Elem_00434020>::*DestroyFn_00434020)(Elem_00434020*);

// Taking this address makes allocator::destroy get emitted out of line.
DestroyFn_00434020 g_destroy_00434020 = &std::allocator<Elem_00434020>::destroy;

class Class_00433540 {
public:
    void FUN_00433540();
};

// FUNCTION: 0x433540
void Class_00433540::FUN_00433540()
{
    // Explicit destructor call from a method, as in 0x4330b0.cpp.
    ((Outer_00434020*)this)->~vector();
}