// PacketReceiver: one player's received-frame buffer and the ten per-player
// frame queues behind it, a member of the global PacketManager at +0xb300. The
// one declaration of the class for the files that share its layout; the
// initialisers and destructors of g_packetManager need the same view, so the
// default constructor, the (void*) constructor and the virtual destructor keep
// their inline bodies here. PlayerFrameInfo and its per-player ring are held
// by value in entries, so they are declared here too; the ring's destructor
// stays in packets_460f60.cpp. packets.cpp keeps its own view, where the same
// bytes are named through FrameQueue.
#ifndef PACKET_RECEIVER_H
#define PACKET_RECEIVER_H

struct Obj_00462d30 {
    int a, b, c;
    Obj_00462d30() { a = 0; b = 0; c = -1; }
};

// The per-player tail: the frame ring's receive buffer and its saved frames.
struct Buffers_00462d30 {
    char* a;                           // +0x0
    int baseTick;
    int bufferSize;
    int skipCount;
    char* b;                           // +0x10
    Obj_00462d30* c;                   // +0x14
    ~Buffers_00462d30();
};

struct PlayerFrameInfo {
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

class PacketReceiver {
public:
    PacketReceiver() { }
    // capacity stays in the initialiser list: the vtable store must come after it.
    PacketReceiver(void* o)
        : unused(0), owner(o), fromId(-1), toId(-1), savedFrameEntry(0), buffer(0), spare(0),
          capacity(0), length(0), spareLength(0), spareFromId(-1), spareToId(-1)
    {
    }
    virtual ~PacketReceiver()
    {
        void* p = spare;
        if (!p)
            p = buffer;
        operator delete(p);
    }
    int unused;                        // +0x04
    void* owner;                       // +0x08
    int fromId;                        // +0x0c
    int toId;                          // +0x10
    PlayerFrameInfo* savedFrameEntry;  // +0x14
    char* buffer;                      // +0x18
    char* spare;                       // +0x1c
    PlayerFrameInfo entries[10];       // +0x20
    int capacity;                      // +0x228
    int length;                        // +0x22c
    int spareLength;                   // +0x230
    int spareFromId;                   // +0x234
    int spareToId;                     // +0x238

    PlayerFrameInfo* FindPlayerFrameInfo(long id);
    int ResetReceiveBuffer();
    int ReceiveFrame(void* net, unsigned char* data, int* size);
};

#endif
