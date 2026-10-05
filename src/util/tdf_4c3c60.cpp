// Decompiled by Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of std::out_of_range from
// the MSVC 5 <stdexcept>. Its vtable at 0x4fdcb4 is preceded by an RTTI
// locator naming ".?AVout_of_range@std@@". The inlined destructor chain
// stores logic_error's vtable, frees the message string and calls
// exception::~exception, as in logic_error's own (0x4c38f0).
//
// The global below exists only to make the compiler emit out_of_range's
// vtable (and with it this COMDAT) here; in the game the constructor use
// came from inlined STL code that throws.
#include <stdexcept>

// FUNCTION: 0x4c3c60 ??_Gout_of_range@std@@UAEPAXI@Z
static std::out_of_range s_error("");
