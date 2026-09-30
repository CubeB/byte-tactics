// Decompiled by Opus, finished by GPT-6.1-sol. Names are provisional.
// Codex / GPT-6 retest in #13:
// byte temporaries, encoding helpers and the preceding compression
// entry points did not change the XOR operand order. The baseline remains
// 96.2%, with the original copying the index byte before XORing memory.
// GPT-6.1-sol retest: current best scores 98.5%. An explicit byte temporary
// produces the same code; the encrypt loop still reverses the XOR operands.
// GPT-6.1-sol stop note: byte-temporary and cursor-loop variants also scored
// 98.5%; original uses mov bl,cl then xor bl,[ecx+esi] in the encrypt loop.
// deepseek-v4.1-flash: full 128-set header sweep, inline-helper, two-statement
// temp and pointer-cursor rewrites are all inert; the single residual is the
// commutative XOR load order (ours loads [ecx+esi] first). This is the rare
// compiler-state tie from earlier functions in the original translation unit.
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

int __stdcall FUN_004d0f60(char* out, char* in, int size);
void __stdcall FUN_004d1c80(char* out, int* outSize, char* in, int size);

// FUNCTION: 0x4d1820
int __stdcall FUN_004d1820(Chunk_4d1820* chunk, int* chunkSize, char* data, int size, int method, int encrypt)
{
    Chunk_4d1820 header;
    char* out = (char*)(chunk + 1);
    if (out == 0) {
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
    int length;
    switch (method) {
    case 1:
        length = FUN_004d0f60(out, data, size);
        break;
    case 2:
        length = *chunkSize;
        FUN_004d1c80(out, &length, data, size);
        break;
    }
    int total = length + 0x13;
    if (total > *chunkSize) {
        return 5;
    }
    if (encrypt) {
        for (unsigned int i = 0; i < length; i++) {
            out[i] = (i ^ out[i]) + i;
        }
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
