// Decompiled by space-bunny-free, improved by Claude Opus 5.5. Names are provisional.
//
// Returns the world position of animation piece `index` of `obj` (with z
// negated, as the callers add it to obj->pos at +0x6a):
//
//   obj->recs (+0x9e) points to a table whose count is at +0x00 and whose
//   pieces start at +0x22, each 0x36 bytes. A piece has an offset pointer at
//   +0x00, x/y/z at +0x04/+0x08/+0x0c, its three angles at
//   +0x10/+0x12/+0x14 and a `next` at +0x32. `obj` has three base angles at
//   +0x64/+0x66/+0x68.
//
//   The base piece's offset (p->+0x10/+0x14/+0x18 plus x/y/z) seeds the
//   result; each node of the `next` chain rotates the result by its angles
//   (FUN_004b6cc0), adding the object's base angles on the last node, then
//   adds its own offset.
//
// Still differs from the original: 94.2%, 359 bytes like the original.
//
// The source shape of the body is right. The old 64% notes blamed the base
// piece's x load (the original folds it as [ecx+eax*2+0x26] before the item
// pointer exists); that, the folded `add reg, mem` loads and the
// FUN_004b6cc0 argument registers were all compiler state. With N unused
// `extern int` declarations in front, N = 42..295 (and every window of about
// 250 in each period of about 525 up to N = 6000) gives 94.2%, and
// `<string.h>` alone, `<windows.h>` alone or `<ddraw.h>` alone reach the
// same state. No N and no header set (tools/headers.py) goes past 94.2%.
//
// What still differs, both in the return blocks:
// - the final copy: the original loads x and y into edx and esi, copies the
//   return pointer into edi, negates z (ecx) and stores all three; ours
//   copies the pointer into edx and loads x and y through esi one after the
//   other. Every spelling of the final return scores the same: `result.z =
//   -result.z; return result;`, a separate `Vec3 out` in any field order,
//   `Vec3 out = result;` (56%), an inline MakeVec3(x, y, -z) or Flip(const
//   Vec3&) / Flip(Vec3) helper, a Vec3 constructor, a pointer to the result,
//   `int x = ...` locals, and `int result[3]`.
// - the index-out-of-range zero block: the original zeroes x, y, z (edx,
//   esi, ecx); ours zeroes x, z, y (edx, ecx, esi). Zeroing it x, y, z
//   (like the null block) gives the null block's registers, and MSVC then
//   merges the two blocks (87.3%); every other order, chained `a = b = c =
//   0`, memset, ZeroVec()/SetZero()/MakeVec3(0, 0, 0) helpers in either
//   block, and zeroing `result` itself are 87.3 to 94.2%.
// Both blocks in the original use the same registers (x = edx, y = esi,
// z = ecx, pointer = edi), one step on from the null block's (x = ecx,
// y = edx, z = esi); throwaway global stores before or inside either zero
// block did not rotate them. Also tried: one shared `result` returned from both the range block
// and the end (78 to 87%), nested `if (obj && obj->recs) { if (in range)
// {...} }` (47 to 55%, all zero blocks merge), and the range check plus
// computation in an inline helper (41%).

#include <string.h>

struct Vec3 {
    int x;
    int y;
    int z;
};

void __stdcall FUN_004b6cc0(Vec3* in, Vec3* out, short* angles);

struct Ptr_0043def0 {
    char unknown_0[0x10];
    int f10;                           // +0x10
    int f14;                           // +0x14
    int f18;                           // +0x18
};

#pragma pack(push, 2)
struct Item_0043def0 {
    Ptr_0043def0* p;                   // +0x00
    int x;                             // +0x04
    int y;                             // +0x08
    int z;                             // +0x0c
    short f10;                         // +0x10
    short f12;                         // +0x12
    short f14;                         // +0x14
    char unknown_16[0x32 - 0x16];
    Item_0043def0* next;               // +0x32
};

struct Block_0043def0 {
    int count;                         // +0x00
    char unknown_4[0x22 - 4];
    Item_0043def0 items[1];            // +0x22
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Object_0043def0 {
    char unknown_0[0x64];
    short f64;                         // +0x64
    short f66;                         // +0x66
    short f68;                         // +0x68
    char unknown_6a[0x9e - 0x6a];
    Block_0043def0* recs;              // +0x9e
};
#pragma pack(pop)

// FUNCTION: 0x43def0
Vec3 __stdcall FUN_0043def0(Object_0043def0* obj, int index)
{
    if (obj == 0 || obj->recs == 0) {
        Vec3 v;
        v.x = 0;
        v.y = 0;
        v.z = 0;
        return v;
    }
    Block_0043def0* block = obj->recs;
    if (index < 0 || index >= block->count) {
        Vec3 w;
        w.z = 0;
        w.x = 0;
        w.y = 0;
        return w;
    }
    Item_0043def0* item = &block->items[index];
    Vec3 result;
    result.x = item->p->f10 + item->x;
    result.y = item->p->f14 + item->y;
    result.z = item->p->f18 + item->z;
    for (Item_0043def0* n = item->next; n != 0; n = n->next) {
        short angles[3];
        angles[0] = n->f14;
        angles[2] = n->f10;
        angles[1] = n->f12;
        if (n->next == 0) {
            angles[0] = angles[0] + obj->f64;
            angles[2] = angles[2] + obj->f68;
            angles[1] = angles[1] + obj->f66;
        }
        FUN_004b6cc0(&result, &result, angles);
        result.x += n->p->f10 + n->x;
        result.y += n->p->f14 + n->y;
        result.z += n->p->f18 + n->z;
    }
    result.z = -result.z;
    return result;
}
