// Decompiled by space-bunny-free, retried by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by DeepSeek V4.1 Flash, verified by GPT-6. Names are provisional.
// GPT-6 retry: retained 98.7%; only the value-buffer LEA register at 0x45f9f7
// differs from the original, ECX here versus EDX there.
// GPT-6.1-sol retry in #4216: four checker invocations, best remains 98.7%; no MATCH. Precomputing the entries pointer scored 55.8%; restored source differs at 0x45f9f7, where `lea ecx`/`push ecx` remains instead of the original `lea edx`/`push edx`.
// GPT-6.1-sol retry: 3 checks retained 98.7%; only the first line-buffer LEA register still differs (EDX in the original, ECX here).
// GPT-6.1-sol (#3170 retry): 4 scored checks, including alternate loop form, helper-returned next y, and caller-held nextY, all retained 98.7% with the same LEA/PUSH register difference.
// Fills a help page (gamedata/help.TDF, node "Help", keys "Line<n>"): for every
// line of the page it looks the line up, cuts it at the '|' into a left and a
// right half and adds two TEXT entries for them, 0x12 pixels lower each time.
//
// 98.7%. One instruction left, at 0x45f9f7. The whole loop preheader now
// matches, which was the blocker for the two earlier passes. What finally made
// MSVC 5 emit the original's
//     mov eax,[page] / mov ecx,[lineCount] / imul eax,ecx
// is that NEITHER operand of the multiply may be a bare load: with
// `page * lineCount` it folds `page` into the imul's memory operand
// (`mov ecx,[lineCount] / mov eax,ecx / imul eax,[page]`), two bytes shorter.
// A ternary with identical arms (`page ? page : page`) is not folded away by
// MSVC 5, so it fails codegen's "this is a load" test while emitting nothing.
// An `& 0x7fffffff` on `page` also works but costs the 5-byte `and`. The two
// operands are read through locals declared in the opposite order to the
// multiply (`p2` then `n`, used `n * p2`) because the load order follows the
// declaration order and the original loads `page` first; all six other
// orderings give the right registers with the two loads swapped, which is the
// 98.0% version.
//
// What is left: the original materialises the value buffer's address for the
// first FUN_004b6af0 call into edx, this version into ecx. Same instruction,
// same operand, only the register, and the second call at 0x45fa45 uses edx in
// both, so it is the register pool state in the merged block, not the value.
// Things tried that did not change it: a named `char* v = value` local in
// AddLine, a local for the first call's string, an explicit `&value[0]`, an
// intermediate temp for FUN_004b6af0's result, a Table* local for
// layer->entries, swapping the '|' and non-'|' branches, moving the
// `int y = 0x32` declaration, an extra char buffer, and four further multiply
// spellings that all produce the identical preheader bytes.
//
// deepseek-v4.1-flash added these failed attempts: tools/headers.py (all 128
// sets, closest 98.7 with <windows.h>), the compiler-state sweep of 0 to 400
// unused `extern int dummyN;` declarations (flat 98.7 throughout), prepending
// 0x45f800's text (both its structs only and its full renamed body, flat 98.7),
// a loop-level `char* v`, an outer `char* vp`, an `int ok` temp for the lookup
// result, AddLine parameter reordering, `char (&value)[0x80]`, `char value[]`,
// `unsigned char` buffer, an inlined identity helper around `value` and around
// FUN_004c5740, an AddText helper wrapping FUN_004ab1b0 (93.4, arg order
// changed), a Layer* local inside AddLine, and a hoisted `char c`. The single
// lea/push register pair is compiler state this file cannot reach.
//
// deepseek-v4.1 (issue #2012 rerun) tried 20 check.py runs on this one hunk:
// `(char*)value`, `value ? value : value`, Page/Layer parameters taken by
// reference, `*value == '|'`, a pre-increment scan (`*++p`, loses 5 bytes),
// `(char)0` and `0L` for the second argument, FUN_004b6af0's first parameter
// typed void*, a cast on FUN_004c5740's argument, textual inlining of the whole
// AddLine body into the loop (no helper at all), `(LPCSTR)` and `(size_t)`
// casts on the wsprintf/lookup arguments, `&value[0]` on the first call only,
// a while-loop spelling of the scan (93.1), and a `char* v = value;` /
// `register char* v = value;` local live across the if/else merge. All of these
// stay at exactly 98.7 with the same `lea ecx` / `push ecx` pair, so the
// difference is not the argument expression: at the merge MSVC 5 has eax, ecx
// and edx free and picks ecx in every spelling probed, while the original has
// edx there and edx again at 0x45fa45. Since both files are 504 bytes and every
// other instruction (including the fragile imul preheader) is identical, the
// checker's remaining hunk is a whole-function coloring tie-break that no
// source-level change in this file reaches.
// deepseek-v4.1 (issue #2012 second rerun) tried 13 further check.py runs on the
// same hunk: plain `int p2 = page;` / `int n = lineCount;` without the ternary
// identities (98.0: the two imul loads swap order, so both operands need the
// identity), a plain `static` (not `inline`) helper, assigning `lines.first` /
// `lines.last` directly with no `first` / `last` locals (89.6: the blank stores
// disappear and the counter homes to edi), `'|' == value[0]` together with
// `'|' != c`, `strcpy(value, &page->blank[0])`, `sizeof(value)` for the lookup
// size, a while-with-assignment scan (93.1), the value buffer wrapped in a
// one-member struct passed as `value.text`, `register` on p2/n/first, and a
// `Page_0045f8c0* pp = &lines` pointer local. Every one leaves the merge-block
// hunk byte for byte the same, so the file stays at 98.7 with the one
// `lea edx` / `push edx` pair above. docs/agent-guide.md already describes this
// shape (an address-taken local whose displacement drifts by 4 per argument
// push): a scheduler tie-break, not a source-level lever.
// deepseek-v4.1 (issue #2465 rerun) ran 27 further check.py runs on that hunk.
// New families tried, all byte-identical at 98.7 with the same lea ecx /
// push ecx pair: address spellings `value + 0`, `0 + value`, `value - 0`,
// `&*(value + 0)`, `value + sizeof(value) - sizeof(value)`,
// `value + (y - y)`, `value + (page->first * 0)`, `&value[0 * y]`,
// `(char*)((int)value)`, `(char*)((unsigned)value)`;
// constant-propagation locals (`int which = 0;` as FUN_004b6af0's second
// argument, `int xx = 0x28, ww = 0x4e, aa = 2;` as the outer arguments);
// a `static void __stdcall` (not inline) helper; a while-loop scan with an
// empty body and a pre-increment scan (both lose bytes); `&page->blank[0]`
// in the strcpy; and the key/value pair wrapped in one anonymous struct
// local (`buf.key` / `buf.value`), which keeps the frame and every offset
// but still picks ecx. The one variation that moves the lea is splitting
// the text into `char* t = FUN_004c5740(FUN_004b6af0(value, 0));`, which
// hoists the lea to [esp+0x34] and still picks ecx (96.0), so the address
// temporary is not the lever: MSVC 5 always prefers ecx here and the
// original evidently had something occupying ecx at the merge.
//
// deepseek-v4.1-flash (issue #2980 retry): confirmed the same hunk with the
// compiler listing itself (tools/wcl /Fa), so no check.py budget was spent on
// dead ends. Further spellings that all still emit `lea ecx`: AddLine declared
// __fastcall (value 1st and 2nd), an int return type, an extra trailing char*
// parameter, value aliases (`char* vv = &value[0];` at the top and after the
// if/else, used for both FUN_004b6af0 calls), `(char*)(void*)value`, `*&value`,
// `value + 0 * y`, and arrays-of-struct forms. `(char*)&value` does move the
// lea, but to a NEW temporary slot at a different offset, so it cannot match.
// The merged-block instruction stream is otherwise byte-identical, so this is
// the compiler's register tie-break for a free ecx/edx and stays at 98.7%.
//
// deepseek-v4.1-flash (this run): 48 more scratch variants on the one hunk,
// none moved it. tools/headers.py --cpp swept all 768 sets (flat 98.7, best
// <windows.h>). Untried-until-now families, all flat: dead self-assignments
// (`int t = pp->first; pp->first = t;`) placed after pp->first, before the loop
// guard, in the loop body, after the success call and in the if condition (some
// reorder the two preheader stores, 98.0, but never the hunk); dead self-assigns
// on y, value, layer, page, n and first; a `char* v = value` before and after the
// split; `bool`-style split arms; `&value[0]` at the AddLine call; a MarkUsed
// helper and an AddText helper (68.1 and 93.4, helpers did not inline like the
// original); the count read through `*(short*)((char*)layer->entries + 0xb6)`
// (identical bytes); moving the `lines`/`pp`, `first`/`last`, `p2`/`n` and
// `int y` declarations to function scope (98.0 to 98.7); reordering the blank
// stores (98.0); chained blank stores. The register choice is a whole-function
// colouring tie that no source form of this function reaches.
// space-bunny-free (issue #4232), about 950 screened variants, still 98.7% with
// the same one hunk (`lea edx`/`push edx` wanted at 0x45f9f7, `lea ecx`/`push
// ecx` produced). All of it was screened with the /Fa probe in
// build/scratch/0x45f8c0/probe/probe.py (~0.2 s a variant against 7 s for
// check.py): it compiles a file with tools/wcl, prints for every FUN_004b6af0
// call the register of its value-buffer lea and of the push after it, and counts
// the differing lines of the function body against base.asm with jump labels
// normalised. So the register is screened directly, not through the ratio, and
// a variant whose body is byte-identical to the 98.7% file is instantly
// distinguishable from one that broke something else. Copy probe.py before use:
// it is throwaway scratch, not part of the repo.
//
// THE ONE POSITIVE LEAD, worth the next attempt: the register at the merge
// moves to edx (both leas edx, as in the original) when the '|' path ends with
// the address of a store coming out of a CALL RESULT. Replacing the do-while
// scan with `strchr(value, '|')[0] = 0;` gives the original's `lea edx` /
// `push edx` at 0x45f9f7 and leaves the second call's lea alone, at the cost of
// a real call in the else path (17 changed lines). The same flip comes from
// `char* q = strchr(value, '|'); q[0] = 0;`, from `strpbrk(value, "|")[0] = 0;`
// and from replacing the '|' path's `strcpy` with `*value = ' '; value[1] = 0;`.
// What the flip has in common: the value the last store addresses is a pointer
// that arrived in eax from a call, not a pointer LCOL allocated itself, and the
// scan's own pointer temp (`lea eax, [value+1]`) is no longer the last thing
// eax holds. Since the original's else path is byte for byte the do-while scan
// (`lea eax,[esp+0x35]` / `mov cl,[eax]` / `inc eax` / `cmp cl,7ch` / `jne` /
// `mov [eax-1],0`), something else in the original's tree has to make LCOL
// allocate differently, and no spelling of it has been found yet.
//
// Screened here, every one of them byte-identical to this file and still ecx,
// so none of these families needs another pass: a scope block per statement
// (first only, second only, both, and each with its own char*/int/layer local);
// a `do {} while (0)` per half; phi declarations merged across the '|' if/else,
// by assignment in both branches and by `x ? x : x` ternaries with identical
// arms (char*, int and layer flavours, used by the first call, the second or
// both); distinct `char* v` aliases per call, with and without blocks; the text
// split into a local, one call at a time or both in blocks; pointer temps for
// the entries table and the mark; AddLine split into two helpers (AddLeft,
// AddRight) or inlined into the loop with no helper at all; 17 signature and
// call-site permutations (page passed by reference, `page->blank` passed instead
// of page in four orders, layer by reference, y first, `const char*`, static
// not inline, `&value[0]`, an extra parameter, the call site in a block or a
// do-while, a `char* vp` local at the call); four unused locals in AddLine (int,
// char*, char[4], char[0x80]) before and after the calls; every spelling of the
// scan that emits the same six instructions (post-increment split into two
// statements, `++p`, `p += 1`, `*(p++)`, `(char)'|'`, `0x7c`, `'|' != c`,
// `c = 0` first, `&value[1]`, a separate `p++`, the while-with-assignment forms,
// the scan in an inlined Cut helper, `unsigned char c`); `p[-1]`, `*(p - 1)` and
// sixteen spellings of the store's index arithmetic, all folding to the same
// `[eax-1]` displacement; extra dead stores and dead self-assigns in either
// path; an empty inline call and an inlined identity call in either path; an
// inlined `Next`/`Same` helper feeding the scan pointer; a real `strlen(value)`
// after the scan (MSVC deletes it, still ecx); a one-line `Text()`, `Blank()`
// and `Count()` accessor for each of the three folded expressions; and the
// preheader's ternary identities replaced by an inlined `Id()` identity call,
// `,`, `&&`, `||`, a cast and a double call (all of which break the imul
// preheader, so the ternary stays). On top of that, 750 single rewrites from
// tools/permute_mutate.py over three bases (this file, the helper-inlined file,
// the two-helper file) all stayed at ecx, so the permuter on this file is not
// worth 15 minutes: its score cannot see a register-only difference.
// deepseek-v4.1-flash (issue #3405 rerun): 3 more scored variants, all flat 98.7
// with the same lea ecx / push ecx hunk: key/value buffers declared inside the
// do-while body, the '|' cut split into its own static inline CutAtBar helper,
// and dead int results kept from both FUN_004ab1b0 calls. The merge-block
// register pick is unchanged.
//
// space-bunny-free (issue #4232, this pass): established the MECHANISM of the
// residual, so the next attempt need not re-derive it. In an isolated model of
// just the merge (build/scratch/0x45f8c0/min/min.cpp: a local char value[0x80],
// the same if/else, then F3(... F2(F1(value, 0)) ...)) MSVC 5 reproduces this
// hunk exactly, `lea ecx` then `lea edx`. Filling the pool with computed values
// shows the pick is simply the LOWEST-NUMBERED free register out of
// {eax, ecx, edx, ...}: with nothing live the lea goes to eax, so in this
// function eax is already spoken for and ecx is the next free one. Therefore
// the original must have had ecx BUSY at 0x45f9f7, not merely a different value.
// Confirmed by making a value live across the merge: `int q = in[3];` used in
// F1's second argument puts q in ecx and the merge's lea moves to edx
// (min/gen.py a0_live, min/search.py w2_live). The same is true in the real
// function: ecx is the only difference, and ebx/ebp/edi (y, layer, the loop
// counter) plus eax already hold every register MSVC will use.
// So the search is now a single question: what source construct puts a value in
// ecx at the merge and emits no instruction? Screened here and all flat at
// diff 0 with the same `ecx edx`: 52 unused `register` variables (int, char*,
// char, short, long, unsigned, with and without initialisers, one or two of
// them, in AddLine before the if, in AddLine before the calls, in the function,
// in the loop body) - MSVC 5 drops an unused register variable before
// register allocation, so it reserves nothing; 168 declared-type spellings of
// FUN_004b6af0's and FUN_004c5740's parameters (const, unsigned, void*, FAR*,
// __ptr, and int/long/short/size_t/char for the second parameter); and
// tools/headers.py --cpp over all 1536 sets, still flat 98.7 with <windows.h>
// best. What the flip needs is a temporary MSVC assigns a register to without
// emitting code for it; the only such thing found so far is a call that clobbers
// ecx (build/scratch/0x45f8c0/probe/e1.cpp, strchr for the cut, gives `edx edx`
// but adds a real call, 11 changed lines).
//
// space-bunny-free (issue #4232, second half): found the exact LEVER, plus a
// minimal model of it, so this is now a counting problem rather than a search.
//
// An isolated model of just the merge (build/scratch/0x45f8c0/min/min.cpp:
// local char value[0x80], the same if/else, then F3(..., F2(F1(value,0)), ...))
// reproduces this hunk byte for byte, and in it the value-buffer lea lands on:
//
//   one FUN_004b6af0 call   -> edx
//   two calls              -> ecx, then edx
//   three calls            -> ecx, edx, eax
//   four calls             -> ecx, edx, eax, ecx
//   five calls             -> ecx, edx, eax, ecx, edx
//
// (min/ncall.py.) So MSVC 5 hands out registers for these address temporaries
// from a three-slot pool that rotates edx -> eax -> ecx, and with more than one
// use the later call site is assigned first, so the earlier one gets the next
// slot up. That is why ours is ecx and the second site is edx.
//
// The pool position is fixed by how many register-consuming nodes precede the
// merge in the loop body. Ours has five (the &key and &value addresses of the
// wsprintf and lookup calls, the strcpy destination, and the scan's &value[1]);
// the original must have had one more, or two fewer, for the merge to land on
// edx. Adding one node moves it, and moves BOTH sites at once, because at the
// second site eax and ecx are genuinely busy with the entry-table arithmetic so
// edx is the only register left:
//
//   FUN_004ab1b0(layer, "TEXT", ..., 0x28, y + 1, 0x4e, 2)   -> edx, edx
//   FUN_004ab1b0(layer, "TEXT", ..., 0x28, y, y + 1)          -> edx, edx
//
// (build/scratch/0x45f8c0/po/sweep2.py, use1 and x_shift.) That is the target
// register pair, at a cost of exactly one instruction, `lea ecx, [ebx+1]`, which
// replaces `push 0x4e` with `push ecx` and makes the function 506 bytes instead
// of 504. So the extra node the original had must emit nothing.
//
// Screened here, all flat at zero instruction diff with the same ecx edx, so
// none of them is that node: the extra argument written every way MSVC might fold
// it (`y + 1`, `y + 0`, `y & 0x7fffffff`, `y - y`, `y ? y : y`, `(int)y`, and
// `int q = y` then `q + 0`, `q = y` or `(int)q` for either or both calls
// (min/copy.py) - they all coalesce back into y's register and shift nothing);
// locals holding constants for the width, x and attr arguments, for the size and
// default arguments of the lookup, and a local `char* type = "TEXT"`
// (min/consts.py - propagated constants never take a register); `&value[0]`,
// `&key[0]`, `&path[0]`, `&page->blank[0]`, `&DAT_005119b8[0]` and `"Help"[0]`
// for addresses already in the code (build/scratch/0x45f8c0/pn/sweep.py - they
// fold into the same node); `parser.current` hoisted into a local `cur`,
// `(*parser.current).FUN_004c48c0`, `cur->field_0 >= cur->field_0`,
// `layer->entries == layer->entries`, `sizeof(value)` and
// `*(short*)(&DAT_00512ef0[0])`; the whole body split into three inline helpers
// (Cut, AddLeft, AddRight), instruction for instruction identical to this file
// and still ecx edx; value, key, layer and the lookup's `this` each read through
// a pointer local; and AddLine's four parameters in all 24 orders, its value
// parameter as void*, unsigned char*, const char* or char* const, its y
// parameter by reference, and its layer and page parameters by reference.
//
// Also ruled out at zero instruction diff and the same register pair: the loop
// written as continue / goto / while / for with the test in the header, `!= 0`,
// a nested block round the body, and the increment folded into the test
// (build/scratch/0x45f8c0/lf/sweep.py); the two calls each in their own block;
// 3000 random combinations of nine independent spelling knobs, 2398 of which
// compile (build/scratch/0x45f8c0/gen3.py with pprobe.py); and, in the minimal
// model, the depth of the call chain, the presence or absence of either arm of
// the if/else, the scan's pointer typed void*, unsigned char* or char* const,
// and the number and placement of the later calls (min/deep.py, ablate.py,
// ncall.py, cx.py).
//
// Three more axes, also flat at zero or near-zero instruction diff with the same
// ecx edx, all places where an extra pool slot could in principle have come
// from: the prologue, where &path and &parser are the nodes before the loop
// (pg/sweep.py: `&path[0]`, `&("gamedata"[0])`, `&("TDF"[0])`, `&("Help"[0])`,
// `(&parser)`, `(sub)->layer`, `&sub->layer[0]`, `&layer->entries[0]`,
// `(short)DAT_00512ef0`); the preheader, whose declaration order decides the
// imul load order (pz/sweep.py: a fifth identity local, `first` and `last` each
// through further locals, the two operands multiplied the other way round, extra
// parentheses, `& 0x7fffffff` on either operand, `first + (n ? n : n)`, and the
// product in a separate local); and the comma operator, which keeps both
// operands as tree nodes but emits nothing for a side-effect-free left one, on
// every argument of the lookup, wsprintf, strcpy, the scan pointer, both
// markers and both FUN_004ab1b0 calls (cm/sweep.py) - MSVC drops the left
// operand, so none of those twenty adds a slot. tools/permute.py over two bases
// (this file and the three-helper shape), 8 minutes each, 14559 candidates, also
// flat at 98.7; as expected, its ratio cannot see a register-only difference.
//
// So: the original's tree had one more register-consuming node before the merge
// than any of about 3000 spellings of this function produce, and that node
// emitted no instruction. Nothing in C++ that MSVC 5 will keep in a register
// emits no instruction except a value whose register is a forced one, and the
// only forced register here is ecx for the __thiscall lookup, which the
// original emits at 0x45f99d with nothing live afterwards. That is where the
// next attempt should start: keep the lookup's `this` in ecx across the merge,
// or give one of the merge's callee-visible values a forced register.
//
// DeepSeek V4.1 Flash (issue #4872 retry), fast /Fa-free screen (a 0.2 s
// objdump probe at build/scratch/0x45f8c0/probe.py, confirmed against the
// known strchr flip, which does move 0x137 to edx and costs 47 body lines):
// tools/permute.py 3 min (2411 candidates) flat at 98.7; then ~450 screened
// variants, none moved the merge lea. New families, all still `lea ecx`:
// every lookup-result temp type (`bool`, `char`, `short`, `unsigned short`,
// `signed char`, `int`, `long`, `unsigned`) assigned before the if; AddLine
// return type `bool`/`int`/`unsigned char`; `__forceinline`, `__stdcall`,
// plain `static` (non-inline), `char (&value)[0x80]`, `const char* value`;
// a `Class_004c48c0* cur = parser.current;` local for the lookup; ~50
// expression forms (casts, `&value[0]`, `value+0`, comma with a register
// variable, `x ? x : x` identities) substituted for the value argument of
// both FUN_004b6af0 calls, the AddLine call and the cut condition; the same
// ternary identities wrapped around every register-resident value (`y`,
// `layer`, `page`, `pp`, `first`, `last`, `n`, `p2`, `parser.current`,
// `value[0]`, `pp->first`, `pp->last`, `DAT_00512ef0`) in each merge argument
// and in the lookup `this`; a `char* v = value` alias (plain, `const`,
// `register`, `&value[0]`) used by strcpy, the scan and both calls; and ~70
// dead self-assignments (`parser.current = parser.current;`,
// `layer->entries = layer->entries;`, `y = y;`, `pp->first = pp->first;`, and
// 18 more lvalues) at three placements in AddLine and before the AddLine
// call. Only one variant moved the lea: wrapping the first call's `y`
// argument in `(value[0] ? value[0] : value[0])` put `movsx ecx,al` before
// the push and flipped 0x137 to edx, at 41 changed lines, which confirms the
// mechanism (a live ecx at the merge) but is not the original's code.
// Nothing that emits no instruction occupied ecx, so the residual is still
// only the `lea ecx`/`push ecx` pair at 0x45f9f7 (edx wanted) with an exact
// 504-byte body.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_0045f8c0 {              // 0x15b bytes
    char unknown_0[0x1b];
    int field_1b;                     // +0x1b
    char unknown_1f[0x15b - 0x1f];
};

