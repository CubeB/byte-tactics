// Decompiled by space-bunny-free. Names are provisional.
// std::vector<Class_00471cc0*>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward inlined; the
// sixteen push_back sites call it out of line (they inline the count-is-one
// overload instead). Taking the member's address makes the compiler emit the
// template instantiation out of line, as in the original file. The member
// pointer must return void: spelled with an iterator return, VC5 resolves the
// wrong overload (C2563, or C2440 with a cast), because it prefers the
// two-argument insert.
//
// Still differs (57.9%): the original puts `this` in ebp and the count in ebx
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
