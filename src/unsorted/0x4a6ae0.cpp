// Decompiled by deepseek-v4.1. Names are provisional.
// Prior attempt by space-bunny-free, verified by GPT-6.1-sol.
// PARTIAL 84.2% (1695 bytes against the original's 1703). Command-button
// click/key handler for the 0x15b-byte entry table.
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
            return 0;
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
        for (;;) {
            if (found >= entries->count + 1) {
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
            if (entry->field_138) {
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
