// Decompiled by DeepSeek V4.1 Flash, Claude Opus 5.5, deepseek-v4.1-flash, space-bunny-free, Opus, Haiku, Sonnet, deepseek-v4.1, mimo-v2.6-pro, opus, GPT-6, fledge-alpha-free, Space Bunny Free and muse-spark-1.3-free. Names are provisional.
// The packet manager (the global g_packetManager): eleven per-player packet
// channels, the outgoing send buffer and a PacketReceiver, with the packet
// pools, rings and per-player frame queues they are built from.
//
// Layout of the manager: m_defaultSendPacingMs at +0x04 (the name comes from
// 0x461020's debug string), eleven 0x1044-byte per-player channels from +0x08,
// a {buffer, size, capacity} triple at +0xb2f4 and a PacketReceiver member at
// +0xb300 that is handed the owner.
#include <string.h>
#include <memory.h>
#include <windows.h>
#include <ddraw.h>
#include <new.h>
// Included only for their symbol ids: the functions below match at this count.
#include <setjmp.h>
#include <signal.h>

void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* ptr);
void __cdecl PacketTrace(const char* fmt, ...);
unsigned int GetTicks();
void __stdcall CountMessage(unsigned char type, int len, int which);
char* __stdcall HAPINET_GetDPErrorString(int error);
void __stdcall ReportPacketGap(int a, int b, int c);

class PacketBuffer;
class PacketChannel;

// A packet record: where its data sits in which buffer, and its queue state.
struct Packet {
    int frame;                         // +0x00
    int offset;                        // +0x04, into the buffer's data
    int size;                          // +0x08
    PacketBuffer* owner;               // +0x0c
    int queued;                        // +0x10, >= 0 while in the queue
    int sentTime;                      // +0x14, when it left the queue
    Packet* prev;                      // +0x18
    Packet* next;                      // +0x1c
};

class PacketBuffer {
public:
    PacketChannel* pool;               // +0x00
    int start;                         // +0x04, the index of its first packet
    int count;                         // +0x08
    int length;                        // +0x0c
    Packet* first;                     // +0x10
    unsigned char data[0x42a];         // +0x14

    int AppendPacket(Packet* packet, int index, const void* src, unsigned int size, Packet* prev);
    void FreePackets();
    void FindOwnerChainTail();
    int IsReusable(int minRetain);
};

// The ring of queued packets (0x462370 is its push, 0x4623b0 its pop).
class PacketRing {
public:
    int count;                         // +0x0
    int readIdx;                       // +0x4
    int writeIdx;                      // +0x8
    Packet* buf[0x400];                // +0xc

    PacketRing();
    int PushPacket(Packet* value);
    Packet* Peek()
    {
        if (count > 0)
            return buf[readIdx];
        return 0;
    }
    Packet* Pop()
    {
        if (count > 0) {
            count--;
            Packet* value = buf[readIdx];
            readIdx++;
            if (readIdx >= 0x400)
                readIdx = 0;
            return value;
        }
        return 0;
    }
    int Push(Packet* value)
    {
        if (count < 0x400) {
            writeIdx = writeIdx + 1;
            if (writeIdx >= 0x400) {
                writeIdx = 0;
            }
            buf[writeIdx] = value;
            count = count + 1;
            return 1;
        }
        return 0;
    }
};

// Read side of the ring, out of line (0x4623b0).
class Class_004623b0 {
public:
    int count;                         // +0x00
    int index;                         // +0x04
    int unused;                        // +0x08
    Packet* buffer[0x400];             // +0x0c

    Packet* PopPacket();
};

// Sets the minimum retain time of a channel (0x462860).
class Class_00462860 {
public:
    void SetMinRetainMs(unsigned int ms);
};

// Sets the ticks between sends of a channel (0x4628a0).
class Class_004628a0 {
public:
    void SetSendPacingMs(int ms);
};

class Class_00462ae0 {
public:
    PacketChannel* manager;            // +0x0
    char unknown_4[0xc];
    Packet* first;                     // +0x10

    void RemovePacket(Packet* packet);
};

// One queued frame of a player's ring: the tick it is due, the data and size.
struct Frame {
    int tick;                          // +0x0
    void* data;                        // +0x4
    int size;                          // +0x8
};

// The queued frames, a ring of 0x200 (0x180c bytes).
struct FrameRing {
    int count;                         // +0x0
    int head;                          // +0x4
    int tail;                          // +0x8
    Frame frames[0x200];               // +0xc

    FrameRing() { count = 0; head = 0; tail = -1; }

    Frame* Pop()
    {
        if (count > 0) {
            count--;
            Frame* f = &frames[head];
            if (++head >= 0x200)
                head = 0;
            return f;
        }
        return 0;
    }

    int Push(int tick, void* data, int size)
    {
        if (count >= 0x200)
            return 0;
        if (++tail >= 0x200)
            tail = 0;
        frames[tail].data = data;
        frames[tail].tick = tick;
        frames[tail].size = size;
        count++;
        return 1;
    }
};

// Bit reader, see src/network/net_stats.cpp.
class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int ReadBits(int bits);
};

// Length of each packet command, one word per 4-byte entry.
extern unsigned short g_packetSizes[][2];

class FrameQueue {
public:
    int field_0;                       // +0x00
    unsigned int field_4;              // +0x04
    int field_8;                       // +0x08
    char* field_c;                     // +0x0c
    FrameRing* buffer;                 // +0x10
    int field_14;                      // +0x14
    int field_18;                      // +0x18

    int Count() { return buffer ? buffer->count : 0; }

    ~FrameQueue();
    FrameQueue();
    int ResetFrames();
    int QueueFrames(char* src, unsigned int size, int tick, int a4, int a5, int a6);
    Frame* Peek()
    {
        if (buffer == 0 || buffer->count <= 0)
            return 0;
        return &buffer->frames[buffer->head];
    }
    // Pops the frame at the head if it is due (or if there is no tick).
    void* Take(Frame* f, int tick, int& size)
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

#pragma pack(push, 1)
struct GameEntry {
    int field_0;                       // +0x00
    int id;                            // +0x04
    char unknown_8[0x14b - 8];
};

struct Game {
    char unknown_0[0x14];
    char session[4];                   // +0x14
    char unknown_18[0x1b63 - 0x18];
    GameEntry players[10];             // +0x1b63
    char unknown_2851[0x2a3e - 0x2851];
    unsigned short tail;               // +0x2a3e
    unsigned short head;               // +0x2a40
    char unknown_2a42[0x38a47 - 0x2a42];
    int tick;                          // +0x38a47
};

class NetCondenser {
public:
    char unknown_0[0x21];
    int field_21;                      // +0x21

