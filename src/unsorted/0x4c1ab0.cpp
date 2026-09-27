// Decompiled by Opus. Names are provisional.
// Codex / GPT-6 retest in #13:
// unsigned queue indices, a pop helper given the capacity and a
// separate entry pointer did not reproduce the size-before-entry load.
// Pops the next entry from a small ring buffer (0 when empty); 0x4c1b00
// peeks at it and 0x4c1b20 pushes. Remaining difference: the original loads
// `size` before the entry, this loads the entry first. Source order,
// temporaries, inline helpers, ++/+= forms, element types, volatile and
// header sets all left that order unchanged.

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

// FUNCTION: 0x4c1ab0
int FUN_004c1ab0(void)
{
    Queue_004c1ab0* q = FUN_004b6220();
    int v;
    if (q->head == q->tail) {
        v = 0;
    } else {
        v = q->entries[q->tail];
        q->tail++;
        if (q->tail == q->size)
            q->tail = 0;
    }
    return v;
}
