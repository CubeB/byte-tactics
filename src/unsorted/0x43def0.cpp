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
// Size: ours is 354 bytes against the original's 359. The whole 5 byte
// deficit is the `add reg, mem` folds below (2 bytes each, in base y, base z
// and the loop's y), and one byte in the base x block. Everything else is the
// same size, only the registers differ.
//
// The one structural difference is how the BASE piece's x is addressed. The
// original keeps index*27 in a register and folds the scale into two
// separate addressing modes:
//   lea eax, [eax + eax*2]              ; index*3
//   lea eax, [eax + eax*8]              ; index*27
//   mov edx, [ecx + eax*2 + 0x26]       ; item->x, folded, issued FIRST
//   lea eax, [ecx + eax*2 + 0x22]       ; &item, materialised after
//   mov ecx, [eax]                      ; item->p
// i.e. x is loaded before the item pointer exists, and y/z then reuse the
// materialised pointer as [eax+8] and [eax+0xc]. Below, the same shape is
// produced but with p taking the folded slot (+0x22) and x becoming
// [eax+4]. So the only thing to change is WHICH use of the address family
// &block->items[index] MSVC treats as the first one: the first use keeps the
// folded scale-2 form and the lea is materialised just after it. In the
// original that first use is the x load, here it is the p load.
//
// A second pass over this function (issue #154) added these, all of which
// also leave the p load in the folded slot and compile to 354 bytes / 63.6%,
// byte for byte the same as the version below. Do not repeat:
// - swapping the addends of the base, both `item->p->f10 + item->x` and
//   `item->x + item->p->f10`: byte identical output. MSVC 5 normalises the
//   operand order of a commutative +, so the order of the two loads is not
//   reachable from the source. This is the root cause, not a side effect.
// - casting an addend to break the normalisation: `item->x + (unsigned)
//   item->p->f10`, `(unsigned)item->x + item->p->f10`, and both cast to
//   unsigned: all byte identical, the pass runs before the conversion.
// - the same swap in the loop's `+=`, and regrouping the loop's `+=` into a
//   left or right associated `result.x = result.x + a + b`: byte identical.
// - splitting the base x into two statements, `result.x = item->x;` then
//   `result.x += item->p->f10;` (and the same for y and z): MSVC merges the
//   pair back, giving `mov ecx, [eax + 4]` / `add ecx, [edx + 0x10]`, still
//   354 bytes but now with the p load fused as well, so strictly worse.
// - building the element address by hand, `(Item_0043def0*)((char*)block +
//   0x22 + index * 0x36)`: byte identical, the address is still one family.
// - dropping the `block` local and writing `obj->recs` at each use: byte
//   identical, the pointer is CSE'd either way.
// - modelling p's three offsets as `int v[3]` and writing `p->v[0]` and so
//   on: byte identical.
// - hoisting `short angles[3]` out of the loop body to function scope: 58.6%,
//   worse, the local ordering changes and the frame allocation follows it.
// - repeating the index expression for x, `block->items[index].x +
//   item->p->f10` (61.1%): MSVC then CSEs the whole family into
//   `lea eax, [ecx + eax*2]` (no displacement) and indexes +0x22/+0x26 off
//   that, which is 1 byte longer and loses the scale-2 lea.
// - writing the three base statements in the order z, y, x (61.9%): the
//   folded load is still p, so the use order is not the source order.
// - declaring Ptr_0043def0's fields `long` instead of `int`: identical, so
//   the operand order is not type driven either.
// - hoisting `Ptr_0043def0* pp = item->p;` into a local and using pp->f10
//   (54.0%, 350 bytes): the p pointer then becomes a variable, the code
//   shrinks by 9 bytes and stops reloading it the way the original does.
//
// The remaining differences are register allocation noise around the same
// code, and the matched neighbour 0x43e060 (which adds two Vec3 with the
// inline operator+ the other files here use) shows that MSVC also refuses to
// fold those two loads into the add, so the original's "both operands in
// registers" style is not a fold MSVC would ever choose if the IR were
// shaped differently here. Three places still differ on that:
// - base y/z and the loop's y fold the p->fNN load into the add, so the item
//   field becomes the accumulator; the original always keeps p->fNN in a
//   register and adds the item field to it. In the loop, x and z already
//   come out that way, so it is the same IR with a different allocation.
// - the three FUN_004b6cc0 pointer arguments go to edx, eax, ecx in the
//   original and to eax, ecx, edx here (same order on the stack, only the
//   register choice differs), and that choice then propagates into the
//   first register used after the call.
// - the final copy: the original loads x and y into edx and esi first and
//   keeps the return pointer in edi, then negates z and stores all three;
//   here the return pointer stays in edx and the y load is interleaved.
//   Giving the return value its own `Vec3 out` local changes nothing.
// - the index-out-of-range zero block zeroes ecx and esi in the opposite
//   order (the other zero block, reached from the obj/recs test, matches).

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
