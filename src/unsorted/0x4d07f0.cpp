// Decompiled by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 88.7% (ours 275 bytes, original 275, code size exact, 1 real
// check.py run this pass plus the baseline). The whole
// RIFF/WAVE chunk walk, the rotated loop, the len/0 exit phi, the 0x10 byte
// fmt read and the three output stores are all right. Two differences are
// left, both block scheduling inside a single basic block rather than type or
// argument error.
//
// NEW EVIDENCE (this pass, all free with check.py --sym). "pos = 0x14" belongs
// BETWEEN the two reads that end the loop preheader, not after them. That one
// move takes the function from 85.4% / 273 bytes to 88.7% / 275 bytes and it
// does two things at once:
//   * it stops the local value numbering pass from folding "total + 8" into
//     the in place add dword ptr [total],8, so the load, the add and the
//     store survive as three instructions, in exactly the original's shape
//     and in exactly the original's position relative to the two argument
//     pushes of the seek that follows. This is the same instruction shape the
//     sibling 0x4d0720 gets, and no spelling of the statement itself ever
//     reached the register state that produces it. Earlier passes chased the
//     statement; the lever is the statement AFTER it.
//   * it moves "mov edi, 0x14" up into the early block, which is where the
//     original's block scheduler keeps the copy, even though the original's
//     copy lands back down beside the strncmp.
// The loop body and latch were already exact, so the copy placement is a
// preheader scheduling question, not a loop shape question, and the preheader
// is not rotated here (unlike 0x4d0720, which needs the other way round).
//
// What is left, both in the same early block:
// 1. The "total + 8" temp register. Original:
//      mov edx,[esp+8] / push 0xc / add edx,8 / push esi / mov [esp+0x10],edx
//    Ours, node for node the same sequence, but:
//      mov edi,[esp+8] / push 0xc / add edi,8 / push esi / mov [esp+0x10],edi
//    Every other register in the function matches, so this is one register
//    allocator decision for one temp. The temp is a named local's read
//    modify write, so it is a graph node the allocator sees; the position of
//    pos's initialisation, which is the only live range that can overlap it,
//    is the obvious lever and has been swept (before the seek, after the seek,
//    between the two reads, after the second read, in the declaration).
// 2. "mov edi, 0x14" one slot too early. The original has it after the last
//    push of the strncmp call, we emit it just before that push.
// 3. Epilogue. The original pops edi after loading the three fmt values and
//    before the first store, and pops esi after the last store, so its arg
//    displacements are 8 higher than ours. Ours hoists both pops to the top
//    of the tail block and shares one teardown with the return 0 block.
//    Reading the three fmt fields into named locals first (sr, bps, ch) does
//    not change it: the rematerialised loads and the pop placement come out
//    the same. Both diffs 1 and 2 and 3 are one instruction list, so per the
//    guide's "look for one shared cause" rule the whole early block plus the
//    epilogue is probably a single allocator state that a stronger model can
//    find, not three separate fixes.
//
// REJECTED, all scored free with check.py --sym on scratch copies:
//   * "pos = 0x14" before the seek that follows the "total + 8" update: 88.7%
//     but the temp becomes ecx and the update moves after both pushes. The
//     one that gave ecx.
//   * "pos = 0x14" in the declaration: 84.4% / 269 bytes, update folded.
//   * "pos = 0x14" after both reads (the previous file's placement): 85.4% /
//     273 bytes, update folded to the in place add.
//   * no separate "n" for the exit phi, the len local doubled as the phi
//     (len = 0; break; ... if (len < 0x10) return 0;): 82.7% / 273 bytes.
//     Same code, but the exit block lands in the other order, so the two
//     "je"/"jae" targets swap and the jmp over "xor eax,eax" is 4 bytes
//     further from the test.
//   * "total += 8" and "total = 8 + total": byte identical, see the sweep.
//   * reading fmt + 4, fmt + 14 and fmt + 2 into three locals before the
//     three stores: no change to the tail.
//   * headers.py: <string.h> is already the best set.
//
// The signature is confirmed by the mangled name
// ?FUN_004d07f0@@YGHPAXPAH11@Z and the frame: the len local lives in the dead
// file argument slot at [esp+0x28], total at [esp+0x08], the tag at
// [esp+0x0c] and the 0x10 byte fmt buffer at [esp+0x10].
//
// SECOND PASS (deepseek-v4.1-flash), all free with check.py --sym. The 88.7%
// version above is confirmed the best of roughly 200 further spellings. What
// was swept and made no difference at all (all byte identical, 275 bytes):
//   * ~60 permutations of the five scalar declaration orders, five loop
//     shapes (for(;;), do/while, while, bottom test, goto), four spellings of
//     the "total + 8" update, and both placements of pos: the only states
//     reachable are 88.7%, 85.4%, 69.8% and 69.1%, and 88.7% is always this
//     exact preheader (mov edi,[total] / push 0xc / add edi,8 / push esi /
//     mov [total],edi) with the two diffs already recorded.
//   * headers.py: 128 sets, 88.7% flat (the 16 that compile).
//   * every alternative exit-phi shape: no separate n, n pre-set to 0, an ok
//     flag combined with `t = ok ? len : 0` (MSVC if-converts it to
//     neg/sbb/and and drops the jmp), the exit test on the memory field
//     (cmp dword ptr [esp+0x28],0x10), reads through named locals, reads
//     through an int* cast, the fmt buffer modelled as a struct, tag modelled
//     as an int or as a struct member: all either 88.7% flat or worse.
//   * the tail wrapped in do/while(0), a nested block, if(1), or split across
//     two returns: no change, so the epilogue diff follows the same allocator
//     state and is not a separate block-shape question.
//   * casts on the call arguments, int/unsigned variants of every local,
//     macro/enum/sizeof spellings of 8 and 0x14, dead inline helpers and dead
//     globals before the function, extern globals for the locals: no change.
// What was newly learned about the residual:
//   * The 88.7% state is robust to the entire legal source space; the two
//     remaining diffs only appear as a package. Free probes in
//     build/scratch/0x4d07f0/p4..p55 show that a read-modify-write of an
//     address-taken local is emitted by MSVC 5 as `add dword ptr [local],8`
//     (never the register form) unless the updated value stays live across
//     an intervening call, in which case the register form appears but the
//     store lands at the POSITIVE argument displacement, which is what the
//     original has here. In this function the updated total is provably dead
//     after the store, so the original's compiler state is the only thing
//     that kept the register copy. This is the same tie the sibling 0x4d0910
//     shows (94.7%, same two-instruction diff with a different local set) and
//     that docs/agent-guide.md records at 0x4b6570, 0x426200 and 0x4a35a0.
//   * It is NOT a missing source construct: the original's compute `0x4d02a0`
//     (the AI file reader) carries the identical 275 byte chunk walk and it
//     too emits `mov edi,[esp+0x14] / push 0xc / add edi,8 / push esi /
//     mov [esp+0x1c],edi`, i.e. an argument displacement, for exactly the
//     same statement, so both the register copy and the positive displacement
//     are this compiler state, not a property of the source.
// The signature is confirmed by the mangled name
//
// FOURTH PASS (space-bunny-free): the body is unchanged because nothing beat
// it. Baseline re-confirmed with a real run: 88.7%, 275 of 275 bytes, the same
// three hunks (temp register edx versus edi, "mov edi, 0x14" one slot early,
// epilogue pops hoisted). Three new probes, all scored free with --sym, all
// 88.7% flat and byte identical to the third pass's output:
//   * "t = total; total = t + 8;" with a new live scratch local: MSVC 5's front
//     end COALESCES the copy away, so the extra graph node the third pass says
//     is needed to reach edx is never born. This is brief item 3 in reverse:
//     a local fed from memory and dead afterwards is removed before the
//     allocator runs, so it cannot demote anything.
//   * the same scratch with "pos = 0x14" hoisted above the update as well, so
//     two values are live at the temp point: still 88.7%, temp still edi.
//   * "t = total + 8; total = t;": still 88.7%.
// So the second live scratch value the third pass needed cannot be introduced
// by any local assignment here. It would take a value that survives a call or
// a real branch, and no such value exists in this preheader: the only calls
// are the two seeks and the three reads, and the "total + 8" result is
// provably dead after its store. The register choice and the store schedule
// stay mutually exclusive.
//
// THIRD PASS (deepseek-v4.1-flash): the body is unchanged because nothing beat
// it. This pass used the compiler directly (tools/wcl, /Fa listing) so it
// could score thousands of variants without spending real check.py runs, and
// it strengthens the conclusion that the residual is one unreachable
// allocator/scheduler state:
//   * True edit distance over the instruction lists (not difflib, per the
//     brief's item 17): this body is 11 instructions from the original out of
//     97. Every other shape tried is 12 or worse.
//   * All 2520 orderings of the six preheader statements, with `pos = 0x14`
//     inserted at any of the seven slots, were compiled and the listing read:
//     2004 keep the temp in edi, and all 516 that use edx put the update one
//     call late (scheduled into the tag read's pushes). No ordering gives edx
//     with the original's early schedule (mov edx,[total] / push 0xc / add
//     edx,8 / push esi / mov [total],edx / call skip). The register choice and
//     the schedule are mutually exclusive from this source.
//   * Making edi live at the temp point (any early `pos = 0x14`, in the
//     declaration or before the read or before the update) forces the temp out
//     of edi but only as far as ecx, and always moves the update after both
//     pushes. Reaching edx needs a second live scratch value there; no
//     spelling produced one.
//   * The epilogue pop placement follows the same state. Adding an unrelated
//     `test eax,eax; jge` on the fmt read (which the original lacks) makes the
//     pop edi / pop esi land in the original's slots, so the epilogue diff is
//     a scheduling consequence of the preheader block, not a separate source
//     problem.
//   * Defining the real preceding function 0x4d0720 above this one in the same
//     file changed nothing: still 88.7%, same three hunks.
//   * 336 combinations of preheader order x loop form (for/while/do) x tail
//     store order and if/else shape: none MATCH and none below edit distance
//     11. The tail's three assignments are settled.
//   * Headers by hand (<windows.h>, <string>, <iostream>, <vector>+<map>,
//     <stdio.h>, <stdlib.h>, <math.h>, <mmsystem.h> and combinations) and
//     0..400 plus 500..5000 unused `extern int` declarations: flat at 88.7%.
//
// FIFTH PASS (deepseek-v4.1-flash, 900s micro-run; one real check.py run for
// the baseline, the rest compiler-direct with /Fa listings so no check budget
// was spent). The body is unchanged: nothing reached the original's temp
// register. The residual is still the one decision the fourth pass named, the
// `total + 8` temp in edx versus edi, with the `mov edi, 0x14` slot and the
// three tail pops following it. New negatives, all compiler-direct:
//   * total as a struct field of an address-taken local, and as a wrapper
//     struct with `__inline get()/set()` accessors: still edi (the accessor
//     form moved "mov edi, 0x14" six instructions later but kept edi).
//   * `unsigned int* pt = &total; *pt = *pt + 8;` and `void* f = file;`
//     (every call through the copy): still edi. `register`, `int` locals and
//     pos-declared-first: still edi.
//   * seven update expression trees besides `total = total + 8` (byte/short
//     casts, `8 + total`, `+=`, `(total+4)+4`, `(total^0)+8`, `& 0xffffffff`):
//     the plain tree is edi; only the two cast trees move it, and only to ecx.
//     Never edx. Adding one or two named locals fed from memory and used after
//     a call did not move it at all (the front end coalesces them, as the
//     fourth pass already found).
// So edx is reachable neither as the next free scratch after edi nor after
// ecx, which is the same wall 0x4d0910 and 0x4d0720 report. Leaving the file
// at 88.7%.
// SIXTH PASS (space-bunny-free, 900s micro-run): the body is unchanged, still
// 88.7% / 275 of 275 bytes, confirmed by a real run at the start of the pass.
// Two new negatives, both scored free with check.py --sym:
//   * the 0x4d02a0 shape (off = 0x14 AFTER both reads, the loop condition at
//     the top of a `for (; strncmp(...) != 0; )` and `r = len;` after the loop
//     instead of an n = len / n = 0 exit phi): 67.7% / 263 bytes. The RMW folds
//     to `add dword ptr [total], 8` again, and `seek(pos + len)` becomes
//     `lea edx,[edi+ecx]`. So the 0x4d02a0 reconstruction of this walk is NOT
//     the shape of 0x4d07f0, whatever its own score.
//   * that same loop shape with `pos = 0x14` moved back between the tag read
//     and the len read: 83.8% / 269 bytes, so the exit phi is worth ~5 points
//     on its own and is not what blocks the temp register.
//   * `unsigned int pos;` declared AFTER `char fmt[0x10]` (which moves pos's
//     frame slot to the top and shifts fmt by 4 bytes, so the fmt loads use
//     different esp displacements): 69.1% / 275 bytes. The frame layout IS
//     load bearing, and the current declaration order is the one that keeps
//     fmt at [esp+0x10] with the tail loads at +0xc/+0x16/+0xa.
// SEVENTH PASS (deepseek-v4.1-flash, 900s micro-run; one real check.py run for
// the baseline, then compiler-direct and free --sym scoring). Body unchanged,
// still 88.7% / 275 of 275 bytes, the same three hunks. New negatives:
//   * compiler state via unused FUNCTION PROTOTYPES (not just `extern int`):
//     0..2000 in steps of 25, both `extern int dummyN(int);` and
//     `int dummyN(int);`, every N byte identical at 88.7%. So the prototype
//     kind of a declaration is not the missing state lever either.
//   * BT_TOOLCHAIN=msvc5-rtm (the unpatched compiler): 88.7% / 275, same bytes.
//   * address/reference spellings that might reserve a scratch register at the
//     update (`unsigned int* p = &total; *p = *p + 8;`, `unsigned int& r =
//     total; r = r + 8;`), buffer-address locals (`char* tp = tag;`,
//     `unsigned int* lp = &len;`, `char* fp = fmt;`, and both at once), a live
//     `int flag`, a local `const char* needle = "fmt ";` and a local length
//     `int nl = 4;`, and an extra assigned-but-unused `unsigned int q;`: all
//     byte identical at 88.7%. Nothing reserves ecx at the temp point, so the
//     original's edx stays out of reach, as the third pass concluded.
// The residual is the backend allocator/scheduler tie the sibling 0x4d0910 and
// the guide (0x4b6570, 0x426200) record: the `total + 8` temp is edx in the
// original and edi here, and the `mov edi, 0x14` slot plus the tail pops follow
// from the same state.
// EIGHTH PASS (deepseek-v4.1-flash): body unchanged, still 88.7% / 275 of 275
// bytes, the same three hunks. One new negative, scored free with --sym:
//   * a distinct `lim = total + 8` local (total provably dead after, the loop
//     compares against lim instead of total): 40.2% / 267 bytes. The extra
//     local makes the allocator reserve a callee-saved register for the frame
//     (push ebx appears), moves file to edi and pushes &total to a higher
//     slot. So the RMW has to stay on the same local; the original's edx is
//     still the allocator tie the three hunks above describe, not a missing
//     second variable.
// NINTH PASS (deepseek-v4.1-flash, one real check.py run for the baseline plus
// free scoring; body unchanged, still 88.7% / 275 of 275 bytes, the same three
// hunks). Genuinely new evidence:
//   * the sibling ORIGINAL 0x4d02a0 (the AI file reader) contains the identical
//     chunk walk at 0x4d030f and emits `mov edi,[esp+0x14] / push 0xc /
//     add edi,8 / push esi / mov [esp+0x1c],edi`, i.e. edi again, for the exact
//     same statement and the same 275 byte walk. So the default register for
//     `total + 8` is edi, and 0x4d07f0's original edx is the anomaly, fixed by
//     that one function's allocator state. That is as strong a proof as this
//     kind of tie gets that the register is not a property of the source.
//   * a persistent `unsigned int* ptotal` local used for the read call and for
//     the update was scored free (build/scratch/0x4d07f0/vA.cpp): byte
//     identical, 88.7%. The address rematerialises, so no register is held.
//     The temp can only leave edi for edx if edi AND ecx are both reserved at
//     the update, and no reachable source value occupies both; this matches the
//     allocator order (edi, then ecx, then edx) the sibling 0x4d0910 derived.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

// FUNCTION: 0x4d07f0
int __stdcall FUN_004d07f0(void* file, int* sampleRate, int* bitsPerSample, int* channels)
{
    unsigned int total;
    char tag[4];
    unsigned int pos;
    unsigned int len;
    unsigned int n;
    char fmt[0x10];

    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &total, 4);
    total = total + 8;
    FUN_004bb710(file, 0xc);
    FUN_004bb7c0(file, tag, 4);
    pos = 0x14;
    FUN_004bb7c0(file, &len, 4);
    for (;;) {
        if (strncmp(tag, "fmt ", 4) == 0) {
            n = len;
            break;
        }
        FUN_004bb710(file, pos + len);
        pos += len;
        if (pos >= total) {
            n = 0;
            break;
        }
        FUN_004bb7c0(file, tag, 4);
        FUN_004bb7c0(file, &len, 4);
        pos += 8;
    }
    if (n < 0x10)
        return 0;
    FUN_004bb7c0(file, fmt, 0x10);
    *sampleRate = *(int*)(fmt + 4);
    *bitsPerSample = *(unsigned short*)(fmt + 14);
    *channels = *(unsigned short*)(fmt + 2);
    return 1;
}
