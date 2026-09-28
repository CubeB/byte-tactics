// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Find_004bc4b0 {
    char dir[0x100];      // +0x000
    char name[0x100];     // +0x100
    int state;            // +0x200
    char recursive;       // +0x204
    long handle;          // +0x205
    int index;            // +0x209
};
#pragma pack(pop)

void* FUN_004d83b0(char* name, unsigned int size);
void FUN_004d85a0(void* p);
int __stdcall FUN_004bc640(Find_004bc4b0* f, void* fd);
long __cdecl _findfirst(const char* spec, void* fileinfo);

// Strips the directory from a path in place ("a\\b\\c.txt" -> "c.txt").
static char* StripDir(char* path)
{
    int len = (int)strlen(path);
    int i = len - 1;
    while (i >= 0 && path[i] != '\\')
        i--;
    int j = i + 1;
    int k = 0;
    do {
        path[k] = path[j];
        k++;
    } while (path[j++] != 0);
    return path;
}

// FUNCTION: 0x4bc4b0
int __stdcall FUN_004bc4b0(const char* path, void* fd, int state, char recursive)
{
    Find_004bc4b0* f = (Find_004bc4b0*)FUN_004d83b0("Find Files structure", 0x20d);
    strcpy(f->dir, path);
    for (int i = strlen(f->dir); i >= 0; i--) {
        if (f->dir[i] == '\\') {
            f->dir[i + 1] = '\0';
            break;
        }
    }
    strcpy(f->name, path);
    StripDir(f->name);
    if (strcmp(f->name, "*.*") == 0)
        strcpy(f->name, "*");
    f->recursive = recursive;
    f->state = state;
    if (state == -1) {
        f->handle = _findfirst(path, fd);
        if (f->handle != -1)
            return (int)f;
        if (f->recursive == 0)
            goto fail;
        f->state = 0;
    }
    f->index = -1;
    if (FUN_004bc640(f, fd) != -1)
        return (int)f;
fail:
    FUN_004d85a0(f);
    return -1;
}
