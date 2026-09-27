// Decompiled by space-bunny-free. Names are provisional.
// Slot 4 of Class_00471430 (vtable 0x4fd588; see 0x472d50.cpp for the class
// and 0x471cc0.cpp for the family). It first makes room in the container at
// +0xc for however many ten-tick units the object's field_4 has fallen behind
// the current tick, reallocating when the room runs out, then appends one
// element built from the three positions plus a random value, and finally sets
// field_8 to ten ticks ahead of the current tick.
//
// Three source shapes were needed to get this close to the original:
// - the container's three pointers are worked on as bytes. A source-level
//   `/ 52` on a difference of 52-byte-class pointers makes MSVC 5 SP3 emit the
//   division twice (it normalises by sizeof(T) and then divides again); as
//   bytes each count comes out with a single division.
// - the element copy has to be a call to the inline helper below, not a plain
//   memcpy and not a struct assignment: only then does the copy stay one
//   block between the `if (to)` test and the two pointer bumps, with the
//   loop's live value reloaded from the stack after every `rep movsd`.
// - the count guards are written as size()/capacity() member calls and
//   `first == 0 ? 0 : ...` inside them, not as one flat ternary over the
//   field: the other spelling makes the zero case the branch target instead
//   of the fall-through, and the member call is what keeps the two reads of
//   items.first from being folded into one.
// - the array count is clamped into a copy of need, not into need itself: the
//   original keeps the unclamped need in esi (it is the size of the new end
//   pointer a few instructions later) and spills it around the copy loop.
// 95.5 percent, 489 of 493 bytes. The post-realloc count is written as the raw
// ternary `items.first == 0 ? 0 : (items.last - items.first) / 52` rather than
// as a size() member call, with a `char* stop = data + need * 52;` local.
//
// Still different from the original (see the diff): it stores the
// operator delete[] argument into the stack slot it has just finished with
// (`mov [esp+0x18], eax` after the push), it interleaves the `add esp, 4`
// after that call with the two size leas differently, and it keeps the new end
// pointer in edx with an immediate store where this file keeps it in esi and
// stores it after the count division. Both are allocator and scheduler choices
// rather than semantic differences, so this is left here rather than churned
// further.
// - in the last two
// computations and in the first of the three position copies it uses ecx
// where the original uses eax (or the other way round): register choices only,
// same instructions.
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct Vec3_004737c0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Game_004737c0 {
    char unknown_0[0x147f3];
    void* unknown_147f3;                // +0x147f3
    char unknown_147f7[0x38a47 - 0x147f7];
    int field_38a47;                    // +0x38a47
};
#pragma pack(pop)

extern Game_004737c0* g_game;

// Declared returning int: the original uses the full eax without masking it.
int __stdcall FUN_004b7f60(void* ptr);

// The element copy of the realloc: one call, so the block copy stays whole.
static inline void Copy52_004737c0(char* to, char* from)
{
    if (to)
        memcpy(to, from, 52);
}

class Elem_004737c0 {                   // one element, 0x34 bytes
public:
    void* field_0;
    Vec3_004737c0 pos1;
    Vec3_004737c0 pos2;
    Vec3_004737c0 dir;
    int field_28;
    int field_2c;
    int field_30;
};

// The container at +0xc: three pointers of the same shape as std::vector's,
// at +0x10, +0x14 and +0x18 of the object. 0x4758c0 is its insert, called
// with (end, 1, element).
class List_004737c0 {
public:
    char* head;                         // +0x00
    char* first;                        // +0x04
    char* last;                         // +0x08
    char* end;                          // +0x0c

    int size()
    {
        return first == 0 ? 0 : (last - first) / 52;
    }
    int capacity()
    {
        return first == 0 ? 0 : (end - first) / 52;
    }

    void FUN_004758c0(char* where, int count, const Elem_004737c0& val);
};

class Class_00471430 {
public:
    virtual void FUN_00471430();        // slot 0
    virtual void FUN_00472d50();        // slot 1
    virtual void FUN_00472e30(int);     // slot 2
    virtual int FUN_00472e70();         // slot 3
    virtual void FUN_004737c0();        // slot 4
    virtual int FUN_00472e00();         // slot 5
    virtual void FUN_004736e0(int, int, int); // slot 6

    int field_4;                        // +0x04
    int field_8;                        // +0x08
    List_004737c0 items;                // +0x0c
    int field_1c;                       // +0x1c
    Vec3_004737c0 pos1;                 // +0x20
    Vec3_004737c0 pos2;                 // +0x2c
    Vec3_004737c0 dir;                  // +0x38
};

// FUNCTION: 0x4737c0
void Class_00471430::FUN_004737c0()
{
    int grow = (field_4 - g_game->field_38a47 + 10) / 10;

    if (grow > 0) {
        int size = items.size();
        int need = grow + size;
        int capacity = items.capacity();

        if (capacity < need) {
            int want = need;
            if (want < 0)
                want = 0;
            char* data = new char[want * 52];
            char* from = items.first;
            char* last = items.last;

            for (char* to = data; from != last; from += 52, to += 52)
                Copy52_004737c0(to, from);

            delete[] items.first;
            char* stop = data + need * 52;
            int count = items.first == 0 ? 0 : (items.last - items.first) / 52;
            items.end = stop;
            items.first = data;
            items.last = data + count * 52;
        }
    }

    for (int i = 0; i < 1; i++) {
        Elem_004737c0 e;

        e.pos1 = pos1;
        e.pos2 = pos2;
        e.dir = dir;
        e.field_30 = g_game->field_38a47 + field_1c;
        e.field_0 = g_game->unknown_147f3;
        e.field_28 = FUN_004b7f60(g_game->unknown_147f3) - 1;
        e.field_2c = (int)(((__int64)rand() * e.field_28) / 0x8000);
        List_004737c0* v = &items;
        v->FUN_004758c0(v->last, 1, e);
    }

    field_8 = g_game->field_38a47 + 10;
}
