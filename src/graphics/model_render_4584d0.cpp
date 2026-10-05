// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by
// claude-opus-5-5 (#4634), finished by deepseek-v4.1-flash, finished by
// space-bunny-free, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished
// by Fledge Alpha Free. Names are provisional.
// claude-opus-5-5 (#4634): still 82.3%. An indexed vertex loop (projected[i] /
// vertices[i]) scores 76.9%; a 12-minute permuter run (207 candidates) found
// nothing.
// Retry (deepseek-v4.1-flash, issue 3042): best unchanged at 79.6% (457 vs 461
// bytes). Tested register keyword, label+goto / outer-for loop-nesting forms,
// while-loop copy loop, idx-before-j, function-scope j, and a 0..403
// dummy-declaration sweep; all flat at 79.6%, so headers.py plus the dummy sweep
// confirm the residual i-vs-info eviction is a compiler-state tie.
// GPT-6.1-sol refinement: five checker invocations in this pass. Baseline reproduced at 79.6%.
// A single-use inline face-count getter was byte-identical (79.6%). A by-value
// projected-point helper changed frame allocation and fell to 44.4%; discarded.
// FOURTH PASS (deepseek-v4.1): nine variants, none above the 79.6% baseline, so
// this file is unchanged. The face loop rewritten as the Ghidra do-while shape
// (`if (i < info->faceCount) do { ...; i++; face++; } while (i < info->faceCount);`)
// compiles to byte-identical output (79.6%, 457 bytes), so the loop form is not
// the lever; `q[0].x`/`q[0].y`, hoisting `q` with `Point_4584d0* q;` plus a
// comma init, `int j = 0;` in the copy loop, and swapping the face-loop
// increments are all also exactly 79.6%. Moving `q++` into the loop body is
// 74.1%, y-store-first is 77.6%, swapping the vertex-loop increments is 74.1%,
// and swapping the copy-loop increments drops to 78.9%. WHAT STILL DIFFERS is
// exactly the two clusters above: (1) the vertex loop's bias convention, the
// original sinks `lea ecx,[esp+0xec]` BELOW the `jle` guard and biases the
// destination by +4 (`add ecx,8` early, stores at [ecx-0xc]/[ecx-8]) with an
// unbiased source walked late, while this file hoists `lea ecx,[esp+0xe8]`
// above the guard, biases the SOURCE by +4 (`add eax,4`, `[eax-4]`/`[eax+4]`)
// and advances the destination mid-body; (2) the single eviction at the face
// loop, where the original spills `i` to [esp+0x10] and keeps `info` in ebx,
// while this file keeps `i` in edi, spends ebx on the copy-loop index temp and
// reloads `info` from [esp+0x3f78] once per face.
//
// FIFTH PASS (deepseek-v4.1-flash): the two levers the fourth pass listed as
// untried are both measured dead. (a) The vertex loop as Ghidra's do-while
// countdown with `int* q = (int*)projected + 1` scores 55.4% with a walked
// `Vertex*` and 74.1% with `vertices[i]`, both below this for-loop baseline, so
// the vertex-loop shape is not a separate lever. (b) Hoisting `info->faceCount`
// into a loop-local `int n` scores 50.5%, much worse. Forcing `i` to have a home
// with `int* dummy = &i;` is a no-op byte-for-byte (79.6%, the store to
// [esp+0x10] never appears), so the home is a real allocator eviction, not an
// address-taken local. Everything still differs at the same two clusters.

