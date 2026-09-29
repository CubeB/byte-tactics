// Decompiled by LongCat 2.5 Preview Free, finished by space-bunny-free. Names are provisional.
// NOT MATCHING: 13.9% (586 bytes original, 474 ours). What still differs:
//  - the class layout is now right (the old file was 4 bytes out: it forgot that
//    the vtable pointer occupies +0x0), and the semantics of the function are
//    recovered, but the register allocation is completely different. Original:
//    edi = this, ebp = unit index, ebx = axis, esi = walked int pointer, four
//    spilled stack slots (0x18 frame, 6 locals). Ours: this lands in ebp/ebx,
//    the walked pointer is materialised as a real pointer, frame is 0x14.
//  - the original reloads the base `mov edx, [edi + 0x14]` and keeps a BYTE
//    offset in esi (`[esi + ecx - 0x18]`, `[esi + ecx]`, `[esi + ecx + 0x18]`),
//    i.e. the three value arrays are at +0x10, +0x28, +0x40 of a 0x4c byte
//    element and the source reached them through one walked pointer q with
//    q[-6], q[0], q[6]. Tried both that and three separate e->val0[j] style
//    expressions; both materialise the element pointer instead of splitting
//    base + offset.
//  - the limit arrays (+0x04, +0x1c, +0x34) are reached through a SECOND
//    induction variable kept in memory in INT units: it starts at 13 and grows
//    by 0x13 per unit, and the three reads are `[base + idx*4 - 0x30]`,
//    `[base + idx*4]`, `[base + idx*4 - 0x18]`. So the source must have had a
//    separate `int*` walking in element units, 19 ints (0x4c) per unit. Tried
//    a hoisted `int* p = (int*)ptr14 + 13; ... p += 0x13;` (11.7%) and
//    `ptr14[i].lim2[j]` (12.6%); both worse than the plain form kept here.
//  - block 3 spills r and the wanted value to the frame (2 extra dwords) and
//    re-reads them; we keep them in registers, which is most of the 112 byte
//    size difference.
// Further analysis (space-bunny-free), for whoever picks this up:
//  - the element really is 0x4c = 19 dwords and the three "value" arrays are
//    0x10, 0x28, 0x40 while the three "limit" arrays are 0x04, 0x1c, 0x34; the
//    reads are lim0 = base+idx*4-0x30, lim2 = base+idx*4, lim1 = base+idx*4-0x18
//    with idx = d + j, d starting at 13 and growing by 0x13 per element. So the
//    original held TWO independent induction variables over the same array:
//    esi = byte offset (0x4c per element, +0x28 then +4 per axis) against a
//    base that is REMATERIALISED from this->ptr14 every time (mov edx,[edi+0x14]),
//    and a second one in dword units (13, +0x13) also in memory. That is why
//    the flag stores go through a stack slot ([esp+0x14] + reloaded base) while
//    the value reads go through esi. The limit reads' anchor is lim2[0], the
//    value reads' anchor is val1[0], so the two IVs are two source pointers,
//    not one.
//  - the six locals are [0x10]=d, [0x14]=element byte offset, [0x18]=axis j,
//    [0x1c]=element i, [0x20]=spilled r, [0x24]=spilled want. param_1 is read
//    straight out of the argument slot ([esp+0x2c]) every time, never copied.
//  - i and j are reloaded from their slots right after the third virtual call
//    (`mov ebx,[esp+0x18]; mov ebp,[esp+0x1c]`), which only happens if both
//    loop counters are memory locals that are live across that call.
//  - the prologue tests param_1 with `test eax,eax` and only then pushes
//    ebp/edi, so no zero register exists yet: the whole body must be ONE
//    `if (param_1 != 0 && field_18 != 0)` block, and esi/ebx are pushed after
//    the `count <= 0` early return.
//  - tried this run, all worse than the form kept here (12.8% each): walking the
//    element as a `Rec_4b1c00*` plus a separate `int* lims = (int*)ptr14 + 0xd`
//    walked by `lims += 0x13`; and the same with the element walked as a
//    `char*` and `q = (int*)e + 10`. Both make MSVC materialise q as a real
//    pointer (`lea edi,[edx+0x28]`) and put this in ebp instead of edi, and
//    grow the frame to 0x1c.
//  - also no effect (identical 474-byte object, 13.9%): dropping the dead
//    `k`, hoisting `i` out of the for, hoisting `j` out of the inner for. The
//    element count is already written inline twice, as the original reloads it
//    at the bottom of the loop (`mov ecx,[edi+8]; mov eax,[ecx+8]`), so it is
//    not worth turning that into a named local.
#include <stdlib.h>

