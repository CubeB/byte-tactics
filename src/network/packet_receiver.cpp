// Decompiled by Opus, space-bunny-free, deepseek-v4.1-flash, GPT-6, deepseek-v4.1 and fledge-alpha-free. Names are provisional.
// PacketReceiver (vtable 0x4fd518, one slot): the receive side of the packet
// manager, with a PlayerFrameInfo entry (a ring of queued frames) for each of
// ten players.
#include <string.h>

void __cdecl PacketTrace(const char* fmt, ...);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
char* __stdcall HAPINET_GetDPErrorString(int error);

// One queued frame of a player's ring: the tick it is due, the data and size.
struct Frame_00462f30 {
    int tick;                          // +0x0
    void* data;                        // +0x4
    int size;                          // +0x8
};

// 0x180c-byte ring of 0x200 frames.
struct Ring_00462f30 {
    int count;                         // +0x0
    int head;                          // +0x4
    int tail;                          // +0x8
    Frame_00462f30 frames[0x200];      // +0xc

    Ring_00462f30() { count = 0; head = 0; tail = -1; }
    Frame_00462f30* Pop()
    {
        if (count > 0) {
            count--;
            Frame_00462f30* f = &frames[head];
            if (++head >= 0x200)
                head = 0;
            return f;
        }
        return 0;
    }
};

// The tail of a PlayerFrameInfo entry (see 0x463730 and 0x463790).
class FrameQueue {
public:
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    char* field_c;                     // +0x0c
    Ring_00462f30* buffer;             // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    int QueueFrames(char* src, unsigned int size, int tick, int a4, int a5, int a6);
    void Init()
    {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0;
        field_c = 0;
        field_14 = -1;
        field_18 = -1;
        if (buffer == 0)
            buffer = new Ring_00462f30;
    }
    Frame_00462f30* Peek()
    {
        if (buffer == 0 || buffer->count <= 0)
            return 0;
        return &buffer->frames[buffer->head];
    }
    // Pops the frame at the head if it is due (or if there is no tick).
    void* Take(Frame_00462f30* f, int tick, int& size)
    {
        if (f != 0) {
            int d = f->tick - tick;
            if (tick == 0 || d <= 0 || d > 0x1e) {
                f = buffer->Pop();
                size = f->size;
                return f->data;
            }
        }
        return 0;
    }
    void* GetFrame(int tick, int& size)
    {
        size = 0;
        return Take(Peek(), tick, size);
    }
};

// The saved out-of-order frame and the ring of queued frames: what an entry
// frees.
struct FrameStore {
    char* frame;                       // +0x00 the saved frame
    FrameQueue tail;                   // +0x04

    ~FrameStore()
    {
        delete frame;
        delete tail.buffer;
        delete tail.field_c;
    }
};

struct PlayerFrameInfo {
    int field_0;                       // +0x00 the id
    int field_4;                       // +0x04
    int field_8;                       // +0x08 last sequence number, -1 for none
    int field_c;                       // +0x0c saved frame length
    int field_10;                      // +0x10 saved frame capacity
    // Own member: anchors the destructor's loop pointer at +0x14 of each entry.
    FrameStore store;                  // +0x14
    // Init and Reset stay inline methods, the tail with its own inline Init:
    // the store order depends on it.
    void Init(long id)
    {
        PacketTrace("PlayerFrameInfo::Initialize: %ld", id);
        field_0 = id;
        field_4 = -1;
        field_8 = -1;
    }
    void Reset()
    {
        field_c = 0;
        store.tail.Init();
    }
};

#pragma pack(push, 1)
struct GameEntry_00462d90 {
    int id;                            // +0x00 (g_game + 0x1b67)
    char unknown_4[0x14b - 4];
};

