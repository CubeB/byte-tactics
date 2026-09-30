// Decompiled by Opus, finished by GPT-6.1-sol and space-bunny-free. Names are provisional.
// Constructor of the loaded-image reader (the function-local static at
// 0x528a78, built by 0x4de0a0 from GetModuleHandle(0)): maps the module's
// own file through the memory-mapped-file base class (0x4e1560), then finds
// the image's debug directory, which 0x4ddfa0 and 0x4ddfe0 search for the FPO
// records.
// The last block's eax/ecx pair is won by writing the sum through a `char*`
// local assigned inside the if-body and read after a second test of
// numDebugDirs, not as one expression over the members (see the note below).
#include <windows.h>

class Class_004e1590 {
public:
    void FUN_004e1590(const char* fileName);
};

class Class_004e1560 {
public:
    void* hFile;      // +0x0
    void* hMapping;   // +0x4
    void* view;       // +0x8
    int size;         // +0xc
    int state;        // +0x10

    Class_004e1560(const char* fileName);
};

class Class_004ddf00 : public Class_004e1560 {
public:
    HMODULE module;                        // +0x14
    unsigned int imageBase;                // +0x18
    IMAGE_DOS_HEADER* dosHeader;           // +0x1c
    IMAGE_NT_HEADERS* ntHeaders;           // +0x20
    IMAGE_DEBUG_DIRECTORY* debugDirs;      // +0x24
    unsigned int numDebugDirs;             // +0x28

    Class_004ddf00(HMODULE m);
};

// The count must be stored before debugDirs is cleared (that order makes
// MSVC reload ntHeaders for the final sum, as the original does).
// `base` and the second `if (numDebugDirs)` are what pick the registers: the
// load of imageBase then lands in EAX and the data-directory RVA in ECX, so
// the sum is `add ecx, eax`, exactly as the original does. Written any other
// way (a local assigned before the if, a one-expression sum over the members,
// any cast of either operand) MSVC always gives the mirror image
// `mov ecx,[base] / mov eax,[rva] / add eax,ecx`, whatever the header set.
// FUNCTION: 0x4ddf00
Class_004ddf00::Class_004ddf00(HMODULE m) : Class_004e1560(0)
{
    char path[1000];
    module = m;
    imageBase = (unsigned int)m;
    dosHeader = (IMAGE_DOS_HEADER*)m;
    if (GetModuleFileNameA(m, path, sizeof(path)))
        path[sizeof(path) - 1] = 0;
    else
        path[0] = 0;
    ((Class_004e1590*)this)->FUN_004e1590(path);
    ntHeaders = (IMAGE_NT_HEADERS*)((char*)dosHeader + dosHeader->e_lfanew);
    numDebugDirs = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size / sizeof(IMAGE_DEBUG_DIRECTORY);
    debugDirs = 0;
    if (numDebugDirs)
    {
        char* base = (char*)imageBase;
        if (numDebugDirs)
            debugDirs = (IMAGE_DEBUG_DIRECTORY*)((char*)base + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
    }
}
