// Decompiled by Opus. Names are provisional.
// std::_Tree<...>::_Erase(_Nodeptr) from MSVC 5's <xtree> (recursively frees
// a subtree under a lock) for the std::map<unsigned int, UnitSyncEntry> tree
// whose _Nil node is DAT_0051e598. Its callers are itself and the tree's
// erase(first, last) (0x46e890), which inlines one level of it.
#include <map>

struct UnitSyncEntry {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

typedef std::map<unsigned int, UnitSyncEntry>::_Imp Tree_0046f6d0;

struct Access_0046f6d0 : Tree_0046f6d0 {
    typedef void (Tree_0046f6d0::*EraseFn)(_Nodeptr);
    static EraseFn fn;
};

// FUNCTION: 0x46f6d0 ?_Erase@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEXPAU_Node@12@@Z
Access_0046f6d0::EraseFn Access_0046f6d0::fn = &Access_0046f6d0::_Erase;