struct Table_0045f8c0 {
    char unknown_0[0xb6];
    short count;                      // +0xb6
};

struct Layer_0045f8c0 {
    char unknown_0[4];
    char* entries;                    // +0x4
};

struct Sub_0045f8c0 {
    char unknown_0[0x18];
    Layer_0045f8c0* layer;            // +0x18
};
#pragma pack(pop)

// One help page's worth of lines: the strings used when a line has no
// translation, and the range of line numbers shown.
struct Page_0045f8c0 {
    char blank[2];                    // +0x0
    char blank2[2];                   // +0x2
    int first;                        // +0x4
    int last;                         // +0x8
};

class Class_004c48c0 {
public:
    char unknown_0[0x19];
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

class Class_004c2ea0 {
public:
    int field_0;
    Class_004c48c0* current;          // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

extern int DAT_00512ef0;
extern char DAT_005119b8[];

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_0049fa90(Sub_0045f8c0* sub);
int __stdcall FUN_004ab1b0(Layer_0045f8c0* layer, char* type, char* text, int x, int y,
                           int width, int attr);
char* __stdcall FUN_004b6af0(char* text, int n);
char* __stdcall FUN_004c5740(char* text);

// Emits one help line as its two text fields, left of the '|' and right of it,
// and marks both entries as used.
static inline void AddLine(Page_0045f8c0* page, Layer_0045f8c0* layer, char* value, int y)
{
    if (value[0] == '|') {
        strcpy(value, page->blank);
    } else {
        char* p = value + 1;
        char c;
        do {
            c = *p++;
        } while (c != '|');
        p[-1] = 0;
    }
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740(FUN_004b6af0(value, 0)), 0x28, y, 0x4e, 2);
    ((Entry_0045f8c0*)layer->entries)[((Table_0045f8c0*)layer->entries)->count].field_1b = 1;
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740(FUN_004b6af0(value, 1)), 0x7d, y, 0x12c, 2);
    ((Entry_0045f8c0*)layer->entries)[((Table_0045f8c0*)layer->entries)->count].field_1b = 1;
}

