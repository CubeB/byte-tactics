// Decompiled by Opus; renamed to the real std::map member by the orchestrator. Names are provisional.
// The out-of-line _Tree::_Lrotate of the std::map<unsigned int, Rect_0046e160> used by 0x46d040 and 0x46ef50, emitted by an explicit instantiation (it is protected).
#include <map>
struct Rect_0046e160 {
    int x;
    int y;
    short w;
    short h;
    int unknown_c;
};
// FUNCTION: 0x46feb0 ?_Lrotate@?$_Tree@IU?$pair@IURect_0046e160@@@std@@U_Kfn@?$map@IURect_0046e160@@U?$less@I@std@@V?$allocator@URect_0046e160@@@3@@2@U?$less@I@2@V?$allocator@URect_0046e160@@@2@@std@@IAEXPAU_Node@12@@Z
template class std::_Tree<unsigned int, std::pair<const unsigned int, Rect_0046e160>, std::map<unsigned int, Rect_0046e160>::_Kfn, std::less<unsigned int>, std::allocator<Rect_0046e160> >;
