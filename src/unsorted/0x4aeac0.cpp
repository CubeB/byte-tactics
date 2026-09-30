// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash,
// finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
//
// 75.3 %. Only the loop's bottom block differs, and one register in case 6
// follows from it. Everything else is byte-identical.
//
// Second pass (space-bunny-free) added: headers.py swept all 128 header sets
// again, best is still 75.3 % (<windows.h> and <string.h> tie). The diff was
// read instruction by instruction: OUR build emits a private copy of the latch
// at the end of every switch case, and those copies are then tail-merged with
// each other (some copies lose the `add ebp,0x15b` because they jump into the
// copy that has it). So ours is 880 bytes against the original's 732, and the
// +148 is exactly that duplication. The `xor ebx,ebx` in the latch is not a
// source statement in either build: ebx is MSVC's chosen zero register, and it
// is re-materialised at the loop join point because liveness is imprecise
// across the switch's indirect jump.
//
// The original is a top-tested loop with ONE shared latch at 0x4aed26:
//   0x4aeb10  mov [esp+0x1c], esi     ; i = 0
//   0x4aeb14  lea ebp, [eax+0xd6]    ; induction variable = obj + 0xd6
//   0x4aeb1a  lea ecx, [esp+0x10]    ; LOOP HEAD
//   0x4aeb1e  call 0x4c3e10          ; FUN_004c3e10 (reset)
//   0x4aeb23  push esi
//   0x4aeb24  lea ecx, [esp+0x14]
//   0x4aeb28  call 0x4c3490          ; FUN_004c3490(i)
//   0x4aeb2d  test eax, eax
//   0x4aeb2f  je 0x4aed3c            ; exit
//   ... switch, every case jmps 0x4aed26 ...
//   0x4aed26  mov esi, [esp+0x1c]    ; SHARED LATCH
//   0x4aed2a  add ebp, 0x15b
//   0x4aed30  inc esi
//   0x4aed31  xor ebx, ebx
//   0x4aed33  mov [esp+0x1c], esi
//   0x4aed37  jmp 0x4aeb1a
//
// The top of our loop matches the original exactly. What still differs: MSVC 5
// copies the latch (inc esi / add ebp / xor ebx) into every switch case instead
// of jumping to one shared block. The `while (1) { ...; if (!find) break; ... }`
// form below is the only shape found that keeps the test at the top; every `for`
// form (`for (i = 0; reset(), find(i); i++)`, `for (;;)` plus break, a
// `do { } while (1)`, and a goto-built loop) is rotated by /O2, which duplicates
// the reset/find guard into the latch instead.
//
// Tried and still duplicated the latch: an explicit `goto` to a shared label in
// every case; per-case increments plus a default; `continue` in every case with
// the increment in a for header; wrapping the condition in a `static inline`
// helper returning 0/1 (that alone does keep the test at top); an inline helper
// for the increment; a named `int def = 0` default; an explicit `case 9:` and
// `default:`; a re-derived element pointer at the latch; replacing the latch's
// `i++` with `i = i + 1`. None changed the duplication.
//
// Case 6 is one register off as a consequence: the original keeps the old
// hotornot in ecx AFTER the call (`mov ecx,[ebp-0xe]; xor eax,ecx; and eax,1;
// xor eax,ecx`); ours loads it into esi before the call. All three spellings
// tried (a local t, one expression, a separate `old` local after the call) put
// it in esi or edx. Likely snaps once the loop allocation matches.
//
// Everything else is byte-identical: the 0x114 frame and local order, the
// induction variable folded to obj+0xd6 with stride 0x15b, the 11-entry jump
// table on e->type (case 9 empty, default -> latch), the maxchars clamp, the
// tail merge of the "text" read that cases 3 and 4 share, the inlined strcpy,
// and the exit's (short)(i-1) store to obj+0xb6.
//
// Rechecked by deepseek-v4.1-flash: headers.py swept all 128 header sets and
// none changes it. An explicit `Elem* e` induction pointer, a `do/while(1)`
// (rotates), a goto-built loop, a named `def = 0` default (compiles
// byte-identically) and `for (i=0;;i++)` (rotates) all leave the same tail
// duplication. An if/else-if chain instead of the switch scores 75.4% but drops
// the original 11-entry jump table, so it can never match; the switch version
// below is the faithful one. The remaining fix is stopping MSVC from
// tail-duplicating the shared latch into every switch case.
//
// Third pass (deepseek-v4.1) measured how that duplication behaves, by
// compiling the loop under local /Fa listings and counting "add ebp, 0x15b":
//  - It is a /Ot ("favor fast code") pass. /Os removes it completely, but /Os
//    also switches the whole function to a frame pointer (ebp), an imul for
//    the induction and DIFFERENT scheduling, so /Os is not the original's flag
//    set; /O2 /Ob2 /MT /Os scores far worse. /G3, /G4, /G5, /G6 and the
//    msvc5-rtm toolchain all leave the duplication in place.
//  - The pass runs on the post-register-allocation code and duplicates a
//    target block of at most SIX instructions into every predecessor that ends
//    in an unconditional jmp. Seven instructions stops it: adding any real
//    instruction to the latch ("ret = i;", "parser.field_0 = 0;", an extra
//    call) gives exactly one shared latch, but each of those adds visible
//    code, so none of them can match. The original's 0x4aed26 is six
//    instructions, and MSVC's own shared copy here (the $L600 block, which is
//    what case 10 falls into and what jump-table entry 9 points at) is
//    instruction for instruction identical to it; only the nine extra copies
//    are wrong.
//  - Statements the optimizer drops before that pass cannot enlarge the block:
//    "i = i;", "i + 0", "i * 1", "i += 0", "(void)i;", "i;", "i | 0",
//    "i++, 0;", an unused label, an empty inline helper called from the latch,
//    "if (1) { i++; }", "do { i++; } while (0);", "switch (0) { default: i++; }"
//    and "i ? i++ : i++" either vanish or change the loop (the last one loses
//    the induction and drops to 279 lines).
//  - Loop shapes: only "while (1)" plus an internal "if (!find(i)) break;"
//    keeps the reset+find guard at the top. for (;;), for (i = 0; ; i++),
//    do { } while (1), while (reset(), find(i)) and
//    for (i = 0; reset(), find(i); i++) are all rotated by /O2: the guard
//    moves to the bottom, an entry test is peeled, and the induction pointer
//    becomes an imul. So the source below is right and the duplication is the
//    only remaining difference.
//  - A scan of the whole exe for latches with five or more predecessor jumps
//    finds exactly this one function, so MSVC 5 almost always duplicates;
//    this function is the exception, which points at compiler state or at an
//    instruction that existed at the duplication pass and was deleted later
//    (a redundant move, load or store), not at the source shape.
//  - Preceding dummy functions in the same TU (small, large, or with the same
//    loop) change nothing either.
// Fourth pass (deepseek-v4.1): the do-while form
//   do { reset(); if (!find(i)) break; ...body...; } while (i++, 1);
// keeps ONE shared latch (all case breaks jump to 0x4aed2a, no copies:
// 756 bytes total vs our 880), because MSVC only duplicates into
// predecessors ending in an UNCONDITIONAL jmp and here the back-edge is the
// loop condition. But /O2 rotates that loop (guard moves to the bottom, an
// entry test is peeled), which is a different layout from the original, and
// scores 74.6%. The original has the guard at the top AND a shared latch,
// so the source shape must be a top-tested loop whose bottom is still not
// an unconditional-jmp predecessor of the latch; not found within the
// timebox. Scratch variant kept at build/scratch/0x4aeac0/v1.cpp.
//
// Fifth pass (deepseek-v4.1-flash): tested the tail-MERGE hypothesis, that the
// original writes `i++` at the end of every case and MSVC merges the identical
// tails into one shared latch rather than duplicating it. Writing `i++` in all
// ten cases plus an explicit `default: i++; break;` and no shared increment
// gives 860 bytes and 36.2% (the whole allocation scrambles; the switch layout
// is not preserved), so the shared post-switch `i++` remains the best form at
// 75.3%. The only remaining difference is still the 8 copied latches.
#include <string.h>

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3e10 {
public:
    void FUN_004c3e10();
};

