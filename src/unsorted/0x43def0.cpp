// Decompiled by space-bunny-free. Names are provisional.
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
// Still differs from the original (63.6%). The structure is right: the frame,
// the 5 locals (angles[3] + result.x/y/z), the `ret 0xc` hidden-return
// calling convention, the piece stride 0x36 and both zero-return blocks all
// match.
//
// The one thing that does not match is how the BASE piece's x is addressed.
// The original keeps index*27 in a register and folds the scale into two
// separate addressing modes:
//   lea eax, [eax + eax*2]              ; index*3
//   lea eax, [eax + eax*8]              ; index*27
//   mov edx, [ecx + eax*2 + 0x26]       ; item->x, folded, issued FIRST
//   lea eax, [ecx + eax*2 + 0x22]       ; &item, materialised after
//   mov ecx, [eax]                      ; item->p
// i.e. x is loaded before the item pointer exists, and y/z then reuse the
// materialised pointer as [eax+8] and [eax+0xc].
//
// Already tried, all of which REGRESS to 61.1% (the compiler CSEs the base
// into a single `lea eax, [ecx + eax*2]` and indexes +0x22/+0x26 off it,
// losing the scale-2 addressing mode), do not repeat:
// - loading x into its own local first: `int bx = block->items[index].x;`
// - repeating the index expression per component instead of keeping an
//   `item` pointer, e.g. `block->items[index].p->f10 + block->items[index].x`.
//
// The best version is the one below (a single materialised `item` pointer).
// What is still unfixed, besides the x addressing: the three FUN_004b6cc0
// pointer arguments land in a different register order, and the final copy
// does not reuse the register that still holds result.z for the `neg`.

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
