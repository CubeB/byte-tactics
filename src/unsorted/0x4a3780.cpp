// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Retry 10-min note (deepseek-v4.1-flash): still 35.4%, 1841 bytes against the
// original's 1832, frame 0x34 against 0x3c. From a fresh disassembly of this
// exact file the real slot maps are now pinned down. Original: orig_sel 0x10,
// entries 0x14, n/span 0x18, step/flag8 0x1c, flags 0x20, x0 0x24, DEAD 0x28,
// x1 0x2c, y1 0x30, point 0x34. Ours: orig_sel 0x10, x0 0x14, y1 0x18, x1
// 0x1c, entries 0x20, n 0x24, span/step/flag8/remain share one slot 0x28,
// point 0x2c. So we are two slots short: the original keeps step and flags in
// slots of their own. Tried and measured no change: moving n after y1 or
// before x1; declaring x0,y0,x1,y1 (and n) up front then assigning; adding a
// dead local; initializing `int i = 1;` right after / before the point copy
// (i is still given edi and point.y still lands in esi, never edi). The
// register rotation is the cause: point.y in esi leaves one fewer busy
// callee-saved reg during the flags block, so MSVC keeps flags in a register
// instead of spilling it, which is why our frame is 8 bytes smaller and every
// [esp+N] is 8 low. No source shuffle found that gives point.y edi.
// Retry note (deepseek-v4.1-flash): computing point.y before point.x (swapping
// the two adjustment statements) raised the score from 34.7% to 35.4% with the
// same 1841-byte output, confirming that source statement order moves the
// allocator. Declaring `int flags;` at function scope while leaving its
// assignment in place, and swapping the declaration order of n and x1, both
// changed nothing (still 35.4%). Still differs: frame is 0x34 against the
// original's 0x3c because this version has seven scalar slots where the
// original has nine. The two missing slots are `flags` (original frame 0x10,
// kept in a register here because point.x/point.y are rotated: this version
// keeps point.y in esi and reloads point.x into edi, the original keeps
// point.y in edi and reloads point.x into esi, so no callee-saved register is
// left free to force the flags spill) and a dead 4-byte slot the original
// allocates at frame 0x18 and never touches.
// Earlier attempts by deepseek-v4.1-flash, space-bunny-free and GPT-6 are kept
// below this line. Still partial: the frame is 0x34 where the original has
// 0x3c, so every esp+N offset is 8 low and the callee-saved registers differ.
// The original has 9 scalar slots before the 6-dword point copy (0x10 orig_sel,
// 0x14 entries, 0x18 n/span, 0x1c step/flag8, 0x20 flags, 0x24 x0, 0x28 dead,
// 0x2c x1, 0x30 y1) while this version puts x0/x1/y1 in 0x14/0x1c/0x18 and
// keeps step and flags in registers, which also shifts the point copy to
// [esp+0x2c] instead of [esp+0x34].
// Measured again by space-bunny-free: 34.7%, unchanged. The frame is still 0x34
// against the original's 0x3c and that one cause explains most of the diff, so
// every fix below was tried only as a way to raise the slot count.
// Findings worth carrying over:
// * The original's real local slots, offset from the bottom of the 0x3c frame,
//   are 0x00 orig_sel, 0x04 entries (reused later as the row pointer), 0x08 n
//   then span, 0x0c step then flag8, 0x10 flags, 0x14 x0, 0x18 NEVER TOUCHED,
//   0x1c x1, 0x20 y1, and the 24-byte point copy at 0x24. So the original has
//   nine scalar slots, eight live and one dead, where this file has seven.
//   0x18 being dead but allocated is the clue: some source local survived frame
//   allocation and then died in the optimiser.
// * Our slot at frame offset 0x14 is shared by n, span, step, flag8, remain and
//   itemp, so this version's allocator is merging more aggressively than the
//   original's.  The original keeps step (live across the whole 0x10-flag block,
//   read at [esp+0x1c] 0x4a3b35) in a slot of its own, and keeps flags (stored
//   0x4a3b03, read 0x4a3b7e) in a slot of its own.
// * The original keeps point.y in EDI and the first loop's index in ESI.  Here
//   it is the other way round (point.y in ESI, index in EDI), which is why our
//   loop pointer spills to [esp+0x28] and the original recomputes it with
//   lea/add instead.  Fixing that is worth points on its own.
// * Original `y1` is `f19 + y0 - 1` then `-= 3` (0x4a3820 lea, 0x4a3829 sub).
//   Writing it as `f19 + y0 - 4`, and even splitting it into two statements with
//   nothing between them, both fold straight back to `lea [ecx+ebx-4]`.
// * `me->field_c0` is read once into a SHORT local (0x4a395b `mov si, word ptr
//   [ebp+0xc0]` / `test si,si`; 0x4a39be `movsx ecx,si`; 0x4a39c6 `dec esi`
//   storing si).  Reading it into `short nsel` reproduces that shape in the
//   first block but does not move the percentage.
// * Hoisting `int flags = me->flags;` above the FUN_004ab570 call to try to make
//   it spill into a slot of its own is WORSE, 28.9% and 1834 bytes: MSVC then
//   keeps the loaded flags in a register through the whole prologue and every
//   epilogue shifts.  Do not repeat that.
// Re-checked by deepseek-v4.1: still 34.7%, 1856 bytes against the original's
// 1832.  New measured facts from a fresh disassembly of this exact file:
// * span and step DO get memory slots here, but at [esp+0x2c] and [esp+0x30]
//   (the original has span at [esp+0x18] and step at [esp+0x1c]); [esp+0x2c] is
//   also where our 6-dword point copy starts, so the copy and span share the
//   same frame dword and the post-call compare at 0x4a393c reloads that slot.
//   Making point.x/point.y live in registers instead (the original keeps
//   point.y in edi across the FUN_004ab570 call) is what frees the two slots
//   the original's frame has.
// * Slot map of the original, frame offset = [esp+N] - 0x10 with a 0x3c frame
//   and four pushes: 0x00 orig_sel, 0x04 entries, 0x08 n then span,
//   0x0c step then flag8, 0x10 flags, 0x14 x0, 0x18 dead, 0x1c x1, 0x20 y1,
//   0x24 the 24-byte point copy.  Ours has orig_sel at 0x00 like the original
//   ([esp+0x10], already correct), x0 at 0x04, y1 at 0x08, x1 at 0x0c, n at
//   0x14 and the point copy at 0x1c, i.e. six scalar slots against nine.
// * Ours also emits 24 extra bytes (1856 vs 1832) and two extra jumps near
//   0x4a38e5/0x4a390e/0x4a3911, at the `list == 0 ? FUN_004c1450() : ...`
//   and `field_da == 0 ? size + 1 : field_da` ternaries, where the original
//   falls through on one arm and has no jmp.
// Re-probed by deepseek-v4.1-flash at a 900s wall: the frame size and the
// register rotation were attacked directly, with no improvement over 34.7%.
// Measured facts from that run:
// * Moving `int i = 1;` before the point copy raises the frame to 0x38 but
//   drops the score to 27.9%: the allocator then demotes y0 to a stack slot
//   and gives i ebx. Every position tried for i (before x1, before point, at
//   function top, and `int i = 1; for (; ...)` with the init pulled out of the
//   for) reaches that same 27.9% once i is born before the point copy, so the
//   0x34 frame and 34.7% are the better branch of the allocator.
// * Declaring x0,y0,x1,y1 together, caching me->field_c0 in a `short nsel`,
//   reading me->field_da into a `short da` for the span choice, and swapping
//   the loop compare to `me->group == n` all leave the bytes unchanged.
// * <stdlib.h>, <stdio.h> and all 128 header sets headers.py tries change
//   nothing; headers.py reports 34.7% as the ceiling for every set.
// * The matched siblings 0x4a9830 and 0x4a99c0 share this function's group
//   loop, size/step and FUN_004b6af0/strncmp blocks, but their exact loop form
//   (`int n = 0; int i = 1; for (; i < entries->count + 1; i++)`) is what
//   raises our frame to 0x38 and costs points here, so this original did not
//   use that form.
// The remaining diff is one global allocator colouring: the original spills
// step and flags to slots of their own and leaves frame offset 0x18 dead,
// while this version keeps flags in ecx and shares one slot between the loop
// pointer and step. No single source construct found so far forces it.
// Re-probed by deepseek-v4.1 under a 10 minute limit (still 34.7%/1841 bytes):
// * Moving the `point` copy below the span/step computation drops to 21.1%
//   (1794 bytes): the copy is then emitted last and the whole setup reshuffles.
// * Declaring n, span, step, flag8, flags, x0, y0, x1, y1 up front (before the
//   point copy, assignments left where they are) grows the frame to 0x38 but
//   only reaches 27.7%: MSVC hoists the arg load above the frame setup and
//   colours `entries` into ebx, so 0x38 is an allocator attractor (same one
//   `int i = 1;` early reaches), not the original 0x3c shape.
// * Declaring span/step/flag8 early while keeping the copy first changes
//   nothing at all (identical 1841 bytes, 34.7%).
#include <string.h>


