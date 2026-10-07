// Decompiled by deepseek-v4.1-flash and Opus. Names are provisional.
// std::out_of_range's out-of-line members from the MSVC 5 <stdexcept>, after
// logic_error's (logic_error.cpp). Its vtable 0x4fdcb4 is preceded by an RTTI
// locator naming ".?AVout_of_range@std@@".
//
// _Doraise is the header's {_RAISE(*this); }, i.e. `throw (*this)`: the
// inlined copy of *this onto the stack (exception's copy constructor at
// 0x4e8230, then the message string _Str and out_of_range's vtable), then
// _CxxThrowException with _TI3?AVout_of_range (0x4fefb8). The copy
// constructor is the same copy, out of line.
//
// Keep the two statics at the end: they make the vtable and copy constructor
// get emitted out of line.
#include <stdexcept>

// FUNCTION: 0x4c3af0 ?_Doraise@out_of_range@std@@MBEXXZ
// FUNCTION: 0x4c3c60 ??_Gout_of_range@std@@UAEPAXI@Z
// FUNCTION: 0x4c3cc0 ??0out_of_range@std@@QAE@ABV01@@Z
static std::out_of_range s_src("");
static std::out_of_range s_dst(s_src);
