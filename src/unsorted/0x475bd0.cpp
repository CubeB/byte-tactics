// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// std::vector<Record_00475bd0>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward inlined. The
// vector's 0x3c-byte element is the frame record of 0x4743a0 (the exe's only
// caller passes end(), 1 and a record). Taking the member's address emits the
// template instantiation out of line, as the original file did.
//
// PARTIAL (91.5%): every instruction matches except the source pointer that the
// inliner builds for the third _Ucopy in the reallocation branch,
// `_Ucopy(_P, _Last, _Q + _M)`. The original computes it as
// `(_P + _Q + _M) - _Q - _M` (lea eax,[esi+edx]; sub eax,ebx; sub eax,ecx);
// this build computes `(_Q + _M) - _Q + _P - _M` (mov eax,edx; sub eax,ebx;
// add eax,esi; sub eax,ecx), one byte longer, which shifts every later jump
// displacement by one; no jump-target difference remains once that is fixed.
// The same template instantiated for 0x34 (0x4758c0) and 0x44 (0x475ef0)
// elements uses a third shape, `(_P - _Q) + dest - _M` in both exes, again not
// the one this toolchain emits, so the choice is not this instantiation's
// register assignment either. Nothing moved it: the real preceding function
// 0x4758c0 compiled first in the same file, 0 to 4096 extra declarations, every
// <windows.h>/<stdio.h>/... header set before and after <vector> (768 in
// tools/headers.py plus the C++ headers), element types from int[15] through
// the real 0x4743a0 record and non-trivial copy constructors, a hand-written
// body of the same template with the destination bound to a local, and
// /G3../G6 /Gz /Gr. The value is identical either way; it looks like compiler
// state from the original translation unit, which needs the
// regroup-into-original-files phase to reproduce.
//
// Space Bunny Free, second pass: the remaining difference is the *term order*
// of MSVC 5's reassociation pass, and it is a property of the compiler build,
// not of the source. Compile-only sweeps (no source shape scores better than
// the file as it stands, so no check.py runs were spent on them):
//   - this template instantiated for element sizes 4, 8, ... 72 bytes: all 18
//     emit the identical `mov eax,edx / sub eax,ebx / add eax,esi / sub eax,ecx`;
//   - eight element types at 0x3c bytes (int[15], void*[15], float[15],
//     double[7]+char, __int64[7]+int, a class with a member function, a union,
//     a char/short/long mix, and a class whose copy constructor is a memcpy):
//     every one of them emits the same four instructions, so neither the
//     element's alignment (4 or 8) nor its member count nor a user-defined
//     copy constructor moves the term order;
//   - the exe's other two instantiations of the same template (0x34 bytes at
//     0x4758c0 and 0x44 bytes at 0x475ef0) also order their four terms
//     differently from what this toolchain produces, and differently from each
//     other, which is what a compiler-build difference looks like: the source
//     of all three is one STL header;
//   - a micro-probe of the reassociation pass (14 spellings of
//     `p + d - q - k` as pointers, as char* differences and as long
//     arithmetic, with 4- and 60-byte element types) never produces a chain
//     whose first operation is the add of the two pointers, which is what the
//     original's `lea eax,[esi+edx]` requires: the pass always emits the
//     subtractions first and moves the add into the middle or the tail.

#include <vector>

struct Record_00475bd0 {
    int field_00;
    int field_04;
    int field_08;
    int field_0c;
    int field_10;
    int field_14;
    int field_18;
    int field_1c;
    int field_20;
    int field_24;
    int field_28;
    int field_2c;
    int field_30;
    int field_34;
    int field_38;
};

typedef std::vector<Record_00475bd0> Vec_00475bd0;
typedef void (Vec_00475bd0::*InsertFn_00475bd0)(
    Vec_00475bd0::iterator, Vec_00475bd0::size_type, const Record_00475bd0&);

// FUNCTION: 0x475bd0 ?insert@?$vector@URecord_00475bd0@@V?$allocator@URecord_00475bd0@@@std@@@std@@QAEXPAURecord_00475bd0@@IABU3@@Z
InsertFn_00475bd0 g_insert_00475bd0 = &Vec_00475bd0::insert;
