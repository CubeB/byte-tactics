// Decompiled by space-bunny-free, finished by space-bunny-free. Names are
// provisional.
//
// 80.1% (799 bytes against 798). Both non-reallocating branches, the whole
// prologue and every call target are byte identical; the single byte and every
// remaining diff are the register allocator's choice inside the reallocating
// branch, so both branch targets downstream of it are off by one.
//
// std::vector<Class_0046ded0>::insert(iterator, size_type, const value_type&)
// from MSVC 5's <vector>, emitted out of line as its own COMDAT: the callee at
// 0x4b4f10 is allocator.allocate's operator new and the one at 0x4b4f20 is
// deallocate's operator delete, both reached through the header.
//
// The element class is 0x5c bytes and its three out-of-line members are the
// copy constructor 0x470390, the copy assignment 0x470040 and the destructor
// 0x46ded0. data/symbols.csv gives the copy assignment to Class_0046eaa0 and
// the copy constructor to Class_0046ded0, so the two are modelled here as a
// base and an empty derived class, which is the only arrangement in which both
// mangled names come out right for one 0x5c-byte element. The operator= below
// is the crucial line: written as an implicit member it is emitted as its own
// out-of-line COMDAT (40.2%, and fill()/copy_backward() then call
// ??4Class_0046ded0@@ instead of 0x470040); written by hand it is one call to
// the base's and /Ob2 inlines it, which takes the score to 80.1% and puts all
// six copy-construction and three assignment loops on the right callees.
//
// What still differs, all in the reallocating branch: the original keeps _P in
// edi across the allocate call (`mov edi,[esp+0x24]` after it), and MSVC here
// spills _P to [esp+0x20] instead and reuses edi for the counter of the inlined
// _Ufill loop, which the original spills to [esp+0x20]. The two values are
// swapped between a callee-saved register and the frame, so one priority
// decision in the allocator is responsible for every diff here, including the
// *23 sign-fixup register (ecx against edx) and the order of the two stores to
// _End and the size() reload that follow. Tried and did not change it:
// the copy assignment as an implicit member, insert reached by a plain call
// (which inlines it into the caller and leaves no COMDAT at all), and a cdecl
// function-pointer typedef, which VC5 rejects (C4234, then C2563).
// What would need hand-rolling: the vector's own insert body, so that _Ufill's
// counter is prevented from coalescing with the caller's _M, which is what
// costs _P its callee-saved register here.
//
// Second run, still 80.1%, but it fixed a LATENT SYMBOL BUG worth recording:
// Class_0046eaa0 must be spelled `struct`, not `class`. check.py prints the
// original's references from the disassembly and its own from the object, and
// at +0x26c/+0x2e6/+0x303 they now both read
// ??4Class_0046eaa0@@QAEAAU0@ABU0@@Z. The byte score is identical either way
// (80.1%), so the mismatch is invisible to a percentage and only shows up in
// that reference list, but it is a wrong name and would have blocked a MATCH
// outright. Mangled names encode class-vs-struct as V against U in the return
// and parameter type of an assignment operator, so a "class"-spelled base
// silently costs a U. Worth checking this list on any function whose
// assignment operator is a hand-written forwarding wrapper.
//
// Also tried this run, both free scratch scores, both no better than 80.1%:
// headers.py over all 128 sets (best is 80.1%, four sets tie), operator= taking
// the base type as its parameter (40.2%, the assignment then gets a second
// mangled name), a default constructor on the derived class, and spending the
// /Ob2 inline budget on a dead __inline helper called three times, which was
// the obvious candidate for the allocator decision and is a no-op here.
//
// Fourth run (space-bunny-free), still 80.1%, all free scratch scores except the
// one baseline. Frame arithmetic pinned down, and a suspected original bug found.
// THIS FRAME: with sub esp,0xc plus four pushes the frame is 0x1c, so the
// return address sits at [esp+0x1c] and the three arguments at +0x20 (_P), +0x24
// (_Ns) and +0x28 (_X). The three locals are [esp+0x10] = this (spilled, because
// ecx does not survive the operator new call), [esp+0x14] = the allocation
// element count and [esp+0x18] = the new first pointer; [esp+0x1c] is unused.
// The two mid-function reads that look odd are just the call-argument slot still
// pushed: `mov edi,[esp+0x24]` at 0x46f85e is 4 bytes before the `add esp,4`, so
// it is argument 1, _P, not _Ns. Both loop counters in the reallocating branch
// live in argument home slots that are dead by then ([esp+0x20] at 0x46f88e,
// [esp+0x24] at 0x46fa46), which is why the original has no separate frame slot
// for them and why the register allocator is so tight there.
// SUSPECTED ORIGINAL BUG: at 0x46f8ca the third loop of the reallocating branch
// computes its destination as _New_finish + (_P - _New_start) - _Ns*0x5c, which
// is identically _P, the POINTER INTO THE OLD BUFFER, not into the new one
// (ebx at 0x46f86c is the operator new result, so `sub edi,ebx` is the new
// first pointer and not _Myfirst). The tail above _P is therefore copied onto
// itself, and the tail of the new buffer at [_New_start + _M + _Ns, _Mylast) is
// never constructed, while _Mylast at 0x46f95b is advanced over it. The
// disassembly is unambiguous, and the source that produces it is
// `_Ucopy(_P, _M_finish, _New_start + _Ns + (_P - _New_start) - _Ns)`, i.e. the
// new buffer is subtracted where _Myfirst belongs.
// TRIED, no better than 80.1% (free scratch scores): a fully hand-rolled
// std::vector plus std::allocator, the only way to get the body, the branch
// structure and the ternary layout all at once. It gets the frame right once
// the empty allocator is made the FIRST member of the class (an empty member is
// one byte, so _Myfirst then sits at +4 exactly as the original has it, and that
// is where VC5's INCLUDE/VECTOR puts it: `vector(...) : allocator(_Al), _First,
// _Last, _End`) and once placement new is declared inline by hand
// (`inline void* operator new(unsigned, void*) { return p; }`, since a plain
// declaration makes every construct site call ??2@YAPAXIPAX@Z instead). It then
// matches the prologue and both non-reallocating branches, and scores 38.2% on
// 941 bytes, needing the three capacity ternaries to lay out branch-first
// (the original jumps to the computation and falls through to the zero) and the
// /Ob2 budget to keep _Ucopy inlined. Not finished.
// Third run (muse-spark-1.3-free), still 80.1%, all free scratch scores:
// emitting insert through a file-scope member pointer global (the 0x408f30
// recipe) and through a derived access struct both score exactly 80.1% with a
// byte-identical diff, so the emission path is not the lever. A two-statement
// operator= body (call, then return *this) folds to the same inline and also
// scores 80.1%. int[0x17] instead of char[0x5c] for the base storage, and
// adding <string> or <algorithm> on top of <vector> (the headers headers.py
// does not cover), all score exactly 80.1%. The COMDAT body is fully
// determined by the header plus T: every variant tried lands on the same 799
// bytes, so the remaining allocator decision (_P in edi with the _Ufill
// counter spilled, against our _P in ecx with the counter in edi) is out of
// reach from this file, the same residue 0x408f30 kept at 88.7%.
#include <vector>

