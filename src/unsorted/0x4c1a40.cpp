// Decompiled by Opus. Names are provisional.
// Empties the ring buffer (sibling of 0x4c1a60, which also resets head/tail).

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

// FUNCTION: 0x4c1a40
void FUN_004c1a40(void)
{
    Queue_004c1ab0* q = FUN_004b6220();
    q->head = 0;
    q->tail = 0;
}
