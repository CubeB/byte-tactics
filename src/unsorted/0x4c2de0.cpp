// Decompiled by Opus. Names are provisional.
// Peeks at the next event in the queue without removing it (compare the
// pop at 0x4c2d60).

struct Event_4c2de0 {
    int data[6];
};

#pragma pack(push, 2)
struct Queue_4c2de0 {
    char unknown_0[0x186];
    int capacity;                // +0x186
    Event_4c2de0* entries;       // +0x18a
    int head;                    // +0x18e
    int tail;                    // +0x192
    Event_4c2de0 empty;          // +0x196
};
#pragma pack(pop)

int FUN_004b6220(void);

// FUNCTION: 0x4c2de0
int __stdcall FUN_004c2de0(Event_4c2de0* out)
{
    Queue_4c2de0* q = (Queue_4c2de0*)FUN_004b6220();
    if (q->head == q->tail) {
        *out = q->empty;
        return 0;
    }
    *out = q->entries[q->tail];
    return 1;
}
