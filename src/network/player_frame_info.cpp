// Decompiled by Opus and Sonnet. Names are provisional.

// The frame buffer the queue allocates.
struct FrameBuffer {
    int count;                         // +0x0
    int used;                          // +0x4
    int current;                       // +0x8
    char data[0x180c - 0xc];

    FrameBuffer() { count = 0; used = 0; current = -1; }
};

// The two halves of the object are members with their own inline
// constructors; that is what places the operator new argument push after
// the first six stores.
struct FrameHead {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    void* field_14;                    // +0x14

    FrameHead() { field_0 = -1; field_4 = -1; field_8 = -1; field_c = 0; field_10 = -1; field_14 = 0; }
    void Init(long id) { field_0 = id; field_4 = -1; field_8 = -1; field_c = 0; }
};

// The same layout as the FrameQueue class (src/network/frame_queue.cpp).
struct FrameQueue {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    void* field_c;                     // +0x0c
    FrameBuffer* buffer;               // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    FrameQueue() { buffer = 0; field_0 = 0; field_4 = 0; field_8 = 0; field_c = 0; field_14 = -1; field_18 = -1; }
    void Init()
    {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0;
        field_c = 0;
        field_14 = -1;
        field_18 = -1;
        if (buffer == 0)
            buffer = new FrameBuffer;
    }
};

void __cdecl PacketTrace(const char* fmt, ...);

extern void __cdecl operator delete(void* p);

class PlayerFrameInfo {
public:
    FrameHead head;                    // +0x00
    FrameQueue tail;                   // +0x18

    PlayerFrameInfo();
    void Initialize(long id);
    ~PlayerFrameInfo();
};

// FUNCTION: 0x4635b0
PlayerFrameInfo::PlayerFrameInfo()
{
    tail.buffer = new FrameBuffer;
}

// PlayerFrameInfo::Initialize (from its debug string): resets the fields the
// constructor sets and allocates the buffer if there is none yet. The tail's
// reset is an inline method of the member; written out flat, MSVC hoists the
// buffer load above the head stores.
// FUNCTION: 0x463610
void PlayerFrameInfo::Initialize(long id)
{
    PacketTrace("PlayerFrameInfo::Initialize: %ld", id);
    head.Init(id);
    tail.Init();
}

// FUNCTION: 0x463680
PlayerFrameInfo::~PlayerFrameInfo()
{
    operator delete(head.field_14);
    operator delete(tail.buffer);
    operator delete(tail.field_c);
}
