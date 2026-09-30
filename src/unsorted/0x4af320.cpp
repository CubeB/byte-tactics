// Decompiled by space-bunny-free, finished by space-bunny-free, finished by Sonnet 5.5. Names are provisional.
// MATCH. What the earlier 92.9 percent versions missed: the entry counter
// (num++) sits inside the "is a wanted entry" if of each loop (the name and
// attribute tests jump past it), and the close call FUN_004bc8d0 sits inside
// the `find != -1` guard together with the do/while (a failed search jumps
// over the close). The check.py diff normalises jump targets, so both showed
// up only as different jump distances (0x4af418 against 0x4af40b).
// Frame notes: one function scope search handle shared by both searches; the
// file walk runs in every case and continues the sizes list of the directory walk.
#include <stdlib.h>
#include <string.h>
#include <io.h>

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
int __stdcall FUN_004aefa0(char* names, char* sizes, void* times, int count);
int __stdcall FUN_004bc4b0(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall FUN_004bc640(int handle, struct _finddata_t* fd);
void __stdcall FUN_004bc8d0(int handle);
char* __stdcall FUN_004bb0f0(char* name);
long __stdcall FUN_004bbc40(char* name);

// FUNCTION: 0x4af320
int __stdcall FUN_004af320(char* path, char* list, char* sizes, int mode, int flag, int what)
{
    char* first = list;
    int num = 0;
    char* times = (char*)FUN_004d83b0("FILETIMES", 0x2ee0);
    long* tp = (long*)times;
    struct _finddata_t fd;
    char text[256];
    int find;

    if (mode == 1) {
        find = FUN_004bc4b0("*.", &fd, -1, 1);
        if (find != -1) {
            do {
                if (fd.name[0] != '.' && fd.attrib == 0x10) {
                    strncpy(list, "\\", 1);
                    list++;
                    strcpy(list, fd.name);
                    list += strlen(fd.name);
                    *list++ = 0;
                    if (sizes) {
                        strcpy(sizes, "<DIR>");
                        sizes += 6;
                    }
                    num++;
                }
            } while (FUN_004bc640(find, &fd) != -1);
            FUN_004bc8d0(find);
        }
    }
    {
        find = FUN_004bc4b0(path, &fd, -1, 1);
        if (find != -1) {
            do {
                if (fd.name[0] != '.' && fd.attrib != 0x10) {
                    strcpy(list, fd.name);
                    if (flag == 1) {
                        FUN_004bb0f0(list);
                    }
                    list += strlen(list) + 1;
                    *tp++ = fd.time_write;
                    if (sizes) {
                        _itoa(FUN_004bbc40(fd.name), text, 10);
                        strcpy(sizes, text);
                        sizes += strlen(text) + 1;
                    }
                    num++;
                }
            } while (FUN_004bc640(find, &fd) != -1);
            FUN_004bc8d0(find);
        }
    }
    if (what) {
        if (what == 2) {
            FUN_004aefa0(first, 0, 0, num);
        } else {
            FUN_004aefa0(first, 0, times, num);
        }
    }
    FUN_004d85a0(times);
    *list = 0;
    return num;
}
