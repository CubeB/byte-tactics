// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>
#include <io.h>

// The file-search data filled in by FUN_004bc640; the name follows the header.
struct FindData_004bcd10 {
    int flags;                          // +0x0
    char unknown_4[0x10];
    char name[260];                     // +0x14
};

// The opaque search state FUN_004bc4b0 allocates (same layout as 0x4bc8d0).
#pragma pack(push, 1)
struct FindHandle_004bcd10 {
    char unknown_0[0x200];
    int state;                          // +0x200, negative while the handle is open
    char unknown_204;
    long handle;                        // +0x205
};
#pragma pack(pop)

FindHandle_004bcd10* __stdcall FUN_004bc4b0(const char* path, FindData_004bcd10* fd, int a, int b);
int __stdcall FUN_004bc640(FindHandle_004bcd10* handle, FindData_004bcd10* fd);
void __cdecl FUN_004d85a0(FindHandle_004bcd10* p);

// Walks the search opened by FUN_004bc4b0 and stops on the `index`-th entry
// that is neither "." nor ".." (skipping entries that fail the flag test when
// `flag` is set). The close of the search is FUN_004bc8d0 inlined.
// FUNCTION: 0x4bcd10
void __stdcall FUN_004bcd10(FindData_004bcd10* fd, int index, const char* pattern, int flag)
{
    int count = 0;
    FindHandle_004bcd10* handle = FUN_004bc4b0(pattern, fd, -1, 1);
    if (handle != (FindHandle_004bcd10*)-1) {
        do {
            if (strcmp(fd->name, ".") != 0 && strcmp(fd->name, "..") != 0) {
                if (flag == 0 || (fd->flags & 0x10) != 0) {
                    if (count == index)
                        break;
                    count++;
                }
            }
        } while (FUN_004bc640(handle, fd) != -1);
        if (handle != 0) {
            if (handle->state < 0)
                _findclose(handle->handle);
            FUN_004d85a0(handle);
        }
    }
}
