// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::logic_error's compiler-generated copy constructor from the MSVC 5
// <stdexcept> (logic_error derives from exception and holds the message
// string _Str at +0xc). The vtable at 0x4fdca4 is logic_error's (see
// 0x4c38f0.cpp, which emits its scalar deleting destructor the same way).
// The body copies the exception base with exception::exception(const
// exception&) at 0x4e8230, then copy-constructs _Str: the (empty) allocator
// base byte at +0xc, _Tidy zeroing _Ptr/_Len/_Res at +0x10/+0x14/+0x18, and
// the inlined assign(_X, 0, npos) with its self-assign, share and grow paths.
// The two static objects exist only to make the compiler emit the implicit
// copy constructor out of line, as in the original.
//
// check.py reports this 100% byte-identical with one BAD reference: the call
// to the exception copy constructor. data/functions.csv (line 0x4e8230) calls
// it ??0exception@@QAE@ABV0@@Z, but data/symbols.csv names only the default
// constructor 0x4e8190 as "exception::exception". base_name() in check.py
// collapses both ??0exception@@QAE@XZ and ??0exception@@QAE@ABV0@@Z to
// "exception::exception", so the reloc to 0x4e8230 is compared against
// 0x4e8190. The same collision affects std::out_of_range's copy constructor
// (0x4c3cc0). It needs a row in data/aliases.csv:
//   exception::exception,0x4e8230,second overload: ??0exception@@QAE@ABV0@@Z
#include <stdexcept>

// FUNCTION: 0x4c3950 ??0logic_error@std@@QAE@ABV01@@Z
static std::logic_error s_src_004c3950("");
static std::logic_error s_dst_004c3950(s_src_004c3950);
