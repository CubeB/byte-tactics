// Decompiled by deepseek-v4.1. Names are provisional.
// Unpacks a "SQSH" chunk (see FUN_004d1820 for the packer).
// Best so far: 77.8% (307 bytes vs 296). The prologue and the 0x13-byte header
// copy now match: `int sum = 0;` must be declared at the very top with its
// initialiser for MSVC to home `data` in edx before the register pushes
// (sub esp,0x14 / mov edx,[esp+0x1c] / push ebx / mov eax,edx / push ebp),
// and the sum loop needs named src/end locals for the original
// lea esi,[..+edx] bound. Still differs: that top initialiser zeroes sum early
// in ebp where the original zeroes edi late (sum ends up ebp and compressedSize
// ebx, the original the other way round), the allocator rematerialises `data`
// from its stack slot in the case-2 memcmp instead of splitting an ebx copy
// (no `mov ebx,edx`), and the case-2 arm stores the destLen value in each
// branch instead of once in eax at the call setup. Tried, all worse or equal:
// sum without initialiser or declared later (71.2%), length at the top with
// and without an initialiser (71.2% / 58.7%), inline loop bound (76.6%),
// unsigned long length, inverted ternary, char* alias for the memcpy source or
// for the case-2 memcmp, plain memcpy vs struct assignment.
// Re-tried here with the same result: dropping the top initialiser and doing
// `sum = 0;` at the loop (73.0%) or declaring sum after src/end (73.0%) gives
// the original's ebp=compressedSize / edi=sum / `xor edi,edi` in the loop
// preheader, but the parameter then homes in ebx from entry (`push ebx` /
// `mov ebx,[esp+0x20]` / `mov eax,ebx`) instead of edx plus a later
// `mov ebx,edx`, and the header copy switches from ecx to edx as its temp.
// A `Chunk_4d1970*`/`char*` alias for the case-2 uses is coalesced away.
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

int __stdcall FUN_004d1480(unsigned char* dest, unsigned char* src);
int __stdcall _uncompress(unsigned char* dest, unsigned long* destLen, unsigned char* source, unsigned long sourceLen);

// FUNCTION: 0x4d1970
int __stdcall FUN_004d1970(char* dest, char* data)
{
    Chunk_4d1970 header;
    int sum = 0;
    memcpy(&header, data, 0x13);
    if (memcmp(&header, "SQSH", 4) != 0) {
        return 1;
    }
    if (header.method >= 4) {
        return 4;
    }
    unsigned char* src = (unsigned char*)(data + 0x13);
    unsigned char* end = src + header.compressedSize;
    for (unsigned char* p = src; p < end; p++) {
        sum += *p;
    }
    if (header.checksum != sum) {
        return 2;
    }
    if (header.encrypt) {
        for (unsigned int i = 0; i < header.compressedSize; i++) {
            data[0x13 + i] = (data[0x13 + i] - i) ^ i;
        }
    }
    int length;
    switch (header.method) {
    case 1:
        length = FUN_004d1480((unsigned char*)dest, (unsigned char*)(data + 0x13));
        break;
    case 2:
        length = memcmp(data, "SQSH", 4) == 0 ? *(int*)(data + 0xb) : 0;
        _uncompress((unsigned char*)dest, (unsigned long*)&length, (unsigned char*)(data + 0x13), header.compressedSize);
        break;
    }
    return (header.size - length) ? 3 : 0;
}
