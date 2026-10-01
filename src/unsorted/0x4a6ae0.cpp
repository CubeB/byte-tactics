// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, space-bunny-free.
// Names are provisional.
//
// deepseek-v4.1-flash (timeboxed pass): no scored change, this file stays at
// 87.2% (1691 of 1703 bytes). Diff analysis: our field_138 region emits TWO
// memory compares `cmp word [ebp+0x138],0` while the original loads ONCE
// (`mov ax,[ebp+0x138]; test ax,ax`) and re-tests AX twice more. The CSE only
// happens when NO store to field_138 lies on the paths between the three
// tests, and the original's block order (test1/0x2000 at 0x4a6ef9, test2 at
// 0x4a6f43, RECT at 0x4a6f4c, tail+search at 0x4a6f8d, N at 0x4a7060, D at
// 0x4a7069) says the tail+search block is INSIDE the rect-in-branch, with N
// and D forming its else. Untested variants of exactly that shape are in
// build/scratch/0x4a6ae0/v1.cpp (goto tail into the in-rect branch, goto
// d_body into the else), v2.cpp (v1 with `int bound`), v3.cpp (d_body
// inlined before RECT): written but never scored because the timebox fired.
// The loop bound still wants `movsx eax,[edi+0xb6]; lea ebp,[eax+1]`
// (`int bound = entries->count + 1`) but every tried `int` form recolors the
// register homes (obj=EBX/entry=EBP lost) and drops to ~60%.
// Earlier pass by space-bunny-free, verified by GPT-6.1-sol.
// PARTIAL 87.2% (1691 bytes against the original's 1703). Command-button
// click/key handler for the 0x15b-byte entry table.
//
// space-bunny-free pass: the else-branch of the key handler had an INVERTED
// condition. It read `if (entry->field_138) { field_138 = 1; FUN_004a5f40;
// }`, but the original's 0x4a714b is `cmp word ptr [ebp+0x138],0` /
// `jne 0x4a7163`, i.e. the store and the FUN_004a5f40 only happen when
// field_138 is ALREADY 0. Fixing it to `if (entry->field_138 == 0)` is
// worth +0.2 (87.0 -> 87.2) and is a genuine semantic correction.
//
// A true-LCS alignment (build/scratch/0x4a6ae0/lcs.py, which is difflib-free
// and only ever costs a free --sym score) says every remaining difference is
// EITHER a jump target that moved because the function is 12 bytes short
// (all of them point at the shared `fail` epilogue) OR one of exactly two
// content differences:
//   1. the loop bound: original `movsx eax,word [edi+0xb6]; lea ebp,[eax+1]`
//      (2 ins, 7 bytes) against our `mov ax,...; inc ax; movsx ebp,ax`
//      (3 ins, 6 bytes), and
//   2. the field_138 region, where the original is 3 ins + 15 ins against our
//      18 ins. That is the whole of the missing 12 bytes.
// So nothing else in this function is wrong; the register homes (obj=EBX,
// entry=EBP, entries=EDI, flags=EDX/DH, team=CL) and every call site already
// match.
//
// Retried this pass, all worse (free scratch scores): the semantically correct
// `if (f138) { if (flags&0x2000) ... else if (inrect) fail; else zero+out; }
// else if (inrect) { set 1; DAT=0xf; } else fail;` chain, 57.1, confirms the
// note below; a `short f138 = entry->field_138;` cached across all three
// tests, 60.3 (spills, recolors the whole region again); `int bound =
// entries->count + 1;`, 60.4, which does reach `movsx` but allocates the bound
// to ECX and demotes team to BL and loses flags/DH; and putting
// `entries->count + 1` straight into the loop test with no named bound, 84.4.
//
// deepseek-v4.1-flash retry: materialising the search-loop bound as a
// single `short bound = entries->count + 1;` (declared right before the
// loop, used in `found >= bound`) lifts 84.4 -> 87.0. The bytes become
// `mov ax,[edi+0xb6]; inc ax; movsx ebp,ax` (original: `movsx eax,
// [edi+0xb6]; lea ebp,[eax+1]`). Every other way of naming the bound
// (`int` / `unsigned short` / `short n; int bound = n+1` / putting the
// +1 back in the condition) recolors the whole search block and drops to
// 60.4. The one-`short` local fits the existing frame (sub esp stays
// 0x28, same as the original) where an `int` local forces a reload.
//
// The field_138 region cannot be moved out of line without losing the
// obj=EBX / entry=EBP homes: every if/else form (clean else-if, nested
// if/else, the semantically correct `if (f138 == 0) A else B/D`) flips
// obj to EBP and entry to EBX and scores 57 to 60. The sequential-if
// form below is what keeps the homes, so it stays even though its A
// block is inline while the original jumps to it out of line.
//
// deepseek-v4.1 round 3 (2611): writing the `field_138 == 0` bailout after
// the field_138 rect-set block as `goto fail` instead of `return 0` lifts
// 84.2 -> 84.4 (the shared epilogue at the end of the function is now
// reached by a `je`, as the original's 0x4a7060 is). Two more tries this
// pass, both worse: `short f138 = entry->field_138;` used for both top
// tests spills the local (frame +4, 62.7), and materialising the loop
// bound as `short n = entries->count; int bound = n + 1;` recolors the
// search block (60.0).
//
// What finally moved the object/entry register home: writing the
// field_138 / flags-0x2000 region as sequential `if`s instead of an
// if/else-if/else chain (vO). The original tests field_138 twice, so it is
// a sequence of statements, not a chain:
//     if (entry->field_138) { A }
//     if (entry->field_138 == 0) return 0;
//     if (entry->flags & 0x2000) { B } else { C }
// That form also puts obj in EBX and entry in EBP, exactly as the original
// (the older chain form homed obj in EBP and left the search block reading
// entry->team as `mov bl,...` instead of the original's `mov cl,...`).
// An `int flags = entry->flags;` plus `unsigned char team = entry->team;`
// hoisted just before the search block is what the original has: it keeps
// flags in EDX (`test dh,0x18` then `test dh,0x10`) and team in CL across
// the loop, and the loop's no-match path is a plain `xor esi,esi` reached
// from the pre-test, which needs the `for(;;) { if (found >= count+1)
// { found = 0; break; } ... }` form.
//
// What still differs (all in the field_138 region, plus two register
// picks): the original loads the first test as `mov ax,[ebp+0x138];
// test ax,ax` and re-tests that same AX after the 0x2000 body; this file
// emits two memory compares and an extra `jmp` over its early `return 0`,
// so the 0x2000 body lands at the end of the function here (original:
// inline, right after the first test) and A sits before B. The loop bound
// is `movsx ebp,[...count]; inc ebp` here, the original has `movsx
// eax,[...count]; lea ebp,[eax+1]` (hoisting `int bound = count + 1` or
// naming it costs 24%: it recolors the whole search block).
//
// Free-scratch tries that did NOT help on top of the current form:
// if/else-if/else chains (vD 52.7, vE 42.2, vK 57.4), A moved after the
// B/C decision (vS 72.6), named loop bound (vQ2 60.0), while-with-&& loop
// (vA 50.0), flags/team locals with the old chain (vH 58.6), hoisted bound
// inside the loop (vQ 84.2 flat). Earlier 50.0%-era tries (entry/entries
// form swaps, point-coord locals, deferred entry, header sweep) are
// recorded in build/scratch/SHARED.md.
//
// deepseek-v4.1 round 2 (2497): the three field_138 tests all live in AX
// because no store dominates tests 2 and 3: 0x4a6f00 (je -> the rect-set
// block at 0x4a6f4c), 0x4a6f43 (`f138 != 0` guard whose false path falls
// INTO that rect-set block) and 0x4a7060 (`f138 == 0` -> fail, on the
// rect-set block's not-in-rect exit), whose fall-through is the D body at
// 0x4a7069. So the rect-set block sits out of line, after the 0x2000 body,
// and falls straight into the shared tail at 0x4a6f8d, and the D body is
// entered only from the 0x4a6f43 guard. A goto-forced transcription of that
// shape (labels tail/T3/D, `goto tail` out of the 0x2000 body, A reached by
// the guard's fall-through) compiles to the right block order but scores
// 62.7: it costs the obj=EBX/entry=EBP homes, so this file keeps the 84.2
// form below.
//
// deepseek-v4.1 round: the original's `mov ax,[ebp+0x138]/test ax,ax`
// followed later by a bare `test ax,ax` means the first test's block
// layout is: ax = field_138; if (ax == 0) -> the rect-check that sets
// field_138 = 1 and DAT_0051fbac = 0xf; else the flags-0x2000 test,
// whose else path re-tests ax and jumps to the D rect-check. Rewriting
// the source to match (if (f138 == 0) { rect, set, DAT } else { ...C/D })
// scores 58.7 with if/else and 81.6 with two sequential ifs, both below
// the current form, and caching field_138 in a short local (v3) spills
// it to the stack (frame +4, 62.7). Also: the loop bound really is
// `movsx eax,[edi+0xb6]; lea ebp,[eax+1]` (2 bytes more than our
// `movsx ebp,..; inc ebp`), and the original loads point.y and r.top
// into two registers before comparing in all four rect checks.
#pragma pack(push, 1)

