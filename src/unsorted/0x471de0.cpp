// Decompiled by space-bunny-free, finished by DeepSeek V4.1 Flash, verified by GPT-6, finished by claude-opus-5-5, finished by GPT-6. Names are provisional.
// #5435 Codex retry: re-confirmed 98.6%; the sole SIB byte still needs an
// artificial compiler symbol-count state.
// #5410 Codex retry: re-confirmed 98.6%; the one-byte SIB order and the
// specialization's swapped register pair remain the only known outcomes.
// #5426 Codex retry: re-confirmed 98.6%; no new source form reaches the
// original SIB order while preserving the register pair.
// #5395 retry: 98.6% remains best. Issue #4841 confirms the explicit std::copy
// specialization reaches the delta-base SIB, but swaps the end/delta registers
// and scores 91.7%, so the original's combined state remains unreachable.
// Symbol ids, read with `c2prio.py --symbols` (docs/c2-regalloc.md, "Symbol
// ids"; Claude Opus 5.5): the count below is the file total, the front end's
// symbol count at the end of the file (5055 here), which seeds the ids C2
// gives its own symbols; it reaches 5482 by this function's allocation.
// Declarations at the end of the file alone give the window: MATCH for
// totals 65257..65554 (and 65556), i.e. 64859 to 65156 symbols of headers
// counting what they add at the end of the file, where <vector> gives 4657.
// Nearest real sets: what TA's imports suggest (<windows.h> <ddraw.h>
// <dsound.h> <dplay.h> <shlobj.h> <imagehlp.h>, six CRT headers, <vector>
// <list> <map> <algorithm> <string>) gives 42209; every DirectX, shell,
// CRT, STL and old iostream header together (docs) gives 53234, a total of
// 53632 here (98.6%), still 11625 short. Only kitchen-sink sets with MAPI,
// LAN Manager, TAPI and ODBC headers get there.
// GAVE UP (claude-opus-5-5, #5160), 98.6%. The source is right; the one byte
// left is compiler state. The SIB register order of the store in the inlined
// erase's shift loop (original delta-base `mov [edx+eax], ebp`, ours
// walker-base `mov [eax+edx], ebp`) depends on the translation unit's symbol
// count, which C2 keeps in 16 bits:
//  - every declaration in the TU counts, before or after the function (an
//    unused `extern int` or an enumerator is one, structs and prototypes
//    more), and the effect wraps every 65536;
//  - with <vector> alone the original order appears only with 60204 to 60500
//    extra symbols (unused `extern int`s or enumerators; checked with
//    check.py, which printed MATCH at 60250), and with the full <windows.h>
//    on top only at about 32100 to 32350 more;
//  - no set of real system headers tried reaches that: the full <windows.h>
//    adds about 28000, and ddraw, dsound, dplay, d3d, commctrl, math and the
//    CRT headers only tens to hundreds each (400 random header sets, all
//    walker-base). So the count most likely came from Cavedog's own headers,
//    which we do not have, and the docs forbid committing dummy declarations;
//  - in small tests the function's own local symbols push the other way (more
//    locals, delta-base), which is why the original's other erase loops in
//    this TU (0x471eb0, 0x471fd0) are walker-base;
//  - path length, /Gi, /Z7, /Zd, /GR, /Gy, /Gf, /J, /Op and a precompiled
//    header do not change it.
// Scratch drivers: build/scratch/0x471de0/{win,winb,wenum,gz,gl,gid,cf}.py
// (win.py sweeps the extern count over 0 to 65535 for a header set).
// GPT-6 retry: confirmed 98.6%; the single SIB difference remains the base
// and index order at the inlined erase shift.
// #5381 retry: re-confirmed 98.6%; the inlined erase still emits the walker-
// base SIB instead of the original's delta-base encoding.
// #5364 retry: re-confirmed 98.6%; the inlined erase still uses the walker-
// base SIB, while the original's compiler-state encoding uses the delta base.
// GPT-6 retry recheck: current main still emits the walker-base SIB. Prior
// scratch sweeps show the opposite base requires a compiler state this inline
// erase does not reach, while the standalone destructor twin matches.
// #5311 Codex retry: current main still emits the walker-base SIB at 98.6%;
// the saved source and compiler-state sweeps remain the best available result.
// #5272 Codex retry: re-confirmed 98.6%; the single SIB base/index order is
// unchanged. Prior header, symbol-count, and erase-shape sweeps cover this
// compiler-state mismatch.
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
// Measured again by space-bunny-free in #4376, still one byte, 98.6%. Built a
// whole-function harness (build/scratch/471de0: sweep.py compiles one variant
// per directory with /Fa, 0.2 s each, and prints three binary features read off
// the listing instead of the percentage: which register holds the end pointer,
// which holds the -4 delta, and which SIB slot the delta lands in). 196
// variants measured. The compiler itself is deterministic here: 12 parallel
// compiles of this file all give the same SIB, so nothing below is luck.
// THE MODEL THAT FITS ALL OF THEM: one bit, "is the -4 a source-level integer
// or an optimizer temp?", fixes the SIB slot AND the end/delta register pair.
//  A. delta = a source-level `int` (a byte-delta specialisation of
//     `std::copy` for `Listener**`): the delta takes the BASE slot
//     (`mov [ecx+eax],ebp`), the end pointer takes edx and the delta ecx
//     (`mov edx,[esi]`, `sub ecx,eax`).
//  B. delta = the optimizer's induction-variable temp `edi - ebx` (the library
//     `copy(_P + 1, end(), _P)`): the walker takes the base slot
//     (`mov [eax+edx],ebp`), the end pointer takes ecx and the delta edx
//     (`mov ecx,[esi]`, `sub edx,ebx`).
// The original is A's slot with B's registers, and no spelling produces it.
// Two consequences worth keeping:
//  * The ENTRY TEST is not what flips it. A byte-delta body wrapped in an
//    explicit `if (_F != _L) { int _K = ...; do {...} while (...); }` keeps
//    the guard, stays 195 bytes and scores 91.7% - the whole residual is the
//    five instructions of the ecx/edx pair, including the wanted
//    `mov [edx + eax], ebp`. build/scratch/471de0/spec_guard.cpp. An
//    UNGUARDED do-while also reaches A's slot but drops the `cmp`/`je`, so it
//    is 70 instructions.
//  * The delta's OPERAND is independent of both: `int _K = (char*)_X -
//    (char*)_H` with `Listener** _H = _F` walking a second copy `_S` gives A's
//    slot with the library's `sub ecx, ebx` operand (build/scratch/471de0/
//    tb_hs_dowhile). So it is the delta's kind, not the register the
//    subtraction reads, that fixes the slot.
//  * Only a byte-scaled address reaches A's slot. An element-unit delta
//    (`_S + _K` on a `Listener**`) is a SCALED index, x86 forces it into the
//    index slot, and the store is walker-base `mov [eax+ecx]` again - which is
//    why 0x4bc370's rule ("`ptr[int local]` is emitted as `[int + ptr + 0]`, the
//    integer in the base slot") is the right lead: the delta has to look like a
//    source-level integer on an unscaled address.
// Also flat, each measured, all with B intact:
//  * tools/headers.py --cpp over all 1536 sets (C++ headers x C headers).
//  * the per-TU function-order effect that fixed the SIB base at 0x4bc370: 22
//    variants compiling the real neighbours 0x471d00, 0x471d10, 0x471d50,
//    0x471d70, 0x471d90, 0x471eb0, 0x471f40, 0x471f90, 0x471fd0 before or after
//    this function (each alone, the pool trio, all of them).
//  * the compiler build: BT_TOOLCHAIN=msvc5-rtm emits the same walker-base SIB.
//  * flags: /O2 /Ob1 and /O2 with /G3 /G4 /G5 /G6 /G7 /Ot /Ow /Gs all give
//    `[eax+edx]` (/O1, /Os and /Ob0 do not compile this shape).
//  * 110 spellings of the two bodies: delta type int/unsigned/long/short, byte
//    versus element units, guard / while / do-while / for / early return /
//    goto / break, the delta declared before or after a walker local and an end
//    local, the delta written first or subtracted, one or two induction
//    variables, two end pointers live at once, a counter, `const`, `register`,
//    a read-modify-write destination, the delta through a helper.
//  * tools/permute.py on this file, 14 minutes, 7712 candidates (statement and
//    declaration moves, split and merged declarations and initialisers, scope
//    changes, commutative swaps, negated ifs, temporaries, inline helpers,
//    do/while rewrites, 1069 header sets, zero compares, casts, sign flips,
//    goto polarity, dead declarations): every one equal or worse, no MATCH.
// Scratch: build/scratch/471de0/{sweep.py,variants*.py,dump.py,spec_guard.cpp,
// tb_hs_dowhile.cpp}; build/permute/0x471de0/{best.json,stats.json}.
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
// Retried by DeepSeek V4.1 Flash in #4800, still one byte, 98.6%. permute.py
// (3 min, 2186 candidates, statement/declaration/scope/commutative rewrites)
// found nothing. ~40 hand variants of the explicit specialization (int delta
// in the library `copy` slot: delta type, declaration order, delta computed
// from _L / from a dest walker / inside the guarded body, for and do-while and
// index forms, `(char*)` vs `(int)` subtraction, separate walker and end
// locals) all give A's slot with the swapped register pair (91.7%) or worse,
// never the original's end=ecx / delta=edx. Diagnostic: deleting the
// `delete *it` call entirely (build/scratch/0x471de0/n01) still emits the
// walker-base SIB `[eax+edx]`, so the minimal-file "one call before the loop"
// lever does NOT transfer to this function; the inlined copy here always
// takes the walker form regardless of the call. Scratch:
// build/scratch/0x471de0/{run.py,drive.py,n01_nodelete.cpp,...}.
// GPT-6 retry (#5160): rechecked at 98.6% (195 B); the walker-base SIB remains.
// #5340 retry: re-confirmed 98.6%; the inlined erase still emits the walker-
// base SIB, while the original's delta-base encoding remains TU-state-specific.
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
