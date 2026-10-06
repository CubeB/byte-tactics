// Decompiled by Opus, DeepSeek V4.1 Flash and Space Bunny Free. Names are provisional.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Net_0044f9c0 {
    char unknown_0[0x4b5];
    int field_4b5;                   // +0x4b5
};
#pragma pack(pop)

int __stdcall HAPINET_receivepacket(void* net, void* data, int* size);
void __stdcall CountPacket(int size, int overhead, int sent);
int __stdcall LzssExpand(char* out, char* in);
void LzssDisablePreset();
void LzssEnablePreset();

extern char* g_game;

int __stdcall LzssCompress(char* out, char* in, int size);
void LzssDisablePreset();
void LzssEnablePreset();
int __stdcall HAPINET_sendpacket(void* net, unsigned long from, unsigned long to, void* data, unsigned long size);
void __stdcall CountPacket(int size, int overhead, int sent);

#pragma pack(push, 1)
class NetCondenser {
public:
    char* buffer;                    // +0x0
    int unknown_4;                   // +0x4
    int unknown_8;                   // +0x8
    int unknown_c;                   // +0xc
    char* sendBuffer;                // +0x10
    char* buffer_14;                 // +0x14
    char* packet;                    // +0x18
    unsigned char sendCount;         // +0x1c
    int sendSize;                    // +0x1d
    int to;                          // +0x21

    NetCondenser();
    ~NetCondenser();
    void Accumulate(void* data, int size);
    int ReceivePacket(void* net, char* data, int* size);
    int SendPacket(void* session, int from);
    // 0x4626e0, in packets_4626e0.cpp: it was compiled with the packet code,
    // which calls SendPacket where this file would inline it.
    void SendPacketTo(void* session, int from, int value, void* data, int size);
};
#pragma pack(pop)

// Two global instances; the compiler builds their initialisers (0x44f720,
// 0x44f7e0) and atexit destructors (0x44f7a0, 0x44f860) with the bodies inlined.
// FUNCTION: 0x44f720 _$E4
// FUNCTION: 0x44f7a0 _$E2
NetCondenser g_receiveCondenser;

// FUNCTION: 0x44f7e0 _$E9
// FUNCTION: 0x44f860 _$E7
NetCondenser g_sendCondenser;

// FUNCTION: 0x44f8a0
NetCondenser::NetCondenser()
{
    buffer = new char[0x6d60];
    unknown_4 = 0;
    unknown_8 = 0;
    unknown_c = 0;
    sendBuffer = new char[0xaf0];
    packet = new char[0xaf0];
    buffer_14 = new char[0xaf0];
    sendCount = 0;
    sendSize = 0;
    to = 0;
}

// The out-of-line destructor of the net condenser class whose constructor is
// 0x44f8a0 (the global copies at 0x44f720/0x44f7e0 inline the same body).
// FUNCTION: 0x44f900
NetCondenser::~NetCondenser()
{
    delete[] buffer;
    delete[] sendBuffer;
    delete[] packet;
    delete[] buffer_14;
}

// Net condenser: copies an outgoing packet into the send buffer, growing it
// past its default BUFFER_ACC_SIZE (0xaf0) when needed.
// FUNCTION: 0x44f940
void NetCondenser::Accumulate(void* data, int size)
{
    char msg[256];
    if (size > 0xaf0) {
        delete sendBuffer;
        sendBuffer = new char[size];
        sprintf(msg, "netCondenser: BUFFER_ACC_SIZE = %ld, requested send size = %ld , resized buffer to accomodate.\n",
                0xaf0, size);
    }
    memcpy(sendBuffer, data, size);
    sendSize = size;
    sendCount++;
}