struct Game {
    char unknown_0[0x1b67];
    GameEntry_00462d90 players[10];    // +0x1b67
    char unknown_2855[0x38a47 - 0x2855];
    int tick;                          // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

class NetCondenser {
public:
    int ReceivePacket(void* net, char* data, int* size);
};

extern NetCondenser g_receiveCondenser;

void __stdcall ReportPacketGap(int a, int b, int c);

// The sequence numbers next to n, held below -1 (-1 or more becomes -2).
static int Prev_00462f30(int n)
{
    int r = n - 1;
    if (r >= -1)
        r = -2;
    return r;
}

static int Next_00462f30(int n)
{
    int r = n + 1;
    if (r >= -1)
        r = -2;
    return r;
}

class PacketReceiver {
public:
    // In packets_462c00.cpp: it builds the entries through their constructors,
    // which would need the entries here to have no destructor.
    PacketReceiver(void* o);
    virtual ~PacketReceiver();
    int field_4;                       // +0x04
    void* owner;                       // +0x08
    int field_c;                       // +0x0c current frame's sender
    int field_10;                      // +0x10
    PlayerFrameInfo* field_14;         // +0x14 entry whose saved frame is in use
    char* buffer;                      // +0x18
    char* spare;                       // +0x1c
    PlayerFrameInfo entries[10];       // +0x20
    int capacity;                      // +0x228
    int length;                        // +0x22c
    int field_230;                     // +0x230 spare buffer's length
    int field_234;                     // +0x234
    int field_238;                     // +0x238

