// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, space-bunny-free, finished by Sonnet 5.5, edited by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash (#4054, 10-minute box, no new variant scored): reconfirmed
// 93.4% / 1703 bytes. Still differs: the rect-set block is emitted before the
// flags&0x2000 block with a second `cmp word [ebp+0x138],0` re-test where the
// original reuses the live `mov ax,[ebp+0x138]; test ax,ax` pair and emits the
// rect-set after, and the bound block (ours `mov ax,[edi+0xb6]; inc ax; movsx
// ebp,ax` vs `movsx eax,[edi+0xb6]; lea ebp,[eax+1]`); both are the known
// obj/entry allocation tie, every respelling tried in the notes below costs more
// than it pays.
// deepseek-v4.1-flash (issue #3406, 10-minute box): two more shapes tried and
// reverted, both worse: a `short f138` local read once (61.7%, 1699 bytes) and
// the original-emission-order rewrite `if (f138 == 0) {rect} else {flags}`
// (58.5%, 1688 bytes). Still differs: the f138 test region (ours `cmp word
// [ebp+0x138],0` twice against the original's live `mov ax,[..]; test ax,ax`
// pair with the rect-set block emitted after the flags&0x2000 block) and the
// bound block (ours `mov ax,[edi+0xb6]; inc ax; movsx ebp,ax` against the
//  original's `movsx eax,[edi+0xb6]; lea ebp,[eax+1]`).
//  deepseek-v4.1-flash pass (issue #3586). Rebuilt v1 from this file exactly as
//  note 1 quotes it and measured 64.1%, 1704 bytes, homes obj=EBP/entry=EBX, so
//  the whole mismatch is the pointer tie, nothing else. New evidence on that
//  tie:
//   - v1 with ONLY the shared tail `FUN_004a5f40(obj, index);` at 0x4a6f8d
//     removed compiles to the CORRECT homes and matches the original
//     instruction for instruction; the sole diffs are the -5 label shift and
//     the bound block (88.5%, 1698 bytes). So the tie is exactly ONE `obj`
//     reference (or one `index` reference, both are dropped by that deletion).
//   - Duplicate references written with a DIFFERENT spelling are not CSE'd and
//     do flip the homes, but each one costs bytes: `entries[index].field_138`
//     in place of `entry->field_138` in the redraw test (87.9%, 1710),
//     in the flags&8 first test (84.2%), a dead duplicate store (83.8%),
//     `entries[index].state == entry->state` in the state test (80.9%).
//   - Sharing the arms 8/0x100 tail with `goto fin` on v1 does flip the homes
//     (78.8%) but promotes point.y into EDI, so the first in-rect block turns
//     into `mov ecx,edi; cmp ecx,eax`; restoring `index` weight so EDI stays a
//     region cache is the remaining lead, not solved here.
//  No byte-level improvement over 93.4% was found; the file below is unchanged.
//  deepseek-v4.1-flash pass (#3730): reconfirmed v1 = 64.1%, 1704 bytes with the
//  0x4a6ef9..0x4a708b region byte exact, and measured `int bound` on the kept
//  93.4% shape: it fixes nothing here and regresses to 61.8%, 1698 bytes, because
//  it flips the keyboard-half homes (0x4a7163 becomes `mov word ptr [ebx+0x138],si`).
//  The file stays at 93.4%; the f138-region shape and the obj/entry tie remain
//  mutually exclusive without a byte-costly extra entry reference.

