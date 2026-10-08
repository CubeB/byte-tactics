// std::basic_string<char>::assign from the compiler's own <xstring>.
//  - It sits among the members of the original's string.obj (_Xlen before it,
//    max_size, length_error and _Xran after it), compiled the same way. It
//    calls max_size, the game row 0x4e3e10.
//  - The members Cavedog's TDF code instantiated (0x4c4ac0 to 0x4c50a0) are in
//    src/util/tdf_4c2ea0.cpp.
#include <string>

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > String;

// FUNCTION: 0x4e3c00 ?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z
template String& String::assign(const String&, String::size_type, String::size_type);
