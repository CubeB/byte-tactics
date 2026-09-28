// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Enumerates the directory entries matching path and inserts each name (except
// "." and "..", and, when param_2 is set, only directories) into the vector-like
// container param_3 through its out-of-line insert at 0x4be6c0. The handle is a
// directory search built by 0x4bc4b0 and advanced by 0x4bc640, and is closed by
// 0x4bc8d0, whose body is inlined at the tail.
#include <string.h>
#include <io.h>

struct FindData_004bca30 {
    int attr;                          // +0x0
    char unknown_4[0x10];
    char name[260];                    // +0x14
};

#pragma pack(push, 1)
struct Find_004bc8d0 {
    char unknown_0[0x200];
    int state;                         // +0x200 (negative while a search is open)
    char unknown_204;
    long handle;                       // +0x205
};
#pragma pack(pop)

void FUN_004d85a0(int* param_1);

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { FUN_004c9390(); }
};

class Class_004be6c0 {
public:
    char unknown_0[4];
    int* _First;                       // +0x4
    int* _Last;                        // +0x8
    int* _End;                         // +0xc
    void FUN_004be6c0(int* pos, int n, const Class_004c91a0* value);
};

int __stdcall FUN_004bc4b0(const char* path, FindData_004bca30* fd, int a, int b);
int __stdcall FUN_004bc640(int handle, FindData_004bca30* fd);

// FUNCTION: 0x4bca30
void __stdcall FUN_004bca30(const char* path, int param_2, Class_004be6c0* param_3)
{
    FindData_004bca30 fd;
    int h = FUN_004bc4b0(path, &fd, -1, 1);
    if (h != -1) {
        do {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0
                && (param_2 == 0 || (fd.attr & 0x10) != 0)) {
                Class_004c91b0 key(fd.name);
                param_3->FUN_004be6c0(param_3->_Last, 1, &key);
            }
        } while (FUN_004bc640(h, &fd) != -1);
        if (h != 0) {
            Find_004bc8d0* f = (Find_004bc8d0*)h;
            if (f->state < 0)
                _findclose(f->handle);
            FUN_004d85a0((int*)f);
        }
    }
}
