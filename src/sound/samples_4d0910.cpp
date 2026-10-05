// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol. Names are provisional.
// MATCH (deepseek-v4.1-flash). Fourteen earlier passes as a free __stdcall
// function stalled at 94.7% with exactly two preheader diffs: the `size + 8`
// temp sat in edi instead of the original's edx, and `mov edi, 0x14` was
// emitted one slot early (before the last push of the first strncmp).
// The fix is the same lever that matched the siblings 0x4d0720 and 0x4d07f0
// in this issue: the original is a member function with an unused `this` (a
// this-less __thiscall, docs/agent-guide.md 0x4c5b70). Declaring it as a class
// method changes the implicit `this` register's effect on the allocator, and
// with the `pos = 0x14` statement in its natural place after the len read the
// size update keeps its register form AND the copy lands after the last push,
// byte for byte as the original has it. The earlier passes could only trade
// one diff for the other because the free-function allocator reserved edi for
// pos as soon as pos was live.
//
// Walks a chunked file's marker table looking for the record tagged "data" and
// returns that record's 4 byte header field (the record length), or 0 when the
// walk runs past the table.
#include <string.h>

int __stdcall FUN_004bb710(void* file, int pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);

class Class_004d0910 {
public:
    int FindDataChunkSize(void* file);
};

// FUNCTION: 0x4d0910
int Class_004d0910::FindDataChunkSize(void* file)
{
    char tag[4];
    unsigned int size;
    unsigned int pos;
    unsigned int len;

    FUN_004bb710(file, 4);
    FUN_004bb7c0(file, &size, 4);
    size += 8;
    FUN_004bb710(file, 0xc);
    FUN_004bb7c0(file, tag, 4);
    FUN_004bb7c0(file, &len, 4);
    pos = 0x14;
    for (;;) {
        if (strncmp(tag, "data", 4) == 0)
            return len;
        FUN_004bb710(file, pos + len);
        pos += len;
        if (pos >= size)
            return 0;
        FUN_004bb7c0(file, tag, 4);
        FUN_004bb7c0(file, &len, 4);
        pos += 8;
    }
}