    PlayerFrameInfo* FindPlayerFrameInfo(long id);
    int ResetReceiveBuffer();
    int ReceiveFrame(void* net, unsigned char* data, int* size);
};

// Needed so the compiler emits the vtable (and with it this COMDAT).
// FUNCTION: 0x462cc0 ??_GPacketReceiver@@UAEPAXI@Z
static PacketReceiver s_obj(0);

// The destructor: frees one of two buffers; the ten entries are then
// destroyed in reverse order, each freeing its three buffers.
// FUNCTION: 0x462d30
PacketReceiver::~PacketReceiver()
{
    if (spare)
        operator delete(spare);
    else
        operator delete(buffer);
}

// Looks up (or creates) the PlayerFrameInfo entry for an id in a table of ten
// of them, the layout of the class built by the constructor 0x462c00: an entry
// is 0x34 bytes at this+0x20, with a three-int head (the debug string gives
// PlayerFrameInfo away) and a tail holding three zeroed ints, a buffer pointer
// and two -1s, the same tail as the sibling 0x463610 (see 0x4635b0.cpp and
// 0x4636b0.cpp for the buffer object).
//
// Three ways an entry is found: one already holding the id, an unused one
// (-1) after the first hole, or, when the table is full, one whose id no
// player in g_game's table of ten 0x14b-byte players still refers to (that one
// is recycled, and the old id keeps its g_game player). Returns 0 when all ten
// are still in use.

// FUNCTION: 0x462d90
PlayerFrameInfo* PacketReceiver::FindPlayerFrameInfo(long id)
{
    unsigned int i;
    int j;
    PlayerFrameInfo* e;

    for (i = 0; i < 10; i++) {
        e = &entries[i];
        if (e->field_0 == id)
            return e;
        if (e->field_0 == -1)
            break;
    }
    if (i < 10) {
        for (; i < 10; i++) {
            // Separate from e: else the test is strength-reduced and the second
            // call site loses its recomputed address.
            PlayerFrameInfo* p = &entries[i];
            if (p->field_0 == -1) {
                entries[i].Init(id);
                e = &entries[i];
                e->Reset();
                return e;
            }
        }
    }
    for (i = 0; i < 10; i++) {
        int used = 0;
        for (j = 0; j < 10; j++) {
            if (g_game->players[j].id == entries[i].field_0) {
                used = 1;
                break;
            }
        }
        if (used)
            continue;
        e = &entries[i];
        e->Init(id);
        e->Reset();
        return e;
    }
    return 0;
}

// Makes sure the 0x42a-byte buffer at +0x18 exists (reusing the spare one at
// +0x1c if there is one), resets the length at +0x22c and detaches the
// object at +0x14. Returns 0 only when the allocation fails.

// FUNCTION: 0x462ed0
int PacketReceiver::ResetReceiveBuffer()
{
    if (buffer == 0) {
        if (spare != 0) {
            buffer = spare;
            spare = 0;
        } else {
            buffer = new char[0x42a];
            if (buffer == 0) {
                return 0;
            }
            capacity = 0x42a;
        }
    }
    length = 0;
    if (field_14 != 0) {
        field_14->field_c = 0;
        field_14 = 0;
    }
    return 1;
}

// Receives the next frame for the local player: first any frame queued in a
// player's ring whose tick is due, otherwise a saved out-of-order frame or a new
// packet from HAPINET_receivepacket (growing the buffer on DPERR_BUFFERTOOSMALL).
// Sequenced packets (first dword not -1) are checked against the sender's last
// sequence number: an out-of-order frame is saved in the entry (and
// DPERR_NOMESSAGES returned), a gap is reported through the empty 0x4568b0, and a
// saved frame is swapped in when it can be used. The frame is then queued in the
// player's ring by 0x463790 and the next due frame is returned.
// FUNCTION: 0x462f30
int PacketReceiver::ReceiveFrame(void* net, unsigned char* data, int* size)
{
    int tick = g_game->tick;
    PlayerFrameInfo* entry;
    unsigned int i;
    void* src;
    int len;
    int rc;

    for (i = 0; i < 10; i++) {
        // Loop pointer, not entries[i]: keeps the found path from recomputing the address.
        PlayerFrameInfo* e = &entries[i];
        if (e->field_0 == -1)
            break;
        src = e->store.tail.GetFrame(tick, len);
        if (src != 0) {
            *(int*)((char*)net + 0x4b5) = e->store.tail.field_14;
            *(int*)((char*)net + 0x4b9) = e->store.tail.field_18;
            memcpy(data, src, len);
            *size = len;
            // Both copy-outs goto the one `ok: return 0;`.
            goto ok;
        }
    }

    entry = 0;
    if (length == 0) {
        int n = 0;
        if (field_14 != 0) {
            if (spare != 0) {
                buffer = spare;
                if (field_230 > 0) {
                    length = field_230;
                    field_c = field_234;
                    field_10 = field_238;
                    n = length - 4;
                    if (n > 0)
                        field_14->field_8 = *(int*)buffer;
                } else {
                    length = 0;
                }
                spare = 0;
                field_14->field_c = 0;
                field_14 = 0;
            } else if (field_14->field_c > 0) {
                spare = buffer;
                field_234 = field_c;
                field_230 = 0;
                field_238 = field_10;
                buffer = field_14->store.frame;
                field_14->field_8 = *(int*)buffer;
                length = field_14->field_c;
                field_c = field_14->field_0;
                field_10 = field_14->field_4;
                n = length - 4;
                field_14->field_c = 0;
            } else {
                field_14 = 0;
            }
        }
        if (n == 0) {
            length = capacity;
            rc = g_receiveCondenser.ReceivePacket((char*)g_game + 0x14, buffer, &length);
            // Plain while with no test of rc after it; each DPERR_NOMESSAGES exit is
            // its own `length = 0; return` block, not a shared goto label.
            while (rc != 0) {
                if (rc == (int)0x887700be) {    // DPERR_NOMESSAGES
                    length = 0;
                    return (int)0x887700be;
                }
                if (rc != (int)0x8877001e)      // DPERR_BUFFERTOOSMALL
                    goto error;
                // delete/new expressions, not operator delete/new (here and in the
                // out-of-order save): the operators shift the temporary rotation.
                delete buffer;
                capacity = length;
                length = 0;
                buffer = new char[capacity];
                if (buffer == 0) {
                    capacity = 0;
                    return (int)0x8007000e;
                }
                length = capacity;
                rc = g_receiveCondenser.ReceivePacket((char*)g_game + 0x14, buffer, &length);
            }
            // Always true here (the enclosing test), so MSVC emits no test; the error
            // block stays after B only as this if's else arm.
            if (n == 0) {
                field_c = *(int*)((char*)net + 0x4b5);
                field_10 = *(int*)((char*)net + 0x4b9);
                if (*(int*)((char*)net + 0x4b5) != 0) {
                    if ((unsigned int)length >= sizeof(int)) {
                        if (length == sizeof(int))
                            return (int)0x80004005;
                        if (*(int*)buffer != -1) {
                            entry = ((PacketReceiver*)this)->FindPlayerFrameInfo(field_c);
                            if (entry != 0) {
                                if (entry->field_8 != -1) {
                                    int prev = entry->field_8 - 1;
                                    int cur;
                                    int d;
                                    // cur and d are set in both arms: the copies merge after the join.
                                    if (prev >= -1) {
                                        prev = -2;
                                        cur = *(int*)buffer;
                                        d = prev - cur;
                                    } else {
                                        cur = *(int*)buffer;
                                        d = prev - cur;
                                    }
                                    int flag = entry->field_c > 0;
                                    if (d > 0) {
                                        if (entry->field_c <= 0) {
                                            // Out of order: save it in the entry.
                                            if (length > entry->field_10) {
                                                delete entry->store.frame;
                                                entry->store.frame = new char[length];
                                                if (entry->store.frame == 0) {
                                                    PacketTrace("no memory for allocating saved receive frame\n");
                                                    entry->field_10 = -1;
                                                    return (int)0x8007000e;
                                                }
                                                entry->field_10 = length;
                                            }
                                            memcpy(entry->store.frame, buffer, length);
                                            entry->field_4 = field_10;
                                            entry->field_0 = field_c;
                                            entry->field_c = length;
                                            length = 0;
                                            return (int)0x887700be;
                                        }
                                        prev = Prev_00462f30(entry->field_8);
                                        if (*(int*)entry->store.frame <= cur) {
                                            if (prev != cur)
                                                ReportPacketGap(field_c, prev, Next_00462f30(cur));
                                            int p2 = Prev_00462f30(*(int*)buffer);
                                            if (p2 != *(int*)entry->store.frame)
                                                ReportPacketGap(field_c, p2, Next_00462f30(*(int*)entry->store.frame));
                                            flag = 0;
                                            field_14 = entry;
                                        } else {
                                            if (prev != *(int*)entry->store.frame)
                                                ReportPacketGap(field_c, prev, Next_00462f30(*(int*)entry->store.frame));
                                            int p2 = Prev_00462f30(*(int*)entry->store.frame);
                                            if (p2 != *(int*)buffer)
                                                ReportPacketGap(field_c, p2, Next_00462f30(*(int*)buffer));
                                        }
                                    }
                                    if (flag && entry->field_c > 0) {
                                        // Swap the saved frame in, keep this one as the spare.
                                        spare = buffer;
                                        field_230 = length;
                                        field_234 = field_c;
                                        field_238 = field_10;
                                        buffer = entry->store.frame;
                                        length = entry->field_c;
                                        field_c = entry->field_0;
                                        field_10 = entry->field_4;
                                        field_14 = entry;
                                        entry->field_c = 0;
                                    }
                                }
                                entry->field_8 = *(int*)buffer;
                            } else {
                                return (int)0x80004005;
                            }
                        }
                    } else {
                        return (int)0x80004005;
                    }
                } else {
                    // From sender 0 (a system message): hand it out directly.
                    if (*size >= length) {
                        memcpy(data, buffer, length);
                        *size = length;
                        length = 0;
                        return 0;
                    } else {
                        *size = length;
                        return (int)0x8877001e;
                    }
                }
            } else {
            error:
                PacketTrace("HAPINET_receivepacket failed (%s)\n", HAPINET_GetDPErrorString(rc));
                length = 0;
                return rc;
            }
        }
    }

    if (entry == 0) {
        entry = ((PacketReceiver*)this)->FindPlayerFrameInfo(field_c);
        if (entry == 0) {
            length = 0;
            return (int)0x887700be;
        }
    }
    // Never taken (entry is not 0 here). C2 counts the store before jump threading
    // removes the arm, which gives tick the frame slot the original has (see the top).
    if (entry == 0)
        tick = 0;
    {
        // Peek in both arms and one Take after the join: one GetFrame there loses
        // the duplicated Peek.
        FrameQueue* tail = &entry->store.tail;
        Frame_00462f30* f;
        if (tail->QueueFrames(buffer, length, tick, field_c, field_10, field_14 == 0)) {
            length = 0;
            len = 0;
            f = tail->Peek();
        } else {
            len = 0;
            f = tail->Peek();
        }
        src = tail->Take(f, tick, len);
    }
    if (src != 0) {
        *(int*)((char*)net + 0x4b5) = entry->store.tail.field_14;
        *(int*)((char*)net + 0x4b9) = entry->store.tail.field_18;
        memcpy(data, src, len);
        *size = len;
        goto ok;
    }

    length = 0;
    return (int)0x887700be;
ok:
    return 0;
}