#pragma pack(push, 1)
struct Entry_004a3780 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01
    char unknown_02[0x13 - 0x02];
    short field_13;                    // +0x13
    short field_15;                    // +0x15
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x28 - 0x1f];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0)
        int field_b6;                  // +0xb6 (the scroll repeat timer)
    };
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    void* field_c2;                    // +0xc2
    int field_c6;                      // +0xc6
    char unknown_ca[0xce - 0xca];
    void (__stdcall* field_ce)(void*, void*);  // +0xce
    char unknown_d2[0xd6 - 0xd2];
    int field_d6;                      // +0xd6
    short field_da;                    // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct List_004a3780 {
    char unknown_0[0xc];
    unsigned short* field_0c;          // +0x0c
};

struct Holder_004a3780 {
    int current;                       // +0x00
    Entry_004a3780* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a3780* list;               // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                      // +0x20
};

struct Point_004a3780 {                // 0x18 bytes, copied with rep movsd x6
    int x;                             // +0x00
    int y;                             // +0x04
    int unknown_08[4];                 // +0x08
};

struct Object_004a3780 {
    char unknown_00[0x18];
    Holder_004a3780* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a3780 point;              // +0x3c
    char unknown_54[0x60 - 0x54];
    int field_60;                      // +0x60
    int focus;                         // +0x64
    char unknown_68[0xcca - 0x68];
    int field_cca;                     // +0xcca
};
#pragma pack(pop)

