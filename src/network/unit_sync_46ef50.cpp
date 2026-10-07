// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::insert(const value_type&) from MSVC 5's <xtree>, emitted
// out of line for the std::map<unsigned int, UnitSyncEntry> tree whose _Nil
// node is DAT_0051e598.
#include <map>

struct UnitSyncEntry {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

typedef std::map<unsigned int, UnitSyncEntry>::_Imp Tree_0046ef50;
typedef Tree_0046ef50::_Pairib (Tree_0046ef50::*InsertFn_0046ef50)(
    const Tree_0046ef50::value_type&);

// FUNCTION: 0x46ef50 ?insert@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAE?AU?$pair@Viterator@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@_N@2@ABU?$pair@IUUnitSyncEntry@@@2@@Z
InsertFn_0046ef50 g_insert_0046ef50 = &Tree_0046ef50::insert;
