// Decompiled by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash: 83.1% / 334 bytes (was 72.4%). The one change that
// matters: the body store is `buf[i] = *s;`, not `buf[i] = c;`. With `c` the
// optimizer knows c == *s at the loop top and fuses the 0xff test into
// `cmp byte ptr [..], 0xff`, dropping the load; with `*s` it must materialise
// the value, yielding the original's `mov al,[..] / cmp al,0xff / mov [..],al`.
// That also fixes the whole rest of the loop shape; the body is now
// instruction-for-instruction the original's with only the two known defects
// below. Removing the tail hack still collapses `text` out of ebp (61.4%),
// so it is still required. Declaration order of i/c/s remains inert.
//
// What still differs (72.4% note, still true):
// 72.4% match. Builds a word-wrapped copy of `text` in a buffer allocated
// from the pool: the number of characters per line is width / (width of one
// digit), and a line is broken at a space or '-' when a word would exceed
// `width` pixels. `index` selects the current font entry (FUN_004a1810) when
// it is not -1.
//
// The head (through the memset) matches the original exactly once `text` is
// forced to stay live across the whole loop: the original keeps `text` in
// ebp (its only reads are the initial `*text` and `s = text`), which demotes
// `len` to esi and `index` to edi. A trailing use of `text` after the loop
// reproduces that allocation. What still differs:
//   * the loop counter and the input pointer have swapped callee-saved
//     registers (i in edi and s in esi here; the original has i in esi and
//     s in edi). Declaration order, int/unsigned/long, and adding live
//     locals all failed to swap them.
//   * the trailing `if (text == buf) buf[0] = 1;` that keeps `text` live is a
//     real instruction pair the original does not have (8 extra bytes).
// All other bytes are exact; the wrap-back loop and the tail match.
//
// space-bunny-free, 72.4%, same two diffs, no improvement:
//   * Removing the trailing use entirely (so `text` is only read by `*text`
//     and `s = text`) collapses `text` out of ebp entirely and the score falls
//     to 60.8%: with only those two adjacent uses MSVC 5 copy-propagates
//     `text` into `s` and `text` dies at the last call, so the head rewrites
//     (`mov ebp,[esp+0x10]` becomes `mov esi,[esp+0x14]`). The original really
//     does keep the parameter in ebp, so some use of it past the calls is real
//     and the cheapest one available is that 8-byte pair.
//   * The esi/edi swap of `i` and `s` is completely insensitive to the source
//     shape. All of these emit the identical 333 bytes, same swap: the six
//     declaration orders of i/c/s/p, `unsigned`/`long` for i, `unsigned char*`
//     for s, `const char*` for s, and splitting the declarations from the
//     assignments. It is not a tie-break on definition order. What the
//     allocator seems to use is the number of IR references: `i` is
//     referenced more often (3 incs, 2 decs, 6 indexed stores, 2 leas) than
//     `s` (1 def, 4 incs/decs, 5 loads) yet `s` wins esi, so a reference has
//     to be added to `s` or removed from `i` without emitting an instruction,
//     and no zero-cost spelling of that was found.
//   * Rewriting the loop as `while (1) { if (c == 0) break; ... }` (technique
//     9) does not swap them either, it only adds 2 bytes.
//   * The def of `i` is emitted in the slot between the two halves of the
//     inlined memset and the def of `s` just after the loop-entry `test al,al`;
//     in this version those two slots are the other way round. Whatever
//     decides which variable wins that slot is upstream of everything the
//     source spelling can reach from here.
//   * Scoring note: rewriting the 0xff test as `if (c == (char)0xff) break;`
//     scores 73.2% / 332 bytes, one byte better, but it keeps `c` in al across
//     the latch and loses the original's `mov al,[edi]` reload at the top of
//     the loop body, so the instruction sequence stops being the original's
//     with two registers renamed. This file keeps the exact-shape version,
//     which is one register fix away from a match, not two.
//
// space-bunny-free, second pass, 72.4% / 333 bytes, 1 check.py run, no change.
// Built the free oracle (build/scratch/0x4ac4c0, d3m.py for 0x4ac4c0) and
// classified the difference. The whole function is instruction-for-instruction
// the original's, with only two defects, and their sizes are known exactly:
//   1. (b) register rename, size neutral: `i` and `s` have the callee-saved
//      registers swapped (original i=esi, s=edi; here i=edi, s=esi). It touches
//      24 instructions, all of the same length.
//   2. (c) three extra instructions at the tail, exactly 8 bytes: the
//      `if (text == buf) buf[0] = 1;` below emits
//      `cmp dword ptr [esp+0x18], ebx / jne +3 / mov byte ptr [ebx], 1`.
//      333 - 325 = 8, that is all of the size difference, so the original
//      really has no code there.
// FIRST DIVERGENCE: original 0x4ac547 `xor esi,esi` (i = 0, in the gap between
// the two halves of the inlined memset) where this file emits `mov esi,ebp`
// (s = text) in that slot and puts `xor edi,edi` (i = 0) where the original
// has `mov edi,ebp`. So the register choice and the schedule are ONE decision:
// the two defs are emitted as a pair that swaps between the gap slot and the
// slot just after the `mov al,[ebp]` load, and whichever lands in the gap also
// wins esi (ebp is text, ebx is buf in both). Class (b) with a (d) schedule
// swap of the same two defs, not (a) and not (c).
// New negative results, all free-oracle runs, every one the identical 333
// bytes with the same swap:
//   * all six orders of the initialised declarations of i, c, s, declarations
//     split from their assignments, `p` declared first, and `i` declared with
//     no initialiser at the top of the function and assigned in place. The
//     emitted order of the two defs is (s = text, c = *text, i = 0) in every
//     one, so the preheader schedule cannot be reached from source order at
//     all. Hoisting `int i = 0` to the top of the body does change the
//     allocation (text takes esi, index takes ebx) but costs 338 bytes/52.2%.
//   * `unsigned` / `long` for i, `unsigned char*` and `const char*` for s, a
//     `const char*` parameter, and an explicit cast on the copy: none of them
//     blocks the copy propagation or moves a def.
//   * `i += 0;` and `i = i + 0;` as no-op extra references to i: inert.
//   * compiler state: `extern int dummyN;` blocks for N = 0,1,2,3,4,5,6,7,9,
//     11,16,24,32,48,64,96,128,192,256, all identical.
//   * swapping the declarations of `wrapped` and `m` in the inner block: inert.
//   * spreading three defs across the preheader with fresh locals to probe
//     which one the scheduler picks for the gap: inert, the two defs always
//     come out as the same pair.
// What the copy-propagation question really needs, confirmed here: with the
// tail hack in place the copy is NOT propagated (text stays in ebp and the
// `mov esi,ebp` copy survives), and without it the copy IS propagated. So
// liveness is the only thing that blocks it, and no spelling of the copy
// blocks it: the comma `(text, text)`, the constant conditional
// `len ? text : text`, a doubled `s = text; s = text;` and a cast all still
// propagate (all 321 bytes, 60.8%, `mov ebp,[esp+0x10]` becomes
// `mov esi,[esp+0x14]`). The original therefore has a use of `text` after the
// calls that the front end records and the optimiser deletes, which emits
// nothing; a plain comparison or store cannot be it, since it costs bytes.
// The register choice between i and s is independent of that: it is the same
// in the 321-byte variant and in this one.
#include <string.h>

