// Decompiled by space-bunny-free. Names are provisional.
//
// BYTES MATCH 100%, but check.py reports one BAD reference, so this is not
// MATCH:
//
//   BAD +0x0a2  UElem_00434360::?$vector::~?$vector  0x433a80
//     0x433a80 is already named
//     'UElem_00434020::V?$vector::?$vector::~?$vector' in data/symbols.csv
//
// The entry for 0x433a80 in data/symbols.csv looks wrong. The one 16-byte
// element type of this class is called three different placeholder names in
// the references this function makes, all of them 4-byte trivial structs:
//   * its operator= is 0x434770, '??4?$vector@UElem_00434360@@...',
//   * its _Destroy is 0x433d90, 'UElem_00433d50::?$vector::_Destroy',
//   * its destructor is 0x433a80, 'UElem_00434020::V?$vector::?$vector::~...'.
// No single spelling of the element type satisfies all three, so nothing that
// uses this vector can match until the three placeholders are reconciled.
// The call graph says they are one type: 0x433a80's only two callers are
// 0x433270 and 0x4340f0 (the insert of
// 'UElem_00434360::V?$vector::?$vector::insert'), and both of those call
// 0x434770 on the element right before, so 0x433a80 is the destructor of
// Elem_00434360. 0x433a80 and 0x434020 (erase of the same class) are
// byte-identical to their Elem_00434020-named counterparts because the
// element is a struct holding exactly one std::vector, so this file can reach
// the same bytes with either placeholder.
//
// Class_00433270::FUN_00433270(short): resizes the vector at +0 (its _First at
// +4, 16-byte elements) through the inlined std::vector<Column>::resize(_N, _X),
// with _X the default-constructed Column temporary in this frame (which is why
// its destructor runs here and why the function pops 4 argument bytes). The
// extra Wrap_00433270 layer inside Elem_00434360 is what keeps the innermost
// _Destroy/allocator::deallocate calls out of line at the original's exact
// inline depth; without it they inline and the code is 10 bytes short.
#include <vector>

struct Elem_00433d50 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Wrap_00433270 {
    std::vector<Elem_00433d50> v;      // +0x0
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
