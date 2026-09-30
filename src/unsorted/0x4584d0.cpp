// Decompiled by GPT-6-Luna, finished by Space Bunny Free, finished by
// deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
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
        unsigned short* idx = face->indices;
        for (j = 0; j < face->count; j++)
            poly[j] = projected[*idx++];
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
