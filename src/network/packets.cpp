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
#include <malloc.h>

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
struct NetPacketPendingEntry {
    int frame;                         // +0x00
    int offset;                        // +0x04, into the buffer's data
    int size;                          // +0x08
    PacketBuffer* owner;               // +0x0c
    int queued;                        // +0x10, >= 0 while in the queue
    int sentTime;                      // +0x14, when it left the queue
    NetPacketPendingEntry* prev;       // +0x18
    NetPacketPendingEntry* next;       // +0x1c

    void RemoveFromBuffer();
    int GetData();
};

class PacketBuffer {
public:
    PacketChannel* pool;               // +0x00
    int start;                         // +0x04, the index of its first packet
    int count;                         // +0x08
    int length;                        // +0x0c
    NetPacketPendingEntry* first;      // +0x10
    unsigned char data[0x42a];         // +0x14

    int AppendPacket(NetPacketPendingEntry* packet, int index, const void* src, unsigned int size, NetPacketPendingEntry* prev);
    void FreePackets();
    void FindOwnerChainTail();
    int IsReusable(int minRetain);
    void RemovePacket(NetPacketPendingEntry* packet);
};

// The ring of queued packets (0x462370 is its push, 0x4623b0 its pop).
class PacketRing {
public:
    int count;                         // +0x0
    int readIdx;                       // +0x4
    int writeIdx;                      // +0x8
    NetPacketPendingEntry* buf[0x400];  // +0xc

    PacketRing();
    int PushPacket(NetPacketPendingEntry* value);
    // Read side of the ring, out of line (0x4623b0).
    NetPacketPendingEntry* PopPacket();
    NetPacketPendingEntry* Peek()
    {
        if (count > 0)
            return buf[readIdx];
        return 0;
    }
    NetPacketPendingEntry* Pop()
    {
        if (count > 0) {
            count--;
            NetPacketPendingEntry* value = buf[readIdx];
            readIdx++;
            if (readIdx >= 0x400)
                readIdx = 0;
            return value;
        }
        return 0;
    }
    int Push(NetPacketPendingEntry* value)
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

// Unused here: forward declarations of real functions; their symbol ids keep
// the allocation (docs/c2-regalloc.md).
void RegisterUnitOrders();
void RegisterGroundOrders();
void EnableAICommands();
void RegisterAICommands();
void ResetAIPlayers();
void RegisterVtolOrders();
void StepAllGafSequences();
void ResetNetStats();
void InitCommands();
int UpdatePlacementGhostValidity();
void RefreshSelectionOrders();
void DispatchOrdersPanelPageFlags();
void ResetCameraState();
void SetUpEndMissionScreen();
void StartScreenFade();
void StepScreenFade();
void FreeRadar();
void StopAllSounds();
void FlipScreen();
void RestoreStartDirectory();

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

#include "bit_reader.h"

// Length of each packet command, one int per command byte (the size in
// the low word, the writer leaves the high word zero).
extern int g_packetSizes[45];

#include "packet_receiver.h"

static inline int QueueCount(FrameQueue* q) { return q->buffer ? q->buffer->count : 0; }

static inline Frame* PeekFrame(FrameQueue* q)
{
    if (q->buffer == 0 || q->buffer->count <= 0)
        return 0;
    return &q->buffer->frames[q->buffer->head];
}

// Pops the frame at the head if it is due (or if there is no tick).
static inline void* TakeFrame(FrameQueue* q, Frame* f, int tick, int& size)
{
    if (f != 0) {
        int d = f->tick - tick;
        if (tick == 0 || d <= 0 || d > 0x1e) {
            f = q->buffer->Pop();
            size = f->size;
            return f->data;
        }
    }
    return 0;
}

static inline void* GetFrame(FrameQueue* q, int tick, int& size)
{
    size = 0;
    return TakeFrame(q, PeekFrame(q), tick, size);
}

#pragma pack(push, 1)
struct GameEntry {
    int active;                        // +0x00
    int id;                            // +0x04
    char unknown_8[0x14b - 8];
};

struct Game {
    char unknown_0[0x14];
    char session[4];                   // +0x14
    char unknown_18[0x1b63 - 0x18];
    GameEntry players[10];             // +0x1b63
    char unknown_2851[0x2a3e - 0x2851];
    unsigned short chatHudWriteIdx;    // +0x2a3e
    unsigned short chatHudReadIdx;     // +0x2a40
    char unknown_2a42[0x38a47 - 0x2a42];
    int tick;                          // +0x38a47
};

class NetCondenser {
public:
    char unknown_0[0x21];
    int to;                            // +0x21

