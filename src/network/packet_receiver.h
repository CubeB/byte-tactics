// PacketReceiver: one player's received-frame buffer and the ten per-player
// frame queues behind it, a member of the global PacketManager at +0xb300. The
// one declaration of the class for the files that share its layout. The
// default constructor keeps its inline body here; the (void*) constructor and
// the virtual destructor are defined in packets.cpp (0x462c00, 0x462d30), and
// packets_460e20.cpp and packets_460f60.cpp repeat them inline, since the
// initialiser and the static destructor of g_packetManager inline them.
// PlayerFrameInfo and its per-player queue are held by value in entries, so
// they are declared here too.
#ifndef PACKET_RECEIVER_H
#define PACKET_RECEIVER_H

struct FrameRing;

// One player's queue of received frames: a copy of the packet and the ring of
// the frames cut out of it.
class FrameQueue {
public:
    int baseTick;                      // +0x00
    unsigned int bufferSize;           // +0x04
    int skipCount;                     // +0x08
    char* recvBuffer;                  // +0x0c
    FrameRing* buffer;                 // +0x10
    int fromId;                        // +0x14
    int toId;                          // +0x18

    ~FrameQueue();
    FrameQueue();
    int ResetFrames();
    int QueueFrames(char* src, unsigned int size, int tick, int a4, int a5, int a6);
};

class PlayerFrameInfo {
public:
    int playerNetId;                   // +0x00 the id
    int pendingDpToId;                 // +0x04
    int frameSeq;                      // +0x08 last sequence number, -1 for none
    int pendingBytes;                  // +0x0c saved frame length
    int pendingCap;                    // +0x10 saved frame capacity
    char* frame;                       // +0x14 the saved out-of-order frame
    FrameQueue tail;                   // +0x18

    PlayerFrameInfo();
    void Initialize(long id);
    ~PlayerFrameInfo();
};

class PacketReceiver {
public:
    PacketReceiver() { }
    PacketReceiver(void* o);
    virtual ~PacketReceiver();
    int unused;                        // +0x04
    void* owner;                       // +0x08
    int fromId;                        // +0x0c current frame's sender
    int toId;                          // +0x10
    PlayerFrameInfo* savedFrameEntry;  // +0x14 entry whose saved frame is in use
    char* buffer;                      // +0x18
    char* spare;                       // +0x1c
    PlayerFrameInfo entries[10];       // +0x20
    int capacity;                      // +0x228
    int length;                        // +0x22c
    int spareLength;                   // +0x230 spare buffer's length
    int spareFromId;                   // +0x234
    int spareToId;                     // +0x238

    PlayerFrameInfo* FindPlayerFrameInfo(long id);
    int ResetReceiveBuffer();
    int ReceiveFrame(void* net, unsigned char* data, int* size);
};

#endif
