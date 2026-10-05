// Decompiled by space-bunny-free. Names are provisional.
// Loads a whole file into a named heap block: opens the name (falling back to
// an already loaded item through FUN_004bb2e0), takes the length from the
// shared item's info block or from the file descriptor, rewinds, copies the
// name into a local buffer, strips the directory from it (the same in-place
// strip as 0x4bb150, so the leaf name is what names the block), allocates the
// block, reads into it and stores the length through `size`. `size` is set to
// -1 first, and again after the open, so a caller that only wanted the size
// sees a failure. The tail is FUN_004bb5d0 (close, release the two block
// pointers, free the handle) written out inline, with its fclose results dead
// because the return value is the block.
// Two shapes had to be spelled exactly this way:
//   * the length/rewind/read tests are an `if / else if / else` chain with the
//     failure arms as the earlier branches. Written as a chain of `if` blocks
//     with gotos, or with the success tests first, MSVC 5 merges the two
//     identical `data = 0; goto close` blocks and lays the whole success path
//     out as the fallthrough, which is the mirror image of the original.
//   * the in-place strip is two indices into the same array, copied from 0x4bb150.
//     Pointer versions let MSVC address the destination as an offset from the
//     source and peel the first iteration, which the original does not do.
// The `name` argument is enregistered in edi, the handle in ebp, the length in
// ebx and `data` shares esi with the `size` argument, which is reloaded from
// its stack slot for its last use.
#include <stdio.h>
#include <string.h>
#include <io.h>

struct Info_004bbe50 {
    int unknown_0;
    int size;
};

struct Shared_004bbe50 {
    FILE* fp;                 // +0x0
    int pos;                  // +0x4
    void* node;               // +0x8
    int refCount;             // +0xc
    int field_10;             // +0x10
};

struct File_004bbe50 {
    FILE* fp;                 // +0x0
    Shared_004bbe50* shared;  // +0x4
    Info_004bbe50* info;      // +0x8
    int pos;                  // +0xc
    void* buffer;             // +0x10
    void* buffer2;            // +0x14
};

File_004bbe50* __stdcall FUN_004bb2e0(char* filename, const char* mode);
long __stdcall FUN_004bb710(File_004bbe50* file, long pos);
int __stdcall FUN_004bb7c0(void* file, void* buf, int size);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4bbe50
char* __stdcall FUN_004bbe50(char* name, int* size)
{
    char buf[0x100];
    char* data = 0;
    File_004bbe50* f;
    int len;

    if (size) {
        *size = -1;
    }
    f = FUN_004bb2e0(name, "rb");
    if (!f) {
        return 0;
    }
    if (size) {
        *size = -1;
    }
    if (f->shared) {
        len = f->info->size;
    } else if (f->fp) {
        len = _filelength(_fileno(f->fp));
    } else {
        len = 0;
    }
    if (len <= 0) {
        data = 0;
    } else if (FUN_004bb710(f, 0) == -1) {
        data = 0;
    } else {
        strcpy(buf, name);
        {
            int n = (int)strlen(buf);
            int i = n - 1;
            while (i >= 0 && buf[i] != '\\') {
                i--;
            }
            int j = i + 1;
            int k = 0;
            do {
                buf[k] = buf[j];
                k++;
            } while (buf[j++] != 0);
        }
        data = (char*)FUN_004d83b0(buf, len);
        if (FUN_004bb7c0(f, data, len) <= 0) {
            FUN_004d85a0(data);
            data = 0;
            goto close;
        }
        if (size) {
            *size = len;
        }
    }
close:
    if (f->shared != 0) {
        f->shared->refCount--;
        if (f->shared->refCount == 0 && f->shared->field_10 == 0) {
            fclose(f->shared->fp);
            f->shared->fp = 0;
        }
    } else {
        fclose(f->fp);
    }
    if (f->buffer != 0) {
        FUN_004d85a0(f->buffer);
    }
    if (f->buffer2 != 0) {
        FUN_004d85a0(f->buffer2);
    }
    FUN_004d85a0(f);
    return data;
}
