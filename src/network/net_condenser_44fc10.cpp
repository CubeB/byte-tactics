// Decompiled by Space Bunny Free. Names are provisional.
// A method of the net condenser (class layout from 0x44f940.cpp and
// 0x44f8a0.cpp): builds the outgoing packet in buffer_18, a 3 byte header
// (kind byte, 16 bit checksum) followed by the data XORed with its index,
// then hands it to DirectPlay, or drops it to simulate packet loss.
#include <stdlib.h>
#include <string.h>

extern char* g_game;

int __stdcall FUN_004d0f60(char* out, char* in, int size);
void FUN_004d1800();
void FUN_004d1810();
int __stdcall HAPINET_sendpacket(void* net, unsigned long from, unsigned long to, void* data, unsigned long size);
void __stdcall CountPacket(int size, int overhead, int sent);

#pragma pack(push, 1)
class Class_0044fc10 {
public:
    char unknown_0[0x10];
    char* sendBuffer;                  // +0x10
    char* unknown_14;                  // +0x14
    char* packet;                      // +0x18
    unsigned char flag_1c;             // +0x1c
    int size_1d;                       // +0x1d
    int to_21;                         // +0x21

    int SendPacket(void* session, int from);
};
#pragma pack(pop)

// FUNCTION: 0x44fc10
int Class_0044fc10::SendPacket(void* session, int from)
{
    unsigned int compressed;
    if (size_1d > 12) {
        FUN_004d1810();
        compressed = FUN_004d0f60(packet + 3, sendBuffer, size_1d);
        FUN_004d1800();
    } else {
        compressed = 0xffff;
    }

    unsigned char flag;
    int total;
    if (compressed + 3 < size_1d && *(int*)(g_game + 0x4ed) == 0) {
        flag = 4;
        total = compressed + 3;
    } else {
        total = size_1d + 3;
        flag = 3;
        memcpy(packet + 3, sendBuffer, size_1d);
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
        result = HAPINET_sendpacket(session, from, to_21, packet, total);
    if (result == 0)
        CountPacket(size_1d + 3, total, 1);
    size_1d = 0;
    flag_1c = 0;
    return result;
}
