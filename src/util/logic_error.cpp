// Decompiled by Haiku, deepseek-v4.1-flash and Opus. Names are provisional.
// std::logic_error's out-of-line members from the MSVC 5 <stdexcept>. Its
// vtable at 0x4fdca4 is preceded by an RTTI locator whose type descriptor
// (0x50a630) is ".?AVlogic_error@std@@"; the COMDATs sit in the order MSVC
// emits them: what() (0x4c3730), _Doraise (0x4c3740), the inline destructor
// (0x4c38a0), the scalar deleting destructor (0x4c38f0) and the copy
// constructor (0x4c3950). The out_of_range set follows at 0x4c3aa0. The
// constructor from a string (0x4c35c0) needs the class written by hand, so it
// has a file of its own.
//
// _Doraise is the header's {_RAISE(*this); }, i.e. `throw (*this)`: it copies
// *this onto the stack (the exception base through exception(const
// exception&) at 0x4e8230, then the message string _Str at +0xc) and calls
// _CxxThrowException with logic_error's throw info (0x4fefd8). The copy
// constructor copies the exception base the same way, then copy-constructs
// _Str: the (empty) allocator byte at +0xc, _Tidy zeroing _Ptr/_Len/_Res at
// +0x10/+0x14/+0x18, and the inlined assign(_X, 0, npos) with its
// self-assign, share and grow paths.
//
// Keep the two statics at the end: they make the vtable and copy constructor
// get emitted out of line.
#include <stdexcept>

// FUNCTION: 0x4c3730 ?what@logic_error@std@@UBEPBDXZ
// FUNCTION: 0x4c3740 ?_Doraise@logic_error@std@@MBEXXZ
// FUNCTION: 0x4c38f0 ??_Glogic_error@std@@UAEPAXI@Z
// FUNCTION: 0x4c3950 ??0logic_error@std@@QAE@ABV01@@Z
static std::logic_error s_src("");
static std::logic_error s_dst(s_src);
