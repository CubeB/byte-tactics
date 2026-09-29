// Decompiled by GPT-5.6-Terra, finished by space-bunny-free. Names are provisional.
// std::vector<Class_00471cc0*>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward inlined; the
// sixteen push_back sites call it out of line (they inline the count-is-one
// overload instead). Taking the member's address makes the compiler emit the
// template instantiation out of line, as in the original file. The member
// pointer must return void: spelled with an iterator return, VC5 resolves the
// wrong overload (C2563, or C2440 with a cast), because it prefers the
// two-argument insert.
//
// Still differs (57.9%, checked twice in this retry): the original puts `this` in ebp and the count in ebx
// (`mov ebx, [esp+0x18]; mov ebp, ecx`), this build puts `this` in ebx and the
// count in ebp, and that one choice cascades into every block (ours is 547
// bytes, the original 537, because this has to reload `this` from its stack
// slot where the original keeps it in a register). Nothing else differs: every
// other difference in the diff is that same ebp/ebx/esi/edi permutation, the
// branch structure, the pointer sums and the two calls to operator new and
// operator delete are all identical.
//
// The exe's own 0x408f30, the same template instantiated on vector<Unit*>,
// has the register assignment this build produces, so the game holds both
// variants of the template and the one here is the rarer one. It is compiler
// state from the rest of the original translation unit, not the template, and
// no file-level change reaches it. Beyond everything the first attempt tried
// (all of it still true, every one compiles to the same `mov ebx, ecx;
// mov ebp, __M$`), these do not change it either:
// - the unpatched compiler, `BT_TOOLCHAIN=msvc5-rtm` (its C1XX.DLL differs from
//   SP3's), gives the identical 547 bytes, so this is not an older compiler
//   build; VECTOR, XTREE, ALGORITHM and XSTRING are byte-identical between
//   toolchain/msvc5-rtm/INCLUDE and toolchain/msvc5-sp3/INCLUDE, so a
//   different STL revision cannot explain it either
// - the exe's own neighbourhood, in the exe's emission order and in one file:
//   vector<Elem_00473500>::_Destroy (0x4732d0) first, then this insert, then
//   _Ucopy (0x473500) and _Ufill (0x473530), with the real Class_00471cc0
//   (vtable 0x4fd5a8, constructor 0x471cc0, destructor 0x471d00, three pure
//   virtuals). check.py's whole output is byte-identical to the plain file's
// - a real caller that *inlines* this insert, 0x471820's
//   Class_00471820::FUN_00471820 with its pool operator new and the
//   Class_00474cd0 hierarchy, compiled before the out-of-line emission the
//   other sixteen call sites link against: unchanged
// - spelling the default allocator out, `std::vector<Class_00471cc0*,
//   std::allocator<Class_00471cc0*> >`, which needs the complete element type
//   (XMEMORY(33): error C2027 with only a forward declaration): unchanged
// The build is invariant, so this needs the regrouping-into-original-
// translation-units phase, and the state that decides the choice is not
// something the file can carry. Nothing before this attempt changed it:
// - all 128 header sets (tools/headers.py), plus <string>, <map>, <list>,
//   <algorithm> and <iostream> after <windows.h>
// - 100 to 4000 unused function prototypes, 40 inline function definitions,
//   40 function definitions, 200 class definitions, 200 extern variables
// - `template class std::vector<Class_00471cc0*>;`, both insert overloads
//   emitted, the address taken through a derived struct or from inside a
//   function, a file-scope static pointer, the class defined before <vector>
// - the element's own definition: plain class, the real class with its
//   constructor, destructor and three pure virtuals, or a struct
// - taking the vector's protected _Ufill, _Ucopy and _Destroy out of line too,
//   in all eight combinations
//
// A later attempt went past all of that by hand-rolling std::vector itself. A
// stand-in `class allocator` and a stand-in `template<class _Ty, class _A>
// class vector`, both in namespace std and written so the template arguments
// mangle to the same `?$vector@PAVClass_00471cc0@@V?$allocator@PAVClass_00471cc0@@
// @std@@@std@@` and the member to the same `?insert@...QAEXPAPAV...IABQAV3@@Z`,
// with the SP3 <vector>'s insert body, size(), _Ucopy, _Ufill, _Destroy and the
// SP3 XUTILITY fill and copy_backward copied verbatim, DOES reach the original's
// register assignment at the top of the function:
//   push ebx / push ebp / mov ebx, [esp+0x18] / mov ebp, ecx
// that is, the count in ebx and `this` in ebp, with the two moves in the
// original's order. So the original's prologue is reachable from this source
// shape, which is the strongest evidence yet that the algorithm and the member
// are right and that the remaining difference is one allocator decision.
//
// The construct that decides that prologue is `allocator::construct`, and it
// decides the body shape with it, the two cannot be had separately (all six
// combinations of construct, allocate and destroy were measured; the variants
// are in build/scratch/0x4732e0/, v1 to v4 and wa to wf):
// - construct calling the nested `_Construct(_P, _V)` placement-new helper, the
//   real XMEMORY shape, gives the body this build gives, 546 or 547 bytes, and
//   the wrong top assignment, `this` in ebx and the count in ebp: 57.1% with the
//   hand-rolled vector, 57.9% with the real <vector>
// - construct written as the assignment `*_P = _V` straight in the allocator
//   gives the original's top assignment and a 511 byte body that is 26 bytes
//   short: 38.2%. `allocate` (its own inline clamp and `operator new`, or the
//   nested `_Allocate` template) and `destroy` (empty, or the nested `_Destroy`
//   pseudo-destructor call) change neither, so they are not the lever
// With the assignment form the body then differs in about ten places at once,
// all of them downstream of the prologue: `_End - _Last` lands in ecx instead of
// eax, `_N` is built with a `lea` instead of an `add` and is spilled into the
// dead _M argument slot instead of a local, the new buffer keeps a second live
// copy in esi, the first _Ucopy loop increments before it stores and borrows
// ebp as its load temporary so `this` has to be reloaded from its slot, and the
// tail recomputes `this` into edx. Fixing the prologue alone is worth 19 points;
// fixing the body alone is not reachable, so this file keeps the 57.9% shape.
#include <vector>

class Class_00471cc0 {
public:
    int field_4;                            // +0x4
};

typedef std::vector<Class_00471cc0*> Vec_004732e0;
typedef void (Vec_004732e0::*InsertFn_004732e0)(
    Vec_004732e0::iterator, Vec_004732e0::size_type, Class_00471cc0* const&);

// FUNCTION: 0x4732e0 ?insert@?$vector@PAVClass_00471cc0@@V?$allocator@PAVClass_00471cc0@@@std@@@std@@QAEXPAPAVClass_00471cc0@@IABQAV3@@Z
InsertFn_004732e0 g_insert_004732e0 = &Vec_004732e0::insert;