extern Holder_004a3780* DAT_0051fba4;
extern char DAT_00502a20[];

void __stdcall FUN_004c1420(int id);
int FUN_004c1450();
void* __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
char* __stdcall FUN_004b6af0(void* text, int line);
int FUN_004b6340();
int __stdcall FUN_004ab570(Object_004a3780* obj, int mask);
int __stdcall FUN_004ab510(Object_004a3780* obj, int mask);
int __stdcall FUN_004ab5b0(Object_004a3780* obj, int mask);
void __stdcall FUN_004ab690(Object_004a3780* obj, int param_2);
void __stdcall FUN_0049fc50(Object_004a3780* obj, int index);
void __stdcall FUN_004a1b40(Object_004a3780* obj, int index);
void __stdcall FUN_004a2be0(Object_004a3780* obj, int index);

// FUNCTION: 0x4a3780
int __stdcall FUN_004a3780(Object_004a3780* obj, int index, int param_3)
{
    if (obj->field_60 != -1)
        return 0;
    Entry_004a3780* entries = obj->holder->entries;
    Entry_004a3780* me = &entries[index];
    int orig_sel = me->field_ba;
    if (me->field_c0 == 0)
        return 0;
    int x0;
    int y0;
    if (me->type == 0) {
        x0 = 0;
        y0 = 0;
    } else {
        x0 = me->field_13;
        y0 = me->field_15;
    }
    int x1 = me->field_17 + x0 - 1;
    int n = 0;
    int y1 = me->field_19 + y0 - 4;
    y0 += 2;
    Point_004a3780 point = obj->point;
    point.y -= entries[0].field_15;
    point.x -= entries[0].field_13;
    int i;
    for (i = 1; i < entries[0].count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                FUN_004c1420(entries[i].field_d6);
                break;
            }
            n++;
        }
    }
    if (i == entries[0].count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    int size = (DAT_0051fba4->list == 0) ? FUN_004c1450()
        : (*(unsigned short*)((char*)FUN_004b7f30(DAT_0051fba4->list->field_0c, 0x49) + 2) + 2);
    int span;
    if (me->field_da == 0)
        span = size + 1;
    else
        span = me->field_da;
    int step = (me->field_19 - 2) / span;
    int flag8 = 0;

    if (FUN_004ab570(obj, 1)) {
        if (point.x < x0 || point.x > x1 || point.y < y0 || point.y > y1)
            goto after;
        if (me->field_c0 != 0) {
            if (!(me->flags & 0x200))
                return 1;
            {
                int line = (point.y - y0) / span + me->field_bc;
                me->field_ba = (short)line;
                if ((short)line < 0)
                    return 1;
                int off = (short)line - me->field_bc;
                if (off > step - 1)
                    me->field_ba = (short)(step + me->field_bc - 1);
                if ((short)me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                char* s = FUN_004b6af0(me->field_c2, me->field_ba);
                if (strncmp(DAT_00502a20, s, 2) != 0)
                    return 1;
                me->field_ba = orig_sel;
                return 0;
            }
        }
    } else if (FUN_004ab510(obj, 1)) {
        if (point.x >= x0 && point.x <= x1 && point.y >= y0 && point.y <= y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 1);
        }
    } else if (FUN_004ab510(obj, 2)) {
        if (point.x >= x0 && point.x <= x1 && point.y >= y0 && point.y <= y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 2);
        }
    }

