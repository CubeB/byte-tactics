// Decompiled by space-bunny-free. Names are provisional.
//
// 0x4b3020 is the out-of-line copy MSVC 5 emitted of
// _Tree<int, pair<const int,int>, _Kfn, less<int>, allocator<int>>::_Insert
// (_Nodeptr _X, _Nodeptr _Y, const _Ty& _V) from the toolchain's INCLUDE/XTREE
// (lines 459-505): the node allocation, the linking and the red-black
// rebalance of std::map<int,int>::insert, whose _Nil sentinel is
// DAT_0051fbbc. The returned iterator is a class, so it comes back through a
// hidden first stack argument: that is why the callee takes four dwords of
// stack arguments and returns that pointer in eax.
//
// The tree is
// std::map<int,int> because the _Nil node at 0x51fbbc and the 0x18-byte nodes
// with an 8-byte value at +0xc and a colour at +0x14 are that instantiation's.
//
// _Insert is protected and VC5 offers no other way in than this define.
#define protected public
#include <map>
#undef protected

typedef std::map<int,int> Map_004b3020;
typedef Map_004b3020::_Imp Tree_004b3020;
typedef Map_004b3020::value_type Vt_004b3020;
typedef Map_004b3020::iterator Iter_004b3020;

typedef Iter_004b3020 (Tree_004b3020::*InsertFn_004b3020)(
    Tree_004b3020::_Nodeptr, Tree_004b3020::_Nodeptr, const Vt_004b3020&);

// Taking the address keeps the out-of-line copy in the object file.
InsertFn_004b3020 g_insert_004b3020 = &Tree_004b3020::_Insert;

// FUNCTION: 0x4b3020 ?_Insert@?$_Tree@HU?$pair@HH@std@@U_Kfn@?$map@HHU?$less@H@std@@V?$allocator@H@2@@2@U?$less@H@2@V?$allocator@H@2@@std@@QAE?AViterator@12@PAU_Node@12@0ABU?$pair@HH@2@@Z
