// Decompiled by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash (issue 799, solved): the three-instruction tail diff is
// gone. The fix is one source reordering that the earlier passes missed: feed
// the CSE temp into rect.d first, copy it to rect.b, then add c to rect.d:
//     rect.d = c * index + 0x28;
//     rect.b = rect.d;
//     rect.d = rect.d + c;
// This is the tail the previous passes listed as unreachable. Reading rect.b
// for the sum makes the sum depend on the rect.b store, so the scheduler emits
//     mov eax,[esp+0x24] / mov [esp+0x14],eax / add eax,edi
// (store rect.b before the add, add drains the temp into eax) and keeps the
// rect.a store above the argument pushes, exactly as the original does. The
// earlier passes only tried rect.b -> rect.d (rect.d = rect.b + c), which
// lets the allocator coalesce the sum into edi and sink both stores. Writing
// rect.d from the expression and rect.b from rect.d reverses the dependency.
//
// space-bunny-free (issue 799, third pass): the tail is one cause, not three,
// and the cause is the SCHEDULING of the rect.b store, not the add's spelling.
// The add's destination register is a symptom: the add is `t + c` (t is the
// spilled c*index+0x28 temp in the dead first-argument slot), so the allocator
// can put it in eax only if t is already dead, and t is dead only once
// `mov [rect.b], eax` has been emitted. Ours emits that store after the
// argument pushes, so t stays live across the add, so the allocator commutes
// the commutative add into edi and the add and the store both move down.
// Evidence: rewriting the last two statements as
// `rect.d = c*index + 0x28; rect.d += c;` (same values, add still t + c) but
// dropping the `rect.b = ...` assignment makes MSVC emit exactly the
// original's `add eax,edi` plus the rect.d store after the pushes, because t
// then has a single use. So the add's register is downstream of the store's
// position, and one lever (hoist the rect.b store above the pushes) would fix
// all three instructions.
// Measured this pass, all free scratch scores, 96 variants:
//  * 24 orders of the four tail assignments x three rect.d spellings. Only
//    A-first orders reach 93.7%, and there ACBD, ABCD and ABDC are all 93.7%:
//    the IR order of the last three statements is irrelevant, which is further
//    evidence the difference is the allocator's commute and not statement
//    order. Any order with C or B before A swaps esi/edi and drops to 86-90%.
//  * `rect.d = rect.b + c` and `rect.d = c + rect.b` are byte-identical (item
//    27 of the brief in reverse: the operand order is not a lever here).
//  * `rect.d = c*index + 0x28 + c` (the add folded, no read of rect.b) gives
//    74-88.9% at 200/198 bytes, never better.
//  * `int w` before the call, after rect.a, after rect.b, split into two
//    statements, `w *= 2`, `x - w - w`, `* 200 / total` (91.2%, 194 bytes),
//    an `int x = width; x -= 0x5a` split, a cast read of rect.b, a nested
//    block, `(int)(x+5)` in the call, `rect.d = c; rect.d = rect.d + rect.b`
//    and a compound `rect.d = t; rect.d += c` with rect.b still assigned
//    (70.4%, 194 bytes) are all 93.7% or worse.
//  * a named `int y` local for c*index+0x28 is 42-56% wherever it is placed,
//    which is the frame evidence above: the temp must stay an IR temp.
// TRAP FOR THE NEXT WORKER: a variant that drops the `rect.b = ...`
// assignment entirely scores 97.6%, the best number seen on this function,
// and is wrong. It is 192 bytes, not 196, and the rect.b store is missing.
// Always read the byte count in the check.py header, not just the percentage.
// GPT-6-Luna tested alternate operand and store orders. Both scored below
// this file's 93.7% best, so the prior source shape is retained.
// Draws one labelled bar: FUN_004c1450 returns the current text line height
// (c); the empty bar is outlined in white, the label is drawn, then the fill
// bar for g_game->values[index] is drawn at 100/total scale.
//
// The temporary c*index+0x28 has no local slot of its own: the original CSEs
// the two occurrences into a temp and spills it into the dead first-argument
// slot, which is why the frame is exactly the 4 dwords of rect (the original
// has no `int y` local).
//
// Still differs from the original in the last block (93.7%, both 196 bytes,
// three instructions, all one effect). The original drains the spilled temp
// into eax, stores eax to rect.b, then accumulates into that same register
// (`add eax,edi`) for rect.d, and it keeps the rect.b and rect.a stores ABOVE
// the three argument pushes. This compiler instead writes the sum into edi
// (`add edi,eax`) and sinks all three rect stores below the pushes, which
// forces the temp to stay live across the add and so forces the add to pick
// edi as its destination. Get the stores hoisted and the add follows.
//
// space-bunny-free re-tested this with about 60 free scratch variants and none
// beat 93.7%: all 24 orderings of the four tail assignments (with `w` as a
// named local and again with the divide folded into rect.a, 48 variants),
// `c + rect.b` and `rect.b + c` (identical code, so the operand order is not
// the lever, brief item 27 in reverse), a copy of the temp through a local,
// `rect.d = (rect.b = c * index + 0x28) + c`, a repeated second occurrence
// (74%, it demotes ESI to EDI and breaks the first block), stores through a
// `Rect*` local, a nested block around the tail, `const` on x/c/w, and a
// nested block around the call. Ordering that puts `rect.a` first is required
// for the original's non-destructive `mov ecx,esi / sub ecx,eax`; any order
// with `rect.c` first lets MSVC clobber esi instead and drops to 88%.
//
// deepseek-v4.1-flash (issue 799 re-run): the tail is not reachable by source
// order or spelling. tools/headers.py tried 128 header sets (all 93.7); the
// N-declarations test with 0..400 extern ints AND 0..3000 unused prototypes is
// flat at 93.7, so it is not compiler state. A 1152-case grid over all 24 tail
// statement orders x four rect.d spellings x three rect.b spellings x two
// call-argument spellings stays at 93.7 (only A-first orders reach it). Reading
// rect.b back through a local, a pointer, a reference, a cast, a struct copy,
// an inline GetB/Add/SetB helper, and in-expression assignment all still emit
// `add edi,eax` and sink the rect.a/rect.b stores past the pushes. const,
// register and `+=` spellings change nothing. The destination of the add is
// therefore not source-reachable from this shape; see the note in the pull
// request. Everything above is the best known result, 93.7%.
//
// deepseek-v4.1-flash (issue 799 re-run): about 1500 further free scratch
// variants were scored with no change to the 3-instruction tail diff: named
// spill temps at every placement (before/after the call, const, reference,
// self-assigned), pointer, reference and cast reads of rect.b to force a
// reload, all 24 first-block store orders (only the current a,c,b,d order
// matches the first block, the rest fall to 67.6), every w and divide spelling
// (including `w *= 2`, `2 * (...)`, `<< 1`), 298 hand-picked STL and Windows
// header sets, preceding function definitions, and dead inline helpers. All
// score 93.7 or worse. The one thing that DOES reach MATCH is the compiler flag
// /Gi: compiling this exact source with `/O2 /Ob2 /MT /Gi` prints MATCH, and
// the /Gi listing emits the original tail (`mov eax,[esp+0x24]` /
// `mov [esp+0x14],eax` / `add eax,edi`, with the rect.a and rect.b stores above
// the argument pushes). /Gi is not the project's flag: it breaks 0x4d1480
// (99.3), so the original was not built with it globally. The residual is a
// scheduler/coalescing decision that /Gi flips and that no source spelling
// found so far flips under the fixed /O2 /Ob2 /MT.
struct Rect_0046b900 {
    int a;                          // +0x0
    int b;                          // +0x4
    int c;                          // +0x8
    int d;                          // +0xc
};

