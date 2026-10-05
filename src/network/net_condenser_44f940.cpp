// Decompiled by Opus. Names are provisional.
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)
class Class_0044f940 {
public:
    char* buffer;                    // +0x0
    int unknown_4;                   // +0x4
    int unknown_8;                   // +0x8
    int unknown_c;                   // +0xc
    char* sendBuffer;                // +0x10
    char* buffer_14;                 // +0x14
    char* buffer_18;                 // +0x18
    unsigned char sendCount;         // +0x1c
    int sendSize;                    // +0x1d

    void FUN_0044f940(void* data, int size);
};
#pragma pack(pop)

// Net condenser: copies an outgoing packet into the send buffer, growing it
// past its default BUFFER_ACC_SIZE (0xaf0) when needed.
// FUNCTION: 0x44f940
void Class_0044f940::FUN_0044f940(void* data, int size)
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
