// Decompiled by DeepSeek V4.1 Flash and Claude Opus 5.5, finished by deepseek-v4-flash, finished by space-bunny-free. Names are provisional.
// MATCH. The body is exactly the version deepseek-v4-flash left: the only
// change needed to reach 100% was `#include <memory.h>` at the top, which
// changes the frame layout enough for the register allocator to restore
// `this` before the index on the reuse-success path (the two reloads at
// 0x461bcf/0x461bd3 that were swapped in the 99.0% version). The bodies of
// the earlier attempts (this->head = ix, a duplicated store, a temporary, an
// unsigned long index) were all attempts to fix that reload order by hand.

#include <memory.h>

void __cdecl PacketTrace(const char* fmt, ...);
unsigned int GetTicks();

struct Packet_004629b0;

class PacketBuffer {
public:
    char unknown_0[8];
    int count;                          // +0x8
    char unknown_c[4];
    Packet_004629b0* first;             // +0x10

    void FreePackets();
};

struct Packet_004629b0 {
    char unknown_0[0xc];
    PacketBuffer* owner;                // +0xc
    int queued;                         // +0x10
    int sentTime;                       // +0x14
    char unknown_18[4];
    Packet_004629b0* next;              // +0x1c
};

class Class_00461fd0 {
public:
    int GrowPools(int unused, int size);
};

class Class_00461b10 {
public:
    int head;                           // +0x0
    char unknown_4[4];
    PacketBuffer** array;               // +0x8
    unsigned int count;                 // +0xc
    char unknown_10[8];
    int minRetain;                      // +0x18

    PacketBuffer* AllocBuffer();
};

static inline int IsReusable(PacketBuffer* buf, int minRetain)
{
    if (buf->count == 0)
        return 1;
    Packet_004629b0* p = buf->first;
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

// FUNCTION: 0x461b10
PacketBuffer* Class_00461b10::AllocBuffer()
{
    for (;;) {
        if (count > 0) {
            unsigned int ix = head;
            head = ix;
            ++ix;
            if (ix >= count)
                ix = 0;
            PacketBuffer* buf = array[ix];
            if (IsReusable(buf, minRetain)) {
                buf->FreePackets();
                head = ix;
                return buf;
            }
            if (count >= 0x22) {
                buf->FreePackets();
                head = ix;
                PacketTrace("force-allocated a previously-used buffer, ix=%ld\n", ix);
                return buf;
            }
        }
        if (((Class_00461fd0*)this)->GrowPools(0x10, 0x320) == 0)
            return 0;
    }
}
