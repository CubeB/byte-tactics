// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and space-bunny-free. Names are provisional.
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
// space-bunny-free (still 78.9%): the twin settles what is left. The game's
// other insert of the same element shape, 0x488fb0, has THIS build's exact
// register family (edi the third _Ucopy's destination, esi its source and
// its end, _Q reloaded into ecx after every call), while 0x437580 is the
// mirror image (edi the source, esi the destination). The two originals are
// otherwise identical instruction for instruction: same helpers, same element
// size, same argument slots. The one value that ties the two loops together
// is _Q: it is the first loop's end AND the third inlined _Ucopy's source,
// so whichever of edi/esi the third _Ucopy gives its source is the register
// _Q survives in from the first loop, and the original never reloads it. The
// remaining gap is that esi/edi tie, and the game's own two copies disagree
// about it, so no source in this file can decide it (see "the esi/edi
// priority tie" in docs/agent-guide.md). Note also that the third inlined
// _Ucopy copies [_Q, _Endofstorage) to _New, not [_Mid, _Endofstorage): the
// compiler forms the end as _Q - _Mid + _New - _M * 8, which is where
// `sub edi, ebx / add edi, esi / sub edi, eax` comes from.
// uv run tools/headers.py 0x437580 (the default 128 sets): nothing above
// 78.9%.
#include <vector>

class Class_004c9390 {
public:
    char* data;

    void FUN_004c9390();
};

// The reference-counted string handle: copy constructor 0x4c91a0, operator=
// 0x4c93b0 (named Class_004c93b0::FUN_004c93b0 in data/symbols.csv), release
// 0x4c9390.
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

typedef std::vector<Class_00437820> Vec_00437580;
typedef void (Vec_00437580::*InsertFn_00437580)(
    Vec_00437580::iterator, Vec_00437580::size_type, const Class_00437820&);

// FUNCTION: 0x437580 ?insert@?$vector@VClass_00437820@@V?$allocator@VClass_00437820@@@std@@@std@@QAEXPAVClass_00437820@@IABV3@@Z
InsertFn_00437580 g_insert_00437580 = &Vec_00437580::insert;
