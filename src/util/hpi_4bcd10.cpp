// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>
#include <io.h>

// The file-search data filled in by HAPI_FindNext; the name follows the header.
struct FindData_004bcd10 {
    int flags;                          // +0x0
    char unknown_4[0x10];
    char name[260];                     // +0x14
};

// The opaque search state HAPI_FindFirst allocates (same layout as 0x4bc8d0).
#pragma pack(push, 1)
struct FindFiles {
    char unknown_0[0x200];
    int state;                          // +0x200, negative while the handle is open
    char unknown_204;
    long handle;                        // +0x205
};
#pragma pack(pop)

FindFiles* __stdcall HAPI_FindFirst(const char* path, FindData_004bcd10* fd, int a, int b);
int __stdcall HAPI_FindNext(FindFiles* handle, FindData_004bcd10* fd);
void __cdecl FUN_004d85a0(FindFiles* p);

// Walks the search opened by HAPI_FindFirst and stops on the `index`-th entry
// that is neither "." nor ".." (skipping entries that fail the flag test when
// `flag` is set). The close of the search is HAPI_FindClose inlined.
// FUNCTION: 0x4bcd10
void __stdcall GetDirectoryEntry(FindData_004bcd10* fd, int index, const char* pattern, int flag)
{
    int count = 0;
    FindFiles* handle = HAPI_FindFirst(pattern, fd, -1, 1);
    if (handle != (FindFiles*)-1) {
        do {
            if (strcmp(fd->name, ".") != 0 && strcmp(fd->name, "..") != 0) {
                if (flag == 0 || (fd->flags & 0x10) != 0) {
                    if (count == index)
                        break;
                    count++;
                }
            }
        } while (HAPI_FindNext(handle, fd) != -1);
        if (handle != 0) {
            if (handle->state < 0)
                _findclose(handle->handle);
            FUN_004d85a0(handle);
        }
    }
}