after:
    if (obj->focus != index)
        goto end;
    if (!FUN_004ab5b0(obj, 3))
        obj->focus = -1;
    if (point.x < x0)
        goto out;
    if (point.x > x1)
        goto out;
    if (point.y < y0)
        goto scroll_up;
    if (point.y > y1)
        goto out;
    {
        obj->holder->field_20 = index;
        int flags = me->flags;
        if (flags & 0x10) {
            int line = (point.y - y0) / span + me->field_bc;
            me->field_ba = (short)line;
            if ((short)line < 0) {
                me->field_ba = orig_sel;
            } else {
                int off = (short)line - me->field_bc;
                if (off > step - 1)
                    me->field_ba = (short)(step + me->field_bc - 1);
                if (me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                if (me->field_ba < 0)
                    me->field_ba = 0;
                if (flags & 0x200) {
                    char* s = FUN_004b6af0(me->field_c2, me->field_ba);
                    if (strncmp(DAT_00502a20, s, 2) == 0)
                        me->field_ba = orig_sel;
                }
                for (i = 1; i <= entries[0].count; i++) {
                    Entry_004a3780* e = &entries[i];
                    if (e->type == 2 && e->kind == me->kind) {
                        short v = e->field_c0;
                        v--;
                        if (me->field_ba < v)
                            v = me->field_ba;
                        e->field_ba = v;
                    }
                }
            }
        } else if ((flags & 0x20) || (flags & 0x80)) {
            flag8 = (flags >> 7) & 1;
            int bc = me->field_bc;
            char* fixed = (char*)(me->field_c6 + bc * 0x18);
            int* itemp = (int*)(me->field_c6 + bc * 4);
            int remain = point.y - y0 - 2;
            int n2 = 0;
            int k = bc;
            while (1) {
                char* row = flag8 ? fixed : *(char**)((*itemp) + 0x28);
                int h = (me->field_da != 0) ? span : *(unsigned short*)(row + 2);
                remain -= h;
                if (remain <= 0) {
                    me->field_ba = (short)(n2 + bc);
                    break;
                }
                n2++;
                k++;
                fixed += 0x18;
                itemp++;
                if (k > me->field_c0 - 1)
                    break;
            }
        }
    }

select_check:
    if (orig_sel != me->field_ba) {
        FUN_004a1b40(obj, index);
        if (me->field_ce)
            me->field_ce(obj, me);
    }
    if (me->flags & 0x40)
        return 1;
    obj->field_cca = 1;
    goto end;

out:
    if (point.y >= y0)
        goto scroll_down;

scroll_up:
    if (me->field_bc > 0 && me->field_b6 < FUN_004b6340()) {
        me->field_b6 = FUN_004b6340() + 2;
        if (me->field_ba > me->field_bc)
            me->field_ba = me->field_bc;
        me->field_ba--;
        me->field_bc--;
        if (me->field_c2 != 0) {
            int sel = me->field_ba;
            char* s = FUN_004b6af0(me->field_c2, (sel < 0) ? 0 : sel);
            if (strncmp(DAT_00502a20, s, 2) == 0)
                me->field_ba = orig_sel;
        }
        goto finish;
    }
    if (me->field_ba > 0) {
        me->field_ba = 0;
        goto finish;
    }
    goto end;

scroll_down:
    if (point.y <= y1)
        goto end;
    if (me->field_bc >= me->field_be)
        goto end;
    if (me->field_b6 >= FUN_004b6340())
        goto end;
    me->field_b6 = FUN_004b6340() + 2;
    me->field_bc++;
    {
        short sel = (short)(step + me->field_bc - 1);
        me->field_ba = (short)sel;
        if (me->field_c2 != 0) {
            char* s = FUN_004b6af0(me->field_c2, sel);
            if (strncmp(DAT_00502a20, s, 2) == 0)
                me->field_ba = orig_sel;
        }
    }

finish:
    FUN_004a1b40(obj, index);
    FUN_004a2be0(obj, index);

end:
    return obj->field_60 != -1;
}
