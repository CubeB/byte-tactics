// Decompiled by Opus, Sonnet, Haiku, deepseek-v4.1-flash and Claude Opus 5.5. Names are provisional.
// The network statistics module: a bit writer and reader, and the counters and
// text that report packet and byte rates.

// This header set must stay: FormatNetStats needs it.
#include <windows.h>
#include <stdio.h>
#include <ddraw.h>
// Included only for its symbol ids: FormatNetStats matches at this count with
// the BitReader header's inline ReadBit (see bit_reader.h).
#include <io.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x148e7];
    int count;                         // +0x148e7
    void** items;                      // +0x148eb
    char unknown_148ef[0x38a47 - 0x148ef];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

struct Pair_00419560 {
    int a;
    int b;
};

// A bit writer with a 0x100-dword inline buffer (a 0x410-byte stack object in
// 0x48b710). The read counterpart is 0x415dc0.
class BitWriter {
public:
    int bit;                           // +0x0 current word index
    int index;                         // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10

    BitWriter();
    void FreeBuffer();
    void GrowBuffer();
    void WriteBits(int value, int bits);
    void SetByteAt(int offset, unsigned char value);
};

#include "bit_reader.h"

class PacketManager {
public:
    void NopRet_D();
};

void __stdcall RegisterOrderTypes(void* table, int id);
void __stdcall StepGafSequence(void* item);

unsigned int __cdecl GetTicks();

// GLOBAL: 0x511de8
extern Game* g_game;
extern char g_vtolOrders[];
extern int g_messageBytesReceived;
extern int g_messageBytesSent;
extern int g_packetBytesSent;
extern int DAT_00511c20;
extern int g_packetsSent;
extern int g_packetsReceived;
extern int g_packetBytesReceived;
extern int g_compressedBytesSent;
extern int g_messageCountByType[45][2];
extern int g_messageBytesByType[45][2];
extern unsigned int g_netStatsTick;
extern int g_lastMessageBytesReceived;
extern int g_lastMessageBytesSent;
extern int g_lastCompressedBytesSent;
extern int g_lastPacketBytesSent;
extern int g_lastPacketBytesReceived;
extern int g_lastPacketsSent;
extern int g_lastPacketsReceived;
extern unsigned int g_messageBytesReceivedRate;
extern unsigned int g_messageBytesSentRate;
extern int g_compressionPercent;
extern unsigned int g_packetBytesSentRate;
extern unsigned int g_packetBytesReceivedRate;
extern unsigned int g_packetsSentRate;
extern unsigned int g_packetsReceivedRate;
extern PacketManager g_packetManager;

// Registers a table with RegisterOrderTypes under a numeric id; one of several
// small functions doing the same for different tables.
// FUNCTION: 0x415b20
void RegisterVtolOrders()
{
    RegisterOrderTypes(g_vtolOrders, 0x16);
}

// FUNCTION: 0x415b30
void StepAllGafSequences(void)
{
    for (int i = g_game->count - 1; i >= 0; i--)
        StepGafSequence(g_game->items[i]);
}

// FUNCTION: 0x415b60
BitWriter::BitWriter()
{
    bit = 0;
    index = 0;
    capacity = 0x100;
    data = buffer;
    *data = 0;
}

// FUNCTION: 0x415b90
void BitWriter::FreeBuffer()
{
    if (data != buffer) {
        operator delete(data);
    }
}

static inline void CopyWords(unsigned int* first, unsigned int* last, unsigned int* dst)
{
    for (; first != last; first++) {
        *dst = *first;
        dst++;
    }
}

// The original allocates with `new unsigned int(n)` (one dword holding n)
// instead of `new unsigned int[n]`, and frees with `delete` rather than
// `delete[]` (docs/bugs.md).
// FUNCTION: 0x415bb0
void BitWriter::GrowBuffer()
{
    int newCapacity = capacity * 2;
    unsigned int* grown = new unsigned int(newCapacity);
    CopyWords(data, data + capacity, grown);
    if (data != buffer) {
        delete data;
    }
    capacity = newCapacity;
    data = grown;
}

// Appends the low `bits` bits of `value`, lowest bits first. When the word is
// full it may spill into the next word and double the buffer.
// FUNCTION: 0x415c10
void BitWriter::WriteBits(int value, int bits)
{
    if (bits + index < 0x20) {
        data[bit] |= (value & ((1 << bits) - 1)) << index;
        index += bits;
        return;
    }
    if (index == 0) {
        data[bit] = value;
        bit++;
        if (bit == capacity) {
            GrowBuffer();
        }
        data[bit] = 0;
        return;
    }
    int n = 0x20 - index;
    data[bit] |= (value & ((1 << n) - 1)) << index;
    bit++;
    if (bit == capacity) {
        GrowBuffer();
    }
    index = bits - n;
    data[bit] = ((unsigned int)value >> n) & ((1 << index) - 1);
}

// FUNCTION: 0x415da0
void BitWriter::SetByteAt(int offset, unsigned char value)
{
    ((unsigned char*)data)[offset] = value;
}

// FUNCTION: 0x415dc0
int BitReader::ReadBits(int bits)
{
    if (bit + bits < 32) {
        int r = (data[index] >> bit) & ((1 << bits) - 1);
        bit += bits;
        return r;
    }
    if (bit == 0) {
        return data[index++];
    }
    int low = 32 - bit;
    int r = (data[index] >> bit) & ((1 << low) - 1);
    bit = bits - low;
    index++;
    r |= (data[index] & ((1 << bit) - 1)) << low;
    return r;
}

