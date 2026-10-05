// Decompiled by Sonnet; renamed to the real std::map<int,int> member by the orchestrator (#580). Names are provisional.
// The out-of-line _Tree::begin of the std::map<int,int> used by 0x4b26f0.
#include <map>
typedef std::map<int, int> Map_004b2840;
typedef Map_004b2840::_Imp Tree_004b2840;
typedef Tree_004b2840::iterator (Tree_004b2840::*BeginFn_004b2840)();
// FUNCTION: 0x4b2840 ?begin@?$_Tree@HU?$pair@HH@std@@U_Kfn@?$map@HHU?$less@H@std@@V?$allocator@H@2@@2@U?$less@H@2@V?$allocator@H@2@@std@@QAE?AViterator@12@XZ
BeginFn_004b2840 g_begin_004b2840 = &Tree_004b2840::begin;
