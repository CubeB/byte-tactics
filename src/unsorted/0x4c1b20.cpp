// Decompiled by Opus. Names are provisional.
// Pushes an entry onto the small ring buffer that 0x4c1ab0 pops and 0x4c1b00
// peeks at; the entry is dropped when the buffer is full.
// The store indexes with a separate `next` local (the original writes through
// entries[next - 1], displacement 0xf2), while the full test recomputes
// q->head + 1 itself; using `next` in the test too loads head straight into
// the callee-saved register, and indexing with q->head gives displacement
// 0xf6.

#pragma pack(push, 2)
struct Queue_004c1ab0 {
    char unknown_0[0xf2];
    int size;                          // +0xf2
    int entries[30];                   // +0xf6
    int head;                          // +0x16e
    int tail;                          // +0x172
};
#pragma pack(pop)

Queue_004c1ab0* FUN_004b6220(void);

// FUNCTION: 0x4c1b20
void __stdcall FUN_004c1b20(int v)
{
    Queue_004c1ab0* q = FUN_004b6220();
    int next = q->head + 1;
    if ((q->head + 1) % q->size != q->tail) {
        q->entries[next - 1] = v;
        q->head++;
        if (q->head == q->size)
            q->head = 0;
    }
}
