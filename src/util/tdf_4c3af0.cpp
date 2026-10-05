// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::out_of_range::_Doraise() const from the MSVC 5 <stdexcept>. The body is
// the inlined copy construction of *this onto the stack (_RAISE(*this) is
// `throw (*this)`), followed by _CxxThrowException: exception's copy
// constructor at 0x4e8230, then the inlined copy of the message string _Str,
// then the store of out_of_range's vtable 0x4fdcb4 (whose RTTI locator names
// ".?AVout_of_range@std@@"; see 0x4c3c60.cpp) and the throw
// (_TI3?AVout_of_range, 0x4fefb8). The static global below exists only to make
// the compiler emit out_of_range's vtable, and with it this inline COMDAT.
//
// check.py reports this 100% byte-identical with one BAD reference: the call
// to exception's copy constructor. data/functions.csv calls 0x4e8230
// ??0exception@@QAE@ABV0@@Z, but data/symbols.csv names only the default
// constructor 0x4e8190 as "exception::exception". base_name() in check.py
// collapses both ??0exception@@QAE@XZ and ??0exception@@QAE@ABV0@@Z to
// "exception::exception", so the reloc to 0x4e8230 is compared against
// 0x4e8190. The same collision affects 0x4c3950 and 0x4c3cc0. It needs a row
// in data/aliases.csv:
//   exception::exception,0x4e8230,second overload: ??0exception@@QAE@ABV0@@Z
#include <stdexcept>

// FUNCTION: 0x4c3af0 ?_Doraise@out_of_range@std@@MBEXXZ
static std::out_of_range s_doraise_error("");