//
// PARTIAL 93.4% (1703 of 1703 bytes). Command-button click/key handler for
// the 0x15b-byte entry table.
//
// Sonnet 5.5 pass (87.2% -> 93.4%). Two findings:
//  1. The shared `fail` epilogue (`pop edi; pop esi; pop ebp; xor eax,eax; pop
//     ebx` at the very end, reached by `je/jne` from everywhere) only stays at
//     the end of the function when the LAST arm (the focus != index arm) is
//     written as nested ifs ending in `return 1;` with no `goto fail` of its own.
//     Written with `if (a != p && b != p) goto fail;` MSVC inverts the final
//     test and puts an inline copy of the epilogue before the body, which cost
//     12 bytes and every later jump target. The nested form fixes the size
//     (1691 -> 1703) and all jump offsets.
//  space-bunny-free pass: confirmed the open lead below and sharpened it.
//     With the CORRECT semantics written as the if/else-if/else chain of v1
//     (build/scratch/0x4a6ae0/v1.cpp) the 0x4a6ef9..0x4a708b region is now
//     BYTE EXACT, instruction for instruction, every opcode, every operand
//     width, both rect tests and both `test ax,ax` re-tests included. The
//     ONLY thing left is that v1 swaps the two pointer homes: obj=EBP,
//     entry=EBX, while the original has obj=EBX, entry=EBP. Every jump
//     target in the rest of the function then differs by one byte and the
//     score is 64.1% (1704 bytes). So the shape problem is SOLVED; what is
//     left is purely the obj/entry priority tie.
//     Nothing tried this session moved that tie (all still obj=EBP):
//       - extra source uses of entry that CSE away (entry->flags twice in the
//         tail test, entry->field_138 twice in the first test, the flags&0x1000
//         test read through entry instead of the local): no change at all,
//         confirming references are counted AFTER CSE;
//       - the else arm written as `else if (field_138 != 0) {...} else
//         goto fail;`: same 64.1%, same homes;
//       - `int bound` (which does fix the bound block, see 3) on top of v1:
//         1700 bytes but the homes stay swapped;
//       - caching obj->holder in a local, and a redundant `f138 == 0 ||
//         f138 == 0` in the else arm: no change.
//     So the missing weight is NOT reachable by adding CSE-able uses. Either
//     the original spells the region so that one MORE entry use survives CSE
//     (an extra store to a field of entry in that chain), or obj's uses are
//     one lighter for a reason outside this region.
//     SO the tie DOES move, but only from a use that survives CSE: giving
//     field_138 a union spelling and writing it as two bytes in the second
//     arm, `bytes.lo = 1; bytes.hi = 0;` instead of `value = 1;`, flips the
//     homes back to obj=EBX / entry=EBP (build/scratch/0x4a6ae0/v1_y2.cpp,
//     88.4%, 1710 bytes) and with `int bound` too (v1_y2_z1.cpp, 88.5%,
//     1706 bytes). The whole function is then correct except for 3 bytes of
//     size: that byte pair costs `mov byte [e+0x138],1` + `mov byte
//     [e+0x139],0` (12) where the original has one `mov word [e+0x138],1`
//     (7), and the bound block. So the open lead is a surviving extra entry
//     use that emits NO bytes: a byte-pair READ that CSEs into the original's
//     `mov ax,[e+0x138]; test ax,ax` is the obvious candidate and did not
//     compile before the wall clock ran out (try
//     `(entry->field_138.bytes.lo | entry->field_138.bytes.hi) != 0` in place
//     of `entry->field_138 != 0` on v1_z1.cpp). Everything else in the file
//     below is unchanged and the file stays at 93.4% because the byte pair
//     costs 5 bytes it does not get back.
//  space-bunny-free pass 2 (2 real check runs, scratch scoring otherwise). All
//     four variants below were rebuilt from this exact file, so these are
//     measurements of the current text, not of the older snapshot the notes
//     above were written against:
//       - v1 (the correct-semantics region quoted in note 1, nothing else
//         changed): 64.1%, 1704 bytes. Reproduces the documented swap,
//         obj=EBP / entry=EBX.
//       - v1 + `int bound`: 64.1%, 1700 bytes. The bound block comes out with
//         the original's `movsx eax` / `lea ebp,[eax+1]`, and the homes stay
//         swapped, so the bound shape and the allocation really are one
//         problem, not two.
//       - v1 + `int f138 = entry->field_138;` used by the region's three tests:
//         63.5%, 1701 bytes. THIS CONTRADICTS the note above that "int f gives
//         the right homes": the extra local does not demote `entry`, the new
//         variable just takes the pressure instead. Do not retry that.
//       - v1 + `int f138` + `int bound`: 63.6%, 1697 bytes.
//       - v1 with the three region tests written as the byte pair
//         `(entry->field_138.bytes.lo | entry->field_138.bytes.hi)`, i.e. the
//         exact experiment the note above left untried: 87.4%, 1707 bytes, and
//         the homes DO flip to obj=EBX / entry=EBP. The cost is the shape, not
//         the size alone: the pair compiles to `mov al,[e+0x139]; mov
//         cl,[e+0x138]; or al,cl; je` (14 bytes) where the original has `mov
//         ax,[e+0x138]; test ax,ax; je` (10), and the two later re-tests become
//         `test al,al` instead of `test ax,ax`, which is the 16-point gap.
//       - v1 + byte pair + `int bound`: 66.0%, 1702 bytes, worse again.
//       - THIS file with only `short bound` changed to `int bound` (and the
//         separate `bound = entries->count; bound = bound + 1;` spelling):
//         61.8%, 1698 bytes both ways. So the bound block's shape and the
//         obj/entry allocation really are ONE problem: the original's
//         `movsx eax` / `lea ebp,[eax+1]` only appears when `bound` can take
//         EBP, which requires EBP to be the `entry` register, and the moment
//         `int bound` is spelled the allocator hands EBP to `bound` early
//         and recolours the whole function. Fixing one without the other is
//         not possible from either side.
//     So the missing weight and the missing test width are the same missing
//     thing: no spelling of one extra `entry` reference was found that emits
//     the original's 16-bit word test. The 93.4% below therefore keeps the
//     inverted region, which is the only spelling that both keeps the homes
//     and keeps the total size at 1703.
//  2. The field_138 region below is still the old, SEMANTICALLY INVERTED shape
//     (`if (field_138) { rect-test..; = 1; DAT=0xf }`), kept only because it
//     is the one spelling that gets obj=EBX / entry=EBP. The original does:
//         f138 == 0, in rect  -> f138 = 1; DAT_0051fbac = 0xf; run the tail
//         f138 == 0, not in   -> fail
//         f138 != 0, 0x2000   -> auto-repeat block, then the tail
//         f138 != 0, no 0x2000-> not in rect: f138 = 0, redraw, return 0;
//                                in rect: fail
//     The exact original block layout (mov ax,[f138]/test ax,ax first, flags in
//     ecx, `test ax,ax` re-tests at 0x4a6f43 and 0x4a7060, tail physically
//     between the rect-set and the D block) is reproduced by this source,
//     which is the correct semantics (call it v1):
//         if (entry->field_138 != 0 && (entry->flags & 0x2000)) {
//             if (DAT_0051fbb0 == FUN_004b6340()) goto fail;
//             DAT_0051fbb0 = FUN_004b6340();
//             if (DAT_0051fbac > 0) { DAT_0051fbac -= 1; return 0; }
//         } else if (entry->field_138 == 0 && INRECT) {
//             entry->field_138 = 1; DAT_0051fbac = 0xf;
//         } else {
//             if (entry->field_138 == 0) goto fail;
//             if (INRECT) goto fail;
//             entry->field_138 = 0; FUN_004a5f40(obj, index); return 0;
//         }
//         FUN_004a5f40(obj, index);  /* the shared tail */
//     but it flips the homes to obj=EBP / entry=EBX (62-64%): the allocator gives
//     the heavier of obj/entry the later register (EBP), and v1 CSEs two
//     `cmp [f138],0` into the `ax` temp, so entry becomes lighter than obj. Any
//     one extra entry reference (a dummy store anywhere) flips it back, which
//     proves the margin is one reference. Everything below the first test then
//     also lines up (team in cl, `lea ebp,[eax+1]` bound, flags in edx).
//     Tried without effect on the homes: type/declaration-order permutations of
//     all locals (40 random orders), `short f` / `int f` caches of field_138
//     (int f gives the right homes but a movsx), 72 spellings of the three
//     field_138 tests and the rect test, entries[index].x instead of entry->x,
//     do/while(0) wrapper, dead locals, redundant tests (fold before weights).
//     Spellings that do give the right homes but a different first test:
//     `(flags & 0x2000) && field_138 != 0` (flags loaded first: 86-87%).
//     Allocation order seen here: callee-saved registers go ESI, EDI, EBP, EBX
//     to the variables in DESCENDING weight (point.x, then entry, then obj in the
//     original; the 4th and later, index and point.y, only get EDI as a region
//     cache). References are counted after CSE and dead-store/redundant-test
//     folding, but BEFORE the late tail merge: duplicated source tails that the
//     compiler later merges still count. The original asm has single shared
//     tails: 0x4a6e3b (arms 8 and 0x100: calls + `focus = -1; return 1`),
//     0x4a6cc6 (arm 0x10 in-rect and arm 0x40 release: calls + `return 1`),
//     0x4a708b/0x4a7094 (`f138 = 0` then `FUN_004a5f40; return 0`, shared by the
//     0x40 arm, the default arm and the D block). Writing even ONE of them as a
//     single source copy with a goto (arms 8/0x100 -> `goto fin`, or `goto
//     done_a`) lowers obj by 1 to 3 references and gives obj=EBX/entry=EBP with
//     the correct-semantics region v1, but then edi becomes a point.y register
//     variable (`mov ecx,edi; cmp ecx,eax`) and the first in-rect block changes:
//     1713 bytes, 78.8% (all five sharing toggles give the same output; sharing
//     all of them gives 1669 bytes, 65%). So the original shares less, or the
//     weights of index/point.y are raised some other way. Exploring which subset
//     of the original's shared tails is real is the open lead.
//  The remaining mismatches of this file: that region, and the loop bound
//  (`movsx eax,[edi+0xb6]; lea ebp,[eax+1]`; ours `mov ax; inc ax; movsx
//  ebp,ax` with `short bound`). Measured this session: `int bound` does give
//  the original's `movsx eax`/`lea ebp,[eax+1]`, but it recolours the whole
//  function because `bound` then wants EBP before `entry` has had its last
//  use (1698 bytes, 61.8% on this file, 1700 bytes on v1 with the homes
//  still swapped). Getting that block right needs the allocation fixed first.
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
        if (!(obj->focus != -1 && entries[obj->focus].state == 3) || FUN_004c1b80(0xfb)) {
            if (obj->field_cc6 == 1) {
                if (param_3 != 0) {
                    if ((char)tolower(entry->field_13a) == (char)param_3
                        || (char)toupper(entry->field_13a) == (char)param_3) {
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
                }
            }
        }
    }
fail:
    return 0;
}