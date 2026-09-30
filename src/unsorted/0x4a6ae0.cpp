// Decompiled by space-bunny-free, verified by GPT-6.1-sol. Names are provisional.
// PARTIAL 50.0%. Header sweep found no improvement. Remaining differences include object/entry register allocation and branch layout.
// PARTIAL 50.0% (1685 bytes against the original's 1703). Greenfield.
// Command-button click/key handler for the 0x15b-byte entry table.
//
// What is right: the rect build, the 24-byte point copy, both FUN_004ab510
// hit-test tails, the flags 0x10/0x40/8/0x100 arms, the "button still down"
// arm, the type-4 same-team search and its callback, and the whole
// focus!=index block, all follow the original instruction for instruction.
// The single shared `goto fail` block at the end is worth 2.7%: every
// conditional early-out jumps to it (this file `jne 0x7119`, the original
// `jne 0x717b`), because MSVC 5 will not merge two identical `return 0`s,
// so the original's shared 0x4a717b epilogue is a real goto target.
//
// The remaining diff is ONE allocator decision: the original keeps obj in
// EBX and the entry pointer in EBP; this file has obj in EBP and entry in
// EBX, and every [ebx+...]/[ebp+...] in the function follows from that. The
// prologue differs by one instruction: the original loads obj with
// `mov ebx,[esp+0x30]` right after `push ebx`, this file loads it into EBP
// after `push ebp`. Nothing else changes. The two already-matched
// entry-table neighbours, 0x4a0340 and 0x4a6a40, put param_1 in EBP and the
// entries pointer in EBX, so this file agrees with their allocator; the
// original of this function ranks obj above the pointer.
//
// Free-scratch tries that did NOT move the swap, byte-identical or below:
// header sweep (headers.py, 128 sets, flat at 47.3% on the earlier 47.3%
// source); holder local briefly (v2) and live to the end (v7/v22 39.4%);
// `entry = entries + index` (v3); `entry = &obj->holder->entries[index]`
// (v8); no `entry` local, `entries[index].` written out (v9 46.7%); no
// `entries` local, `obj->holder->entries` written out (v16 49.7%); a local
// copy of obj (v23 50.0%) and a reference to it (v24 no compile); `int i =
// index;` (v17); a trailing `return 0;` (v6); `entry` uninitialised and
// assigned after the guard (v20); an `Entry&` reference (v14); `entry +=
// index` (v15).
// Caching the search block's flags in an int and its team in a char (the
// original holds flags in EDX and team in CL across the loop, and its loop
// has no extra bound compare) scored 49.8% (v12/v13/v19), so those locals
// are NOT here. A named `int n = count + 1` loop bound scored 50.0% but
// turned the original's `lea` bound into `inc` (v18), so it is not here
// either. Reordering the declarations of `entry` and `entries` and assigning
// them separately also stayed at 50.0%; the EBX/EBP swap remains unresolved.
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
        if (entry->field_138 != 0) {
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
        } else {
            if (!(point.x >= r.left && point.x <= r.right
                  && point.y >= r.top && point.y <= r.bottom))
                goto fail;
            entry->field_138 = 1;
            DAT_0051fbac = 0xf;
        }
        FUN_004a5f40(obj, index);
        if (!(entry->flags & 0x1800))
            goto fail;
        int found = 1;
        for (; found < entries->count + 1; found++) {
            if (entries[found].state == 4 && entries[found].team == entry->team)
                break;
        }
        if (found == entries->count + 1)
            found = 0;
        if (found == -1)
            goto fail;
        Entry_004a6ae0* f = &entries[found];
        short off = f->field_140;
        if (entry->flags & 0x1000) {
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
