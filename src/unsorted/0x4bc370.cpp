// Decompiled by GPT-5.6-Terra, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Case-insensitive wildcard match of a string against a pattern that may
// contain '?' (any one character) and '*' (zero or more characters). The
// pattern positions still alive after each input character are kept in a
// stack of at most 100 entries, so a '*' backtracks in linear time.
//
// Best result: 86.4%, and the ENTIRE remainder is one instruction of 5 bytes
// against 4. Everything else in the 314 bytes is byte exact, including the
// register roles, so this is a pure SIB operand-order problem, not a
// register-allocation one:
//   original 0x4bc3cb: 0f be 0c 28            movsx ecx,BYTE PTR [eax+ebp*1]
//                        modrm 0x0c = SIB, no disp; SIB 0x28 = scale 1, index
//                        ebp (the pattern index), base eax (pat)
//   ours               0f be 4c 05 00         movsx ecx,BYTE PTR [ebp+eax*1+0x0]
//                        modrm 0x4c = SIB + disp8; SIB 0x05 = scale 1, index
//                        eax (pat), base ebp (the index)
// The reversed pair costs the disp8 byte, so ours is 315 bytes and every
// following branch target is one byte out. Nothing else differs.
//
// Re-attempt (deepseek-v4.1-flash, issue 1228): re-verified lever 1 (the
// callers' pushes and each callee's ret N) and the SIB base/index slot order
// against this census. No new lever found, so the body is unchanged. This is a
// front-end SIB base/index wall, not a source difference.
//
// A FREE WAY TO SEARCH THIS (no check.py runs, ~1.5 s per variant): put N
// copies of this whole function in one file under different names, vary only
// the construct, compile once with tools/wcl /c /O2 /Ob2 /MT, and read the
// SIB byte with
//   objdump -d -M intel build/scratch/0x4bc370/q*.obj | grep movsx
// The /Fa listing prints the same operands as objdump here, but per the guide
// the listing is not always faithful to the SIB byte, so trust objdump.
// Working scripts: build/scratch/0x4bc370/sib.py (header sets), q.py and y.py
// (expression and parameter-type variants), findsib.py (binary census).
//
// CENSUS: this scale-1, two-register SIB load occurs only EIGHT times in the
// whole executable. Decoded properly (SIB = scale in bits 7-6, INDEX in bits
// 5-3, BASE in bits 2-0), SEVEN of the eight put the INTEGER in the base slot
// and the POINTER in the index slot, which is what this file compiles to:
//   0x4bc3cb  0f be 0c 28   mod=00 SIB 0x28 -> base eax(pat) index ebp(idx)
//   0x4a0712  0f be 54 0d 00  mod=01 SIB 0x0d -> base ebp(j) index ecx(ptr)
//   0x4af21d  8b 7c 15 00     mod=01 SIB 0x15 -> base ebp   index eax
//   0x4b0f00 0x4b0fa0 0x4b11fa, 0x4c78fd: the same, base=ebp
// So 0x4bc3cb is the ONLY site in the exe with the pointer in the base slot.
// NOTE the earlier note in this file pointed at 0x4a05e0 as the neighbour that
// reaches the base=pointer form. It does not: at 0x4a06d0 ebp is zeroed as the
// loop index j and edx holds the string, so 0x4a0712 is base = the integer j and
// index = the pointer, i.e. our form. Both "matched" neighbours (0x4a05e0 at
// 53.8%, 0x4aefa0 at 72.2% in their own headers) are partial anyway, so no file
// in this tree currently reproduces the base=pointer shape. Do not spend runs
// looking for a construct in a neighbour that reaches it: none does.
//
// Tried without effect (all stay at 86.4%):
//  * every spelling of the address: pat[idx], idx[pat], *(pat+idx),
//    *(idx+pat), (char*)pat+idx, integer-cast forms, a `const char*` local
//    copy, an `int& idx` reference, and an inlined `GetPat(pat, idx)` helper.
//  * declaring idx at function scope (before n, before i, after c), and
//    idx typed unsigned/unsigned short/unsigned char/char/short.
//  * tools/headers.py and headers.py --cpp: no header set makes it match.
//  * the N-declarations sweep, N = 0..400 step 16: flat at 86.4%, so the
//    remaining difference is not compiler state reachable that way.
//  * defining the real neighbour FUN_004bc360 above it, and including
//    <direct.h>/<io.h>/<sys/stat.h>/<stdio.h>/<stdlib.h>.
//  * a 32-spelling sweep scored on the SIB byte directly (free, see above):
//    pat[idx], *(pat+idx), *(idx+pat), pat[idx*1], *(pat+(idx*1)), *(&pat[idx]),
//    *(&pat[0]+idx), pat[(unsigned)idx], a struct member array
//    (((Pat*)pat)->c[idx]), a separate statement into a char then to toupper,
//    a signed char temp, a char* / const char* local copy of pat bound inside
//    the loop, a pre-added `const char* q = pat + idx` (that one gives a
//    scalar add, as the second loop of this function also does), a redundant
//    self-correction after the load, an int temp, a long temp for the index,
//    and the integer-cast forms *(char*)((long)pat + idx),
//    *(char*)((unsigned)pat + idx), *(char*)((int)pat + idx),
//    *(char*)((long)pat + (long)idx) with and without a named long. MSVC
//    folds every cast back into a segoffset, so all give the same node.
//  * parameter constness and type: const char*/char*/signed char* for pat and
//    for str, in all four combinations. No effect.
//  * 26 header sets, from <windows.h>, <math.h>, <stdio.h>+<stdlib.h> and
//    <windows.h>+<math.h> through <vector>, <iostream>, <string>, <map> and
//    <list>, each also with a hand-written `extern "C" int toupper(int);`.
//    Every one gives SIB 0x05.
// Micro-tests show MSVC 5 canonicalises every char* + int form to base = the
// integer index and index = the pointer, for scale 1; the original's base =
// pointer order could not be reproduced from any expression shape tried.
//
// Additional negative results (deepseek-v4.1-flash, still 86.4%):
//  * The SIB order is not a spelling of the address. Twenty further forms
//    all give [ebp+eax*1+0x0]: array of 1-byte structs, pointer to a
//    char[1] array, reference binds, an explicit int temp, inline
//    GetPat/AddP helpers in both argument orders, an inline helper that
//    returns the index, indexing pat[stack[i]] directly, int/long/
//    unsigned/short/char idx, and all four constness combinations of
//    str/pat.
//  * A fine N-declarations sweep (N = 0..1998, step 1 then 4) stays at
//    SIB 0x05, so this is not reachable with `extern int` declarations,
//    and it is not the period-525 window the guide describes.
//  * Isolated reproductions: a leaf `p[i]` gives base = pointer, but every
//    form whose index is loaded from memory (the shape here) gives
//    base = index, index = pointer. A sign- or zero-extended short/char
//    index flips it to base = pointer, but that adds an extension the
//    original does not have. So with a full-dword-loaded index the
//    pointer-in-base form is not reachable from the address expression.
//
// New negative results (space-bunny-free, still 86.4%, ~30 more free compiles):
//  * The emitter's slot choice is NOT a register-pressure effect. Bisecting
//    this function (build/scratch/0x4bc370/v2.py, v3.py, all scored on the
//    SIB byte without check.py) moves the index out of ebp into edi or esi by
//    deleting the trailing pattern loop or parts of the if/else chain, and the
//    choice does not change: base is always the integer, index always the
//    pointer. The only thing register pressure changes is the extra disp8
//    (`0f be 0c 07` instead of `0f be 4c 05 00`), so the disp8 is a
//    consequence of ebp being the base, not a second problem to fix.
//  * A standalone reproduction of the shape (build/scratch/0x4bc370/sib2.cpp,
//    16 variants: global-array index, local-array index, register index,
//    unsigned index, struct member at 0, reference, int/long casts, signed
//    char pointer, no call) gives the same wrong assignment in every case,
//    so it is a fixed property of the X86RM builder for a scale-1
//    `char* + int`, not something this function's context can flip.
//  * Also tried, all giving SIB 0x05: pat[idx + 0], pat[idx - 0],
//    pat[idx + (i - i)] (MSVC folds the zero away, so no disp8 survives),
//    *((char*)pat + idx), ((P*)pat)->c[idx] with both a char[1024] and a
//    1-byte struct member, (unsigned)idx, a volatile extra local, a pointer
//    walk `int* sp = stack; idx = *sp;`, and a dead `stack[99] = 0`.
//
// New negative results (deepseek-v4.1-flash, second pass, still 86.4%):
//  * tools/headers.py 0x4bc370: 128 header sets, none flips the SIB.
//  * 138 dead-`__inline`-call variants (one helper, three body sizes, call
//    counts 0..39 plus 50/64/80/100/128/200) to move the /Ob2 budget: flat.
//  * 300 combinations of the index type (int, unsigned, long, unsigned long,
//    short, unsigned short, char, signed char, unsigned char, size_t) times
//    four constness spellings of `pat` times five address spellings
//    (pat[idx], *(pat+idx), *(idx+pat), pat[(int)idx], pat[(unsigned)idx]):
//    all SIB 0x05, instruction count unchanged at 117.
//  * Binding a reference to the pointer (`const char*& pr = pat;` and the
//    const-qualified form, plus `(*(const char**)&pat)[idx]`), which is the
//    lever the guide records for a pointer field re-read each iteration: no
//    effect here.
//  * Prototype-state sweep: 0..6000 `extern int pfN(int,int,int);` in steps
//    of 100, plus 2700..5500 in steps of 50: flat. So this is not the
//    period-525 prototype window either.
//  * Diagnostic for the next attempt, from isolated probes: the pointer-in-base
//    form IS reachable, but only in the degenerate leaf case. With the index
//    loaded from memory into ecx and the pointer in edx, the bare
//    `return p[idx];` emits `mov al,[edx+ecx*1]` (base = pointer). The moment
//    the loaded char is used at all (stored to a local, pushed to a call, or
//    returned through a temporary) MSVC flips to `[ecx+edx*1]` (base = index)
//    with the SAME registers. So the SIB byte is decided by how the load's
//    result is consumed, not by the address expression, and the original's
//    `0f be 0c 28` would be a genuine source-level or compiler-state
//    difference in that consumption. No consumption shape tried in the real
//    function recovered it.
//
// New negative results (space-bunny-free, second pass, still 86.4%, ~25 more
// free compiles, scored on the SIB byte with build/scratch/0x4bc370/z.py and
// y2.py):
//  * Top-level const on the pattern parameter (`char* const pat`,
//    `const char* const pat`), swapping the || order (`pc == c || pc == '?'`,
//    and constant-first comparisons), `int pc` instead of `char pc`, splitting
//    the index load into a declaration and an assignment, `register int idx`,
//    `*(pat + idx)`, `*(idx + pat)`, a long / unsigned / unsigned long index,
//    `(void*)pat + idx`, a short staging local, and a `const char* pt = pat`
//    local: all stay at SIB 0x05 with `4c 05 00`.
//  * "The index has a frame home, so the RM uses it as a base" is NOT the
//    cause. Deleting the idx local altogether, so the index is the CSE temp
//    `stack[i]` at all four use sites, keeps base = index. So does a pointer
//    walk (`int* sp = &stack[i]; int idx = *sp;`) and `(&stack[0])[i]`.
//  * The disp8 is NOT a second problem to fix, it is only the base-is-EBP
//    encoding constraint. `pat[(int)(char)idx]` reaches the pointer-in-base
//    form with no disp8 at all (`0f be 14 01`, base = pointer in ecx, index =
//    the cast temp in eax, result in edx), and so does the original. Only the
//    base = EBP form has to encode the zero disp as `4c 05 00`. Fix the slot
//    order and the size becomes 314 on its own.
//
// Diagnostic for the next attempt: the pointer-in-base form IS reachable in
// this very function, and the only construct found that reaches it puts a
// THIRD live SCE in the expression (the cast result), which shifts all three
// registers down one (pat eax->ecx, index ebp->eax, result ecx->edx). The
// original has pat in eax, idx in ebp, result in ecx and only two SCEs. So
// what is wanted is a construct that makes the RM take the pointer as SCE1
// WITHOUT adding a live value, and no address spelling, index type, parameter
// constness, header or prototype-state lever tried so far does that.
//
// Third pass (space-bunny-free, 86.4% confirmed by one real check.py run, nine
// more free compiles in build/scratch/0x4bc370/w.py scored on the SIB byte).
// Still stuck at 86.4%, and the body below is unchanged from the best above.
//  * An explicit cast chain on the LOADED VALUE rather than on the address,
//    which is a shape not in any earlier list: toupper((char)pat[idx]),
//    toupper((signed char)pat[idx]), toupper((int)(char)pat[idx]),
//    toupper(*(const char*)(pat + (int)idx)), toupper((char)*(pat + idx)),
//    toupper(pat[idx] + 0), toupper(pat[(int)idx]), toupper(pat[idx * 1]),
//    toupper(((const char *)pat)[idx]). All nine emit SIB 0x05 with disp8,
//    byte for byte the wrong form, so the cast node on the value is not the
//    lever either.
//  * Confirms the earlier census reading: with a full-dword index already in a
//    register, MSVC 5's X86RM always fills the base slot with the integer and
//    the index slot with the pointer, and needs the explicit disp8 of zero
//    because the RM is built as [int + ptr + 0]. Reaching the original's
//    [ptr + int] needs that build order to change, which no expression tried
//    in three passes does.
// Semantics confirmed from the disassembly while reading it (no change needed,
// the existing comments were right): arg 1 is the text, arg 2 the pattern.
// The first loop walks *arg1 (spilled to arg1's own stack slot at 0x4bc393)
// and the inner loop indexes arg2 with the pattern positions from the stack,
// and the closing loop at 0x4bc456 tests arg2 for end of string and for a
// trailing '*'. So the function is a case-insensitive DOS-style wildcard match
// of a text against a pattern, which is what the top comment says.
//
// Fourth pass (deepseek-v4.1-flash, 86.4% baseline reproduced with one real
// check.py run; the rest is free SIB-byte inspection with build/scratch/
// 0x4bc370/mine*.py). Nothing below changes the bytes; the body is unchanged.
//  * tools/headers.py 0x4bc370 --cpp (768 header sets, 0 compile failures):
//    "no header set makes it match", all 86.4%. So it is not header state.
//  * The unpatched compiler (BT_TOOLCHAIN=msvc5-rtm) emits the SAME SIB
//    (`4c 05 00`), so the RTM/SP3 build is not the difference either.
//  * Defining the real preceding TU members above it (0x4bc300, 0x4bc320,
//    0x4bc360, from their matched files) leaves the SIB unchanged, so it is
//    not the source file's preceding-function state.
//  * Isolated probes of the addressing rule: for a scale-1 `char* + int` MSVC 5
//    ALWAYS makes the INTEGER the base and the POINTER the index when the load
//    feeds a call (`[idx + pat*1]`), and this holds for every index type (int,
//    unsigned, long, unsigned long, short, unsigned short, char, signed char,
//    unsigned char, size_t), both parameter and memory-loaded indices, pointer
//    and `unsigned char*` pointer types, and 1-byte struct/array element types
//    (`struct { char c; }[]`, `char[1][]`). The only construct that reaches the
//    original's pointer-in-base form is a SIGN- OR ZERO-EXTENDED index temp
//    (`pat[(char)idx]`, `(unsigned char)idx`, `(short)idx`, a `char`/`uchar`
//    staging local, or an `idx & 0xffff` mask), which emits the exact target
//    encoding `0f be ... [pat + temp*1]` but adds the extension instruction and
//    one live SCE, shifting the whole allocation (pat eax->ecx, idx ebp->ebx,
//    result ecx->edx); it can therefore never be byte-identical. Pure masks
//    that keep the width (`idx & 0xffff`) flip the temp back to integer-base.
//  * Consumption probes that keep the value byte-exact (`int ch = pat[idx];
//    char pc = toupper(ch);`, `char pc = pat[idx]; pc = toupper(pc);`) do not
//    flip it. `toupper((unsigned char)pat[idx])` changes the opcode to movzx,
//    so it is a different (and semantically wrong) source, not a lever.
//  * Conclusion for the next attempt: the original's `[pat + idx]` needs the
//    index in the index slot WITHOUT an extension SCE, i.e. the RM must take
//    the pointer as base for a plain scale-1 `char* + int`. No source spelling,
//    element type, index type, header set, declaration order or preceding
//    function found in four passes does that; it looks like a backend choice
//    the SP3 compiler cannot be steered into from this file.
//
// Fifth pass (space-bunny-free, 86.4% confirmed by one real check.py run, then
// ~30 free compiles scored on the SIB byte with build/scratch/0x4bc370/sb2.py,
// probe.cpp, bis2.cpp, bis3.cpp and bis4.cpp). The body below is unchanged.
//
// CORRECTION TO AN EARLIER NOTE IN THIS FILE. The fourth pass's diagnostic says
// "a leaf `p[i]` gives base = pointer, but every form whose index is loaded from
// memory gives base = index", and the second pass says the same. Both are
// WRONG, and the mistake is a misread of which register holds what. Re-decoded
// register by register in build/scratch/0x4bc370/probe.cpp (nine small probes,
// objdump'd with the push-count walked back to identify each argument):
//   prA  for (i=0;i<n;i++){ int j=a[i]; s+=toupper(p[j]); }
//        ->  0f be 0c 28   ebp = p (the POINTER, arg1), eax = a[i] (the
//            integer), so base = eax = the INTEGER. Not the pointer.
//   prB  int j=a[i]; return toupper(p[j]);
//        ->  0f be 0c 02   edx = a[i] (integer) in the base, eax = p in the
//            index. Same wrong order.
//   prC, prF (no call, and result stored to a char local first), prG, prH
//   (pointer copied to a local inside the loop), prI (p[a[i]]): all the same.
// So the pointer-in-base form is NOT reachable even in the degenerate leaf case.
// There is no consumption-shape story at all: the SIB slot order is the same
// wrong way for a value that is returned, stored, pushed to a call, or indexed
// straight out of the array, and it is the same way our own function's own
// SECOND loop reads `pat[stack[i]]`.
//
// THE ACTUAL CAUSE, and it settles the question. MSVC 5's X86RM always puts the
// INTEGER in the base slot of a scale-1 two-register SIB built from
// `char* + int`, and it always fills the index slot with the pointer. That is
// uniform across every probe and every spelling in five passes. The only thing
// that varies is WHICH REGISTER the integer ends up in, and that alone decides
// the instruction length, because base = EBP has to encode the zero disp as
// `4c 05 00` while any other base register takes mod=00 and is 4 bytes.
//   build/scratch/0x4bc370/bis3.cpp, all free compiles of the real function
//   with only the inner loop's else branch changed:
//     w1  else { if(--n==0) return 0; stack[i]=stack[n]; i--; }  (ours)  4c 05 00
//     w2  else { if(--n==0) return 0; }                            0c 07
//     w3  else { if(--n==0) return 0; i--; }                       0c 07
//     w4  else { if(--n==0) return 0; stack[i]=stack[n]; }         0c 07
//     w5  else { if(n==1) return 0; stack[i]=stack[--n]; }         0c 07
//     w6  else { n--; if(n==0) return 0; stack[i]=stack[n]; i--; } 4c 05 00
//     w7  else with int* sp/sn pointer locals                       4c 05 00
//   In w2..w5 the index lands in EDI and &stack[i] moves to EBP, so the SIB is
//   4 bytes with no disp. In w1, w6 and w7 the index stays in EBP and the
//   &stack[i] pointer keeps EDI, so EBP is the base and the zero disp must be
//   spelled out. Verified by disassembly: w4's `mov edi,[ebp]` is the index and
//   `mov eax,[esp+0x1b0]` is the pointer, i.e. `0f be 0c 07` is base = index.
// So the disp8 is NOT an independent second problem to fix, and the 5-byte
// form is forced by the rest of the byte-exact code: the original's EDI holds
// &stack[i] (it does `lea edi,[esp+0x18]`, `mov ebp,[edi]`, `sub edi,4`), which
// is why the index has to be in EBP, which is why the base is EBP, which is
// why the zero disp costs a byte. Fixing the slot order alone would give 314;
// there is no spelling of the else branch that gives 314 AND keeps EDI on
// &stack[i].
//
// New negative results (fifth pass, all still 0f be 4c 05 00):
//  * the array reached through a pointer: `int sb[100]; int* stack = sb;` with
//    every access as `stack[i]`, so the `stack` IR node is a pointer variable
//    rather than a decaying array name.
//  * `const signed char* pat` instead of `const char*` (the extension source
//    changes, the sign-extension instruction does not).
//  * `--i` instead of `i--`, and `stack[i] = ++idx` / `stack[n++] = ++idx`
//    instead of `= idx + 1` (the latter is interesting on its own: it is
//    literally the original's `inc ebp`, so the source probably did say
//    `= ++idx`, but it does not move the SIB).
//  * `int* sp = &stack[i]; int idx = *sp;` for the loop-1 read.
//  * `*(pat + idx)` and the declaration order `i, n` then `c`.
// CONCLUSION: this is a backend slot choice that the SP3 compiler does not
// expose to source, consistent with the census in the header (7 of the 8
// scale-1 two-register SIB loads in the whole executable put the integer in
// the base slot; only 0x4bc3cb does not). Anyone picking this up should not
// spend runs on address spellings, index types, headers or prototype state.
// Sixth pass (space-bunny-free, 86.4% re-confirmed by one real check.py run,
// then ~18 free compiles scored by byte-exact diff). A new free harness is at
// build/scratch/0x4bc370/mine/free.py: it compiles a variant and compares the
// function's .text bytes against the exe's 314 (call displacements masked), so
// it prints the LENGTH, the first differing byte and, with one compile per
// variant, the same verdict check.py gives, at about 12 s a variant with no
// budget spent. build/scratch/0x4bc370/mine/idis.py then aligns the two
// instruction streams (targets blanked) and confirms that of 110 instructions
// EXACTLY ONE differs: the SIB at 0x4bc3cb. The instruction count, the
// register roles, the 0x198 frame, every branch and the whole epilogue agree,
// so there is no second problem hiding behind the byte count.
// New negative results, all still 315 bytes with SIB 0x05 and a disp8:
//  * the index read straight out of the array with no local at all
//    (pat[stack[i]]), through an int* walk (&stack[i])[0], and with the array
//    and the index typed long or unsigned: 315, SIB 0x05.
//  * the pattern pointer bound to a local for the whole loop (pt = pat), which
//    also breaks the `mov eax,[esp+0x1b0]` reload: the SIB does NOT move, so
//    the slot order does not depend on the pointer leaf being a parameter
//    reload rather than a register copy.
//  * a reference bound to the parameter (const char*& pr = pat, then pr[idx]),
//    a `const char pat[]` parameter, *(pat + idx), *(idx + pat),
//    *(char*)((char*)pat + idx), pat[(int)(long)idx] and a cast pointer
//    ((const char*)pat)[idx]: 315, SIB 0x05.
//  * a char index (which does reach the pointer-in-base form) still costs the
//    movsx, and lands the first difference at 0x1e instead of 0x3a, so it is
//    worse, not better.
// CONCLUSION unchanged: with a full-dword index the SP3 X86RM puts the integer
// in the base slot, and nothing tried in six passes, several hundred free
// compiles, moves it. Anyone picking this up should start from the free
// harness above rather than spending check.py runs.
#include <ctype.h>

// FUNCTION: 0x4bc370
int __stdcall FUN_004bc370(const char* str, const char* pat)
{
    int stack[100];
    int n;
    int i;
    char c;

    stack[0] = 0;
    n = 1;
    while ((c = toupper(*str++)) != 0) {
        for (i = 0; i < n; i++) {
            int idx = stack[i];
            char pc = toupper(pat[idx]);
            if (pc == '?' || pc == c) {
                stack[i] = idx + 1;
            } else if (pc == '*') {
                if (n < 100)
                    stack[n++] = idx + 1;
            } else {
                if (--n == 0)
                    return 0;
                stack[i] = stack[n];
                i--;
            }
        }
    }
    for (i = 0; i < n; i++) {
        char pc = pat[stack[i]];
        if (pc == 0)
            return 1;
        if (pc == '*' && pat[stack[i] + 1] == 0)
            return 1;
    }
    return 0;
}
