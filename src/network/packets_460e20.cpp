// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Stays in a file of its own: this initialiser matches only with the views of
// PacketChannel and PacketRing below, which packets_460f40.cpp cannot share.
// The compiler-generated dynamic initialiser (_$E4) of the global
// g_packetManager (vtable 0x4fd514): it runs the constructor (0x4611e0) and
// registers the atexit destructor _$E2 (0x460f60).
//
// Layout: m_defaultSendPacingMs at +0x04 (the name comes from 0x461020's
// debug string), eleven 0x1044-byte per-player channels from +0x08, a
// {buffer, used, capacity} triple at +0xb2f4 and a PacketReceiver member at
// +0xb300 that is handed the owner. The destructors 0x460f60, 0x461340 and
// 0x461420 walk "entries at +0x10": that is each channel's own +0x08 (the
// items/count pair), not a second array.

class PacketRing {
public:
    int count;
    int readIdx;
    int writeIdx;

    PacketRing() { count = 0; readIdx = 0; writeIdx = -1; }
};

struct Buffers_00462d30 {
    char* a;                           // +0x0
    int baseTick;
    int bufferSize;
    int skipCount;
    char* b;                           // +0x10
    char* c;                           // +0x14
    ~Buffers_00462d30()
    {
        operator delete(a);
        operator delete(c);
        operator delete(b);
    }
};

class PlayerFrameInfo {
public:
    int playerNetId;
    int pendingDpToId;
    int frameSeq;
    int pendingBytes;
    int pendingCap;
    Buffers_00462d30 buffers;          // +0x14
    int queuedFromId;
    int queuedToId;

    PlayerFrameInfo();
};

struct PacketChannel {
    int bufferIndex;                   // +0x00
    unsigned int sendPacingTicks;      // +0x04, set by 0x4628a0
    void** items;                      // +0x08
    unsigned count;                    // +0x0c
    int frameNumber;                   // +0x10
    int dpid;                          // +0x14
    unsigned int timeoutTicks;         // +0x18, set by 0x462860
    int packetIndex;                   // +0x1c
    int queuedBytes;                   // +0x20
    int nextSendTick;                  // +0x24
    void* packets;                     // +0x28
    int packetCount;                   // +0x2c
    int firstPacket;                   // +0x30
    int lastPacket;                    // +0x34
    PacketRing queue;                  // +0x38
    char unknown_44[0x1044 - 0x44];

    // Only declared: with bodies in this file /Ob2 inlines 0x4628a0.
    void SetMinRetainMs(unsigned int ms);
    void SetSendPacingMs(int ms);

    PacketChannel()
        : bufferIndex(-1), sendPacingTicks(0), items(0), count(0), frameNumber(-2), dpid(-1), timeoutTicks(0),
          packetIndex(-1), queuedBytes(0), nextSendTick(0), packets(0), packetCount(0), firstPacket(0), lastPacket(0)
    {
        SetMinRetainMs(4000);
        SetSendPacingMs(200);
    }
    // Element destructors (here and PlayerFrameInfo's) make the entries array
    // use the vector constructor iterator, not an inlined loop.
    ~PacketChannel()
    {
        if (items) {
            for (unsigned i = 0; i < count; i++)
                operator delete(items[i]);
            operator delete(items);
        }
        operator delete(packets);
    }
};

class PacketReceiver {
public:
    virtual ~PacketReceiver()
    {
        void* p = spare;
        if (!p)
            p = buffer;
        operator delete(p);
    }
    int field_4;                       // +0x04
    void* owner;                       // +0x08
    int fromId;                        // +0x0c
    int toId;                          // +0x10
    int savedFrameEntry;               // +0x14
    void* buffer;                      // +0x18
    void* spare;                       // +0x1c
    PlayerFrameInfo entries[10];       // +0x20
    int capacity;                      // +0x228
    int length;                        // +0x22c
    int spareLength;                   // +0x230
    int spareFromId;                   // +0x234
    int spareToId;                     // +0x238

    // capacity stays in the initialiser list: the vtable store must come after it.
    PacketReceiver(void* o)
        : field_4(0), owner(o), fromId(-1), toId(-1), savedFrameEntry(0), buffer(0), spare(0),
          capacity(0), length(0), spareLength(0), spareFromId(-1), spareToId(-1)
    {
    }
};

class PacketManager {
public:
    virtual ~PacketManager() { }
    int m_defaultSendPacingMs;         // +0x04
    PacketChannel channels[11];        // +0x08
    char* buffer;                      // +0xb2f4
    int used;                          // +0xb2f8
    int capacity;                      // +0xb2fc
    PacketReceiver member;             // +0xb300

    PacketManager();
};

// FUNCTION: 0x460e20 _$E4
PacketManager g_packetManager;

PacketManager::PacketManager() : m_defaultSendPacingMs(200), buffer(0), used(0), capacity(0), member(this) { }
