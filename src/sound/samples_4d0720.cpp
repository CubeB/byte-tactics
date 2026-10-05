// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol. Names are provisional.
// MATCH. Thirteen earlier passes left one two-instruction displacement in the
// rotated loop preheader: the original loads the `target` parameter into ebx
// before the `push 4` of the strncmp argument setup, while a free __stdcall
// function always emitted the same load one push later (its displacement is
// 0x20 against the original's 0x1c, which is the same fault). The lever is the
// calling convention: the original is a member function with an unused `this`
// (a `this`-less __thiscall, the same shape the guide records at 0x4c5b70).
// Declaring the function as a class method makes MSVC 5 hoist the parameter
// load to the top of the preheader, byte for byte as the original has it.
// Everything else (the frame, both epilogues, the rotated loop and the latch)
// was already exact, and no source shape of the free function could reach the
// load's position (the earlier passes swept declaration orders, loop shapes,
// target copies, headers and flags without moving it).
//
// Walks a chunked file's marker table: the header holds the table size (plus
// the 8 bytes of the two header fields) and the first marker, then the table
// is a run of [4 byte name][4 byte offset] pairs. Returns the offset of the
// named marker, or 0 when the table runs out.
#include <string.h>

int __stdcall HAPI_SeekFile(void* file, int pos);
int __stdcall HAPI_readfromfile(void* file, void* buf, int size);

class Class_004d0720 {
public:
    int FindChunkSize(void* file, char* target);
};

// FUNCTION: 0x4d0720
int Class_004d0720::FindChunkSize(void* file, char* target)
{
    char name[4];
    unsigned int total;
    unsigned int pos;
    unsigned int off;

    HAPI_SeekFile(file, 4);
    HAPI_readfromfile(file, &total, 4);
    total += 8;
    HAPI_SeekFile(file, 0xc);
    HAPI_readfromfile(file, name, 4);
    HAPI_readfromfile(file, &off, 4);
    pos = 0x14;
    for (;;) {
        if (strncmp(name, target, 4) == 0)
            return off;
        HAPI_SeekFile(file, pos + off);
        pos += off;
        if (pos >= total)
            return 0;
        HAPI_readfromfile(file, name, 4);
        HAPI_readfromfile(file, &off, 4);
        pos += 8;
    }
}
