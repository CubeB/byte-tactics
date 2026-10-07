// Decompiled by Opus. Names are provisional.
// std::list<int>::iterator::operator++(int), out of line: return the old
// position and step to the next node. Its callers (e.g. 0x46c920) use it in
// the inlined list::erase(first, last) loop, `erase(_F++)`, on the list at
// +0x20 of the object 0x46d040 builds. The node is 0xc bytes (the constructor
// allocates the head with `new(0xc)`), so the element is a 4-byte value
// compared against an id (0x46d860); int is a guess.
#include <list>

typedef std::list<int> List_0046fac0;
typedef List_0046fac0::iterator (List_0046fac0::iterator::*PostIncFn_0046fac0)(int);

// FUNCTION: 0x46fac0 ??Eiterator@?$list@HV?$allocator@H@std@@@std@@QAE?AV012@H@Z
PostIncFn_0046fac0 g_postinc_0046fac0 = &List_0046fac0::iterator::operator++;
