// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>
#include <io.h>

struct FindData_004bc930 {
    unsigned char attributes;          // +0x0
    char unknown_1[0x13];
    char name[260];                    // +0x14
};

#pragma pack(push, 1)
struct FindHandle_004bc930 {
    char unknown_0[0x200];
    int field_200;                     // +0x200
    unsigned char field_204;           // +0x204
    int field_205;                     // +0x205
};
#pragma pack(pop)

int __stdcall FUN_004bc4b0(const char* path, FindData_004bc930* fd, int a, int b);
int __stdcall FUN_004bc640(int handle, FindData_004bc930* fd);
void FUN_004d85a0(void* p);

// FUNCTION: 0x4bc930
int __stdcall FUN_004bc930(const char* path, int flag)
{
    FindData_004bc930 fd;
    int count = 0;
    int handle = FUN_004bc4b0(path, &fd, -1, 1);

    if (handle != -1) {
        do {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0
                && (flag == 0 || (fd.attributes & 0x10))) {
                count++;
            }
        } while (FUN_004bc640(handle, &fd) != -1);
        if (handle != 0) {
            FindHandle_004bc930* h = (FindHandle_004bc930*)handle;
            if (h->field_200 < 0)
                _findclose(h->field_205);
            FUN_004d85a0(h);
        }
    }
    return count;
}
