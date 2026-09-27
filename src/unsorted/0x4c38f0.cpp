// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of std::logic_error from
// the MSVC 5 <stdexcept>. Its vtable at 0x4fdca4 is preceded by an RTTI
// locator whose type descriptor (0x50a630) is ".?AVlogic_error@std@@"; the
// neighbours are logic_error::what (0x4c3730), _Doraise (0x4c3740), the inline
// destructor (0x4c38a0) and the copy constructor (0x4c3950), in the same order
// MSVC emits these COMDATs. The out_of_range set follows at 0x4c3aa0.
//
// The global below exists only to make the compiler emit logic_error's
// vtable (and with it this COMDAT) here; in the game the constructor use
// came from inlined STL code that throws.
#include <stdexcept>

// FUNCTION: 0x4c38f0 ??_Glogic_error@std@@UAEPAXI@Z
static std::logic_error s_error("");
