// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// std::_Tree<...>::erase(iterator first, iterator last) from MSVC 5's <xtree>
// for the std::map<unsigned int, Rect_0046e160> tree (_Nil is DAT_0051e598,
// _Nilrefs DAT_0051e59c), emitted out of line. Its three callers (0x46c920,
// 0x46ca60, 0x46d1a0) inline ~_Tree(): erase(begin(), end()), free the head,
// then drop _Nilrefs under a lock. The real template reproduces every call:
// _Erase() is inlined once and its recursion calls 0x46f6d0, erase(_F++)
// calls iterator::_Inc() (0x46ea10) and then erase(iterator) (0x46f1e0).
// The two erase overloads share one name for tools/check.py (it compares
// names without their argument lists), so the reference to erase(iterator)
// needs an entry in data/aliases.csv; see docs/consolidation.md.
#include <map>

struct Rect_0046e160 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

typedef std::map<unsigned int, Rect_0046e160>::_Imp Tree_0046e890;
typedef Tree_0046e890::iterator (Tree_0046e890::*EraseFn_0046e890)(
    Tree_0046e890::iterator, Tree_0046e890::iterator);

// FUNCTION: 0x46e890 ?erase@?$_Tree@IU?$pair@IURect_0046e160@@@std@@U_Kfn@?$map@IURect_0046e160@@U?$less@I@std@@V?$allocator@URect_0046e160@@@3@@2@U?$less@I@2@V?$allocator@URect_0046e160@@@2@@std@@QAE?AViterator@12@V312@0@Z
EraseFn_0046e890 g_erase_0046e890 = &Tree_0046e890::erase;
