// Decompiled by Opus. Names are provisional.
// std::list<int>::erase(iterator), out of line: its callers (0x46c920,
// 0x46ca60, 0x46d040's neighbours) call it as `erase(_F++)` with the
// iterator post-increment at 0x46fac0, on the list at +0x20 of
// UnitSync. Taking the member's address makes the compiler emit the
// template instantiation out of line.
#include <list>

typedef std::list<int> List_0046eb60;
typedef List_0046eb60::iterator (List_0046eb60::*EraseFn_0046eb60)(List_0046eb60::iterator);

// FUNCTION: 0x46eb60 ?erase@?$list@HV?$allocator@H@std@@@std@@QAE?AViterator@12@V312@@Z
EraseFn_0046eb60 g_erase_0046eb60 = &List_0046eb60::erase;
