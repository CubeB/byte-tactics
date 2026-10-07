// Decompiled by Opus, finished by GPT-6.1-sol, finished by Claude Sonnet 5.5. Names are provisional.
// BUG: `length` is uninitialized for method 0 and 3 (the switch only sets it for
// cases 1 and 2), yet `total = length + 0x13` uses it, so a valid method < 4 can
// compute a bogus compressed size from stack garbage.
// Builds a "SQSH" compressed chunk: header, then the (optionally
// compressed and encrypted) data.
#include <string.h>

#pragma pack(push, 1)
struct Chunk_4d1820 {
    int marker;                      // +0x00 "SQSH"
    unsigned char version;           // +0x04
    unsigned char method;            // +0x05
    unsigned char encrypt;           // +0x06
    int compressedSize;              // +0x07
    int size;                        // +0x0b
    int checksum;                    // +0x0f
};
#pragma pack(pop)

int __stdcall LzssCompress(char* out, char* in, int size);
void __stdcall FUN_004d1c80(char* out, int* outSize, char* in, int size);

static inline unsigned int encryptByte(unsigned int i, char* out) { return (i ^ out[i]) + i; }

// FUNCTION: 0x4d1820
int __stdcall SquashPack(Chunk_4d1820* chunk, int* chunkSize, char* data, int size, int method, int encrypt)
{
    Chunk_4d1820 header, * payload = chunk + 1;
    char* out = (char*)payload;
    if ((char*)payload == 0) {
        return 6;
    }
    if (data == 0) {
        return 6;
    }
    if (size == 0) {
        return 6;
    }
    if (method >= 4) {
        return 4;
    }
    // Left uninitialised: the original has no zeroing store here.
    int length;
    switch (method) {
    case 1:
        length = LzssCompress(out, data, size);
        break;
    case 2:
        length = *chunkSize;
        FUN_004d1c80(out, &length, data, size);
        break;
    }
    // total unsigned, test spelled (length + 0x13): sets the XOR operand order.
    unsigned int total = length + 0x13;
    if ((length + 0x13) > *chunkSize) {
        return 5;
    }
    if (encrypt) {
        // Helper call, index copies and do-while wrappers all stay: XOR operand order.
        do {
            do {
                for (unsigned int i = 0; i < length; i++) {
                    do {
                        unsigned int index = i, idx = index;
                        out[i] = encryptByte(idx, out);
                    } while (0);
                }
            } while (0);
        } while (0);
    }
    memcpy(&header.marker, "SQSH", 4);
    header.version = 2;
    header.method = method;
    header.encrypt = encrypt;
    header.compressedSize = length;
    header.size = size;
    int sum = 0;
    for (unsigned char* p = (unsigned char*)out; p < (unsigned char*)out + length; p++) {
        sum += *p;
    }
    header.checksum = sum;
    *chunk = header;
    *chunkSize = total;
    return 0;
}