    void SendPacketTo(void* session, int from, int value, void* data, int size);
    void Accumulate(void* data, int size);
    int SendPacket(void* session, int from);
    int ReceivePacket(void* net, char* data, int* size);
};
#pragma pack(pop)

extern Game* g_game;
extern NetCondenser g_sendCondenser;
extern NetCondenser g_receiveCondenser;
// Unused here: the symbol id this declaration takes keeps the allocation
// of 0x461da0 and 0x462bf0 (docs/c2-regalloc.md).
extern int g_packetModes[];
extern int g_usePacketManager;
extern int g_netFrameRateConfig;

// One player's packet channel: a pool of 0x43e-byte buffers, a pool of
// 0x20-byte packet records that point into them, and a ring of queued
// packets that SendQueued hands to the outgoing buffer.
class PacketChannel {
public:
    int bufferIndex;                   // +0x00, the buffer last handed out
    unsigned int sendPacingTicks;      // +0x04, ticks between sends
    PacketBuffer** buffers;            // +0x08
    unsigned int bufferCount;          // +0x0c, the buffer count
    int frameNumber;                   // +0x10, the frame number
    int dpid;                          // +0x14, the player's DPID
    unsigned int timeoutTicks;         // +0x18, the minimum retain time
    int packetIndex;                   // +0x1c, the packet last handed out
    unsigned int queuedBytes;          // +0x20, queued bytes
    unsigned int nextSendTick;         // +0x24, the next send time
    NetPacketPendingEntry* packets;    // +0x28
    unsigned int count;                // +0x2c, the packet count
    NetPacketPendingEntry* firstPacket;  // +0x30, the first packet in use
    NetPacketPendingEntry* lastPacket;  // +0x34, the last packet in use
    PacketRing queue;                  // +0x38

    PacketChannel();
    ~PacketChannel();
    PacketBuffer* AllocBuffer();
    NetPacketPendingEntry* AllocPacket(int param_1);
    int GetMinRetainMs();
    int GetPacketEntry(int param_1);
    int InitPools(int a1, unsigned int a2, int a3, int a4);
    void EnqueuePacket(NetPacketPendingEntry* item);
    int GrowPools(int growbufs, int growpackets);
    void DequeuePacket(NetPacketPendingEntry* item);
    void ResetChannel();
    int SendQueued(int force);
    int AddPacket(int param_1, void* param_2, unsigned int param_3);
    void SetMinRetainMs(unsigned int ms);
    void SetSendPacingMs(int ms);
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
    int QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4);
    void* QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4);
    void SetDefaultSendPacing(int rate);
    void HandleIntegrityNop(int, int, int);
};

// Defined in packets_460e20.cpp, whose dynamic initialiser (_$E4) and static
// destructor (packets_460f60.cpp, _$E2) need views of their own.
extern PacketManager g_packetManager;

// Unused here: the symbol ids these declarations take keep the allocation,
// standing in for the view classes joined into PacketRing, PacketBuffer and
// NetPacketPendingEntry (docs/c2-regalloc.md).
int __stdcall DefaultPacketHandler(int arg1);
int __stdcall RejectPacketType2(int arg1);
void __stdcall AnnouncePlayerLeft(int id);
int __stdcall GetPlayerName(int dpid, char* shortName, char* longName);

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
            if (g_packetManager.member.savedFrameEntry != 0) {
                g_packetManager.member.savedFrameEntry->pendingBytes = 0;
                g_packetManager.member.savedFrameEntry = 0;
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
    g_sendCondenser.to = to;
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
    g_sendCondenser.to = to;
    g_sendCondenser.Accumulate(data, size);
    int result = ((NetCondenser*)&g_sendCondenser)->SendPacket(session, from);
    return result == 0 ? 1 : 0;
}