struct Class_004a6ae0;

union Field136_004a6ae0 {              // 2 bytes at +0x136
    short value;
    struct { unsigned char lo; unsigned char hi; } bytes;
};

struct Entry_004a6ae0 {                // 0x15b bytes
    unsigned char state;               // +0x00
    unsigned char team;                // +0x01
    char unknown_02[0x13 - 0x02];
    short x0;                          // +0x13
    short y0;                          // +0x15
    short x1;                          // +0x17
    short y1;                          // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x2f - 0x1f];
    unsigned short* field_2f;          // +0x2f
    char unknown_33[0xb6 - 0x33];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x136 - 0xb8];
    union Field136_004a6ae0 field_136; // +0x136
    short field_138;                   // +0x138
    char field_13a;                    // +0x13a
    char unknown_13b[0x13c - 0x13b];
    unsigned char field_13c;           // +0x13c
    char unknown_13d[0x140 - 0x13d];
    short field_140;                   // +0x140
    char unknown_142[0x144 - 0x142];
    void (__stdcall* callback)(Class_004a6ae0*, int);  // +0x144
    char unknown_148[0x14a - 0x148];
    int callbackArg;                   // +0x14a
    char unknown_14e[0x15b - 0x14e];
};

struct Holder_004a6ae0 {
    char unknown_00[4];
    Entry_004a6ae0* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14
};

