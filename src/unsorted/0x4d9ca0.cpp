// Decompiled by space-bunny-free, finished by muse-spark-1.3-free. Names are provisional.
// NOT A MATCH: 69.5% best (v18). Correct shapes found this run, allocation still open.
//
// CERTAIN (byte-exact regions in v18, verified against disassembly):
// - The grouped-bit conditions are SIGNED `i % 8`, not abs(): `i % 8` compiles to
//   exactly cdq/xor/sub/and/xor/sub with ONE cdq (see build/scratch/0x4d9ca0/abstest2.cpp,
//   g1), while abs(abs(i)&7) emits TWO cdqs. Original has one cdq in both loops.
// - `len` is unsigned (original breaks with jbe on `len <= 0x1e`; signed gives jle).
// - Loop 2 advances both walkers by one element (`q++; s++;`): the latch shares one
//   `mov eax,4` for both adds. (`q += 4` would emit add 16.)
// - `m = copied` up front as a named local helps (+4 over direct reads); the loop 2
//   bound and last-line check then read the slot exactly like the original.
// - Loop bodies use walking pointers (r/q/s) with per-iteration slot reloads;
//   `len -= strlen(p); p += strlen(p);` in that order with two strlen calls each.
// - Separator default " " is loaded before the modulo (ternary materialization).
//
// STILL WRONG (the whole remaining diff is one register-allocation decision):
// - Original keeps `this` memory-resident (spilled to [esp+0x20] in the prologue,
//   reloaded for the walker reseeds) with i in ebp, a count temp in esi, p in ebx,
//   six stack slots (sub esp,0x18). Mine keeps `this` in ebp, i in esi+memory slot.
// - Tried: up-front n (count) hurts (62.2), up-front q0 (pc) collapses (33.8),
//   up-front n+m (61.1), base=ret pointer uniformly 60.7 with len taking ebp,
//   shared single `int i` across loops (+0.9), decl-order shuffle (no change),
//   __inline RetPtr/StackPtr accessors (no change).
// - Hypothesis for next attempt: the prologue's copied/pc/count loads may be
//   MSVC-hoisted invariant loads (m succeeds as a real local but n/q0 do not),
//   so forcing them all into locals is the wrong model; the spill of `this`
//   likely needs a different pressure lever (single shared upstream cause per
//   brief lesson: the ebp/esi swap and the missing spill are one decision).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class Class_004d9ca0 {
public:
    unsigned long ret[0x1e];
    int count;
    int stack[0x800];
    int copied;
    unsigned long* pc;
    char buf[0xa44c];

    void FUN_004d9ca0();
};

// FUNCTION: 0x4d9ca0
void Class_004d9ca0::FUN_004d9ca0()
{
    char* p = buf;
    int i = 0;
    int m = copied;
    unsigned int len = 0xa44c;

    if (count > 0) {
        sprintf(p, "Call stack:\n");
        len -= strlen(p);
        p += strlen(p);
        unsigned long* r = ret;
        for (i = 0; i < count; i++) {
            if (len <= 0x1e)
                break;
            sprintf(p, "%08lX", *r);
            strcat(p, (i == count - 1 || i % 8 == 7) ? "\n" : " ");
            len -= strlen(p);
            p += strlen(p);
            r++;
        }
    } else {
        p[0] = 0;
    }
    if (m > 0 && len > 0x1e) {
        sprintf(p, "Stack dump:\n");
        len -= strlen(p);
        p += strlen(p);
        unsigned long* q = pc;
        int* s = stack;
        for (i = 0; i < m; i++) {
            if (len <= 0x1e)
                break;
            if (i % 8 == 0) {
                sprintf(p, "%08lX: ", q);
                len -= strlen(p);
                p += strlen(p);
            }
            sprintf(p, "%08lX", (unsigned long)*s);
            strcat(p, (i == m - 1 || i % 8 == 7) ? "\n" : " ");
            len -= strlen(p);
            p += strlen(p);
            q++;
            s++;
        }
    }
}
