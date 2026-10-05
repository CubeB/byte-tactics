// std::basic_string<char> members from the compiler's own <xstring>, compiled
// with the game's options. LIBCPMT.LIB's copies are built with other options,
// and only their smallest members are byte-identical to these.
//  - 0x4c4ac0 to 0x4c50a0 sit among Cavedog's own functions: the copies one of
//    Cavedog's objects instantiated. _Copy (0x4c4fa0) is between them too; it
//    has a try/catch frame and no FPO record, so it is the gap region
//    src/runtime/basic_string_4c4fa0.cpp.
//  - assign (0x4e3c00) sits among the members of the original's string.obj
//    (_Xlen before it, max_size, length_error and _Xran after it), compiled
//    the same way. It calls max_size, the game row 0x4e3e10 (matched as
//    FUN_004e3e10, so data/aliases.csv gives the name that address too).
#include <string>

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > String;

// FUNCTION: 0x4c4ac0 ?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z
template void String::_Tidy(bool);

// FUNCTION: 0x4c4b10 ?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@II@Z
template String& String::erase(String::size_type, String::size_type);

// FUNCTION: 0x4c4c30 ?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z
template void String::_Eos(String::size_type);

// FUNCTION: 0x4c4c50 ?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z
template bool String::_Grow(String::size_type, bool);

// FUNCTION: 0x4c50a0 ?_Split@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXXZ
template void String::_Split();

// FUNCTION: 0x4e3c00 ?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z
template String& String::assign(const String&, String::size_type, String::size_type);
