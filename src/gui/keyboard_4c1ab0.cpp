// Decompiled by Opus, finished by GPT-6.1-sol, finished by DeepSeek V4.1 Flash. Names are provisional.
// Pops the next entry from a small ring buffer (0 when empty); 0x4c1b00
// peeks at it and 0x4c1b20 pushes.
// Matching the original's instruction order needs the same idiom the push
// sibling uses: hold the *next* index in a local (`int next = q->tail + 1;`)
// and read the entry as `entries[next - 1]`. VC5 folds `next - 1` back into
// the pre-increment index register (displacement 0xf6, `inc ecx` after the
// load), and the local makes it schedule the `size` load before the entry
// load. Writing the plain `v = entries[q->tail]; q->tail++;` reads the entry
// first and swaps exactly those two loads.

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
        int next = q->tail + 1;
        v = q->entries[next - 1];
        q->tail++;
        if (q->tail == q->size)
            q->tail = 0;
    }
    return v;
}
