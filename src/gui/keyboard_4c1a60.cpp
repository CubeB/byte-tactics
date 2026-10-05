// Decompiled by Sonnet. Names are provisional.
// Sibling of 0x4c1ab0/0x4c1b00/0x4c1b20: (re)initialises the ring buffer,
// clamping the requested size to the buffer's capacity (30 entries).

#pragma pack(push, 2)
struct Queue_004c1ab0 {
    char unknown_0[0xf2];
    int size;                          // +0xf2
    int entries[30];                   // +0xf6
    int head;                          // +0x16e
    int tail;                          // +0x172
};
#pragma pack(pop)

Queue_004c1ab0* GetDisplay(void);

// FUNCTION: 0x4c1a60
void __stdcall InitKeyQueue(int size)
{
    Queue_004c1ab0* q = GetDisplay();
    if (size <= 0x1e)
        q->size = size;
    else
        q->size = 0x1e;

    q = GetDisplay();
    q->head = 0;
    q->tail = 0;
}
