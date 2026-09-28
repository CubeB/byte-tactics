// Decompiled by space-bunny-free. Names are provisional.
// Sends one effect-descriptor dword to the "Microsoft Game Device" driver that
// 0x4e38e0 opened (\\.\GDPERF, see the caller at 0x4e36c0). Two device families
// are handled, selected by DAT_00529ea0, which holds the CPU family nibble:
// 5 gets a two-call sequence, 6 a single call. 0x4e3930 takes its value as one
// 64-bit argument: it puts the low dword in the middle of its 12-byte input
// buffer and the high dword in the last, which is why every call here builds a
// 64-bit value and sign-extends it with cdq.
#include <windows.h>

extern bool FUN_004e38e0(DWORD a, void* out);
extern bool FUN_004e3930(DWORD a, __int64 b);
extern char DAT_00529e9c;
extern int DAT_00529ea0;

// Still differs (89% of the bytes match; check.py run 4 of 15):
//  - the char local holding bits 16..21 of the effect word. The original keeps
//    it in a dead argument slot, one per branch, and the two branches do not
//    agree: the family-5 branch stores it with `mov byte ptr [esp+0x20], bl`
//    (the fourth argument's slot) after the 0x4e38e0 argument pushes, but its
//    fall-through arm reloads `mov eax,[esp+0x18]; and eax,0xff; cdq`, which
//    is the *second* argument's slot, the one the family-6 branch writes. A
//    plain char local is register-allocated here (`movsx eax, bl`) and never
//    stored, so 4 bytes are missing and the reload disappears.
//  - the order of the two flag terms. The original evaluates the 0x40 term
//    (fourth argument, `mov al,[esp+0x2c]`) first and the 0x80 term (third
//    argument, `mov cl,[esp+0x28]`) second. This version always loads the
//    third argument first, in every source spelling of the `|` I tried
//    (both operand orders, `?: 0 : 0` against `& -`, `+` against `|`, and
//    named int locals in both orders). MSVC 5's evaluation order for a
//    commutative expression follows the declaration order of the operands'
//    definitions, and the two parameters are both plain arguments here, so
//    the load order does not move.

// FUNCTION: 0x4e3750
bool FUN_004e3750(unsigned int effect, unsigned int level, char f1, char f2)
{
    char sub;
    if (level > 1)
        return false;
    if (!DAT_00529e9c)
        return false;
    if ((effect >> 28) != (unsigned int)DAT_00529ea0)
        return false;
    if (((level + 1) & (effect >> 8) & 3) == 0)
        return false;
    if (DAT_00529ea0 == 5) {
        __int64 val;
        sub = (char)((effect >> 16) & 0x3f);
        FUN_004e38e0(0x11, &val);
        val &= (level ? 0xffffu : 0xffff0000u);
        FUN_004e3930(0x11, val);
        FUN_004e3930(0x12 + (level != 0), 0);
        val |= ((f2 != 0) ? 0x40 : 0) | ((f1 != 0) ? 0x80 : 0);
        if (sub == 0x3f)
            return FUN_004e3930(0x11, val | 0x100);
        return FUN_004e3930(0x11, val | (char)sub);
    }
    if (DAT_00529ea0 == 6) {
        sub = (char)(effect >> 16);
        int v = (0x4400 | (effect & 0xff)) * 0x100
              | ((f1 != 0) ? 0x10000 : 0) | ((f2 != 0) ? 0x20000 : 0)
              | (unsigned char)sub;
        return FUN_004e3930(0x186 + (level != 0), v);
    }
    return false;
}
