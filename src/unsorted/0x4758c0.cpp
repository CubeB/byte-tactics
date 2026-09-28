// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL, 84.0 percent (783 of 779 bytes). std::vector<Class_00473590>::
// insert(iterator _P, size_type _M, const _Ty& _X) for a 52-byte (0x34)
// trivially copyable element type, emitted out of line. Taking the member's
// address makes MSVC 5 instantiate the template here instead of inlining the
// three-argument insert into its callers (0x4737c0 is the only caller, and it
// calls this address). The element layout is copied from 0x472e30.cpp and
// 0x4737c0.cpp; the vector members are _First +4, _Last +8, _End +0xc.
//
// The first branch (the reallocate arm) is the only difference, and it is the
// known two-spelling allocator wall on this STL template (see 0x40d020.cpp,
// 0x425480.cpp and 0x4732e0.cpp). After `call operator new` this build loads
// _P into ECX:
//     mov ecx,[esp+0x20] / mov [esp+0x18],eax / mov ebx,eax / ...
// where the original loads it into EDX:
//     mov edx,[esp+0x20] / ...
// Everything downstream follows from that one choice. With _P in ecx, the
// per-element `rep movsd` copy clobbers ecx, so this build reloads _P from the
// argument slot inside the loop (`mov ecx,[esp+0x1c]`, the extra 4 bytes) and
// keeps the _Ufill counter in edx; the original instead keeps _P in edx across
// both copies and spills the _Ufill counter to the stack. The instructions are
// otherwise identical, and the middle and last branches match byte for byte
// once the 4-byte length difference is accounted for.
//
// Things tried that did NOT change the register: element type (13-int array,
// signed/unsigned shorts, chars, nested structs, class with methods, pointer
// fields), a hand-written List_004737c0::FUN_004758c0 with the same body (it
// compiles to exactly this 783-byte code), an explicit member specialization of
// insert, `iterator _P2 = _P;` locals declared before/inside the arm, a static
// inline getter for _P, preceding the real 0x475880 _Ucopy in the file, 0 to
// 512 filler struct/extern declarations, 15 header sets (windows.h, ddraw.h,
// dsound.h, stdio.h, string.h, math.h, stdlib.h, time.h, memory.h, new.h, io.h
// and combinations), and /Gz /Gd /G3..G6 /Oy /Oa /Ow /Ob1 /Ox /Gs. This needs
// the regrouping-into-original-translation-units phase.
#include <vector>

struct Vec3_00473590 {
    int x;
    int y;
    int z;
};

struct Class_00473590 {                  // one element, 0x34 bytes
    void* field_0;                       // +0x00
    Vec3_00473590 pos1;                  // +0x04
    Vec3_00473590 pos2;                  // +0x10
    Vec3_00473590 dir;                   // +0x1c
    int field_28;                        // +0x28
    int field_2c;                        // +0x2c
    int field_30;                        // +0x30
};

typedef std::vector<Class_00473590> Vec_00473590;
typedef void (Vec_00473590::*InsertFn_00473590)(
    Vec_00473590::iterator, Vec_00473590::size_type, const Class_00473590&);

// FUNCTION: 0x4758c0 ?insert@?$vector@UClass_00473590@@V?$allocator@UClass_00473590@@@std@@@std@@QAEXPAUClass_00473590@@IABU3@@Z
InsertFn_00473590 g_insert_00473590 = &Vec_00473590::insert;
