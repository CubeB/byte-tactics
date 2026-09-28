// Decompiled by deepseek-v4.1-flash. Names are provisional.
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
// The same template instantiated for 0x34 (0x4758c0, matched) and 0x44-byte
// elements (0x475ef0) uses the `(_P - _Q) + dest - _M` shape, so the choice is
// tied to this instantiation's register assignment (this in ebp, _M in edi),
// not to the template. Nothing moved it: the real preceding function 0x4758c0
// compiled first in the same file, 0 to 4096 extra declarations, every
// <windows.h>/<stdio.h>/... header set before and after <vector> (768 in
// tools/headers.py plus the C++ headers), element types from int[15] through
// the real 0x4743a0 record and non-trivial copy constructors, a hand-written
// body of the same template with the destination bound to a local, and
// /G3../G6 /Gz /Gr. The value is identical either way; it looks like compiler
// state from the original translation unit, which needs the
// regroup-into-original-files phase to reproduce.
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
