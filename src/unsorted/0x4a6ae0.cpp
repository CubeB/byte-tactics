// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, space-bunny-free, finished by Sonnet 5.5. Names are provisional.
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
//  (`movsx eax,[edi+0xb6]; lea ebp,[eax+1]`; ours `mov ax; inc ax; movsx ebp,ax`
//  with `short bound`; `int bound` recolors everything, see above).
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