// PARTIAL, 79.6% (unchanged by the second and third passes). Callers (0x4584b4, 0x458997, 0x4593ff,
// 0x459476) push the surface pointer as the second argument and the address of a
// 12-byte {x,y,z} struct as the third, so the argument order is
// (model, surface, camera, info, vertices, palette, useColor). `ret 0x1c` is
// seven dwords, so this is __thiscall with `this` in ecx plus six stack
// arguments, and every argument list here was checked against the pushes and
// against the callees' `ret N`: 0x4b7ee0 `ret 4`, 0x4b7f30 `ret 8`,
// 0x4c0310 and 0x4c7580 `ret 0x10` each. All four are already correct, so the
// residual below is a genuine allocation difference, not a wrong argument list.
//
// WHAT IS SOLVED, and the one change that bought most of it. The vertex loop
// must walk the DESTINATION with a pointer and index the SOURCE by `i`. That
// is, `Point* q = projected; for (i = 0; i < n; i++, q++) q->x = ...` with
// `vertices[i].x` inside, rather than `projected[i].x = ...` with
// `vertices[i]`. Indexing the destination instead costs 5.5 points
// (74.1% -> 79.6%) because it is what makes MSVC bias ecx by +4 and hoist
// `lea ecx,[esp+0xe8]` above the loop-test, and it fixes both stores. I swept
// the neighbouring shapes: walking the source too (`Vertex* u = vertices`)
// is 37.8%, `projected + i` inside the body is 74.1%, and biasing the
// destination by hand (`q = projected + 1`, `q[-1]`, a `short*` with `qx += 2`)
// is 72.8 / 74.1 / 65.3%. So the win needs BOTH a walked destination AND an
// indexed source; neither alone does it.
//
// FRAME ARITHMETIC, measured not guessed. The `mov eax,0x3f58 / call
// _alloca_probe` size fixes `projected[2000]` and `poly[25]` exactly: 2000 and
// 25 reproduce 0x3f58, and I measured EVERY neighbouring size (1994 to 2002
// and 22 to 28) at 66-67%, i.e. one instruction off, because the frame constant
// changes. Do not touch these two numbers. `poly[25]` at frame+0x20 and
// `projected[2000]` at frame+0xe8 are the only two arrays; the other frame dword
// in use is `off.y` at frame+0x18.
//
// WHAT IS STILL DIFFERENT, and it is one register-priority tie I could not
// break. The whole face loop, its pre-loop test and its copy loop are one
// block: the original gives the face counter `i` a STACK HOME at frame+0x10
// (`mov dword ptr [esp+0x10],edi` at 0x458582 and again at 0x458686, reloaded
// by `mov edi,dword ptr [esp+0x10]` at 0x4585d3), which frees edi for the copy
// loop's index, and the copy loop then runs edx = indices pointer, ecx = `j`,
// edi = index. This file keeps `i` in edi, so the copy loop has to use ebx for
// the index and reloads ebx with `info` afterwards. I could not make MSVC give
// `i` a home: putting it in a local aggregate (`struct { int i; } c;`) scores
// exactly 79.6%, a separate declaration for the face counter is 79.6%, a
// `short` count 79.6%, declaring `int i, j;` together 79.6%. Hoisting
// `info->faceCount` into a named local, which is what the original's
// `mov eax,[ebx+8]` before the loop test suggests, is much WORSE (52.2%), as is
// an explicit `int k = *idx++` (58.0%) and a walked `poly` pointer (71.4%).
// Ten inner-loop spellings (field-by-field assignment, `idx[j]`, a walked
// `poly`, a walked `idx`, a hoisted count, y-before-x) all measure 69-79% and
// none reaches the original's shape. It is a priority tie, not a scheduling
// accident: do not re-sweep it without a new idea. Since then, declaring the
// inner counter `j` at its point of use inside the loop body was tried and
// scores exactly the same 79.6% (457 bytes), so that lever is dead too.
//
// A MEASURED NEGATIVE worth keeping: the brief's "redundant store is a
// variable initialiser" lever is dead here. Removing `int found = 0` style
// initialisers, and hoisting the loop bounds, all lower the score. The one
// thing that is a genuinely redundant store in the original is
// `mov dword ptr [esp+0x10],edi` at 0x458582, written before the pre-loop test
// and then rewritten at the latch; I could not get MSVC to emit it.
//
// No suspected bug in the original. The face counter is spilled to the frame
// and reloaded, which is an allocation artefact, not a mistake.
//
// deepseek-v4.1-flash sweep, all measured with a real or free check run, none
// above 79.6%: the missing `mov [esp+0x10],edi` is one tie between `i` and
// `info`. Both versions spill exactly two values across the copy loop; the
// original spills `i` and `surface`, this file spills `info` and `surface`.
// `surface` loses in both because ebp is the projected-load scratch, so the
// only free choice is which of `i`/`info` keeps a callee-saved register, and
// the original picks `info` (ebx) while this file follows the ESI/EDI/EBX/EBP
// preference and gives edi to `i`. The vertex-loop diff (source biased +4 in
// this file, destination biased +4 in the original) is the same tie read out
// in a different block. Sweeps that did NOT move it: walked source and/or
// walked destination plus bias forms (projected+1, pre-increment, int* copies,
// struct copy, q[0], q+i, vertices+i); a separate vertex counter (scoped or
// top-level); a separate or late face counter; `while`/`for(;;)` loop forms;
// `i`/`j`/`k` init and declaration reordering; `unsigned`/`short` counters; a
// named copy index (`int k`, 58.0%); a `static inline` copy helper (72.2%);
// framing `j` in its own scope; STL headers (`<string>`, `<vector>`, `<list>`,
// `<map>`, `<iostream>`, etc. all drop to 51.4%) and the C headers (all 79.6%).
// The allocator choice is stable across every one of these, so the next attempt
// needs a construct that changes the tie, not a re-sweep of loop shape.
//
// THIRD PASS (space-bunny-free), one new lever, negative, but it pins the
// remaining difference down to a single eviction.
// 1. headers.py: all 128 header sets give exactly 79.6%, so nothing here is a
//    header side effect.
// 2. Forcing the face counter onto a frame slot with a reference alias
//    (`int faceSlot; int& fi = *(int*)&faceSlot;`, the 0x4db450 trick applied to
//    a local instead of a dead parameter) does NOT work on MSVC5: it folds the
//    reference straight back into a register and the generated bytes are
//    identical, 79.6%, not one instruction different. So the original's
//    `mov [esp+0x10],edi` is not a real frame home reached that way; it has to
//    be a register-allocation spill, and the only route to it is to make the
//    allocator evict that variable.
// 3. WHAT IS STILL DIFFERENT, counted exactly. The copy loop of the original
//    has SEVEN simultaneously live values: eax = poly walk, ebp = x scratch,
//    edx = indices walk, ecx = j, edi = index, esi = face walk, ebx = info.
//    That is all seven general registers, and the index temp is what forces the
//    eviction: `mov [esp+0x10],edi` at 0x458582 sits between `mov eax,[ebx+8]`
//    and the compare, where there is no room to fold a memory operand into the
//    compare, so it is a spill. The face counter goes to [esp+0x10], the freed
//    edi becomes the index temp, and `info` keeps ebx for the whole function.
//    This file instead spends ebx on the index temp and loses `info`, which is
//    then re-loaded from its argument home once per face (the extra
//    `mov ebx,[esp+0x3f78]` at the bottom of the copy loop, and the
//    `cmp edi, dword ptr [ebx+8]` pre-test with no `mov eax` before it).
//    So the whole residual is ONE eviction decision: which of the face counter
//    and `info` has the lower register priority at the copy loop. Guide
//    "Register priority" (line 515) says the way to move that is a use of the
//    winning variable that folds away, and nothing tried here has yet given
//    `info` one more foldable reference than the counter, which has three
//    (init, test, latch). Two untried ideas in that direction: read
//    `info->vertexCount` once more inside the vertex loop in a form that folds
//    into the load already there, or compare against `info->faceCount` in the
//    copy loop instead of through the walked `face`.
// EIGHTH TO TENTH PASS (Fledge Alpha Free, issue 4848): 82.3% -> MATCH.
//
// The two ties the seventh pass left were broken by source shape, not by a
// different compiler state:
//  1. The vertex loop's SOURCE must be walked through the PARAMETER itself,
//     `for (i = 0; i < info->vertexCount; i++, vertices++)` with `vertices->x`
//     etc. An explicit local `Vertex* v = vertices` (or `int* u`) moves off.y
//     into a register and changes the frame to 0x3f4c; walking the parameter
//     keeps the frame at 0x3f58 and makes MSVC leave the source pointer
//     UNBIASED ([eax], [eax+8], then [eax-8]) instead of biasing it +4. The
//     seventh pass had listed this as "NOT REACHABLE".
//  2. The `off` aggregate must declare its spilled field in the middle,
//     `struct Off_4584d0 { int a; int y; int b; };`. With {a,b,y} the frame is
//     0x3f5c and off.y lands at [esp+0x1c]; with {a,y,b} off.y lands at
//     [esp+0x18], every displacement matches, and the face-loop eviction flips
//     to the original's (i gets its home at [esp+0x10], info keeps ebx). That
//     single field order was worth 12 points (87.2 -> 99.3).
//  3. The last 0.7% was a schedule tie between the two reloads after the copy
//     loop (surface then i in the original). A permuter run from the 99.3%
//     version found the match in 0.32 min with three mutations, two of which
//     survived cleanup: the copy loop as `if (FaceCount(face) > j) { while (1)
//     { ...; int count = face->count; if (j >= count) break; } }` and a named
//     return value inside the guard helper.
//
// Load-bearing constructs in the final file, each measured by removing it
// alone: `#include <string.h>` (without it 55.7%); the FaceCount helper for the
// copy loop guard (without it 79.9%); the copy loop's `while (1)` shell and its
// `count` local (a `do/while`, a `for (;;)`, or a direct `j >= face->count`
// drop to 99.3% or worse); the parameter walk above; and the `{a, y, b}` field
// order. The eighth pass's other permuter leftovers were tidied back to
// plausible source: no self-assignments, no leftover temporaries, the both-arms
// firstFace assignment and the `goto skip0` early exit are all that remain of
// them. Renaming the helper and the temporary and reindenting do not change the
// bytes.

