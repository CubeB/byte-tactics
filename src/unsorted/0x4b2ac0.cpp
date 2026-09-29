// Decompiled by GPT-6. Names are provisional.
// All 1254 bytes match, but iterator::_Inc at 0x4b3590 is still named
// Class_004b3590::FUN_004b3590 in data/symbols.csv, so check.py rejects it.
#include <map>

typedef std::map<int, int> Map_004b2ac0;
typedef std::_Tree<int, std::pair<const int, int>, Map_004b2ac0::_Kfn,
    std::less<int>, std::allocator<int> > Tree_004b2ac0;
typedef Tree_004b2ac0::iterator (Tree_004b2ac0::*Erase_004b2ac0)(Tree_004b2ac0::iterator);
Erase_004b2ac0 erase_004b2ac0 = &Tree_004b2ac0::erase;

// FUNCTION: 0x4b2ac0 ?erase@?$_Tree@HU?$pair@HH@std@@U_Kfn@?$map@HHU?$less@H@std@@V?$allocator@H@2@@2@U?$less@H@2@V?$allocator@H@2@@std@@QAE?AViterator@12@V312@@Z
