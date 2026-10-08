// Decompiled by Opus, Sonnet, Haiku, deepseek-v4.1-flash and Claude Opus 5.5. Names are provisional.
// The network statistics module: a bit writer and reader, and the counters and
// text that report packet and byte rates.

// This header set must stay: FormatNetStats needs it.
#include <windows.h>
#include <stdio.h>
#include <ddraw.h>

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x148e7];
    int count;                         // +0x148e7
    void** items;                      // +0x148eb
    char unknown_148ef[0x38a47 - 0x148ef];
    int field_38a47;                   // +0x38a47
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

// Reads bit fields from an array of dwords, lowest bits first.
class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int ReadBits(int bits);
    int ReadSignedBits(int bits);
};

class PacketManager {
public:
    void FUN_00461610();
};

void __stdcall RegisterOrderTypes(void* table, int id);
void __stdcall StepGafSequence(void* item);

unsigned int __cdecl GetTicks();

// GLOBAL: 0x511de8
extern Game* g_game;
extern char g_vtolOrders[];
extern int DAT_00511bc0;
extern int DAT_00511bc4;
extern int DAT_00511bc8;
extern int DAT_00511c20;
extern int DAT_00511c34;
extern int DAT_00511c48;
extern int DAT_00511c50;
extern int g_compressedBytesSent;
extern int g_messageCountByType[45][2];
extern int g_messageBytesByType[45][2];
extern Pair_00419560 DAT_00511a60[44];
extern Pair_00419560 DAT_00511c60[44];
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
    DAT_00511c20 = g_game->field_38a47;
    DAT_00511bc0 = 0;
    DAT_00511bc4 = 0;
    Pair_00419560* p = DAT_00511c60;
    for (int i = 0; i < 44; i++) {
        DAT_00511a60[i].a = 0;
        DAT_00511a60[i].b = 0;
        DAT_00511c60[i].a = 0;
        p->b = 0;
        p++;
    }
    DAT_00511c34 = 0;
    DAT_00511bc8 = 0;
    DAT_00511c48 = 0;
    DAT_00511c50 = 0;
}

// Adds an amount to the player's total (DAT_00511bc0) and, for kinds 2..44,
// bumps the per-kind count and total (the tables ResetNetStats resets from
// kind 1, at DAT_00511a60 and DAT_00511c60).
// FUNCTION: 0x415ef0
void __stdcall CountMessage(unsigned char kind, int amount, int player)
{
    (&DAT_00511bc0)[player] += amount;
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
        DAT_00511c34++;
        DAT_00511bc8 += size;
    } else {
        DAT_00511c48++;
        DAT_00511c50 += size;
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
        g_messageBytesReceivedRate = (DAT_00511bc0 * 30 - g_lastMessageBytesReceived * 30) / elapsed;
        int packets = DAT_00511bc4 - g_lastMessageBytesSent;
        g_messageBytesSentRate = packets * 30 / elapsed;
        // The previous counters are updated only after the rates are computed.
        g_lastMessageBytesReceived = DAT_00511bc0;
        g_lastMessageBytesSent = DAT_00511bc4;
        if (packets > 0)
            g_compressionPercent = (g_lastCompressedBytesSent + packets - g_compressedBytesSent) * 100 / packets;
        else
            g_compressionPercent = 0;
        g_lastCompressedBytesSent = g_compressedBytesSent;
        g_packetBytesSentRate = (DAT_00511bc8 * 30 - g_lastPacketBytesSent * 30) / elapsed;
        g_packetBytesReceivedRate = (DAT_00511c50 * 30 - g_lastPacketBytesReceived * 30) / elapsed;
        g_packetsSentRate = (DAT_00511c34 * 30 - g_lastPacketsSent * 30) / elapsed;
        g_packetsReceivedRate = (DAT_00511c48 * 30 - g_lastPacketsReceived * 30) / elapsed;
        g_lastPacketBytesSent = DAT_00511bc8;
        g_lastPacketBytesReceived = DAT_00511c50;
        g_lastPacketsSent = DAT_00511c34;
        g_lastPacketsReceived = DAT_00511c48;
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
    Pair_00419560* p = DAT_00511c60;
    for (int i = 0; i < 44; i++) {
        sum1 += DAT_00511a60[i].a;
        sum2 += DAT_00511a60[i].b;
        sum3 += DAT_00511c60[i].a;
        sum4 += p->b;
        p++;
    }
    g_packetManager.FUN_00461610();
}
