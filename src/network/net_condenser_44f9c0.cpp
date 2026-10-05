// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
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
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
class NetCondenser {
public:
    char* buffer;                    // +0x0
    int unknown_4;                   // +0x4
    int unknown_8;                   // +0x8
    int unknown_c;                   // +0xc

    int ReceivePacket(void* net, char* data, int* size);
};
#pragma pack(pop)

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
