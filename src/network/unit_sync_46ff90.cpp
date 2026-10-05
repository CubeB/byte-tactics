// Decompiled by Opus; renamed to the real std::map member by the orchestrator. Names are provisional.
// The out-of-line iterator::_Dec of the std::map<unsigned int, UnitSyncEntry> used by 0x46ef50.
#include <map>
struct UnitSyncEntry {
    int x;
    int y;
    short w;
    short h;
    int unknown_c;
};
typedef std::map<unsigned int, UnitSyncEntry> Map_0046ff90;
typedef Map_0046ff90::_Imp::iterator Iter_0046ff90;
typedef void (Iter_0046ff90::*DecFn_0046ff90)();
// FUNCTION: 0x46ff90 ?_Dec@iterator@?$_Tree@IU?$pair@IUUnitSyncEntry@@@std@@U_Kfn@?$map@IUUnitSyncEntry@@U?$less@I@std@@V?$allocator@UUnitSyncEntry@@@3@@2@U?$less@I@2@V?$allocator@UUnitSyncEntry@@@2@@std@@QAEXXZ
DecFn_0046ff90 g_dec_0046ff90 = &Iter_0046ff90::_Dec;
