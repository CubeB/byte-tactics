// Decompiled by Opus. Names are provisional.
// std::_Tree<...>::iterator::_Inc() from MSVC 5's <xtree> (the in-order
// successor of a red-black tree node, with the inlined _Min taking its own
// lock) for the std::map<unsigned int, Rect_0046e160> tree whose _Nil node
// is DAT_0051e598. Its callers are the tree's erase(first, last) (0x46e890)
// and erase(iterator) (0x46f1e0), both through erase(_F++), and 0x46dad0.
// Renamed from Class_0046ea10::FUN_0046ea10 in #249; the real template
// compiles to the same bytes. Built without /GX, as Cavedog did, so the
// locks need no EH frame.
#include <map>

struct Rect_0046e160 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

typedef std::map<unsigned int, Rect_0046e160>::_Imp Tree_0046ea10;
typedef void (Tree_0046ea10::iterator::*IncFn_0046ea10)();

// FUNCTION: 0x46ea10 ?_Inc@iterator@?$_Tree@IU?$pair@IURect_0046e160@@@std@@U_Kfn@?$map@IURect_0046e160@@U?$less@I@std@@V?$allocator@URect_0046e160@@@3@@2@U?$less@I@2@V?$allocator@URect_0046e160@@@2@@std@@QAEXXZ
IncFn_0046ea10 g_inc_0046ea10 = &Tree_0046ea10::iterator::_Inc;