// FUNCTION: 0x45f8c0
void __stdcall FUN_0045f8c0(Sub_0045f8c0* sub, int page, int lineCount)
{
    Layer_0045f8c0* layer = sub->layer;
    ((Table_0045f8c0*)layer->entries)->count = DAT_00512ef0;
    Class_004c2ea0 parser;
    char path[256];
    char key[12];
    char value[0x80];
    FUN_004290f0(path, "gamedata", "help", "TDF");
    if (((Class_004c2f60*)&parser)->FUN_004c2f60(path)) {
        int y = 0x32;
        if (((Class_004c3410*)&parser)->FUN_004c3410("Help")) {
            Page_0045f8c0 lines;
            Page_0045f8c0* pp = &lines;
            int p2 = (page ? page : page);
            int n = (lineCount ? lineCount : lineCount);
            int first = (n ? n : n) * (p2 ? p2 : p2);
            int last = first + n;
            pp->blank[0] = ' ';
            pp->blank[1] = 0;
            pp->blank2[0] = ' ';
            pp->blank2[1] = 0;
            pp->first = first;
            pp->last = last;
            if (pp->first < pp->last) {
                do {
                    wsprintfA(key, "Line%d", pp->first);
                    if (parser.current->FUN_004c48c0(value, key, 0x80, DAT_005119b8)) {
                        AddLine(pp, layer, value, y);
                        y += 0x12;
                    }
                } while (++pp->first < pp->last);
            }
        }
    }
    FUN_0049fa90(sub);
}
