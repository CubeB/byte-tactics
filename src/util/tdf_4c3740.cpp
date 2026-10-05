// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::logic_error::_Doraise() const from the MSVC 5 <stdexcept>. It is the
// third vtable slot of logic_error (0x4fdca4), after what() (0x4c3730) and the
// scalar deleting destructor (0x4c38f0). The body is the header's
// {_RAISE(*this); }, i.e. `throw (*this)`: it copies *this onto the stack
// (exception base copy via exception::exception(const exception&) at 0x4e8230,
// then the message string _Str at +0xc) and calls _CxxThrowException with
// logic_error's throw info (0x4fefd8). The static global below exists only to
// make the compiler emit logic_error's vtable, and with it this inline COMDAT.
//
// check.py reports this 100% byte-identical with one BAD reference: the call
// to the exception copy constructor. data/functions.csv (0x4e8230) calls it
// ??0exception@@QAE@ABV0@@Z, but data/symbols.csv names only the default
// constructor 0x4e8190 as "exception::exception". base_name() in check.py
// collapses both ??0exception@@QAE@XZ and ??0exception@@QAE@ABV0@@Z to
// "exception::exception", so the reloc to 0x4e8230 is compared against
// 0x4e8190. The same collision affects logic_error's and out_of_range's copy
// constructors (0x4c3950, 0x4c3cc0) and out_of_range::_Doraise (0x4c3af0).
// It needs a row in data/aliases.csv:
//   exception::exception,0x4e8230,second overload: ??0exception@@QAE@ABV0@@Z
#include <stdexcept>

// FUNCTION: 0x4c3740 ?_Doraise@logic_error@std@@MBEXXZ
static std::logic_error s_doraise_error("");
