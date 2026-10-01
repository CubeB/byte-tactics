// Decompiled by Opus, finished by GPT-6.1-sol, finished by Claude Sonnet 5.5. Names are provisional.
// Retry (deepseek-v4.1-flash, issue 3061): 98.5%, 333 bytes exact. Sole
// residual is the commutative XOR operand order in the encrypt loop (original
// `mov bl,cl; xor bl,[ecx+esi]`, ours reversed). 30+ spellings compile
// memory-first under both sp3 and rtm; headers.py 128-set, dummy-declaration
// sweep 0..128, and prepending matched 0x4d1970 are all flat.
// BUG: `length` is uninitialized for method 0 and 3 (the switch only sets it for
// cases 1 and 2), yet `total = length + 0x13` uses it, so a valid method < 4 can
// compute a bogus compressed size from stack garbage; the original binary shows
// no zero-init store before the switch.
// Codex / GPT-6 retest in #13:
// byte temporaries, encoding helpers and the preceding compression
// entry points did not change the XOR operand order. The baseline remains
// 96.2%, with the original copying the index byte before XORing memory.
// GPT-6.1-sol retest: current best scores 98.5%. An explicit byte temporary
// produces the same code; the encrypt loop still reverses the XOR operands.
// GPT-6.1-sol stop note: byte-temporary and cursor-loop variants also scored
// 98.5%; original uses mov bl,cl then xor bl,[ecx+esi] in the encrypt loop.
// GPT-6.1-sol: an int accumulator caused a 66.9% register-allocation cascade;
// a byte helper and reversed xor operands both leave the 98.5% best unchanged.
// deepseek-v4.1-flash: full 128-set header sweep, inline-helper, two-statement
// temp and pointer-cursor rewrites are all inert; the single residual is the
// commutative XOR load order (ours loads [ecx+esi] first). This is the rare
// compiler-state tie from earlier functions in the original translation unit.
// deepseek-v4.1-flash, 30-min checkpoint (same 98.5%, 333 bytes): a systematic
// search found no source shape that emits the register-first form. Tried:
// ~1500 generated spellings of the loop (xor/plus operand order, casts to
// char/unsigned char/short, byte and int temps, compound assignment, comma,
// ternary, pointer and reference aliases of the index, bitfield/union reads,
// const and register locals, inlined per-byte and per-loop helpers, methods,
// do/while, goto, C vs C++ front end); every one loads [ecx+esi] into the
// destination first. The register-first form DOES appear in this compiler when
// the XOR's left operand is a loop-carried byte variable (a key assigned at the
// loop bottom, e.g. `c = i;` after the store): then MSVC keeps the key in its
// own byte register and emits `xor bl,[mem]`. In our builds MSVC folds a
// top-of-body `c = (unsigned char)i` into cl, which forces the memory-first
// form, so the original file's key variable must have survived in a register
// (state-dependent fold, not a source shape we can spell). Also flat: all 768
// headers.py sets, zlib.h, 0..8000 dummy prototypes, 0..320 extern ints, 240
// random declaration mixes, repeating a real function body 1..50 times, every
// real neighbour before/after this function in TU order (0x4d1480, 0x4d1670,
// 0x4d1800, 0x4d1810 are MATCH, 0x4d0f60 is the only partial one), /G6 /G5
// /Ot /Os /Oy /Oi /Ob1 /O1 and the RTM toolchain. The one thing not reproduced
// is the exact node/register state left by the original 0x4d0f60 body (84%
// partial, 1304 bytes, immediately before this function), which is the best
// lead for the next attempt.
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

static inline unsigned int encryptByte(unsigned int i, char* out) { return (i ^ out[i]) + i; }

// Claude Sonnet 5.5 (found with tools/permute.py): MATCH (was 98.5%). The XOR
// operand order came right once the encrypt byte went through an inline helper
// fed by a copy of the loop index inside nested do { } while (0) blocks, with
// the payload pointer (chunk + 1) in its own local and the size test spelled
// `(length + 0x13) > *chunkSize` while `total` is unsigned. Measured: without
// the payload local 98.5%; without the two index temporaries 98.5%; without
// the do-while wrappers 98.5%; comparing `total > *chunkSize` 97.7%; `total`
// as int 98.5%. The include change the permuter also made (memory.h) is not
// needed. The reason the wrappers matter is not understood (compiler state).
// FUNCTION: 0x4d1820
int __stdcall FUN_004d1820(Chunk_4d1820* chunk, int* chunkSize, char* data, int size, int method, int encrypt)
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
    unsigned int total = length + 0x13;
    if ((length + 0x13) > *chunkSize) {
        return 5;
    }
    if (encrypt) {
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
