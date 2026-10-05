// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Closes a file handle opened by FUN_004bb2e0 and returns its size.
// Sibling of 0x4bbd00, which reports the size without closing.
#include <stdio.h>
#include <io.h>

struct Class_004bbc40_File
{
    char unknown_0[0x10];
    int fd; // +0x10
};

struct Class_004bbc40_Other
{
    char unknown_0[4];
    long value; // +0x4
};

struct Class_004bbc40_Entry
{
    FILE* fp;       // +0x0
    int unknown_4;  // +0x4
    int unknown_8;  // +0x8
    int refcount;   // +0xc
    int unknown_10; // +0x10
};

struct Class_004bbc40
{
    Class_004bbc40_File* file;   // +0x0
    Class_004bbc40_Entry* entry; // +0x4
    Class_004bbc40_Other* other; // +0x8
    int unknown_c;               // +0xc
    int unknown_10;              // +0x10
    int unknown_14;              // +0x14
};

extern void* __stdcall FUN_004bb2e0(char* param_1, const char* param_2);
extern void __cdecl FUN_004d85a0(void* param_1);

// FUNCTION: 0x4bbc40
long __stdcall FUN_004bbc40(char* param_1)
{
    Class_004bbc40* h = (Class_004bbc40*)FUN_004bb2e0(param_1, "rb");
    if (h == 0)
        return 0;

    long len;
    if (h->entry != 0)
        len = h->other->value;
    else if (h->file != 0)
        len = _filelength(h->file->fd);
    else
        len = 0;

    if (h->entry != 0) {
        h->entry->refcount--;
        if (h->entry->refcount == 0 && h->entry->unknown_10 == 0) {
            fclose(h->entry->fp);
            h->entry->fp = 0;
        }
    } else {
        fclose((FILE*)h->file);
    }

    if (h->unknown_10 != 0)
        FUN_004d85a0((void*)h->unknown_10);
    if (h->unknown_14 != 0)
        FUN_004d85a0((void*)h->unknown_14);
    FUN_004d85a0(h);

    return len;
}