struct Point_004a6ae0 {                // 24 bytes, copied with rep movsd
    int x;
    int y;
    int unknown_08[4];
};

struct Class_004a6ae0 {
    char unknown_00[0x18];
    Holder_004a6ae0* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a6ae0 point;              // +0x3c
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64
    int field_68;                      // +0x68
    char unknown_6c[0xcc6 - 0x6c];
    int field_cc6;                     // +0xcc6
    char unknown_cca[0xcce - 0xcca];
    int field_cce;                     // +0xcce
};
#pragma pack(pop)

extern int DAT_0051fbb0;
extern int DAT_0051fbac;

int __stdcall FUN_004ab510(Class_004a6ae0* obj, unsigned char buttons);
int __stdcall FUN_004ab5b0(Class_004a6ae0* obj, unsigned int mask);
void __stdcall FUN_004ab690(Class_004a6ae0* obj, int value);
int __stdcall FUN_0049fc50(Class_004a6ae0* obj, int index);
void __stdcall FUN_004a5f40(Class_004a6ae0* obj, int index);
void __stdcall FUN_004a0340(Class_004a6ae0* obj, int index);
void __stdcall FUN_004a2580(Class_004a6ae0* obj, int index);
void __stdcall FUN_004a2be0(Class_004a6ae0* obj, int index);
int FUN_004b6340();
int __stdcall FUN_004c1b80(int param);
void FUN_004c1ab0();
int __cdecl tolower(int c);
int __cdecl toupper(int c);