struct Rec_4b1c00 {
    int flag;         // +0x00: set when a channel could not be smoothed
    int lim0[3];      // +0x04
    int val0[3];      // +0x10
    int lim1[3];      // +0x1c
    int val1[3];      // +0x28
    int lim2[3];      // +0x34
    int delta[3];     // +0x40
};

class Class_004b1c00 {
public:
    char unknown_0[0x4];              // the vtable pointer
    void* field_8;                    // +0x8: count at +8
    char unknown_c[0x14 - 0xc];
    Rec_4b1c00* ptr14;                // +0x14
    int field_18;                     // +0x18

    virtual void FUN_00480c50(int, int, int) = 0;  // slot 0
    virtual void FUN_00480ce0(int, int, int) = 0;  // slot 1
    virtual void FUN_00480d50(int, int) = 0;       // slot 2
    virtual void FUN_00480db0(int, int) = 0;       // slot 3
    virtual void FUN_00480df0(int, int) = 0;       // slot 4
    virtual int FUN_00480c30(int, int) = 0;        // slot 5
    virtual int FUN_00480cb0(int, int) = 0;        // slot 6

    void FUN_004b1c00(int param_1);
};

// FUNCTION: 0x4b1c00
void Class_004b1c00::FUN_004b1c00(int param_1)
{
    if (param_1 == 0)
        return;
    if (field_18 == 0)
        return;
    field_18 = 0;
    if (((int*)field_8)[2] <= 0)
        return;

    Rec_4b1c00* e = ptr14;
    int k = 0xd;

    for (int i = 0; i < ((int*)field_8)[2]; i++) {
        if (e->flag != 0) {
            e->flag = 0;
            int* q = e->val1;
            for (int j = 0; j <= 2; j++) {
                if (q[-6] != 0) {
                    int v = FUN_00480c30(i, j) + param_1 * q[-6];
                    int lim = e->lim0[j];
                    if (q[-6] > 0) {
                        if (v < lim)
                            e->flag = 1;
                        else {
                            v = lim;
                            q[-6] = 0;
                        }
                    } else {
                        if (v > lim)
                            e->flag = 1;
                        else {
                            v = lim;
                            q[-6] = 0;
                        }
                    }
                    FUN_00480c50(i, j, v);
                }
                if (q[6] != 0) {
                    q[0] += q[6];
                    if (q[6] > 0) {
                        if (q[0] >= e->lim2[j]) {
                            q[0] = e->lim2[j];
                            q[6] = 0;
                        }
                    } else {
                        if (q[0] <= e->lim2[j]) {
                            q[0] = e->lim2[j];
                            q[6] = 0;
                        }
                    }
                }
                if (q[0] != 0) {
                    int r = FUN_00480cb0(i, j) + param_1 * q[0];
                    int want = e->lim1[j];
                    if (want != -1) {
                        if (q[0] > 0) {
                            if (abs(want - r + 0x10000) & 0xffff > param_1 * q[0])
                                e->flag = 1;
                            else {
                                r = want;
                                q[0] = 0;
                            }
                        } else {
                            if (abs(r - want + 0x10000) & 0xffff > -(param_1 * q[0]))
                                e->flag = 1;
                            else {
                                r = want;
                                q[0] = 0;
                            }
                        }
                    } else {
                        e->flag = 1;
                    }
                    FUN_00480ce0(i, j, r & 0xffff);
                }
                q++;
            }
        }
        e++;
        k += 0x13;
    }
}