struct Gadget_004ac4c0;
struct Dialog_004ac4c0 {
    int unknown_0;
    Gadget_004ac4c0* gadgets;
};
struct Menu_004ac4c0 {
    char unknown_0[0x18];
    Dialog_004ac4c0* dialog;
};

void* FUN_004d83b0(char* name, unsigned int size);
int __stdcall FUN_004a1810(Gadget_004ac4c0* gadgets, int index);
int __stdcall FUN_004a5030(unsigned char* text);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, unsigned char* text);

// FUNCTION: 0x4ac4c0
char* __stdcall FUN_004ac4c0(Menu_004ac4c0* menu, char* text, int width, int index)
{
    Gadget_004ac4c0* gadgets = menu->dialog->gadgets;
    int len = strlen(text);
    if (index != -1)
        FUN_004a1810(gadgets, index);
    int w;
    if (index == -1)
        w = FUN_004a5030((unsigned char*)"d");
    else
        w = FUN_004c1480(FUN_004c1440(), (unsigned char*)"d");
    int size = len + 3 * (len / (width / w)) + 2;
    char* buf = (char*)FUN_004d83b0("WordWrap", size);
    memset(buf, 0, size);
    int i = 0;
    char c = *text;
    char* s = text;
    char* p = buf;
    while (c != 0) {
        if (*s == (char)0xff)
            break;
        buf[i] = *s;
        char next = s[1];
        i++;
        s++;
        if (next == ' ' || next == '\n' || next == '-') {
            int wrapped = 0;
            int m;
            if (index == -1)
                m = FUN_004a5030((unsigned char*)p);
            else
                m = FUN_004c1480(FUN_004c1440(), (unsigned char*)p);
            if (width <= m) {
                buf[i] = 0;
                wrapped = 1;
                char ch = s[-1];
                i--;
                s--;
                while (ch != ' ' && ch != '-') {
                    buf[i] = 0;
                    ch = s[-1];
                    i--;
                    s--;
                }
                buf[i] = '\r';
                i++;
                buf[i] = '\n';
                i++;
                s++;
            }
            if (wrapped)
                p = buf + i;
        }
        c = *s;
        if (c == '\n')
            p = buf + i + 1;
    }
    if (text == buf)
        buf[0] = 1;
    buf[i] = 0;
    return buf;
}