    void SendPacketTo(void* session, int from, int value, void* data, int size);
    void Accumulate(void* data, int size);
    int SendPacket(void* session, int from);
    int ReceivePacket(void* net, char* data, int* size);
};
#pragma pack(pop)

extern Game* g_game;
extern NetCondenser g_sendCondenser;
extern NetCondenser g_receiveCondenser;
extern int DAT_005129f1;
extern int g_usePacketManager;
extern int g_netFrameRateConfig;

// One player's packet channel: a pool of 0x43e-byte buffers, a pool of
// 0x20-byte packet records that point into them, and a ring of queued
// packets that SendQueued hands to the outgoing buffer.
class PacketChannel {
public:
    int field_0;                       // +0x00, the buffer last handed out
    unsigned int field_4;              // +0x04, ticks between sends
    PacketBuffer** buffers;            // +0x08
    unsigned int field_c;              // +0x0c, the buffer count
    int field_10;                      // +0x10, the frame number
    int field_14;                      // +0x14, the player's DPID
    unsigned int field_18;             // +0x18, the minimum retain time
    int field_1c;                      // +0x1c, the packet last handed out
    unsigned int field_20;             // +0x20, queued bytes
    unsigned int field_24;             // +0x24, the next send time
    Packet* packets;                   // +0x28
    unsigned int count;                // +0x2c, the packet count
    Packet* field_30;                  // +0x30, the first packet in use
    Packet* field_34;                  // +0x34, the last packet in use
    PacketRing queue;                  // +0x38

    PacketChannel();
    ~PacketChannel();
    PacketBuffer* AllocBuffer();
    Packet* AllocPacket(int param_1);
    int GetMinRetainMs();
    int GetPacketEntry(int param_1);
    int InitPools(int a1, unsigned int a2, int a3, int a4);
    void EnqueuePacket(Packet* item);
    int GrowPools(int growbufs, int growpackets);
    void DequeuePacket(Packet* item);
    void ResetChannel();
    int SendQueued(int force);
    int AddPacket(int param_1, void* param_2, unsigned int param_3);
};

class PacketReceiver {
public:
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

struct Msg_00461900 {
    char unknown_0[0x10];
    int field_10;                      // +0x10
    int field_14;                      // +0x14
};

class PacketManager {
public:
    virtual ~PacketManager();
    unsigned long m_defaultSendPacingMs;   // +0x04
    PacketChannel channels[11];        // +0x08
    unsigned char* buffer;             // +0xb2f4
    unsigned int size;                 // +0xb2f8
    unsigned int capacity;             // +0xb2fc
    PacketReceiver member;             // +0xb300

    PacketManager();
    int AppendToSendBuffer(unsigned char* data, unsigned int len);
    void ClearSendBuffer();
    void NopRet_D();
    PacketChannel* FindChannel(int param_1, int param_2);
    int InitChannels(int arg1, int arg2);
    void ReleaseChannel(int id);
    void ReleaseAllChannels();
    int SendAllQueued(int param_1);
    int SendFrameState(int from, Msg_00461900* msg);
    void QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4);
    void* QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4);
    void SetDefaultSendPacing(int rate);
};

// Defined in packets_460e20.cpp, whose dynamic initialiser (_$E4) and static
// destructor (packets_460f60.cpp, _$E2) need views of their own.
extern PacketManager g_packetManager;

// FUNCTION: 0x460f40
PacketRing::PacketRing()
{
    count = 0;
    readIdx = 0;
    writeIdx = -1;
}

// FUNCTION: 0x461020
int __stdcall InitPacketManager(int param_1, int param_2)
{
    if (g_netFrameRateConfig != 0) {
        g_packetManager.SetDefaultSendPacing(g_netFrameRateConfig);
    }
    if (g_usePacketManager != 0) {
        do {
            if (g_packetManager.member.buffer == 0) {
                if (g_packetManager.member.spare != 0) {
                    g_packetManager.member.buffer = g_packetManager.member.spare;
                    g_packetManager.member.spare = 0;
                } else {
                    g_packetManager.member.buffer = (char*)operator new(0x42a);
                    if (g_packetManager.member.buffer == 0) {
                        return 0;
                    }
                    g_packetManager.member.capacity = 0x42a;
                }
            }
            g_packetManager.member.length = 0;
            if (g_packetManager.member.field_14 != 0) {
                g_packetManager.member.field_14->pendingBytes = 0;
                g_packetManager.member.field_14 = 0;
            }
            g_packetManager.channels[0].InitPools(0, g_packetManager.m_defaultSendPacingMs, param_1, param_2);
            PacketChannel* p = &g_packetManager.channels[1];
            for (int i = 1; i < 11; i++, p++) {
                p->InitPools(-1, g_packetManager.m_defaultSendPacingMs, 2, 100);
            }
        } while (0);
        SetThreadPriority(GetCurrentThread(), -2);
    }
    return 1;
}

// An empty debug-print stub: callers pass a format string and values, but
// the release build compiled the body out.
// FUNCTION: 0x461170
void __cdecl PacketTrace(const char* fmt, ...)
{
}

// Storing the send result in a local first keeps `sete` (a direct
// `== 0 ? 1 : 0` on the call folds to neg/sbb/inc).
// FUNCTION: 0x461180
int __stdcall SendToDPID(int from, int to, void* data, int size)
{
    PacketTrace("bytes to send to (DPID)(%ld): %ld\n", to, size);
    void* session = g_game->session;
    DAT_005129f1 = to;
    g_sendCondenser.Accumulate(data, size);
    int result = ((NetCondenser*)&g_sendCondenser)->SendPacket(session, from);
    return result == 0 ? 1 : 0;
}

// FUNCTION: 0x4611e0
PacketManager::PacketManager() : m_defaultSendPacingMs(200), buffer(0), size(0), capacity(0), member(this)
{
}

// FUNCTION: 0x461340 ??_GPacketManager@@UAEPAXI@Z
// FUNCTION: 0x461420
PacketManager::~PacketManager()
{
}

// The send is the body of SendToDPID, inlined.
static inline int SendTo(int from, int to, void* data, int size)
{
    PacketTrace("bytes to send to (DPID)(%ld): %ld\n", to, size);
    void* session = g_game->session;
    DAT_005129f1 = to;
    g_sendCondenser.Accumulate(data, size);
    int result = ((NetCondenser*)&g_sendCondenser)->SendPacket(session, from);
    return result == 0 ? 1 : 0;
}

// The same reset as 0x462470.
static inline void Reset(PacketChannel* e)
{
    e->InitPools(-1, 0xc8, 2, 0x64);
    e->field_0 = -1;
    e->field_1c = -1;
    e->field_24 = 0;
}

// Appends len bytes to the net send buffer, growing it (by at least 0x200
// bytes) when needed, and accounts the message in the byte counters. The very
// first allocation reserves 4 extra bytes and starts the buffer with the
// one dword type word -2, which ClearSendBuffer confirms by setting size to 4
// when the buffer is non null.
// FUNCTION: 0x4614e0
int PacketManager::AppendToSendBuffer(unsigned char* data, unsigned int len)
{
    if (size + len > capacity) {
        unsigned int newcap = capacity + (len > 0x200 ? len : 0x200);
        if (buffer == 0) {
            newcap += 4;
        }
        unsigned char* newbuf = new unsigned char[newcap];
        if (newbuf != 0) {
            if (buffer != 0) {
                memcpy(newbuf, buffer, size);
            } else {
                size = 4;
                *(int*)newbuf = -2;
            }
            delete[] buffer;
            buffer = newbuf;
            capacity = newcap;
        } else {
            return 0;
        }
    }
    memcpy(buffer + size, data, len);
    size += len;
    CountMessage(*data, len, 1);
    return 1;
}

// FUNCTION: 0x4615f0
void PacketManager::ClearSendBuffer()
{
    size = (buffer == 0 ? 0 : 4);
}