// The same reset as 0x462470.
static inline void Reset(PacketChannel* e)
{
    e->InitPools(-1, 0xc8, 2, 0x64);
    e->bufferIndex = -1;
    e->packetIndex = -1;
    e->nextSendTick = 0;
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

// Unused here: the symbol ids these declarations take keep the allocation (docs/c2-regalloc.md).
void WriteScreenshot(char*, char*, int, int, int, int);

// An empty method, called once (from 0x453d40) on the global g_packetManager,
// like its neighbour 0x461610.
// FUNCTION: 0x461620
void PacketManager::HandleIntegrityNop(int, int, int)
{
}

// Finds the entry (of the 11 inside this) whose dpid is param_1 and returns
// it. When there is none, param_2 decides whether a new entry may be made:
// first an unused slot (dpid == -1), otherwise the first slot whose
// dpid is not the color of any of the ten player slots in g_game. The
// chosen entry is re-initialised with InitPools and returned.
// FUNCTION: 0x461630
PacketChannel* PacketManager::FindChannel(int param_1, int param_2)
{
    unsigned i;
    for (i = 0; i <= 10; i++) {
        if (channels[i].dpid == param_1)
            return &channels[i];
    }
    if (param_2 == 0)
        return 0;
    for (i = 0; i <= 10; i++) {
        if (channels[i].dpid == -1) {
            channels[i].InitPools(param_1, m_defaultSendPacingMs, 2, 0x64);
            return &channels[i];
        }
    }
    for (i = 1; i <= 10; i++) {
        int used = 0;
        // Must walk a pointer here; the outer loops stay plain array indexing.
        GameEntry* p = &g_game->players[0];
        for (int j = 0; j < 10; j++, p++) {
            if (p->id == channels[i].dpid) {
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
        if (member.savedFrameEntry != 0) {
            member.savedFrameEntry->pendingBytes = 0;
            member.savedFrameEntry = 0;
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
        e->bufferIndex = -1;
        e->packetIndex = -1;
        e->nextSendTick = 0;
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
        if (channels[i].dpid != -1) {
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
int PacketManager::QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4)
{
    int a = param_4;
    int b = param_3;
    int c = param_1;
    return param_2->AddPacket(c, (void*)b, a);
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
        channels[i].sendPacingTicks = (m_defaultSendPacingMs * 30 + 999) / 1000;
    }
}

// One `return` per outcome: that gives the original's un-rotated scan loop
// and its block order.
static inline int IsReusable(PacketBuffer* buf, int minRetain)
{
    if (buf->count == 0)
        return 1;
    NetPacketPendingEntry* p = buf->first;
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
    : bufferIndex(-1), sendPacingTicks(0), buffers(0), bufferCount(0), frameNumber(-2), dpid(-1), timeoutTicks(0),
      packetIndex(-1), queuedBytes(0), nextSendTick(0), packets(0), count(0), firstPacket(0), lastPacket(0)
{
    SetMinRetainMs(4000);
    SetSendPacingMs(200);
}

// Destructor of the class built by 0x461a70: frees each block of the pointer
// array at +0x8 (count at +0xc), the array itself, then the packets at +0x28.
// FUNCTION: 0x461ac0
PacketChannel::~PacketChannel()
{
    if (buffers) {
        for (unsigned int i = 0; i < bufferCount; i++) {
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
        if (bufferCount > 0) {
            unsigned int ix = bufferIndex;
            bufferIndex = ix;
            ++ix;
            if (ix >= bufferCount)
                ix = 0;
            PacketBuffer* buf = buffers[ix];
            if (IsReusable(buf, timeoutTicks)) {
                buf->FreePackets();
                bufferIndex = ix;
                return buf;
            }
            if (bufferCount >= 0x22) {
                buf->FreePackets();
                bufferIndex = ix;
                PacketTrace("force-allocated a previously-used buffer, ix=%ld\n", ix);
                return buf;
            }
        }
        if (GrowPools(0x10, 0x320) == 0)
            return 0;
    }
}

// FUNCTION: 0x461c20
NetPacketPendingEntry* PacketChannel::AllocPacket(int param_1)
{
    unsigned int idx;
    NetPacketPendingEntry* packet;
    for (;;) {
        PacketTrace("current packet pool index: %ld\n", packetIndex);
        idx = packetIndex + 1;
        if (idx >= count)
            idx = 0;
        packet = &packets[idx];
        PacketBuffer* owner = packet->owner;
        if (owner == 0)
            break;
        if (IsReusable(owner, timeoutTicks)) {
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
    packetIndex = idx;
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
    unsigned int val = timeoutTicks;
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
    dpid = a1;
    sendPacingTicks = (a2 * 30 + 999) / 1000;
    if (packets == 0) {
        if (a4 == 0)
            a4 = 100;
        NetPacketPendingEntry* p = (NetPacketPendingEntry*)operator new(a4 * 32);
        if (p) {
            // A countdown over a separate walking pointer: this spelling is
            // what gives `lea edx,[esi-1] / cmp / jl` and `inc edx` at 0x461e06.
            k = a4 - 1;
            NetPacketPendingEntry* q = p;
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
        for (bufferCount = 0; bufferCount < a3; bufferCount++) {
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
            buffers[bufferCount] = q;
            if (buffers[bufferCount] == 0)
                goto fail;
        }
    }
    for (i = 0; i < bufferCount; i++) {
        // The pointer local is what puts the store in eax and the call in ecx.
        PacketBuffer* q = buffers[i];
        q->count = 0;
        buffers[i]->FreePackets();
    }
    NetPacketPendingEntry t;
    t.offset = 0;
    t.size = 0;
    t.owner = 0;
    t.queued = -1;
    t.sentTime = 0;
    t.prev = 0;
    t.next = 0;
    for (j = 0; j < count; j++)
        packets[j] = t;
    bufferIndex = -1;
    packetIndex = -1;
    firstPacket = 0;
    lastPacket = 0;
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
    queuedBytes = 0;
    return 1;
    // Single shared `return 0` via goto; counters sit at function scope so no
    // jump skips an initialiser.
fail:
    return 0;
}

// Queues an item in the ring buffer at +0x38 (the inlined push of
// 0x462370), marks it queued and adds its size to the total.
// FUNCTION: 0x461f90
void PacketChannel::EnqueuePacket(NetPacketPendingEntry* item)
{
    queue.Push(item);
    item->queued = 0;
    queuedBytes += item->size;
}

// DequeuePacket as GrowPools and RemovePacket expand it: the ring's pop and push
// stay calls (0x4623b0 and 0x462370), where 0x4623e0 inlines them.
static inline void DequeueByCalls(PacketChannel* channel, NetPacketPendingEntry* item)
{
    item->queued = -1;
    item->sentTime = GetTicks();
    int n = channel->queue.count;
    while (n-- > 0) {
        NetPacketPendingEntry* value = channel->queue.PopPacket();
        if (value == item)
            break;
        channel->queue.PushPacket(value);
    }
    channel->queuedBytes -= item->size;
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
                 bufferCount, count);
    PacketTrace("bufs to grow by: %d, packets to grow by: %d\n", growbufs, growpackets);
    PacketTrace("current buffer pool ix: %d, current packet pool ix: %d\n",
                 bufferIndex, packetIndex);
    if (bufferCount <= 0 || count <= 0) {
        return 1;
    }

    // operator new/delete, not new[]: new[] references ??_U / ??_V instead.
    int total = bufferCount + growbufs;
    PacketBuffer** np = (PacketBuffer**)operator new(total * 4);
    if (np) {
        memset(np, 0, total * 4);
        if (bufferIndex >= 0) {
            int i = 0;
            int ix = bufferIndex;
            while (i < bufferCount) {
                ix = ix + 1;
                if (ix >= bufferCount) {
                    ix = 0;
                }
                np[i] = buffers[ix];
                i = i + 1;
            }
        } else {
            memcpy(np, buffers, bufferCount * 4);
        }
        bufferIndex = bufferCount - 1;
        int ok = 1;
        while (1) {
            if (bufferCount >= (unsigned int)total) {
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
            np[bufferCount] = q;
            ok = (np[bufferCount] != 0);
            bufferCount = bufferCount + 1;
            if (!ok) {
                break;
            }
        }
        operator delete(buffers);
        buffers = np;
        if (ok) {
        int totalentries = growpackets + count;
        NetPacketPendingEntry* ne = (NetPacketPendingEntry*)operator new(totalentries * 32);
        NetPacketPendingEntry* base;
        if (ne) {
            NetPacketPendingEntry* q = ne;
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
        if (bufferIndex >= 0) {
            lastPacket = 0;
            firstPacket = 0;
            for (int i = 0; i < bufferCount; i++) {
                PacketBuffer* p = buffers[i];
                if (p->count > 0) {
                    NetPacketPendingEntry* c = p->first;
                    if (c != 0) {
                        p->start = n;
                        j = 0;
                        p->first = &base[n];
                        while (c != 0) {
                            if (c->owner != p) {
                                break;
                            }
                            NetPacketPendingEntry* e = &base[n];
                            n++;
                            *e = *c;
                            if (c->queued >= 0) {
                                DequeueByCalls(this, c);
                                EnqueuePacket(e);
                            }
                            e->prev = lastPacket;
                            e->owner = p;
                            e->next = 0;
                            // Both arms store tail = e: gives e the register weight the
                            // allocation needs; the stores are merged again.
                            if (lastPacket) {
                                lastPacket->next = e;
                                lastPacket = e;
                            } else {
                                lastPacket = e;
                            }
                            if (!firstPacket) {
                                firstPacket = e;
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
        packetIndex = n - 1;
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
int PacketRing::PushPacket(NetPacketPendingEntry* value)
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
NetPacketPendingEntry* PacketRing::PopPacket()
{
    if (count > 0) {
        count--;
        NetPacketPendingEntry* value = buf[readIdx];
        readIdx++;
        if (readIdx < 0x400) {
            return value;
        }
        readIdx = 0;
        return value;
    }
    return 0;
}
#pragma auto_inline(on)

// FUNCTION: 0x4623e0
void PacketChannel::DequeuePacket(NetPacketPendingEntry* item)
{
    item->queued = -1;
    item->sentTime = GetTicks();
    int n = queue.count;
    while (n-- > 0) {
        NetPacketPendingEntry* value = queue.Pop();
        if (value == item)
            break;
        queue.Push(value);
    }
    queuedBytes -= item->size;
}

// FUNCTION: 0x462470
void PacketChannel::ResetChannel()
{
    InitPools(-1, 0xc8, 2, 0x64);
    bufferIndex = -1;
    packetIndex = -1;
    nextSendTick = 0;
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
                 dpid, sendPacingTicks, nextSendTick, now);
    // Keep this nesting with the trailing `return 1`: it gives the three epilogues.
    if (now >= nextSendTick || force != 0) {
        nextSendTick = now + sendPacingTicks;
        int n;
        while ((n = queue.count) != 0) {
            int headFrame = queue.Peek()->frame;
            int sent = 0;
            PacketTrace("assigning packets to frame number: %ld\n", frameNumber);
            for (int i = 0; i < n; i++) {
                NetPacketPendingEntry* entry = queue.Peek();
                queue.Pop();
                if (entry->frame == headFrame) {
                    // Keep owner->data[offset] written out as an expression each time.
                    PacketTrace("extracted packet (len=%ld, type=%d, data=\"%s\")\n",
                                 entry->size, entry->owner->data[entry->offset],
                                 &entry->owner->data[entry->offset + 1]);
                    entry->queued = frameNumber;
                    entry->sentTime = GetTicks();
                    if (g_packetManager.AppendToSendBuffer(&entry->owner->data[entry->offset], entry->size) == 0)
                        return 0;
                    sent++;
                } else {
                    queue.Push(entry);
                }
            }
            queuedBytes = 0;
            if (sent > 0) {
                PacketTrace("sending %ld packets in frame: %ld\n", sent, frameNumber);
                *(int*)g_packetManager.buffer = dpid != 0 ? -1 : frameNumber;
                // Read in this order (nbytes, id, data) before the log call.
                unsigned int nbytes = g_packetManager.size;
                int id = dpid;
                unsigned char* data = g_packetManager.buffer;
                PacketTrace("bytes to send to (DPID)(%ld): %ld\n", id, nbytes);
                g_sendCondenser.SendPacketTo(g_game->session, headFrame, id, data, nbytes);
                g_packetManager.size = g_packetManager.buffer != 0 ? 4 : 0;
                frameNumber--;
                if (frameNumber >= -1)
                    frameNumber = -2;
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
    to = value;
    this->Accumulate(data, size);
    this->SendPacket(session, from);
}
#pragma auto_inline(on)

// FUNCTION: 0x462710
int PacketChannel::AddPacket(int param_1, void* param_2, unsigned int param_3)
{
    if (queuedBytes >= 0x42a || queue.count == 0x400) {
        SendQueued(1);
        if (queue.count != 0)
            return 0;
    }
    PacketBuffer* block = 0;
    int idx = bufferIndex;
    if (idx >= 0)
        block = buffers[idx];
    if (block == 0) {
        block = AllocBuffer();
        if (block == 0)
            return 0;
    }
    NetPacketPendingEntry* pkt = AllocPacket(param_1);
    if (pkt == 0)
        return 0;
    int r = block->AppendPacket(pkt, packetIndex, param_2, param_3, lastPacket);
    if (r == 0) {
        packetIndex = packetIndex - 1;
        block = AllocBuffer();
        if (block != 0) {
            pkt = AllocPacket(param_1);
            if (pkt != 0)
                r = block->AppendPacket(pkt, packetIndex, param_2, param_3, lastPacket);
            else
                r = 0;
        }
    }
    if (r != 0) {
        if (firstPacket == 0)
            firstPacket = pkt;
        if (lastPacket != 0)
            lastPacket->next = pkt;
        lastPacket = pkt;
        queue.Push(pkt);
        pkt->queued = 0;
        queuedBytes += pkt->size;
        return 1;
    }
    if (block != 0 && pkt->owner == block)
        block->RemovePacket(pkt);
    return 0;
}

// Clamps a time in milliseconds to 4000..60000 and stores it converted to
// 30 Hz ticks, rounded up (compare 0x4628a0).
// FUNCTION: 0x462860
void PacketChannel::SetMinRetainMs(unsigned int ms)
{
    if (ms > 60000)
        ms = 60000;
    else if (ms < 4000)
        ms = 4000;
    timeoutTicks = (ms * 30 + 999) / 1000;
}

// FUNCTION: 0x4628a0
void PacketChannel::SetSendPacingMs(int param_1)
{
    unsigned int v = param_1 * 30 + 999;
    sendPacingTicks = v / 1000u;
}

// Appends `size` bytes at `src` to the buffer's inline storage (at +0x14),
// fills in the output packet `p`, and bumps the buffer's packet count.
// `prev` is the previously queued packet (0x462710 passes its tail), stored
// in the packet's prev field at +0x18.
// FUNCTION: 0x4628d0
int PacketBuffer::AppendPacket(NetPacketPendingEntry* p, int index, const void* src, unsigned int size, NetPacketPendingEntry* prev)
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
        NetPacketPendingEntry* packets = pool->packets;
        while (n--) {
            PacketTrace("initialize freeing packet %ld\n", i);
            NetPacketPendingEntry* p = &packets[i];
            i++;
            p->owner->RemovePacket(p);
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
        NetPacketPendingEntry* p = first;
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
        NetPacketPendingEntry* p = first;
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
void PacketBuffer::RemovePacket(NetPacketPendingEntry* packet)
{
    PacketTrace("removing packet (len=%ld, type=%d, data=\"%s\")\n",
                 packet->size,
                 *(unsigned char*)((char*)packet->offset + (int)packet->owner + 0x14),
                 (char*)packet->offset + (int)packet->owner + 0x15);

    if (packet->queued >= 0) {
        PacketTrace("Warning! RemovePacket called for packet in pending queue!\n");
        PacketChannel* mgr = pool;
        packet->queued = -1;
        packet->sentTime = GetTicks();
        int n = mgr->queue.count;
        while (n-- > 0) {
            NetPacketPendingEntry* value = mgr->queue.PopPacket();
            if (value == packet)
                break;
            mgr->queue.PushPacket(value);
        }
        mgr->queuedBytes -= packet->size;
    }

    if (first == packet) {
        NetPacketPendingEntry* nxt = packet->next;
        if (nxt != 0 && nxt->owner == this)
            first = nxt;
        else
            first = 0;
    }

    PacketChannel* m = pool;
    if (m->lastPacket == packet)
        m->lastPacket = packet->prev;
    if (m->firstPacket == packet)
        m->firstPacket = packet->next;
    if (packet->prev != 0)
        packet->prev->next = packet->next;
    if (packet->next != 0) {
        packet->next->prev = packet->prev;
        packet->next = 0;
    }
    packet->prev = 0;
}

// FUNCTION: 0x462bd0
void NetPacketPendingEntry::RemoveFromBuffer()
{
    owner->RemovePacket(this);
    owner = 0;
}

// FUNCTION: 0x462bf0
int NetPacketPendingEntry::GetData()
{
    int eax = this->offset;
    int ecx = (int)this->owner;
    return eax + ecx + 0x14;
}

// The real constructor of PacketReceiver (vtable 0x4fd518, scalar deleting
// destructor 0x462cc0, destructor 0x462d30).
// FUNCTION: 0x462c00
PacketReceiver::PacketReceiver(void* o)
    : unused(0), owner(o), fromId(-1), toId(-1), savedFrameEntry(0), buffer(0), spare(0),
      capacity(0), length(0), spareLength(0), spareFromId(-1), spareToId(-1)
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
    if (savedFrameEntry != 0) {
        savedFrameEntry->pendingBytes = 0;
        savedFrameEntry = 0;
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
        src = GetFrame(&e->tail, tick, len);
        if (src != 0) {
            *(int*)((char*)net + 0x4b5) = e->tail.fromId;
            *(int*)((char*)net + 0x4b9) = e->tail.toId;
            memcpy(data, src, len);
            *size = len;
            // Both copy-outs goto the one `ok: return 0;`.
            goto ok;
        }
    }

    entry = 0;
    if (length == 0) {
        int n = 0;
        if (savedFrameEntry != 0) {
            if (spare != 0) {
                buffer = spare;
                if (spareLength > 0) {
                    length = spareLength;
                    fromId = spareFromId;
                    toId = spareToId;
                    n = length - 4;
                    if (n > 0)
                        savedFrameEntry->frameSeq = *(int*)buffer;
                } else {
                    length = 0;
                }
                spare = 0;
                savedFrameEntry->pendingBytes = 0;
                savedFrameEntry = 0;
            } else if (savedFrameEntry->pendingBytes > 0) {
                spare = buffer;
                spareFromId = fromId;
                spareLength = 0;
                spareToId = toId;
                buffer = savedFrameEntry->frame;
                savedFrameEntry->frameSeq = *(int*)buffer;
                length = savedFrameEntry->pendingBytes;
                fromId = savedFrameEntry->playerNetId;
                toId = savedFrameEntry->pendingDpToId;
                n = length - 4;
                savedFrameEntry->pendingBytes = 0;
            } else {
                savedFrameEntry = 0;
            }
        }
        if (n == 0) {
            length = capacity;
            rc = g_receiveCondenser.ReceivePacket((char*)&g_game->session[0], buffer, &length);
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
                rc = g_receiveCondenser.ReceivePacket((char*)&g_game->session[0], buffer, &length);
            }
            // Always true here (the enclosing test), so MSVC emits no test; the error
            // block stays after B only as this if's else arm.
            if (n == 0) {
                fromId = *(int*)((char*)net + 0x4b5);
                toId = *(int*)((char*)net + 0x4b9);
                if (*(int*)((char*)net + 0x4b5) != 0) {
                    if ((unsigned int)length >= sizeof(int)) {
                        if (length == sizeof(int))
                            return (int)0x80004005;
                        if (*(int*)buffer != -1) {
                            entry = this->FindPlayerFrameInfo(fromId);
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
                                            entry->pendingDpToId = toId;
                                            entry->playerNetId = fromId;
                                            entry->pendingBytes = length;
                                            length = 0;
                                            return (int)0x887700be;
                                        }
                                        prev = Prev_00462f30(entry->frameSeq);
                                        if (*(int*)entry->frame <= cur) {
                                            if (prev != cur)
                                                ReportPacketGap(fromId, prev, Next_00462f30(cur));
                                            int p2 = Prev_00462f30(*(int*)buffer);
                                            if (p2 != *(int*)entry->frame)
                                                ReportPacketGap(fromId, p2, Next_00462f30(*(int*)entry->frame));
                                            flag = 0;
                                            savedFrameEntry = entry;
                                        } else {
                                            if (prev != *(int*)entry->frame)
                                                ReportPacketGap(fromId, prev, Next_00462f30(*(int*)entry->frame));
                                            int p2 = Prev_00462f30(*(int*)entry->frame);
                                            if (p2 != *(int*)buffer)
                                                ReportPacketGap(fromId, p2, Next_00462f30(*(int*)buffer));
                                        }
                                    }
                                    if (flag && entry->pendingBytes > 0) {
                                        // Swap the saved frame in, keep this one as the spare.
                                        spare = buffer;
                                        spareLength = length;
                                        spareFromId = fromId;
                                        spareToId = toId;
                                        buffer = entry->frame;
                                        length = entry->pendingBytes;
                                        fromId = entry->playerNetId;
                                        toId = entry->pendingDpToId;
                                        savedFrameEntry = entry;
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
        entry = this->FindPlayerFrameInfo(fromId);
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
        if (tail->QueueFrames(buffer, length, tick, fromId, toId, savedFrameEntry == 0)) {
            length = 0;
            len = 0;
            f = PeekFrame(tail);
        } else {
            len = 0;
            f = PeekFrame(tail);
        }
        src = TakeFrame(tail, f, tick, len);
    }
    if (src != 0) {
        *(int*)((char*)net + 0x4b5) = entry->tail.fromId;
        *(int*)((char*)net + 0x4b9) = entry->tail.toId;
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
    baseTick = 0;
    bufferSize = 0;
    skipCount = 0;
    recvBuffer = 0;
    fromId = -1;
    toId = -1;
    buffer = new FrameRing;
}

// Destructor of the class built by 0x4636b0: frees the buffer at +0x10, then
// the block at +0xc.
// FUNCTION: 0x463710
FrameQueue::~FrameQueue()
{
    operator delete(buffer);
    operator delete(recvBuffer);
}

// Resets the object and allocates its 0x180c-byte buffer if it has none;
// returns 1 when the buffer already existed.
// FUNCTION: 0x463730
int FrameQueue::ResetFrames()
{
    baseTick = 0;
    bufferSize = 0;
    skipCount = 0;
    recvBuffer = 0;
    fromId = -1;
    toId = -1;
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
    if (QueueCount(this) != 0) {
        skipCount++;
        int n = buffer->count;
        while (n--) {
            Frame f = *buffer->Pop();
            buffer->Push(tick, f.data, f.size);
        }
        return 0;
    }

    skipCount = 0;
    if (size > bufferSize) {
        // delete/new expressions, not operator calls: fixes the temporary rotation.
        delete recvBuffer;
        recvBuffer = new char[size + 0x100];
        if (recvBuffer == 0) {
            bufferSize = 0;
            return 1;
        }
        bufferSize = size + 0x100;
    }
    memcpy(recvBuffer, src, size);
    fromId = a4;
    toId = a5;
    size -= 4;

    // Declared in this order: gives the original's reload of this for recvBuffer.
    int n = 0;
    int remaining = size;
    char* p = recvBuffer + 4;
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
            w = g_packetSizes[c];
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
            int span = tick - baseTick;
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
            char* q = recvBuffer + 4;
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
                    w = g_packetSizes[c];
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

        char* q = recvBuffer + 4;
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
                w = g_packetSizes[c];
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

void __cdecl AllocNotifyNop(int);

#include "../map/grid.h"

// Constructor of the per-player record (0x14b bytes, 11 of them inside the
// game object built by 0x41d920).
#pragma pack(push, 1)
struct Player {
    int active;                        // +0x0
    char unknown_4[0x27 - 0x4];
    union {
        char* data;                    // +0x27 (0xb9 bytes)
        void* info;
    };
    char unknown_2b[0x73 - 0x2b];
    char type;                         // +0x73
    char unknown_74[0x7c - 0x74];
    Grid grid;                         // +0x7c
    char unknown_8c[0x146 - 0x8c];
    char index;                        // +0x146
    char unknown_147[0x14b - 0x147];

    Player();
    void FreeSideDataAndFogSightCounts();
    void SetType(int param_1);
};
#pragma pack(pop)

// FUNCTION: 0x463be0
Player::Player() : active(0), type(0)
{
    index = 10;
    data = (char*)operator new(0xb9);
    AllocNotifyNop((int)data);
    memset(data, 0, 0xb9);
}

// FUNCTION: 0x463c40
void Player::FreeSideDataAndFogSightCounts()
{
    delete info;
    delete grid.cells;
}

// FUNCTION: 0x463c60
void Player::SetType(int param_1)
{
    type = (char)param_1;
    if (param_1 != 3) {
        *((char*)info + 0x94) = (char)param_1;
    }
}

// FUNCTION: 0x463c80
void ResetChatHudIndices()
{
    g_game->chatHudWriteIdx = 0;
    g_game->chatHudReadIdx = 0;
}
