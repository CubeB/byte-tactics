// Decompiled by Opus; renamed to the real std::map member by the orchestrator. Names are provisional.
// The out-of-line _Tree::_Buynode of the std::map<unsigned int, UnitSyncEntry> used by 0x46d040 and 0x46ef50, emitted by an explicit instantiation (it is protected).
#include <map>
struct UnitSyncEntry {
    int x;
    int y;
    short w;
    short h;
    int unknown_c;
};
// FUNCTION: 0x46ff70 ?_Buynode@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@IAEPAU_Node@12@PAU312@W4_Redbl@12@@Z
template class std::_Tree<unsigned int, std::pair<const unsigned int, UnitSyncEntry>, std::map<unsigned int, UnitSyncEntry>::_Kfn, std::less<unsigned int>, std::allocator<UnitSyncEntry> >;
