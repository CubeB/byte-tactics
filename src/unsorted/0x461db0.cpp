// Decompiled by space-bunny-free. Names are provisional.
// Partial (94.8%, 465 of 468 bytes). Everything matches except the final
// countdown loop, where the original re-reads field_38 at the top of the loop
// (`mov eax,[ebx+0x38]; cmp eax,ebp; jle latch` at 0x461f43) and compares the
// field in memory in the latch (`cmp dword ptr [ebx+0x38],ebp; jne` at
// 0x461f61), while this version keeps the loop's test value in a register
// across the back edge. The original is a do/while (that alone gives the
// memory-operand latch, see build/scratch notes) behind an entry test that
// MSVC 5 here always folds into the loop, which rotates it back to this form.
// A plain do/while (no guard), a guarded do/while, a guarded while, a goto
// loop, an inline helper for the condition, and a local copy of field_38 for
// the guard were all tried; only the unguarded do/while keeps the latch and it
// loses the entry test, so the loop stayed a while. 3 bytes out of 468.
//
// The three failure exits all jump to one shared epilogue at 0x461f78 in the
// original, which needs a single `return 0` in the source, hence `goto fail`.
// The loop counters are declared at function scope (uninitialised) so the
// jumps do not skip an initialiser.
//
// Two oddities kept as the original has them: the fresh pool entries get only
// dwords 1..7 of each 0x20-byte record initialised (0x461e11), and the reset
// copy at the end copies a stack template whose first dword is never written
// (0x461ee8 writes 0x4614..0x462c, the copy starts at 0x4610), so a new
// pool's first dword stays whatever operator new returned. It does not matter
// because nothing in this function reads it. Also `q->count = 0` before
// FUN_004629b0() (0x461ecb) is redundant: that method sets count to 0 itself
// (0x4629b0.cpp).

void* operator new(unsigned int size);

class Class_00462470;

// One 0x20-byte pool entry. The first dword is never written by this function,
// neither by the allocation loop nor by the reset copy further down.
struct Entry_00461db0 {
    int field_0;                    // +0x00, left alone
    int field_4;                    // +0x04
    int field_8;                    // +0x08
    int field_c;                    // +0x0c
    int field_10;                   // +0x10, reset to -1
    int field_14;                   // +0x14
    int field_18;                   // +0x18
    int field_1c;                   // +0x1c
};

class Class_004629b0 {
public:
    Class_00462470* pool;           // +0x00
    int start;                      // +0x04
    int count;                      // +0x08
    int field_c;                    // +0x0c
    int field_10;                   // +0x10
    void FUN_004629b0();
};

class Packet_00461db0 : public Class_004629b0 {
public:
    char unknown_14[0x43e - 0x14];
};

class Class_00462470 {
public:
    int field_0;                    // +0x00
    int field_4;                    // +0x04
    Packet_00461db0** packets;      // +0x08
    unsigned int field_c;           // +0x0c
    char unknown_10[4];
    int field_14;                   // +0x14
    char unknown_18[4];
    int field_1c;                   // +0x1c
    int field_20;                   // +0x20
    char unknown_24[4];
    Entry_00461db0* entries;        // +0x28
    unsigned int count;             // +0x2c
    int field_30;                   // +0x30
    int field_34;                   // +0x34
    int field_38;                   // +0x38
    int field_3c;                   // +0x3c

    int FUN_00461db0(int a1, unsigned int a2, int a3, int a4);
};

// FUNCTION: 0x461db0
int Class_00462470::FUN_00461db0(int a1, unsigned int a2, int a3, int a4)
{
    unsigned int i;
    unsigned int j;
    int k;
    field_14 = a1;
    field_4 = (a2 * 30 + 999) / 1000;
    if (entries == 0) {
        if (a4 == 0)
            a4 = 100;
        Entry_00461db0* p = (Entry_00461db0*)operator new(a4 * 32);
        if (p) {
            // A countdown over a separate walking pointer: this spelling is
            // what gives `lea edx,[esi-1] / cmp / jl` and `inc edx` at 0x461e06.
            k = a4 - 1;
            Entry_00461db0* q = p;
            while (k >= 0) {
                q->field_4 = 0;
                q->field_8 = 0;
                q->field_c = 0;
                q->field_10 = -1;
                q->field_14 = 0;
                q->field_18 = 0;
                q->field_1c = 0;
                q++;
                k--;
            }
        } else {
            p = 0;
        }
        entries = p;
        if (p == 0)
            goto fail;
        count = a4;
    }
    if (packets == 0) {
        if (a3 == 0)
            a3 = 2;
        packets = (Packet_00461db0**)operator new(a3 * 4);
        if (packets == 0)
            goto fail;
        for (field_c = 0; field_c < a3; field_c++) {
            Packet_00461db0* q = (Packet_00461db0*)operator new(0x43e);
            if (q) {
                q->pool = this;
                q->start = -1;
                q->count = 0;
                q->field_c = 0;
                q->field_10 = 0;
            } else {
                q = 0;
            }
            packets[field_c] = q;
            if (packets[field_c] == 0)
                goto fail;
        }
    }
    for (i = 0; i < field_c; i++) {
        // The pointer local is what puts the store in eax and the call in ecx.
        Packet_00461db0* q = packets[i];
        q->count = 0;
        packets[i]->FUN_004629b0();
    }
    Entry_00461db0 t;
    t.field_4 = 0;
    t.field_8 = 0;
    t.field_c = 0;
    t.field_10 = -1;
    t.field_14 = 0;
    t.field_18 = 0;
    t.field_1c = 0;
    for (j = 0; j < count; j++)
        entries[j] = t;
    field_0 = -1;
    field_1c = -1;
    field_30 = 0;
    field_34 = 0;
    // See the note at the top: the original reloads field_38 at the top of
    // this loop, this spelling keeps the value in a register across the back
    // edge (3 bytes out).
    while (field_38 != 0) {
        if (field_38 > 0) {
            field_38--;
            field_3c++;
            if (field_3c >= 0x400)
                field_3c = 0;
        }
    }
    field_20 = 0;
    return 1;
fail:
    return 0;
}
