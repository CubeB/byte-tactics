// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: 82.7% (273 of 275 bytes). Verified with the argument-list and callee
// ret-N check first: the signature is right (4 stack args, __stdcall, ret 0x10),
// FUN_004bb710 is ret 0x8 with 2 args, FUN_004bb7c0 is ret 0xc with 3 args, and
// strncmp is the usual cdecl. So the three remaining differences are scheduling
// ties, not an argument or type error:
// 1. "total + 8": the original computes it in a register
//    (mov edx,[esp+8]; add edx,8; mov [esp+0x10],edx) with the store after the
//    argument pushes; this source folds it to an in-place
//    "add dword ptr [esp+0x10], 8".
// 2. The loop exit: the original merges two paths into one check in a REGISTER
//    (success "mov eax,[esp+0x28]; jmp 0x4d08b0", failure "xor eax,eax",
//    merge "cmp eax,0x10"). Writing "len = 0; break;" forces a store to the len
//    slot, which the original has no instruction for. Introducing a separate
//    variable for the merged value costs an extra stack slot and drops the
//    score to 69.1%, so the phi node has to come from a construct that needs no
//    new local.
// 3. The original pops edi before writing the three output pointers; this source
//    pops esi first, so the epilogue's esp displacements sit 8 bytes lower.
// The function walks a RIFF/WAVE file looking for the "fmt " chunk, then reads
// 0x10 bytes of format data and extracts nSamplesPerSec, wBitsPerSample and
// nChannels from it.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

// FUNCTION: 0x4d07f0
int __stdcall FUN_004d07f0(void* file, int* sampleRate, int* bitsPerSample, int* channels)
{
    char tag[4];
    unsigned int total;
    unsigned int pos;
    unsigned int len;
    char fmt[0x10];

    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &total, 4);
    total = total + 8;
    FUN_004bb710(file, 0xc);
    FUN_004bb7c0(file, tag, 4);
    FUN_004bb7c0(file, &len, 4);
    pos = 0x14;
    while (strncmp(tag, "fmt ", 4) != 0) {
        FUN_004bb710(file, len + pos);
        pos += len;
        if (pos >= total) {
            len = 0;
            break;
        }
        FUN_004bb7c0(file, tag, 4);
        FUN_004bb7c0(file, &len, 4);
        pos += 8;
    }
    if (len < 0x10)
        return 0;
    FUN_004bb7c0(file, fmt, 0x10);
    *sampleRate = *(int*)(fmt + 4);
    *bitsPerSample = *(unsigned short*)(fmt + 14);
    *channels = *(unsigned short*)(fmt + 2);
    return 1;
}
