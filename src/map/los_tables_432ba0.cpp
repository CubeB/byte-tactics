// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00432be0>::~vector() (its size() is 0x432be0). Each element
// is a reference-counted string handle released by ReleaseRef. The original
// calls it on a local vector in 0x42a8d0.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct Elem_00432be0 {
    char* data;                        // +0x0

    ~Elem_00432be0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

typedef std::vector<Elem_00432be0> Inner_00432ba0;
typedef std::vector<Inner_00432ba0> Outer_00432ba0;
typedef Outer_00432ba0& (Outer_00432ba0::*AssignFn_00432ba0)(const Outer_00432ba0&);

// Taking this operator='s address makes the destructor get emitted out of line.
AssignFn_00432ba0 g_assign_00432ba0 = &Outer_00432ba0::operator=;

// FUNCTION: 0x432ba0 ??1?$vector@UElem_00432be0@@V?$allocator@UElem_00432be0@@@std@@@std@@QAE@XZ