struct Class_0046eaa0 {                // 0x5c bytes, the base subobject
public:
    char unknown_0[0x5c];

    Class_0046eaa0& operator=(const Class_0046eaa0& rhs);
};

class Class_0046ded0 : public Class_0046eaa0 {   // 0x5c bytes, no members
public:
    Class_0046ded0(const Class_0046ded0& other);
    ~Class_0046ded0();

    Class_0046ded0& operator=(const Class_0046ded0& rhs)
    {
        return (Class_0046ded0&)Class_0046eaa0::operator=(rhs);
    }
};

// One indirect call, so insert is emitted as its own out-of-line copy.
typedef void (std::vector<Class_0046ded0>::*InsertFn_0046f7a0)(Class_0046ded0*,
                                        std::vector<Class_0046ded0>::size_type,
                                        const Class_0046ded0&);

void FUN_0046f7b0(std::vector<Class_0046ded0>* v, Class_0046ded0* p,
                  std::vector<Class_0046ded0>::size_type n, const Class_0046ded0& x)
{
    InsertFn_0046f7a0 f = &std::vector<Class_0046ded0>::insert;
    (v->*f)(p, n, x);
}

// FUNCTION: 0x46f7a0 ?insert@?$vector@VClass_0046ded0@@V?$allocator@VClass_0046ded0@@@std@@@std@@QAEXPAVClass_0046ded0@@IABV3@@Z
