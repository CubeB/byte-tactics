// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Moves an object towards the cursor: decrements the hide counter, reads the
// cursor, stores it as the object's rectangle, offsets the object by the
// bitmap's half size, blits it from the screen with FUN_004c6b70, then, if the
// counter is still not positive, copies the rectangle and draws the bitmap
// with FUN_004b7f90. GetRect is the inlined copy helper (memcpy of the 24 byte
// rectangle at +0x196).
//
// NOT MATCHING (98.8%, 281 of 282 bytes). The single difference is where MSVC
// puts the null test of field_1b2 relative to the two stores of the cursor
// position into the rectangle (0x4c28be):
//
//   original: mov ecx,[esp+0xc] / mov edx,[esp+0x10] / test eax,eax
//             / mov [esi+0x196],ecx / mov [esi+0x19a],edx / je
//   ours:     ... / mov [esi+0x196],ecx / test eax,eax / mov [esi+0x19a],edx
//             / je
//
// The original has the test in front of both stores, we get it between them.
// The stores are unconditional in the original (the je at 0x4c28cc sits after
// them, so they run on both paths), so the source shape below is the right
// one; only the scheduler's choice differs. What was tried, all of which leave
// the test between the two stores or move it further away:
//   * the two stores as one statement (comma), as a POINT/struct assignment of
//     the whole rectangle, as memcpy of 8 bytes, through a local int*, through
//     int& references, through a member method of the rectangle sub-object
//     (that one blocks the field_1b2 load from being hoisted, giving
//     stores-then-test instead), and as an inlined helper with a by-pointer,
//     by-value or by-reference argument;
//   * the bmp pointer in a local, with and without the body re-reading the
//     field, and behind a static inline getter;
//   * the stores inside the if, after the if, in the if/else arms, in the arms
//     of an && with the test, in a comma inside the if condition, and in the
//     condition of the if as `(S1, S2, o->bmp != 0)` (that one always puts the
//     test between the stores);
//   * both outer tests written as one &&, as early returns, and the local
//     POINT declared at the top of the function;
//   * every header set tools/headers.py tries (128 of them, all 98.8%).
//
// The pattern the original uses does occur elsewhere: a matched file such as
// 0x4cb4c0.cpp has the same "test, store, je" order when the store belongs to
// the value the test consumes (`x = f(); if (x) ...`, see 0x4c2a80.cpp), and
// the same "store, test, store, je" order we get here, so the original's
// source probably had the two rectangle stores inside the tested expression in
// some spelling that MSVC 5 schedules the way it does there. No spelling
// tried here does that.
//
// One more thing worth recording precisely, because it narrows what is left:
// the three loads are byte-identical and in the same order in both versions
// (`mov eax,[esi+0x1b2]` for the tested pointer, then the two cursor words
// from [esp+0xc] and [esp+0x10]). The test sits in the same place relative to
// those three loads in both. So the only thing that moves is the first store,
// which is the sole difference, and it moves across a `test` that it does not
// depend on and that does not depend on it (`mov` does not touch the flags).
// Naming the tested pointer in a local first, `Bitmap* bm = o->bmp;` and then
// `if (bm != 0)`, so that the test's operand is a distinct earlier load node,
// was tried here and changes nothing: still 98.8%, still the test between the
// stores.
#include <string.h>
#include <windows.h>

struct Rect_004c2870 {
    int x;                             // +0x0
    int y;                             // +0x4
    int data[4];
};

struct Bitmap_004c2870 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short dx;                          // +0x4
    short dy;                          // +0x6
};

#pragma pack(push, 1)
struct Obj_004c2870 {
    char unknown_0[0x196];
    Rect_004c2870 rect;                // +0x196
    int count;                         // +0x1ae
    Bitmap_004c2870* bmp;              // +0x1b2
    int x;                             // +0x1b6
    int y;                             // +0x1ba
    int* field_1be;                    // +0x1be
    char unknown_1c2[0x1ce - 0x1c2];
    int mode;                          // +0x1ce
};
#pragma pack(pop)

Obj_004c2870* FUN_004b6220(void);
void __stdcall FUN_004c6b70(void* dst, void* bmp, int x, int y);
void __stdcall FUN_004b7f90(void* dst, Bitmap_004c2870* bmp, int x, int y);

static inline void GetRect(Rect_004c2870* out)
{
    Obj_004c2870* p = FUN_004b6220();
    memcpy(out, &p->rect, sizeof(Rect_004c2870));
}

// FUNCTION: 0x4c2870
void FUN_004c2870(void)
{
    Obj_004c2870* o = FUN_004b6220();
    if (o->mode != 1) {
        if (--o->count <= 0) {
            POINT pt;
            GetCursorPos(&pt);
            o->rect.x = pt.x;
            o->rect.y = pt.y;
            if (o->bmp != 0) {
                o->x = pt.x - o->bmp->dx;
                o->y = pt.y - o->bmp->dy;
                o->field_1be[0] = o->bmp->width;
                o->field_1be[1] = o->bmp->height;
                o->field_1be[2] = o->bmp->width;
            }
            FUN_004c6b70(o->field_1be, 0, -o->x, -o->y);
            Obj_004c2870* o2 = FUN_004b6220();
            if (o2->mode != 1 && o2->count <= 0) {
                Rect_004c2870 r;
                GetRect(&r);
                FUN_004b7f90(0, o2->bmp, r.x, r.y);
            }
        }
    }
}
