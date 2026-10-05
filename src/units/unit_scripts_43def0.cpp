// Decompiled by space-bunny-free, improved by Claude Opus 5.5, edited by deepseek-v4.1, finished by DeepSeek V4.1 Flash, checked by GPT-6, finished by Claude Opus 5.5. Names are provisional.
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
//   (RotateByAngles), adding the object's base angles on the last node, then
//   adds its own offset.
//
// MATCH (Claude Opus 5.5, #5127). Eleven earlier passes reached 99.2%. They
// left one pair of xors in the out-of-range return in the wrong order.
// tools/c2prio.py shows why no spelling of that block on its own can work.
// Its three zeros are three local temps of equal priority in one block, so
// their def order sets both their registers (by the +0x40 key: ecx, edx, esi)
// and the order of the xors, and the original needs two different orders.
// The fix is one `Vec3 out`, filled in both arms of an if/else and returned
// once. Each field of `out` is then one web across both arms, so the
// out-of-range zeros share their registers with the computed values. z wants
// ecx, because `-result.z` is copied from the loop's ecx temporary, and that
// pushes x to edx and y to esi. C2 still copies the return tail into each arm,
// so the out-of-range block keeps its own `ret`, now with the xors in
// x, y, z order.
// `Vec3 result` has to be declared outside the else arm: declared inside it,
// the function falls to 48.1%. As before, `<string.h>` (or about 40 to 295 unused
// declarations in front) is the compiler state the main path needs; without it
// this is 70.3%. Earlier notes found that an unrolled loop copy was the only
// way to give the end block its three-value shape. The shared `out` makes
// that copy unnecessary.

#include <string.h>

struct Vec3 {
    int x;
    int y;
    int z;
};

void __stdcall RotateByAngles(Vec3* in, Vec3* out, short* angles);

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
Vec3 __stdcall GetPieceOffset(Object_0043def0* obj, int index)
{
    if (obj == 0 || obj->recs == 0) {
        Vec3 v;
        v.x = 0;
        v.y = 0;
        v.z = 0;
        return v;
    }
    Block_0043def0* block = obj->recs;
    Vec3 out;
    Vec3 result;
    if (index < 0 || index >= block->count) {
        out.x = 0;
        out.y = 0;
        out.z = 0;
    } else {
        Item_0043def0* item = &block->items[index];
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
            RotateByAngles(&result, &result, angles);
            result.x += n->p->f10 + n->x;
            result.y += n->p->f14 + n->y;
            result.z += n->p->f18 + n->z;
        }
        out.x = result.x;
        out.y = result.y;
        out.z = -result.z;
    }
    return out;
}
