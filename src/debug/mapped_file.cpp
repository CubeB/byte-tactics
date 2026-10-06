// Decompiled by Sonnet and Opus. Names are provisional.

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
    void CloseMappedFile();
};

// Constructor of a memory-mapped-file wrapper: initialises the handle/state
// fields, then opens the file if a name was given (see OpenMappedFile).
// FUNCTION: 0x4e1560
MappedFile::MappedFile(const char* fileName)
{
    hFile = (void*)-1;
    hMapping = 0;
    view = 0;
    size = 0;
    state = 0;
    if (fileName != 0)
        ((Class_004e1590*)this)->OpenMappedFile(fileName);
}

// Closes a memory-mapped file (the wrapper built by 0x4e1560): unmaps the
// view, then closes the mapping and file handles.
// FUNCTION: 0x4e1650
void MappedFile::CloseMappedFile()
{
    if (view != 0) {
        UnmapViewOfFile(view);
    }
    if (hMapping != 0) {
        CloseHandle(hMapping);
    }
    if (hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(hFile);
    }
}
