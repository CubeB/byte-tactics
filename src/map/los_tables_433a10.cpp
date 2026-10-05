// Decompiled by Claude Opus 5.5. Names are provisional.
// std::vector<Elem_00434020>::vector(const allocator&) from MSVC 5's <vector>,
// out of line: copies the empty allocator byte and zeroes _First, _Last and
// _End. It is also the default constructor (the allocator is a default
// argument). Its caller 0x433380 builds the fill value of a
// vector<Elem_00434360> resize with it, an Elem_00434360 whose implicit
// constructor inlines down to this call (the held vector's destructor is
// 0x433a30). A constructor's address can't be taken, so an explicit
// instantiation of the whole class emits it, as in 0x40c510.cpp; that needs
// the element's comparison operators, which are only declared.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

bool operator==(const Elem_00434020&, const Elem_00434020&);
bool operator<(const Elem_00434020&, const Elem_00434020&);

template class std::vector<Elem_00434020>;

// FUNCTION: 0x433a10 ??0?$vector@UElem_00434020@@V?$allocator@UElem_00434020@@@std@@@std@@QAE@ABV?$allocator@UElem_00434020@@@1@@Z
