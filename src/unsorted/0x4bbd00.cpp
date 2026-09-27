// Decompiled by Sonnet. Names are provisional.
#include <io.h>

struct Class_004bbd00_Other
{
    char unknown_0[4];
    long value; // +0x4
};

struct Class_004bbd00_File
{
    char unknown_0[0x10];
    int fd; // +0x10
};

struct Class_004bbd00
{
    Class_004bbd00_File* file; // +0x0
    int flag;                  // +0x4
    Class_004bbd00_Other* other; // +0x8
};

// FUNCTION: 0x4bbd00
long __stdcall FUN_004bbd00(Class_004bbd00* param_1)
{
    if (param_1->flag != 0)
        return param_1->other->value;
    if (param_1->file != 0)
        return _filelength(param_1->file->fd);
    return 0;
}
