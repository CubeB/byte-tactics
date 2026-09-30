// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads a whole file into a freshly allocated buffer. It takes the length from
// the file handle (cached size when the handle is shared, else _filelength),
// rejects an empty file, seeks to the start, copies the path into a local
// buffer, strips the directory with a backward scan for '\\', allocates that
// many bytes tagged with the base name, reads the file into them and returns
// the buffer. The size is written back through the third argument, which is
// preset to -1 so a failure leaves it at -1.
//
// The base-name copy uses two indices into the same array, not pointers:
// pointer versions let MSVC address the destination as an offset from the
// source instead of keeping two walking pointers (same idiom as 0x4bb150).
#include <io.h>
#include <string.h>

struct Fp_004bbff0 {
    char unknown_0[0x10];
    int fd;                            // +0x10
};

struct Info_004bbff0 {
    char unknown_0[4];
    int size;                          // +0x4
};

struct File_004bbff0 {
    Fp_004bbff0* fp;                   // +0x0
    void* shared;                      // +0x4
    Info_004bbff0* info;               // +0x8
    unsigned int pos;                  // +0xc
    void* buffer;                      // +0x10
    void* buffer2;                     // +0x14
};

long __stdcall FUN_004bb710(File_004bbff0* file, long pos);
int __stdcall FUN_004bb7c0(File_004bbff0* file, void* buf, int size);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4bbff0
void* __stdcall FUN_004bbff0(char* name, File_004bbff0* file, unsigned int* outSize)
{
    long len;
    char buf[256];
    void* data;
    int n;
    int i;
    int j;
    int k;

    if (outSize != 0) {
        *outSize = 0xffffffff;
    }
    if (file->shared != 0) {
        len = file->info->size;
    } else if (file->fp != 0) {
        len = _filelength(file->fp->fd);
    } else {
        len = 0;
    }
    if (len <= 0) {
        return 0;
    }
    if (FUN_004bb710(file, 0) == -1) {
        return 0;
    }
    strcpy(buf, name);
    n = strlen(buf);
    i = n - 1;
    while (i >= 0 && buf[i] != '\\') {
        i--;
    }
    j = i + 1;
    k = 0;
    do {
        buf[k] = buf[j];
        k++;
    } while (buf[j++] != 0);
    data = FUN_004d83b0(buf, len);
    if (FUN_004bb7c0(file, data, len) <= 0) {
        FUN_004d85a0(data);
        return 0;
    }
    if (outSize != 0) {
        *outSize = len;
    }
    return data;
}