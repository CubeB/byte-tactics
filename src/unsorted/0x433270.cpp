// Decompiled by space-bunny-free. Names are provisional.
//
// The object at +0 is a std::vector<Column_00433270>, where the column is a
// std::vector<Elem_00434360> and Elem_00434360 is a struct holding one
// std::vector<Elem_00434020> (see docs/consolidation.md, STL instantiations):
// its insert is 0x4340f0, the column's operator= 0x434770 and destructor
// 0x433a80, and the held vector's _Destroy 0x433d90 and deallocate 0x433da0.
//
// Class_00433270::FUN_00433270(short): resizes the vector at +0 (its _First at
// +4, 16-byte elements) through the inlined std::vector<Column>::resize(_N, _X),
// with _X the default-constructed Column temporary in this frame (which is why
// its destructor runs here and why the function pops 4 argument bytes). The
// extra Wrap_00433270 layer inside Elem_00434360 is what keeps the innermost
// _Destroy/allocator::deallocate calls out of line at the original's exact
// inline depth; without it they inline and the code is 10 bytes short.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Wrap_00433270 {
    std::vector<Elem_00434020> v;      // +0x0
};

struct Elem_00434360 {
    Wrap_00433270 w;                   // +0x0
};

typedef std::vector<Elem_00434360> Column_00433270;

class Class_00433270 : public std::vector<Column_00433270> {
public:
    void FUN_00433270(short n);
};

// FUNCTION: 0x433270
void Class_00433270::FUN_00433270(short n)
{
    Column_00433270 x;
    resize(n, x);
}