// FUNCTION: 0x4a6ae0
int __stdcall FUN_004a6ae0(Class_004a6ae0* obj, int index, int param_3)
{
    Entry_004a6ae0* entries = obj->holder->entries;
    Entry_004a6ae0* entry = &entries[index];
    if (entry->field_13c & 1)
        goto fail;

    struct Rect { int left, top, right, bottom; } r;
    if (entry->state == 0) {
        r.left = 0;
        r.top = 0;
    } else {
        r.left = entry->x0;
        r.top = entry->y0;
    }
    r.right = entry->x1 + r.left - 1;
    r.bottom = entry->y1 + r.top - 1;

    Point_004a6ae0 point = obj->point;
    point.x -= entries->x0;
    point.y -= entries->y0;

    if (point.x >= r.left && point.x <= r.right
        && point.y >= r.top && point.y <= r.bottom) {
        obj->field_68 = index;
        if (FUN_004ab510(obj, 1)) {
            obj->focus = -1;
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 1);
            obj->field_cce = entry->field_138;
        } else if (FUN_004ab510(obj, 2)) {
            obj->focus = -1;
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 2);
            obj->field_cce = entry->field_138;
        }
    }

    if (obj->focus == index) {
        if (entry->flags & 0x10) {
            if (!FUN_004ab5b0(obj, 3))
                goto fail;
            obj->focus = -1;
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom) {
                entry->field_138 = obj->field_cce;
                FUN_004a5f40(obj, index);
                return 0;
            }
            entry->field_138 = 1;
            FUN_004a0340(obj, index);
            FUN_004a5f40(obj, index);
            return 1;
        }
        if (entry->flags & 0x40) {
            if (!FUN_004ab5b0(obj, 3)) {
                obj->focus = -1;
                if (point.x < r.left || point.x > r.right
                    || point.y < r.top || point.y > r.bottom) {
                    entry->field_138 = obj->field_cce;
                    FUN_004a5f40(obj, index);
                    return 0;
                }
                entry->field_138 = (obj->field_cce == 0);
                FUN_004a0340(obj, index);
                FUN_004a5f40(obj, index);
                return 1;
            }
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom) {
                if (entry->field_138 == 0)
                    goto fail;
                entry->field_138 = 0;
                FUN_004a5f40(obj, index);
                return 0;
            }
            if (entry->field_138 != 0)
                goto fail;
            entry->field_138 = 1;
            FUN_004a5f40(obj, index);
            return 0;
        }
        if (entry->flags & 8) {
            if (FUN_004ab5b0(obj, 3))
                goto fail;
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom)
                goto fail;
            if (entry->field_138 == 1)
                entry->field_138 = 0;
            else if (entry->field_138 == 0)
                entry->field_138 = 1;
            FUN_004a0340(obj, index);
            FUN_004a5f40(obj, index);
            obj->focus = -1;
            return 1;
        }
        if (entry->flags & 0x100) {
            if (!FUN_004ab510(obj, 1))
                goto fail;
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom)
                goto fail;
            unsigned short* p = entry->field_2f;
            if (p != 0) {
                if (entry->field_138 < *p - 1)
                    entry->field_138 += 1;
                else
                    entry->field_138 = 0;
            }
            FUN_004a0340(obj, index);
            FUN_004a5f40(obj, index);
            obj->focus = -1;
            return 1;
        }
        if (!FUN_004ab5b0(obj, 3)) {
            obj->focus = -1;
            entry->field_138 = 0;
            FUN_004a0340(obj, index);
            if (point.x >= r.left && point.x <= r.right
                && point.y >= r.top && point.y <= r.bottom
                && !(entry->flags & 0x1800)) {
                if (entry->field_136.bytes.lo != 0) {
                    entry->field_136.bytes.hi += 1;
                    if (entry->field_136.bytes.hi >= entry->field_136.bytes.lo)
                        entry->field_136.bytes.hi = 0;
                }
                FUN_004a5f40(obj, index);
                return 1;
            }
            FUN_004a5f40(obj, index);
            return 0;
        }
        if (entry->field_138) {
            if (!(point.x >= r.left && point.x <= r.right
                  && point.y >= r.top && point.y <= r.bottom))
                goto fail;
            entry->field_138 = 1;
            DAT_0051fbac = 0xf;
        }
        if (entry->field_138 == 0) {
            goto fail;
        }
        if (entry->flags & 0x2000) {
            if (DAT_0051fbb0 == FUN_004b6340())
                goto fail;
            DAT_0051fbb0 = FUN_004b6340();
            if (DAT_0051fbac > 0) {
                DAT_0051fbac -= 1;
                return 0;
            }
        } else {
            if (point.x >= r.left && point.x <= r.right
                && point.y >= r.top && point.y <= r.bottom)
                goto fail;
            entry->field_138 = 0;
            FUN_004a5f40(obj, index);
            return 0;
        }
        FUN_004a5f40(obj, index);
        int flags = entry->flags;
        if (!(flags & 0x1800))
            goto fail;
        unsigned char team = entry->team;
        int found = 1;
        short bound = entries->count + 1;
        for (;;) {
            if (found >= bound) {
                found = 0;
                break;
            }
            if (entries[found].state == 4 && entries[found].team == team)
                break;
            found++;
        }
        if (found == -1)
            goto fail;
        Entry_004a6ae0* f = &entries[found];
        short off = f->field_140;
        if (flags & 0x1000) {
            if (off > 0)
                f->field_140 = off - 1;
        } else {
            if (off < f->field_136.value - 1)
                f->field_140 = off + 1;
        }
        if (obj->holder)
            obj->holder->field_14 = 1;
        FUN_004a2580(obj, found);
        FUN_004a2be0(obj, found);
        if (f->callback)
            f->callback(obj, f->callbackArg);
        return 0;
    } else {
        if (obj->focus != -1 && entries[obj->focus].state == 3) {
            if (!FUN_004c1b80(0xfb))
                goto fail;
        }
        if (obj->field_cc6 != 1)
            goto fail;
        if (param_3 == 0)
            goto fail;
        if ((char)tolower(entry->field_13a) != (char)param_3
            && (char)toupper(entry->field_13a) != (char)param_3)
            goto fail;
        if (entry->flags & 0x40) {
            entry->field_138 = (entry->field_138 == 0);
            FUN_004a5f40(obj, index);
        } else if (entry->flags & 0x10) {
            if (entry->field_138 == 0) {
                entry->field_138 = 1;
                FUN_004a5f40(obj, index);
            }
        }
        FUN_004a0340(obj, index);
        FUN_004c1ab0();
        return 1;
    }
fail:
    return 0;
}
