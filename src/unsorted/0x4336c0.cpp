// Decompiled by Opus. Names are provisional.
// std::vector<T>::~vector() from MSVC 5's <vector>, out of line, for a
// trivial element type: byte-identical to 0x433a30 (vector<Elem_00434020>),
// but a separate instantiation with no callers, so the element type is not
// known. Emitted the way 0x433a30.cpp emits its copy.
#include <vector>

struct Elem_004336c0 {
    int value;                         // +0x0
};

typedef std::vector<Elem_004336c0> Inner_004336c0;
typedef std::vector<Inner_004336c0> Outer_004336c0;
typedef Outer_004336c0& (Outer_004336c0::*AssignFn_004336c0)(const Outer_004336c0&);

AssignFn_004336c0 g_assign_004336c0 = &Outer_004336c0::operator=;

// FUNCTION: 0x4336c0 ??1?$vector@UElem_004336c0@@V?$allocator@UElem_004336c0@@@std@@@std@@QAE@XZ
