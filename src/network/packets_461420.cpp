// Decompiled by Space Bunny Free. Names are provisional.
// Out-of-line destructor of the class whose vtable is 0x4fd514 (its scalar
// deleting destructor is 0x461340, which has this body inlined). It first
// destroys the PacketReceiver member at +0xb300, then the eleven big entries
// at +0x10 in reverse order. The class declarations for PacketReceiver are
// copied from 0x462cc0.cpp so its destructor is inlined here.
// The channels start at +0x08; the +0x10 here is each channel's own
// items/count pair at its +0x08 (settled in #225, see 0x460e20.cpp).
//
// The rest of PacketManager is in packet_manager.cpp; this view of the
// channel cannot be shared with the constructor (see there).

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
    PacketReceiver();
    virtual ~PacketReceiver()
    {
        if (field_1c)
            operator delete(field_1c);
        else
            operator delete(field_18);
    }
    int field_4;
    int field_8;
    int field_c;
    int field_10;
    int field_14;
    void* field_18;                    // +0x18
    void* field_1c;                    // +0x1c
    PlayerFrameInfo entries[10];       // +0x20
};

struct Entry_00461420 {
    void** items;                      // +0x0
    unsigned count;                    // +0x4
    char unknown_8[0x18];
    void* field_20;                    // +0x20
    char unknown_24[0x1020];
    ~Entry_00461420()
    {
        if (items) {
            for (unsigned i = 0; i < count; i++)
                delete items[i];
            delete items;
        }
        delete field_20;
    }
};

class PacketManager {
public:
    PacketManager();
    virtual ~PacketManager();
    int field_4;
    char unknown_8[8];
    Entry_00461420 entries[11];        // +0x10
    int field_b2fc;
    PacketReceiver member;              // +0xb300
};

static PacketManager s_obj;

// FUNCTION: 0x461420
PacketManager::~PacketManager()
{
}
