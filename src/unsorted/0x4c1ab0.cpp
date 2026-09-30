// Decompiled by Opus, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry in #1905: best 95.0% after 3 checks. The only remaining
// mismatch is that VC5 loads entries[tail] before size, while the original
// loads size first. An early-return variant scored 68.4% and was discarded.
// Codex / GPT-6 retest in #13:
// unsigned queue indices, a pop helper given the capacity and a
// separate entry pointer did not reproduce the size-before-entry load.
// Pops the next entry from a small ring buffer (0 when empty); 0x4c1b00
// peeks at it and 0x4c1b20 pushes. Remaining difference: the original loads
// `size` before the entry, this loads the entry first. Source order,
// temporaries, inline helpers, ++/+= forms, element types, volatile and
// header sets all left that order unchanged.
// deepseek-v4.1-flash retest (#1337): size-hoisting local, early-return form,
// pointer-into-entries, post/pre-increment and unsigned fields all still
// produce the entry-first order; it is an MSVC scheduler tie-break, not IL
// order. Remaining 3-byte gap is that single swapped load pair.
// GPT-6.1-sol retest: pointer arithmetic and an inlined Queue::pop keep 95.0%;
// reference aliases and one-store next-index forms regress, then were reverted.
// GPT-6.1-sol refinement for #2874: hoisting size into the branch or reading
// it through a single-use helper still emits the entry load first (95.0%).

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
