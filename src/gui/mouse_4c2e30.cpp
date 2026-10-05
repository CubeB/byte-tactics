// Decompiled by Opus. Names are provisional.
// Pushes an event onto the queue unless it is full (compare the pop at
// 0x4c2d60).

struct Event_4c2e30 {
    int data[6];
};

#pragma pack(push, 2)
struct Queue_4c2e30 {
    char unknown_0[0x186];
    int capacity;                // +0x186
    Event_4c2e30* entries;       // +0x18a
    int head;                    // +0x18e
    int tail;                    // +0x192
    Event_4c2e30 empty;          // +0x196
};
#pragma pack(pop)

int GetDisplay(void);

// FUNCTION: 0x4c2e30
void __stdcall PushMouseEvent(Event_4c2e30* ev)
{
    Queue_4c2e30* q = (Queue_4c2e30*)GetDisplay();
    if ((q->head + 1) % q->capacity != q->tail) {
        q->entries[q->head] = *ev;
        if (++q->head == q->capacity) {
            q->head = 0;
        }
    }
}