// A method of the net condenser (class layout from 0x44f8a0.cpp and
// 0x44f940.cpp): finishes receiving a packet. If a decompressed buffer is
// still pending it is copied out first; otherwise a DirectPlay packet is
// received, its header and XOR checksum are verified and the payload is
// decompressed (kind 4) or copied (kind 3).
//
// The `goto` at the tail is not dead: it gives the shared error test two
// predecessors (the 0x887700be path and the 0x8877001e path), which is what
// stops MSVC 5 from propagating the branch's known value into the test and
// folding the (always taken) compare away. Without it the tail loses the
// second `cmp esi, 0x8877001e`.
// FUNCTION: 0x44f9c0
int NetCondenser::ReceivePacket(void* net, char* data, int* size)
{
    char msg[256];
    unsigned int n;
    int clientSize = *size;

    if (unknown_8 != 0) {
        if (unknown_c > clientSize) {
            sprintf(msg, "netCondenser[1]: r_unpackedSize = %ld, client buffersize = %ld\n",
                    unknown_c, clientSize);
            unknown_8 = 1;
            *size = unknown_c;
            return 0x8877001e;
        }
        memcpy(data, buffer, unknown_c);
        *size = unknown_c;
        unknown_8 = 0;
        return 0;
    }

    int result = HAPINET_receivepacket(net, data, size);
    if (((Net_0044f9c0*)net)->field_4b5 != 0) {
        if (result == 0) {
            CountPacket(*size, 0, 0);
            if (data[0] != 3 && data[0] != 4)
                return 0;
            if (*size < 4)
                return 0x887700be;
            n = *size - 3;
            unsigned short sum = 0;
            for (unsigned int i = 3; i < n; i++) {
                sum += (unsigned char)data[i];
                data[i] ^= (unsigned char)i;
            }
            if (*(unsigned short*)(data + 1) != sum)
                return 0x887700be;
            if (data[0] == 4) {
                LzssEnablePreset();
                unknown_c = LzssExpand(buffer, data + 3);
                LzssDisablePreset();
            } else {
                memcpy(buffer, data + 3, n);
                unknown_c = n;
            }
            if (unknown_c > clientSize) {
                sprintf(msg, "netCondenser[2]: r_unpackedSize = %ld, client buffersize = %ld\n",
                        unknown_c, clientSize);
                unknown_8 = 1;
                *size = unknown_c;
                return 0x8877001e;
            }
            memcpy(data, buffer, unknown_c);
            *size = unknown_c;
            return 0;
        }
        if (result != 0x887700be) {
            if (result != 0x8877001e)
                return result;
        } else {
            goto common_check;
        }
common_check:
        if (result != 0x8877001e)
            return result;
        sprintf(msg, "netCondenser[3]: needed %ld, had client buffersize = %ld\n",
                clientSize, *size);
    }
    return result;
}

// A method of the net condenser (class layout from 0x44f940.cpp and
// 0x44f8a0.cpp): builds the outgoing packet in packet, a 3 byte header
// (kind byte, 16 bit checksum) followed by the data XORed with its index,
// then hands it to DirectPlay, or drops it to simulate packet loss.
// FUNCTION: 0x44fc10
int NetCondenser::SendPacket(void* session, int from)
{
    unsigned int compressed;
    if (sendSize > 12) {
        LzssEnablePreset();
        compressed = LzssCompress(packet + 3, sendBuffer, sendSize);
        LzssDisablePreset();
    } else {
        compressed = 0xffff;
    }

    unsigned char flag;
    int total;
    if (compressed + 3 < sendSize && *(int*)(g_game + 0x4ed) == 0) {
        flag = 4;
        total = compressed + 3;
    } else {
        total = sendSize + 3;
        flag = 3;
        memcpy(packet + 3, sendBuffer, sendSize);
    }

    // Suspected original bug: the data spans packet[3 .. total-1], so this
    // stops three bytes early. The last three data bytes go out unencrypted
    // and are left out of the checksum. Kept as the original has it.
    unsigned short sum = 0;
    for (int i = 3; i < total - 3; i++) {
        packet[i] ^= (unsigned char)i;
        sum += (unsigned char)packet[i];
    }
    packet[0] = flag;
    *(unsigned short*)(packet + 1) = sum;

    // A simulated packet loss: the packet is thrown away unless the dice say so.
    int result;
    if (*(int*)(g_game + 0x37f35) != 0 && (int)((__int64)rand() * 101 / 32768) <= *(int*)(g_game + 0x37f35))
        result = 0;
    else
        result = HAPINET_sendpacket(session, from, to, packet, total);
    if (result == 0)
        CountPacket(sendSize + 3, total, 1);
    sendSize = 0;
    sendCount = 0;
    return result;
}
