// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Stays in a file of its own: this destructor matches only with the inline
// PacketReceiver, PlayerFrameInfo and FrameQueue destructors below, where
// packets.cpp needs them out of line. The compiler-generated static
// destructor (_$E2) of the global object g_packetManager, whose dynamic
// initialiser is 0x460e20 and whose out-of-line destructor is 0x461420.

void __cdecl operator delete(void*);

#include "packet_receiver.h"

// One of the eleven per-player objects embedded in the global.
struct Sub_00460f60 {
    void** items;                      // +0x0
    unsigned int count;                // +0x4
    char pad[0x18];
    void* q;                           // +0x20
    char pad2[0x1020];
    ~Sub_00460f60();
};

// Member layout sets the order in which the destructors are inlined.
class PacketManager {
public:
    char pad0[0xc];
    Sub_00460f60 subs[11];             // +0x10
    char pad1[4];
    PacketReceiver member;             // +0xb300
    virtual ~PacketManager() {}
};

// FUNCTION: 0x460f60 _$E2
PacketManager g_packetManager;

inline FrameQueue::~FrameQueue()
{
    operator delete(buffer);
    operator delete(recvBuffer);
}

inline PlayerFrameInfo::~PlayerFrameInfo()
{
    operator delete(frame);
}

inline PacketReceiver::~PacketReceiver()
{
    void* p = spare;
    if (!p)
        p = buffer;
    operator delete(p);
}

Sub_00460f60::~Sub_00460f60()
{
    if (items) {
        for (unsigned int i = 0; i < count; i++)
            operator delete(items[i]);
        operator delete(items);
    }
    operator delete(q);
}
