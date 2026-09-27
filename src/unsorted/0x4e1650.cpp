// Decompiled by Opus. Names are provisional.
// Closes a memory-mapped file (the wrapper built by 0x4e1560): unmaps the
// view, then closes the mapping and file handles.
#include <windows.h>

class Class_004e1650 {
public:
    HANDLE hFile;                      // +0x0
    HANDLE hMapping;                   // +0x4
    void* view;                        // +0x8

    void FUN_004e1650();
};

// FUNCTION: 0x4e1650
void Class_004e1650::FUN_004e1650()
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