// An empty method, called once (from 0x4161f0) on the global g_packetManager.
// FUNCTION: 0x461610
void PacketManager::NopRet_D()
{
}

// An empty method, called once (from 0x453d40) on the global g_packetManager,
// like its neighbour 0x461610.
class Class_00461620 {
public:
    void HandleIntegrityNop(int, int, int);
};

// FUNCTION: 0x461620
void Class_00461620::HandleIntegrityNop(int, int, int)
{
}

// Finds the entry (of the 11 inside this) whose field_14 is param_1 and returns
// it. When there is none, param_2 decides whether a new entry may be made:
// first an unused slot (field_14 == -1), otherwise the first slot whose
// field_14 is not the color of any of the ten player slots in g_game. The
// chosen entry is re-initialised with InitPools and returned.
// FUNCTION: 0x461630
PacketChannel* PacketManager::FindChannel(int param_1, int param_2)
{
    unsigned i;
    for (i = 0; i <= 10; i++) {
        if (channels[i].field_14 == param_1)
            return &channels[i];
    }
    if (param_2 == 0)
        return 0;
    for (i = 0; i <= 10; i++) {
        if (channels[i].field_14 == -1) {
            channels[i].InitPools(param_1, m_defaultSendPacingMs, 2, 0x64);
            return &channels[i];
        }
    }
    for (i = 1; i <= 10; i++) {
        int used = 0;
        // Must walk a pointer here; the outer loops stay plain array indexing.
        GameEntry* p = &g_game->players[0];
        for (int j = 0; j < 10; j++, p++) {
            if (p->id == channels[i].field_14) {
                used = 1;
                break;
            }
        }
        if (used == 0) {
            channels[i].InitPools(param_1, m_defaultSendPacingMs, 2, 0x64);
            return &channels[i];
        }
    }
    return 0;
}

// FUNCTION: 0x461750
int PacketManager::InitChannels(int arg1, int arg2)
{
    if (g_usePacketManager == 0) {
        return 0;
    }
    // The do/while (0) must end before SetThreadPriority; the failure return
    // stays inside it.
    do {
        if (member.buffer == 0) {
            if (member.spare != 0) {
                member.buffer = member.spare;
                member.spare = 0;
            } else {
                member.buffer = (char*)operator new(0x42a);
                if (member.buffer == 0) {
                    return 0;
                }
                member.capacity = 0x42a;
            }
        }
        member.length = 0;
        if (member.field_14 != 0) {
            member.field_14->pendingBytes = 0;
            member.field_14 = 0;
        }
        channels[0].InitPools(0, m_defaultSendPacingMs, arg1, arg2);
        PacketChannel* e = channels + 1;
        int n = 10;
        do {
            e->InitPools(-1, m_defaultSendPacingMs, 2, 100);
            e++;
        } while (--n);
    } while (0);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
    return 1;
}

// Looks up an entry and resets it the same way as 0x462470.
// FUNCTION: 0x461820
void PacketManager::ReleaseChannel(int id)
{
    PacketChannel* e = FindChannel(id, 0);
    if (e != 0) {
        e->InitPools(-1, 0xc8, 2, 0x64);
        e->field_0 = -1;
        e->field_1c = -1;
        e->field_24 = 0;
    }
}

// Resets all 11 entries the same way as 0x462470 (inlined here).
// FUNCTION: 0x461860
void PacketManager::ReleaseAllChannels()
{
    for (int i = 0; i < 11; i++) {
        Reset(&channels[i]);
    }
}

