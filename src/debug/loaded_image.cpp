// Decompiled by Opus, GPT-6.1-sol and space-bunny-free. Names are provisional.

#include <windows.h>

class Class_004e1590 {
public:
    void OpenMappedFile(const char* fileName);
};

class MappedFile {
public:
    HANDLE hFile;     // +0x0
    HANDLE hMapping;  // +0x4
    void* view;       // +0x8
    int size;         // +0xc
    int state;        // +0x10

    MappedFile(const char* fileName);
};

// One FPO_DATA record (see 0x4de020).
struct Fpo_004de020 {
    unsigned int offStart;             // +0x0
    unsigned int procSize;             // +0x4
    unsigned int locals;               // +0x8
    unsigned short params;             // +0xc
    unsigned short flags;              // +0xe
};

class LoadedImage : public MappedFile {
public:
    HMODULE module;                        // +0x14
    unsigned int imageBase;                // +0x18
    IMAGE_DOS_HEADER* dosHeader;           // +0x1c
    IMAGE_NT_HEADERS* ntHeaders;           // +0x20
    IMAGE_DEBUG_DIRECTORY* debugDirs;      // +0x24
    int numDebugDirs;                      // +0x28

    LoadedImage(HMODULE m);
    Fpo_004de020* GetFpoRecords();
};

// Constructor of the loaded-image reader (the function-local static at
// 0x528a78, built by 0x4de0a0 from GetModuleHandle(0)): maps the module's
// own file through the memory-mapped-file base class (0x4e1560), then finds
// the image's debug directory, which GetFpoRecords and 0x4ddfe0 search for the FPO
// records.
// FUNCTION: 0x4ddf00
LoadedImage::LoadedImage(HMODULE m) : MappedFile(0)
{
    char path[1000];
    module = m;
    imageBase = (unsigned int)m;
    dosHeader = (IMAGE_DOS_HEADER*)m;
    if (GetModuleFileNameA(m, path, sizeof(path)))
        path[sizeof(path) - 1] = 0;
    else
        path[0] = 0;
    ((Class_004e1590*)this)->OpenMappedFile(path);
    ntHeaders = (IMAGE_NT_HEADERS*)((char*)dosHeader + dosHeader->e_lfanew);
    numDebugDirs = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size / sizeof(IMAGE_DEBUG_DIRECTORY);
    // Count stored before debugDirs is cleared: ntHeaders is reloaded for the sum.
    debugDirs = 0;
    if (numDebugDirs)
    {
        // base local and the second test pick the registers of the sum.
        char* base = (char*)imageBase;
        if (numDebugDirs)
            debugDirs = (IMAGE_DEBUG_DIRECTORY*)((char*)base + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
    }
}

// Returns the image's FPO records (debug directory type 3,
// IMAGE_DEBUG_TYPE_FPO), or 0 if it has none.
// FUNCTION: 0x4ddfa0
Fpo_004de020* LoadedImage::GetFpoRecords()
{
    if (debugDirs == 0)
        return 0;
    for (int i = 0; i < numDebugDirs; i++) {
        if (debugDirs[i].Type == IMAGE_DEBUG_TYPE_FPO)
            return (Fpo_004de020*)((char*)view + debugDirs[i].PointerToRawData);
    }
    return 0;
}
