// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by
// deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by GPT-6.1-sol. Names are provisional.
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

void* __stdcall FUN_004b7ee0(Pic_4584d0* ref);
void* __stdcall FUN_004b7f30(unsigned short* table, int index);
void __stdcall FUN_004c0310(void* surface, Point_4584d0* points, int count, int flags);
void __stdcall FUN_004c7580(void* surface, void* pic, Point_4584d0* points, void* src);

class Class_004584d0 {
public:
    void FUN_004584d0(Model_4584d0* model, void* surface, Vec3_4584d0* camera,
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

// FUNCTION: 0x4584d0
void Class_004584d0::FUN_004584d0(Model_4584d0* model, void* surface,
    Vec3_4584d0* camera, PieceInfo_4584d0* info, Vertex_4584d0* vertices,
    unsigned int palette, int useColor)
{
    Point_4584d0 projected[2000];
    Point_4584d0 poly[25];
    int i;
    View_4584d0* view = model->view;
    struct Off_4584d0 { int a; int b; int y; int c; };
    Off_4584d0 off;
    off.a = view->originX - camera->x;
    off.y = view->originY;
    off.b = view->originZ - camera->z;
    {
        Point_4584d0* q = projected;
        for (i = 0; i < info->vertexCount; i++, q++) {
            q->x = (short)((vertices[i].x + off.a) >> 16) + 0x80;
            q->y = (short)((off.b - vertices[i].z) >> 16)
                - ((short)((vertices[i].y + off.y) >> 16) >> 1) + 0x20;
        }
    }
    Face_4584d0* face = info->faces;
    if (info->firstFace != -1) {
        face++;
        i = 1;
    } else {
        i = 0;
    }
    for (; i < info->faceCount; i++, face++) {
        int j;
        unsigned short* p;
        for (j = 0, p = face->indices; j < face->count; j++, p++)
            poly[j] = projected[*p];
        Flags_4584d0 flags = face->flags;
        if (!flags.bits.a) {
            if (face->count == 4) {
                void* pic;
                if (flags.bits.b) {
                    if (flags.bits.c) {
                        int player = palette & 0xff;
                        int unit = *(int*)((char*)g_game + 0x1b8a + player * 0x14b);
                        pic = FUN_004b7f30(face->color, *(unsigned char*)(unit + 0x96));
                    } else if (useColor) {
                        pic = FUN_004b7f30(face->color, 0);
                    } else {
                        pic = FUN_004b7ee0(&face->pic);
                    }
                } else {
                    pic = face->pic.pic;
                }
                FUN_004c7580(surface, pic, poly, 0);
            }
        } else {
            FUN_004c0310(surface, poly, face->count, face->unknown_0);
        }


    }
}
