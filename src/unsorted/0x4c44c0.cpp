// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free. Names are provisional.
// Bounds-checked accessor into the vector of pointers held at +4 (_First at
// +8). The first size() guard returns 0 when the index is out of range; the
// inlined std::vector::at that follows repeats the check and throws
// out_of_range, exactly as MSVC 5's <vector> defines it (`_Xran`'s
// _THROW(out_of_range, "invalid vector<T> subscript")).
//
// This compiles to the original's 253 bytes. The only reference check.py
// cannot confirm is the last one: the inlined logic_error constructor does
// `exception("")`, i.e. the `exception::exception(const char* const&)`
// overload at 0x4e81d0. data/symbols.csv names `exception::exception` only at
// 0x4e8190 (the default constructor), and check.py's base_name drops the
// signature, so it reports "a reference is wrong". Same overload collision as
// #249: needs the row
//   exception::exception,0x4e81d0,"overload: exception(const char* const&), reached by the inlined std::vector::at in 0x4c44c0; 0x4e8190 is the default constructor"
// in data/aliases.csv (with it this function matches and nothing else changes).
#include <vector>

struct Named_004c44c0 {
    char* name;                         // +0x0
};

class Class_004c44c0 {
public:
    int unknown_0;
    std::vector<Named_004c44c0*> entries;   // +0x4 (_First at +0x8)

    Named_004c44c0* FUN_004c44c0(int index);
};

// FUNCTION: 0x4c44c0
Named_004c44c0* Class_004c44c0::FUN_004c44c0(int index)
{
    if ((unsigned int)index >= entries.size())
        return 0;
    return entries.at(index);
}