#include <string.h>
extern char* g_game;

#pragma pack(push, 1)
struct View_4584d0 {
    char unknown_0[0x6a];
    int originX;                             // +0x6a
    int originY;                             // +0x6e
    int originZ;                             // +0x72
};
#pragma pack(pop)

struct Model_4584d0 {
    char unknown_0[0xc];
    View_4584d0* view;                       // +0xc
};

struct Point_4584d0 { int x; int y; };
struct Vertex_4584d0 { int x; int y; int z; };
struct Vec3_4584d0 { int x; int y; int z; };

struct Pic_4584d0 {
    void* pic;                               // +0x0
    char unknown_4[4];
};

struct Flags_4584d0 {
    union {
        unsigned int raw;
        struct {
            unsigned int a : 1;
            unsigned int b : 1;
            unsigned int c : 1;
            unsigned int rest : 29;
        } bits;
    };
};

struct Face_4584d0 {
    int unknown_0;                           // +0x00
    int count;                               // +0x04
    int unknown_8;                           // +0x08
    unsigned short* indices;                 // +0x0c
    Pic_4584d0 pic;                          // +0x10
    unsigned short* color;                   // +0x18
    Flags_4584d0 flags;                      // +0x1c
};

struct PieceInfo_4584d0 {
    char unknown_0[4];
    int vertexCount;                         // +0x04
    int faceCount;                           // +0x08
    int firstFace;                           // +0x0c
    char unknown_10[8];
    unsigned short* color;                   // +0x18
    char unknown_1c[0xc];
    Face_4584d0* faces;                      // +0x28
};

