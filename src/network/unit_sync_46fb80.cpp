// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Tree<...>::_Insert(_Nodeptr _X, _Nodeptr _Y, const _Ty& _V) from
// MSVC 5's <xtree> for the std::map<unsigned int, Rect_0046e160> tree whose
// _Nil node is DAT_0051e598. It is the red-black tree insert: allocate and
// link a new red node below _Y, then fix the tree up in the loop. Taking the
// protected member's address makes the compiler emit the instantiation out of
// line, exactly as 0x46e890 does for erase and 0x46f6d0 for _Erase.
#include <map>

struct Rect_0046e160 {                 // 0x10 bytes
    int x;                             // +0x0
    int y;                             // +0x4
    short w;                           // +0x8
    short h;                           // +0xa
    int unknown_c;                     // +0xc
};

typedef std::map<unsigned int, Rect_0046e160> Map_0046fb80;
typedef Map_0046fb80::_Imp Tree_0046fb80;

struct Access_0046fb80 : Tree_0046fb80 {
    typedef iterator (Tree_0046fb80::*InsertFn)(_Nodeptr, _Nodeptr, const value_type&);
    static InsertFn fn;
};

// FUNCTION: 0x46fb80 ?_Insert@?$_Tree@IU?$pair@IURect_0046e160@@@std@@U_Kfn@?$map@IURect_0046e160@@U?$less@I@std@@V?$allocator@URect_0046e160@@@3@@2@U?$less@I@2@V?$allocator@URect_0046e160@@@2@@std@@IAE?AViterator@12@PAU_Node@12@0ABU?$pair@IURect_0046e160@@@2@@Z
Access_0046fb80::InsertFn Access_0046fb80::fn = &Access_0046fb80::_Insert;