// FUNCTION: 0x4618a0
int PacketManager::SendAllQueued(int param_1)
{
    if (g_usePacketManager == 0) {
        return 0;
    }
    for (unsigned int i = 0; i <= 10; i++) {
        if (channels[i].field_14 != -1) {
            if (channels[i].SendQueued(param_1) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

// Puts one dword (the message's field_10, or -1 when field_14 is set) in the
// send buffer and sends it; the send is the body of SendToDPID, inlined.
// FUNCTION: 0x461900
int PacketManager::SendFrameState(int from, Msg_00461900* msg)
{
    *(int*)buffer = msg->field_14 ? -1 : msg->field_10;
    SendTo(from, msg->field_14, buffer, size);
    size = buffer ? 4 : 0;
    return 1;
}

// A method of the global g_packetManager (its one caller, 0x451df0, sets ecx to
// it) that ignores `this` and forwards to 0x462710 on the object it is
// given, which that caller passes as &g_packetManager + 8.
// FUNCTION: 0x461990
void PacketManager::QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4)
{
    int a = param_4;
    int b = param_3;
    int c = param_1;
    param_2->AddPacket(c, (void*)b, a);
}

// FUNCTION: 0x4619b0
void* PacketManager::QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4)
{
    PacketChannel* obj = FindChannel(param_2, 1);
    if (obj != 0) {
        return (void*)obj->AddPacket(param_1, param_3, param_4);
    }
    return 0;
}

// FUNCTION: 0x4619e0
void PacketManager::SetDefaultSendPacing(int rate)
{
    if (rate < 0) {
        g_usePacketManager = 0;
        return;
    }
    if (rate == 0) {
        m_defaultSendPacingMs = 200;
    } else {
        if (rate < 2) {
            rate = 2;
        } else if (rate > 30) {
            rate = 30;
        }
        m_defaultSendPacingMs = 1000 / rate;
    }
    PacketTrace("setting m_defaultSendPacingMs to: %lums\n", m_defaultSendPacingMs);
    for (int i = 0; i < 11; i++) {
        channels[i].field_4 = (m_defaultSendPacingMs * 30 + 999) / 1000;
    }
}

// One `return` per outcome: that gives the original's un-rotated scan loop
// and its block order.
static inline int IsReusable(PacketBuffer* buf, int minRetain)
{
    if (buf->count == 0)
        return 1;
    Packet* p = buf->first;
    unsigned int now = GetTicks();
    int mustBeSentBefore = now - minRetain;
    PacketTrace("cur game time: %ld, min retain=%ld, mustBeSentBefore=%ld\n",
                 now, minRetain, mustBeSentBefore);
    for (; p != 0; p = p->next) {
        if (p->owner != buf)
            break;
        if (p->queued >= 0)
            return 0;
        if (p->sentTime > mustBeSentBefore && p->sentTime <= now)
            return 0;
    }
    PacketTrace("buffer eligible for reuse:  all packets have been sent more than %lu game ticks ago\n",
                 minRetain);
    return 1;
}

// FUNCTION: 0x461a70
PacketChannel::PacketChannel()
    : field_0(-1), field_4(0), buffers(0), field_c(0), field_10(-2), field_14(-1), field_18(0),
      field_1c(-1), field_20(0), field_24(0), packets(0), count(0), field_30(0), field_34(0)
{
    ((Class_00462860*)this)->SetMinRetainMs(4000);
    ((Class_004628a0*)this)->SetSendPacingMs(200);
}

// Destructor of the class built by 0x461a70: frees each block of the pointer
// array at +0x8 (count at +0xc), the array itself, then the packets at +0x28.
// FUNCTION: 0x461ac0
PacketChannel::~PacketChannel()
{
    if (buffers) {
        for (unsigned int i = 0; i < field_c; i++) {
            delete buffers[i];
        }
        delete buffers;
    }
    delete packets;
}

// FUNCTION: 0x461b10
PacketBuffer* PacketChannel::AllocBuffer()
{
    for (;;) {
        if (field_c > 0) {
            unsigned int ix = field_0;
            field_0 = ix;
            ++ix;
            if (ix >= field_c)
                ix = 0;
            PacketBuffer* buf = buffers[ix];
            if (IsReusable(buf, field_18)) {
                buf->FreePackets();
                field_0 = ix;
                return buf;
            }
            if (field_c >= 0x22) {
                buf->FreePackets();
                field_0 = ix;
                PacketTrace("force-allocated a previously-used buffer, ix=%ld\n", ix);
                return buf;
            }
        }
        if (GrowPools(0x10, 0x320) == 0)
            return 0;
    }
}

// FUNCTION: 0x461c20
Packet* PacketChannel::AllocPacket(int param_1)
{
    unsigned int idx;
    Packet* packet;
    for (;;) {
        PacketTrace("current packet pool index: %ld\n", field_1c);
        idx = field_1c + 1;
        if (idx >= count)
            idx = 0;
        packet = &packets[idx];
        PacketBuffer* owner = packet->owner;
        if (owner == 0)
            break;
        if (IsReusable(owner, field_18)) {
            owner->FreePackets();
            break;
        }
        if (count >= 0x6a4) {
            PacketTrace("force-initializing a in-use buffer which was not eligible for reuse!\n");
            owner->FreePackets();
            break;
        }
        GrowPools(0, 0x320);
    }
    field_1c = idx;
    PacketTrace("current packet pool index set to: %ld\n", idx);
    packet->frame = param_1;
    return packet;
}

// FUNCTION: 0x461d60
void __stdcall NopRetC_B(int, int, int)
{
}

// FUNCTION: 0x461d70
int __fastcall GetCurrentBuffer(int *ecx)
{
    int val = ecx[0];
    int result = 0;
    if (val >= 0) {
        result = ((int*)ecx[2])[val];
    }
    return result;
}

// FUNCTION: 0x461d80
int PacketChannel::GetMinRetainMs()
{
    unsigned int val = field_18;
    return (val / 30) * 1000;
}

// FUNCTION: 0x461da0
int PacketChannel::GetPacketEntry(int param_1)
{
    return param_1 * 0x20 + (int)packets;
}

// The fresh pool entries get only dwords 1..7 of each 0x20-byte record
// initialised (0x461e11), and the reset copy at the end copies a stack
// template whose first dword is never written (0x461ee8 writes 0x4614..0x462c,
// the copy starts at 0x4610), so a new pool's first dword stays whatever
// operator new returned. It does not matter because nothing in this function
// reads it. Also `q->count = 0` before FreePackets() (0x461ecb) is redundant:
// that method sets count to 0 itself (0x4629b0).
// FUNCTION: 0x461db0
int PacketChannel::InitPools(int a1, unsigned int a2, int a3, int a4)
{
    unsigned int i;
    unsigned int j;
    int k;
    int* p;
    field_14 = a1;
    field_4 = (a2 * 30 + 999) / 1000;
    if (packets == 0) {
        if (a4 == 0)
            a4 = 100;
        Packet* p = (Packet*)operator new(a4 * 32);
        if (p) {
            // A countdown over a separate walking pointer: this spelling is
            // what gives `lea edx,[esi-1] / cmp / jl` and `inc edx` at 0x461e06.
            k = a4 - 1;
            Packet* q = p;
            while (k >= 0) {
                q->offset = 0;
                q->size = 0;
                q->owner = 0;
                q->queued = -1;
                q->sentTime = 0;
                q->prev = 0;
                q->next = 0;
                q++;
                k--;
            }
        } else {
            p = 0;
        }
        packets = p;
        if (p == 0)
            goto fail;
        count = a4;
    }
    if (buffers == 0) {
        if (a3 == 0)
            a3 = 2;
        buffers = (PacketBuffer**)operator new(a3 * 4);
        if (buffers == 0)
            goto fail;
        for (field_c = 0; field_c < a3; field_c++) {
            PacketBuffer* q = (PacketBuffer*)operator new(0x43e);
            if (q) {
                q->pool = this;
                q->start = -1;
                q->count = 0;
                q->length = 0;
                q->first = 0;
            } else {
                q = 0;
            }
            buffers[field_c] = q;
            if (buffers[field_c] == 0)
                goto fail;
        }
    }
    for (i = 0; i < field_c; i++) {
        // The pointer local is what puts the store in eax and the call in ecx.
        PacketBuffer* q = buffers[i];
        q->count = 0;
        buffers[i]->FreePackets();
    }
    Packet t;
    t.offset = 0;
    t.size = 0;
    t.owner = 0;
    t.queued = -1;
    t.sentTime = 0;
    t.prev = 0;
    t.next = 0;
    for (j = 0; j < count; j++)
        packets[j] = t;
    field_0 = -1;
    field_1c = -1;
    field_30 = 0;
    field_34 = 0;
    // p keeps the latch's test a memory operand, so the body's own test at
    // the top of the loop reloads queue.count (0x461f43).
    p = &queue.count;
    if (queue.count != 0) {
        do {
            if (queue.count > 0) {
                queue.count--;
                queue.readIdx++;
                if (queue.readIdx >= 0x400)
                    queue.readIdx = 0;
            }
        } while (*p != 0);
    }
    field_20 = 0;
    return 1;
    // Single shared `return 0` via goto; counters sit at function scope so no
    // jump skips an initialiser.
fail:
    return 0;
}

// Queues an item in the ring buffer at +0x38 (the inlined push of
// 0x462370), marks it queued and adds its size to the total.
// FUNCTION: 0x461f90
void PacketChannel::EnqueuePacket(Packet* item)
{
    queue.Push(item);
    item->queued = 0;
    field_20 += item->size;
}

// DequeuePacket as GrowPools and RemovePacket expand it: the ring's pop and push
// stay calls (0x4623b0 and 0x462370), where 0x4623e0 inlines them.
static inline void DequeueByCalls(PacketChannel* channel, Packet* item)
{
    item->queued = -1;
    item->sentTime = GetTicks();
    int n = channel->queue.count;
    while (n-- > 0) {
        Packet* value = ((Class_004623b0*)&channel->queue)->PopPacket();
        if (value == item)
            break;
        channel->queue.PushPacket(value);
    }
    channel->field_20 -= item->size;
}

// Grows both pools of a channel: a new buffer-pointer array of `growbufs` more
// buffers and a new packet array of `growpackets` more packets, then moves every
// packet that still belongs to a buffer into the new packet array, re-queueing
// the pending ones (the inlined 0x4623e0 and 0x461f90) and relinking the
// in-use list.
// FUNCTION: 0x461fd0
int PacketChannel::GrowPools(int growbufs, int growpackets)
{
    PacketTrace("current buffer pool count: %d, current packet pool count: %d\n",
                 field_c, count);
    PacketTrace("bufs to grow by: %d, packets to grow by: %d\n", growbufs, growpackets);
    PacketTrace("current buffer pool ix: %d, current packet pool ix: %d\n",
                 field_0, field_1c);
    if (field_c <= 0 || count <= 0) {
        return 1;
    }

    // operator new/delete, not new[]: new[] references ??_U / ??_V instead.
    int total = field_c + growbufs;
    PacketBuffer** np = (PacketBuffer**)operator new(total * 4);
    if (np) {
        memset(np, 0, total * 4);
        if (field_0 >= 0) {
            int i = 0;
            int ix = field_0;
            while (i < field_c) {
                ix = ix + 1;
                if (ix >= field_c) {
                    ix = 0;
                }
                np[i] = buffers[ix];
                i = i + 1;
            }
        } else {
            memcpy(np, buffers, field_c * 4);
        }
        field_0 = field_c - 1;
        int ok = 1;
        while (1) {
            if (field_c >= (unsigned int)total) {
                break;
            }
            PacketBuffer* q = (PacketBuffer*)operator new(0x43e);
            if (q) {
                q->pool = this;
                q->start = -1;
                q->count = 0;
                q->length = 0;
                q->first = 0;
            } else {
                q = 0;
            }
            np[field_c] = q;
            ok = (np[field_c] != 0);
            field_c = field_c + 1;
            if (!ok) {
                break;
            }
        }
        operator delete(buffers);
        buffers = np;
        if (ok) {
        int totalentries = growpackets + count;
        Packet* ne = (Packet*)operator new(totalentries * 32);
        Packet* base;
        if (ne) {
            Packet* q = ne;
            for (int i = 0; i <= totalentries - 1; i++) {
                q->offset = 0;
                q->size = 0;
                q->owner = 0;
                q->queued = -1;
                q->sentTime = 0;
                q->prev = 0;
                q->next = 0;
                q++;
            }
            base = ne;
        } else {
            base = 0;
        }
        if (base) {
        int n = 0;
        int j;
        if (field_0 >= 0) {
            field_34 = 0;
            field_30 = 0;
            for (int i = 0; i < field_c; i++) {
                PacketBuffer* p = buffers[i];
                if (p->count > 0) {
                    Packet* c = p->first;
                    if (c != 0) {
                        p->start = n;
                        j = 0;
                        p->first = &base[n];
                        while (c != 0) {
                            if (c->owner != p) {
                                break;
                            }
                            Packet* e = &base[n];
                            n++;
                            *e = *c;
                            if (c->queued >= 0) {
                                DequeueByCalls(this, c);
                                EnqueuePacket(e);
                            }
                            e->prev = field_34;
                            e->owner = p;
                            e->next = 0;
                            // Both arms store tail = e: gives e the register weight the
                            // allocation needs; the stores are merged again.
                            if (field_34) {
                                field_34->next = e;
                                field_34 = e;
                            } else {
                                field_34 = e;
                            }
                            if (!field_30) {
                                field_30 = e;
                            }
                            // One j++ in the latch: keeps its temporary in esi.
                            j++;
                            c = c->next;
                        }
                        p->count = j;
                    } else {
                        p->start = -1;
                        p->count = 0;
                    }
                }
            }
        }
        operator delete(packets);
        packets = base;
        count = totalentries;
        field_1c = n - 1;
        PacketTrace("current packet pool index set to: %ld\n", n - 1);
        return 1;
        }
        }
    }
    return 0;
}

// The original never inlined PushPacket and PopPacket (GrowPools, RemovePacket
// and DequeuePacket's inlined copies call them out of line).
#pragma auto_inline(off)
// FUNCTION: 0x462370
int PacketRing::PushPacket(Packet* value)
{
    if (count < 0x400) {
        writeIdx = writeIdx + 1;
        if (writeIdx >= 0x400) {
            writeIdx = 0;
        }
        buf[writeIdx] = value;
        count = count + 1;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4623b0
Packet* Class_004623b0::PopPacket()
{
    if (count > 0) {
        count--;
        Packet* value = buffer[index];
        index++;
        if (index < 0x400) {
            return value;
        }
        index = 0;
        return value;
    }
    return 0;
}
#pragma auto_inline(on)

// FUNCTION: 0x4623e0
void PacketChannel::DequeuePacket(Packet* item)
{
    item->queued = -1;
    item->sentTime = GetTicks();
    int n = queue.count;
    while (n-- > 0) {
        Packet* value = queue.Pop();
        if (value == item)
            break;
        queue.Push(value);
    }
    field_20 -= item->size;
}

// FUNCTION: 0x462470
void PacketChannel::ResetChannel()
{
    InitPools(-1, 0xc8, 2, 0x64);
    field_0 = -1;
    field_1c = -1;
    field_24 = 0;
}

// Sends one player's queued packets: every packet whose frame matches the
// queue head's is appended to the outgoing buffer (g_packetManager's buffer
// and size), the others go back on the queue, and the buffer is handed to
// g_sendCondenser with the frame and the player's DPID.
//
// A packet's data is `owner->data[offset]`: +0x4 is the offset and +0xc the
// owning buffer (PacketBuffer, data at +0x14).
// FUNCTION: 0x4624a0
int PacketChannel::SendQueued(int force)
{
    unsigned int now = GetTicks();
    PacketTrace("player: %ld, ticks betw sends=%lu, nextsend=%lu, gametimereal=%lu\n",
                 field_14, field_4, field_24, now);
    // Keep this nesting with the trailing `return 1`: it gives the three epilogues.
    if (now >= field_24 || force != 0) {
        field_24 = now + field_4;
        int n;
        while ((n = queue.count) != 0) {
            int headFrame = queue.Peek()->frame;
            int sent = 0;
            PacketTrace("assigning packets to frame number: %ld\n", field_10);
            for (int i = 0; i < n; i++) {
                Packet* entry = queue.Peek();
                queue.Pop();
                if (entry->frame == headFrame) {
                    // Keep owner->data[offset] written out as an expression each time.
                    PacketTrace("extracted packet (len=%ld, type=%d, data=\"%s\")\n",
                                 entry->size, entry->owner->data[entry->offset],
                                 &entry->owner->data[entry->offset + 1]);
                    entry->queued = field_10;
                    entry->sentTime = GetTicks();
                    if (g_packetManager.AppendToSendBuffer(&entry->owner->data[entry->offset], entry->size) == 0)
                        return 0;
                    sent++;
                } else {
                    queue.Push(entry);
                }
            }
            field_20 = 0;
            if (sent > 0) {
                PacketTrace("sending %ld packets in frame: %ld\n", sent, field_10);
                *(int*)g_packetManager.buffer = field_14 != 0 ? -1 : field_10;
                // Read in this order (nbytes, id, data) before the log call.
                unsigned int nbytes = g_packetManager.size;
                int id = field_14;
                unsigned char* data = g_packetManager.buffer;
                PacketTrace("bytes to send to (DPID)(%ld): %ld\n", id, nbytes);
                g_sendCondenser.SendPacketTo(g_game->session, headFrame, id, data, nbytes);
                g_packetManager.size = g_packetManager.buffer != 0 ? 4 : 0;
                field_10--;
                if (field_10 >= -1)
                    field_10 = -2;
                if (queue.count == 0)
                    return 1;
            }
        }
    }
    return 1;
}

// The original called this out of line from SendQueued.
#pragma auto_inline(off)
// FUNCTION: 0x4626e0
void NetCondenser::SendPacketTo(void* session, int from, int value, void* data, int size)
{
    field_21 = value;
    ((NetCondenser*)this)->Accumulate(data, size);
    ((NetCondenser*)this)->SendPacket(session, from);
}
#pragma auto_inline(on)

// FUNCTION: 0x462710
int PacketChannel::AddPacket(int param_1, void* param_2, unsigned int param_3)
{
    if (field_20 >= 0x42a || queue.count == 0x400) {
        SendQueued(1);
        if (queue.count != 0)
            return 0;
    }
    Class_00462ae0* block = 0;
    int idx = field_0;
    if (idx >= 0)
        block = (Class_00462ae0*)buffers[idx];
    if (block == 0) {
        block = (Class_00462ae0*)AllocBuffer();
        if (block == 0)
            return 0;
    }
    Packet* pkt = AllocPacket(param_1);
    if (pkt == 0)
        return 0;
    int r = ((PacketBuffer*)block)->AppendPacket(pkt, field_1c, param_2, param_3, field_34);
    if (r == 0) {
        field_1c = field_1c - 1;
        block = (Class_00462ae0*)AllocBuffer();
        if (block != 0) {
            pkt = AllocPacket(param_1);
            if (pkt != 0)
                r = ((PacketBuffer*)block)->AppendPacket(pkt, field_1c, param_2, param_3, field_34);
            else
                r = 0;
        }
    }
    if (r != 0) {
        if (field_30 == 0)
            field_30 = pkt;
        if (field_34 != 0)
            field_34->next = pkt;
        field_34 = pkt;
        queue.Push(pkt);
        pkt->queued = 0;
        field_20 += pkt->size;
        return 1;
    }
    if (block != 0 && pkt->owner == (PacketBuffer*)block)
        block->RemovePacket(pkt);
    return 0;
}

// Clamps a time in milliseconds to 4000..60000 and stores it converted to
// 30 Hz ticks, rounded up (compare 0x4628a0).
// FUNCTION: 0x462860
void Class_00462860::SetMinRetainMs(unsigned int ms)
{
    if (ms > 60000)
        ms = 60000;
    else if (ms < 4000)
        ms = 4000;
    ((PacketChannel*)this)->field_18 = (ms * 30 + 999) / 1000;
}

// FUNCTION: 0x4628a0
void Class_004628a0::SetSendPacingMs(int param_1)
{
    unsigned int v = param_1 * 30 + 999;
    ((PacketChannel*)this)->field_4 = v / 1000u;
}

// Appends `size` bytes at `src` to the buffer's inline storage (at +0x14),
// fills in the output packet `p`, and bumps the buffer's packet count.
// `prev` is the previously queued packet (0x462710 passes its tail), stored
// in the packet's prev field at +0x18.
// FUNCTION: 0x4628d0
int PacketBuffer::AppendPacket(Packet* p, int index, const void* src, unsigned int size, Packet* prev)
{
    PacketTrace("adding packet %ld (data=\"%s\")\n", index,
                 (const char*)src + 1);
    PacketTrace("current buffer length: %ld, toadd=%ld, max=%ld\n", length,
                 size, 0x42a);
    if (length + size <= 0x42a) {
        memcpy(data + length, src, size);
        // The dead store `length = t` must stay: it keeps t a memory reload.
        int t = length;
        length = t;
        p->offset = t;
        p->owner = this;
        p->size = size;
        p->prev = prev;
        p->next = 0;
        length += size;
        if (count++ == 0) {
            start = index;
            PacketTrace("set first packet ix to: %ld\n", index);
            first = p;
        }
        PacketTrace("assigned packet count this buf: %ld\n", count);
        return 1;
    }
    PacketTrace("out of space in buffer!\n");
    return 0;
}

// Frees the `count` packets assigned from index `start` (wrapping at the
// pool size), removing each from its owning queue, then resets the range.
// FUNCTION: 0x4629b0
void PacketBuffer::FreePackets()
{
    int n = count;
    if (n > 0) {
        unsigned int size = pool->count;
        PacketTrace("freeing packets assigned. range: %ld for %ld\n", start, n);
        unsigned int i = start;
        Packet* packets = pool->packets;
        while (n--) {
            PacketTrace("initialize freeing packet %ld\n", i);
            Packet* p = &packets[i];
            i++;
            ((Class_00462ae0*)p->owner)->RemovePacket(p);
            p->owner = 0;
            if (i >= size)
                i = 0;
        }
    }
    length = 0;
    count = 0;
    start = -1;
    first = 0;
}

// FUNCTION: 0x462a40
void PacketBuffer::FindOwnerChainTail()
{
    if (count != 0) {
        Packet* p = first;
        while (p != 0 && p->owner == this) {
            p = p->next;
        }
    }
}

// Returns 1 when the buffer can be reused: none of its packets is still
// queued, and none was sent within the last `minRetain` game ticks.
// FUNCTION: 0x462a60
int PacketBuffer::IsReusable(int minRetain)
{
    if (count) {
        Packet* p = first;
        unsigned int now = GetTicks();
        int mustBeSentBefore = now - minRetain;
        PacketTrace("cur game time: %ld, min retain=%ld, mustBeSentBefore=%ld\n",
                     now, minRetain, mustBeSentBefore);
        for (; p && p->owner == this; p = p->next) {
            if (p->queued >= 0)
                return 0;
            if (p->sentTime > mustBeSentBefore && p->sentTime <= now)
                return 0;
        }
        PacketTrace("buffer eligible for reuse:  all packets have been sent more than %lu game ticks ago\n",
                     minRetain);
    }
    return 1;
}

// FUNCTION: 0x462ae0
void Class_00462ae0::RemovePacket(Packet* packet)
{
    PacketTrace("removing packet (len=%ld, type=%d, data=\"%s\")\n",
                 packet->size,
                 *(unsigned char*)((char*)packet->offset + (int)packet->owner + 0x14),
                 (char*)packet->offset + (int)packet->owner + 0x15);

    if (packet->queued >= 0) {
        PacketTrace("Warning! RemovePacket called for packet in pending queue!\n");
        PacketChannel* mgr = manager;
        packet->queued = -1;
        packet->sentTime = GetTicks();
        int n = mgr->queue.count;
        while (n-- > 0) {
            Packet* value = ((Class_004623b0*)&mgr->queue)->PopPacket();
            if (value == packet)
                break;
            mgr->queue.PushPacket(value);
        }
        mgr->field_20 -= packet->size;
    }

    if (first == packet) {
        Packet* nxt = packet->next;
        if (nxt != 0 && nxt->owner == (PacketBuffer*)this)
            first = nxt;
        else
            first = 0;
    }

    PacketChannel* m = manager;
    if (m->field_34 == packet)
        m->field_34 = packet->prev;
    if (m->field_30 == packet)
        m->field_30 = packet->next;
    if (packet->prev != 0)
        packet->prev->next = packet->next;
    if (packet->next != 0) {
        packet->next->prev = packet->prev;
        packet->next = 0;
    }
    packet->prev = 0;
}

#pragma pack(push, 1)
class Class_00462bd0 {
public:
    char unknown_0[0xc];
    void* field_c;

    void RemoveFromBuffer();
};
#pragma pack(pop)

// FUNCTION: 0x462bd0
void Class_00462bd0::RemoveFromBuffer()
{
    ((Class_00462ae0*)field_c)->RemovePacket((Packet*)this);
    field_c = 0;
}

struct Class_00462bf0 {
    char unknown_0[4];
    int field_0x4;
    char unknown_8[4];
    int field_0xc;

    int GetData();
};

// FUNCTION: 0x462bf0
int Class_00462bf0::GetData()
{
    int eax = this->field_0x4;
    int ecx = this->field_0xc;
    return eax + ecx + 0x14;
}

// The real constructor of PacketReceiver (vtable 0x4fd518, scalar deleting
// destructor 0x462cc0, destructor 0x462d30).
// FUNCTION: 0x462c00
PacketReceiver::PacketReceiver(void* o)
    : field_4(0), owner(o), field_c(-1), field_10(-1), field_14(0), buffer(0), spare(0),
      capacity(0), length(0), field_230(0), field_234(-1), field_238(-1)
{
}

// The destructor: frees one of two buffers; the ten entries are then
// destroyed in reverse order, each freeing its three buffers.
// FUNCTION: 0x462cc0 ??_GPacketReceiver@@UAEPAXI@Z
// FUNCTION: 0x462d30
PacketReceiver::~PacketReceiver()
{
    if (spare)
        operator delete(spare);
    else
        operator delete(buffer);
}

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

// Looks up (or creates) the PlayerFrameInfo entry for an id in a table of ten
// of them: an entry is 0x34 bytes at this+0x20, with a three-int head (the
// debug string gives PlayerFrameInfo away) and a tail holding three zeroed
// ints, a buffer pointer and two -1s.
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
        if (e->playerNetId == id)
            return e;
        if (e->playerNetId == -1)
            break;
    }
    if (i < 10) {
        for (; i < 10; i++) {
            // Separate from e: else the test is strength-reduced and the second
            // call site loses its recomputed address.
            PlayerFrameInfo* p = &entries[i];
            if (p->playerNetId == -1) {
                entries[i].Initialize(id);
                e = &entries[i];
                return e;
            }
        }
    }
    for (i = 0; i < 10; i++) {
        int used = 0;
        for (j = 0; j < 10; j++) {
            if (g_game->players[j].id == entries[i].playerNetId) {
                used = 1;
                break;
            }
        }
        if (used)
            continue;
        e = &entries[i];
        e->Initialize(id);
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
        field_14->pendingBytes = 0;
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
        if (e->playerNetId == -1)
            break;
        src = e->tail.GetFrame(tick, len);
        if (src != 0) {
            *(int*)((char*)net + 0x4b5) = e->tail.field_14;
            *(int*)((char*)net + 0x4b9) = e->tail.field_18;
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
                        field_14->frameSeq = *(int*)buffer;
                } else {
                    length = 0;
                }
                spare = 0;
                field_14->pendingBytes = 0;
                field_14 = 0;
            } else if (field_14->pendingBytes > 0) {
                spare = buffer;
                field_234 = field_c;
                field_230 = 0;
                field_238 = field_10;
                buffer = field_14->frame;
                field_14->frameSeq = *(int*)buffer;
                length = field_14->pendingBytes;
                field_c = field_14->playerNetId;
                field_10 = field_14->pendingDpToId;
                n = length - 4;
                field_14->pendingBytes = 0;
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
                                if (entry->frameSeq != -1) {
                                    int prev = entry->frameSeq - 1;
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
                                    int flag = entry->pendingBytes > 0;
                                    if (d > 0) {
                                        if (entry->pendingBytes <= 0) {
                                            // Out of order: save it in the entry.
                                            if (length > entry->pendingCap) {
                                                delete entry->frame;
                                                entry->frame = new char[length];
                                                if (entry->frame == 0) {
                                                    PacketTrace("no memory for allocating saved receive frame\n");
                                                    entry->pendingCap = -1;
                                                    return (int)0x8007000e;
                                                }
                                                entry->pendingCap = length;
                                            }
                                            memcpy(entry->frame, buffer, length);
                                            entry->pendingDpToId = field_10;
                                            entry->playerNetId = field_c;
                                            entry->pendingBytes = length;
                                            length = 0;
                                            return (int)0x887700be;
                                        }
                                        prev = Prev_00462f30(entry->frameSeq);
                                        if (*(int*)entry->frame <= cur) {
                                            if (prev != cur)
                                                ReportPacketGap(field_c, prev, Next_00462f30(cur));
                                            int p2 = Prev_00462f30(*(int*)buffer);
                                            if (p2 != *(int*)entry->frame)
                                                ReportPacketGap(field_c, p2, Next_00462f30(*(int*)entry->frame));
                                            flag = 0;
                                            field_14 = entry;
                                        } else {
                                            if (prev != *(int*)entry->frame)
                                                ReportPacketGap(field_c, prev, Next_00462f30(*(int*)entry->frame));
                                            int p2 = Prev_00462f30(*(int*)entry->frame);
                                            if (p2 != *(int*)buffer)
                                                ReportPacketGap(field_c, p2, Next_00462f30(*(int*)buffer));
                                        }
                                    }
                                    if (flag && entry->pendingBytes > 0) {
                                        // Swap the saved frame in, keep this one as the spare.
                                        spare = buffer;
                                        field_230 = length;
                                        field_234 = field_c;
                                        field_238 = field_10;
                                        buffer = entry->frame;
                                        length = entry->pendingBytes;
                                        field_c = entry->playerNetId;
                                        field_10 = entry->pendingDpToId;
                                        field_14 = entry;
                                        entry->pendingBytes = 0;
                                    }
                                }
                                entry->frameSeq = *(int*)buffer;
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
        FrameQueue* tail = &entry->tail;
        Frame* f;
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
        *(int*)((char*)net + 0x4b5) = entry->tail.field_14;
        *(int*)((char*)net + 0x4b9) = entry->tail.field_18;
        memcpy(data, src, len);
        *size = len;
        goto ok;
    }

    length = 0;
    return (int)0x887700be;
ok:
    return 0;
}

// FUNCTION: 0x4635b0
PlayerFrameInfo::PlayerFrameInfo()
    : playerNetId(-1), pendingDpToId(-1), frameSeq(-1), pendingBytes(0), pendingCap(-1), frame(0)
{
}

// PlayerFrameInfo::Initialize (from its debug string): resets the fields the
// constructor sets and allocates the buffer if there is none yet.
// FUNCTION: 0x463610
void PlayerFrameInfo::Initialize(long id)
{
    PacketTrace("PlayerFrameInfo::Initialize: %ld", id);
    playerNetId = id;
    pendingDpToId = -1;
    frameSeq = -1;
    pendingBytes = 0;
    tail.ResetFrames();
}

// FUNCTION: 0x463680
PlayerFrameInfo::~PlayerFrameInfo()
{
    operator delete(frame);
}

// FUNCTION: 0x4636b0
FrameQueue::FrameQueue()
{
    buffer = 0;
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
    field_14 = -1;
    field_18 = -1;
    buffer = new FrameRing;
}

// Destructor of the class built by 0x4636b0: frees the buffer at +0x10, then
// the block at +0xc.
// FUNCTION: 0x463710
FrameQueue::~FrameQueue()
{
    operator delete(buffer);
    operator delete(field_c);
}

// Resets the object and allocates its 0x180c-byte buffer if it has none;
// returns 1 when the buffer already existed.
// FUNCTION: 0x463730
int FrameQueue::ResetFrames()
{
    field_0 = 0;
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
    field_14 = -1;
    field_18 = -1;
    if (!buffer) {
        buffer = new FrameRing;
        return 0;
    }
    return 1;
}

// Queues one received packet's commands in the ring at +0x10. If frames are
// already queued, it only re-stamps each of them with the new tick (pop, push) and
// returns 0. Otherwise it copies the packet into the buffer at +0xc, counts the
// commands after the 4-byte sequence number (a command is 2..0x2c; 0x2c carries its
// own 16-bit length, the others' lengths are in the table at 0x512ad8), skips the
// first n - 0x200 0x2c commands when there are more than 0x200 commands, and pushes
// the rest: spread over up to 30 ticks when a6 is set, all at `tick` otherwise.
// FUNCTION: 0x463790
int FrameQueue::QueueFrames(char* src, unsigned int size, int tick, int a4, int a5, int a6)
{
    if (size <= 0)
        return 1;
    if (Count() != 0) {
        field_8++;
        int n = buffer->count;
        while (n--) {
            Frame f = *buffer->Pop();
            buffer->Push(tick, f.data, f.size);
        }
        return 0;
    }

    field_8 = 0;
    if (size > field_4) {
        // delete/new expressions, not operator calls: fixes the temporary rotation.
        delete field_c;
        field_c = new char[size + 0x100];
        if (field_c == 0) {
            field_4 = 0;
            return 1;
        }
        field_4 = size + 0x100;
    }
    memcpy(field_c, src, size);
    field_14 = a4;
    field_18 = a5;
    size -= 4;

    // Declared in this order: gives the original's reload of this for field_c.
    int n = 0;
    int remaining = size;
    char* p = field_c + 4;
    while (remaining > 0) {
        unsigned char c = *p;
        if (c <= 1 || c >= 0x2d)
            break;
        unsigned short w;
        if (c == 0x2c) {
            BitReader reader;
            reader.data = (unsigned int*)p;
            reader.index = 0;
            reader.bit = 0;
            reader.ReadBits(8);
            w = (unsigned short)reader.ReadBits(0x10);
        } else {
            w = g_packetSizes[c][0];
        }
        remaining -= w;
        if (remaining < 0)
            break;
        p += w;
        n++;
    }

    if (n > 0) {
        int left = n - 0x200;
        if (a6 != 0) {
            int span = tick - field_0;
            if (span > 0x1e)
                span = 0x1e;
            else if (span <= 0)
                span = 1;
            int spacing = 0x10;
            if (n > span)
                spacing = (n << 4) / span;
            // x is its own local, defined before progress, i and q.
            int x = tick;
            unsigned int progress = 0;
            unsigned int i = 0;
            char* q = field_c + 4;
            // Both tails end `remaining -= w; q += w; n--;` and the loop tests n > 0.
            do {
                unsigned char c = *q;
                unsigned short w;
                if (c == 0x2c) {
                    BitReader reader;
                    reader.data = (unsigned int*)q;
                    reader.index = 0;
                    reader.bit = 0;
                    reader.ReadBits(8);
                    w = (unsigned short)reader.ReadBits(0x10);
                    if (left > 0) {
                        left--;
                        remaining -= w;
                        q += w;
                        n--;
                        continue;
                    }
                } else {
                    w = g_packetSizes[c][0];
                }
                if (!buffer->Push(x, q, w))
                    return 1;
                i++;
                if ((i << 4) >= progress) {
                    progress += spacing;
                    x++;
                }
                remaining -= w;
                q += w;
                n--;
            } while (n > 0);
            return 1;
        }

        char* q = field_c + 4;
        int rem = size;
        // Exits are `break` to the single return 1 below.
        while (rem > 0) {
            unsigned char c = *q;
            if (c <= 1 || c >= 0x2d)
                break;
            unsigned short w;
            if (c == 0x2c) {
                BitReader reader;
                reader.data = (unsigned int*)q;
                reader.index = 0;
                reader.bit = 0;
                reader.ReadBits(8);
                w = (unsigned short)reader.ReadBits(0x10);
                if (left > 0) {
                    left--;
                    rem -= w;
                    q += w;
                    goto next2;
                }
            } else {
                w = g_packetSizes[c][0];
            }
            rem -= w;
            if (rem < 0)
                break;
            if (!buffer->Push(tick, q, w))
                break;
            q += w;
        next2:
            ;
        }
    }
    return 1;
}

extern char g_emptyAtexitRegistered;
extern void __cdecl atexit(void*);
void EmptyAtexitHandler(void);

// FUNCTION: 0x463ba0
void RegisterEmptyAtexit()
{
    if ((g_emptyAtexitRegistered & 1) == 0) {
        g_emptyAtexitRegistered = g_emptyAtexitRegistered | 1;
    }
    atexit(EmptyAtexitHandler);
}

// FUNCTION: 0x463bd0
void EmptyAtexitHandler(void)
{
}

void __cdecl FUN_004d83a0(int);

// Constructor of the per-player record (0x14b bytes, 11 of them inside the
// game object built by 0x41d920).
#pragma pack(push, 1)
struct Grid_00463be0 {
    void* cells;                       // +0x0
    int width;                         // +0x4
    int height;                        // +0x8
    int field_c;                       // +0xc
    Grid_00463be0() { width = 0; height = 0; field_c = 0; cells = 0; }
};

class Class_00463be0 {
public:
    int field_0;                       // +0x0
    char unknown_4[0x27 - 0x4];
    char* data;                        // +0x27 (0xb9 bytes)
    char unknown_2b[0x73 - 0x2b];
    char field_73;                     // +0x73
    char unknown_74[0x7c - 0x74];
    Grid_00463be0 grid;                // +0x7c
    char unknown_8c[0x146 - 0x8c];
    char field_146;                    // +0x146
    char unknown_147[0x14b - 0x147];

    Class_00463be0();
};
#pragma pack(pop)

// FUNCTION: 0x463be0
Class_00463be0::Class_00463be0() : field_0(0), field_73(0)
{
    field_146 = 10;
    data = (char*)operator new(0xb9);
    FUN_004d83a0((int)data);
    memset(data, 0, 0xb9);
}

#pragma pack(push, 1)
class Class_00463c40 {
public:
    char unknown_0[0x27];
    void* field_27;
    char unknown_2b[0x51];
    void* field_7c;

    void FreeSideDataAndFogSightCounts();
};
#pragma pack(pop)

// FUNCTION: 0x463c40
void Class_00463c40::FreeSideDataAndFogSightCounts()
{
    delete field_27;
    delete field_7c;
}

#pragma pack(push, 1)
struct Player {
    char unknown_0[0x27];
    int field_27;
    char unknown_2b[0x73 - 0x2b];
    char field_73;
    void SetType(int param_1);
};
#pragma pack(pop)

// FUNCTION: 0x463c60
void Player::SetType(int param_1)
{
    field_73 = (char)param_1;
    if (param_1 != 3) {
        *(char*)((char*)field_27 + 0x94) = (char)param_1;
    }
}

// FUNCTION: 0x463c80
void ResetChatHudIndices()
{
    g_game->tail = 0;
    g_game->head = 0;
}
