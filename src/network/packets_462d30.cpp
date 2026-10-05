// Decompiled by Opus. Names are provisional.
// Destructor: frees one of two buffers, then the ten entries (constructed by
// 0x462c00) are destroyed in reverse order, each freeing its three buffers.
// 0x462cc0 is the scalar deleting destructor with this body inlined.

struct Obj_00462d30 {
    int a, b, c;
    Obj_00462d30() { a = 0; b = 0; c = -1; }
};

struct Buffers_00462d30 {
    char* a;                           // +0x0
    int field_4;
    int field_8;
    int field_c;
    char* b;                           // +0x10
    Obj_00462d30* c;                   // +0x14
    ~Buffers_00462d30()
    {
        delete a;
        delete c;
        delete b;
    }
};

struct PlayerFrameInfo {
    int field_0;
    int field_4;
    int field_8;
    int field_c;
    int field_10;
    Buffers_00462d30 buffers;          // +0x14
    int field_2c;
    int field_30;
};

class PacketReceiver {
public:
    virtual ~PacketReceiver();
    int field_4;
    int field_8;
    int field_c;
    int field_10;
    int field_14;
    void* field_18;                    // +0x18
    void* field_1c;                    // +0x1c
    PlayerFrameInfo entries[10];       // +0x20
};

// FUNCTION: 0x462d30
PacketReceiver::~PacketReceiver()
{
    if (field_1c)
        operator delete(field_1c);
    else
        operator delete(field_18);
}
