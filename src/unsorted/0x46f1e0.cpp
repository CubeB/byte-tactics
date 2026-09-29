// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::erase(iterator) from MSVC 5's <xtree> for the
// std::map<unsigned int, Rect_0046e160> tree whose _Nil node is
// DAT_0051e598. The real template reproduces the function: it inlines one
// level of _Erase (0x46f6d0) and calls iterator::_Inc (0x46ea10) and
// _Lockit/_Lockit::~_Lockit around the node unlinks. data/aliases.csv gives
// both erase overloads the one name used by the map instantiation.
#include <map>

struct Rect_0046e160 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

typedef std::map<unsigned int, Rect_0046e160>::_Imp Tree_0046f1e0;
typedef Tree_0046f1e0::iterator (Tree_0046f1e0::*EraseFn_0046f1e0)(
    Tree_0046f1e0::iterator);

// FUNCTION: 0x46f1e0 ?erase@?$_Tree@IU?$pair@IURect_0046e160@@@std@@U_Kfn@?$map@IURect_0046e160@@U?$less@I@std@@V?$allocator@URect_0046e160@@@3@@2@U?$less@I@2@V?$allocator@URect_0046e160@@@2@@std@@QAE?AViterator@12@V312@@Z
EraseFn_0046f1e0 g_erase_0046f1e0 = &Tree_0046f1e0::erase;