class Class_004c3e20 {
public:
    void* FUN_004c3e20();
};

class Class_004c3e30 {
public:
    void FUN_004c3e30(void* p);
};

class Class_004c3240 {
public:
    void FUN_004c3240();
};

class Class_004c3490 {
public:
    int FUN_004c3490(int index);
};

class Class_004c2ea0 {
public:
    void* field_0;
    Class_004c46c0* current;            // +0x4
    void* field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

extern char DAT_005119b8[];

char* __stdcall FUN_004baff0(char* name, char* out, const char* ext);
char* __stdcall FUN_004c5740(char* text);

#pragma pack(push, 1)
struct Sub2_004aeac0 {
    char pad0[0xce - 0xb6];
    int field_ce;                      // +0xce
    char pad1[0xd6 - 0xce - 4];
    int field_d6;                      // +0xd6
    short itemheight;                  // +0xda
    char pad2[0x136 - 0xdc];
};

struct Sub6_004aeac0 {
    char pad0[0xc8 - 0xb6];
    unsigned int hotornot;             // +0xc8
    char pad1[0x136 - 0xcc];
};

union Body_004aeac0 {
    char text[0x80];                   // +0xb6
    int nuttin;                        // +0xb6
    short total;                       // +0xb6
    Sub2_004aeac0 s2;
    Sub6_004aeac0 s6;
};

struct Sub34_004aeac0 {
    short range;                       // +0x136
    short maxchars;                    // +0x138
    char pad0[0x13c - 0x13a];
    int thick;                         // +0x13c
    short knobpos;                     // +0x140
    short knobsize;                    // +0x142
    int field_144;                     // +0x144
    char pad1[0x15b - 0x148];
};

struct Sub5_004aeac0 {
    char link[0x11];                   // +0x136
    char field_147;                    // +0x147
    char pad0[0x15b - 0x148];
};

union Tail_004aeac0 {
    Sub34_004aeac0 s34;
    Sub5_004aeac0 s5;
};

struct Elem_004aeac0 {
    unsigned char type;                // +0x000
    char pad0[0xb6 - 0x001];
    Body_004aeac0 body;                // +0xb6
    Tail_004aeac0 tail;                // +0x136
};
#pragma pack(pop)

void __stdcall FUN_004ad350(Elem_004aeac0* obj, Class_004c2ea0* tree);
void __stdcall FUN_004ad890(Elem_004aeac0* obj, Class_004c2ea0* tree);
void __stdcall FUN_004adc70(Elem_004aeac0* obj, Class_004c2ea0* tree);

// FUNCTION: 0x4aeac0
int __stdcall FUN_004aeac0(Elem_004aeac0* obj, char* name)
{
    Class_004c2ea0 parser;
    int i;
    int ret = 0;
    char path[256];
    FUN_004baff0(name, path, "GUI");
    if (((Class_004c2f60*)&parser)->FUN_004c2f60(path) == 1) {
        ret = 1;
        i = 0;
        while (1) {
            ((Class_004c3e10*)&parser)->FUN_004c3e10();
            if (!((Class_004c3490*)&parser)->FUN_004c3490(i))
                break;
            void* cur = ((Class_004c3e20*)&parser)->FUN_004c3e20();
            Elem_004aeac0* e = obj + i;
            FUN_004ad350(e, &parser);
            ((Class_004c3e30*)&parser)->FUN_004c3e30(cur);
            switch (e->type) {
            case 0:
                FUN_004ad890(e, &parser);
                break;
            case 1:
                FUN_004adc70(e, &parser);
                break;
            case 2:
                e->body.s2.field_ce = 0;
                e->body.s2.field_d6 = 0;
                e->body.s2.itemheight = (short)parser.current->FUN_004c46c0("itemheight", 0);
                break;
            case 3:
                e->tail.s34.maxchars = (short)parser.current->FUN_004c46c0("maxchars", 0);
                if (e->tail.s34.maxchars > 0x80)
                    e->tail.s34.maxchars = 0x80;
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "text", 0x80, DAT_005119b8);
                strcpy(e->body.text, FUN_004c5740(e->body.text));
                break;
            case 4:
                e->tail.s34.range = (short)parser.current->FUN_004c46c0("range", 0);
                e->tail.s34.thick = (short)parser.current->FUN_004c46c0("thick", 0);
                e->tail.s34.knobpos = (short)parser.current->FUN_004c46c0("knobpos", 0);
                e->tail.s34.knobsize = (short)parser.current->FUN_004c46c0("knobsize", 0);
                e->tail.s34.field_144 = 0;
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "text", 0x80, DAT_005119b8);
                strcpy(e->body.text, FUN_004c5740(e->body.text));
                break;
            case 5:
                e->tail.s5.link[0] = 0;
                e->tail.s5.field_147 = 0;
                memset(e->body.text, 0, sizeof(e->body.text));
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "text", 0x80, DAT_005119b8);
                strncpy(e->body.text, FUN_004c5740(e->body.text), 0x7f);
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->tail.s5.link, "link", 0x10, DAT_005119b8);
                break;
            case 6: {
                unsigned int t = parser.current->FUN_004c46c0("hotornot", 0) ^ e->body.s6.hotornot;
                e->body.s6.hotornot = (t & 1) ^ e->body.s6.hotornot;
                }
                break;
            case 7:
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "filename", 0x20, DAT_005119b8);
                break;
            case 8:
                ((Class_004c48c0*)parser.current)->FUN_004c48c0(e->body.text, "filename", 0x20, DAT_005119b8);
                break;
            case 10:
                e->body.nuttin = parser.current->FUN_004c46c0("nuttin", 0);
                break;
            }
            i++;
        }
        obj->body.total = (short)(i - 1);
        ((Class_004c3240*)&parser)->FUN_004c3240();
    }
    return ret;
}
