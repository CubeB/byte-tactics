// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, and GPT-6.1-sol, edited by deepseek-v4.1, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, issue 5064), from 86.5%. The list gadget's scroll
// bar thumb: while the entry has the focus, either drag the offset (+0x140)
// with the mouse or step it by one when the mouse is outside the thumb, then
// clamp it to 0..field_136-1 and, if it changed, mark the holder dirty,
// redraw (FUN_004a2580, FUN_004a2be0) and call the entry's callback. Without
// the focus, a left or right press inside the gadget takes the focus, and a
// press on the thumb starts a drag.
//
// What closed it: the clamp-and-notify tail is one inline helper,
// OffsetChanged, called at the end of BOTH arms (drag and step). MSVC
// cross-jumps the two identical copies into one, so the code is the same as a
// single shared tail, but the duplicated zero uses (the clamp's compare and
// store, the holder test) are what make MSVC keep the constant 0 in EDX from
// the FUN_004ab5b0 test to the FUN_004a2580 call: the original's per-arm
// `xor edx,edx`, `cmp [ebp+0x78],edx`, `cmp/mov word [ebx+0x140],dx` and
// `cmp eax,edx`. With one shared tail (the 74.4% to 86.5% bodies of earlier
// passes) the body was byte-identical to the original except that zero
// register; a throwaway `e->field_157 = 0;` before the call proved it.
// Spelling the arms with `e->off--` / `e->off++` and a store per path (not one
// `e->off = v` after the if/else) is what gives the original's 16-bit
// `mov ax,[ebx+0x140]` / `movsx ecx,ax` and the store that the no-change path
// skips.

#pragma pack(push, 1)
struct Entry_004a4170 {                // 0x15b bytes, the table of 0x4a23b0
    char unknown_00[0x13];
    short x1;                          // +0x13
    short y1;                          // +0x15
    char unknown_17[0x1b - 0x17];
    unsigned char flags;               // +0x1b, bit 1 = vertical, 0x10 = dead
    char unknown_1c[0x136 - 0x1c];
    short field_136;                   // +0x136, largest usable offset
    char unknown_138[0x140 - 0x138];
    short off;                         // +0x140, the scroll offset
    char unknown_142[0x144 - 0x142];
    int (__stdcall *cb)(void*, int);   // +0x144, called when off changed
    char unknown_148[0x14a - 0x148];
    int field_14a;                     // +0x14a, cb's second argument
    char unknown_14e[0x157 - 0x14e];
    int field_157;                     // +0x157, entry is being dragged
};
#pragma pack(pop)

struct Holder_004a4170 {
    char unknown_00[4];
    Entry_004a4170* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14, set when off changed
};

struct Point_004a4170 {                // 24 bytes, copied with rep movsd
    int x;
    int y;
    int unknown_08[4];
};

struct Object_004a4170 {
    char unknown_00[0x18];
    Holder_004a4170* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a4170 point;              // +0x3c, the mouse, table relative
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64, -1 when nothing has the focus
    char unknown_68[0x78 - 0x68];
    int field_78;                      // +0x78, non-zero while dragging
    Point_004a4170 saved;              // +0x7c, the mouse when the drag began
    short field_94;                    // +0x94, off when the drag began
};

void __stdcall FUN_0049fc50(Object_004a4170* obj, int index);
void __stdcall FUN_004a23b0(Entry_004a4170* base, int index, int* r1, int* r2);
void __stdcall FUN_004a2580(Object_004a4170* obj, int index);
void __stdcall FUN_004a2be0(Object_004a4170* obj, int index);
int __stdcall FUN_004ab510(Object_004a4170* obj, unsigned char buttons);
int __stdcall FUN_004ab5b0(Object_004a4170* obj, unsigned int mask);
void __stdcall FUN_004ab690(Object_004a4170* obj, int param_2);

static inline void OffsetChanged_004a4170(Object_004a4170* obj, int index, Entry_004a4170* e, int old)
{
    if (e->off > e->field_136 - 1)
        e->off = e->field_136 - 1;
    if (e->off < 0)
        e->off = 0;
    if (e->off == old)
        return;
    if (obj->holder)
        obj->holder->field_14 = 1;
    FUN_004a2580(obj, index);
    FUN_004a2be0(obj, index);
    if (e->cb)
        e->cb(obj, e->field_14a);
}

// FUNCTION: 0x4a4170
void __stdcall FUN_004a4170(Object_004a4170* obj, int index)
{
    Entry_004a4170* entries = obj->holder->entries;
    Entry_004a4170* e = &entries[index];
    if (e->flags & 0x10)
        return;
    if (e->field_157)
        return;

    Point_004a4170 p = obj->point;
    p.x -= entries->x1;
    p.y -= entries->y1;
    int r1[4];
    int r2[4];
    FUN_004a23b0(entries, index, r1, r2);

    if (obj->focus == index) {
        if (!FUN_004ab5b0(obj, 3)) {
            obj->focus = -1;
            obj->field_78 = 0;
        }
        if (obj->field_78) {
            int old = e->off;
            if (e->flags & 1)
                e->off = obj->field_94 - obj->saved.x + p.x;
            else
                e->off = obj->field_94 - obj->saved.y + p.y;
            OffsetChanged_004a4170(obj, index, e, old);
        } else {
            int old = e->off;
            if (e->flags & 1) {
                if (p.x < r2[0])
                    e->off--;
                else if (p.x > r2[2])
                    e->off++;
            } else {
                if (p.y < r2[1])
                    e->off--;
                else if (p.y > r2[3])
                    e->off++;
            }
            OffsetChanged_004a4170(obj, index, e, old);
        }
        return;
    }

    if (obj->field_78)
        return;
    if (FUN_004ab510(obj, 1)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        FUN_0049fc50(obj, index);
        FUN_004ab690(obj, 1);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
        return;
    }
    if (FUN_004ab510(obj, 2)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        FUN_0049fc50(obj, index);
        FUN_004ab690(obj, 2);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
    }
}
