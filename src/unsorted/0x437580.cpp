// Decompiled by Claude Opus 5.5, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// std::vector<Class_00437820>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, for the 8-byte {string handle, int} element of the
// static vector at 0x5122c0 (see 0x434a30.cpp); its only caller, 0x4373a0,
// does a push_back. The element's copy constructor (0x437820) and operator=
// (0x437800) are the compiler-generated ones: they compile byte-identical to
// those two addresses and come out in the same order (insert, operator=,
// copy constructor). Taking the member's address makes the compiler emit the
// template instantiation out of line.
//
// PARTIAL (78.9%). Two things still differ:
// - Registers in the reallocating branch: the original keeps _P in edi from
//   the first _Ucopy loop to the start of the third (so _Ufill counts in ebp
//   and reloads _X from the stack), while this build reloads _P into ecx after
//   every copy-constructor call. The other branches are identical. This build
//   is byte-identical to the game's other insert of the same element shape,
//   0x488fb0 (the twin static vector at 0x51e6b0, same helpers 0x4c91a0,
//   0x4c93b0, 0x4c9390), so the difference is not in the template. Header
//   sets (tools/headers.py and <windows.h>, <string>, <map>, <iostream>,
//   <list>), 0 to 1200 dummy externs, dummy types or functions, compiler
//   flags, element destructor/copy/assignment variants and forcing the
//   helpers out of line first all leave it at 78.9%.
// - 0x437800 is named Class_00437800::Class_00437800 in data/symbols.csv, but
//   it is Class_00437820::operator= (??4Class_00437820@@QAEAAV0@ABV0@@Z), so
//   the three operator= references would still be reported wrong.
//
// deepseek-v4.1-flash follow-up (still 78.9%):
// - The whole 10-byte gap (639 vs 649) is inside the reallocating prologue:
//   the fast path from 0x4376ed to the end is byte-identical. In the original
//   the first _Ucopy keeps its end (_P) in edi, a callee-saved register, so the
//   copy-constructor loop needs no reload and _Ufill then takes the count in
//   ebp and reloads the value; this build (and 0x488fb0's matched twin) keeps
//   _P in ecx, reloads it after every call, and cascades into the third
//   _Ucopy dest/src register split. Same source, different allocation.
// - uv run tools/headers.py 0x437580 --cpp tried 768 header sets, all 78.9%.
//   Explicitly declaring the element's copy constructor, operator= and
//   destructor instead of letting them be implicit also leaves 78.9%. This is
//   TU compiler state, the same wall as 0x408f30 / 0x40cca0 / 0x40d290.
//
// deepseek-v4.1-flash, batch of TU/AST perturbations (all exactly 78.9%,
// 649 bytes, identical diff):
// - <vector> included after the class definitions; <windows.h>, <string> and
//   <map> before <vector>; spelled-out std::allocator argument; element copy
//   constructor declared, and copy constructor defined inline in the class;
//   1 to 20 dummy static function definitions before the instantiation; a
//   dummy ElemDummy_00437580 vector instantiated first; the address-taken
//   global made static and used. None moves _P from ecx to edi.
//   Diagnosis: the original's first _Ucopy end (_P) is in edi and its _M
//   counter in ebp with &_X re-read per iteration; our allocator puts _P in
//   ecx (reloaded after every copy-constructor call), _M in edi and hoists
//   &_X into ebp. All four callee-saved registers are allocated differently,
//   so no local spelling of the instantiation reaches it.
//
// deepseek-v4.1-flash retry (still 78.9%, 649 bytes): moved the body out of
// the class with an explicit member specialization and re-tested element
// declaration combos (only copy ctor, only operator=, only an inline
// destructor, copy ctor + destructor). All 78.9% with the identical diff.
// Hand-expanding the first _Ucopy loop made it worse (673 bytes, 73.5%).
// The allocation of {_P, _M, _X} over {edi, ebp} is fixed by TU state.
// deepseek-v4.1-flash (this run): best 79.3%, 649 bytes. Everything outside the
// reallocating branch is now byte-identical to the original modulo the 10-byte
// shift; the only difference left is that branch's register allocation.
//
// The vector class is now a hand-written clone (as in the matched 0x476210.cpp)
// so that the parameter order of the inlined _Ufill at the first in-place
// branch can be flipped to (size_type _N, iterator _F, const _Ty& _X). That one
// change makes the second in-place branch's load order match the original
// (79.3% vs 78.9%, 123 vs 125 diff lines). A /Fa listing confirms the clone is
// otherwise the library template line for line.
//
// The reallocating branch still differs as the earlier notes describe: the
// original keeps _P in edi from the load after operator new through the third
// _Ucopy, so _M counts in ebp and &_X is re-read per iteration; this build
// keeps _P in a caller-saved register (ecx, reloaded after every constructor
// call), gives edi to the _Ufill counter and hoists &_X into ebp.
//
// Measured this run (each variant scored by check.py, about 2 s each):
// - hand-written clone baseline: reports 648 bytes / 78.6%, but its instruction
//   bytes are identical to the <vector> build's (the one-byte size difference
//   is layout padding after the function), so the true baseline is 78.9%.
// - _Ucopy with the destination parameter first (iterator _P, const_iterator
//   _F, const_iterator _L) at the third copy: 638 bytes, 72.5%. It does move
//   _P into a callee-saved register and re-reads &_X per iteration, but the
//   assignment becomes _P=ebp, _S=edi, counter=ebx: one rotation away from the
//   original's _P=edi, _S=ebx, counter=ebp.
// - All 64 combinations of _Ucopy/_Ufill parameter order at the six inlined
//   call sites give exactly one of those two allocations. 256 combinations
//   that also replace fill/copy_backward with hand-written equivalents are
//   worse (best 75.7%, 636 bytes); 320 combinations with explicit loops at the
//   prefix and third copies and expression variants (&_Q[_M], _M + _Q, a
//   destination local, nested _Ufill inside the third _Ucopy, _Q assigned
//   after declaration, a size_type local for the fill count) are all 79.3% or
//   worse. So the third copy's parameter order is the only lever that moves
//   this allocation, and it moves it to a fixed second shape, not the
//   original's.
// - 300 dummy function definitions before the instantiation (the global
//   variable counter moves by 6 each) do not change the 638-byte variant at
//   all; 0 to 512 extern int declarations and 0 to 160 dummy functions do not
//   change the <vector> build. The numbering is real (the /Fa listing shows
//   __N$4666/__S$4667, and 4696/4697 with 5 dummies) but shifting it uniformly
//   leaves every variable's relative order unchanged.
// - element class variants (extra constructor + SetChecksum as in
//   0x4373a0.cpp, handle class derived from Class_004c9390): no change.
// - explicit instantiation does not compile; forcing the instantiation from a
//   call site makes MSVC inline insert and emit _Ucopy/fill/copy_backward
//   instead of the out-of-line insert, so the address-taken global is needed.
// - BT_TOOLCHAIN=msvc5-rtm compiles this file to the same 649-byte build
//   (79.3%), so the difference is not a compiler-version difference either.
// - custom allocator types (one derived from std::allocator, one written from
//   scratch) change the code shape (663 and 652 bytes), so the original used
//   plain std::allocator.
// - the extra members the real <vector> has (the 1-arg insert, erase, size,
//   capacity, empty) do not change the 649-byte build.
// - all six parameter permutations of the prefix and third _Ucopy (so every
//   binding position of the bound, the source and the destination), all six of
//   the in-place _Ufill, three of each in-place _Ucopy, dead extra parameters
//   in every position, a pointer instead of a reference for _X, algebraic
//   variants of the fill count and of the third copy's destination, and a
//   different element copy constructor (declared, not implicit) all stay at
//   79.3% or below. The only lever that changes the family is the third
//   copy's parameter order; it yields 638 bytes / 72.5%, not the original.
// - a randomized search of 600 variants over all six parameter arrangements of
//   each of the four inlined _Ucopy sites plus four of each _Ufill site, and
//   class-layout variants (typedef order, the
//   final assignments written as &_S[_N], a size_type cast on deallocate's
//   argument) all stay at 79.3% or below. Replacing fill/copy_backward with
//   hand-written helpers of the same body is much worse (661/657/636 bytes),
//   so the header versions must stay. Nothing produces 639 bytes except
//   the third copy's destination-first order, which gives 638 / 72.5%.
// - flag sweep with check.py --flags: /Oa gives 623 bytes (19.3%), /Os and /O1
//   give 558 (9.4%), /G6 654 (74.1%), /Ge 656 (73.9%); /Ow, /Ot, /Oy, /Og,
//   /Ox, /Ob1, /G5, /GB, /GA all give the same 649 / 79.3%. No flag set
//   reproduces the original, so this is not a flag difference.
// - the in-place _Ufill order also moves the _P reload ahead of the _M reload
//   in the reallocating branch (the diff shrinks from 125 to 123 lines), which
//   shows the numbering does reach that branch, but it never changes the
//   register choice.
//
// Best lead for the next attempt: the destfirst third copy proves the original
// allocation is reachable in principle (P in a callee-saved register, &_X
// spilled), so the missing piece is one more rotation of the register web
// order, which needs a lever that changes web partitioning rather than the
// argument order. The sibling 0x46f7a0 (same edi family, 83.8%) is stuck the
// same way; the guide already records this family as compiler state.
#include <climits>
#include <memory>
#include <xutility>

