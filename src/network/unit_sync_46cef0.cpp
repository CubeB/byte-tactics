// Decompiled by Space Bunny Free. Names are provisional.
// Dispatches a 0xe-byte command packet. arg 0x65 re-sends the stored copy that
// has the same id; any other id widens the range and, when it is the one right
// after cur, runs cur and then every queued entry that follows it, asking the
// sender for the next one (packet 0x1a 0x65) whenever the queue runs dry.
// Class_0046eba0 is a std::vector<Packet_0046cef0> (allocator byte, _First,
// _Last, _End); FUN_0046eba0 is its insert(end(), count, val), out of line.

#pragma pack(push, 1)
struct Packet_0046cef0 {         // 0xe bytes
    unsigned char type;          // +0x0
    unsigned char arg;           // +0x1
    unsigned int id;             // +0x2
    int field_6;                 // +0x6
    int field_a;                 // +0xa
};
#pragma pack(pop)

int __cdecl FUN_0044fe00();
void __stdcall FUN_00451bc0(int a, unsigned int b, void* c, int d);

class Class_0046eba0 {
public:
    char unknown_0[4];
    Packet_0046cef0* first;      // +0x4
    Packet_0046cef0* last;       // +0x8
    Packet_0046cef0* end;        // +0xc
    void FUN_0046eba0(Packet_0046cef0* where, int count, Packet_0046cef0* val);
};

class Class_0046d6c0 {
public:
    void FUN_0046d6c0(void* param_1, int param_2);
};

class Class_0046cef0 {
public:
    char unknown_0[4];
    unsigned int cur;            // +0x4
    unsigned int max;            // +0x8
    Class_0046eba0 sent;         // +0xc
    Class_0046eba0 queue;        // +0x1c
    void FUN_0046cef0(Packet_0046cef0* packet, int param_2, void* param_3, unsigned int target);
};

// FUNCTION: 0x46cef0
void Class_0046cef0::FUN_0046cef0(Packet_0046cef0* packet, int param_2, void* param_3, unsigned int target)
{
    if (packet->arg == 0x65) {
        for (Packet_0046cef0* p = sent.first; p != sent.last; p++) {
            if (p->id == packet->id) {
                FUN_00451bc0(FUN_0044fe00(), target, p, 0xe);
                break;
            }
        }
        return;
    }
    if (packet->id > max) {
        max = packet->id;
    }
    if (packet->id == cur + 1) {
        cur = packet->id;
        ((Class_0046d6c0*)param_3)->FUN_0046d6c0(packet, param_2);
        for (unsigned int i = cur + 1; i <= max; i++) {
            Packet_0046cef0* p;
            for (p = queue.first; p != queue.last; p++) {
                if (p->id == i) break;
            }
            if (p == queue.last) break;
            cur = i;
            ((Class_0046d6c0*)param_3)->FUN_0046d6c0(p, param_2);
        }
        return;
    }
    if (packet->id <= cur) {
        return;
    }
    // Through a reference, so the call sets up ecx before evaluating its
    // arguments, as the original does.
    Class_0046eba0& v = queue;
    v.FUN_0046eba0(v.last, 1, packet);
    for (unsigned int i = cur + 1; i <= max; i++) {
        Packet_0046cef0* p;
        for (p = queue.first; p != queue.last; p++) {
            if (p->id == i) break;
        }
        if (p == queue.last) {
            Packet_0046cef0 msg;
            msg.type = 0x1a;
            msg.arg = 0x65;
            msg.id = i;
            FUN_00451bc0(FUN_0044fe00(), target, &msg, 0xe);
        }
    }
}