void* __stdcall GetGafSequenceFrame(Pic_4584d0* ref);
void* __stdcall GetGafFrame(unsigned short* table, int index);
void __stdcall FillPolygon(void* surface, Point_4584d0* points, int count, int flags);
void __stdcall DrawFrameQuad(void* surface, void* pic, Point_4584d0* points, void* src);

class Class_004584d0 {
public:
    void DrawPiece(Model_4584d0* model, void* surface, Vec3_4584d0* camera,
        PieceInfo_4584d0* info, Vertex_4584d0* vertices, unsigned int palette,
        int useColor);
};

// SIXTH PASS (space-bunny-free): 82.3%, 457 of 461 bytes, up from 79.6%. Only
// ONE change bought it, and it is the inner copy loop. Written as
//     for (j = 0, p = face->indices; j < face->count; j++, p++)
//         poly[j] = projected[*p];
// (a separate walked index pointer, initialised in the for header, instead of
// `poly[j] = projected[*idx++]` with `idx = face->indices` above the loop) MSVC
// now agrees with the original on six instructions of that loop: `mov edx,[esi +
// 0xc]` (index array into edx), `xor ecx,ecx` (j into ecx), `inc ecx`, `add eax,
// 8`, `add edx,2` and `mov [eax-8],ebp`. Before, edx and ecx held those two the
// other way round. The same loop as `for (j = 0, k = 0; ... j++, k++) poly[j] =
// projected[k];` with an `unsigned short k` scores 80.3%: it also gets j into
// ecx, but then has to zero-extend k (`mov ebx,edx; and ebx,0xffff`) where the
// original gets the zero extension free from `mov di, word ptr [edx]`. Do not go
// back to the `*idx++` spelling: with it, j lands in edx and the index pointer in
// ecx, and the whole function sits at 79.6%.
//
// WHAT IS STILL DIFFERENT, from an instruction-level differ
// (build/scratch/0x4584d0/differ.py, 121 of 151 aligned instructions equal). It
// is TWO independent ties, and the second one explains thirteen of the thirty
// remaining instructions.
//  (1) THE VERTEX LOOP (original instruction indices 20-34, eight instructions).
//      Which array gets the biased induction variable. The original sinks
//      `lea ecx,[esp+0xec]` BELOW the `jle` guard and biases the DESTINATION by
//      +4, with the increment at the top of the body (`add ecx,8`, stores at
//      [ecx-0xc] and [ecx-8]); the source pointer is unbiased ([eax], [eax+8],
//      then `add eax,0xc` and [eax-8]). This file hoists the `lea` above the
//      guard, anchors the DESTINATION at +0 (stores at [ecx] and [ecx-4]) and
//      biases the SOURCE by +4 instead (`add eax,4`, [eax-4], [eax+4],
//      [eax-0xc]). MSVC puts the +4 on the source in every spelling I could
//      reach, including an explicitly walked `Vertex* u`, a walked `int* u`, and
//      `int* u = (int*)vertices` with u[0], u[2], u[1]. MEASURED DEAD, all on
//      this base: walked source 55.4% (and it also moves the frame to 0x3f4c and
//      pushes off.y to a stack slot, so the original cannot have walked the
//      source), `projected[i]` 76.9%, `q[i]` through a pointer 82.3% (identical
//      bytes), counting down from vertexCount 70.9% / 71.6%, two induction
//      variables 82.3% (identical), `char*` offsets 55.4%, x and y stored in the
//      other order 77.6%, the values computed into locals first 82.3% (identical),
//      a struct assignment 47.1%, do-while 65.1%, while 74.1%.
//      The one spelling that DOES produce the original's store offsets is a
//      biased `int*`: `int* q = (int*)projected + 1; ... q[-3] = x; q[-2] = y;
//      q += 2` gives `mov [ecx-0xc],ebx` in the right place, but only scores
//      75.5-76.9% because the `lea` is still hoisted above the guard and the
//      second store lands at [ecx-0x10]. So the anchor is reachable and the
//      increment position is not; they are two separate decisions.
//  (2) THE FACE LOOP EVICTION (indices 44, 45, 50, 53, 55-58, 66, 68, 71, 73-77,
//      79, 140, 141). Still exactly the `i`-versus-`info` eviction every earlier
//      pass described, and it is worth thirteen instructions. The copy loop has
//      seven simultaneously live values (eax poly walk, ebp x scratch, edx index
//      walk, ecx j, edi index, esi face walk, ebx info) plus the face counter
//      `i`, so exactly one of them has to live in memory. The original evicts
//      `i` to frame+0x10 (`mov [esp+0x10],edi` in the preheader, reload
//      `mov edi,[esp+0x10]` after the copy loop, store again at the latch), which
//      frees edi for the index temp and leaves `info` in ebx for the whole loop;
//      with `i` in edi the temp takes ebx instead and `info` is reloaded from its
//      argument slot once per face. Everything else in that cluster follows from
//      this one choice: the pre-test materialises the count (`mov eax,[ebx+8];
//      mov [esp+0x10],edi; cmp edi,eax`) because edi is about to be reused, and
//      the latch stores `i` again for the same reason.
//
// MEASURED DEAD for tie (2), all flat at 82.3% on this base: declaring and
// assigning the face counter as a separate variable from the vertex counter;
// swapping the declarations of `j`, `p` and the flags local; `idx[j]` instead of
// `*p`; `poly` walked as well (73.7%); short, unsigned short and char for `j`
// (66-74%); a hoisted `int n = face->count` (72.2%); a hoisted
// `int nf = info->faceCount`; duplicated `info->firstFace`, `info->faceCount`
// and `info->vertexCount` reads to raise `info`'s priority (53-76%, all worse
// because the extra compare survives); a null-pointer or dead-store check in the
// face loop, before it and in the vertex loop; `i = i`, `flags = flags` and
// `for (i = i, face = face; ...)` self-assignments (all 82.3%, so the
// self-assignment lever is dead here too); and writing `projected[i]` through a
// pointer. Uncalled `static inline` helpers at file scope are flat as well: three
// shapes (identity int, do-nothing int*, unsigned short cast) at N = 0 to 8
// helpers all score exactly the same, so that lever is exhausted.
//
// No suspected bug in the original. The face counter being spilled to the frame
// and reloaded around the copy loop is an allocation artefact of eight values
// competing for seven registers, not a mistake.
// SIXTH PASS (space-bunny-free): best 80.3% (457 of 461 bytes), up from 79.6%.
// The one change that bought it is the inner copy loop: giving the vertex index
// its own walked `unsigned short` counter, `for (j = 0, k = 0; j < face->count;
// j++, k++) poly[j] = projected[k];`, instead of `projected[*idx++]`. That puts
// `j` in ecx and the index counter in edx, which is what the original does
// (`xor ecx,ecx` / `inc ecx`, `cmp ecx,edi`), so the copy loop now agrees on two
// of its three registers where before it had them the other way round.
// Uncalled `static inline` helpers at file scope are flat here: three shapes
// (identity int, do-nothing int*, unsigned short) at N = 0 to 8 every score
// 79.6%, so the "compiler state" reading of the residual is confirmed for that
// lever. An instruction-level differ (build/scratch/0x4584d0/differ.py) shows
// 118 of 154 aligned instructions already equal, in three clusters:
//  (1) the vertex loop (original instruction indices 20-34): the original sinks
//      `lea ecx,[esp+0xec]` below the `jle` guard and biases the DESTINATION by
//      +4 with the increment at the top of the body (`add ecx,8`, stores at
//      [ecx-0xc]/[ecx-8]) while walking the source unbiased; this file hoists
//      the `lea` above the guard, biases the SOURCE by +4 instead and advances
//      the destination mid-body. Every spelling of the pair I tried keeps the
//      bias on the source: walked source (52.7%), walked source with the
//      countdown (`u++`) (61.9%), `projected[i]` (74.1%), `(int*)projected+1`
//      with p[-3]/p[-2] (67.3%), counting down from vertexCount (70.9%),
//      `q[i]` through a pointer (79.6%, identical), two induction variables
//      (79.6%), char* offsets (74.1%), and x/y swapped in the body (77.6%).
//  (2) the face loop (44-79): the original gives the face counter `i` a stack
//      home at frame+0x10 and keeps `info` in ebx, this file keeps `i` in edi,
//      spends ebx on the copy-loop index and reloads `info` once per face.
//  (3) the latch (140-141): the extra `mov [esp+0x10],edi`.
// No suspected bug in the original. The face counter being spilled to the frame
// and reloaded is an allocation artefact, not a mistake.
//
// SEVENTH PASS (space-bunny-free): best UNCHANGED at 82.3% (457 of 461 bytes).
// Baseline reproduced, then two 15-minute permuter runs: 11320 candidates and
// 8556 candidates, both 82.3% -> 82.3%. Neither wrote a `best_ratio.cpp`, so
// there was nothing to copy back, and the file is byte-identical to the version
// the sixth pass left. What follows is the negative evidence this pass bought,
// and the one positive finding (the eviction lever is real and is now proved).
//
// METHOD, and it is the part worth reusing: a compile-only probe
// (build/scratch/0x4584d0/probe.py, with the variant generators mk.py, mk2.py,
// mk3.py, mk4.py, mk5.py beside it) compiles a scratch copy of the file with
// the project's own flags and reports six facts in about ten seconds instead of
// a sixty-second `check`: the `_alloca_probe` constant, whether `poly` still
// has its `lea [esp+0x20]`, whether `lea ecx,[esp+0xec]` lands BELOW the `jle`
// loop guard or above it, whether the two destination stores are the original's
// `[ecx-0xc]`/`[ecx-8]` or this file's `[ecx]`/`[ecx-4]`, whether the face
// counter gets a home at [esp+0x10], and whether the copy loop's index temp is
// in edi (the original) or ebx (this file). Both remaining clusters here are
// register and displacement choices rather than structure, so that screening is
// what found the two facts below. Roughly 60 variants were screened this way
// and only six were ever worth a real `check`.
//
// (1) THE VERTEX LOOP ALREADY HAS THE RIGHT LENGTH, WHICH IS WHY EVERY CHANGE TO
//     IT COSTS SCORE. Measured encodings: the original's loop is 79 bytes and
//     this file's is 82, and the whole +3 is `add eax,4` (3) plus `mov
//     ebx,[eax-4]` (3) against `mov ebx,[eax]` (2), less `mov [ecx],ebx` (2)
//     against `mov [ecx-0xc],ebx` (3). The totals differ, so touching the loop
//     shifts every later jump target and the checker's BYTE-similarity
//     percentage falls even when the instruction stream agrees: this file is
//     reported as 82.3% with "ignoring internal jump targets" 87.1%, while a
//     variant that fixes two real instructions and grows by one byte is
//     reported as 78.2% with 89.8% once jump targets are ignored. Fix the
//     encodings only if you can fix them at 79 bytes.
//
// MEASURED DEAD, an 18-cell destination x source sweep (mk2.py) plus seven
// single-variable forms (mk.py): the original needs three things at once, and
// no source produces all three.
//   - `lea ecx,[esp+0xec]` BELOW the `jle` guard. Reached by an indexed
//     destination (`projected[i].x`, `projected[i].y`, both arrays indexed), but
//     that form then writes y at [ecx-4] instead of [ecx-8]. Every hand-biased
//     destination hoists the `lea` above the guard instead.
//   - the destination's `add ecx,8` at the TOP of the body with the stores at
//     [ecx-0xc] and [ecx-8]. Reached, together with `lea ecx,[esp+0xec]` and
//     the frame still at 0x3f58, by exactly one spelling: a biased `int*`,
//     `int* r = (int*)projected + 1; for (...) { r += 2; r[-3] = x; r[-2] = y; }`.
//     It scores 78.2% (458 bytes) because the `lea` is hoisted. Wrapping the
//     loop in an explicit `if (info->vertexCount > 0)` pushes the `lea` back
//     below the guard but MSVC then emits the guard test twice: 77.7%, 462
//     bytes. Both are the wrong trade, correct instructions at the wrong length.
//   - the source walked UNBIASED (`mov ebx,[eax]`, `mov ebp,[eax+8]`,
//     `add eax,0xc`, `mov ebp,[eax-8]`). NOT REACHABLE. `vertices[i]`, a walked
//     `Vertex* u` with `u++`, and a walked `int* u` with `u += 3` and
//     `u[0]/u[2]/u[1]` all get the +4 bias, and every walked-source form also
//     moves the frame to 0x3f4c with the arrays at 0x14 and 0xdc, twelve bytes
//     smaller. The MATCHed sibling 0x4581e0 spells its source as
//     `Vertex_4581e0* v = piece->vertices; ... v++` and does keep that walk
//     unbiased, so the bias here really is a tie and the 0x3f4c frame is the
//     price of every spelling that reaches it, not evidence that the original
//     did not walk the source.
//
// (2) THE FACE LOOP EVICTION IS AN ALLOCATION TIE, AND THIS PASS PROVED IT AND
//     FOUND THE MISSING PIECE. Adding ONE more live reference to `info` inside
//     the copy loop flips the eviction exactly as the original has it: bounding
//     the copy loop by `face->count && j < info->vertexCount` gives the counter
//     its home at [esp+0x10] AND drops the per-face reload of `info` to zero,
//     both of which only this file gets wrong. The extra compare survives, so
//     it is 478 bytes, seventeen over, and the frame moves to 0x3f5c. So the
//     lever the guide's "register priority" entry asks for is real and is a use
//     of `info` inside the copy loop that FOLDS AWAY. Nine candidates that add
//     a reference to `info` without adding an instruction are all byte-identical
//     to this file: `int& faceCount = info->faceCount` read by both the
//     pre-test and the latch; `PieceInfo_4584d0& inf = *info`; `int first =
//     info->firstFace` hoisted; `Face_4584d0* faces = info->faces` hoisted;
//     `(unsigned)i < (unsigned)info->faceCount`; `i = 0` written before the
//     firstFace branch (455 bytes, two under); the branch polarity swapped; `p =
//     face->indices` as its own statement; a null check on the index array.
//     Ten more that permute the copy loop's per-iteration pointer locals, which
//     the guide says decides which register holds the loop's end value, all
//     still put the index temp in ebx: the poly walk declared before the index
//     pointer (455 bytes), the index pointer initialised on its own, both walks
//     declared with the index pointer first (455 bytes), two separate
//     `poly[j].x`/`poly[j].y` stores (456 bytes, and BOTH `mov di` and `mov bx`
//     disappear, so the index temp is gone entirely), the value through a
//     temporary, `poly` walked as an `int*`, a `Face_4584d0* f = face` alias, `j`
//     zeroed before the loop, and the loop as a do/while (449 bytes, zero
//     reloads of `info`, but twelve under).
//
// (3) THE `static inline` PREDICATE IS INERT HERE, IN BOTH DIRECTIONS, WHICH IS
//     WORTH RECORDING BECAUSE THE BRIEF FLAGS THE RETURN TYPE AS DECISIVE.
//     `static inline int Less(int a, int b) { return a < b; }` and
//     `static inline int NotMinus1(int a) { return a != -1; }` around the
//     vertex loop test, the copy loop test, the face loop pre-test and the
//     firstFace test, each alone and all four together, are byte-identical to
//     this file (457 bytes, every probe fact unchanged). The same predicates
//     returning `bool` are NOT inert: all four together score 37.5% (520 bytes,
//     frame 0x3f5c, no `mov di` and no `mov bx` at all), because the branch is
//     materialised. So `int` is the right return type here and buys nothing; the
//     0x438ea0 and 0x487bf0 results do not transfer to this function.
//
// No suspected bug in the original. The face counter being spilled to the frame
// and reloaded around the copy loop is an allocation artefact of eight values
// competing for seven registers, and the `info->vertexCount` experiment above
// reproduces it exactly from the same body, which is the strongest evidence
// that the source is faithful.
//
// No `volatile` field is warranted here and none is used. `info` is a parameter,
// not a field, and its re-reads sit at post-loop and post-call points (0x458561
// after the vertex loop, 0x458582 before the face pre-test, 0x458686 at the
// latch), which a plain `info->faceCount` re-read already produces.

