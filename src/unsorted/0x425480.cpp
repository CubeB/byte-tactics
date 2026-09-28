// Decompiled by Space Bunny Free. Names are provisional.
// std::vector<Class_004c2ea0*>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward all
// inlined. The feature list DAT_00511fb4 is this vector type (0x4251e0 is its
// out-of-line _Destroy); the only call site, 0x4222e0, loads it into ecx and
// pushes end(), 1 and the address of a new object, so this is the append a
// push_back compiles to. Taking the member's address makes the compiler emit
// the template instantiation out of line, as the original file did, and the
// member pointer has to return void or VC5 resolves the two-argument insert.
//
// Partial (57.9%, 547 of 537 bytes). NOTE: this is not an isolated case, it is
// the same unsolved problem as src/unsorted/0x4732e0.cpp, which sits at the
// same 57.9% with the same 547-against-537 byte count and the identical
// `this` in ebp / count in ebx swap on the same STL template. 0x4732e0's own
// file records that the earlier attempt there already established this is not
// reachable from a single file: it tried the unpatched compiler
// (BT_TOOLCHAIN=msvc5-rtm), the exe's own neighbourhood in emission order in
// one file, a real caller that inlines the insert, and spelling the default
// allocator out explicitly, and concluded it needs the regrouping-into-original-
// translation-units phase. The two should be held for that phase rather than
// re-issued independently. For contrast the game's other instantiation of the
// same template, 0x4c4d70, matches at 100% with the *other* assignment
// (this in ebx, count in ebp), so both variants exist in the exe and only one of
// them is reachable from a single file.
//
// The only difference here is which callee-saved
// register the allocator gives `this` and the count. The original does
// `push ebx; push ebp; mov ebx, [esp+0x18]; mov ebp, ecx` (count in ebx, this
// in ebp), this build does `push ebx; mov ebx, ecx; push ebp; mov ebp, [esp+0x18]`
// (this in ebx, count in ebp), and everything else follows from that: the
// original's block order, its `mov ebp, [esp+0x10]` reloads in the fill and
// copy_backward loops, its two reloads against this build's four (which is the
// whole 10-byte size difference) and the pop order all read as that one swap.
// A normalised diff with ebx and ebp exchanged instruction for instruction
// leaves nothing else: same branches, same pointer sums, same
// operator new[]/operator delete calls.
//
// It is the rarer of the two spellings and the build is invariant here, so
// this is the same wall as 0x4732e0, 0x40d020 and 0x425210. What this attempt
// added to the list of things that do not move it:
// - the element type is irrelevant: vector<int*>, vector<void*>, vector<char*>,
//   vector<short*> and vector<double*> all give this build's assignment, as
//   do a one-field class, a class with four fields, a class with a constructor
//   and destructor, a struct, the allocator spelled out, and the pointer taken
//   from inside a function, through a derived class, from a file-scope static
//   pointer, both insert overloads, `template class std::vector<...>;` and a
//   second instantiation of the same vector in the same file
// - the /Gz <xutility> of the original file (the __stdcall copy, copy_backward,
//   fill, fill_n, _cpp_min/_cpp_max, mismatch block from 0x424c00.cpp, with
//   _XUTILITY_ defined) in front of the real <vector>, and the sibling
//   vector<unsigned short> insert (0x425210) in the same file: unchanged
// - <windows.h> and <ddraw.h> before <vector>, which is what flips one of the
//   sum orders in 0x40d290: unchanged
// - an explicit member specialisation of insert in namespace std, with the
//   template body written out by hand, so the function is compiled as a plain
//   function rather than as a template instantiation: same assignment, and
//   the same mangled name
// - 100 files with 0, 1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192,
//   256, 384, 512 and 768 filler declarations before the instantiation (class
//   definitions, plain function definitions, inline function definitions,
//   class definitions with a method, extern variables), testing the "flips
//   every 256 declarations" state that 0x40d290's notes report: every one
//   gives the identical 547 bytes
// - a local copy of <vector> with the body respelled as
//   `size() + _M > capacity()`, or as the VC6 growth
//   `capacity() - size() < _M` with `(size() < _M ? _M : size())`: both give
//   575 bytes, so the codegen moves and the assignment still does not
// This needs the regrouping-into-original-translation-units phase.
#include <vector>

class Class_004c2ea0 {                 // the element, only its size is used
public:
    int field_0;
};

typedef std::vector<Class_004c2ea0*> Vec_00425480;
typedef void (Vec_00425480::*InsertFn_00425480)(
    Vec_00425480::iterator, Vec_00425480::size_type, Class_004c2ea0* const&);

// FUNCTION: 0x425480 ?insert@?$vector@PAVClass_004c2ea0@@V?$allocator@PAVClass_004c2ea0@@@std@@@std@@QAEXPAPAVClass_004c2ea0@@IABQAV3@@Z
InsertFn_00425480 g_insert_00425480 = &Vec_00425480::insert;
