// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// A method of the net condenser (class layout from 0x44f8a0.cpp and
// 0x44f940.cpp): finishes receiving a packet. If a decompressed buffer is
// still pending it is copied out first; otherwise a DirectPlay packet is
// received, its header and XOR checksum are verified and the payload is
// decompressed (kind 4) or copied (kind 3).
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
class Class_0044f9c0 {
public:
    char* buffer;                    // +0x0
    int unknown_4;                   // +0x4
    int unknown_8;                   // +0x8
    int unknown_c;                   // +0xc

    int FUN_0044f9c0(void* net, char* data, int* size);
};
#pragma pack(pop)

int __stdcall FUN_004c9840(void* net, void* data, int* size);
void __stdcall FUN_00415f40(int size, int overhead, int sent);
int __stdcall FUN_004d1480(char* out, char* in);
void FUN_004d1800();
void FUN_004d1810();

// FUNCTION: 0x44f9c0
int Class_0044f9c0::FUN_0044f9c0(void* net, char* data, int* size)
{
    char msg[256];
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

    int result = FUN_004c9840(net, data, size);
    if (*(int*)((char*)net + 0x4b5) != 0) {
        if (result == 0) {
            FUN_00415f40(*size, 0, 0);
            if (data[0] != 3 && data[0] != 4)
                return 0;
            if (*size < 4)
                return 0x887700be;
            unsigned int n = *size - 3;
            unsigned short sum = 0;
            for (unsigned int i = 3; i < n; i++) {
                sum += (unsigned char)data[i];
                data[i] ^= (unsigned char)i;
            }
            if (*(unsigned short*)(data + 1) != sum)
                return 0x887700be;
            if (data[0] == 4) {
                FUN_004d1810();
                unknown_c = FUN_004d1480(buffer, data + 3);
                FUN_004d1800();
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
        if (result == 0x887700be) {
            if (result == 0x8877001e) {
                sprintf(msg, "netCondenser[3]: needed %ld, had client buffersize = %ld\n",
                        *size, clientSize);
            }
            return result;
        }
        if (result == 0x8877001e) {
            sprintf(msg, "netCondenser[3]: needed %ld, had client buffersize = %ld\n",
                    *size, clientSize);
        }
    }
    return result;
}
