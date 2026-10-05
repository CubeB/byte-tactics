// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Opens a memory-mapped file: create the file, map it read-only, then map a
// view. On each failure the handles are closed and a state code is stored
// (1 = file open failed, 2 = mapping failed, 3 = view failed).
#include <windows.h>

class Class_004e1590 {
public:
    HANDLE hFile;      // +0x0
    HANDLE hMapping;   // +0x4
    void* view;        // +0x8
    DWORD size;        // +0xc
    int state;         // +0x10

    void OpenMappedFile(const char* fileName);
};

// FUNCTION: 0x4e1590
void Class_004e1590::OpenMappedFile(const char* fileName)
{
    hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        state = 1;
        return;
    }
    size = GetFileSize(hFile, NULL);
    hMapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hMapping == NULL) {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        state = 2;
        return;
    }
    view = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if (view == NULL) {
        CloseHandle(hMapping);
        hMapping = NULL;
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
        state = 3;
    }
}
