// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 94.7%, 206 bytes (code size exact). Walks a chunked file's marker
// table looking for the record tagged "data" and returns that record's 4 byte
// header field (the record length), or 0 when the walk runs past the table.
//
// SIXTH PASS (space-bunny-free, one real check.py run for the baseline plus
// free check.py --sym scoring of 39 variants). Still 94.7%, 206 bytes, the same
// two diffs, so nothing below changed the file. New negatives, all free:
//   * the update spelling: "size = size + 8", "8 + size", "(unsigned)(size+8)",
//     "size + 4 + 4" and a named temp (t = size; t += 8; size = t;) are all
//     byte identical with the same edi temp.
//   * sinking the "mov edi, 0x14" copy cannot be had together with the
//     register form of the update. Putting "pos = 0x14;" after the len read is
//     the only spelling that moves the copy next to the strncmp (as the
//     original has it), and it also folds the update to
//     "add dword ptr [size],8": 77.3% / 202 bytes. So the copy's position and
//     the update's register form are ONE allocator decision, which is why the
//     previous passes could only trade one for the other.
//   * adding a second live value across the update does move the temp, but only
//     as far as ecx: "pos = 0x14;" (or a "char *pt = tag" feeding the strncmp)
//     hoisted above "size += 8" both give
//       mov ecx,[esp+0x10] / push 0xc / add ecx,8 / push esi / mov [esp+0x10],ecx
//     i.e. the temp leaves edi for ecx and the load's displacement flips to
//     the positive one. 94.7%, still 206 bytes, still the same diff count, and
//     nothing ever reached edx, so edx is not simply the next free register
//     after edi and ecx in this function's state.
//   * also flat at 94.7%: a goto top-tested loop, the loop in a nested block,
//     "pos = 8 + 8 + 4", "pos = 0; pos += 0x14", a dead "int extra = 0" before
//     the update, the tag as an unsigned int cast to char* at the strncmp,
//     "register unsigned int size", "long size", and the declaration orders
//     size/pos/tag/len and len/size/pos/tag (tag last is 94.7%, tag before len
//     is 78.9% / 207 bytes, it pushes ebx).
//   * worse, do not retry: while (1) 68.5%, do/while(strncmp != 0) 70.6%,
//     the test as a for(;cond;) 89.2% / 218 bytes and the test inverted into an
//     else arm 89.2% / 218 bytes, the strncmp result stored in a local 90.7% /
//     199 bytes, "pos += 8" moved above the latch's two reads 93.4%, all-int
//     locals 93.4%, the update moved after the seek 42.8%.
//
// Progress this pass: the whole disagreement shrank from 9 differing
// instructions to 2, all in the preheader, once the locals became
// `unsigned int` and were declared in the order char tag[4] / size / pos / len
// (any order with pos before len works). That one type/order change is what
// un-folds the size update and turns the seek argument back into the
// original's destructive `mov ecx,[len] / add ecx,edi / push ecx`; the
// previous file's `int` locals produced `lea edx,[edi+ecx]`.
//
// What still differs (both in the preheader, everything else byte exact,
// including the frame, the two callee saves, the whole loop body, the
// top-tested exit and both epilogues):
//   1. The size-update temp register. Original:
//        mov edx,[esp+8] / push 0xc / add edx,8 / push esi / mov [esp+0x10],edx
//      ours is node for node the same but with edi as the temp. edi is the
//      register pos occupies once `mov edi,0x14` runs, so this is one
//      allocator decision for one short lived temp. Swept free with
//      check.py --sym: all 24 declaration orders, `+= 8`, `size + 8`,
//      `8 + size`, a cast, a named temp `t = size + 8; size = t;`, `long`
//      size/len, `pos` assigned at the top of the function, before the
//      update, between the two reads and after both, `for (pos = 0x14;;)`
//      and a declaration initialiser. Every spelling is either the untouched
//      edi temp (94.7%) or the folded `add dword ptr [size],8` (92.0% to
//      93.3%); none gives edx. Assigning pos before the update does push the
//      temp out of edi, but only as far as ecx, not edx. Also flat at 94.7%
//      with the same edi temp: a tag pointer local `char* pt = tag;` used for
//      every tag access, `unsigned int* psz = &size; *psz = *psz + 8;`,
//      `register unsigned int size;`, and defining 0x4d07f0 above this
//      function in the same file. A single struct local for size/tag/len is
//      much worse (68.4%, 207 bytes, it pushes ebx and moves the frame).
//   2. `mov edi,0x14` sits one slot early: ours emits it before the last
//      push of the first strncmp, the original after that push. This is the
//      same scheduler tie the neighbour 0x4d0720 records for its target load.
// The two are one allocator/scheduler state, not two fixes, exactly as the
// notes on 0x4d07f0 (which has the identical edx/edi diff) conclude.
// The 0..400 unused `extern int` declaration sweep is flat at 94.7%, and
// headers.py finds no set that matches, so this is not compiler state either.
// SEVENTH PASS (deepseek-v4.1-flash, free check.py --sym scoring only; the
// baseline was one real run). Still 94.7%, 206 bytes, the same two preheader
// diffs. New negatives, all scored free with check.py --sym:
//   * the update spelling is irrelevant while pos sits between the two reads:
//     "size += 8", "size = size + 8", "size += 4 + 4", "size += (unsigned
//     char)8", "size += 0x10 - 8", "size += 8u", a named temp
//     ({ unsigned int t = size; t += 8; size = t; }), "size -= -8",
//     "size += 1; size += 7", "size = 8 + size", "size += 2; size += 6",
//     "size = (size + 8) & 0xffffffff", "size += 8 | 0" and
//     "size += sizeof(unsigned int) * 2" are all byte identical at 94.7%,
//     for unsigned int, int, long and unsigned long size alike.
//   * moving pos after both reads (the original's slot) fixes the
//     "mov edi,0x14" position but folds the update to "add dword ptr
//     [size],8" (200 bytes, 93.3%); every update spelling above still folds
//     there. So the fold tracks pos's position in the block, not the
//     statement, and the two diffs are one state.
//   * a struct holding size+tag reproduces the frame offsets and, with pos
//     between the reads, still gives the edi temp; with pos after, it folds.
//   * adding a live "guard" read across the len read moves file out of esi
//     and pushes ebx, so it is worse. Swapping the two callee declarations,
//     "long" on the seek, "void" on the read, a manual strncmp prototype,
//     unused locals, <windows.h> or <stdio.h>/<stdlib.h>, and 4u for the
//     count are all byte identical at 94.7%.
// Conclusion: pos=0x14 between the reads leaves edi free for the size temp
// (edx in the original); defining pos after both reads reserves edi (why the
// original has edx) but then the same block folds size's update. No source
// spelling was found that separates the two, so this is the same backend
// allocator plus scheduler state the earlier passes identified.
//
// EIGHTH PASS (deepseek-v4.1-flash, free check.py --sym and direct /Fa
// listings; no real run beyond the baseline). Code unchanged at 94.7%, the
// same two preheader diffs. Purpose was to attack the single allocator state
// directly. All new negatives:
//   * confirmed with /Fa listings that the temp follows pos's register: with
//     pos not yet live the temp coalesces onto edi (pos's eventual register);
//     hoisting pos=0x14 above the update moves the temp to ecx, and no
//     spelling reached edx. A second live value was needed to occupy ecx and
//     none existed: `char* pt = tag` or `char* pt` fed to the tag read and the
//     strncmp, `unsigned int* psz = &size` used for the read and the update,
//     and a struct/union holding size+tag all either coalesced away (address
//     values rematerialise) or changed the frame, never reaching edx.
//   * struct locals `struct H { unsigned int size; char tag[4]; }` (size
//     first) are byte identical to the plain locals, temp still edi, and with
//     pos after the reads still fold. `char tag[4]` first is 86.8%.
//     A union of size and char[4] is 31.6%.
//   * defining pos at the top and deriving the seek offsets from it
//     (push pos-0x10 == 4, push pos-8 == 0xc) folds to the same immediates but
//     drops to 92.0%, so the original's seeks are plain literals and pos is not
//     live in the preheader.
//   * bridging the update and the seek with a read+mask local (brief item 6,
//     e.g. `{ unsigned int d = size & 0xff; }`) is eliminated by the front end
//     and leaves the fold untouched; an unmasked read local, `size = size`, and
//     a named temp are the same 93.3%. A real `if (size == 0)` is 73.2%.
//   * the update spelling sweep was repeated at --sym for `+=`, `= size + 8`,
//     `+= 4 + 4`, `+= 2 * 4`, `+= sizeof(int) * 2`, `= 8 + size`, `-= -8`,
//     `+= 1 << 3`, `+= 8u`, a split `+= 4; += 4;`, a named temp and a cast,
//     each with pos both between and after the reads: every one is 94.7 with
//     the register form (temp edi) when pos is between, and 93.3 with the fold
//     when pos is after. Nothing separates the register choice from the
//     schedule, which remains the blocker.
//
// NINTH PASS (deepseek-v4.1-flash, free check.py --sym scoring and direct /Fa
// listings only; the baseline was one real run). Code unchanged at 94.7%, the
// same two preheader diffs. This pass found the actual mechanism of the fold
// and confirmed it is not reachable from source:
//   * the +8 update compiles to the original's register form exactly when some
//     value is live across the loop. Add a real second parameter used in the
//     strncmp (this is what 0x4d0720 has) and the fold disappears, but the temp
//     then goes to edi, not edx. An unused second parameter, an extra local, an
//     extra read and a global read used in the loop all change nothing.
//   * with pos assigned at the TOP (before the seek(0xc)) or just before the
//     update, the register form returns and pos's edi is reserved, but the temp
//     is then ecx and `mov edi,0x14` is emitted at the top, not at the end. So
//     MSVC's free-register order here is edi, then ecx, then edx: the original's
//     edx needs edi AND ecx both reserved at the update, and pos alone only
//     takes edi.
//   * a full position sweep of `pos = 0x14;` over all six preheader statement
//     boundaries, singly and in all pairs and triples, has only two states:
//     pos on or before the size update (94.7, temp edi or ecx, copy at the top)
//     or pos after it (93.3, fold). No position gives the copy at the end and a
//     register update at once.
//   * wrappers that add a use without adding bytes (brief item 2, guide's
//     0x4bcb50 trick) do not flip it: `__inline` Seek/Read helpers around the
//     calls, `ReadRef(file, size)` by reference, and per-call wrapping of each
//     of the three reads are all byte identical at 94.7%.
//   * pointer and reference spellings of the update (`(&size)[0] += 8`,
//     `*(&size) = *(&size) + 8`, `unsigned int* p = &size; *p += 8;`,
//     `unsigned int& r = size; r += 8;`, `size[0]` on an `unsigned int size[1]`,
//     `(size & 0xffffffff) + 8`, a comma sequence) all fold in the pos-after
//     shape and leave the edi temp in the pos-between shape.
//   * headers.py again: no header set matches; the closest is 94.7%.
// The conclusion of the eight passes stands: the residual is where the backend
// materialises the two preheader defs inside one basic block, and it is the same
// allocator plus scheduler tie the neighbour 0x4d0720 records.
// TENTH PASS (space-bunny-free: one real check.py run for the baseline, then
// free check.py --sym scoring, ~0.3 s a variant, of 40+ more variants and
// direct /Fa listings). Code unchanged, still 94.7%, 206 bytes, the same two
// preheader diffs. The point of this pass was to characterise the FOLD, which
// every earlier pass only described, and the characterisation is sharp:
//   * The size update stays `mov r,[size] / add r,8 / mov [size],r` (the
//     original's shape) if and only if the `pos = 0x14` store is NOT the last
//     statement of the preheader, i.e. if something follows it. With
//     pos between the two reads, or hoisted above the update, the update keeps
//     the register form; with pos after both reads (the original's slot, and
//     the only position that puts `mov edi,0x14` where the original has it,
//     because then the preheader's statement list is identical in shape to
//     the latch's and the scheduler places the copy in the latch's slot
//     instead of ours) the update always folds to `add dword ptr [size],8`,
//     200 bytes, 93.3%. Confirmed with /Fa: the folded form is attributed to
//     the update's own source line, so it is decided at the graph level, not
//     by a scheduler peephole.
//   * New negatives, all free, all with pos after both reads (the folding
//     position): the update through a local pointer, `unsigned int* psize =
//     &size; *psize = *psize + 8;`, `*psize += 8;` and `psize[0] = psize[0]
//     + 8;` all fold. So does keeping the value in a `char hdr[4]` array and
//     punning it, `*(unsigned int *)hdr = *(unsigned int *)hdr + 8;` with
//     `pos >= *(unsigned int *)hdr` in the loop, and the same through a
//     `unsigned int *ph = (unsigned int *)hdr;` local. `int size` instead of
//     `unsigned int size` folds. So does a `goto` back to a top tested label
//     (93.3%, the same diff) and `while (strncmp(...) != 0) { ... }` with one
//     `return len;` at the end (89.0%). Swapping the two reads and putting pos
//     after both (90.7%, 200 bytes) folds too, and a do/while(strncmp != 0) is
//     77.3% / 236 bytes. All 24 declaration orders swept again for both pos
//     positions: `after` is 93.3% wherever pos precedes len in the declaration
//     list and 80.0% wherever len comes first (it pushes ebx), `between` is
//     94.7% in the same 12 and 78.9% in the other 12. Nothing separates the
//     register form from the fold.
//   * The temp register still only ever reaches edi (pos not live at the
//     update) or ecx (pos hoisted so edi is taken); nothing reached edx, and
//     per the allocator's preference order that needs edi AND ecx occupied at
//     the update, which needs a second live value there, and every candidate
//     (a tag pointer local, a size pointer local, a guard read) either
//     rematerialises, changes the frame or pushes ebx.
// Bottom line for the next pass: the preheader's statement order is forced by
// the original (setup, then the two reads, then the pos copy, then the test),
// and in that order MSVC 5 folds the size update, while every order that keeps
// the register form misplaces the pos copy. That is the single blocker, and it
// is a graph level decision, not register allocation.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

// FUNCTION: 0x4d0910
int __stdcall FUN_004d0910(void* file)
{
    char tag[4];
    unsigned int size;
    unsigned int pos;
    unsigned int len;

    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &size, 4);
    size += 8;
    FUN_004bb710(file, 0xc);
    FUN_004bb7c0(file, tag, 4);
    pos = 0x14;
    FUN_004bb7c0(file, &len, 4);
    for (;;) {
        if (strncmp(tag, "data", 4) == 0)
            return len;
        FUN_004bb710(file, pos + len);
        pos += len;
        if (pos >= size)
            return 0;
        FUN_004bb7c0(file, tag, 4);
        FUN_004bb7c0(file, &len, 4);
        pos += 8;
    }
}
