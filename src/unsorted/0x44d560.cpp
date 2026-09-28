// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Clears a std::vector<Point_0044eec0> and then appends a copy of the point
// at +0x8, with its y moved by (field_c + field_10) / 32. The compiler inlines
// vector::clear (the loop at the top, whose copy range is provably empty) and
// vector::push_back, whose inlined insert leaves the vector helpers _Ucopy,
// _Ufill and _Destroy out of line. The two fields at +0xc and +0x10 are loaded
// as field_10 then field_c so that the sum's operand order matches.
//
// Bytes match 100%. One reference still differs by name only: the inlined
// insert calls std::vector<Point_0044eec0>::size, which the original has out of
// line at 0x44ee70, but data/symbols.csv records 0x44ee70 as
// 'Class_0044ee70::FUN_0044ee70' (0x44ee70.cpp). It is the same function: its
// body is (_First == 0 ? 0 : (_Last - _First) / 4), and it is called from the
// same inlined vector::insert at 0x44d1d7, 0x44db0f, 0x44db1f and 0x44db95.
// Every sibling instantiation in symbols.csv uses the real STL name
// (0x40c560 PAUUnit::?$vector::size, 0x40c5b0 UElem_0040cc40::?$vector::size,
// ...), so 0x44ee70 should be renamed to
// UPoint_0044eec0::?$vector::size. The call cannot be spelled with the STL
// mangled name and the provisional one at the same time from source.
#include <vector>

struct Point_0044eec0 {
    short x;
    short y;
};

typedef std::vector<Point_0044eec0> Vec_0044d560;

class Class_0044d560 {
public:
    char unknown_0[8];
    Point_0044eec0 pos;                // +0x8
    int field_c;                       // +0xc
    int field_10;                      // +0x10

    void FUN_0044d560(Vec_0044d560* list);
};

// FUNCTION: 0x44d560
void Class_0044d560::FUN_0044d560(Vec_0044d560* list)
{
    list->clear();
    Point_0044eec0 p = pos;
    p.y = p.y + (field_10 + field_c) / 32;
    list->push_back(p);
}
