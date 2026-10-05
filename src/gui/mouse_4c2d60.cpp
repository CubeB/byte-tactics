// Decompiled by Opus. Names are provisional.

struct Event_4c2d60 {
    int data[6];
};

#pragma pack(push, 2)
struct Queue_4c2d60 {
    char unknown_0[0x186];
    int capacity;                // +0x186
    Event_4c2d60* entries;       // +0x18a
    int head;                    // +0x18e
    int tail;                    // +0x192
    Event_4c2d60 empty;          // +0x196
};
#pragma pack(pop)

int GetDisplay(void);

// FUNCTION: 0x4c2d60
int __stdcall PopMouseEvent(Event_4c2d60* out)
{
    Queue_4c2d60* q = (Queue_4c2d60*)GetDisplay();
    if (q->head == q->tail) {
        *out = q->empty;
        return 0;
    }
    *out = q->entries[q->tail];
    if (++q->tail == q->capacity) {
        q->tail = 0;
    }
    return 1;
}
