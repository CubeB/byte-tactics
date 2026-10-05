// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// The compiler-generated dynamic initialiser (_$E4) of the global
// g_packetManager (vtable 0x4fd514): its constructor (out of line at 0x4611e0)
// is inlined, and registers the atexit destructor _$E2 (0x460f60).
//
// Layout: m_defaultSendPacingMs at +0x04 (the name comes from 0x461020's
// debug string), eleven 0x1044-byte per-player channels from +0x08, a
// {buffer, used, capacity} triple at +0xb2f4 and a PacketReceiver member at
// +0xb300 that is handed the owner. The destructors 0x460f60, 0x461340 and
// 0x461420 walk "entries at +0x10": that is each channel's own +0x08 (the
// items/count pair), not a second array. This same class definition also
// compiles _$E2 byte for byte (0x460f60 matched with it in scratch).
//
// What decides the match is which calls /Ob2 leaves out of line. The two
// element types need their destructors (the channel frees its item list,
// PlayerFrameInfo frees three buffers): with them the entries array is built
// through the `vector constructor iterator' (0x401000) instead of an inlined
// loop. The tail at +0x228 has to be initialised in the member-initialiser
// list, so that the PacketReceiver vtable store comes after it. The two
// channel setters (0x462860, 0x4628a0) are only declared: with their bodies
// in the file /Ob2 inlines 0x4628a0, which the original does not.
//
// NOT A CHECKER MATCH YET: every byte matches, but the call at +0xd0 goes to
// the compiler's `vector constructor iterator' ??_H@YGXPAXIHP6EX0@Z@Z, which
// data/symbols.csv calls FUN_00401000 (the ??_H this file emits matches
// 0x401000 byte for byte).

class PacketRing {
public:
    int field_0;
    int field_4;
    int field_8;

    PacketRing() { field_0 = 0; field_4 = 0; field_8 = -1; }
};

class Class_00462860 {
public:
    void SetMinRetainMs(unsigned int ms);
};

class Class_004628a0 {
public:
    void SetSendPacingMs(int ms);
};

struct Buffers_00462d30 {
    char* a;                           // +0x0
    int field_4;
    int field_8;
    int field_c;
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
    int field_0;
    int field_4;
    int field_8;
    int field_c;
    int field_10;
    Buffers_00462d30 buffers;          // +0x14
    int field_2c;
    int field_30;

    PlayerFrameInfo();
};

struct PacketChannel {
    int field_0;                       // +0x00
    unsigned int sendPacingTicks;      // +0x04, set by 0x4628a0
    void** items;                      // +0x08
    unsigned count;                    // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    unsigned int timeoutTicks;         // +0x18, set by 0x462860
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24
    void* field_28;                    // +0x28
    int field_2c;                      // +0x2c
    int field_30;                      // +0x30
    int field_34;                      // +0x34
    PacketRing field_38;               // +0x38
    char unknown_44[0x1044 - 0x44];

    PacketChannel()
        : field_0(-1), sendPacingTicks(0), items(0), count(0), field_10(-2), field_14(-1), timeoutTicks(0),
          field_1c(-1), field_20(0), field_24(0), field_28(0), field_2c(0), field_30(0), field_34(0)
    {
        ((Class_00462860*)this)->SetMinRetainMs(4000);
        ((Class_004628a0*)this)->SetSendPacingMs(200);
    }
    ~PacketChannel()
    {
        if (items) {
            for (unsigned i = 0; i < count; i++)
                operator delete(items[i]);
            operator delete(items);
        }
        operator delete(field_28);
    }
};

class PacketReceiver {
public:
    virtual ~PacketReceiver()
    {
        void* p = field_1c;
        if (!p)
            p = field_18;
        operator delete(p);
    }
    int field_4;                       // +0x04
    void* owner;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    void* field_18;                    // +0x18
    void* field_1c;                    // +0x1c
    PlayerFrameInfo entries[10];       // +0x20
    int field_228;                     // +0x228
    int field_22c;                     // +0x22c
    int field_230;                     // +0x230
    int field_234;                     // +0x234
    int field_238;                     // +0x238

    PacketReceiver(void* o)
        : field_4(0), owner(o), field_c(-1), field_10(-1), field_14(0), field_18(0), field_1c(0),
          field_228(0), field_22c(0), field_230(0), field_234(-1), field_238(-1)
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