#pragma pack(push, 1)
struct Game_0046b900 {
    char unknown_0[0x37e1f];
    int width;                      // +0x37e1f
    char unknown_37e23[0x38d89 - 0x37e23];
    int total;                      // +0x38d89
    int values[8];                  // +0x38d8d
};
#pragma pack(pop)

extern Game_0046b900* g_game;

int FUN_004c1450();
void __stdcall FUN_004bf8c0(int surface, Rect_0046b900* rect, int color);
void __stdcall FUN_004c14f0(int surface, const char* text, int x, int y, int maxWidth);
void __stdcall FUN_004bf6f0(int surface, Rect_0046b900* rect, int color);

// FUNCTION: 0x46b900
void __stdcall FUN_0046b900(int surface, const char* text, int index)
{
    Rect_0046b900 rect;
    int x = g_game->width - 0x5a;
    int c = FUN_004c1450();

    rect.a = x - 0xc8;
    rect.c = 0x27f;
    rect.b = 0x26;
    rect.d = c * 9 + 0x29;
    FUN_004bf8c0(surface, &rect, 0xff);

    FUN_004c14f0(surface, text, x + 5, c * index + 0x28, -1);

    int w = g_game->values[index] * 100 / g_game->total * 2;
    rect.a = x - w;
    rect.c = x;
    rect.d = c * index + 0x28;
    rect.b = rect.d;
    rect.d = rect.d + c;
    FUN_004bf6f0(surface, &rect, index + 1);
}