class Class_004c9390 {
public:
    char* data;
    void FUN_004c9390();
};

class Class_004c91a0 {
public:
    char* ptr;
    Class_004c91a0(const Class_004c91a0& other);
    Class_004c91a0& operator=(const Class_004c91a0& other);
    ~Class_004c91a0() { ((Class_004c9390*)this)->FUN_004c9390(); }
};

class Class_00437820 {
public:
    Class_004c91a0 handle;             // +0x0
    int field_4;                       // +0x4
};

namespace std {

template<class _Ty, class _A = allocator<_Ty> >
class vector {
public:
    typedef vector<_Ty, _A> _Myt;
    typedef _A allocator_type;
    typedef _A::size_type size_type;
    typedef _A::difference_type difference_type;
    typedef _A::pointer iterator;
    typedef _A::const_pointer const_iterator;
    typedef _A::reference reference;
    typedef _A::const_reference const_reference;
    typedef _Ty value_type;
    explicit vector(const _A& _Al = _A())
        : allocator(_Al), _First(0), _Last(0), _End(0) {}
    size_type size() const
        {return (_First == 0 ? 0 : _Last - _First); }
    iterator begin() { return (_First); }
    iterator end() { return (_Last); }
    void insert(iterator _P, size_type _M, const _Ty& _X)
        {if (_End - _Last < _M)
            {size_type _N = size() + (_M < size() ? size() : _M);
            iterator _S = allocator.allocate(_N, (void *)0);
            iterator _Q = _UcopyA(_First, _P, _S);
            _UfillA(_Q, _M, _X);
            _UcopyA(_P, _Last, _Q + _M);
            _Destroy(_First, _Last);
            allocator.deallocate(_First, _End - _First);
            _End = _S + _N;
            _Last = _S + size() + _M;
            _First = _S; }
        else if (_Last - _P < _M)
            {_UcopyA(_P, _Last, _P + _M);
            _UfillB(_M - (_Last - _P), _Last, _X);
            fill(_P, _Last, _X);
            _Last += _M; }
        else if (0 < _M)
            {_UcopyA(_Last - _M, _Last, _Last);
            copy_backward(_P, _Last - _M, _Last);
            fill(_P, _P + _M, _X);
            _Last += _M; }}
protected:
    void _Destroy(iterator _F, iterator _L)
        {for (; _F != _L; ++_F)
            allocator.destroy(_F); }
    iterator _UcopyA(const_iterator _F, const_iterator _L, iterator _P)
        {for (; _F != _L; ++_P, ++_F)
            allocator.construct(_P, *_F);
        return (_P); }
    iterator _UcopyB(iterator _P, const_iterator _F, const_iterator _L)
        {for (; _F != _L; ++_P, ++_F)
            allocator.construct(_P, *_F);
        return (_P); }
    void _UfillA(iterator _F, size_type _N, const _Ty& _X)
        {for (; 0 < _N; --_N, ++_F)
            allocator.construct(_F, _X); }
    void _UfillB(size_type _N, iterator _F, const _Ty& _X)
        {for (; 0 < _N; --_N, ++_F)
            allocator.construct(_F, _X); }
    _A allocator;
    iterator _First, _Last, _End;
    };

} // namespace std

typedef std::vector<Class_00437820> Vec_00437580;
typedef void (Vec_00437580::*InsertFn_00437580)(
    Vec_00437580::iterator, Vec_00437580::size_type, const Class_00437820&);

// FUNCTION: 0x437580 ?insert@?$vector@VClass_00437820@@V?$allocator@VClass_00437820@@@std@@@std@@QAEXPAVClass_00437820@@IABV3@@Z
InsertFn_00437580 g_insert_00437580 = &Vec_00437580::insert;
