// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.
// Sends one effect-descriptor dword to the "Microsoft Game Device" driver that
// 0x4e38e0 opened (\\.\GDPERF, see the caller at 0x4e36c0). Two device families
// are handled, selected by DAT_00529ea0, which holds the CPU family nibble:
// 5 gets a two-call sequence, 6 a single call. 0x4e3930 takes its value as one
// 64-bit argument: it puts the low dword in the middle of its 12-byte input
// buffer and the high dword in the last, which is why every call here builds a
// 64-bit value and sign-extends it with cdq.
//
// The two branches each have their own `unsigned char sub`, the six bits (or
// eight bits, for family 6) of the effect word at +16. Each is live across the
// calls that follow it, so MSVC 5 gives it a stack slot: the frame only has the
// two dwords of `val` in it, and the byte lands in the argument slot of
// `level`, which is dead after `mov esi, [esp+0x18]`. Family 5 stores it before
// the first call and reloads it with `and eax, 0xff` after the compare, family 6
// stores and reloads it inside one expression. The type has to be `unsigned
// char`: with a signed one the reload is a `movsx eax, bl` off the register
// copy instead.
//
// The flag terms are written `(f2 ? 0x40 : 0) | (0x80 & (f1 ? -1 : 0))` rather
// than the more obvious `(f2 ? 0x40 : 0) | (f1 ? 0x80 : 0)`. Both compile to the
// same `neg al; sbb eax, eax; and eax, K` pair, but MSVC 5 canonicalises the
// operands of a commutative operator and always emits the larger mask first, so
// the plain spelling puts f1's 0x80 term first and reverses the original's
// order. With the second term written as a mask on the left of an AND against
// `(f1 ? -1 : 0)` the two terms no longer have the same shape, the sort does
// not fire, and the source order survives. Spelling both terms that way does
// not work; exactly one of the pair has to keep the plain `?:` shape.
#include <windows.h>

extern bool __cdecl ReadGdperf(DWORD a, void* out);
extern bool __cdecl WriteGdperf(DWORD a, __int64 b);
extern char DAT_00529e9c;
extern int DAT_00529ea0;

// FUNCTION: 0x4e3750
bool __cdecl ProgramPerfEvent(unsigned int effect, unsigned int level, char f1, char f2)
{
    __int64 val;
    if (level > 1)
        return false;
    if (!DAT_00529e9c)
        return false;
    if ((effect >> 28) != (unsigned int)DAT_00529ea0)
        return false;
    if (((level + 1) & (effect >> 8) & 3) == 0)
        return false;
    if (DAT_00529ea0 == 5) {
        unsigned char sub = (unsigned char)((effect >> 16) & 0x3f);
        ReadGdperf(0x11, &val);
        val &= (level ? 0xffffu : 0xffff0000u);
        WriteGdperf(0x11, val);
        WriteGdperf(0x12 + (level != 0), 0);
        val |= (f2 ? 0x40 : 0) | (0x80 & (f1 ? -1 : 0));
        if (sub == 0x3f)
            return WriteGdperf(0x11, val | 0x100);
        return WriteGdperf(0x11, val | sub);
    }
    if (DAT_00529ea0 == 6) {
        unsigned char sub = (unsigned char)(effect >> 16);
        int v = (0x4400 | (effect & 0xff)) * 0x100
              | (f1 ? 0x10000 : 0) | (0x20000 & (f2 ? -1 : 0))
              | sub;
        return WriteGdperf(0x186 + (level != 0), v);
    }
    return false;
}
