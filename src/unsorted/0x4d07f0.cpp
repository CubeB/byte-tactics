// Decompiled by LongCat 2.5 Preview Free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, finished by deepseek-v4.1-flash. Names are provisional.
// MATCH. Eleven passes as a free __stdcall function stalled at 88.7% with the
// same three hunks (the `total + 8` temp in edi against the original's edx,
// `mov edi, 0x14` one slot early, and both epilogue pops hoisted to the top of
// the tail). The fix is the same lever that matched the sibling 0x4d0720: the
// original is a member function with an unused `this` (a this-less __thiscall,
// docs/agent-guide.md 0x4c5b70). Declaring it as a class method took it to
// 95.9% and fixed the whole epilogue (both pops now interleave with the three
// fmt stores exactly as the original has them, at the +8 displacements), so
// the third hunk was never a scheduler tie: it followed the implicit `this`'s
// effect on the allocator.
// The last hunk was the preheader schedule. In the free-function form the
// `pos = 0x14` statement had to sit BETWEEN the tag and len reads to keep the
// read modify write of `total` unfolded, but that placement also kept the temp
// in edi and emitted `mov edi, 0x14` before the last strncmp push. In the
// member form the same source scores 95.9% either way, and moving `pos = 0x14`
// to AFTER the len read (its natural position in the source, just before the
// loop) reaches the original byte for byte: the RMW still survives unfolded and
// `mov edi, 0x14` lands after the last push, where the original schedules it.
// So the earlier passes' "statement after it" lever and this pass's placement
// are two spellings of the same block schedule, and the member declaration is
// what makes the late one reachable.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

class Class_004d07f0 {
public:
    int FUN_004d07f0(void* file, int* sampleRate, int* bitsPerSample, int* channels);
};

// FUNCTION: 0x4d07f0
int Class_004d07f0::FUN_004d07f0(void* file, int* sampleRate, int* bitsPerSample, int* channels)
{
    unsigned int total;
    char tag[4];
    unsigned int pos;
    unsigned int len;
    unsigned int n;
    char fmt[0x10];

    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &total, 4);
    total = total + 8;
    FUN_004bb710(file, 0xc);
    FUN_004bb7c0(file, tag, 4);
    FUN_004bb7c0(file, &len, 4);
    pos = 0x14;
    for (;;) {
        if (strncmp(tag, "fmt ", 4) == 0) {
            n = len;
            break;
        }
        FUN_004bb710(file, pos + len);
        pos += len;
        if (pos >= total) {
            n = 0;
            break;
        }
        FUN_004bb7c0(file, tag, 4);
        FUN_004bb7c0(file, &len, 4);
        pos += 8;
    }
    if (n < 0x10)
        return 0;
    FUN_004bb7c0(file, fmt, 0x10);
    *sampleRate = *(int*)(fmt + 4);
    *bitsPerSample = *(unsigned short*)(fmt + 14);
    *channels = *(unsigned short*)(fmt + 2);
    return 1;
}
