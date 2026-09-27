// Decompiled by Opus. Names are provisional.
// Elem_00434360::Elem_00434360(const Elem_00434360&), the implicit copy
// constructor of the struct holding one std::vector<Elem_00434020>: it
// inlines that vector's copy constructor from MSVC 5's <vector> (allocate
// size() elements and copy them with the placement-new construct). Both
// callers, vector<Elem_00434360>::insert (0x433db0) and operator=
// (0x434770), reference it under this name when they are compiled with the
// struct as the element; the vector's own copy constructor is inlined into
// it. Taking insert's address makes the compiler emit both, as in the
// original file.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

typedef std::vector<Elem_00434360> Vec_00434470;
typedef void (Vec_00434470::*InsertFn_00434470)(
    Vec_00434470::iterator, Vec_00434470::size_type, const Elem_00434360&);

// FUNCTION: 0x434470 ??0Elem_00434360@@QAE@ABU0@@Z
InsertFn_00434470 g_insert_00434470 = &Vec_00434470::insert;
