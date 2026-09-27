// Decompiled by Opus. Names are provisional.
// Codex / GPT-6 retest in #13:
// pointer-typed image bases, DWORD-sized arithmetic, a directory
// reference and an RVA helper did not fix the final eax/ecx operand order.
// Constructor of the loaded-image reader (the function-local static at
// 0x528a78, built by 0x4de0a0 from GetModuleHandle(0)): maps the module's
// own file through the memory-mapped-file base class (0x4e1560), then finds
// the image's debug directory, which 0x4ddfa0 and 0x4ddfe0 search for the FPO
// records.
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
// MSVC reload ntHeaders for the final sum, as the original does). Still
// different: the original loads imageBase into eax and the RVA into ecx for
// that sum; every spelling, type and header set tried gives the swapped pair.
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
        debugDirs = (IMAGE_DEBUG_DIRECTORY*)(ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress + imageBase);
}
