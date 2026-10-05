// Decompiled by Opus. Names are provisional.
// Closes a memory-mapped file (the wrapper built by 0x4e1560): unmaps the
// view, then closes the mapping and file handles.
#include <windows.h>

class MappedFile {
public:
    HANDLE hFile;                      // +0x0
    HANDLE hMapping;                   // +0x4
    void* view;                        // +0x8

    void CloseMappedFile();
};

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
