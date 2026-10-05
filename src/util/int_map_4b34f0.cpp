// Decompiled by Opus; renamed to the real std::map<int,int> member by the orchestrator (#580). Names are provisional.
// The out-of-line iterator::_Dec of the std::map<int,int> used by 0x4b26f0.
#include <map>
typedef std::map<int, int> Map_004b34f0;
typedef Map_004b34f0::_Imp::iterator Iter_004b34f0;
typedef void (Iter_004b34f0::*DecFn_004b34f0)();
// FUNCTION: 0x4b34f0 ?_Dec@iterator@?$_Tree@HU?$pair@HH@std@@U_Kfn@?$map@HHU?$less@H@std@@V?$allocator@H@2@@2@U?$less@H@2@V?$allocator@H@2@@std@@QAEXXZ
DecFn_004b34f0 g_dec_004b34f0 = &Iter_004b34f0::_Dec;