static inline int FaceCount(Face_4584d0* face) { return face->count; }

// FUNCTION: 0x4584d0
void Class_004584d0::DrawPiece(Model_4584d0* model, void* surface,
    Vec3_4584d0* camera, PieceInfo_4584d0* info, Vertex_4584d0* vertices,
    unsigned int palette, int useColor)
{
    void* pic;
    int i;
    int unit;
    Point_4584d0 projected[2000];
    Point_4584d0 poly[25];
    View_4584d0* view = model->view;
    struct Off_4584d0 { int a; int y; int b; };
    Off_4584d0 off;
    off.a = view->originX - camera->x;
    off.y = view->originY;
    off.b = view->originZ - camera->z;
    {
        for (i = 0; i < info->vertexCount; i++, vertices++) {
            projected[i].x = 0x80 + (short)((vertices->x + off.a) >> 16);
            projected[i].y = (0x20 + ((short)((off.b - vertices->z) >> 16)
                - ((short)((off.y + vertices->y) >> 16) >> 1)));
        }
    }
    Face_4584d0* face;
    if (info->firstFace != -1) {
        face = info->faces + 1;
        i = 1;
    } else {
        face = info->faces;
        i = 0;
    }
    if (i < info->faceCount) do {
        int j;
        unsigned short* p;
        j = 0, p = face->indices;
        if (FaceCount(face) > j) {
            while (1) {
                poly[j] = projected[*p];
                j++, p++;
                int count = face->count;
                if (j >= count)
                    break;
            }
        }
        Flags_4584d0 flags = face->flags;
        if (!flags.bits.a) {
            if (face->count != 4) goto skip0;
            if (flags.bits.b) {
                if (flags.bits.c) {
                    unit = *(int*)(((char*)g_game + 0x1b8a) + ((palette & 0xff) * 0x14b));
                    pic = GetGafFrame(face->color, *(unsigned char*)(unit + 0x96));
                } else pic = useColor ? GetGafFrame(face->color, 0) : GetGafSequenceFrame(&face->pic);
            } else pic = face->pic.pic;
            DrawFrameQuad(surface, pic, poly, 0);
skip0:;
        } else {
            FillPolygon(surface, poly, face->count, face->unknown_0);
        }
        i++, face++;
    } while (i < info->faceCount);
}