// Decompiled by Opus. Names are provisional.
// std::vector<Elem_00425430>::_Destroy(first, last) from MSVC 5's <vector>:
// empty for a trivial element type. Its caller (0x424c00) shrinks a local
// vector twice, once through the out-of-line erase 0x425430 and once
// through an inlined erase that calls std::copy (0x4256a0) and then this
// with ecx set to the vector. _Destroy is protected, so a derived class
// takes its address to make the compiler emit it out of line.
#include <vector>

struct Elem_00425430 {
    short value;
};

typedef std::vector<Elem_00425430> Vec_00425470;
typedef void (Vec_00425470::*DestroyFn_00425470)(Vec_00425470::iterator, Vec_00425470::iterator);

struct Access_00425470 : Vec_00425470 {
    static DestroyFn_00425470 fn;
};

// FUNCTION: 0x425470 ?_Destroy@?$vector@UElem_00425430@@V?$allocator@UElem_00425430@@@std@@@std@@IAEXPAUElem_00425430@@0@Z
DestroyFn_00425470 Access_00425470::fn = &Access_00425470::_Destroy;