// Reads a signed bit field: ReadBits's value, sign-extended from `bits`.
// FUNCTION: 0x415e60
int BitReader::ReadSignedBits(int bits)
{
    int r = ReadBits(bits);
    if (r & (1 << (bits - 1))) {
        r |= -1 << bits;
    }
    return r;
}

// Same reset sequence as InitCommands and CmdNetStats.
// FUNCTION: 0x415e90
void ResetNetStats()
{
    DAT_00511c20 = g_game->ticks;
    g_messageBytesReceived = 0;
    g_messageBytesSent = 0;
    Pair_00419560* p = (Pair_00419560*)&g_messageBytesByType[1];
    for (int i = 0; i < 44; i++) {
        g_messageCountByType[i + 1][0] = 0;
        g_messageCountByType[i + 1][1] = 0;
        g_messageBytesByType[i + 1][0] = 0;
        p->b = 0;
        p++;
    }
    g_packetsSent = 0;
    g_packetBytesSent = 0;
    g_packetsReceived = 0;
    g_packetBytesReceived = 0;
}

// Adds an amount to the player's total (g_messageBytesReceived) and, for kinds 2..44,
// bumps the per-kind count and total (the tables ResetNetStats resets from
// kind 1, g_messageCountByType and g_messageBytesByType).
// FUNCTION: 0x415ef0
void __stdcall CountMessage(unsigned char kind, int amount, int player)
{
    (&g_messageBytesReceived)[player] += amount;
    if (kind > 1 && kind < 0x2d) {
        g_messageCountByType[kind][player]++;
        g_messageBytesByType[kind][player] += amount;
    }
}

// Network statistics: counts a packet of `size` bytes as sent or received
// (the counters reset by 0x415e90) and adds any positive overhead.
// FUNCTION: 0x415f40
void __stdcall CountPacket(int size, int overhead, int sent)
{
    if (overhead > 0) {
        g_compressedBytesSent += overhead;
    }
    if (sent != 0) {
        g_packetsSent++;
        g_packetBytesSent += size;
    } else {
        g_packetsReceived++;
        g_packetBytesReceived += size;
    }
}

// Network statistics text: about once a second (more than 30 ticks), turns
// the packet and byte counters (see 0x415f40 and 0x416150) into per-second
// rates and a compression percentage, then prints the latest values.
// FUNCTION: 0x415fa0
void __stdcall FormatNetStats(char* text)
{
    unsigned int now = GetTicks();
    unsigned int elapsed = now - g_netStatsTick;
    if (elapsed > 30) {
        g_netStatsTick = now;
        g_messageBytesReceivedRate = (g_messageBytesReceived * 30 - g_lastMessageBytesReceived * 30) / elapsed;
        int packets = g_messageBytesSent - g_lastMessageBytesSent;
        g_messageBytesSentRate = packets * 30 / elapsed;
        // The previous counters are updated only after the rates are computed.
        g_lastMessageBytesReceived = g_messageBytesReceived;
        g_lastMessageBytesSent = g_messageBytesSent;
        if (packets > 0)
            g_compressionPercent = (g_lastCompressedBytesSent + packets - g_compressedBytesSent) * 100 / packets;
        else
            g_compressionPercent = 0;
        g_lastCompressedBytesSent = g_compressedBytesSent;
        g_packetBytesSentRate = (g_packetBytesSent * 30 - g_lastPacketBytesSent * 30) / elapsed;
        g_packetBytesReceivedRate = (g_packetBytesReceived * 30 - g_lastPacketBytesReceived * 30) / elapsed;
        g_packetsSentRate = (g_packetsSent * 30 - g_lastPacketsSent * 30) / elapsed;
        g_packetsReceivedRate = (g_packetsReceived * 30 - g_lastPacketsReceived * 30) / elapsed;
        g_lastPacketBytesSent = g_packetBytesSent;
        g_lastPacketBytesReceived = g_packetBytesReceived;
        g_lastPacketsSent = g_packetsSent;
        g_lastPacketsReceived = g_packetsReceived;
    }
    sprintf(text, "pS=%4d pR=%4d (S=%d/%4d, R=%d/%4d) C=%3d%%\n", g_messageBytesSentRate, g_messageBytesReceivedRate,
            g_packetsSentRate, g_packetBytesSentRate, g_packetsReceivedRate, g_packetBytesReceivedRate, g_compressionPercent);
}

// 0x416150 (GetByteRates) stays in src/network/net_stats_416150.cpp: it
// matches only without <ddraw.h>, and FormatNetStats matches only with it, so
// the two cannot share a translation unit (tools/headers.py).

// The sums are never used (their consumer was presumably compiled out).
// As in InitCommands, the second field of the second table is read through a
// walking pointer.
// FUNCTION: 0x4161f0
void FUN_004161f0()
{
    int sum3 = 0, sum4 = 0, sum1 = 0, sum2 = 0;
    Pair_00419560* p = (Pair_00419560*)&g_messageBytesByType[1];
    for (int i = 0; i < 44; i++) {
        sum1 += g_messageCountByType[i + 1][0];
        sum2 += g_messageCountByType[i + 1][1];
        sum3 += g_messageBytesByType[i + 1][0];
        sum4 += p->b;
        p++;
    }
    g_packetManager.NopRet_D();
}
