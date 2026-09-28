// Decompiled by deepseek-v4.1-flash; rewritten to the real template member by the orchestrator (#382). Names are provisional.
// The reference-counted std::basic_string<char> copy constructor:
// allocator copy, _Tidy(), then the inlined assign(_X, 0, npos). The file
// used to spell it as a hand-written stand-in class; the real <xstring>
// instantiation compiles to the same bytes, and its callees then keep the
// std names that the stdexcept functions (0x4c3740 on) also use.
#include <string>
#include <string.h>

// FUNCTION: 0x4c49a0 ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@@Z
template class std::basic_string<char, std::char_traits<char>, std::allocator<char> >;
