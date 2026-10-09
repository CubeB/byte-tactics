// Decompiled by space-bunny-free. Names are provisional.
//
// The object at +0 is a std::vector<Column_00433270>, where the column is a
// std::vector<Elem_00434360> and Elem_00434360 is a struct holding one
// std::vector<Elem_00434020> (see docs/consolidation.md, STL instantiations):
// its insert is 0x4340f0, the column's operator= 0x434770 and destructor
// 0x433a80, and the held vector's _Destroy 0x433d90 and deallocate 0x433da0.
//
// LosTables::ResizeTables(short): resizes the vector at +0 (its _First at
// +4, 16-byte elements) to n columns, filled with a default-constructed Column.
#include <vector>
#include "los_tables.h"

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

// Extra layer: keeps the innermost _Destroy/deallocate calls out of line.
struct Wrap_00433270 {
    std::vector<Elem_00434020> v;      // +0x0
};

struct Elem_00434360 {
    Wrap_00433270 w;                   // +0x0
};

typedef std::vector<Elem_00434360> Column_00433270;

// FUNCTION: 0x433270
void LosTables::ResizeTables(short n)
{
    Column_00433270 x;
    ((std::vector<Column_00433270>*)this)->resize(n, x);
}
