// Decompiled by space-bunny-free. Names are provisional.
// The scalar deleting destructor of PacketManager (its vtable is 0x4fd514,
// one slot; the out-of-line destructor is 0x461420 and the constructor
// 0x4611e0). Its body is the whole destructor: the PacketReceiver member
// (vtable 0x4fd518) and its ten entries first, then the eleven big entries,
// then the deleting-destructor flag test.
//
// The rest of PacketManager is in packet_manager.cpp; this view of the
// channel cannot be shared with the constructor (see there).
//
// The channels really start at +0x08, as the constructor 0x4611e0 has them;
// what this destructor frees from +0x10 is each channel's own items/count
// pair at its +0x08. This file's entry layout is a view of that and is not a
// bug.

struct Buffers_00462d30 {
    char* a;                           // +0x0
    int field_4;
    int field_8;
    int field_c;
    char* b;                           // +0x10
    char* c;                           // +0x14
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
    char unknown_2c[0x34 - 0x2c];
};

class PacketReceiver {
public:
    virtual ~PacketReceiver()
    {
        if (field_1c)
            operator delete(field_1c);
        else
            operator delete(field_18);
    }
    int field_4;                       // +0x04
    int field_8;
    int field_c;
    int field_10;
    int field_14;
    void* field_18;                    // +0x18
    void* field_1c;                    // +0x1c
    PlayerFrameInfo entries[10];       // +0x20
    char unknown_228[0x24];
};

struct Entry_004611e0 {
    void** items;                      // +0x00
    unsigned count;                    // +0x04
    char unknown_8[0x18];
    void* field_20;                    // +0x20
    char unknown_24[0x1044 - 0x24];
    ~Entry_004611e0()
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
    virtual ~PacketManager() { }
    int field_4;                       // +0x04
    int field_8;
    int field_c;
    Entry_004611e0 entries[11];        // +0x10
    int field_b2fc;                    // +0xb2fc
    PacketReceiver member;             // +0xb300
};

// Needed so the compiler emits the scalar deleting destructor.
static PacketManager s_obj;

// FUNCTION: 0x461340 ??_GPacketManager@@UAEPAXI@Z
