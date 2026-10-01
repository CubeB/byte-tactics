// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, second pass
// by space-bunny-free. Names are provisional.
// Retry (deepseek-v4.1-flash, issue 2972): re-confirmed 98.6% (195 bytes both
// sides). One-byte SIB swap in the inlined erase shift: ours `mov [eax+edx],ebp`
// vs the original `mov [edx+eax],ebp`; the delta is the literal -4 in edx, and
// the base/index pick is compiler state. An outer-pointer loop, a do-while
// counter and a cached end all stay at 195 bytes / 98.6%. The standalone MATCH
// twin 0x470fb0 uses our eax form, so this is not source-reachable.
// Destroys the ten listener lists that 0x471d90 allocates into the game object
// (used by 0x471eb0, 0x471f40 and 0x471f90): every listener is deleted and
// erased from the front of its list, the ten vector members are then destroyed
// (that is the second, backward loop: the implicit ~vector of the array, which
// frees _First and zeroes _First, _Last and _End), the list object itself goes
// back to the global operator new and g_game->lists is cleared. Both loops are
// the body of the destructor at 0x470fb0 (itself MATCH), reached here through
// the inlined `delete`.
// The virtual call is a `delete` of a listener: vtable slot 0 is its scalar
// deleting destructor, called with flag 1 and only for a non-null pointer. It
// is why the list holds exactly one virtual per listener class.
// NOT MATCHED, one byte: the shift loop's store. The original has
// `mov dword ptr [edx + eax], ebp` (SIB 0x14, the pointer-difference in edx as
// the base) where this file produces `mov dword ptr [eax + edx * 1], ebp`
// (SIB 0x10). The delta is the difference MSVC keeps at run time between the
// erase's _First and _First + 1 inside the inlined std::copy; which of the two
// becomes the SIB base is decided when the inliner rewrites the induction
// variable, not by the source. All 128 header sets of tools/headers.py and every
// phrasing tried here (a local pointer instead of the field, `it = erase(it)`,
// a reference to the vector, a Clear() helper, an out-of-line destructor, an
// iterator in a raw pointer, a one-virtual-field element struct, an explicit
// allocator, `!= 0` instead of a bool test) give the SIB 0x10 form. The same
// destructor compiled out of line, 0x470fb0, matches with SIB 0x10 too, so the
// difference is an artefact of that one inlined copy.
// Retried by Claude Opus 5.5 in #349, still one byte:
// - Not compiler state: 0 to 400 unused `extern int` declarations (step 4),
//   400 to 4000 (step 60), 0 to 6000 unused prototypes (step 150), about 30
//   big header sets after <windows.h> (ddraw, dsound, dplay, dinput, mmsystem,
//   commctrl, winsock, ole2, shlobj, vfw, <string>, <list>, <map>, <iostream>)
//   and the lengths of the local names all give 98.6%.
// - The exe has 31 inlined std::copy shift loops of this shape
//   (`mov r, [p]; mov [a + b], r; add p, 4`); this is the only one with the
//   delta as the base. The other 30, including 0x470fb0 and the erase loops of
//   0x471050, 0x471eb0, 0x4715a0, 0x4716e0, 0x471fd0, 0x4720d0 and 0x472200,
//   put the moving pointer first.
// - In MSVC 5 the delta only becomes the base when the destination is
//   indexed by a counter (`P[k] = f[k]`, `*(P + k) = *(f + k)`), and every
//   such form also adds a `mov ecx, eax` copy of the source pointer that the
//   original lacks. Pointer-walking copies always give SIB 0x10: std::copy,
//   explicit `x`/`f` locals in either declaration and increment order, element
//   structs with an operator=, const_iterator sources, a p++ walk or a
//   reference over the ten vectors, erase in a for increment, dummy locals
//   before, inside or after the `if`, and earlier functions in the same file
//   that use the same vector type (erase(begin()), the out-of-line destructor).
// Retried by space-bunny-free in #1867, still one byte, 98.6%, nothing scored
// better: a dummy static function placed before the class and after the
// function (compiler state from earlier functions, the 0x4581e0 effect),
// `#include <xutility>`/`<xmemory>` in front of `<vector>`, the vector named
// through a typedef, a one-pointer element struct instead of a bare pointer, a
// `static inline` shift helper wrapping `erase`, the `for` loop with the erase
// in the increment, raw `Listener**` iterators, and headers.py over all 128
// sets (every one gives 98.6%). The shift loop is the inlined `copy(_P + 1,
// end(), _P)` of `std::vector::erase`, and the only freedom left is which
// register the frame picks as the SIB base; nothing in the source reaches it.
// Retried by space-bunny-free in #1867 (second pass, one-byte residual), still
// 98.6%:
// - The out-of-line twin of this destructor, 0x470fb0 (MATCH), emits the very
//   same loop with the opposite SIB order (`mov [eax + edx], ebp`), so the source
//   is right and only the inliner/allocator tie differs.
// - A fully hand-rolled vector (allocator at +0, _First/_Last/_End, own erase
//   with the shift loop written by hand) still gives the walker as the SIB
//   base: `mov [eax + ecx], edx`. Writing the destination FIRST in the shift
//   (`iterator dest = p; iterator src = p + 1; while (src != last_) { *dest =
//   *src; ++src; ++dest; }`), the ordering that decided the SIB of the inlined
//   `_Ucopy` at 0x435110-style vector::insert functions (guide line 1835), does
//   NOT transfer to this store. It scores 54.2% because the hand-rolled class
//   moves the list pointer from ebx to esi and loses the `mov [esp+0x1c]` spill.
// - Binding the deleted pointer to a local (`Lists* lists = g_game->lists;
//   if (lists) { delete lists; ... }`) is again 98.6% with the same one byte;
//   writing the destructor call out (`->~Lists_00471de0(); operator delete(...)`)
//   is 60.7%.
// So: the 0x4bc370 read/write split has no STORE counterpart, and neither the
// copy's operand order nor the caller's register pressure reaches it.
// Retried by deepseek-v4.1-flash in #1333, still one byte: headers.py --cpp
// (C++ headers on top of all 128 sets) and the erase(it, it+1), explicit
// std::copy+pop_back and static-helper phrasings all score worse or the same
// 98.6%. Only the plain `it = begin(); while (it != end()) erase(it)` form
// reproduces the rest of the function.
// Retried by deepseek-v4.1-flash in #2412, still 98.6%, same one byte: a saved
// `Listener* p = *it; delete p;` local keeps the byte; the index form
// (`lists[i][j]` + `erase(begin()+j)`) and the `front()`/`erase(begin())`
// while-loop drop to 37.8% (they lose a stack local and the whole frame).
// Retried by deepseek-v4.1-flash in #3167. New finding: the SIB IS reachable
// from the source. The library `copy(_P + 1, end(), _P)` lets the optimizer
// create the delta itself, and the optimizer always emits the walker as the
// SIB base. If the delta is written out explicitly (`int _K = (int)((char*)_P
// - (char*)_F); while (_F != _L) *(iterator)((char*)_F + _K) = *_F, ++_F;`)
// the address tree is `(delta + walker)` and the store becomes
// `mov [edx + eax], ebp` (SIB 0x02), exactly the original. It is reachable
// from this file by specializing `std::copy` for `Listener**` (or
// `vector<Listener*>::erase`) with that body: MSVC 5 picks the explicit
// specialization up, the function stays 195 bytes, and the residual becomes
// only the delta/end register pair (91.7%: end in edx and delta in ecx where
// the original has end in ecx and delta in edx, plus `sub ecx, eax` instead
// of `sub edx, ebx`). Both halves are therefore reachable, just not together
// yet. The library form always gives end=ecx, delta=edx, the explicit-delta
// form always gives end=edx, delta=ecx; 60 variants (delta type int/long/
// unsigned/short/char, do-while/while/for, guard before/after the delta,
// separate walker so the subtraction uses the original pointer, the delta
// computed before the end load, helper functions with every parameter order,
// and 0 to 16 preceding copy loops before the class) never mixed them.
// Scratch: build/scratch/0x471de0/spec_*.cpp, b_*.cpp, d1..d4, h4/h5,
// derived1.cpp.
// The flip is a compiler-state flag, not a register tie. In a minimal C file
// the library `copy` inlined into a caller gives the delta-base SIB
// (`sub ecx, eax; mov [ecx+eax], esi`) when the function makes no call, and
// the walker-base SIB (`mov [eax+ecx], esi`) as soon as one `call` sits
// before the loop (call11 vs call13 in build/scratch/0x471de0/copytest5.c:
// the two bodies are instruction-for-instruction identical apart from the
// call and the SIB byte). The copy's argument form matters the same way:
// `copy(q, e, p)` is walker-base, `copy(p + 1, e, p)` is delta-base. So the
// same 5 instructions can encode either way and only the compilation's state
// picks. In this file the delete call before the erase always leaves the
// walker form, which is why every source spelling lands on 98.6%; the
// original's TU state produced the delta form for this one inlined copy.
#include <vector>

class Listener_00471de0 {
public:
    virtual ~Listener_00471de0();
};

// The list object: ten std::vector members, 0x10 bytes each. The allocator is
// the first member of an MSVC 5 vector, so _First sits at +0x4 and the vectors
// run from +0x4 to +0x9f. Allocated by 0x471d90 with the global operator new.
class Lists_00471de0 {
public:
    std::vector<Listener_00471de0*> lists[10];

    ~Lists_00471de0()
    {
        for (int i = 0; i < 10; i++) {
            std::vector<Listener_00471de0*>::iterator it = lists[i].begin();
            while (it != lists[i].end()) {
                delete *it;
                lists[i].erase(it);
            }
        }
    }
};

#pragma pack(push, 1)
struct Game_00471de0 {
    char unknown_0[0x38d77];
    Lists_00471de0* lists;             // +0x38d77
};
#pragma pack(pop)

extern Game_00471de0* g_game;

// FUNCTION: 0x471de0
void FUN_00471de0()
{
    if (g_game->lists) {
        delete g_game->lists;
        g_game->lists = 0;
    }
}
