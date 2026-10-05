// Decompiled by Haiku. Names are provisional.
// std::logic_error::what() from the MSVC 5 <stdexcept>; the vtable at
// 0x4fdca4 is std::logic_error's (see src/util/tdf_4c38f0.cpp, which emits
// its scalar deleting destructor the same way). The global below exists only
// to make the compiler emit logic_error's vtable, and with it this COMDAT.
#include <stdexcept>

// FUNCTION: 0x4c3730 ?what@logic_error@std@@UBEPBDXZ
static std::logic_error s_what_error("");
