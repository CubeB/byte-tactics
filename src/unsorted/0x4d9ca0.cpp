// Decompiled by space-bunny-free, finished by muse-spark-1.3-free, deepseek-v4.1-flash. Names are provisional.
// GPT-6 retry: retained 69.5%. Whole-formatter wrappers and per-loop
// append helpers did not improve allocation; original spill pattern remains.
// NOT A MATCH: 69.5% best (v18). Correct shapes found this run, allocation still open.
//
// deepseek-v4.1-flash (second run): re-derived from the disassembly and re-tested the
// pressure hypothesis. Confirmed the original frame is len=0x10, m=0x14, r/q=0x18,
// n/s=0x1c, this=0x20, pc=0x24 with ONLY p (ebx) and i (ebp) in registers; every value
// used only at a loop boundary (this, len, n, m, pc) stays memory-resident. Declaring
// n/m/q0 as top locals (v1: 59.8%, 630 bytes) makes MSVC promote len into ebp and this
// into esi and gives i edi plus a slot, i.e. it does the exact opposite. Same result for
// a different declaration order (v2 59.8), all-locals-at-function-scope (v5 60.6), a
// free __fastcall copy (v3, identical to thiscall, 69.5) and /Gr /Gz /Gd flags (no
// change for a member fn). Declaring only q0 on top of the 69.5 body collapses to 33.8
// (v7), matching the earlier note. So the missing lever is not "declare n/m/pc": it is
// making i win ebp over len/this while the boundary-only locals stay spilled.
//
// deepseek-v4.1-flash (first run): re-derived the whole function from the disassembly and
// swept ~60 source shapes. The 69.5% body below is still the best. What I confirmed:
// - The original caches BOTH counts in stack slots: n=count at [esp+0x1c], m=copied at
//   [esp+0x14], and pc at [esp+0x24]; this is spilled at [esp+0x20] and i lives in ebp.
//   Frame is 6 dwords (sub esp,0x18); ours is 4 (sub esp,0x10) with this in ebp.
// - Declaring `int n = count;` (or n+m, or n+m+q) drops the score to 61.1 and shrinks the
//   code to 629 bytes: the extra local makes MSVC give len ebp and this esi, it does NOT
//   reproduce the original's spill. Direct member reads for count (below) stay at 69.5.
// - tools/headers.py: 128 sets tried, all 69.5 (and 72 failed to compile), so no header fix.
// - N-declarations sweep (0..400 extern int dummies) is FLAT at 69.5, so this is not
//   compiler state; the source shape or the allocator's choice is what differs.
// - Declaring r/s/q at function scope, shared vs per-loop i, int vs unsigned len, self
//   pointer, inverted if/else, while loops, m via a reference: all <= 69.5.
// Remaining single cause (per the brief's lesson): this must lose ebp to i, and len/n/m/pc
// must all stay memory-resident. Every construct I found that lowers this's register weight
// also promotes len into ebp; the two are one decision I could not split from the source.
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

// deepseek-v4.1 (third run, 10-minute box): re-read the disassembly and re-measured the
// allocator priority. The frame slots are certain (len=frame+0, m=copied=+4, r/q=+8,
// n=count/s=+0xc, this=+0x10, pc=+0x14) and the original really does keep this, m, pc,
// len and the walkers in memory with ONLY p=ebx, i=ebp and a temporary n=esi.
// Confirmed again that adding `int n = count;` (variant a, 59.4%, frame 0x14) makes MSVC
// give EBP to len and ESI to this, i.e. our allocator ranks len > this > n while the
// original ranks i > n > len/this; no source-level lever found in the time box for that
// priority flip, so the 69.5% body below stands (ours: frame 0x10, this=ebp, i=esp+0x18,
// r=esp+0x14, len=esp+0x10; every mid-body [esp+N] is therefore 4 low).
//
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
