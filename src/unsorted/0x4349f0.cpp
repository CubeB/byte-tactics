// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of Elem_00434360, the
// element of the vector whose operator= is 0x434770: a struct holding a
// std::vector<Elem_00434020> with an implicit destructor, so this inlines
// ~vector (free _First, whose trivial destroy loop leaves the dead store to
// the local slot, then zero the three pointers). MSVC only emits it where
// the inline depth runs out: the destroy loop (push 0; mov ecx, elem) in
// that operator=. Taking the operator='s address (unannotated) emits this
// COMDAT; the operator= then compiles byte-identical to 0x434770, which it
// does not when the element is a bare std::vector (598 bytes, not 586).
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Vec_004349f0;
typedef Vec_004349f0& (Vec_004349f0::*AssignFn_004349f0)(const Vec_004349f0&);

// FUNCTION: 0x4349f0 ??_GElem_00434360@@QAEPAXI@Z
AssignFn_004349f0 g_assign_004349f0 = &Vec_004349f0::operator=;
