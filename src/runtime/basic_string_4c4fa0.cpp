// Decompiled by Claude Opus 5.5. Names are provisional.
//
// std::basic_string<char>::_Copy, the compiler's own <xstring> template as
// one of Cavedog's objects instantiated it. Its allocation sits in the
// header's _TRY_BEGIN / _CATCH_ALL block, a try/catch frame, which is why it
// has no FPO record.
#include <string>

// FUNCTION: 0x4c4fa0 ?_Copy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z
template void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Copy(
    std::basic_string<char, std::char_traits<char>, std::allocator<char> >::size_type);
