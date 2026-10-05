// Decompiled by Opus. Names are provisional.
#include <stdio.h>

#pragma pack(push, 1)
struct PackHeader_004d1c00 {
    char unknown_0[5];
    unsigned char packType;            // +0x5
    unsigned char encryptType;         // +0x6
    long packedSize;                   // +0x7
    long unpackedSize;                 // +0xb
};
#pragma pack(pop)

// FUNCTION: 0x4d1c00
void __stdcall SquashDumpHeader(PackHeader_004d1c00* header)
{
    printf("\nheader.packType     = %d\n", header->packType);
    printf("header.encryptType  = %d\n", header->encryptType);
    printf("header.packedSize   = %ld\n", header->packedSize);
    printf("header.unpackedSize = %ld\n", header->unpackedSize);
}
