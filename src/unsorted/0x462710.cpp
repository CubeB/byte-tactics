// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Packet_00462710 {
    char unknown_0[8];
    int size;                          // +0x08
    void* owner;                       // +0x0c
    int queued;                        // +0x10
    char unknown_14[8];
    Packet_00462710* next;             // +0x1c
};

struct Queue_00462710 {
    int count;                         // +0x0
    int readIdx;                       // +0x4
    int writeIdx;                      // +0x8
    int buf[0x400];                    // +0xc

    int Push(int value)
    {
        if (count < 0x400) {
            writeIdx = writeIdx + 1;
            if (writeIdx >= 0x400) {
                writeIdx = 0;
            }
            buf[writeIdx] = value;
            count = count + 1;
            return 1;
        }
        return 0;
    }
};

class Class_00461b10 {
public:
    void* FUN_00461b10();
};

class Class_00461c20 {
public:
    Packet_00462710* FUN_00461c20(int param_1);
};

class Class_004628d0 {
public:
    int FUN_004628d0(Packet_00462710* packet, int number, void* data, unsigned int length, void* prev);
};

class Class_00462ae0 {
public:
    void FUN_00462ae0(void* param);
};

class Class_004624a0 {
public:
    void FUN_004624a0(int param_1);
};

class Class_00462710 {
public:
    int field_00;                      // +0x0
    char unknown_04[4];
    void** field_08;                   // +0x8
    char unknown_0c[0x10];
    int field_1c;                      // +0x1c
    unsigned int field_20;             // +0x20
    char unknown_24[0xc];
    Packet_00462710* field_30;         // +0x30
    Packet_00462710* field_34;         // +0x34
    Queue_00462710 queue;              // +0x38

    int FUN_00462710(int param_1, void* param_2, unsigned int param_3);
};

// FUNCTION: 0x462710
int Class_00462710::FUN_00462710(int param_1, void* param_2, unsigned int param_3)
{
    if (field_20 >= 0x42a || queue.count == 0x400) {
        ((Class_004624a0*)this)->FUN_004624a0(1);
        if (queue.count != 0)
            return 0;
    }
    Class_00462ae0* block = 0;
    int idx = field_00;
    if (idx >= 0)
        block = (Class_00462ae0*)field_08[idx];
    if (block == 0) {
        block = (Class_00462ae0*)((Class_00461b10*)this)->FUN_00461b10();
        if (block == 0)
            return 0;
    }
    Packet_00462710* pkt = ((Class_00461c20*)this)->FUN_00461c20(param_1);
    if (pkt == 0)
        return 0;
    int r = ((Class_004628d0*)block)->FUN_004628d0(pkt, field_1c, param_2, param_3, field_34);
    if (r == 0) {
        field_1c = field_1c - 1;
        block = (Class_00462ae0*)((Class_00461b10*)this)->FUN_00461b10();
        if (block != 0) {
            pkt = ((Class_00461c20*)this)->FUN_00461c20(param_1);
            if (pkt != 0)
                r = ((Class_004628d0*)block)->FUN_004628d0(pkt, field_1c, param_2, param_3, field_34);
            else
                r = 0;
        }
    }
    if (r != 0) {
        if (field_30 == 0)
            field_30 = pkt;
        if (field_34 != 0)
            field_34->next = pkt;
        field_34 = pkt;
        queue.Push((int)pkt);
        pkt->queued = 0;
        field_20 += pkt->size;
        return 1;
    }
    if (block != 0 && pkt->owner == block)
        block->FUN_00462ae0(pkt);
    return 0;
}
