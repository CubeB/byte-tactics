// Decompiled by deepseek-v4.1-flash. Names are provisional.

void __cdecl FUN_00461170(const char* fmt, ...);
unsigned int FUN_004b6340();

class Class_00462ae0;

struct Packet_00462ae0 {
    char unknown_0[4];
    int offset;                        // +0x4
    int size;                          // +0x8
    Class_00462ae0* owner;             // +0xc
    int queued;                        // +0x10
    unsigned int sentTime;             // +0x14
    Packet_00462ae0* prev;             // +0x18
    Packet_00462ae0* next;             // +0x1c
};

class Class_004623b0 {
public:
    int count;                         // +0x0
    int index;                         // +0x4
    int unused;                        // +0x8
    int buffer[0x400];                 // +0xc

    int FUN_004623b0();
};

class Class_00462370 {
public:
    int count;                         // +0x0
    char unknown_4[4];
    int writeIdx;                      // +0x8
    int buf[0x400];                    // +0xc

    int FUN_00462370(int value);
};

struct Manager_00462ae0 {
    char unknown_0[0x20];
    int total;                         // +0x20
    char unknown_24[0xc];
    Packet_00462ae0* field_30;         // +0x30
    Packet_00462ae0* field_34;         // +0x34
    Class_004623b0 queue;              // +0x38
};

class Class_00462ae0 {
public:
    Manager_00462ae0* manager;         // +0x0
    char unknown_4[0xc];
    Packet_00462ae0* first;            // +0x10

    void FUN_00462ae0(Packet_00462ae0* packet);
};

// FUNCTION: 0x462ae0
void Class_00462ae0::FUN_00462ae0(Packet_00462ae0* packet)
{
    FUN_00461170("removing packet (len=%ld, type=%d, data=\"%s\")\n",
                 packet->size,
                 *(unsigned char*)((char*)packet->offset + (int)packet->owner + 0x14),
                 (char*)packet->offset + (int)packet->owner + 0x15);

    if (packet->queued >= 0) {
        FUN_00461170("Warning! RemovePacket called for packet in pending queue!\n");
        Manager_00462ae0* mgr = manager;
        packet->queued = -1;
        packet->sentTime = FUN_004b6340();
        int n = mgr->queue.count;
        while (n-- > 0) {
            int value = mgr->queue.FUN_004623b0();
            if (value == (int)packet)
                break;
            ((Class_00462370*)&mgr->queue)->FUN_00462370(value);
        }
        mgr->total -= packet->size;
    }

    if (first == packet) {
        Packet_00462ae0* nxt = packet->next;
        if (nxt != 0 && nxt->owner == this)
            first = nxt;
        else
            first = 0;
    }

    Manager_00462ae0* m = manager;
    if (m->field_34 == packet)
        m->field_34 = packet->prev;
    if (m->field_30 == packet)
        m->field_30 = packet->next;
    if (packet->prev != 0)
        packet->prev->next = packet->next;
    if (packet->next != 0) {
        packet->next->prev = packet->prev;
        packet->next = 0;
    }
    packet->prev = 0;
}
