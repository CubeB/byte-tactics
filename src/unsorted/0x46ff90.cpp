// Decompiled by Opus; renamed to the real std::map member by the orchestrator. Names are provisional.
// The out-of-line iterator::_Dec of the std::map<unsigned int, Rect_0046e160> used by 0x46ef50.
#include <map>
struct Rect_0046e160 {
    int x;
    int y;
    short w;
    short h;
    int unknown_c;
};
typedef std::map<unsigned int, Rect_0046e160> Map_0046ff90;
typedef Map_0046ff90::_Imp::iterator Iter_0046ff90;
typedef void (Iter_0046ff90::*DecFn_0046ff90)();
// FUNCTION: 0x46ff90 ?_Dec@iterator@?$_Tree@IU?$pair@IURect_0046e160@@@std@@U_Kfn@?$map@IURect_0046e160@@U?$less@I@std@@V?$allocator@URect_0046e160@@@3@@2@U?$less@I@2@V?$allocator@URect_0046e160@@@2@@std@@QAEXXZ
DecFn_0046ff90 g_dec_0046ff90 = &Iter_0046ff90::_Dec;
