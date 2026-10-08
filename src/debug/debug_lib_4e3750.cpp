// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.
// Sends one effect-descriptor dword to the "Microsoft Game Device" driver that
// 0x4e38e0 opened (\\.\GDPERF, see the caller at 0x4e36c0). Two device families
// are handled, selected by DAT_00529ea0, which holds the CPU family nibble:
// 5 gets a two-call sequence, 6 a single call.
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
        // Each branch has its own unsigned char sub; a signed type changes the reload.
        unsigned char sub = (unsigned char)((effect >> 16) & 0x3f);
        ReadGdperf(0x11, &val);
        val &= (level ? 0xffffu : 0xffff0000u);
        WriteGdperf(0x11, val);
        WriteGdperf(0x12 + (level != 0), 0);
        // Exactly one term uses the mask-and-(?: -1) form: keeps the source operand order.
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
