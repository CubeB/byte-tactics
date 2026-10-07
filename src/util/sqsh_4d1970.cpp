// Decompiled by deepseek-v4.1, finished by Sonnet 5.5. Names are provisional.
// Unpacks a "SQSH" chunk (see SquashPack for the packer).
#include <string.h>

#pragma pack(push, 1)
struct Chunk_4d1970 {
    int marker;                      // +0x00 "SQSH"
    unsigned char version;           // +0x04
    unsigned char method;            // +0x05
    unsigned char encrypt;           // +0x06
    int compressedSize;              // +0x07
    int size;                        // +0x0b
    int checksum;                    // +0x0f
};
#pragma pack(pop)

int __stdcall LzssExpand(unsigned char* dest, unsigned char* src);
int __stdcall _uncompress(unsigned char* dest, unsigned long* destLen, unsigned char* source, unsigned long sourceLen);

// FUNCTION: 0x4d1970
int __stdcall SquashUnpack(char* dest, char* data)
{
    Chunk_4d1970 header;
    // Separate from data, which is advanced past the header: keeps the address.
    Chunk_4d1970* chunk = (Chunk_4d1970*)data;
    memcpy(&header, data, 0x13);
    if (memcmp(&header, "SQSH", 4) != 0) {
        return 1;
    }
    if (header.method >= 4) {
        return 4;
    }
    data += 0x13;
    unsigned char* end = (unsigned char*)data + header.compressedSize;
    int sum = 0;
    for (unsigned char* p = (unsigned char*)data; p < end; p++) {
        sum += *p;
    }
    if (header.checksum != sum) {
        return 2;
    }
    if (header.encrypt) {
        for (unsigned int i = 0; i < header.compressedSize; i++) {
            data[i] = (data[i] - i) ^ i;
        }
    }
    int length;
    switch (header.method) {
    case 1:
        length = LzssExpand((unsigned char*)dest, (unsigned char*)data);
        break;
    case 2:
        // Through a temporary: the `== 0 ? size : 0` form adds a jmp.
        { int t = memcmp(chunk, "SQSH", 4) ? 0 : chunk->size;
        length = t; }
        _uncompress((unsigned char*)dest, (unsigned long*)&length, (unsigned char*)data, header.compressedSize);
        break;
    }
    return (header.size - length) ? 3 : 0;
}