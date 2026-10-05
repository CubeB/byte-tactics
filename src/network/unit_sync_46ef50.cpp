// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::insert(const value_type&) from MSVC 5's <xtree>, emitted
// out of line for the std::map<unsigned int, Rect_0046e160> tree whose _Nil
// node is DAT_0051e598. The real template reproduces the whole function: the
// _Multi branch inlines _Insert (which is why that path calls _Buynode,
// _Lrotate and _Rrotate out of line), and the non-_Multi path tails two
// _Insert uses into one call to the out-of-line _Insert at 0x46fb80.
//
// check.py reports 100% of the bytes identical but refuses the function on a
// reference, because data/symbols.csv still carries the placeholder names for
// four callees that are really members of this same template instantiation:
//
//   +0x077  _Buynode   0x46ff70  recorded as Class_0046ff70::FUN_0046ff70
//   +0x155  _Lrotate   0x46feb0  recorded as Class_0046feb0::FUN_0046feb0
//   +0x176  _Rrotate   0x46ff10  recorded as Class_0046ff10::FUN_0046ff10
//   +0x231  iterator::_Dec       0x46ff90  recorded as Class_0046ff90::FUN_0046ff90
//
// The bytes and the call targets are right; only the names disagree. Per
// docs/field-notes.md item 1 this is a symbols.csv consolidation issue, not a
// source problem. (data/symbols.csv also lists this address as
// IUValue_0046d040::IU?$pair::?$_Tree::insert; the real instantiation is the
// Rect_0046e160 one, as 0x46fb80/0x46f6d0/0x46e890 already established.)
#include <map>

struct Rect_0046e160 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

typedef std::map<unsigned int, Rect_0046e160>::_Imp Tree_0046ef50;
typedef Tree_0046ef50::_Pairib (Tree_0046ef50::*InsertFn_0046ef50)(
    const Tree_0046ef50::value_type&);

// FUNCTION: 0x46ef50 ?insert@?$_Tree@IU?$pair@IURect_0046e160@@@std@@U_Kfn@?$map@IURect_0046e160@@U?$less@I@std@@V?$allocator@URect_0046e160@@@3@@2@U?$less@I@2@V?$allocator@URect_0046e160@@@2@@std@@QAE?AU?$pair@Viterator@?$_Tree@IU?$pair@IURect_0046e160@@@std@@U_Kfn@?$map@IURect_0046e160@@U?$less@I@std@@V?$allocator@URect_0046e160@@@3@@2@U?$less@I@2@V?$allocator@URect_0046e160@@@2@@std@@_N@2@ABU?$pair@IURect_0046e160@@@2@@Z
InsertFn_0046ef50 g_insert_0046ef50 = &Tree_0046ef50::insert;
