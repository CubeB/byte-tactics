// Decompiled by Opus, Sonnet, DeepSeek V4.1 Flash, deepseek-v4.1-flash, deepseek-v4.1 and Haiku. Names are provisional.
// The packet manager (the global g_packetManager): eleven per-player packet
// channels, the outgoing send buffer and a PacketReceiver.
#include <string.h>
#include <windows.h>

void* __cdecl operator new(unsigned int size);
void __stdcall CountMessage(unsigned char type, int len, int which);
void __cdecl PacketTrace(const char* fmt, ...);

extern int g_usePacketManager;

#pragma pack(push, 1)
// One player's channel, 0x1044 bytes (packet_channel.cpp).
class PacketChannel {
public:
    int field_0;                       // +0x00
    unsigned long field_4;             // +0x04, ticks between sends
    char unknown_8[0x14 - 0x8];
    int field_14;                      // +0x14, the player's DPID
    char unknown_18[0x1c - 0x18];
    int field_1c;                      // +0x1c
    char unknown_20[4];
    int field_24;                      // +0x24
    char unknown_28[0x1044 - 0x28];

    int InitPools(int a1, unsigned int a2, int a3, int a4);
    int SendQueued(int force);
    int AddPacket(int param_1, void* param_2, unsigned int param_3);
};

// A player slot: the 0x14b byte record the game keeps in g_game->players.
struct Player_00461630 {
    int field_0;                       // +0x00
    int color;                         // +0x04
    char unknown_8[0x14b - 0x8];
};

struct Game {
    char unknown_0[0x14];
    char session[4];                   // +0x14
    char unknown_18[0x1b63 - 0x18];
    Player_00461630 players[10];       // +0x1b63
};
#pragma pack(pop)

extern Game* g_game;

class NetCondenser {
public:
    void Accumulate(void* data, int size);
    int SendPacket(void* session, int from);
};

extern NetCondenser g_sendCondenser;
extern int DAT_005129f1;

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

struct Msg_00461900 {
    char unknown_0[0x10];
    int field_10;                      // +0x10
    int field_14;                      // +0x14
};

struct PlayerFrameInfo {
    char unknown_0[0x34];
};

class PacketReceiver {
public:
    virtual ~PacketReceiver();
    int field_4;                       // +0x04
    void* owner;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    void* field_14;                    // +0x14
    void* field_18;                    // +0x18
    void* field_1c;                    // +0x1c
    PlayerFrameInfo entries[10];       // +0x20
    int field_228;                     // +0x228
    int field_22c;                     // +0x22c
    int field_230;                     // +0x230
    int field_234;                     // +0x234
    int field_238;                     // +0x238
};

class PacketManager {
public:
    // In packets_461420.cpp (and its deleting form in packets_461340.cpp):
    // they need a channel with a destructor, the constructor one without.
    virtual ~PacketManager();
    unsigned long m_defaultSendPacingMs;   // +0x04
    PacketChannel channels[11];        // +0x08
    unsigned char* buffer;             // +0xb2f4
    unsigned int size;                 // +0xb2f8
    unsigned int capacity;             // +0xb2fc
    PacketReceiver member;             // +0xb300

    // In packets_4611e0.cpp (see the destructor).
    PacketManager();
    int AppendToSendBuffer(unsigned char* data, unsigned int len);
    void ClearSendBuffer();
    void FUN_00461610();
    PacketChannel* FindChannel(int param_1, int param_2);
    int InitChannels(int arg1, int arg2);
    void ReleaseChannel(int id);
    void ReleaseAllChannels();
    int SendAllQueued(int param_1);
    int FUN_00461900(int from, Msg_00461900* msg);
    void QueueOnChannel(int param_1, PacketChannel* param_2, int param_3, int param_4);
    void* QueuePacket(int param_1, int param_2, void* param_3, unsigned int param_4);
    void SetDefaultSendPacing(int rate);
};

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
void PacketManager::FUN_00461610()
{
}

// Finds the entry (of the 11 inside this) whose field_14 is param_1 and returns
// it. When there is none, param_2 decides whether a new entry may be made:
// first an unused slot (field_14 == -1), otherwise the first slot whose
// field_14 is not the color of any of the ten player slots in g_game. The
// chosen entry is re-initialised with InitPools and returned.
//
// The one instruction that decides the whole register allocation is in the
// player-colour scan: walking a pointer (`Player* p = &g_game->players[0]; for
// (j = 0; j < 10; j++, p++)`) instead of indexing `g_game->players[j]` adds
// just enough register pressure that MSVC 5 stops assuming ecx survives the two
// InitPools calls, and gives the object pointer the callee-saved ebx
// (`push ebx; mov ebx, ecx`, plus a spill to [esp+0x10] for the block that
// later borrows ebx as a cursor). With plain array indexing every instruction
// is identical but for that missing `mov ebx, ecx` and the rotation it causes,
// which is 74.9%. The outer loops stay plain array indexing, which is what
// gives the `add esi, 0x1044` after the loop guard.
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
        Player_00461630* p = &g_game->players[0];
        for (int j = 0; j < 10; j++, p++) {
            if (p->color == channels[i].field_14) {
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

// The last remaining hunk was the allocation-failure epilogue: the original
// ends with `pop edi; pop esi; xor eax, eax; pop ebx; ret 8` while every
// helper based shape emitted `xor eax, eax; pop edi; pop esi; pop ebx; ret 8`.
// The fix is the shape the matched sibling 0x461020 uses: the whole body after
// the network check is a flat `do { ... } while (0)` region whose exit is
// BEFORE the SetThreadPriority call, and the failure `return 0` lives inside
// the region. C2 then moves that `return 0` out to the end of the function and
// emits it as the function's last block, and the last block is the one that
// puts the register restores before the return value. Without the `while (0)`
// region (or with the failure return written as an out-of-line helper's
// `return 0`, which was the previous 98.6% version) the network check's
// `return 0` is either merged into the failure block or duplicated with the
// xor first. The region must end before SetThreadPriority: putting the call
// inside the region adds a third edge into the `return 1` epilogue and C2
// duplicates that epilogue.
// FUNCTION: 0x461750
int PacketManager::InitChannels(int arg1, int arg2)
{
    if (g_usePacketManager == 0) {
        return 0;
    }
    do {
        if (member.field_18 == 0) {
            if (member.field_1c != 0) {
                member.field_18 = member.field_1c;
                member.field_1c = 0;
            } else {
                member.field_18 = operator new(0x42a);
                if (member.field_18 == 0) {
                    return 0;
                }
                member.field_228 = 0x42a;
            }
        }
        member.field_22c = 0;
        if (member.field_14 != 0) {
            *(int*)((char*)member.field_14 + 0xc) = 0;
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
int PacketManager::FUN_00461900(int from, Msg_00461900* msg)
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
