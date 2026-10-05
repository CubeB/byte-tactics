// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// The buffer scan is an inline helper with one `return` per outcome (as in
// 0x461b10), which gives the original's un-rotated scan loop and its block
// order. The stack slots of `this` and idx swap without the two headers.
#include <windows.h>
#include <ddraw.h>

void __cdecl PacketTrace(const char* fmt, ...);
unsigned int GetTicks();

class PacketBuffer {
public:
    void FreePackets();
};

struct Packet_00461c20;

struct Buffer_00461c20 {
    char unknown_0[8];
    int inUse;                          // +0x8
    char unknown_c[4];
    Packet_00461c20* first;             // +0x10
};

struct Packet_00461c20 {
    int field_0;                        // +0x0
    char unknown_4[8];
    Buffer_00461c20* owner;             // +0xc
    int queued;                         // +0x10
    int sentTime;                       // +0x14
    char unknown_18[4];
    Packet_00461c20* next;              // +0x1c
};

class PacketChannel {
public:
    char unknown_0[0x18];
    unsigned int minRetain;             // +0x18
    int index;                          // +0x1c
    char unknown_20[8];
    Packet_00461c20* packets;           // +0x28
    unsigned int poolSize;              // +0x2c

    Packet_00461c20* AllocPacket(int param_1);
    void GrowPools(int a, int b);
    // Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
    int GetPacketEntry(int);
    int AddPacket(int, void*, unsigned int);
    int InitPools(int, unsigned int, int, int);
};

static inline int IsReusable(Buffer_00461c20* buf, int minRetain)
{
    if (buf->inUse == 0)
        return 1;
    Packet_00461c20* p = buf->first;
    unsigned int now = GetTicks();
    int mustBeSentBefore = now - minRetain;
    PacketTrace("cur game time: %ld, min retain=%ld, mustBeSentBefore=%ld\n",
                 now, minRetain, mustBeSentBefore);
    for (; p != 0; p = p->next) {
        if (p->owner != buf)
            break;
        if (p->queued >= 0)
            return 0;
        if (p->sentTime > mustBeSentBefore && p->sentTime <= now)
            return 0;
    }
    PacketTrace("buffer eligible for reuse:  all packets have been sent more than %lu game ticks ago\n",
                 minRetain);
    return 1;
}

// FUNCTION: 0x461c20
Packet_00461c20* PacketChannel::AllocPacket(int param_1)
{
    unsigned int idx;
    Packet_00461c20* packet;
    for (;;) {
        PacketTrace("current packet pool index: %ld\n", index);
        idx = index + 1;
        if (idx >= poolSize)
            idx = 0;
        packet = &packets[idx];
        Buffer_00461c20* owner = packet->owner;
        if (owner == 0)
            break;
        if (IsReusable(owner, minRetain)) {
            ((PacketBuffer*)owner)->FreePackets();
            break;
        }
        if (poolSize >= 0x6a4) {
            PacketTrace("force-initializing a in-use buffer which was not eligible for reuse!\n");
            ((PacketBuffer*)owner)->FreePackets();
            break;
        }
        ((PacketChannel*)this)->GrowPools(0, 0x320);
    }
    index = idx;
    PacketTrace("current packet pool index set to: %ld\n", idx);
    packet->field_0 = param_1;
    return packet;
}
